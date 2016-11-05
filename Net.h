#pragma once

#include "Neuron.h"

class Net {
	int input_nodes;
	vector<vector<Neuron>> layers;
	vector<Neuron> output_nodes;
	vector<float> output;

public:
	float score;

	Net() {};
	Net(int, int, int, int);
	Net(const Net&);
	~Net();
	void mutate();
	void reset();
	vector<float>* eval(vector<float>&);
	void print();

	bool operator<(Net&);
};


class Trainer {
	vector<Net> nets;
	int survivors;
	int num_nets;
public:
	vector<vector<float>> training_inputs;
	vector<vector<float>> training_outputs;

	Trainer(int, int, int, int, int);
	Net& train(int);

	void score();
	void reproduce();

};