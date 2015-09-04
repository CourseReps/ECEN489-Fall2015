#include <iostream>
#include <string>
#include <sstream>

using namespace std;

int main()
{
    string input;
    int Num1, Num2;
    Num1 = 0;
    Num2 = 0;

    while(true) {
        cout << "Please input first number: ";
        getline(cin, input);
        stringstream myStream(input);
        if (myStream >> Num1)
            break;
        cout << "Invalid number, please try again" << endl;
    }
    while(true){
        cout << "Please input second number: ";
        getline(cin, input);
        stringstream myStream(input);
        if (myStream >> Num2)
            break;
        cout << "Invalid number, please try again" << endl;
    }
    cout << "Your numbers are " << Num1 << " and " << Num2 << endl;
    return 0;
}