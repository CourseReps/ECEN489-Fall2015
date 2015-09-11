#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "matrixTT.h"
#include "hamming.h"

using namespace std;

int main(int argc, char* argv[]){
	if(argc<3){
		cout<<"Please enter input file and output file as 1st and 2nd args, respectively.\n";
		return 0;
	}
	string line;
	ifstream myfile (argv[1]);
	vector<vector<int> > codewords;
	if(myfile.is_open()){
		while(getline(myfile, line) ){
			for(int i=0; (i+6)<line.length(); i+=7){
				vector<int> currentCodeword;
				for(int j=0; j<7; j++){
					if(line[i+j]==48){
						currentCodeword.push_back(0);
					}
					else{
						currentCodeword.push_back(1);
					}
				}
				codewords.push_back(currentCodeword);
			}
		}
		myfile.close();
	}
	vector<vector<int> > decoded;
        for(int i=0; i<codewords.size(); i++){
		decoded.push_back(hamming::decode7Bits(codewords[i]));
	}
	ofstream myoutput(argv[2]);
	if(myoutput.is_open()){
		for(int i=0; i<decoded.size(); i++){
			for(int j=0; j<decoded[i].size(); j++){
				myoutput<<decoded[i][j];
			}
		}
		myoutput.close();
	}
}

