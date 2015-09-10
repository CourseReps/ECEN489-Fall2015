#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]){
	if(argc<2){
		cout<<"Please enter an input file as an argument\n";
		return 0;
	}

	ifstream myinput(argv[1]);
	string line;
	string totalfile = "";
	if(myinput.is_open()){
		while(getline(myinput, line)){
			totalfile+=line;
		}

		myinput.close();
	}
	cout<<totalfile;
}