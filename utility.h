#ifndef __UTILITY_H
#define __UTILITY_H

#include "neuron.h"
#include <vector>

typedef std::vector<std::vector<Neuron> > Network;

Network load(const std::string &file);

std::vector<double> loadInput(int size, const std::string &file);

void run(Network &network, std::vector<double> inputs);

void train(Network &network, const std::vector<std::pair<std::vector<double>, std::vector<double> > > &data, size_t batch_size, double rate);

#endif
