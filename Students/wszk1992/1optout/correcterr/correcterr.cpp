//
// Created by wszk1_000 on 9/6/2015.
//

#include "correcterr.h"
bool correct(int* p_trsmtcode, int* p_synvec)
{
    int H[3][7] =
            {
                    1,0,1,0,1,0,1,
                    0,1,1,0,0,1,1,
                    0,0,0,1,1,1,1
            };
    int i, j;
    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < 7; i++)
        {
            p_synvec[j] += H[j][i] * p_trsmtcode[i];
        }
        p_synvec[j] = p_synvec[j] % 2;
    }
    if (!(p_synvec[0] || p_synvec[1] || p_synvec[2]))
    {
        return 1;
    }
    else
        return 0;
}