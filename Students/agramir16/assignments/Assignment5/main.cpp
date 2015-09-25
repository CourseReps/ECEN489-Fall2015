#include <iostream>

using namespace std;

int main() {
    double a, b;
    cout << "Enter two numbers you wish you to divide by one another" << endl;
    cin >> a;
    while(cin >> b) {
        try {
            if (b==0) {
                throw 20;
            }
            cout << a << "/" << b << " = " << a/b << endl;
        }
        catch (int e) {
            cout << "Invalid input. Please try again" << endl;
        }
        cout << "Enter two numbers you wish you to divide by one another or enter 'exit'" << endl;
        cin >> a;
    }
    return 0;
}