#include <iostream>
#include <stdio.h>

using namespace std;

inline int myexception(int numerator, int denominator) {
	if (denominator == 0)
		throw exception("cannot divide by zero");
	return numerator / denominator;
}

int main() {
	int num1, num2;
	int i = 0;
	cout << "Please input two integers: " << endl;
	cin >> num1 >> num2;
	try {
		i = myexception(num1, num2);
		cout << "first number divided by the second number is: " << i << endl;
	}
	catch (exception e) {
		cout << e.what() << endl;
	}
	catch (...) {
		cout << "Invalid entry";
	}
	system("PAUSE");
	return 0;
}