//
// Created by kaimen-walters on 9/10/15.
//

#include <ostream>
#include <iostream>
#include "SpeedData.h"

SpeedData::SpeedData(double x, string y, int z)
{
    speed = x;
    scale = y;
    year = z;
    //void spdconvert(double x, string y) {}




/*void spdconvert(double x, string y) {
    double spdMph = 0;
    double spdkm = 0;
    const double mphKm = 1.60934;
    if (y.compare("mph")) {
        spdMph = x;
        spdkm = x * mphKm;
    }
    if (y.compare("km/h")) {
        spdkm = x;
        spdMph = x / mphKm;
    }
    cout << "Your speed in km/h is: " << spdkm
    endl;
    cout << "Your speed in mph is: " << spdMph
    endl;
    return 0;*/
}