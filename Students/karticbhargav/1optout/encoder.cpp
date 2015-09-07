//
// Created by Kartic Bhargav on 9/7/2015.
//

#include "encoder.h"

void encoder ( int* c, int* m)
{
    int G[4][7] = {{1,1,1,1,0,0,0},{1,0,1,0,1,0,0},{1,1,0,0,0,1,0},{0,1,1,0,0,0,1}};


    for(int j=0; j<7; j++) {
        for (int i = 0; i < 4; i++) {
            c[j] += m[i] * G[i][j];
        }

        c[j] = c[j]%2;
    }
}