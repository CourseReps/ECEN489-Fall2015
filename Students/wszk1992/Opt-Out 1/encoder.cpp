//
// Created by wszk1_000 on 9/5/2015.
//

#include "encoder.h"



void encoder(int* p_srccode, int* p_trsmtcode)
{
    int G[7][4] =
    {
        1,1,0,1,
        1,0,1,1,
        1,0,0,0,
        0,1,1,1,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    int i, j;
    for (j = 0; j < 7; j++)
    {
        for (i = 0; i < 4; i++)
        {
            p_trsmtcode[j] += G[j][i] * p_srccode[i];
        }
        p_trsmtcode[j] = p_trsmtcode[j] % 2;
    }

}