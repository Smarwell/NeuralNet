#pragma once

#include "Net.h"

bool net_cmp(Net* n1, Net* n2) {
	return n1->score > n2->score;
}

ostream& operator<<(ostream& out, Neuron* n) {
	out << n->weights.size() << endl;
	for (size_t i = 0; i < n->weights.size(); i++) {
		out << n->weights[i] << " ";
	}
	out << endl;
	return out;
}

ostream& operator<<(ostream& out, vector<Neuron*>* list) {
	out << list->size() << endl;
	for (size_t i = 0; i < list->size(); i++) {
		out << (*list)[i];
	}
	return out;
}

ostream& operator<<(ostream& out, vector<vector<Neuron*>*>& list) {
	out << list.size() << endl;
	for (size_t i = 0; i < list.size(); i++) {
		out << list[i];
	}
	return out;
}

ostream& operator<<(ostream& out, Net& net) {
	out << net.input_nodes << endl;
	out << net.layers;
	out << &net.output_nodes << endl;
	return out;
}

void save(Net* n, bool visualize) {
	char fname[16];
	sprintf(fname, "net%d.n\0", (rand() % 1000));
	ofstream file;
	file.open(fname, ios::out);
	file << *n;
	file.close();
	cout << "Net saved at " << fname << endl;
	if (visualize) {
		stringstream comm;
		comm << "python visualize.py " << fname;
		cout << "Visualization exit status: " << system(comm.str().c_str()) << endl;
	}
}


