//
// Created by Kartic Bhargav on 9/7/2015.
//

#include "decoder.h"
#include <string>

void decoder ( int* m, int* r )
{
    int syndrome[7][3] = {{1,0,0},{0,1,0},{0,0,1},{1,1,1},{1,0,1},{1,1,0},{0,1,1}};
    int HT[7][3] = {{1,0,0},{0,1,0},{0,0,1},{1,1,1},{1,0,1},{1,1,0},{0,1,1}};
    int s[3] = {0};
    int ebit;

    for (int j=0; j<3; j++)
    {
        for(int i=0; i<7; i++)
        {
            s[i][j] += r[i]*HT[i][j];
        }

        s[i][j] = s[i][j]%2;
    }

    for (int i=0; i<7; i++)
    {
        if(memcmp(s,syndrome[i],sizeof(s))==0)
        {
            ebit = i;
        }
    }

    r[ebit] = (r[ebit]+1)%2;
    for (int i=0; i<4; i++)
        m[i] = r[i+3];


}
