#include "wav2.h"
 
static ring_buffer_size_t rbs_min(ring_buffer_size_t a, ring_buffer_size_t b)
{
    return (a < b) ? a : b;
}

/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define FILE_NAME       "audio_data.raw"
#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (3)
#define NUM_CHANNELS    (1) //origin 2
#define NUM_WRITES_PER_BUFFER   (4)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0) 


/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif


/**
 * 
 * create a copy of audio_data.raw with the wav header
 * new file named "audio_data.wav"
 * @author matthieu
 * 
 */
void raw_to_wav()
{
    char *file = "audio_data.wav";
    FILE *fp = fopen(file,"w");
    fwrite("RIFF",sizeof(char),4,fp);
    int file_size = SAMPLE_RATE * NUM_SECONDS + 36;
    fwrite(&file_size,sizeof(int),1,fp);
    fwrite("WAVE",sizeof(char),4,fp);
    fwrite("fmt ",sizeof(char),4,fp);
    int format = 16;
    fwrite(&format,sizeof(int),1,fp);
    short type = 1;
    fwrite(&type,sizeof(short),1,fp);
    short channels = 1;
    fwrite(&channels,sizeof(short),1,fp);
    int sample = 44100;
    fwrite(&sample,sizeof(int),1,fp);
    short bitpersample = 16;
    int truc = sample*(int)bitpersample*channels/8;
    fwrite(&truc,sizeof(int),1,fp);
    short mono = (short)((int)bitpersample*channels/8);
    fwrite(&mono,sizeof(short),1,fp);
    fwrite(&bitpersample,sizeof(short),1,fp);
    fwrite("data",sizeof(char),4,fp);
    file_size = file_size - 36;
    fwrite(&file_size,sizeof(int),4,fp);

    FILE *fp2 = fopen(FILE_NAME,"r");
    int e = 1;
    char buffer[100];
    while (e)
    {
        e = fread(buffer,sizeof(char),100,fp2);
        fwrite(buffer,sizeof(char),e,fp);
    }
    fclose(fp2);
    fclose(fp);
}


/* This routine is run in a separate thread to write data from the ring buffer into a file (during Recording) */
static int threadFunctionWriteToRawFile(void* ptr)
{
    paTestData* pData = (paTestData*)ptr;

    /* Mark thread started */
    pData->threadSyncFlag = 0;

    while (1)
    {
        ring_buffer_size_t elementsInBuffer = PaUtil_GetRingBufferReadAvailable(&pData->ringBuffer);
        if ( (elementsInBuffer >= pData->ringBuffer.bufferSize / NUM_WRITES_PER_BUFFER) ||
             pData->threadSyncFlag )
        {
            void* ptr[2] = {0};
            ring_buffer_size_t sizes[2] = {0};

            /* By using PaUtil_GetRingBufferReadRegions, we can read directly from the ring buffer */
            ring_buffer_size_t elementsRead = PaUtil_GetRingBufferReadRegions(&pData->ringBuffer, elementsInBuffer, ptr + 0, sizes + 0, ptr + 1, sizes + 1);
            if (elementsRead > 0)
            {
                int i;
                for (i = 0; i < 2 && ptr[i] != NULL; ++i)
                {
                    fwrite(ptr[i], pData->ringBuffer.elementSizeBytes, sizes[i], pData->file);
                }
                PaUtil_AdvanceRingBufferReadIndex(&pData->ringBuffer, elementsRead);
            }

            if (pData->threadSyncFlag)
            {
                break;
            }
        }

        /* Sleep a little while... */
        Pa_Sleep(20);
    }

    pData->threadSyncFlag = 0;

    return 0;
}

typedef int (*ThreadFunctionType)(void*);

/**
 * 
 * create a thread to be used whit the fn function
 * the windows part was done by PA, linux side by myself
 * @authors PA,matthieu
 * @param pData pointer to the portaudio data struct
 * @param fn fonction that will be used as the start routine
 * for the thread
 * 
 */
static PaError startThread( paTestData* pData, ThreadFunctionType fn )
{
#ifdef _WIN32
    typedef unsigned (__stdcall* WinThreadFunctionType)(void*);
    pData->threadHandle = (void*)_beginthreadex(NULL, 0, (WinThreadFunctionType)fn, pData, CREATE_SUSPENDED, NULL);
    if (pData->threadHandle == NULL) return paUnanticipatedHostError;

    /* Set file thread to a little higher prio than normal */
    SetThreadPriority(pData->threadHandle, THREAD_PRIORITY_ABOVE_NORMAL);

    /* Start it up */
    pData->threadSyncFlag = 1;
    ResumeThread(pData->threadHandle);
#elif __linux__
    int e;
    e = pthread_create(pData->threadHandle,NULL,fn,(void)pData);
    if(e!=0)
    {
        errx(EXIT_FAILURE,"error while creating pthread.");
    }
    pData->threadSyncFlag = 1;
#endif

    /* Wait for thread to startup */
    while (pData->threadSyncFlag) {
        Pa_Sleep(10);
    }

    return paNoError;
}

