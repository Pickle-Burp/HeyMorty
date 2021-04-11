#include "nn.h"

layer *lay = NULL;
int num_layers;
int *num_neurons;
double alpha;
double *cost;
double full_cost;
double **input;
double **desired_outputs;
int num_training_ex;
int n = 1;

void test_neural_network() {
  //Get number of layers
  printf("Enter the number of Layers in Neural Network:\n");
  scanf("%d", &num_layers);

  num_neurons = (int *) malloc(num_layers * sizeof(int));
  memset(num_neurons, 0, num_layers * sizeof(int));

  // Get number of neurons per layer
  for (int i = 0; i < num_layers; i++) {
    printf("Enter number of neurons in layer[%d]: \n", i + 1);
    scanf("%d", &num_neurons[i]);
  }

  printf("\n");

  // Initialize the neural network module
  if (init() != SUCCESS_INIT) {
    printf("Error in Initialization...\n");
    exit(0);
  }

  printf("Enter the learning rate (Usually 0.15): \n");
  scanf("%lf", &alpha);
  printf("\n");

  printf("Enter the number of training examples: \n");
  scanf("%d", &num_training_ex);
  printf("\n");

  input = (double **) malloc(num_training_ex * sizeof(double *));
  for (int i = 0; i < num_training_ex; i++) {
    input[i] = (double *) malloc(num_neurons[0] * sizeof(double));
  }

  desired_outputs = (double **) malloc(num_training_ex * sizeof(double *));
  for (int i = 0; i < num_training_ex; i++) {
    desired_outputs[i] = (double *) malloc(
        num_neurons[num_layers - 1] * sizeof(double));
  }

  cost = (double *) malloc(num_neurons[num_layers - 1] * sizeof(double));
  memset(cost, 0, num_neurons[num_layers - 1] * sizeof(double));

  // Get Training Examples
  get_inputs();

  // Get Output Labels
  get_desired_outputs();

  train_neural_net();
  test_nn();

  if (dinit() != SUCCESS_DINIT) {
    printf("Error in Dinitialization...\n");
  }

  return 0;
}

int init() {
  if (create_architecture() != SUCCESS_CREATE_ARCHITECTURE) {
    printf("Error in creating architecture...\n");
    return ERR_INIT;
  }

  printf("Neural Network Created Successfully...\n\n");
  return SUCCESS_INIT;
}

//Get Inputs
void get_inputs() {
  for (int i = 0; i < num_training_ex; i++) {
    printf("Enter the Inputs for training example[%d]:\n", i);

    for (int j = 0; j < num_neurons[0]; j++) {
      scanf("%lf", &input[i][j]);

    }
    printf("\n");
  }
}

//Get Labels
void get_desired_outputs() {
  for (int i = 0; i < num_training_ex; i++) {
    for (int j = 0; j < num_neurons[num_layers - 1]; j++) {
      printf("Enter the Desired Outputs (Labels) for training example[%d]: \n",
             i);
      scanf("%lf", &desired_outputs[i][j]);
      printf("\n");
    }
  }
}

// Feed inputs to input layer
void feed_input(int i) {
  for (int j = 0; j < num_neurons[0]; j++) {
    lay[0].neu[j].actv = input[i][j];
    printf("Input: %f\n", lay[0].neu[j].actv);
  }
}

// Create Neural Network Architecture
int create_architecture() {
  lay = (layer *) malloc(num_layers * sizeof(layer));

  for (int i = 0; i < num_layers; i++) {
    lay[i] = create_layer(num_neurons[i]);
    lay[i].num_neu = num_neurons[i];
    printf("Created Layer: %d\n", i + 1);
    printf("Number of Neurons in Layer %d: %d\n", i + 1, lay[i].num_neu);

    for (int j = 0; j < num_neurons[i]; j++) {
      if (i < (num_layers - 1)) {
        lay[i].neu[j] = create_neuron(num_neurons[i + 1]);
      }

      printf("Neuron %d in Layer %d created\n", j + 1, i + 1);
    }
    printf("\n");
  }

  printf("\n");

  // Initialize the weights
  if (initialize_weights() != SUCCESS_INIT_WEIGHTS) {
    printf("Error Initilizing weights...\n");
    return ERR_CREATE_ARCHITECTURE;
  }

  return SUCCESS_CREATE_ARCHITECTURE;
}

//Initialize weights
int initialize_weights(void) {
  if (lay == NULL) {
    printf("No layers in Neural Network...\n");
    return ERR_INIT_WEIGHTS;
  }

  printf("Initializing weights...\n");

  for (int i = 0; i < num_layers - 1; i++) {

    for (int j = 0; j < num_neurons[i]; j++) {
      for (int k = 0; k < num_neurons[i + 1]; k++) {
        // Initialize Output Weights for each neuron
        lay[i].neu[j].out_weights[k] =
            ((double) rand()) / ((double) RAND_MAX);
        printf("%d:w[%d][%d]: %f\n", k, i, j,
               lay[i].neu[j].out_weights[k]);
        lay[i].neu[j].dw[k] = 0.0;
      }

      if (i > 0) {
        lay[i].neu[j].bias = ((double) rand()) / ((double) RAND_MAX);
      }
    }
  }
  printf("\n");

  for (int j = 0; j < num_neurons[num_layers - 1]; j++) {
    lay[num_layers - 1].neu[j].bias =
        ((double) rand()) / ((double) RAND_MAX);
  }

  return SUCCESS_INIT_WEIGHTS;
}

