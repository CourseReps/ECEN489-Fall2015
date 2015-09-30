#include<iostream>

using namespace std;

void swap_pointer (int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void swap_reference (int &a, int &b) {
    int temp;
    temp = a;
    a = b;
    b = temp;
}

int main() {
    int a, b, c;
    cout << "\nPlease select if you want to swap numbers by pointers (1) or by reference (2): 1 or 2? ";
    cin >> c;
    if (c == 1) {
        cout << "\nYou select swapping by pointers.\n";
        cout << "\nEnter the first number: ";
        cin >> a;
        cout << "\nEnter the second number: ";
        cin >> b;
        swap_pointer(&a, &b);
        cout << "\nAfter swapping: ";
        cout << "\n\nFirst number: " << a;
        cout << "\n\nSecond number: " << b;
    } else if (c == 2) {
        cout << "\nYou select swapping by references.\n";
        cout << "\nEnter the first number: ";
        cin >> a;
        cout << "\nEnter the second number: ";
        cin >> b;
        swap_reference(a, b);
        cout << "\nAfter swapping: ";
        cout << "\n\nFirst number: " << a;
        cout << "\n\nSecond number: " << b;
    } else {
        cerr << "Invalid input";
    }

    return 1;
}