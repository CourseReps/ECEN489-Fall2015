#include <iostream>
#include <stdlib.h>

using namespace std;

void swap(int *x, int *y);
void swapRef(int &x, int &y);

int main(int argc, char *argv[]) {
    if (argc != 3)
        cout << "Please use two interegers to swap as arguments" << endl;
    else{
        int in1, in2;
        in1 = (int)strtol(argv[1], NULL, 10);
        in2 = (int)strtol(argv[2], NULL, 10);
        swap(&in1, &in2);
        swapRef(in1, in2);
    }
    return 0;
}

void swap(int *x, int *y){
    cout << "Swap by pointer:" << endl;
    int *temp;
    temp = x;
    x = y;
    y = temp;
    cout << *x << " " << *y << endl;
}

void swapRef(int &x, int &y){
    cout << "Swap by reference:" << endl;
    int temp;
    temp = x;
    x = y;
    y = temp;
    cout << x << " " << y << endl;
}