#include <iostream>

using namespace std;

int main() {
    int *num1;
    int *num2;
    int n1,n2,n3,n4;
    int& r1 = n1;
    int& r2 = n2;
    cout << "Please input integer 1: " << endl;
    cin >> n1;
    cout << "Please input integer 2: " << endl;
    cin >> n2;

    num1 = &n2;
    num2 = &n1;

    n3 = *num1;
    n4 = *num2;

    cout << "Swapping Integers . . ." << endl;
    cout << "Integer 1: " << n3 << endl;
    cout << "Integer 2: " << n4 << endl;

    cout << "Swapping References . . ." << endl;
    cout << "Integer 1: " << r2 << endl;
    cout << "Integer 2: " << r1 << endl;
    return 0;
}