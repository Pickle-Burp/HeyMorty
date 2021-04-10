#include <curl/curl.h>
#include "web_client.h"
#include "../utils/string_builder.h"
#define URL "https://speech.googleapis.com/v1/speech:recognize"

static size_t http_callback(char *ptr, size_t count, size_t blocksize, void
*userdata);

sprec_server_response *
sprec_send_audio_data(
    const void *data,
    size_t length,
    const char *apikey,
    const char *language,
    uint32_t sample_rate
) {
  CURL *conn_hndl;



}

void sprec_free_response(sprec_server_response *resp) {
  if (resp) {
    free(resp->data);
    free(resp);
  }
}

static size_t http_callback(char *ptr,
                            size_t count,
                            size_t blocksize,
                            void *userdata) {
  sprec_server_response *response = userdata;
  size_t size = count * blocksize;

  // +1 for terminating NUL byte
  response->data = realloc(response->data, response->length + size + 1);
  memcpy(response->data + response->length, ptr, size);
  response->length += size;

  return size;
}
