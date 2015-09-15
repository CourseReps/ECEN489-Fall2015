#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main() {
    string line;
    ifstream inFile("MarketVal.json");
    ofstream outFile("MarketVal.md");

   typedef vector<string>  Stock;
    Stock CName, CCode, CPrice, CChange, CHead;



    if (!inFile) {
        cout << "Unable to open file" << endl;
        exit(1);
    }




    if(inFile.is_open()) {
        //while(inFile.good())
        // {
        /*// Commented because it is redundant
        getline(inFile,line);
        string temp;
        cout << line << "\n";
        temp = line.substr(1,line.find(":")-2);
        CHead.push_back(temp); */


        while (getline(inFile, line)) {
            string temp;
            if (line.find("{\"")==0) {

                // size_t posN = line.find_first_of("{\"");
                if (line.find_first_of("{\"CompanyName\"") != string::npos) {
                    temp = line.substr(line.find(":\"") + 2, line.find_first_of(",") - line.find(":\"") - 3);
                    CName.push_back(temp);

                }
                line = line.erase(0,line.find(",")+1);
                //cout << line << "\n";
                if (line.find_first_of("\"StockMarketCode\"") != string::npos){
                    temp = line.substr(line.find(":\"") + 2, line.find_first_of(",")-line.find(":\"")-3);
                    //cout << temp << "\n";
                    CCode.push_back(temp);
                }
                line = line.erase(0,line.find(",")+1);
                //cout << line << "\n";
                if (line.find_first_of("\"StockPrice\"") != string::npos){
                    temp = line.substr(line.find(":\"") + 2, line.find_first_of(",")-line.find(":\"")-3);
                    //cout << temp << "\n";
                    CPrice.push_back(temp);
                }
                line = line.erase(0,line.find(",")+1);
                //cout << line << "\n";
                if (line.find_first_of("\"StockPriceChange\"") != string::npos){
                    temp = line.substr(line.find(":\"") + 2, line.find_first_of(",")-line.find(":\"")-4);
                    //cout << temp << "\n";
                    CChange.push_back(temp);
                }
            }

        }
    inFile.close();
    }

    if(outFile.is_open()){
        outFile << "#" << " StockMarket" <<"\n\n";
        outFile << "|" << " Company Name " << "|" << " Stock Code " << "|" << " Stock Price " << " | " << " Price Change " << "|";
        outFile << "\n";
        outFile <<  "| --------- | ----------- | ---------- | ----------- |";
        outFile << "\n";
        for(int i=0;i<CName.size();i++){
            outFile << "|"<< "**" << CName[i] << "**" <<"|" << CCode[i] << "|" << CPrice[i] << "|" << CChange[i] << "|";
            outFile << "\n";
        }

    }
    // Converting to Markdown


    return 0;
}
