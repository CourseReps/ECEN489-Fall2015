/*
 * Anthony Ramirez
 * ECEN 489
 * Assignment 2
 */

#include "SpeedData.h"

using namespace std;

int main() {
    SpeedData data1;
    string userinput;
    while (true) {
        cout << "Enter a speed in mph or km/h (ex: 7 mph, 15 km/h)" << endl;
        cin >> userinput;
        try{
            data1.speed = stoi(userinput);
            cin >> data1.scale;
        }
        catch(...) {
            cout << "Not a valid input" << endl;
        }
        if (data1.scale == "km/h")
            cout << data1.speed << data1.scale << endl
                    << data1.speed * 0.621371 << " mph" << endl;
        else
            cout << data1.speed << data1.scale << endl
                    << data1.speed * 1.60934 << " km/h" << endl;

    }
    return 0;
}