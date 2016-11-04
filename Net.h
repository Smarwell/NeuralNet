#pragma once

#include "Neuron.h"

class Net {
	int input_nodes;
	vector<vector<Neuron>> layers;
	vector<Neuron> output_nodes;
	vector<float> output;

public:
	Net() {};
	Net(int, int, int, int);
	Net(const Net&);
	void mutate();
	void reset();
	vector<float>* eval(vector<float>&);
};