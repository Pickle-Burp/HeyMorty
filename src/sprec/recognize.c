#include <stdlib.h>
#include <string.h>
#include "recognize.h"
#include "../utils/file.h"
#include "../utils/string_builder.h"

const char *REQUEST = "{\n"
                      "  'config': {\n"
                      "    'sampleRateHertz': %ud,\n"
                      "    'languageCode': '%s',\n"
                      "  },\n"
                      "  'audio': {\n"
                      "    'content': '%s'\n"
                      "  }\n"
                      "}";

char *build_request(const char *lang,
                    const char *filename,
                    unsigned int sample_rate) {
  // Read base64 audio file
  unsigned long length;
  char *content = file_read_content(filename, &length);

  // Max length of a string
  unsigned long maxLength = length + strlen(REQUEST) * 2;
  StringBuilder *sb = sb_create();
  sb_appendf_l(sb, REQUEST, maxLength, sample_rate, lang, content);

  char *request = sb_concat(sb);

  sb_free(sb);
  free(content);

  return request;
}

char *sprec_recognize_base64(const char *apikey,
                             const char *lang,
                             const char *filename,
                             unsigned int sample_rate) {

  char *request = build_request(lang, filename, sample_rate);
  //TODO: Send request and get result
  return NULL;
}