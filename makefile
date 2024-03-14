all: train generate_sub evaluate
train: train.cpp utility.o neuron.o MNIST_DATA/mnist_train_vectors.csv
	clang++ -o train train.cpp utility.o neuron.o -std=c++14
MNIST_DATA/mnist_train_vectors.csv:
	cat MNIST_DATA/mnist_train_vectors.csv.part1 MNIST_DATA/mnist_train_vectors.csv.part2 > MNIST_DATA/mnist_train_vectors.csv
generate_sub: generate_sub.cpp
	clang++ -o generate_sub generate_sub.cpp -std=c++14
evaluate: evaluate.cpp utility.o
	clang++ -o evaluate evaluate.cpp utility.o neuron.o -std=c++14
utility.o: utility.cpp utility.h neuron.o
	clang++ -c -o utility.o utility.cpp -std=c++14
neuron.o: neuron.cpp neuron.h
	clang++ -c -o neuron.o neuron.cpp -std=c++14
