#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
    string input;
    int Num1 = 0;
    int Num2 = 0;
    try {
        cout << "Please input first number: ";
        cin >> Num1;
        if (std::cin.fail()) throw std::runtime_error("Input is not an integer\n");
        cout << "Please input second number: ";
        cin >> Num2;
        if (std::cin.fail()) throw std::runtime_error("Input is not an integer\n");

    } catch (const std::runtime_error& e) {
        std::cout << e.what();
        return 1;
    }
    cout << "The sum of your numbers: " << Num1+Num2<< endl;
    return 0;
}
