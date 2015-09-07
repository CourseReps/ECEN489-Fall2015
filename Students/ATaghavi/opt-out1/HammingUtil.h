//
// Created by Austin Taghavi on 9/6/15.
//

#ifndef HAMMINGCODES_HAMMINGUTIL_H
#define HAMMINGCODES_HAMMINGUTIL_H


//
// Created by Austin Taghavi on 9/6/15.
//
#include <mach/mach_port.h>
#include <iostream>
#include <vector>
using namespace std;
class HammingUtil
{
public:
    static int codeGen[7][4]; // Code generation matrix for the 7,4  Hamming Encoding

    static int errorCheck[3][7]; // Error checking matrix for the 7,4 Hamming Encoding

    static string get7DigitEncoding(string data); // Get the 7 digit encoding of a 4 digit binary string

    static string get74HammingEncoding(string code); // Get the 7,4 Hamming encoding of a binary string

    static vector<int> getErrorIndices(string code); // Function to get the indices of the error bits in an arbitrary length (7,4) Hamming encoded string.

    static int getErrorIndex(string data); // Function to get the index of the error in a 7 digit binary string

    static string decode74HammingEncoding(string code);
};

#endif //HAMMINGCODES_HAMMINGUTIL_H
