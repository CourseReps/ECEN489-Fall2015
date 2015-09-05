#include <iostream>
#include"encoder.h"
#include<stdio.h>


using namespace std;

int main()
{
	int p_srccode[4];
	int i,j;
    for(i=0;i<16;i++) {
        int p_trsmtcode[7] = {0};
        p_srccode[0] = i / 8;
        p_srccode[1] = (i % 8) / 4;
        p_srccode[2] = (i % 4) / 2;
        p_srccode[3] = i % 2;
        cout<<"srccode: ";
        for (j = 0; j < 4; j++) {
            cout<<p_srccode[j];
        }
        encoder(p_srccode, p_trsmtcode);
        cout << "-----transmit----->";
        cout << "trsmtcode: ";
        for (j = 0; j < 7; j++) {
            cout << p_trsmtcode[j];
        }
        cout << endl;

    }
    return 0;
}