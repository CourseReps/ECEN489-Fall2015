#include <iostream>
#include <stdlib.h>
#include "cmath"
#include <ctime>
#include "encoder.h"
#include "decoder.h"
#include <stdio.h>
using namespace std;

int main() {
    srand(time(0));
    int P[4]={rand()%2,rand()%2,rand()%2,rand()%2}; //original code
    int x[7]={0}; //transmitted code
    int xr[7]={0}; //received code
    int r[4]={0}; //decoded code
    int ep=rand()%7; //position of error bit
    bool a=true;
    int i,j;
    cout<<"original code: ";
    for (i=0;i<4;i++)
    {
        cout<<P[i];
    }
    cout<<endl<<"transmitted code: ";
    encoder(P,x);
    for (i=0;i<7;i++)
    {
        cout<<x[i];
    }
    cout<<endl;
    for (i=0;i<7;i++)
    {
        xr[i]=x[i];
    }
    xr[ep]=(xr[ep]+1)%2;
    cout<<"received code: ";
    for (i=0;i<7;i++)
    {
        cout<<xr[i];
    }
    decoder(xr,r);
    cout<<endl<<"after decoding: ";
    for (i=0;i<4;i++)
    {
        cout<<r[i];
    }
    cout<<endl;
    for (i=0;i<4;i++)
    {
        if (P[i]!=r[i])
        {
            a=false;
        }
    }
    return 0;
}