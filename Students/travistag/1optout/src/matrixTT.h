#ifndef MATRIXTT_H
#define MATRIXTT_
#pragma once
#include <vector>

using namespace std;

class matrixTT{	
	public:

	static std::vector<std::vector<int> > create2DArray(int r, int c);

	static void print2DArray(std::vector<std::vector<int> > arr);

	static std::vector<std::vector<int> > matrixMultiply(std::vector<std::vector<int> > m1, std::vector<std::vector<int> > m2);

	static std::vector<std::vector<int> > matrixMod2(std::vector<std::vector<int> > mat);

	static std::vector<std::vector<int> > matrixTranspose(std::vector<std::vector<int> > mat);
};

#endif
