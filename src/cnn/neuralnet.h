/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#ifndef PROJECT_NEURALNET_H
#define PROJECT_NEURALNET_H

#include "tensor.h"
#include "loss_functions.h"
#include <err.h>

typedef struct NeuralNet NeuralNet;

struct NeuralNet {
    unsigned int depth; // Number of layers
    unsigned int* shape; // Array of length depth

    bool train;

    Tensor* input; // Input layer
    Tensor* output; // Output layer
    Tensor** layers; // Array of pointers to Tensors of length depth
    Tensor** zs; // Array of pointers to Tensors that correspond to layers without the activations applied
    Tensor** weights; // Array of pointers to tensors of length depth - 1 that stores the connection weights
    Tensor** biases; // Array of pointers to tensors of length depth - 1 that stores the biases for layer 2 onwards

    LossFunction lossFunction;
};

typedef Tensor** NNWeightsBiases;

/**
 * Initialize a new neural network
 * @param depth the depth of the neural network
 * @param shape the shape if the neural network
 * @param lossFunction the loss function
 * @return initialized neural network
 */
NeuralNet* newNeuralNet(unsigned int depth, unsigned int* shape, LossFunction lossFunction);


/**
 * free the memory allocated for the neural network
 * @param nn neural network to free
 */
void freeNeuralNet(NeuralNet* nn);

/**
 * initialize the neural network's biases and weight to random values
 * @param nn
 */
void randInit(NeuralNet* nn);

/**
 * Save Neural Network weights and biases to a binary file for loading later
 * File schema:
 * [offset] | [type]  | [desc]
 * 0000       uint32    Network depth
 * 0004       uint32    Shape of layer 1
 * 0008       uint32    Shape of layer 2
 * ...
 * 00xx       uint32    Shape of layer n - 1
 * 0xxx       float32   weights[0]
 * ...
 * 0xxx       float32   biases[0]
 * ...
 * xxxx       float32   weights[n - 1]
 * ...
 * xxxx       float32   biases[n - 1]
 * @param nn
 * @param fileName
 */
void saveNeuralNet(NeuralNet* nn, char* fileName);

/**
 * load trained neural network
 * @param nn neural network to load
 * @param fileName saved model of neural network
 */
void loadNeuralNet(NeuralNet* nn, char* fileName);

/**
 * forward propagation on neural network
 * @param nn neural network
 */
void forwardPass(NeuralNet* nn);

/**
 * update the weights of the biases
 * @param nn neural network
 * @return weight of biases
 */
NNWeightsBiases* newWeightBiasUpdate(NeuralNet* nn);

/**
 * does the scalar multiplication between weight biases and an scalar
 * @param nn neural network
 * @param wb weight biases
 * @param scalar
 */
void scaleWeightBiasUpdate(NeuralNet* nn, NNWeightsBiases* wb, float scalar);

/**
 * copy the weight and biases update
 * @param nn neural network
 * @param src source tensor to copy
 * @param dest destnation tensor to copy onto
 */
void copyWeightBiasUpdate(NeuralNet* nn, NNWeightsBiases* src, NNWeightsBiases* dest);

/**
 * add the weights biases a, b and c
 * @param nn neural network
 * @param a
 * @param b
 * @param c
 */
void addWeightBiasUpdate(NeuralNet* nn, NNWeightsBiases* a, NNWeightsBiases* b, NNWeightsBiases* c);

/**
 * free the tensor of weight biases
 * @param nn neural network
 * @param wb weight bias
 */
void freeWeightBiasUpdate(NeuralNet* nn, NNWeightsBiases* wb);

/**
 * Back propagation of the neural network
 * @param nn
 * @param wb
 * @param yTrue
 */
void backProp(NeuralNet *nn, NNWeightsBiases* wb, Tensor* yTrue);

/**
 * Apply the back propagation to the neural network
 * @param nn neural network
 * @param wb weight of the bias
 * @param lr learning rate
 */
void applyBackProp(NeuralNet* nn, NNWeightsBiases* wb, float lr);

#endif //PROJECT_NEURALNET_H
