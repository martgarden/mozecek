#include <iostream>
#include <sstream>
#include <fstream>
#include <random>
#include <algorithm>
#include <chrono>
#include "utility.hpp"

using namespace std;

vector<string> explode(const string& str, char delim)
{
    istringstream src(str);
    vector<string> ret;
    for (string buf; getline(src, buf, delim);)
        ret.push_back(std::move(buf));
    return ret;
}

size_t net_check(Network &network, const vector<pair<vector<double>, int > > &test) {
    size_t cnt = 0;
    for(auto &i : test) {
        run(network, i.first);
        int max = -1;
        double max_val = -1.0;
        for(size_t j = 0; j < network.back().size(); j++)
            if(network.back()[j].getOutput() > max_val) {
                max = j;
                max_val = network.back()[j].getOutput();
            }
        if(max == i.second)
            cnt++;
    }
    cout<<cnt<<" / "<<test.size()<<endl;
    return cnt;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        cerr<<"Not enough arguments. Usage: train input_file output_file"<<endl;
        return 0;
    }

    auto network = load(argv[1]);
    ifstream f_vec("MNIST_DATA/mnist_train_vectors.csv");
    ifstream f_lab("MNIST_DATA/mnist_train_labels.csv");
    vector<pair<vector<double>, vector<double> > > data(50000);
    string in;
    for(size_t i = 0; i < 50000; i++) {
        getline(f_vec, in);
        for(auto num: explode(in, ','))
            data[i].first.push_back(stoi(num)/256.0);
        int lab;
        f_lab>>lab;
        for(size_t j = 0; j < 10; j++) {
            data[i].second.push_back(double(j == lab));
        }
    }
    vector<pair<vector<double>, int > > test(10000);
    for(size_t i = 0; i < 10000; i++) {
        getline(f_vec, in);
        for(auto num: explode(in, ','))
            test[i].first.push_back(stoi(num)/256.0);
        int lab;
        f_lab>>lab;
        test[i].second = lab;
    }

    size_t epochs = 13;
    size_t batch_size = 10; //must divide 50000
    double rate = 3.00;

    random_device rd;
    mt19937 g(rd());
    auto start = chrono::steady_clock::now();
    for(size_t i = 0; i < epochs; i++) {
        shuffle(data.begin(), data.end(), g);
        train(network, data, batch_size, rate);
        cout<<"epoch done"<<endl;
    }
    epochs = 10;
    rate = 3.00;
    size_t max_score = 0;
    Network best_one = network;
    for(size_t i = 0; i < epochs; i++) {
        shuffle(data.begin(), data.end(), g);
        train(network, data, batch_size, rate);
        if(size_t score = net_check(network, test) > max_score) {
            max_score = score;
            best_one = network;
        }
    }
    write(best_one, argv[2]);
    auto end = chrono::steady_clock::now();
    chrono::duration<float, ratio<60>> dur = end - start;
    cout<<"Training took "<< dur.count()<<" minutes"<<endl;

    return 0;
}
