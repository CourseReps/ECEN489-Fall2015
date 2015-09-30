#include <iostream>

using namespace std;

void pointswap(int *p, int *q)
{
    int temp = *p;
    *p = *q;
    *q = temp;
}

void refswap(int &x, int &y)
{
    int temp = x;
    x = y;
    y = temp;

}


int main() {
    int x;
    int y;
    int *p = &x;
    int *q = &y;

    cout << "Enter two integers: \n";
    cin >> x >>y;

    cout << "Before pointer swap: x = " << x << ", y = " << y << endl;

    pointswap(p, q);

    cout << "After pointer swap: x = " << x << ", y = " << y << endl;

    cout << "Before reference swap: x = " << x << ", y = " << y << endl;
    refswap(x, y);
    cout << "After reference swap: x = " << x << ", y = " << y << endl;


}