#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

int main() {
    system("curl http://finance.yahoo.com/currency-investing > curlData.txt");
    ifstream inputStream("curlData.txt");
    stringstream s;
    s << inputStream.rdbuf();
    string fileString = s.str();

    int idx = fileString.find("Cross Rates Currency Table");
    int idx2 = fileString.find("yfi-news-commentary");
    int len = idx2-idx;
    string subStr = fileString.substr(idx,len);
    int pos = 0;
    vector< pair<string, string>> exchangeRates;

    pos = subStr.find("row1");

    for(int j=3; j <=8; j++ ) {
        pos = subStr.find("col" + to_string(j), pos);
        pos = subStr.find("s=", pos);
        string currencyType = subStr.substr(pos+5,3);
        pos = subStr.find("X\">", pos);
        string val1 = subStr.substr(pos + 3, subStr.find("<", pos) - pos - 3);
        val1.erase(std::remove(val1.begin(), val1.end(), ' '), val1.end());
        val1.erase(std::remove(val1.begin(), val1.end(), '\t'), val1.end());
        pair<string, string> rate(currencyType, val1);
        exchangeRates.push_back(rate);
    }

    ofstream fileOut("curlData.json");
    fileOut<<"\"USD Exchange Rates \":{\n";
    for(int i=0; i<exchangeRates.size()-1; i++)
    {
        fileOut<<"\t\""<<exchangeRates[i].first<<"\":"<<exchangeRates[i].second<<",\n";
    }
    fileOut<<"\t\""<<exchangeRates[exchangeRates.size()-1].first<<"\":"<<exchangeRates[exchangeRates.size()-1].second<<"\n";
    fileOut<<"}";

    return 0;
}