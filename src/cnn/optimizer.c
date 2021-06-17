/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#include "optimizer.h"

void SGD_datumOptimize(NeuralNet* nn, NNWeightsBiases* wb, float lr) {
}

void SGD_batchOptimize(NeuralNet* nn, NNWeightsBiases* wb, float lr) {
    applyBackProp(nn, wb, lr);
}

void SGD_epochOptimize(NeuralNet* nn, NNWeightsBiases* wb, float lr) {

}

Optimizer SGD = {
    .datumOptimize = SGD_datumOptimize,
    .batchOptimize = SGD_batchOptimize,
    .epochOptimize = SGD_epochOptimize
};