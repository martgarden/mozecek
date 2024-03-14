#include "neuron.h"

using namespace std;

double Neuron::compute(const vector<Neuron> &prev_layer) {
    ksi = bias;
    for(int i = 0; i < prev_layer.size(); i++) {
        ksi += prev_layer[i].getOutput()*weights[i];
    }
    output = activation(ksi);
    return output;
}

void Neuron::gradient(const vector<Neuron> &prev_layer) {
    for(int i = 0; i < prev_layer.size(); i++) {
        derivatives[i] += prev_layer[i].getOutput()*error;
    }
}