// Train Neural Network
void train_neural_net(void) {
  // Gradient Descent
  for (int it = 0; it < 20000; it++) {
    for (int i = 0; i < num_training_ex; i++) {
      feed_input(i);
      forward_prop();
      compute_cost(i);
      back_prop(i);
      update_weights();
    }
  }
}

//Update weights (back propagation)
void update_weights(void) {
  for (int i = 0; i < num_layers - 1; i++) {
    for (int j = 0; j < num_neurons[i]; j++) {
      for (int k = 0; k < num_neurons[i + 1]; k++) {
        // Update Weights
        lay[i].neu[j].out_weights[k] = (lay[i].neu[j].out_weights[k]) -
            (alpha * lay[i].neu[j].dw[k]);
      }

      // Update Bias
      lay[i].neu[j].bias =
          lay[i].neu[j].bias - (alpha * lay[i].neu[j].dbias);
    }
  }
}

//Forward propagation
void forward_prop(void) {
  for (int i = 1; i < num_layers; i++) {
    for (int j = 0; j < num_neurons[i]; j++) {
      lay[i].neu[j].z = lay[i].neu[j].bias;

      for (int k = 0; k < num_neurons[i - 1]; k++) {
        lay[i].neu[j].z = lay[i].neu[j].z +
            ((lay[i - 1].neu[k].out_weights[j]) *
                (lay[i - 1].neu[k].actv));
      }

      // Relu Activation Function for Hidden Layers
      if (i < num_layers - 1) {
        if ((lay[i].neu[j].z) < 0) {
          lay[i].neu[j].actv = 0;
        } else {
          lay[i].neu[j].actv = lay[i].neu[j].z;
        }
      }

        // Sigmoid Activation function for Output Layer
      else {
        lay[i].neu[j].actv = 1 / (1 + exp(-lay[i].neu[j].z));
        printf("Output: %d\n", (int) round(lay[i].neu[j].actv));
        printf("\n");
      }
    }
  }
}

// Compute Total Cost
void compute_cost(int i) {
  double tcost = 0;

  for (int j = 0; j < num_neurons[num_layers - 1]; j++) {
    double tmpcost =
        desired_outputs[i][j] - lay[num_layers - 1].neu[j].actv;
    cost[j] = (tmpcost * tmpcost) / 2;
    tcost = tcost + cost[j];
  }

  full_cost = (full_cost + tcost) / n;
  n++;
  // printf("Full Cost: %f\n",full_cost);
}

// Back Propogate Error
void back_prop(int p) {
  // Output Layer
  for (int j = 0; j < num_neurons[num_layers - 1]; j++) {
    lay[num_layers - 1].neu[j].dz =
        (lay[num_layers - 1].neu[j].actv - desired_outputs[p][j]) *
            (lay[num_layers - 1].neu[j].actv) *
            (1 - lay[num_layers - 1].neu[j].actv);

    for (int k = 0; k < num_neurons[num_layers - 2]; k++) {
      lay[num_layers - 2].neu[k].dw[j] = (lay[num_layers - 1].neu[j].dz *
          lay[num_layers -
              2].neu[k].actv);
      lay[num_layers - 2].neu[k].dactv =
          lay[num_layers - 2].neu[k].out_weights[j] *
              lay[num_layers - 1].neu[j].dz;
    }

    lay[num_layers - 1].neu[j].dbias = lay[num_layers - 1].neu[j].dz;
  }

  // Hidden Layers
  for (int i = num_layers - 2; i > 0; i--) {
    for (int j = 0; j < num_neurons[i]; j++) {
      if (lay[i].neu[j].z >= 0) {
        lay[i].neu[j].dz = lay[i].neu[j].dactv;
      } else {
        lay[i].neu[j].dz = 0;
      }

      for (int k = 0; k < num_neurons[i - 1]; k++) {
        lay[i - 1].neu[k].dw[j] =
            lay[i].neu[j].dz * lay[i - 1].neu[k].actv;

        if (i > 1) {
          lay[i - 1].neu[k].dactv =
              lay[i - 1].neu[k].out_weights[j] * lay[i].neu[j].dz;
        }
      }

      lay[i].neu[j].dbias = lay[i].neu[j].dz;
    }
  }
}

// Test the trained network
//Never stop, except if the user kills the process, the neural network can be tested forever
void test_nn(void) {
  while (1) {
    printf("Enter input to test:\n");

    for (int i = 0; i < num_neurons[0]; i++) {
      scanf("%lf", &lay[0].neu[i].actv);
    }
    forward_prop();
  }
}

int dinit(void) {
  // Free up all the structure
  for (int i = 0; i < num_layers; i++) {
    for (int j = 0; j < num_neurons[i]; j++) {
      free(lay[i].neu[j].out_weights);
      free(lay[i].neu[j].dw);
    }
    free(lay[i].neu);
  }
  free(num_neurons);
  for (int i = 0; i < num_training_ex; i++) {
    free(input[i]);
    free(desired_outputs[i]);
  }
  free(input);
  free(desired_outputs);
  free(cost);
  free(lay);

  return SUCCESS_DINIT;
}