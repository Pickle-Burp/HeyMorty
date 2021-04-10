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
    float actv;
    float *out_weights;
    float bias;
    float z;

    float dactv;
    float *dw;
    float dbias;
    float dz;
} neuron;

neuron create_neuron(int num_out_weights);

#endif //HEYMORTY_NEURON_H
