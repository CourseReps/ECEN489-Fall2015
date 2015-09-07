//
// Created by pranaykumar on 7/09/15.
//

#include "hamming_functions.h"
#include "hamming_matrix.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/* ****************
 * contains functions
 *encoder----for encoding a message with r parity bits
 *get_errIndex --- for finding the index of the error bit in a encoded msg of r parity bits
 * decoder --- for correcting and decoding a msg from a encoded msg with single bit error index
*/

vector<int> encoder(vector<int> message, int n, int r) {

    // generate check matrix

    vector< vector<int>> check_matrix ;
    check_matrix = gen_checkMatrix(r,n);


    // generate generator matrix

    std::cout << "\n \n==========generator matrix==========\n\n";


    int k=n-r;
    vector< vector<int>> gen_mtx ;
    gen_mtx = gen_generatorMatrix(k,n,check_matrix);
    printMatrix(gen_mtx );

    //encoder
    vector<int> encoded_msg(n);
    std::cout << "\n\n=====encoded_msg =====\n\n";

    for (int i=0;i<n;i++)
    {
        encoded_msg[i]=0;
        for (int j=0;j<k;j++)
        {
            encoded_msg[i]=(encoded_msg[i]+message[j]*gen_mtx[j][i]);
        }
        encoded_msg[i]=encoded_msg[i]%2;
        std::cout << encoded_msg[i] << "\t";
    }
    return encoded_msg;

}


int get_errIndex(vector<int> encoded_msg, int r) {


   int  n= encoded_msg.size();
    // generate check matrix

    vector< vector<int>> check_matrix ;
    check_matrix = gen_checkMatrix(r,n);

    std::cout << "\n \n==========check matrix==========\n\n";
    printMatrix(check_matrix );

    //error check
    vector<int> error_mtx(r);
    int error_index =0;

    std::cout << "\n\n=====error index======\n\n";
    for (int j=0;j<r;j++)
    {
        error_mtx[j]=0;
        for (int i=0;i<n;i++)
        {
            error_mtx[j]= error_mtx[j]+ (check_matrix[j][i]*encoded_msg[i]);
        }
        error_index=error_index + (error_mtx[j]%2)* pow(2,(r-j-1));

    }
    std::cout << error_index << "\t";

    return error_index;

}


vector<int> decoder(vector<int> encoded_msg, int error_index, int k ) {

    //correct and decode
    vector<int> decoded_msg(k);
    std::cout << "\n\n=======decoded msg=========\n\n";
    int mask=1;
    int j=0;
    for (int i=0;i<encoded_msg.size();i++)
    {
        if(i+1 == mask ) {
            mask <<= 1;
            // std::cout << i+1<<" is a parity column\n";
        }
        else {
            if((i+1)==error_index){
                decoded_msg[j] = (encoded_msg[i]+1)%2;  // flip the value
            }
            else{ decoded_msg[j] = encoded_msg[i];}
            std::cout << decoded_msg[j] << "\t";
            j++;
        }
    }

    return decoded_msg;

}


void printMatrix(vector<vector<int>> mtx){

    //print check matrix
    for(int j=0;j<mtx.size();j++)   {
        for(int i=0;i<mtx[j].size();i++)   {
            std::cout << mtx[j][i] << "\t";
        }
        std::cout << "\n";
    }

}
