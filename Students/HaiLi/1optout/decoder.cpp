//
// Created by Hai on 2015/9/6.
//

#include "decoder.h"
#include <cmath>

void decoder(int* reccode,int* result)
{
    int H[3][7]={{1,0,1,0,1,0,1},{0,1,1,0,0,1,1},{0,0,0,1,1,1,1}}; //parity-check matrix
    int R[4][7]={{0,0,1,0,0,0,0},{0,0,0,0,1,0,0},{0,0,0,0,0,1,0},{0,0,0,0,0,0,1}}; //decoding matrix
    int i,j;
    int xc[7]={0}; //corrected received code
    int z[3]={0}; //syndrome vector
    int fp=0; //the error bit corresponding to syndrome vector
    for (i=0;i<3;i++)
    {
        for (j=0;j<7;j++)
        {
            z[i]=(z[i]+H[i][j]*reccode[j]);
        }
        z[i]=z[i]%2;
    }
    for (i=0;i<3;i++)
    {
        fp=fp+pow(2,i)*z[i];
    }
    for (i=0;i<7;i++)
    {
        xc[i]=reccode[i];
    }
    xc[fp-1]=(xc[fp-1]+1)%2;
    for (i=0;i<4;i++)
    {
        for (j=0;j<7;j++)
        {
            result[i]=(result[i]+R[i][j]*xc[j])%2;
        }
    }
}