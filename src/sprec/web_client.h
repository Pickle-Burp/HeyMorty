//
// Created by vinetos on 3/20/21.
//

#ifndef HEYMORTY_SRC_SPREC_WEB_CLIENT_H_
#define HEYMORTY_SRC_SPREC_WEB_CLIENT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

typedef struct sprec_server_response {
  char *data;
  size_t length;
} sprec_server_response;

/*
 * Sends the FLAC-encoded audio data.
 * Returns a struct server_response pointer,
 * in which the API's JSON response is present.
 * Should be freed with sprec_free_response().
 * Returns NULL on error.
 */
sprec_server_response *
sprec_send_audio_data(
    const void *data,
    size_t length,
    const char *apikey,
    const char *language,
    uint32_t sample_rate
);

void sprec_free_response(sprec_server_response *resp);

#endif //HEYMORTY_SRC_SPREC_WEB_CLIENT_H_
