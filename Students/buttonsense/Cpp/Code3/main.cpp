#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define kpm 1.60934

using namespace std;


int main() {
	vector<double> miles;
	vector<double> kilo;
	string input;
	cout << "Please enter sequence of speeds in mph." << endl;
	while(getline(cin, input)){
		istringstream ss(input);
		double speed;
		while (ss >> speed) {
			miles.push_back(speed);
		}
		break;
	}

	cout << "Entered speeds in mph are: " << endl;

	for (int i=0; i < miles.size(); ++i) {
		cout << miles[i] << ' ';
	}
	cout << endl;
	cout << "There are " << miles.size() << " number of speeds in mph." << endl;
	for (const auto &j : miles) {
		kilo.push_back(j*kpm);
	}
	cout << "Entered speeds in kph are: " << endl;
	for (int i = 0; i < kilo.size(); ++i) {
		cout << kilo[i] << ' ';
	}
	cout << endl;
	cout << "There are " << kilo.size() << " number of speeds in kph." << endl;
	system("PAUSE");
	return 0;
}