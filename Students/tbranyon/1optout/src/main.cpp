#include <iostream>
#include <string>
#include "Hamming74.h"
using namespace std;

typedef unsigned short int u8;

inline void assemble_code();
inline void disassemble_code();

Hamming74 h74 = Hamming74(); //declare global codex object

int main(int argc, char *argv[])
{
	cout << "*** (7,4) Hamming Encoder/Decoder -- Select an option [1-3]: ***" << endl;
	cout << "1) Encode" << endl;
	cout << "2) Decode" << endl;
	int sel;
	cin >> sel;
	if(sel < 1 || sel > 2)
	{
		cerr << "Error, invalid input, exiting!" << endl;
		return 0;
	}
	switch(sel){
		case(1): assemble_code();
				 return 0;
		case(2): disassemble_code();
				 return 0;
		default: return 0;
	}
	return 1; //code should never get here
}	

inline void assemble_code()
{
	//obtain input, transform to u8
	cout << "Enter a 4-bit binary sequence: ";
	string input;
	cin >> input;
	if(input.length() != 4)
	{
		cerr << "Error, invalid sequence length!\n";
		return;
	}
	u8 data = 0;
	for(u8 x = 0; x < 4; ++x) //transform input from string to 4-bit integer (stored in 8-bits)
	{
		if(input.at(x) != '1' && input.at(x) != '0')
		{
			cerr << "Error, invalid input!\n";
			return;
		}
		if(input.at(x) == '1')
			data += 1 << (3-x);
	}
	u8 codeword = h74.encode(data); //this is the codeword in raw data, would print as a decimal.
	//print the codeword in binary
	cout << "Binary codeword: ";
	/*Using a loop here, besides the overhead of the branch instructions, 
	involves extra operations every iteration to calculate proper bitshifts. Unrolling instead.*/
	cout << ((codeword & 0x40) >> 6) << ((codeword & 0x20) >> 5) << ((codeword & 0x10) >> 4) 
			<< ((codeword & 0x08) >> 3) << ((codeword & 0x04) >> 2) << ((codeword & 0x02) >> 1) 
			<< (codeword & 0x01);
	cout << endl;
	return;
}

inline void disassemble_code()
{
	cout << "Enter 7-bit binary Hamming codeword: ";
	string input;
	cin >> input;
	if(input.length() != 7)
	{
		cerr << "Error, invalid sequence length\n";
		return;
	}
	u8 raw_data[7]; //in the long run it's probably more efficient and certainly easier do to a char array like this
	for(u8 x = 0; x < 7; ++x)
	{
		if(input.at(x) != '1' && input.at(x) != '0')
		{
			cerr << "Error, invalid input!\n";
			return;
		}
		raw_data[x] = (input[x]-48); //now that we've verified that input is ones and zeroes, this converts from ASCII to u8
	}
	u8 data = h74.decode(raw_data);
	cout << "4-bit data: " << ((data & 0x8) >> 3) << ((data & 0x4) >> 2) << ((data & 0x2) >> 1) << (data & 0x1) << endl;
	return;
}
