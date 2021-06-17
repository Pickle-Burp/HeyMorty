#ifndef WAV2_H
#define WAV2_H


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "portaudio.h"
#include "pa_ringbuffer.h"
#include "pa_util.h"

typedef struct
{
    unsigned            frameIndex;
    int                 threadSyncFlag;
    SAMPLE             *ringBufferData;
    PaUtilRingBuffer    ringBuffer;
     FILE               *file;
     void               *threadHandle;
}
paTestData;

int record();
void raw_to_wav();

#endif
