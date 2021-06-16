/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#include <stdbool.h>
#include "data.h"
#include <string.h>

#define IMAGE_SIZE 150000

size_t trainSize;
size_t testSize;
Datum *trainData;
Datum *testData;

unsigned int convertToLE(unsigned int value) {
    return (((value & 0x000000FF) << 24) |
            ((value & 0x0000FF00) << 8) |
            ((value & 0x00FF0000) >> 8) |
            ((value & 0xFF000000) >> 24));
}

size_t loadImages(char* source, Tensor*** dest) {
    FILE* imageFile = fopen(source, "rb");

    unsigned int n;
    unsigned char image[IMAGE_SIZE];
    unsigned int tensorShape[] = {IMAGE_SIZE, 1};

    // Read image amount
    fseek(imageFile, 4, SEEK_SET);
    fread(&n, 4, 1, imageFile);
    n = convertToLE(n);
    *dest = calloc(n, sizeof(Tensor*));

    // Skip unneeded data
    fseek(imageFile, 8, SEEK_CUR);

    // Read in images
    for (int img = 0; img < 10; img++) {
        fread(image, IMAGE_SIZE, 1, imageFile);

        Tensor* imgTensor = newTensor(2, tensorShape);

        (*dest)[img] = imgTensor;
    }

    fclose(imageFile);

    return n;
}

char *expectedSentence(int x) {
    char sentence[144 + 1];

    // Open dictionary file
    FILE *training;
    training = fopen("src/cnn/DATASETS/les_mots_max_144_col.txt", "r");
    char **sentences = calloc(10, sizeof(char) * 145);
    // Check to see if it exists
    if (training == NULL)
        err(1, "Empty file");

    int i = 0;
    // Read in words from dictionary
    while (fscanf(training, "%s", sentence) != EOF) {
        sentences[i] = sentence;
    }
    fclose(training);
    return sentences[x];
}

size_t loadLabels(char* source, Tensor*** dest) {
    FILE* labelFile = fopen(source, "rb");

    unsigned int n;
    unsigned int tensorShape[] = {144*10,1};

    // Read label amount
    fseek(labelFile, 4, SEEK_SET);
    fread(&n, 4, 1, labelFile);
    n = convertToLE(n);
    *dest = calloc(n, sizeof(Tensor*));

    for (int l = 0; l < 10; l++) {
        unsigned char label;
        fread(&label, 1, 1, labelFile);

        Tensor* labelTensor = newTensor(2, tensorShape);
        unsigned int index[] = {0, 0};
        index[0] = label;
        *getElement(labelTensor, index) = 1;

        (*dest)[l] = labelTensor;
    }

    fclose(labelFile);

    return n;
}

void zipData(Tensor **X, Tensor **Y, Datum *dest, size_t size) {
    for (size_t i = 0; i < size; i++) {
        dest[i] = (Datum) {
                .x = X[i],
                .y = Y[i]
        };
    }
    free(X);
    free(Y);
}

Datum getTrainElement(size_t element) {
    return trainData[element];
}

Datum getTestElement(size_t element) {
    return testData[element];
}

Dataset FFT(char* trainImages, char *trainLabels, char* testImages, char *testLabels) {
    Tensor **trainX;
    Tensor **trainY;
    Tensor **testX;
    Tensor **testY;

    trainSize = loadImages(trainImages, &trainX);
    testSize = loadImages(testImages, &testX);
    loadLabels(trainLabels, &trainY);
    loadLabels(testLabels, &testY);

    trainData = calloc(trainSize, sizeof(Datum));
    testData = calloc(testSize, sizeof(Datum));

    zipData(trainX, trainY, trainData, trainSize);
    zipData(testX, testY, testData, testSize);

    return (Dataset) {
            .trainElements = trainSize,
            .getTrainElement = getTrainElement,
            .testElements = testSize,
            .getTestElement = getTestElement,
    };
}

bool datumValid(Datum datum) {
    return datum.x->rank == 2 && datum.x->shape[0] == IMAGE_SIZE && datum.x->shape[1] == 1
           && datum.y->rank == 2 && datum.y->shape[0] == 10 && datum.y->shape[1] == 1;
}

void printDatum(Datum datum) {
    if (datumValid(datum)) {
        printf("O~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~O\n");
        for (int i = 0; i < 10; i++) {
            printf("%s\n", expectedSentence(i));
        }


        int label = -1;
        for (int i = 0; i < 10; i++) {
            if (datum.y->data[i] == 1) {
                label = i;
                break;
            }
        }

        printf("O~~~~~~~~~ Number: %d ~~~~~~~~~~O\n", label);
    } else {
        printf("[printDatum]: Invalid datum\n");
        exit(100);
    }
}