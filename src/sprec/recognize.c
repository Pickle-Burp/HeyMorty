#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "recognize.h"
#include "../utils/file.h"

char *sprec_recognize_base64(const char *apikey,
                             const char *lang,
                             const char *filename,
                             uint32_t sample_rate) {

  unsigned long length;
  char *content = file_read_content(filename, &length);

  return NULL;
}