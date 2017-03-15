
#include <iostream>
#include "Neuron.h"

const float odds_zero = 0.01f;
const float odds_one = 0.01f;
const float odds_negate = 0.01f;
const float odds_neg_one = 0.01f;
const float odds_vary = .05f;
const float max_diff = 1.f;

float chance;
float odds_accum;

Neuron::Neuron(int num_weights) {
	value = 0;
	for (int i = 0; i < num_weights; i++) {
		chance = rand()/float(RAND_MAX);
		if (chance < .4) {
			weights.push_back(1.0f);
		}
		else if (chance < .8) {
			weights.push_back(-1.0f);
		}
		else {
			weights.push_back(0.0f);
		}
	}
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
	chance = rand()/float(RAND_MAX); //assumes that srand has already been called.
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
		return weight + (rand()/float(RAND_MAX)) * 2 * max_diff*weight - max_diff*weight;
	}
	return weight;
}