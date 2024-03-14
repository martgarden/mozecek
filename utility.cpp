#include "utility.hpp"

#include <fstream>

using namespace std;

vector<vector<Neuron> > load(const string &file) {
    ifstream nfile(file);
    string line;
    vector<vector<Neuron> > network;
    int prev_lay = 0;
    do {
        getline(nfile, line);
        if(line == "NEW LAYER") {
            int size;
            nfile>>size;
            network.push_back(vector<Neuron>(size));
            for(int i = 0; i < size; i++) {
                double bias;
                nfile>>bias;
                vector<double> inputs(prev_lay);
                for(int j = 0; j < prev_lay; j++) {
                    double weight;
                    nfile>>weight;
                    inputs[j] = weight;
                }
                network.back().at(i) = Neuron(bias, std::move(inputs));
            }
            prev_lay = size;
        }
    } while (not nfile.eof());
    return std::move(network);
}

vector<double> loadInput(int size, const string &file) {
    ifstream ifile(file);
    vector<double> ret(size);
    for(auto &i: ret) {
        ifile>>i;
    }
    return std::move(ret);
}

void run(vector<vector<Neuron> > &network, vector<double> inputs) {
    for(int i = 0; i < inputs.size(); i++) {
        network[0][i].setOutput(inputs[i]);
    }
    for(auto i = network.begin() + 1; i != network.end(); i++) {
        for(auto &j: *i) {
            j.compute(*(i-1));
        }
    }
}

void train(Network &network, const std::vector<std::pair<std::vector<double>, std::vector<double> > > &data, size_t batch_size, double rate) {
    for(size_t i = 0; i < data.size(); i++) {
        run(network, data[i].first);
        for(size_t j = 0; j < network.back().size(); j++) {
            network.back()[j].c_error(network.back()[j].getOutput() - data[i].second[j]);
        }
        for(size_t j = network.size() - 2; j >= 1; j--) {
            for(size_t k = 0; k < network[j].size(); k++) {
                double scalar = 0;
                for(auto &next_neuron: network[j+1]) {
                    scalar += next_neuron.product(k);
                }
                network[j][k].c_error(scalar);
            }
        }
        for(auto i = network.begin() + 1; i != network.end(); i++) {
            for(auto &j: *i) {
                j.gradient(*(i-1));
            }
        }

        if(i % batch_size == batch_size - 1)
            for(auto &layer: network) {
                for(auto &neuron: layer) {
                    neuron.change(rate/batch_size);
                }
            }
    }
}

void write(const Network &network, const string &file) {
    ofstream nfile(file);
    for(auto &layer: network) {
        nfile<<"NEW LAYER"<<endl;
        nfile<<layer.size()<<endl;
        for(auto &neuron: layer) {
            nfile<<neuron.bias;
            for(auto weight: neuron.weights)
                nfile<<" "<<weight;
            nfile<<endl;
        }
    }
}


