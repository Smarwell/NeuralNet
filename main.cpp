
#include "Net.h"
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void main() {
	vector<vector<float>> input;
	vector<float> invals;
	vector<vector<float>> output;
	vector<float> outvals;
	for (float i = 0; i < 1; i += 0.01) {
		invals = *new vector<float>;
		outvals = *new vector<float>;
		invals.push_back(i);
		outvals.push_back(i*i);
		input.push_back(invals);
		output.push_back(outvals);
	}

	Trainer trainer(1, 5, 5, 1, 50);
	trainer.training_inputs = input;
	trainer.training_outputs = output;
	ofstream outfile;
	outfile.open("out.csv", ios::out);
	Net out = trainer.train(50);
	for (float i = 0; i < 1; i += 0.01) {
		out.reset();
		invals.clear();
		invals.push_back(i);
		outvals=*out.eval(invals);
		outfile << i << "," << outvals[0] << endl;
	}
	outfile.close();
	out.print();
}