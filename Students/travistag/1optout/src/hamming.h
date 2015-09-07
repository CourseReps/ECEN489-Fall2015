#ifndef HAMMING_H
#define HAMMING_H
#pragma once
#include "matrixTT.h"
#include <vector>

using namespace std;
	
class hamming{
	public:

	static std::vector<std::vector<int> > getHammingGMatrix();

	static std::vector<std::vector<int> > getHammingHMatrix();

	static std::vector<std::vector<int> > getHammingRMatrix();
	
	static std::vector<int> encode4Bits(std::vector<int> word);

	static std::vector<int> decode7Bits(std::vector<int> word);

	static std::vector<std::vector<int> > hammingErrorCorrect(std::vector<std::vector<int> > mat);
};

#endif
