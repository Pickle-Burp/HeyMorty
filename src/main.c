#include <err.h>
#include "neural_network/nn.h"
#include "sprec/sprec.h"

#define API_KEY      "API_KEY"
#define LANG         "fr-FR"

int main(int argc, char **argv) {
  if (argc < 2)
    errx(EXIT_FAILURE, "No engough args. ./hey_morty <audio/nn>");
  if (strcmp(argv[1], "audio") == 0) {
    if (argc < 3)

      errx(EXIT_FAILURE, "No engough args. ./hey_morty audio myFile.wav");
    char *txt = sprec_recognize_wav(getenv(API_KEY), LANG,
                                    argv[2], 16000);
    printf("%s", txt);
    return 0;
  }
  if (strcmp(argv[1], "nn") == 0) {
    test_neural_network();
    return 0;
  }
  return 0;
}
