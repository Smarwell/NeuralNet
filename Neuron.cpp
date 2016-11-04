
#include "Neuron.h"

Neuron::Neuron(int num_weights) {
	value = 0;
	for (int i = 0; i < num_weights; i++) {
		chance = rand();
		if (chance < .25) {
			weights.push_back(1);
		}
		else if (chance < .5) {
			weights.push_back(-1);
		}
		else {
			weights.push_back(0);
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
	value = 0;
	for (float weight : old.weights) {
		weights.push_back(weight);
	}
}

void Neuron::mutate() {
	for (float weight : weights) {
		weight = mutate_weight(weight);
	}
}

float mutate_weight(float weight) {
	chance = rand(); //assumes that srand has already been called.
	odds_accum = odds_zero;
	if (chance < odds_accum) {
		weight = 0;
		return;
	}
	odds_accum += odds_one;
	if (chance < odds_accum) {
		weight = 1;
		return;
	}
	odds_accum += odds_negate;
	if (chance < odds_accum) {
		weight = -weight;
		return;
	}
	odds_accum += odds_neg_one;
	if (chance < odds_accum) {
		weight = -1;
		return;
	}
	odds_accum += odds_vary;
	if (chance < odds_accum) {
		weight += rand() * 2 * max_diff*weight - max_diff*weight;
	}
	return weight;
}