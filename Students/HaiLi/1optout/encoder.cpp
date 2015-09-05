//
// Created by Hai on 2015/9/6.
//

#include "encoder.h"

void encoder(int* origcode,int* trsmtcode)
{
    int G[7][4]={{1,1,0,1},{1,0,1,1},{1,0,0,0},{0,1,1,1},{0,1,0,0},{0,0,1,0},{0,0,0,1}}; //generator matrix
    int i,j;
    for (i=0;i<7;i++)
    {
        for (j=0;j<4;j++)
        {
            trsmtcode[i]=(trsmtcode[i]+G[i][j]*origcode[j]);
        }
        trsmtcode[i]=trsmtcode[i]%2;
    }
}
