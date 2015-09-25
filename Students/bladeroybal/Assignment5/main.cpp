#include <iostream>

using namespace std;

int main() {
    float x=0;
    float y=0;
    float ans=0;
    cout << "Hello, User! Enter two numbers, hitting enter after each one" << endl;
    while (true) {
    cin >> x >> y;
    try {
        ans = x / y;
        if (y == 0) {
            throw 0;
        }
        else {
            cout << "Your answer is " << ans << endl;
            break;
        }
    }
    catch (int e) {
        cout << "An exception has occurred. Divide by 0. Please enter new numbers"<<endl;
    }
}

    return 0;
}