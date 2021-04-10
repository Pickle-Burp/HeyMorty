//
// Created by liseg on 10/04/2021.
//

#ifndef HEYMORTY_LAYER_H
#define HEYMORTY_LAYER_H
#include "neuron.h"
#include <errno.h>
#include <limits.h>


typedef struct layer_t
{
    int num_neu;
    struct neuron_t *neu;
} layer;

/**
 *
 * @param num_neurons array containing the number of neurons for each layer.
 * @return a layer with num_neurons
 */
layer create_layer(int num_neurons);

#endif //HEYMORTY_LAYER_H
