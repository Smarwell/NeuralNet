#pragma once

#include <vector>
#include <cstdlib>
#include <iostream>

using namespace std;

class Neuron {
	friend class Net;
	friend class Trainer;
	vector<float> weights;
	float value;

public:
	Neuron() {};
	Neuron(int);
	Neuron(vector<float>*);
	Neuron(const Neuron&);
	~Neuron();
	void mutate();

	void send_input(int id, float input) { value += weights[id] * input; }
	float get_value() { return 1/(1+pow(2.718,value)); }
	void reset() { value = 0; }

};

float mutate_weight(float);
