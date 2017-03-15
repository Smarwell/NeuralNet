#pragma once

#include "Net.h"

Net::Net(int num_input_nodes, int num_layers, int layer_height, int num_output_nodes) {
	input_nodes = num_input_nodes + 1;
	layers.push_back(new vector<Neuron*>);
	for (int i = 0; i < layer_height; i++) {
		layers[0]->push_back(new Neuron(input_nodes));
	}
	for (int i = 1; i < num_layers; i++) {
		layers.push_back(new vector<Neuron*>);
		for (int j = 0; j < layer_height; j++) {
			layers[i]->push_back(new Neuron(layer_height));
		}
	}
	for (int i = 0; i < num_output_nodes; i++) {
		output_nodes.push_back(new Neuron(layer_height));
	}
	output = new float[num_output_nodes];
}




Net::~Net() {
	for (size_t l = 0; l < layers.size(); l++) {
		for (size_t n = 0; n < layers[l]->size(); n++) {
			delete layers[l]->at(n);
		}
	}
	for (size_t l = 0; l < layers.size(); l++) {
		delete layers[l];
	}
	layers.clear();

	for (size_t n = 0; n < output_nodes.size(); n++) {
		delete output_nodes[n];
	}
	output_nodes.clear();
	layers.shrink_to_fit();
	output_nodes.shrink_to_fit();
	delete[] output;
}




Net::Net(const Net& net) {
	input_nodes = net.input_nodes;
	score = net.score;
	for (vector<Neuron*>* v : net.layers) {
		layers.push_back(new vector<Neuron*>);
		for (Neuron* n : *v) {
			layers[layers.size() - 1]->push_back(new Neuron(*n));
		}
	}
	for (Neuron* n : net.output_nodes) {
		output_nodes.push_back(new Neuron(*n));
	}
	if (output) delete[] output;
	output = new float[output_nodes.size()];
}



Net::Net(int id) {
	stringstream filename;
	filename << "net" << id << ".n";
	ifstream file;
	file.open(filename.str(), ios::in);
	if (!file.is_open()) {
		char message[64];
		sprintf(message, "Failed to open net%d.n\0", id);
		throw exception(message);
	}
	int num_layers;
	int layer_height;
	int num_weights;
	int num_output_nodes;
	float weight;
	vector<float> weights;
	file >> input_nodes;
	file >> num_layers;
	for (int i = 0; i < num_layers; i++) {
		layers.push_back(new vector<Neuron*>());
		file >> layer_height;
		for (int n = 0; n < layer_height; n++) {
			file >> num_weights;
			weights.clear();
			for (int w = 0; w < num_weights; w++) {
				file >> weight;
				weights.push_back(weight);
			}
			layers[i]->push_back(new Neuron(&weights));
		}
	}
	file >> num_output_nodes;
	for (int n = 0; n < num_output_nodes; n++) {
		weights.clear();
		file >> num_weights;
		for (int w = 0; w < num_weights; w++) {
			file >> weight;
			weights.push_back(weight);
		}
		output_nodes.push_back(new Neuron(&weights));
	}
	output = new float[num_output_nodes];
	score = 0;
}



void Net::mutate() {
	for (size_t layer = 0; layer < layers.size(); layer++) {
		for (size_t neuron = 0; neuron < layers[layer]->size(); neuron++) {
			layers[layer]->at(neuron)->mutate();
		}
	}
	for (Neuron* neuron : output_nodes) {
		neuron->mutate();
	}
}




void Net::reset() {
	score = 0;
	for (size_t l = 0; l < layers.size(); l++) {
		for (size_t n = 0; n < layers[l]->size(); n++) {
			layers[l]->at(n)->reset();
		}
	}
	for (Neuron* neuron : output_nodes) {
		neuron->reset();
	}
}




float* Net::eval(float* inputs) {
	for (Neuron* neuron : *layers[0]) {
		neuron->reset();
	}
	for (int i = 0; i < input_nodes - 1; i++) {
		for (Neuron* neuron : *layers[0]) {
			neuron->send_input(i, inputs[i]);
		}
	}
	for (Neuron* neuron : *layers[0]) {
		neuron->send_input(input_nodes - 1, 1);
	}
	for (size_t i = 1; i < layers.size(); i++) {
		for (Neuron* neuron : *layers[i]) {
			neuron->reset();
			for (size_t j = 0; j < layers[i]->size(); j++) {
				neuron->send_input(j, (*layers[i - 1])[j]->get_value());
			}
		}
	}
	for (Neuron* neuron : output_nodes) {
		neuron->reset();
	}
	for (size_t i = 0; i < layers[0]->size(); i++) {
		for (Neuron* neuron : output_nodes) {
			neuron->send_input(i, (*layers[layers.size() - 1])[i]->get_value());
		}
	}
	for (size_t i = 0; i < output_nodes.size(); i++) {
		output[i] = output_nodes[i]->get_value();
	}
	return output;
}
