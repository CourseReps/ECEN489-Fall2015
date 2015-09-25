#include <iostream>

using namespace std;

double div(int num1, int num2)
{
	if(num2 == 0)
		throw 99;
	return num1/(double)num2;
}

int main()
{
	int num1, num2;
	int retryflag; 
	double val = 0;
	do{
		retryflag = 0;
		cout << "Enter a number: ";
		cin >> num1;
		cout << "Enter a second number: ";
		cin >> num2;
		try{
			val = div(num1, num2);
		}catch(int e){
			cerr << "Divide by zero exception!\n";
			cerr << "Enter new numbers...\n";
			retryflag = 1;
		}
	}while(retryflag != 0);
	cout << "#1 / #2 = " << val << endl;
}
