#include <iostream>
#include <math.h>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

int mode(vector<double> list) {
	vector<int> elements;
	int big = 0;
	for (int i = 0; i < 11; i++) {
		elements.push_back(count(list.begin(), list.end(), i));
	}
	for (int i = 0; i < 11; i++) {
		if (elements[i] > big) {
			big = elements[i];
		}
	}
	return big;
}

int main() {
	vector<double> vec;
	ifstream ifile("numbers.txt", ios::in);
	if (!ifile.is_open()) {
		cerr << "The file does not exist." << endl;
		exit(1);
	}
	double num = 0.0;
	double sum = 0.0;
	double squaresum = 0.0;
	while (ifile >> num) {
		if (vec.size() == 50)
			break;
		else
			vec.push_back(num);
	}
	cout << "First 50 numbers of decimal expansion of e are: " << endl;
	for (int i = 0; i <= vec.size()-1; i++) {
		cout << vec[i] << ' ';
		sum = sum + vec[i];
	}

	double average = sum / vec.size();

	for (int i = 0; i <= vec.size() - 1; i++) {
		squaresum = squaresum + pow(vec[i] - average, 2);
	}
	
	double variance = squaresum / vec.size();

	cout << endl << "Arithmetic mean is: " << average << endl;
	cout << "Arithmetic Median is: " << vec[vec.size() / 2] << endl;
	cout << "Arithmetic Mode is: " << mode(vec) << endl;
	cout << "Unbaised sample variance is: " << variance << endl;

	system("PAUSE");
}