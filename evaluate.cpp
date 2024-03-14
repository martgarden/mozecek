#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include "utility.h"

using namespace std;

vector<string> explode(const string& str, char delim)
{
    istringstream src(str);
    vector<string> ret;
    for (string buf; getline(src, buf, delim);)
        ret.push_back(move(buf));
    return ret;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        cerr<<"Not enough arguments. Usage: evaluate input_file"<<endl;
        return 0;
    }

    auto network = load(argv[1]);
    ifstream f_vec("MNIST_DATA/mnist_train_vectors.csv");
    ifstream f_vec_test("MNIST_DATA/mnist_test_vectors.csv");
    vector<vector<double> > data(60000);
    string in;
    for(size_t i = 0; i < 60000; i++) {
        getline(f_vec, in);
        for(auto num: explode(in, ','))
            data[i].push_back(stoi(num)/256.0);
    }
    vector<vector<double> > test(10000);
    for(size_t i = 0; i < 10000; i++) {
        getline(f_vec_test, in);
        for(auto num: explode(in, ','))
            test[i].push_back(stoi(num)/256.0);
    }

    ofstream fout("trainPredictions");
    for(auto &i : data) {
        run(network, i);
        int max = -1;
        double max_val = -1.0;
        for(size_t j = 0; j < network.back().size(); j++)
            if(network.back()[j].getOutput() > max_val) {
                max = j;
                max_val = network.back()[j].getOutput();
            }
        fout<<max<<endl;
    }
    ofstream fout_test("actualTestPredictions");
    for(auto &i : test) {
        run(network, i);
        int max = -1;
        double max_val = -1.0;
        for(size_t j = 0; j < network.back().size(); j++)
            if(network.back()[j].getOutput() > max_val) {
                max = j;
                max_val = network.back()[j].getOutput();
            }
        fout_test<<max<<endl;
    }


    return 0;
}
