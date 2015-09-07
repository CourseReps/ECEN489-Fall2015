#include <iostream>
#include "HammingUtil.h"

using namespace std;

int main(int argc, char *argv[]) {
    if(argc < 3)
    {
        cout<<"Incorrect input\n"<<"Usage: [encode|check|decode] [binary string]";
        return 0;
    }

    string usage = argv[1];
    string code = argv[2];

    if(usage == "encode") {
        string result = HammingUtil::get74HammingEncoding(code);
        cout<<result<<"\n";
    }
    else if(usage == "check") {
        vector<int> errors = HammingUtil::getErrorIndices(code);
        for(int i=0; i<errors.size(); i++) {
            cout << "Error found in bit " << errors[i] << '\n';
            if(code[errors[i]-1] == '1') {
                code[errors[i]-1] = '0';
            }
            else {
                code[errors[i]-1] = '1';
            }
        }
        cout<<"Corrected code: "<<code<<'\n';
    }
    else if(usage == "decode") {
        cout<<HammingUtil::decode74HammingEncoding(code)<<"\n";
    }
    else{
        cout<<"Incorrect input\n"<<"Usage: [encode|check|decode] [binary string]";
    }


}

