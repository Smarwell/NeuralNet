#pragma once

#include <vector>
#include <cstdlib>

const float odds_zero = 0.01;
const float odds_one = 0.01;
const float odds_negate = 0.01;
const float odds_neg_one = 0.01;
const float odds_vary = 0.05;
const float max_diff = 0.05;

float chance;
float odds_accum;

using namespace std;

class Neuron {
	vector<float> weights;
	float value;

public:
	Neuron();
	Neuron(int);
	Neuron(vector<float>*);
	Neuron(const Neuron&);
	void mutate();

	void send_input(int id, float input) { value += weights[id] * input; }
	float get_value() { return value; }
	void reset() { value = 0; }

};

float mutate_weight(float);
