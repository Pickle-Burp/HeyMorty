/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#ifndef PROJECT_TRAINER_H
#define PROJECT_TRAINER_H

#include <stdbool.h>
#include "neuralnet.h"
#include "dataset.h"
#include "optimizer.h"

struct TrainingInfo {
    size_t epochIndex;
    float loss;
    float accuracy;
};

typedef struct TrainingInfo TrainingInfo;
/**
 *
 * @param nn neural network
 * @param dataset set of data
 * @param epochIndex current epoch
 * @return TrainingInfo struct updated
 */
TrainingInfo postEpochBenchmarks(NeuralNet* nn, Dataset dataset, size_t epochIndex);

/**
 * train the neural network
 * @param nn neural network
 * @param optimizer
 * @param dataset
 * @param epochs
 * @param batchSize
 * @param learnRate
 * @param epochCallback
 */
void train(NeuralNet* nn, Optimizer optimizer, Dataset dataset,
           size_t epochs, unsigned int batchSize, float learnRate, void (*epochCallback)(TrainingInfo));

/**
 * print the epoch and intels on it (loss, accuracy ...)
 * @param trainingInfo struct of info about the training
 */
void printEpochCallback(TrainingInfo trainingInfo);

#endif //PROJECT_TRAINER_H
