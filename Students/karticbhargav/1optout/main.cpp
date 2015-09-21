#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "encoder.h"
#include "decoder.h"

using namespace std;

int main() {
    //Encoder

    srand(time(NULL));
    int c_t[7] = {0};
    int c_r[7] = {0};
    int ebit = rand()%7;
    int m_t[4] = {rand()%2, rand()%2, rand()%2, rand()%2};
    int m_r[4] = {0};

    cout << "The input message is: " <<endl;

    for(int i=0; i<4; i++)
    {
        cout<<m_t[i];
    }


    encoder(c_t,m_t);

    cout << endl << "Transmitted codeword is: " << endl;

    for( int i=0; i<7;i++)
    {
        cout<<c_t[i];
    }

    //Decoder

    for( int i=0; i<7;i++)
    {
        c_r[i] = c_t[i];
    }

    c_r[ebit] = (c_r[ebit] + 1)%2;

    cout << endl << "Received codeword is: " << endl;
    for( int i=0; i<7;i++)
    {
        cout<<c_r[i];
    }

    decoder(m_r , c_r);

    cout << endl << "Received message post decoding is: " << endl;
    for( int i=0; i<4;i++)
    {
        cout<< m_r[i];
    }

    cout<<endl;

    return 0;
}