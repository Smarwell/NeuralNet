#pragma once

#include "Neuron.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>


class Net {
public:
	int input_nodes;
	vector<vector<Neuron*>*> layers;
	vector<Neuron*> output_nodes;
	float* output;

	float score;

	Net() :output(NULL) {};
	Net(int, int, int, int);
	Net(const Net&);
	Net(int id);
	~Net();
	void mutate();
	void reset();
	float* eval(float*);
	void print();

	bool operator<(Net&);
};

