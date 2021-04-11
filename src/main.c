#include "sprec/sprec.h"

#define API_KEY      "API_KEY"
#define LANG         "fr-FR"
#define FILE_NAME    "src/assets/untitled.wav"

int main() {
  char *txt = sprec_recognize_wav(getenv(API_KEY), LANG,
                                  FILE_NAME, 16000);
  printf("%s", txt);
  return 0;
}
