//
// Created by Austin Taghavi on 9/6/15.
//
#include <iostream>
#include "HammingUtil.h"
using namespace std;

int  HammingUtil::codeGen[7][4] = {
        {1,1,0,1},
        {1,0,1,1},
        {1,0,0,0},
        {0,1,1,1},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
};

int HammingUtil::errorCheck[3][7] = {
        {1,0,1,0,1,0,1},
        {0,1,1,0,0,1,1},
        {0,0,0,1,1,1,1}
};

string HammingUtil::get7DigitEncoding(string data) {
    int dataBits[4] = {0,0,0,0};
    /*
    int codeGen[7][4] = {
            {1,1,0,1},
            {1,0,1,1},
            {1,0,0,0},
            {0,1,1,1},
            {0,1,0,0},
            {0,0,1,0},
            {0,0,0,1}
    };
     */

    string result = "";
    for(int i=0; i<4; i++) {
        if(data[i] == '1')
            dataBits[i] = 1;
    }

    for(int i=0; i<7; i++) {
        int val = 0;
        for(int j=0; j<4; j++) {
            val += dataBits[j]*codeGen[i][j];
        }
        if(val % 2 == 0)
            result += '0';
        else
            result += '1';
    }
    return result;
}

string HammingUtil::get74HammingEncoding(string code) {
    string result = "";
    int numPaddingBits = code.length()%4;
    for(int i=0; i<numPaddingBits; i++) code += '0';

    for(int i=0; i<code.length(); i+=4) {
        result += get7DigitEncoding(code.substr(i,4));
    }

    return result;
}

int HammingUtil::getErrorIndex(string data) {
    int errorIndex = 0;
    int dataBits[7] = {0,0,0,0,0,0,0};
    for(int i=0; i<7; i++) {
        if(data[i] == '1')
            dataBits[i] = 1;
    }
    int multiplier = 1;
    for(int i=0; i<3; i++) {
        int val = 0;
        for(int j = 0; j < 7; j++) {
            val += dataBits[j]*errorCheck[i][j];
        }
        errorIndex += (val%2)*multiplier;
        multiplier *=2;
    }

    return errorIndex;
}

vector<int> HammingUtil::getErrorIndices(string code) {
    vector<int> errorBits;

    for(int i=0; i<code.length(); i+=7) {
        int errorIndex = getErrorIndex(code.substr(i, 7));
        if(errorIndex > 0) {
            errorBits.push_back(errorIndex+i);
        }
    }
    return errorBits;
}

string HammingUtil::decode74HammingEncoding(string code) {
    string result = "";

    for(int i=0; i<code.length(); i+=7) {
        result += code[i+2];
        result += code.substr(i+4,3);
    }

    return result;
}