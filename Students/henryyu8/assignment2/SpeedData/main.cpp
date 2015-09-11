#include <iostream>

using namespace std;

struct SpeedData {
    string scale;
    double speed;
    int year;
} data1, data2;

void app1(SpeedData data) {
    if (data.scale == "mph") {
        cout << data.speed * 1.60934 << " " << "km/h" << " (" << data.year << ")" << endl;
    } else if (data.scale == "km/h") {
        cout << data.speed * 0.621371 << " " << "mph" << " (" << data.year << ")" << endl;
    }
}

void app2(SpeedData data1, SpeedData data2) {
    if ((data1.scale).compare(data2.scale)==0) {
        if (data1.speed >= data2.speed) {
            cout << data2.speed << " " << data2.scale<< " (" << data2.year << ")" << endl;
        } else {
            cout << data1.speed << " " << data2.scale << " (" << data1.year << ")" << endl;
        }
    } else if((data1.scale).compare(data2.scale)!=0) {
        if (data1.scale == "km/h") {
            if (data1.speed * 0.621371 >= data2.speed) cout << "The smallest of the two is " << data2.speed << " " << data2.scale << " (" << data2.year << ")" << endl;
            else cout << "The smallest of the two is " << data1.speed << " " << data1.scale << " (" << data1.year << ")" << endl;
        } else if (data1.scale == "mph") {
            if (data1.speed * 1.60934 >= data2.speed) cout << "The smallest of the two is " << data2.speed << " " << data2.scale << " (" << data2.year << ")" << endl;
            else cout << "The smallest of the two is " << data1.speed << " " << data1.scale << " (" << data1.year << ")" << endl;
        }
    }
}

int main() {
    int number;
    cout << "Which application you want to do? Select 1 or 2: ";
    cin >> number;
    if (number == 1){
        cout << "Enter speed: ";
        cin >> data1.speed;
        cout << "Enter scale: ";
        cin >> data1.scale;
        cout << "Enter year: ";
        cin >> data1.year;
        cout << "You have entered the speed data: " << endl;
        cout << data1.speed << " " << data1.scale << " (" << data1.year << ")" << endl;
        app1(data1);
    } else if (number == 2) {
        cout << "Enter speed 1: ";
        cin >> data1.speed;
        cout << "Enter scale 1: ";
        cin >> data1.scale;
        cout << "Enter year 1: ";
        cin >> data1.year;
        cout << "You have entered the speed data 2: " << endl;
        cout << data1.speed << " " << data1.scale << " (" << data1.year << ")" << endl;

        cout << "Enter speed 2: ";
        cin >> data2.speed;
        cout << "Enter scale 2: ";
        cin >> data2.scale;
        cout << "Enter year 2: ";
        cin >> data2.year;
        cout << "You have entered the speed data 2: " << endl;
        cout << data2.speed << " " << data2.scale << " (" << data2.year << ")" << endl;

        app2(data1,data2);
    }
    return 0;
}
