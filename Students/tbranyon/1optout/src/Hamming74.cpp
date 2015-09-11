#include "Hamming74.h"

Hamming74::Hamming74()
{
}

/*
	Function: encode
	Take in a 4-bit data vector and output a (7,4) Hamming code word.
	Simply multiplies data by the 4x7 generator matrix, G, and returns the result.
*/
u8 Hamming74::encode(u8 data)
{
	u8 tmp[7] {0,0,0,0,0,0,0};
	u8 retval = 0;
	
	for(u8 j = 0; j < 7; ++j) //j corresponds to the column of the result (and also the columns of G)
		for(u8 k = 0; k < 4; ++k) //k is our summation variable, used for the columns of data and rows of G
			tmp[j] ^=  ((data >> 3-k) & G[j][k]); //extract bit of interest from data and modulo 2 multiply it with G[k][j].
	
	//construct u8 from tmp
	for(u8 x = 0; x < 7; ++x)
		retval |= tmp[x] << (6-x); //have to invert indices
	return retval;
}

u8 Hamming74::decode(u8 data[])
{
	u8 retval = 0;
	
	u8 syndrome[3] = {0}; //syndrome vector which indicates 1-bit errors
	//if we were really in need of more efficiency, we could easily unroll the "j" level of this nested loop
	for(u8 i = 0; i < 3; ++i)
		for(u8 k = 0; k < 7; ++k)
			syndrome[i] ^= H[k][i] & data[k];
	
	if(syndrome[0] == 0 && syndrome[1] == 0 && syndrome[2] == 0) //syndrome = [0 0 0], we're done, no errors
		retval = (data[3] << 3) | (data[4] << 2) | (data[5] << 1) | data[6]; //last 4 bits of received codeword are the actual data
	else
	{
		for(u8 x = 0; x < 7; ++x)
			if(!memcmp(H[x], syndrome, 3*sizeof(u8))) //if column vector at H[x] matches syndrome
			{
				data[x] = (~data[x]) & 0x01; //invert bit at position x
				break;
			}
		//combine data bits into a u8
		retval = (data[3] << 3) | (data[4] << 2) | (data[5] << 1) | data[6];
	}
	
	return retval;
}
