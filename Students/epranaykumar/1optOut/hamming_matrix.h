//
// Created by pranaykumar on 7/09/15.
//

#include <vector>

#ifndef INC_1OPTOUT_HAMMING_H
#define INC_1OPTOUT_HAMMING_H

#endif //INC_1OPTOUT_HAMMING_H


// generates check matrix for hamming codes of length n and parity bits r
std::vector< std::vector<int>> gen_checkMatrix( int r,int n) ;


// generates check matrix for hamming codes of length n and msg bits k and with choosen check matrix
std::vector< std::vector<int>> gen_generatorMatrix(int k,int n,std::vector< std::vector<int>> check_mtrix ) ;
