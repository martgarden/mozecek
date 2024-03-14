#ifndef __NEURON_H
#define __NEURON_H

#include<vector>
#include<iostream>
#include<cmath>

class Neuron {
    private:
        double bias;
        std::vector<double> weights;
        double bias_derivative = 0;
        std::vector<double> derivatives;

        double ksi = 0;
        double output = 0;
        double error = 0;
    public:
        Neuron() {;}

        Neuron(double _bias, std::vector<double> _weights):bias(_bias), weights(_weights), derivatives(_weights.size()) {
            //std::cout<<"New neuron. Bias: "<<bias<<std::endl;
            for(auto i:weights) {
                //std::cout<<i<<std::endl;
            }
        }

        void setOutput(double _output) {output = _output;}

        double getOutput() const {return output;}

        double compute(const std::vector<Neuron> &prev_layer);

        void gradient(const std::vector<Neuron> &prev_layer);

        double c_error(double desired) {
            error = activation_d(ksi) * desired;
            bias_derivative += error;
            return error;
        }

        double product(size_t index) const { return weights[index]*error; }

        void change(double rate) {
            bias -= bias_derivative*rate;
            bias_derivative = 0;
            for(size_t i = 0; i < weights.size(); i++) {
                weights[i] -= derivatives[i]*rate;
                derivatives[i] = 0;
            }
        }

        static double activation(double in) {
            return 1.0/(1.0+exp(-in));
        }

        static double activation_d(double in) {
            return activation(in)*(1-activation(in));
        }

        friend void write(const std::vector<std::vector<Neuron> > &network, const std::string &file);
};

#endif
