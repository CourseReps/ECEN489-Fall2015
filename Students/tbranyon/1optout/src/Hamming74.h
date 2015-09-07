/*
	Thomas Branyon
	3 September 2015
	ECEN 489-501	
	
	Class to handle encoding and decoding (with error handling) (7,4) Hamming codes.
	Variables are 8-bit unsigned because the largest piece of data handled is a 7-bit vector.
	In the interest of memory conservation for scaling to large operations, the smallest
	data type that will accomodate this is used.
*/
#ifndef _HAMMING_H_
#define _HAMMING_H_
#include <string.h>
using namespace std;

//just call unsigned short int "u8" so we can type less and avoid confusion
typedef unsigned short int u8;

class Hamming74
{
	u8 G[7][4] { {0,1,1,1}, {1,0,1,1}, {1,1,0,1}, {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} }; //Code generating matrix for (7,4) Hamming code
	u8 H[7][3] { {1,0,0}, {0,1,0}, {0,0,1}, {0,1,1}, {1,0,1}, {1,1,0}, {1,1,1} }; //
	public:
		Hamming74();
		u8 encode(u8);
		u8 decode(u8[]);
};

#endif
