//
// Created by Vinetos on 06/04/2021.
//

#ifndef HEYMORTY_SRC_SPREC_RECOGNIZE_H_
#define HEYMORTY_SRC_SPREC_RECOGNIZE_H_

/*
 * Performs a text recognition on the content of filename
 * then returns the recognized text.
 *
 * Returns NULL on error.
 */
char *sprec_recognize_base64(const char *apikey, const char *lang,
                             const char *filename, unsigned int sample_rate);

char *sprec_recognize_wav(const char *apikey, const char *lang,
                          const char *filename, unsigned int sample_rate);

#endif //HEYMORTY_SRC_SPREC_RECOGNIZE_H_
