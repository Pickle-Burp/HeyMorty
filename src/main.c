#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "sprec/sprec.h"

#define API_KEY "AIzaSyA4e_DmX-kxzXvGiUtrkrRSTxM4Pa5zIEA"
#define LANG    "fr-FR"
#define FILE_NAME    "src/assets/encoded"

int copy_file(char *source_file, char *target_file) {
  FILE *source, *target;

  source = fopen(source_file, "r");
  if (source == NULL)
    return EXIT_FAILURE;

  target = fopen(target_file, "w");
  if (target == NULL) {
    fclose(source);
    return EXIT_FAILURE;
  }

  char ch;
  while ((ch = fgetc(source)) != EOF)
    fputc(ch, target);

  fclose(source);
  fclose(target);

  return 0;
}

int main() {
  /*if (copy_file(FILE_NAME, FILE_COPY) != 0)
    errx(EXIT_FAILURE, "Failed to copy file");

  printf("File copied successfully\n");*/

  sprec_wav_header *hdr = sprec_wav_header_from_params(16000, 16, 2);
  char *txt = sprec_recognize_base64(API_KEY, LANG,
                                   FILE_NAME, hdr);
  printf("%s", txt);
  return 0;
}
