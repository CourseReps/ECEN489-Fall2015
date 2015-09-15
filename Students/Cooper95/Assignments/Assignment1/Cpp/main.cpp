#include <iostream>

using namespace std;

int main() {
    string input1;
    int a, b;
    while(true)
    {
        cout << "Please enter two numbers to be summed, or enter 'quit'" << endl;
        cin >> input1;
        if(input1 == "quit")
            break;
        else{
            try{
                a = stoi(input1);
            }
            catch(...){
                cout << "Invalid input. ";
                continue;
            }
            cin >> b;
            cout << a << "+" << b << "=" << a+b << endl;
        }

    }
    return 0;
}