#include <iostream>

using namespace std;

int main() {
    float a=0, b=0, c=0;
    cout << "Please enter two numbers:" << endl;
    cin >> a >> "  " >> b >> endl;
    c = a+b;
    cout << "The sum of these two numbers is: " << c << endl;
    return 0;
}