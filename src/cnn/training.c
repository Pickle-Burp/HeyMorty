/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#include <stdio.h>
#include <time.h>
#include "training.h"
#include <string.h>
#include <dirent.h>

int main() {
    srand(time(NULL));

    struct dirent *dtrain;  // Pointer for directory entry
    DIR *drtrain = opendir("src/cnn/DATASETS/fft");

    struct dirent *dtest;
    DIR *drtest = opendir("src/cnn/DATASETS/test");
    if (drtest == NULL || drtrain == NULL)  // opendir returns NULL if couldn't open directory
    {
        err(1, "Error on opendir");
    }
    while ((dtrain = readdir(drtrain)) != NULL || ((dtest = readdir(drtest)) != NULL)) {

    Dataset fft = FFT(dtrain->d_name,
                          "src/cnn/DATASETS/les_mots_max_144_col.txt",
                          dtest->d_name,
                          "src/cnn/DATASETS/les_mots_max_144_col.txt");

    // Initialize neural network
    unsigned int nnShape[] = {150000, 16, 16, 10};
    NeuralNet* nn = newNeuralNet(4, nnShape, MeanSquaredError);
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
        printf("Confidence: %f%%\n", 100*nn->output->data[argmax(nn->output)]);
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

