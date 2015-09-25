#include <iostream>
#include <stdio.h>

using namespace std;
int divide_num(int num1, int num2){
    if(num2==0)
        throw 1;
    return num1/num2;
}

int main() {
    int num1, num2;
    cout << "Please input integer 1: " << endl;
    cin >> num1;
    cout << "Please input integer 2: " << endl;
    cin >> num2;
    try {
        cout << divide_num(num1, num2);

    }
    catch (int &code) {
        cout << "Cannot Divide by zero. ";
    }
    catch (...) {
        cout << "An unknown error has occurred.";
    }
    return 0;
}