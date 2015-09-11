#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;


string findString(string s){
	int openq = s.find_first_of("\"");
	s = s.substr(openq+1);
	s = s.substr(0, s.find_first_of("\""));
	return s;
}

string expect(string whole, string search){
	if(whole.find(search)!=string::npos){
		whole = whole.substr(whole.find(search)+search.length());
		cout<<"Got "<<search<<"\n";
	}
	else{
		cout<<"\n\nImproperly formatted JSON!\n\n";
		exit(0);
	}
	return whole;
}

vector<pair<string, int> > processJSON(string file){
	
	vector<pair<string,int> > ret;
	
	file = expect(file, "{");
	pair<string, int> title(findString(file), 0);
	ret.push_back(title);
	while(file.find("{")!=string::npos){
		file = expect(file, "{");
		file = expect(file, "\"name\":");
		string nam = findString(file);
		file = expect(file, ",");
		file = expect(file, "\"ranking\":");
		string nums = file.substr(0, file.find_first_of("}"));
		int num = stoi(nums);
	}

	return ret;
}


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
	string re = expect("{this", "p");
	cout<<re;
	vector<pair<string,int> > info = processJSON(totalfile);
	//cout<<totalfile;
}
