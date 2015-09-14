#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <stdlib.h>

using namespace std;

int main(){
	system("curl http://espn.go.com/college-football/rankings/_/poll/1/week/2 > rankingsrawdata.txt");
    ifstream myinput("rankingsrawdata.txt");
    ofstream myoutput("rankings.json");
    string line;
    if(myinput.is_open()){
        int l=0;
        while(getline(myinput, line)){
            string ss = line.substr(0, 4);
            if(line.find("data:")!=string::npos){
                break;
            }
            l++;
        }
        myinput.close();
    }
    map<string, int> ranx;
    if(myoutput.is_open()){
        //
        while(line.find("rank")!=string::npos){
            //
            line = line.substr(line.find("\"rank\""));
            string ss1 = line.substr(7, line.find_first_of(",")-7);
            line = line.substr(line.find("\"name\""));
            string ss2 = line.substr(8, line.find_first_of(",")-9);
            //
            //if(line.find("rank")!=string::npos)
             //   myoutput<<", ";
            //myoutput<<"\n";
            ranx[ss2] = stoi(ss1);
        }
        //myoutput<<"\n]}\n";
        myoutput<<"{\"rankings\":[";
        for(map<string, int>::iterator i = ranx.begin(); i!=ranx.end(); i++){
            cout<<i->first<<": "<<i->second<<"\n";
            if(i!=ranx.begin())
                myoutput<<",";
            myoutput<<"\n\t{";
            myoutput<<"\"name\":\""<<i->first<<"\", \"ranking\":"<<i->second<<"}";
        }
        myoutput<<"\n]}";
        myoutput.close();
    }
}
