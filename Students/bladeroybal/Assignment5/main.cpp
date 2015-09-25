#include <iostream>

using namespace std;

int main() {
    int x=0;
    int y=0;
    int ans=0;
    cout << "Hello, User! Enter two numbers, hitting enter after each one" << endl;
    while (ans == 0) {
    cin >> x >> y;
    try {
        ans = x / y;
        if (ans == 0) {
            throw 0;
        }
        else {
            cout << "Your answer is " << ans << endl;
        }
    }
    catch (int e) {
        cout << "An exception has occurred. The answer provided was 0. Please enter new numbers"<<endl;
    }
}

    return 0;
}