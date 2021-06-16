/**
 * Author : Lise Giraud
 * Date : 16/06/2021
 */

#ifndef PROJECT_LOSS_FUNCTIONS_H
#define PROJECT_LOSS_FUNCTIONS_H

#include <stdbool.h>
#include "tensor.h"

struct LossFunction {
    // Calculate loss based on a single network output Tensor and a single ground truth Tensor
    float (*loss)(Tensor*, Tensor*);
    // Element-wise loss derivative. Takes ypred, ytrue, dest
    void (*lossDerivative)(Tensor*, Tensor*, Tensor*);
    // Returns true if network prediction matches ground truth. Takes ypred, ytrue
    bool (*correct)(Tensor*, Tensor*);
};

typedef struct LossFunction LossFunction;

extern LossFunction MeanSquaredError;

/**
 * compute the mean squared error for back propagation
 * @param ypred
 * @param ytrue
 * @return the mean squared error
 */
float meanSquaredError(Tensor* ypred, Tensor* ytrue);

/**
 * compute the deriative of mean squared error for back propagation
 * @param ypred
 * @param ytrue
 * @param dest
 */
void meanSquaredErrorDerivative(Tensor* ypred, Tensor* ytrue, Tensor* dest);

/**
 *
 * @param ypred
 * @param ytrue
 * @return a boolean indicating if the computed mean squarred error in correct
 */
bool meanSquaredErrorCorrect(Tensor* ypred, Tensor* ytrue);

#endif //PROJECT_LOSS_FUNCTIONS_H