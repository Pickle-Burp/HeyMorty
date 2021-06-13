#include <err.h>
#include "cmd/execution.h"
#include "neural_network/nn.h"
#include "sprec/sprec.h"

#define API_KEY      "API_KEY"
#define LANG         "fr-FR"

int main(int argc, char **argv) {
  if (argc < 2)
    errx(EXIT_FAILURE, "No enough args. ./hey_morty <audio/nn/cmd>");
  if (strcmp(argv[1], "audio") == 0) {
    if (argc < 3)

      errx(EXIT_FAILURE, "No enough args. ./hey_morty audio myFile.wav");
    char *txt = sprec_recognize_wav(getenv(API_KEY), LANG,
                                    argv[2], 16000);
    printf("%s", txt);
    return 0;
  }
  if (strcmp(argv[1], "cmd") == 0) {
    if(argc < 3)
      errx(EXIT_FAILURE, "No enough args. ./hey_morty cmd text");
    
    char **text = argv+2;
    int n = 0;
    while (text[n] != NULL) {
      n++;
    }
    const char *command = convert_to_command(text, n);
    printf("%s\n", command);
    command_exec(command);
    return 0;
  }
  if (strcmp(argv[1], "nn") == 0) {
    test_neural_network();
    return 0;
  }
}
