
#include "NNWrapper/Net.h"
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
		invals.push_back(float(int(i * 10)) / 10);
		invals.push_back(float(int(i * 100)) / 100);
		outvals.push_back(float(int(i*i / 2.0 * 10)) / 10);
		outvals.push_back(float(int(i*i / 2.0 * 100)) / 100);
		input.push_back(invals);
		output.push_back(outvals);
	}

	Trainer trainer(2, 1, 20, 2, 50);
	trainer.training_inputs = input;
	trainer.training_outputs = output;
	ofstream outfile;
	outfile.open("out.csv", ios::out);
	Net out = trainer.train(100);
	for (float i = 0; i < 1; i += 0.01) {
		out.reset();
		invals.clear();
		invals.push_back(float(int(i * 10)) / 10.0);
		invals.push_back(float(int(i * 100)) / 100.0);
		outvals=*out.eval(invals);
		outfile << i << "," << outvals[0]+outvals[1] << endl;
	}
	cout << 5;
	outfile.close();
	out.print();
}