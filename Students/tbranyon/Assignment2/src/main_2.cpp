#include <iostream>

using namespace std;

int main()
{
	double speed1, speed2;
	cout << "Enter speed 1: ";
	cin >> speed1;
	cout << "Enter speed 2: ";
	cin >> speed2;
	double result = (speed1 > speed2) ? speed1 : speed2;
	cout << "The larger speed is: " << result << endl;
	return 0;
}
