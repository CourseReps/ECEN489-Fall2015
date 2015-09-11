/*
 * Anthony Ramirez
 * ECEN 489
 * Assignment 2
 */

#include <stdlib.h>
#include <string>
#include "SpeedData.h"

using namespace std;

int main() {
    SpeedData data1;
    string userinput;
    while (true) {
        cout << "Enter a speed in mph or km/h (ex: 7 mph, 15 km/h)" << endl;
        cin >> userinput;
        if (data1.scale == "km/h")
            cout << data1.speed << data1.scale << endl
                    << data1.speed * 0.621371 << " mph" << endl;
        else
            cout << data1.speed << data1.scale << endl
                    << data1.speed * 1.60934 << " km/h" << endl;

    }
    return 0;
}