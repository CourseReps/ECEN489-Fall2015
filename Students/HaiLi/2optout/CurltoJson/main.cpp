#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
int main(){
    string url= "\"http://www.ittf.com/ittf_ranking/WR_Table_3_A2.asp?Month1=3&Year1=2015&Gender=M&Category=100M\"";
    string cmd="curl ";
    cmd.append(url);
    cmd.append(" >> curldata.txt");
    const char *cm=cmd.c_str();
    cout<<cm;
    system("curl www.baidu.com >curldata.txt");
    ifstream inputStream("curldata.txt");
    stringstream input;
    input << inputStream.rdbuf();
    string fileString=input.str();
    int m1=fileString.find("Ranking Pts.");
    int m2=fileString.find("2713")+4;
    int len=m2-m1;
    string subStr=fileString.substr(m1,len);
    int pos1 = 0;
    int pos2 = 0;
    vector<pair<string, string>> Pts;
    for(int i = 1;i < 7;i++)
    {
        int p=i;
        string result1;
        ostringstream convert;
        convert<<p;
        result1=convert.str();
        pos1=subStr.find(result1+"&nbsp;&nbsp;");
        subStr=subStr.substr(pos1,m2);
        pos2=subStr.find("&nbsp;</font></td>");
        string name=subStr.substr(264,pos2-268);
        string points=subStr.substr(pos2+359,4);
        pair<string,string> Points(name,points);
        Pts.push_back(Points);
    }
    ofstream fileOut("curldata.json");
    fileOut<<"\"athletes\":[\n";
    for(int i=0; i<Pts.size()-1; i++)
    {
        fileOut<<"\t"<<"{"<<"\""<<"Name"<<"\":"<<"\""<<Pts[i].first<<"\","<<"\"""Points"<<"\":"<<"\""<<Pts[i].second<<"\""<<"},"<<"\n";
        cout<<"\t\""<<Pts[i].first<<"\":"<<Pts[i].second<<",\n";
    }
    fileOut<<"\t"<<"{"<<"\""<<"Name"<<"\":"<<"\""<<Pts[Pts.size()-1].first<<"\","<<"\"""Points"<<"\":"<<"\""<<Pts[Pts.size()-1].second<<"\""<<"}"<<"\n";
    fileOut<<"]";
    return 0;
}
