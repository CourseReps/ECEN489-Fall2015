#include <iostream>

using namespace std;

struct SpeedData {
    string scale = "mph";
    double speed;
    int year;
};

int main() {
    SpeedData input;
    int s1=0, s2=0, s3=0;
    cout << "Type your speed and press Enter: " <<endl;
    cin >> s1;
    input.speed = s1;
    cout << "Your speed is " <<input.speed <<input.scale << endl;
    cout << "Your speed is " <<input.speed*1.609344 << "km/h" <<endl;

    cout <<"Speed Comparison: Type a speed and press Enter: " <<endl;
    cin >>s2;
    cout <<" Type another speed and press Enter: "<<endl;
    cin>>s3;
    cout << " The smallest of the two speeds is "<< min(s2,s3) <<input.scale<<endl;

    return 0;
}