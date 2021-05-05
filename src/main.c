#include <err.h>
#include "cmd/execution.h"
#include "neural_network/nn.h"
#include "sprec/sprec.h"
#include "ui/ui.h"

#define API_KEY      "API_KEY"
#define LANG         "fr-FR"

int main(int argc, char **argv) {
  if (argc < 2)
    errx(EXIT_FAILURE, "No enough args. ./hey_morty <audio/nn/cmd/ui>");
  if (strcmp(argv[1], "audio") == 0) {
    if (argc < 3)
      errx(EXIT_FAILURE, "No enough args. ./hey_morty audio myFile.wav");
    char *txt = sprec_recognize_wav(getenv(API_KEY), LANG,
                                    argv[2], 16000);
    printf("%s", txt);
    return 0;
  }
  if (strcmp(argv[1], "nn") == 0) {
    test_neural_network();
    return 0;
  }
  if (strcmp(argv[1], "cmd") == 0) {
    char text[argc - 2];
    for (int i = 2; i < argc; i++)
      text[i - 1] = *argv[i];
    char **cutting_command = cut_text(text, argc - 2);
    const char *command = convert_to_command(**cutting_command);
    command_exec(command);
  }
  if(strcmp(argv[1], "ui") == 0){
    ui_start(0, NULL);
  }
  return 0;
}
