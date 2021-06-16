#include <err.h>
#include <json-c/json.h>
#include <glib.h>
#include "cmd/execution.h"
#include "cmd/spell_check.h"
#include "neural_network/nn.h"
#include "sprec/sprec.h"
#include "ui/ui.h"
#include "utils/string_builder.h"

#define API_KEY      "API_KEY"
#define LANG         "fr-FR"

int main(int argc, char **argv) {
  if (argc < 2)
    errx(EXIT_FAILURE, "No enough args. ./hey_morty <audio/nn/cmd/ui>");
  if (strcmp(argv[1], "audio") == 0) {
    if (argc < 3)
      errx(EXIT_FAILURE, "No enough args. ./hey_morty audio myFile.wav");

    char *json = sprec_recognize_wav(getenv(API_KEY), LANG,
                                     argv[2], 16000);
    // extract result
    struct json_object *parsed_json = json_tokener_parse(json);
    struct json_object *results = json_object_object_get(parsed_json,
                                                         "results");
    struct json_object *res = json_object_array_get_idx(results, 0);
    struct json_object
        *alternatives = json_object_object_get(res, "alternatives");
    struct json_object *alt = json_object_array_get_idx(alternatives, 0);
    struct json_object *conf = json_object_object_get(alt, "confidence");
    struct json_object *trans = json_object_object_get(alt, "transcript");
    double confidence = json_object_get_double(conf);
    char *str = json_object_get_string(trans);
    printf("%s\n", json_object_to_json_string(alt));
    printf("%f : %s", str, confidence);
    json_object_put(parsed_json);

    return 0;
  }
  if (strcmp(argv[1], "cmd") == 0) {
    if (argc < 3)
      errx(EXIT_FAILURE, "No enough args. ./hey_morty cmd text");
    char **text = argv + 2; // TODO : change argv+2 to neural-network exit
    int n = 0;
    while (text[n] != NULL) {
      n++;
    }
    char **checked_text = spell_check(text, n);
    const char *command = convert_to_command(checked_text, n);
    command_exec(command);
    return 0;
  }
  if (strcmp(argv[1], "nn") == 0) {
    test_neural_network();
    return 0;
  }
  if (strcmp(argv[1], "ui") == 0) {
    ui_start(0, NULL);
  }
}
