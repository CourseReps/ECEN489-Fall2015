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
			cout<<"line length: "<<line.length()<<"\n";
			for(int i=0; (i+3)<line.length(); i+=4){
				vector<int> currentCodeword;
				for(int j=0; j<4; j++){
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
	matrixTT::print2DArray(codewords);
	vector<vector<int> > encoded;
        for(int i=0; i<codewords.size(); i++){
		encoded.push_back(hamming::encode4Bits(codewords[i]));
	}
	matrixTT::print2DArray(encoded);
	ofstream myoutput(argv[2]);
	if(myoutput.is_open()){
		for(int i=0; i<encoded.size(); i++){
			for(int j=0; j<encoded[i].size(); j++){
				myoutput<<encoded[i][j];
			}
		}
		myoutput.close();
	}
}

