/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#ifndef PROJECT_MNIST_DATASET_H
#define PROJECT_MNIST_DATASET_H

#include <stdbool.h>
#include "dataset.h"
#include <err.h>

/**
 * Convert unsigned int from Big Endian to Little Endian
 * @param value is the value to convert for big endian to little endian
 * @return the converted value
 */
unsigned int convertToLE(unsigned int value);

/**
 * load the images for the neural network
 * @param source the file were images are
 * @param dest the destination tensor
 * @return element converted to little endian
 */
size_t loadImages(char* source, Tensor*** dest);


/**
 *
 * @param x the index of the sentence corresponding to the one said in audio file
 * @return the text of the sentence
 */
char *expectedSentence(int x);

/**
 *
 * @param source the file were labels are
 * @param dest the destination tensor
 * @return converted elements
 */
size_t loadLabels(char* source, Tensor*** dest);

/**
 *
 * @param X
 * @param Y
 * @param dest
 * @param size
 */
void zipData(Tensor **X, Tensor **Y, Datum *dest, size_t size);

/**
 * get the training element
 * @param element the training element
 * @return the training element
 */
Datum getTrainElement(size_t element);


/**
 * get the testing element
 * @param element the testing element
 * @return the testing element
 */
Datum getTestElement(size_t element);
/**
 * Fills the fields of Dataset
 * @param trainImages set of training images
 * @param trainLabels set of training labels
 * @param testImages set of images for testing
 * @param testLabels set of lables for testing
 * @return struct Dataset
 */
Dataset FFT(char* trainImages, char* trainLabels, char* testImages, char* testLabels);


/**
 *
 * @param datum the data of the neural network
 * @return the true if the datum is valid, else false
 */
bool datumValid(Datum datum);


/**
 * print results of the neural network
 * @param datum data of the neural network
 */
void printDatum(Datum datum);

#endif //PROJECT_MNIST_DATASET_H
