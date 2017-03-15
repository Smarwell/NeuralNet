#pragma once

#include <vector>
#include <iostream>

using namespace std;

class Neuron {
public:
	vector<float> weights;
	float value;


	Neuron() {};
	Neuron(int);
	Neuron(vector<float>*);
	Neuron(const Neuron&);
	~Neuron();
	void mutate();

	void send_input(int id, float input) { value += weights[id] * input; }
	float get_value() { return float(1/(1+pow(2.718,-value))); }
	void reset() { value = 0; }

};

float mutate_weight(float);



const float odds_zero = 0.05f;
const float odds_one = 0.01f;
const float odds_negate = 0.05f;
const float odds_neg_one = 0.01f;
const float odds_vary = .1f;
const float max_diff = 5.f;

float chance;
float odds_accum;

Neuron::Neuron(int num_weights) {
	value = 0;
	for (int i = 0; i < num_weights; i++) {
		chance = rand() / float(RAND_MAX);
		if (chance < .3) {
			weights.push_back(5.f*rand()/float(RAND_MAX));
		}
		else if (chance < .6) {
			weights.push_back(-5.f*rand() / float(RAND_MAX));
		}
		else {
			weights.push_back(0.0f);
		}
	}
	mutate();
	mutate();
}

Neuron::Neuron(vector<float>* new_weights) {
	value = 0;
	for (float weight : *new_weights) {
		weights.push_back(weight);
	}
}

Neuron::Neuron(const Neuron& old) {
	value = old.value;
	for (float weight : old.weights) {
		weights.push_back(weight);
	}
}

Neuron::~Neuron() {
	weights.clear();
	weights.shrink_to_fit();
}

void Neuron::mutate() {
	for (float& weight : weights) {
		weight = mutate_weight(weight);
	}
}

float mutate_weight(float weight) {
	chance = rand() / float(RAND_MAX);
	odds_accum = odds_zero;
	if (chance < odds_accum) {
		return 0;
	}
	odds_accum += odds_one;
	if (chance < odds_accum) {
		return 1;
	}
	odds_accum += odds_negate;
	if (chance < odds_accum) {
		return -weight;
	}
	odds_accum += odds_neg_one;
	if (chance < odds_accum) {
		return -1;
	}
	odds_accum += odds_vary;
	if (chance < odds_accum) {
		return weight + (rand() / float(RAND_MAX)) * 2 * max_diff*weight - max_diff*weight;
	}
	return weight;
}
