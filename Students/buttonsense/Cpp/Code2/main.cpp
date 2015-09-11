#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#define kpm 1.60934

struct SpeedData {
	string scale;
	double speed;
	int year;
	SpeedData() : scale("mph") {}
};

void printformat(SpeedData sample) {
	if (sample.scale == "mph") {
		cout << "The speed is: " << sample.speed << " mph." << endl;
		cout << sample.speed * kpm << " kph." << endl;
	}
	else {
		cout << "The speed is: " << sample.speed / kpm << " mph" << " or ";
		cout << sample.speed << " kph." << endl;
	}

}

void compare(SpeedData sample1, SpeedData sample2) {
	if (sample1.speed > sample2.speed) {
		cout << "second speed is slower." << endl;
		printformat(sample2);
	}
	else {
		cout << "first speed is slower." << endl;
		printformat(sample1);
	}
}

int main() {
	string input1, input2;
	string::size_type sp;

	SpeedData data1;
	SpeedData data2;

	cout << "Please enter a speed and scale(mph or kph)" << endl;
	cin >> input1;
	data1.speed = stoi(input1, &sp);
	data1.scale = input1.substr(sp);
	printformat(data1);

	cout << "Please enter first speed in mph or kph for comparison" << endl;
	cin >> input1;
	data1.speed = stoi(input1, &sp);
	data1.scale = input1.substr(sp);
	cout << "enter the second speed" << endl;
	cin >> input2;
	data2.speed = stoi(input1, &sp);
	data2.scale = input1.substr(sp);
	compare(data1, data2);

	return 0;
}
