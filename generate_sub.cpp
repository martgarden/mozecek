#include <fstream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

int main(int argc, char** argv) {
    unsigned seed = 1509871302;
    default_random_engine generator(seed);
    normal_distribution<double> distribution(0.0, 1.0);
    srand(time(NULL));
    vector<int> network{784, 45, 10};
    if(argc < 2) {
        cerr<<"File name needs to be specified"<<endl;
        return 0;
    }
    ofstream nfile(argv[1]);
    int prev = 0;
    for(auto i : network) {
        nfile<<"NEW LAYER"<<endl;
        nfile<<i<<endl;
        for(int j = 0; j < i; j++) {
            nfile<<distribution(generator); //bias
            for(int k = 0; k < prev; k++) {
                nfile<<" "<<distribution(generator); //weight
            }
            nfile<<endl;
        }
        prev = i;
    }
}
