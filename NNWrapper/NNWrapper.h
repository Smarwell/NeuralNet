#pragma once
/*
#ifdef NNWRAPPER_EXPORTS
#define NNWRAPPER_API __declspec(dllexport)
#else
#define NNWRAPPER_API __declspec(dllimport)
#endif
*/
#define EXPORT __declspec(dllexport)

#include<vector>
#include<algorithm>
#include<iostream>
#include "Net.h"
#include "Net_defines.h"
#include "Operators.h"
#include <time.h>


class Trainer {
public:
	std::vector<Net*> nets;
	std::vector<float>* curr_results;
	int num_inputs;
	int num_outputs;

	int num_survivors;
};

void print(vector<Net*>& nets) {
	cout << "Nets:" << endl;
	for (size_t i = 0; i < nets.size(); i++) {
		cout << nets[i]->score << endl;
	}
	cout << endl << endl;
}

Trainer t;

extern "C" {

	EXPORT void init(int n_inputs, int n_layers, int height, int n_outputs, int n_nets, int n_survivors) {
		srand((unsigned int)time(NULL));
		if (t.nets.size() > 0) {
			for (size_t i = 0; i < t.nets.size(); i++) {
				delete t.nets[i];
			}
		}
		t.nets.clear();
		for (int i = 0; i < n_nets; i++) {
			t.nets.push_back(new Net(n_inputs, n_layers, height, n_outputs));
		}
		t.num_survivors = n_survivors;
		t.num_inputs = n_inputs;
		t.num_outputs = n_outputs;
	}

	EXPORT float* eval(int net, float* inputs) {
		return t.nets[net]->eval(inputs);
	}

	EXPORT void score_net(int net, float score) {
		t.nets[net]->score = score;
	}

	EXPORT void reproduce() {
		std::sort(t.nets.begin(), t.nets.end(), net_cmp);
		size_t i;
		for (i = t.num_survivors; i < t.nets.size(); i++) {
			delete t.nets[i];
			t.nets[i] = new Net(*t.nets[i%t.num_survivors]);
			t.nets[i]->mutate();
		}
	}

	EXPORT void save_net(int i, bool visualize=false) {
		save(t.nets[i],visualize);
	}

	EXPORT void load_nets(int* nets, int count) {
		for (size_t i = 0; i < (unsigned int)count && i < t.nets.size(); i++) {
			t.nets[i] = new Net(nets[i]);
		}
	}
}