/*
 * Anthony Ramirez
 * ECEN 489
 * Assignment 2
 */

#include "SpeedData.h"

using namespace std;


int main() {
    SpeedData data1, data2;
    string userinput;
    while (true) {
        cout << "Enter a speed in mph or km/h (ex: 7 mph, 15 km/h)" << endl;
        cin >> userinput;
        try {
            data1.speed = stoi(userinput);
            cin >> data1.scale;
        }
        catch (...) {
            cout << "Not a valid input" << endl;
        }
        if (data1.scale == "km/h")
            if (data2.scale == "km/h")
                cout << "The lowest speed is " << min(data1.speed, data2.speed) << " km/h." << endl;
            else
                cout << "The lowest speed is " << min(data1.speed, data2.speed * 1.60934) << " km/h." << endl;
        else
            if (data2.scale == "km/h")
                cout << "The lowest speed is " << min(data1.speed * 1.60934, data2.speed) << " km/h." << endl;
            else
                cout << "The lowest speed is " << min(data1.speed * 1.60934, data2.speed * 1.60934) << " km/h." << endl;

    }
}