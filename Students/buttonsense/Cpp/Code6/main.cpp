#include <iostream>

using namespace std;


void swap_pointer(int *Pnum1, int *Pnum2) {
    int temp;
    temp = *Pnum1;
    *Pnum1 = *Pnum2;
    *Pnum2 = temp;
}

void swap_reference(int &a, int &b) {
    int temp;
    temp = a;
    a = b;
    b = temp;
}

int main() {
    int num1, num2;

    cout << "Please enter two integers seperated by a white space." << endl;
    cin >> num1 >> num2;
    int Pnum1 = num1;
    int Pnum2 = num2;

    cout << "Swapped by pointers" << endl;
    swap_pointer(&Pnum1, &Pnum2);
    cout << Pnum1 << ' ' << Pnum2 << endl;
    cout << "Swapped by reference" << endl;
    int Rnum1 = num1;
    int Rnum2 = num2;
    swap_reference(Rnum1, Rnum2);
    cout << Rnum1 << ' ' << Rnum2 << endl;
    //system("PAUSE");
    return 0;
}
