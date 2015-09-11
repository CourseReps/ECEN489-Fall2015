//Cooper Rehn
//ECEN 489 Assignment 2, Application 2
//9-10-15

#include "SpeedData.h"

int main(){
    SpeedData data1, data2;
    string firstin;
    double kpm = 1.60934;
    while (true) {
        cout << "Please enter 2 speeds in the format 'x mph' or 'x km/h' or 'quit'" << endl;
        cin >> firstin;
        if (firstin == "quit")
            break;
        try{
            data1.speed = stoi(firstin);
            cin >> data1.scale >> data2.speed >> data2.scale;
        }
        catch(...){
            cout << "Not a valid entry" << endl;
            continue;
        }
        if (data1.scale == "mph")
            if (data2.scale == "mph")
                cout << min(data1.speed, data2.speed) << " mph is the lowest speed" << endl;
            else
                cout << min(data1.speed, data2.speed/kpm) << " mph is the lowest speed" << endl;
        else
            if (data2.scale == "mph")
                cout << min(data1.speed, data2.speed*kpm) << " km/h is the lowest speed" << endl;
            else
                cout << min(data1.speed, data2.speed) << " km/h is the lowest speed" << endl;
    }

    return 0;
}


