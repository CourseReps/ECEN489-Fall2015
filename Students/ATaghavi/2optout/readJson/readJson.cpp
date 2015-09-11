#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

int main() {
    ifstream inputStream("../getcurlData/curlData.json");
    stringstream s;
    s << inputStream.rdbuf();
    string fileString = s.str();

    string headerString;
    vector<pair<string,string>> data;

    headerString = fileString.substr(1, fileString.find("\"",1)-1);
    int pos = fileString.find("{");
    while( true )
    {
        pair<string, string> dataPair;
        pos = fileString.find("\"",pos);
        string key = fileString.substr(pos+1, fileString.find("\"",pos+1)-pos-1);
        dataPair.first = key;
        pos = fileString.find(":",pos);
        string value;
        if(fileString.find(",",pos) > 0 && fileString.find(",",pos) < fileString.length()) {
            value = fileString.substr(pos+1, fileString.find(",", pos) - pos - 1);
            dataPair.second = value;
            data.push_back(dataPair);
        }
        else {
            value = fileString.substr(pos+1, fileString.find("}", pos) - pos - 1);
            dataPair.second = value;
            data.push_back(dataPair);
            break;
        }
    }

    ofstream outFile("curlData.md");
    outFile<<"#"<<headerString<<"\n\n";
    outFile<<"| Currency    | Exchange Rate |"<<"\n";
    outFile<<"| --------------| -------------- |\n";
    for(int i=0; i<data.size(); i++) {
        outFile<<"|"<<data[i].first<<"        |"<<data[i].second<<" |\n";
    }
    return 0;
}
