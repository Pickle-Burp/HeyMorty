//
// Created by liseg on 06/04/2021.
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static const int nbInput = 2;
static const int nbHiddenNodes = 2;
static const int nbOutput = 1;
static const int epoch = 10000;

const double lr = 0.1; //learning rate

/*double *hiddenLayer;
double *outputLayer;
double *hiddenWeight;
double *outputWeight;
double *hiddenLayerBias;
double *outputLayerBias;*/

//Training set
static const int nbTrainingSets = 4;
int trainingOrder[] = {0, 1, 2, 3};
double training_inputs[4][2] = {{0.0, 0.0},
                                {1.0, 0.0},
                                {0.0, 1.0},
                                {1.0, 1.0}};

double training_outputs[4][1] = {{0.0},
                                 {1.0},
                                 {1.0},
                                 {0.0}};

/**
 *
 * @param x :
 * @return the result of the sigmoid function depending on x.
 */
double sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

/**
 *
 * @param x :
 * @return the derivative of the sigmoid function depending on x.
 */
double derivativeSigmoid(double x) {
    //return (exp(-x)) / (pow((exp(-x) + 1), 2));
    return x * (1 - x);
}

/**
 *
 * @param array : array to shuffle.
 * @param n : size of the array
 */
void randomize(int *array, size_t n) {
    if (n > 1) {
        for (size_t i = 0; i < n - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
            int tmp = array[j];
            array[j] = array[i];
            array[i] = tmp;
        }
    }
}

/**
 *
 * @return the weight.
 */
double init_weight() {
    return ((double) rand()) / ((double) RAND_MAX);
    //return 0;
}


