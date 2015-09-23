#include <iostream>

using namespace std;

int main() {
    double a, b;
    cout << "Please enter two numbers to divide, exit with a char:" << endl;
    while(cin >> a >> b) {
        try {
            if (b==0) throw 10;
            cout << a << "/" << b << " = " << a/b << endl;
        }
        catch (int e) {
            cout << "Cannot divide by 0, please enter valid numbers this time" << endl;
        }
    }
    return 0;
}