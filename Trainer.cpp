
#include "Net.h"
#include <algorithm>
#include <ctime>


Trainer::Trainer(int input_nodes, int num_layers, int layer_height, int output_nodes, int number_nets) {
	srand(time(NULL)%10000);
	num_nets = number_nets;
	survivors = 2;//int(num_nets / 10.0);
	for (int i = 0; i < num_nets; i++) {
		nets.push_back(*new Net(input_nodes, num_layers, layer_height, output_nodes));
	}
}

Net& Trainer::train(int generations) {
	for (int i = 0; i < generations; i++) {
		score();
		sort(nets.begin(), nets.end());
		reproduce();
		cout << "Best score for generation " << i + 1 << ": " << nets[0].score << endl;
	}
	return nets[0];
}

void Trainer::score() {
	//for (size_t i = 0; i < nets.size(); i++) nets[i].reset();
	vector<float>* out;
	for (Net& net : nets) {
		net.reset();
		for (size_t i = 0; i < training_inputs.size(); i++) {
			out = net.eval(training_inputs[i]);
			for (size_t j = 0; j < training_outputs[i].size(); j++) {
				net.score += fabs((*out)[j] - training_outputs[i][j]);
			}
		}
		//cout << &net << endl;
	}
}

void Trainer::reproduce() {
	for (size_t i = survivors; i < nets.size(); i++) {
		nets[i] = nets[i%survivors];
		nets[i].mutate();
	}
}
