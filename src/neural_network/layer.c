//
// Created by liseg on 10/04/2021.
//
#include "layer.h"

//Create a layer when called.
layer create_layer(int number_of_neurons) {
  layer lay;
  lay.num_neu = -1;
  lay.neu = (struct neuron_t *) malloc(
      number_of_neurons * sizeof(struct neuron_t));
  return lay;
}