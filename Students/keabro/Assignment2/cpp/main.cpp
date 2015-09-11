#include <iostream>

using namespace std;

struct SpeedData {
    string scale = " mph";
    double speed;
    int year = 2015;
};

int main() {
    SpeedData input;
    int s1=0, s2=0, s3=0;
    cout << "please input a speed " << endl;
    cin >> s1;
    input.speed = s1;
    cout << "your speed is "<< input.speed << input.scale << endl;

    cout << " please enter two speeds to compare" << endl;
    cin >> s2 >> s3;
    cout << "The largest of the two is " << max(s2,s3) << endl;

    return 0;
}