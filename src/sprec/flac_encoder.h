//
// Created by Vinetos on 06/04/2021.
//

#ifndef HEYMORTY_SRC_SPREC_FLAC_ENCODER_H_
#define HEYMORTY_SRC_SPREC_FLAC_ENCODER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
 * Converts a WAV PCM file at the path `wavfile'
 * to a FLAC file with the same sample rate,
 * channel number and bit depth. Writes the result
 * to the file at path `flacfile'.
 * Returns a pointer to FLAC data buffer on success,
 * NULL on error. On success, *size will be set to
 * the size of the FLAC data (in bytes).
 */
void *sprec_flac_encode(const char *wavfile, size_t *size);

#endif //HEYMORTY_SRC_SPREC_FLAC_ENCODER_H_
