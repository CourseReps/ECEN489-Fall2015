#include <iostream>
using namespace std;
void swap (int *a, int *b){
	int x;
	x = *a;
	*a = *b;
	*b = x;
}
void swapref (int &a, int &b){
	int * x;
	int y;
	x = &a;
	y = a;
	*x = b;
	x = &b;
	*x = y;
}

int main(){
	int n1, n2;

	cout << "Please input an integer" << endl;
	cin >> n1;
	cout << "Please input a second integer" << endl;
	cin >> n2;
	swap(n1,n2);
	cout << "Your integers reversed are " << n1 << "  "<< n2 << endl;
	swapref(n1,n2);
	cout << "Your integers reversed are " << n1 << "  "<< n2 << endl;
	return 0;
}