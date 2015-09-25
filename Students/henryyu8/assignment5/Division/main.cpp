#include <iostream>

using namespace std;

int main() {
    double num, deno;
    try {
        cout << "Please type in the numerator: ";
        cin >> num;
        cout << "Next input the denominator: ";
        cin >> deno;
        if (deno == 0) throw 20;
        cout << "The division result will be: " << num/deno << '\n';
    }
    catch (int e)
    {
        cout << "Exception occurred: attempted to divide by zero. " << e << '\n';
    }
    return 0;
}