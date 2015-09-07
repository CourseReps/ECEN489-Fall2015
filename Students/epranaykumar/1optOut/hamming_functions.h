//
// Created by pranaykumar on 7/09/15.
//
#include <vector>

#ifndef INC_1OPTOUT_HAMMING_FUNCTIONS_H
#define INC_1OPTOUT_HAMMING_FUNCTIONS_H



std::vector<int> decoder(std::vector<int> encoded_msg, int error_index, int k );

int get_errIndex(std::vector<int> encoded_msg, int r);

std::vector<int> encoder(std::vector<int> message, int n, int r);


void printMatrix(std::vector<std::vector<int>> mtx);

#endif //INC_1OPTOUT_HAMMING_FUNCTIONS_H