/**
 * 
 * wait for the end of a given thread
 * the windows part was done by PA, linux side by myself
 * @authors PA,matthieu
 * @param pData pointer to the portaudio data struct
 * 
 */
static int stopThread( paTestData* pData )
{
    pData->threadSyncFlag = 1;
    /* Wait for thread to stop */
    while (pData->threadSyncFlag) {
        Pa_Sleep(10);
    }
#ifdef _WIN32
    CloseHandle(pData->threadHandle);
    pData->threadHandle = 0;
#elif __linux__
    pthread_join(pData->threadHandle);
    pData->threadHandle = 0;
#endif

    return paNoError;
}


/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    paTestData *data = (paTestData*)userData;
    ring_buffer_size_t elementsWriteable = PaUtil_GetRingBufferWriteAvailable(&data->ringBuffer);
    ring_buffer_size_t elementsToWrite = rbs_min(elementsWriteable, (ring_buffer_size_t)(framesPerBuffer * NUM_CHANNELS));
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;

    (void) outputBuffer; /* Prevent unused variable warnings. */
    (void) timeInfo;
    (void) statusFlags;
    (void) userData;

    data->frameIndex += PaUtil_WriteRingBuffer(&data->ringBuffer, rptr, elementsToWrite);

    return paContinue;
}

static unsigned NextPowerOf2(unsigned val)
{
    val--;
    val = (val >> 1) | val;
    val = (val >> 2) | val;
    val = (val >> 4) | val;
    val = (val >> 8) | val;
    val = (val >> 16) | val;
    return ++val;
}

/*******************************************************************/


/**
 * 
 * launch the recording for a given amount of seconds
 * file will be saved as FILE_NAME 
 * @authors PA,matthieu
 * 
 */
int record()
{
    PaStreamParameters  inputParameters,
                        outputParameters;
    PaStream*           stream;
    PaError             err = paNoError;
    paTestData          data = malloc(sizeof(struct paTestData));
    data.frameIndex = 0;
    unsigned            delayCntr;
    unsigned            numSamples;
    unsigned            numBytes;

    printf("patest_record.c\n"); fflush(stdout);

    /* We set the ring buffer size to about 500 ms */
    numSamples = NextPowerOf2((unsigned)(SAMPLE_RATE * 0.5 * NUM_CHANNELS));
    numBytes = numSamples * sizeof(SAMPLE);
    data.ringBufferData = (SAMPLE *) PaUtil_AllocateMemory( numBytes );
    if( data.ringBufferData == NULL )
    {
        printf("Could not allocate ring buffer data.\n");
        goto done;
    }

    if (PaUtil_InitializeRingBuffer(&data.ringBuffer, sizeof(SAMPLE), numSamples, data.ringBufferData) < 0)
    {
        printf("Failed to initialize ring buffer. Size is not power of 2 ??\n");
        goto done;
    }

    err = Pa_Initialize();
    if( err != paNoError ) goto done;

    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        goto done;
    }
    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(
              &stream,
              &inputParameters,
              NULL,                  
              SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,     
              recordCallback,
              &data );
    if( err != paNoError ) goto done;

    /* Open the raw audio 'cache' file... */
    data.file = fopen(FILE_NAME, "wb");
    if (data.file == 0) goto done;

    /* Start the file writing thread */
    err = startThread(&data, threadFunctionWriteToRawFile);
    if( err != paNoError ) goto done;

    err = Pa_StartStream( stream );
    if( err != paNoError ) goto done;
    printf("\n=== Now recording to '" FILE_NAME "' for %d seconds!! Please speak into the microphone. ===\n", NUM_SECONDS); fflush(stdout);

    /* the RECORDING is limited with TIME, not size of the file and/or buffer, therefore
       increasing NUM_SECONDS increases the recording capacity till disk space is limited */
    delayCntr = 0;
    while( delayCntr++ < NUM_SECONDS )
    {
        printf("index = %d\n", data.frameIndex ); fflush(stdout);
        Pa_Sleep(1000);
    }
    if( err < 0 ) goto done;

    err = Pa_CloseStream( stream );
    if( err != paNoError ) goto done;

    /* Stop the thread */
    err = stopThread(&data);
    if( err != paNoError ) goto done;

    /* Close file */
    fclose(data.file);
    data.file = 0;

    done:
    Pa_Terminate();
    if( data.ringBufferData )       /* Sure it is NULL or valid. */
        PaUtil_FreeMemory( data.ringBufferData );
    if( err != paNoError )
    {
        fprintf( stderr, "An error occured while using the portaudio stream\n" );
        fprintf( stderr, "Error number: %d\n", err );
        fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        err = 1;          /* Always return 0 or 1, but no other return codes. */
    }
    free(data);
    return err;
}
