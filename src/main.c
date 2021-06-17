#include <err.h>
#include <json-c/json.h>
#include <glib.h>
#include "cmd/execution.h"
#include "cmd/spell_check.h"
#include "sprec/sprec.h"
#include "ui/ui.h"
#include "cnn/training.h"

#define API_KEY      "API_KEY"
#define LANG         "fr-FR"

void testNeural() {
  srand(time(NULL));

  struct dirent *dtrain;  // Pointer for directory entry
  DIR *drtrain = opendir("src/cnn/DATASETS/fft");

  struct dirent *dtest;
  DIR *drtest = opendir("src/cnn/DATASETS/test");
  if (drtest == NULL
      || drtrain == NULL)  // opendir returns NULL if couldn't open directory
  {
    err(1, "Error on opendir");
  }
  while ((dtrain = readdir(drtrain)) != NULL
      || ((dtest = readdir(drtest)) != NULL)) {

    Dataset fft = FFT(dtrain->d_name,
                      "src/cnn/DATASETS/les_mots_max_144_col.txt",
                      dtest->d_name,
                      "src/cnn/DATASETS/les_mots_max_144_col.txt");

    // Initialize neural network
    unsigned int nnShape[] = {150000, 16, 16, 10};
    NeuralNet *nn = newNeuralNet(4, nnShape, MeanSquaredError);
    randInit(nn);
    nn->train = true;

    train(nn, SGD, fft, 10, 1, 0.1, printEpochCallback);

    for (size_t i = 0; i < 10; i++) {
      printf("\n");
      Datum datum = fft.getTestElement(i);
      printDatum(datum);

      copyTensor(datum.x, nn->input);
      forwardPass(nn);

      printf("Network prediction: %zu\n", argmax(nn->output));
      printf("Confidence: %f%%\n", 100 * nn->output->data[argmax(nn->output)]);
      printf("Raw output: ");
      printTensor(nn->output);
    }

    printf("\nSaving neural network...\n");
    saveNeuralNet(nn, "mnist_demo_save.dat");
    printf("Neural network saved\n");
    freeNeuralNet(nn);
  }
  closedir(drtrain);
  closedir(drtest);
}

int main(int argc, char **argv) {
  if (argc < 2)
    errx(EXIT_FAILURE, "No enough args. ./hey_morty <audio/nn/cmd/ui/wk>");
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
    const char *str = json_object_get_string(trans);
    printf("%s\n", json_object_to_json_string(alt));
    printf("%f : %s", confidence, str);
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
    testNeural();
    return 0;
  }
  if (strcmp(argv[1], "ui") == 0) {
    ui_start(0, NULL);
  }
  if (strcmp(argv[1], "wk") == 0) {
    if (argc < 3)
      errx(EXIT_FAILURE, "No enough args. ./hey_morty wk myFile.wav");

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
    const char *str = json_object_get_string(trans);
    printf("%f : %s", confidence, str);
    json_object_put(parsed_json);
    const char **text = (const char **) g_strsplit(str, " ", -1);
    int n = 0;
    while (text[n] != NULL) {
      n++;
    }
    char **checked_text = spell_check((char **) text, n);
    const char *command = convert_to_command(checked_text, n);
    command_exec(command);
    return 0;
  }
}
