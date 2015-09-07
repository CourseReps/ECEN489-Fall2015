#include <iostream>

using namespace std;

int main() {
    int a, b;
    bool valid;
    valid = false;
    while(!valid) {
        valid = true;
        cout << "Please enter two numbers" << endl;
        cin >> a, b;
        if(cin.fail()){
            cin.clear();
            cin.ignore();
            cout << "Please enter integers only." << endl;
            valid = false;
            continue;
        }
    }
    cout << "sum of " << a << b << "is " << a+b << "." << endl;
    return 0;
}