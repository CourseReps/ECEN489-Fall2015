//Cooper Rehn
//ECEN 489 Assignment 2
//9-10-15
#include "SpeedData.h"

using namespace std;

int main() {
    SpeedData data;
    string firstin;
    double kpm = 1.60934;
    while (true) {
        cout << "Please enter a speed in the format 'x mph' or 'x km/h' or 'quit'" << endl;
        cin >> firstin;
        if (firstin == "quit")
            break;
        try{
            data.speed = stoi(firstin);
            cin >> data.scale;
        }
        catch(...){
            cout << "Not a valid entry" << endl;
            continue;
        }
        if (data.scale == "mph")
            cout << data.speed << " " << data.scale << " | " << data.speed * kpm << " km/h" << endl;
            //puggypug
        else
            cout << data.speed << " " << data.scale << " | " << data.speed / kpm << " mph" << endl;
    }

    return 0;
}