#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

int main() {
    ifstream jsonfile("curldata.json");
    stringstream input;
    input << jsonfile.rdbuf();
    string fileString = input.str();
    cout << fileString<<endl;
    int pos=fileString.find("\n");
    int pos1=fileString.find("]");
    string header=fileString.substr(1,pos-4);
    string sub=fileString.substr(pos,pos1-pos);
    vector<pair<string, string>> Pts;
    for (int i=1;i<7;i++)
    {
        int pos2=sub.find("Name");
        int pos3=sub.find("Points");
        string name=sub.substr(pos2+7,pos3-pos2-10);
        string points=sub.substr(pos3+9,4);
        pair<string,string>Points(name,points);
        Pts.push_back(Points);
        sub=sub.substr(pos3+1,pos1-pos3);
    }
    ofstream fileout("markdown.MD");
    fileout<<"#"<<header<<endl<<endl;
    fileout<<"* "<<"Name"<<",   "<<"Points"<<endl;
    cout<<"#"<<header<<endl<<endl;
    cout<<"* "<<"Name"<<",   "<<"Points"<<endl;
    for (int i=0;i<Pts.size();i++)
    {
        fileout<<"* "<<Pts[i].first<<",   "<<Pts[i].second<<endl;
        cout<<"* "<<Pts[i].first<<",   "<<Pts[i].second<<endl;
    }
    return 0;
}