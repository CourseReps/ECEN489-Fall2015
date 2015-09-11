//
// Created by kaimen-walters on 9/10/15.
//



#ifndef CODE2_SPEEDDATA_H
#define CODE2_SPEEDDATA_H

#include <string>

using namespace std;

struct SpeedData {

    string scale;
    double speed;
    int year;
    SpeedData(double, string, int);
    //void spdconvert(double, string);


};


#endif //CODE2_SPEEDDATA_H
