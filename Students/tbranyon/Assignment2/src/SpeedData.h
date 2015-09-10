#ifndef _SPEEDDATA_H_
#define _SPEEDDATA_H_

#include <string>

using namespace std;

struct SpeedData
{
	string scale;
	double speed;
	int year;
	SpeedData(string, double, int);
};

#endif
