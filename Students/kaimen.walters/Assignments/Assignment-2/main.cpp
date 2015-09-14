#include <iostream>
#include <vector>
#include "SpeedData.h"

using namespace std;

int main() {
    double speed;
    string scale;
    int year;
    bool run = true;

    cout << "How fast is your car?";
    cin >> speed;

    while(run) {
       cout << "Is that in mph or km/h? ";
        cin >> scale;
        //cout << scale;
        if (scale.compare("mph") && scale.compare("km/h"))
            cerr << "That input is not valid. Try again. ";
        else
            break;
    }

    cout << "What year is your car? ";
    cin >> year;

    SpeedData car(speed, scale, year);
    //cout << car.speed << car.scale << car.year;

    double speedmph = 0;
    double speedkm = 0;
    const double mphtokm = 1.60934;
    //cout << " compare" << car.scale.compare("mph");
    if(car.scale.compare("km/h")){
        speedmph = car.speed;
        speedkm = car.speed*mphtokm;
        cout << "Your speed is " << speedmph << "mph" << endl;
        cout << "and " << speedkm << "km/h.";
    }
    if(car.scale.compare("mph")){
        speedkm = car.speed;
        speedmph = car.speed/mphtokm;
        cout << "Your speed is " << speedmph << "mph" << endl;
        cout << "and " << speedkm << "km/h.";
    }

}
