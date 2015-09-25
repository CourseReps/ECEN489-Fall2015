#include <string>
#include <iostream>
using namespace std;
int main(){
	try{
	double n1;
	double n2;
	double n3;
	cout << "please input a number" << endl;
	cin >> n1;
	cout << "please input a second number" << endl;
	cin >> n2;
	if(n2==0)
		throw 1;
	else{
		n3 = float(n1/n2);
		cout << n1 << " divided by " << n2 << " = " << n3;
		return 0;
	}
	}
	catch(int err_code){
		cout << "Sorry you cant divide by zero" << endl;
	}
	return -1;
}
