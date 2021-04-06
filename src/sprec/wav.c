#include <unistd.h>
#include "wav.h"

#include <alsa/asoundlib.h>

sprec_wav_header *sprec_wav_header_from_data(const FLAC__byte *ptr) {
  sprec_wav_header *hdr;
  hdr = malloc(sizeof *hdr);
  if (hdr == NULL) {
    return NULL;
  }

  /*
   * We could use __attribute__((__packed__)) and a single memcpy(),
   * but we choose this approach for the sake of portability.
   */
  memcpy(&hdr->RIFF_marker, ptr + 0, 4);
  memcpy(&hdr->file_size, ptr + 4, 4);
  memcpy(&hdr->filetype_header, ptr + 8, 4);
  memcpy(&hdr->format_marker, ptr + 12, 4);
  memcpy(&hdr->data_header_length, ptr + 16, 4);
  memcpy(&hdr->format_type, ptr + 20, 2);
  memcpy(&hdr->number_of_channels, ptr + 22, 2);
  memcpy(&hdr->sample_rate, ptr + 24, 4);
  memcpy(&hdr->bytes_per_second, ptr + 28, 4);
  memcpy(&hdr->bytes_per_frame, ptr + 32, 2);
  memcpy(&hdr->bits_per_sample, ptr + 34, 2);

  return hdr;
}

sprec_wav_header *sprec_wav_header_from_params(
    uint32_t sample_rate,
    uint16_t bit_depth,
    uint16_t channels
) {
  sprec_wav_header *hdr;
  hdr = malloc(sizeof *hdr);
  if (hdr == NULL) {
    return NULL;
  }

  memcpy(&hdr->RIFF_marker, "RIFF", 4);
  memcpy(&hdr->filetype_header, "WAVE", 4);
  memcpy(&hdr->format_marker, "fmt ", 4);
  hdr->data_header_length = 16;
  hdr->format_type = 1;
  hdr->number_of_channels = channels;
  hdr->sample_rate = sample_rate;
  hdr->bytes_per_second = sample_rate * channels * bit_depth / 8;
  hdr->bytes_per_frame = channels * bit_depth / 8;
  hdr->bits_per_sample = bit_depth;

  return hdr;
}

int sprec_wav_header_write(FILE *f, sprec_wav_header *hdr) {
  if (hdr == NULL) {
    return -1;
  }

#define CHECK(call) do { if ((call) != 1) { return -1; } } while (0)

  CHECK(fwrite(&hdr->RIFF_marker, 4, 1, f));
  CHECK(fwrite(&hdr->file_size, 4, 1, f));
  CHECK(fwrite(&hdr->filetype_header, 4, 1, f));
  CHECK(fwrite(&hdr->format_marker, 4, 1, f));
  CHECK(fwrite(&hdr->data_header_length, 4, 1, f));
  CHECK(fwrite(&hdr->format_type, 2, 1, f));
  CHECK(fwrite(&hdr->number_of_channels, 2, 1, f));
  CHECK(fwrite(&hdr->sample_rate, 4, 1, f));
  CHECK(fwrite(&hdr->bytes_per_second, 4, 1, f));
  CHECK(fwrite(&hdr->bytes_per_frame, 2, 1, f));
  CHECK(fwrite(&hdr->bits_per_sample, 2, 1, f));
  CHECK(fwrite("data", 4, 1, f));

  uint32_t data_size = hdr->file_size + 8 - 44;
  CHECK(fwrite(&data_size, 4, 1, f));

#undef CHECK

  return 0;
}

int sprec_record_wav(const char *filename,
                     sprec_wav_header *hdr,
                     uint32_t duration_ms) {
  /*
   * Linux, Solaris, etc.
   * Let's hope they have an available ALSA port
   */
  int i;
  int size;
  snd_pcm_t *handle;
  snd_pcm_hw_params_t *params;
  unsigned int val;
  int dir;
  snd_pcm_uframes_t frames;
  char *buffer;
  FILE *f;
  int err;

  /*
   * Open PCM device for recording
   */
  err = snd_pcm_open(&handle, "pulse", SND_PCM_STREAM_CAPTURE, 0);
  if (err) {
    return err;
  }

  /*
   * Allocate a hardware parameters object.
   */
  snd_pcm_hw_params_alloca(&params);

  /*
   * Fill it in with default values, then set
   * the non-default ones manually
   */
  snd_pcm_hw_params_any(handle, params);

  err = snd_pcm_hw_params_set_access(handle,
                                     params,
                                     SND_PCM_ACCESS_RW_INTERLEAVED);
  if (err) {
    snd_pcm_close(handle);
    return err;
  }

  if (hdr->bits_per_sample == 16) {
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
  } else {
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_U8);
  }

  err = snd_pcm_hw_params_set_channels(handle, params, hdr->number_of_channels);
  if (err) {
    snd_pcm_close(handle);
    return err;
  }

  val = hdr->sample_rate;
  err = snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir);
  if (err) {
    snd_pcm_close(handle);
    return err;
  }

  hdr->sample_rate = val;

  /*
   * Interrupt period size equals 32 frames
   */
  frames = 32;
  err = snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir);
  if (err) {
    snd_pcm_close(handle);
    return err;
  }

  /*
   * Write the parameters to the driver
   */
  err = snd_pcm_hw_params(handle, params);
  if (err) {
    snd_pcm_close(handle);
    return err;
  }

  err = snd_pcm_hw_params_get_period_size(params, &frames, &dir);
  if (err) {
    snd_pcm_close(handle);
    return err;
  }

  /*
   * multiply by number of bytes/sample
   * and number of channels
   */
  size = frames * hdr->bits_per_sample / 8 * hdr->number_of_channels;
  buffer = malloc(size);
  if (buffer == NULL) {
    snd_pcm_close(handle);
    return -1;
  }

  /*
   * Calculate number of loops and the size of
   * the raw PCM data
   */
  err = snd_pcm_hw_params_get_period_time(params, &val, &dir);
  if (err) {
    snd_pcm_close(handle);
    free(buffer);
    return err;
  }

  uint32_t pcm_data_size =
      hdr->sample_rate * hdr->bytes_per_frame * duration_ms / 1000;
  hdr->file_size = pcm_data_size + 44 - 8;

  /*
   * Open the WAV file for output;
   * write out the WAV header
   */
  f = fopen(filename, "wb");
  if (f == NULL) {
    snd_pcm_close(handle);
    free(buffer);
    return -1;
  }

  err = sprec_wav_header_write(f, hdr);
  if (err) {
    snd_pcm_close(handle);
    free(buffer);
    fclose(f);
    return err;
  }

  for (i = duration_ms * 1000 / val; i > 0; i--) {
    err = snd_pcm_readi(handle, buffer, frames);
    if (err == -EPIPE) {
      /*
       * minus EPIPE means X-run
       */
      err = snd_pcm_recover(handle, err, 0);
    }

    /* still not good */
    if (err) {
      snd_pcm_close(handle);
      free(buffer);
      fclose(f);
      return err;
    }

    fwrite(buffer, size, 1, f);
  }

  /*
   * Clean up
   */
  fclose(f);
  snd_pcm_drain(handle);
  snd_pcm_close(handle);
  free(buffer);

  return 0;
}