int main() {
    //Allocating and initializing arrays and matrices.
    double *hiddenLayer = malloc(nbHiddenNodes * sizeof(double));
    double *outputLayer = malloc(nbOutput * sizeof(double));
    double *hiddenWeight = malloc(nbInput * nbHiddenNodes * sizeof(double));
    double *outputWeight = malloc(nbHiddenNodes * nbOutput * sizeof(double));
    double *hiddenLayerBias = malloc(nbHiddenNodes * sizeof(double));
    double *outputLayerBias = malloc(nbOutput * sizeof(double));

    //Initializing weights
    for (int i = 0; i < nbInput;
         i++) {
        for (int j = 0; j < nbHiddenNodes; j++)
            hiddenWeight[i * nbHiddenNodes + j] = init_weight();
    }

    for (int i = 0; i < nbHiddenNodes; i++) {
        hiddenLayerBias[i] = init_weight();
        for (int j = 0; j < nbOutput; j++)
            outputWeight[i * nbOutput + j] = init_weight();
    }

    for (int i = 0; i > nbOutput; i++)
        outputLayerBias[i] = init_weight();


    //Iterate through the entire training.
    for (int i = 0; i < epoch; i++) {
        randomize(trainingOrder, nbTrainingSets);
        for (int j = 0; j < nbTrainingSets; j++) {

            int x = trainingOrder[j];

            //Forward propagation

            for (int n = 0; n < nbHiddenNodes; n++) {
                double activation = hiddenLayerBias[n];
                for (int k = 0; k < nbInput; k++)
                    activation += training_inputs[x][k] * hiddenWeight[k * nbInput + n];
                hiddenLayer[n] = sigmoid(activation);
            }

            for (int n = 0; n < nbOutput; n++) {
                double activation = outputLayerBias[n];
                for (int k = 0; k < nbHiddenNodes; k++)
                    activation += hiddenLayer[k] *
                                  outputWeight[k * nbHiddenNodes + n];
                outputLayer[n] = sigmoid(activation);
            }

            /*for (int n=0; n<nbHiddenNodes; n++) {
                double activation=hiddenLayerBias[n];
                for (int k=0; k<nbInput; k++)
                    activation+=training_inputs[x][k]*hiddenWeight[k*nbInput+n];
                hiddenLayer[n] = sigmoid(activation);
            }

            for (int n=0; n<nbOutput; n++) {
                double activation=outputLayerBias[n];
                for (int k=0; k<nbHiddenNodes; k++)
                    activation+=hiddenLayer[k]*outputWeight[k*nbHiddenNodes+n];
                outputLayer[n] = sigmoid(activation);
            }*/

            printf("Input: %f %f    Output: %lf    Expected Output: %lf \n",
                   training_inputs[x][0], training_inputs[x][1], outputLayer[0],
                   training_outputs[x][0]);

            //Back propagation;

            //Compute changes in output weights
            /*double deltaO[nbOutput];
            for (int n = 0; n < nbOutput; n++) {
                double errorO = training_outputs[x][n] - outputLayer[n];
                deltaO[n] = errorO * derivativeSigmoid(outputLayer[n]);
            }

            //Compute changes in hidden weights
            double deltaH[nbHiddenNodes];
            for (int n = 0; n < nbHiddenNodes; n++) {
                double errorH = 0.0;
                for (int k = 0; k < nbOutput; k++) {
                    errorH += deltaO[k] * outputWeight[n * nbOutput + k];
                }
                deltaH[n] = errorH * derivativeSigmoid(hiddenLayer[n]);
            }

            //Apply changes
            for (int n = 0; n < nbOutput; n++) {
                outputLayerBias[n] += deltaO[n] * lr;
                for (int k = 0; k < nbHiddenNodes; k++) {
                    outputWeight[k * nbHiddenNodes + n] +=
                            hiddenLayer[k] * deltaO[n] * lr;
                }
            }

            //Apply changes
            for (int n = 0; n < nbHiddenNodes; n++) {
                hiddenLayerBias[n] += deltaH[n] * lr;
                for (int k = 0; k < nbInput; k++)
                    hiddenWeight[k * nbInput + n] +=
                            training_inputs[x][n] * deltaH[n] * lr;
            }*/

            double deltaOutput[nbOutput];
            for (int n=0; n<nbOutput; n++) {
                double errorOutput = (training_outputs[x][n]-outputLayer[n]);
                deltaOutput[n] = errorOutput* derivativeSigmoid(outputLayer[n]);
            }

            double deltaHidden[nbHiddenNodes];
            for (int n=0; n<nbHiddenNodes; n++) {
                double errorHidden = 0.0;
                for(int k=0; k<nbOutput; k++) {
                    errorHidden+=deltaOutput[k]*outputWeight[n*nbOutput+k];
                }
                deltaHidden[n] = errorHidden*derivativeSigmoid(hiddenLayer[n]);
            }

            for (int n=0; n<nbOutput; n++) {
                outputLayerBias[n] += deltaOutput[n]*lr;
                for (int k=0; k<nbHiddenNodes; k++) {
                    outputWeight[k*nbHiddenNodes+n]+=hiddenLayer[k]*deltaOutput[n]*lr;
                }
            }

            for (int n=0; n<nbHiddenNodes; n++) {
                hiddenLayerBias[n] += deltaHidden[n]*lr;
                for(int k=0; k<nbInput; k++) {
                    hiddenWeight[k*nbInput+n]+=training_inputs[x][k]*deltaHidden[n]*lr;
                }
            }
        }
    }

    //Print weights
    printf("Hidden Weights: \n[");
    for (int i = 0; i < nbHiddenNodes; i++) {
        printf("[");
        for (int j = 0; j < nbInput;
             j++)
            printf("%f ", hiddenWeight[j * nbInput + i]);
        printf("]");
    }
    printf("]\n");

    printf("Hidden Biases: \n[");
    for (int i = 0; i < nbHiddenNodes; i++) {
        printf("%f ", hiddenLayerBias[i]);
    }
    printf("]\n");

    printf("Output Weights: \n[");
    for (int i = 0; i < nbOutput; i++) {
        printf("[");
        for (int j = 0; j < nbHiddenNodes; j++)
            printf("%f ", outputWeight[j * nbHiddenNodes + i]);
        printf("]\n");
    }

    printf("Output Biases: \n[");
    for (int i = 0; i < nbOutput; i++) {
        printf("%f ", outputLayerBias[i]);
    }
    printf("]\n");

    return 0;
}