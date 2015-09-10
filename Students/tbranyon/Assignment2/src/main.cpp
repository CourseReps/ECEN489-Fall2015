#include "SpeedData.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	string scale;
	double speed;
	int year;
	while(1)
	{
		cout << "Enter speed scale [mph / km/h]: ";
		cin >> scale;
		if(scale.compare("mph") && scale.compare("km/h"))
			cerr << "Error, invalid input for scale\n";
		else
			break;
	}
	while(1)
	{
		if(!scale.compare("mph"))
			cout << "Enter speed in mph: ";
		else
			cout << "Enter speed in km/h: ";
		cin >> speed;
		if(speed < 0)
			cerr << "Error, please enter a positive number for speed\n";
		else
			break;
	}
	cout << "Enter year: ";
	cin >> year;
	SpeedData container = SpeedData(scale,speed,year);
	if(!container.scale.compare("mph"))
	{
		double altspeed = speed / 0.62;
		cout << "Speed: " << speed << " mph, " << altspeed << " km/h\n";
	}
	else
	{
		double altspeed = speed * 0.62;
		cout << "Speed: " << speed << " km/h, " << altspeed << " mph\n";
	}
}
