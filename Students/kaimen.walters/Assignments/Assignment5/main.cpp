#include <iostream>

using namespace std;

int main()
{
    char run = 'y';
    const int divideByZero = 10;

    while (run == 'y') {
        try {
            int x, y = 0;
            cout << "Enter two integers: " << endl;
            cin >> x >> y;
                if(y == 0) {
                    throw divideByZero;
                }
            cout << x / y << endl;
            cout << "Do you want to go again? 'y' or 'n': " << endl;
            cin >> run;
        }
            catch(const int divideByZero){
                cout << "Your second integer can not be zero. Please enter two more integers."<< endl;
            }
    }

    return 0;
}