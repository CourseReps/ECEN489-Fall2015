#include <iostream>

using namespace std;

int main() {
    int a, b;
    cout << "Input two integer values:" << endl <<
            "First Value: " << endl;
    cin >> a;
    cout << "Second Value: " << b;

    int value1 = a;
    int value2 = b;

    int *first = value2;
    int *second = value1;

    cout << "First Value: " << *first << endl;
    cout << "Second Value: " << *second << endl;

    return 0;
}