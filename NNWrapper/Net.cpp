
#include "Net.h"
#include <iostream>
#include <iomanip>

using namespace std;

Net::Net(int num_input_nodes, int num_layers, int layer_height, int num_output_nodes) {
	input_nodes = num_input_nodes+1;
	layers.push_back(*new vector<Neuron>);
	for (int i = 0; i < layer_height; i++) {
		layers[0].push_back(*new Neuron(input_nodes));
	}
	for (int i = 1; i < num_layers; i++) {
		layers.push_back(*new vector<Neuron>);
		for (int j = 0; j < layer_height; j++) {
			layers[i].push_back(*new Neuron(layer_height));
		}
	}
	for (int i = 0; i < num_output_nodes; i++) {
		output_nodes.push_back(*new Neuron(layer_height));
	}
	output = new float[num_output_nodes];
}

Net::~Net() {
	layers.clear();
	output_nodes.clear();
	layers.shrink_to_fit();
	output_nodes.shrink_to_fit();
	free(output);
}

Net::Net(const Net& net) {
	input_nodes = net.input_nodes;
	score = net.score;
	for (vector<Neuron> v : net.layers) {
		layers.push_back(*new vector<Neuron>);
		for (Neuron& n : v) {
			layers[layers.size() - 1].push_back(*new Neuron(n));
		}
	}
	for (Neuron n : net.output_nodes) {
		output_nodes.push_back(*new Neuron(n));
	}
	if (output) free(output);
	output = new float[output_nodes.size()];
}

void Net::mutate() {
	for (size_t layer = 0; layer < layers.size(); layer++) {
		for (size_t neuron = 0; neuron < layers[layer].size(); neuron++) {
			layers[layer][neuron].mutate();
		}
	}
	for (Neuron& neuron : output_nodes) {
		neuron.mutate();
	}
}

void Net::reset() {
	score = 0;
	for (vector<Neuron>& layer : layers) {
		for (Neuron& neuron : layer) {
			neuron.reset();
		}
	}
	for (Neuron& neuron : output_nodes) {
		neuron.reset();
	}
}

float* Net::eval(float* inputs) {
	for (int i = 0; i < input_nodes-1; i++) {
		for (Neuron& neuron : layers[0]) {
			neuron.reset();
			neuron.send_input(i, inputs[i]);
		}
	}
	for (Neuron& neuron : layers[0]) {
		neuron.send_input(input_nodes-1, 1);
	}
	for (size_t i = 1; i < layers.size(); i++) {
		for (Neuron& neuron : layers[i]) {
			neuron.reset();
			for (size_t j = 0; j < layers[i].size(); j++) {
				neuron.send_input(j, layers[i - 1][j].get_value());
			}
		}
	}
	for (size_t i = 0; i < layers[0].size(); i++) {
		for (Neuron& neuron : output_nodes) {
			neuron.reset();
			neuron.send_input(i, layers[layers.size() - 1][i].get_value());
		}
	}
	for (size_t i = 0; i < output_nodes.size(); i++) {
		output[i] = output_nodes[i].get_value();
	}
	return output;
}

void Net::print() {
	cout.unsetf(ios::floatfield);
	cout.precision(3);
	for (vector<Neuron>& layer : layers) {
		cout << endl;
		for (Neuron& neuron : layer) {
			cout << setw(10) << neuron.get_value();
		}
	}
	cout << endl;
	for (Neuron& neuron : output_nodes) {
		cout << setw(10) << neuron.get_value();
	}
	cout << endl;
}

bool Net::operator<(Net& comp) {
	return score > comp.score;
}