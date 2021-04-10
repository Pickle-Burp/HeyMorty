//
// Created by liseg on 10/04/2021.
//

#ifndef HEYMORTY_NEURON_H
#define HEYMORTY_NEURON_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct neuron_t
{
    double actv;
    double *out_weights;
    double bias;
    double z;

    double dactv;
    double *dw;
    double dbias;
    double dz;
} neuron;

/**
 *
 * @param num_out_weights
 * @return a neuron
 */
neuron create_neuron(int num_out_weights);

#endif //HEYMORTY_NEURON_H
