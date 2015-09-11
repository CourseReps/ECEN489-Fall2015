#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main(){
    system("curl http://money.cnn.com/data/hotstocks/index.html > stockMar.txt");
    ifstream myinput("stockMar.txt");
    ofstream myoutput("MarketVal.json");
    string line;
    typedef vector<string> Share;
    Share cTitV;
    Share cCodV;
    Share cPriV;
    Share cChaV;

    if (!myinput){
        cout << "unbakjsfk sdjkfjdsf" << endl;
        exit(1);
    }

    string sStart = "<td><a href";       // to select the Company Name containing line
    string titleStart = "title=\"";      // to select the Company Title containing line
    string codeStart = "\">";            // to select the Company Code containing line

    string priceS = "stream=\"last";       // to select the company's share proces
    string pr_E = "</span><";           // to select the company's share proces

    string ChangeS = "stream=\"change_";
    string ch_E = "</span></";

    size_t posS, posPrice, posChan;
    if(myinput.is_open()){

    while(getline(myinput, line)) {

        posS = line.find(sStart);
        posPrice = line.find(priceS);
        posChan = line.find(ChangeS);

        if (posS != string::npos) {
            //cout << posS << "\t";
            string Temp = line; // Backingup the line string

            string ssRem = line.substr(posS,line.find(titleStart)+4);  // deletable string part
            //cout << ssRem << "\t";

            string newSS = line.erase(3,ssRem.length()); // removing the unwanted starting part
            //cout << newSS << "\n";

            string CompName = newSS.substr(0,newSS.find_first_of("\""));   // Extracting Company name
            //cout << CompName << "\t";
            cTitV.push_back(CompName);

            string CompCodeTemp = ssRem.substr(posS,ssRem.find(codeStart)-1);
            //cout << CompCodeTemp << "\n";

            string CodeTemp = ssRem.erase(0,CompCodeTemp.length()+3); // removing the unwanted starting part
            //cout << CodeTemp << "\n";

            string CompCode = CodeTemp.substr(0,CodeTemp.find_first_of("<"));
            //cout << CompCode << "\n";
            cCodV.push_back(CompCode);


        }

        if(posPrice != string::npos){
            //cout << posPrice << "\n";
            string TempP = line;

            string ppRem = line.substr(posPrice,line.find(pr_E)-posPrice);
            //cout << ppRem << "\t";

            string pTemp = ppRem.substr(0,ppRem.find_first_of(">"));  // chage the parameter to apply universal
            string Pricing = ppRem.erase(0,pTemp.length()+1);
            //cout << Pricing << "\t";
            cPriV.push_back(Pricing);

        }



        if(posChan != string::npos){
            //cout << posChan << "\n";
            string TempC = line;

            string ccRem = line.substr(posChan,line.find(ch_E)-posChan);
            //cout << ccRem << "\t";

            string cTemp = ccRem.substr(0,ccRem.find_first_of(">"));
            //cout << cTemp << "\t------------ ";

            string cTemp2 = ccRem.erase(0,ccRem.find_first_of(">")+1);
            //cout << cTemp2 << "\n";

            string cTempTemp = cTemp2.substr(0,cTemp2.find_first_of(">"));
            //cout << cTempTemp << "\t";

            string Changei = cTemp2.erase(0,cTemp2.find_first_of(">")+1);
            //cout << Changei <<"\n";

            cChaV.push_back(Changei);

        }



    }



    myinput.close();


    }



    cPriV.erase(cPriV.begin()+0);
    cPriV.erase(cPriV.begin()+0); // random value in the first three values
    cPriV.erase(cPriV.begin()+0);
    cChaV.erase(cChaV.begin()+0);
    cChaV.erase(cChaV.begin()+0);
    cChaV.erase(cChaV.begin()+0);



    //vector< vector<string> > Details;

    myoutput<< "\"StockMarket\":[" ;
    for(int i=0;i<cPriV.size();i++){

        myoutput << "{\"CompanyName\":";
        myoutput << "\"";
        myoutput << cTitV[i];
        myoutput << "\",";

        myoutput << "\"StockMarketCode\":";
        myoutput << "\"";
        myoutput << cCodV[i];
        myoutput << "\",";

        myoutput << "\"StockPrice\":";
        myoutput << "\"";
        myoutput << cPriV[i];
        myoutput << "\",";

        myoutput << "\"StockPriceChange\":";
        myoutput << "\"";
        myoutput << cChaV[i];
        myoutput << "\"}";

        myoutput << ",";



    }
    myoutput<<"]";



    return(1);
}