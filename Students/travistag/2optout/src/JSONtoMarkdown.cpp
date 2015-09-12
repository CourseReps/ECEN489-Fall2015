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
		//cout<<"Got "<<search<<"\n";
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
		file = expect(file, "}");
		int num = stoi(nums);
		pair<string, int> curr(nam, num);
		ret.push_back(curr);
	}

	return ret;
}

void outputToMarkdown(string filename, vector<pair<string, int> > inf){
	ofstream mdoutput(filename);
	if(mdoutput.is_open()){
		mdoutput<<"# "<<inf[0].first<<"\n\n";
		for(int i=1; i<inf.size(); i++){
			mdoutput<<"**"<<inf[i].first<<":** \t"<<inf[i].second<<"\n\n";
		}
	}
	else{
		cout<<"Could not open file\n";
	}
}


int main(int argc, char* argv[]){
	if(argc<3){
		cout<<"Please enter an input and output file as an argument\n";
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
	//string re = expect("{this", "{");
	//cout<<re;
	vector<pair<string,int> > info = processJSON(totalfile);
	cout<<info[0].first<<"\n\n";
	for(int i=1; i<info.size(); i++){
		cout<<info[i].first<<": "<<info[i].second<<"\n";
	}
	outputToMarkdown(argv[2], info);
	//cout<<totalfile;
}
