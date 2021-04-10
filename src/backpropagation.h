//
// Created by liseg on 10/04/2021.
//

#ifndef HEYMORTY_BACKPROPAGATION_H
#define HEYMORTY_BACKPROPAGATION_H

#include "layer.h"

#define SUCCESS_INIT 0
#define ERR_INIT 1

#define SUCCESS_DINIT 0
#define ERR_DINIT 1

#define SUCCESS_INIT_WEIGHTS 0
#define ERR_INIT_WEIGHTS 1

#define SUCCESS_UPDATE_WEIGHTS 0

#define SUCCESS_CREATE_ARCHITECTURE 0
#define ERR_CREATE_ARCHITECTURE 1


int init(void);

int dinit(void);



/**
 *
 * @return the architecture of the neural network on success, return an error otherwise.
 */
int create_architecture(void);



/**
 *
 * @return the weight of a layer on success, return an error otherwise.
 */
int initialize_weights(void);


/**
 * Feed inputs to input layer
 * @param i is the i-th training example.
 */
void feed_input(int i);


/**
 *train the neural network using gradient descent
 * calls all the needed functions.
 */
void train_neural_net(void);



/**
 *forward propagation function.
 */
void forward_prop(void);


/**
 *compute total cost
 * @param i is the i-th training example.
 */
void compute_cost(int i);


/**
 *
 * @param p is the p-th training example.
 * back propagation function.
 */
void back_prop(int p);


/**
 * update the weights and bias for every epoch.
 */
void update_weights(void);



/**
 * store the training example given by the user.
 */
void get_inputs(void);


/**
 * store output labels for the training.
 */
void get_desired_outputs(void);


/**
 * get and compute the test the user want to do.
 */
void test_nn(void);

int initialize_dummy_weights(void);

#endif //HEYMORTY_BACKPROPAGATION_H
