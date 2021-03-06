#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void *file_read_content(const char *filename, unsigned long *length) {
  FILE *file = fopen(filename, "rb");
  void *content;
  if (file == NULL)
    errx(EXIT_FAILURE, "Failed to open audio utils %s", filename);

  // Get File length
  fseek(file, 0, SEEK_END);
  *length = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Read content
  content = malloc(*length * sizeof(void));
  if (content == NULL)
    errx(EXIT_FAILURE, "Failed to store content");
  fread(content, 1, *length, file);
  fclose(file);
  return content;
}