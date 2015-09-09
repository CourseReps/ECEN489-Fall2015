//
// Created by wszk1_000 on 9/6/2015.
//

#include "decoder.h"
void decoder(int* p_trsmtcode, int* code)
{
    code[0]=p_trsmtcode[2];
    code[1]=p_trsmtcode[4];
    code[2]=p_trsmtcode[5];
    code[3]=p_trsmtcode[6];
}