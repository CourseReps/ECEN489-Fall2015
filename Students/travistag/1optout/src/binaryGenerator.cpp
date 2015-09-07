#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int main(int argc, char* argv[]){
	if(argc<2){
		cout<<"Enter an output file as arg\n";
		return 0;
	}
	int cws=0;
	cout<<"Enter a number of 4 bit words to generate: ";
	cin>>cws;
	ofstream myoutput(argv[1]);
	if(myoutput.is_open()){
		for(int i=0; i<cws*4; i++){
			int rn = rand()%2;
			myoutput<<rn;	
		}
		myoutput.close();
	}
}
