#include <curl/curl.h>
#include <err.h>
#include <string.h>
#include "web_client.h"
#include "../utils/string_builder.h"

//#define URL "https://www.google.com/speech-api/v2/recognize?output=json&key=%s&lang=%s"
#define URL "http://vps.vinetos.fr:3000/audio"
//#define CONTENT_TYPE "Content-Type: audio/l16; rate=%lu;"

static size_t http_callback(char *ptr, size_t count, size_t blocksize, void
*userdata);

sprec_server_response *
sprec_send_audio_data(
    const char *apikey,
    const void *data,
    size_t length,
    const char *lang,
    unsigned long sampleRate __attribute__((unused))
) {
  CURL *curl;
  sprec_server_response *resp;
  struct curl_httppost *form = NULL;

  resp = malloc(sizeof *resp);
  if (resp == NULL) {
    return NULL;
  }

  resp->data = NULL;
  resp->length = 0;

  curl = curl_easy_init();
  if (!curl)
    errx(EXIT_FAILURE, "Failed to initialize curl");

  struct curl_slist *headers = NULL;
  curl_slist_append(headers, "Expect:");
  headers = curl_slist_append(headers, "sb_format(CONTENT_TYPE, sampleRate)");
  curl_easy_setopt(curl, CURLOPT_URL, sb_format(URL, apikey, lang));
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  // SSL certificates are not available, so we have to trust Google
  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, resp);

  /*curl_formadd(
      &form,
      &lastptr,
      CURLFORM_COPYNAME,
      "myfile",
      CURLFORM_CONTENTSLENGTH,
      (long) length,
      CURLFORM_PTRCONTENTS,
      data,
      CURLFORM_END
  );

  curl_easy_setopt(curl, CURLOPT_HTTPPOST, form);*/
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE_LARGE, length);
  /* binary data */
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);

  curl_easy_perform(curl);

  resp->data[resp->length] = '\0';

  curl_formfree(form);
  curl_slist_free_all(headers);
  curl_easy_cleanup(curl);
  return resp;
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
