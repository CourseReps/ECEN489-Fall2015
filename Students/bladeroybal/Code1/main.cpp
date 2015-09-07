#include <iostream>

using namespace std;

int main() {
    float x=0, y=0, sum=0;
    cout << "Hello, User! Please type one number and press enter." << endl;
    cin >> x;
    cout << "Thank you. Please type another number and press enter." << endl;
    cin >> y;
    sum = x+y;
    cout << "Your sum is " << sum << endl;
    return 0;
}