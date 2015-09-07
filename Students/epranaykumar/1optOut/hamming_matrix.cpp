//
// Created by pranaykumar on 7/09/15.
//
#include <iostream>
#include <vector>
#include <math.h>


using namespace std;
typedef vector< vector<int>> imatrix;


imatrix gen_checkMatrix(int width, int len) {

    imatrix check_matrix (width,vector<int> (len));

    for (int i = 0; i < len; i++) {

        int mask = 1 << (width - 1); // right most is least significant bit and left most is most signficant bit

        for (int j = 0; j < width; j++) {

            if (((i + 1) & mask) == 0)
                check_matrix[j][i] = 0;
            else
                check_matrix[j][i] = 1;

            mask >>= 1;
           // std::cout << check_matrix[j][i] << "\t";
        }
       // std::cout << "\n";
    }

    return check_matrix ;
}


// number of message bits (k) = width
// hamming code length = len
imatrix gen_generatorMatrix( int width, int len,imatrix check_matrix) {

    imatrix gen_mtx (width,vector<int> (len));
    int p=0;

    int r = len - width;

    for(int i=r-1;i>=0;i--) {

        int col = (int) pow(2,i)-1;
        int l=0;
        int mask = 1;

        for(int j=0;j<len;j++) {

            if(j+1 == mask ) {
                mask <<= 1;
                // std::cout << "parity column\n";
            }
            else {
                //std::cout << "msg bits\n";
                gen_mtx[l][col] = check_matrix[p][j];
                //std::cout << gen_mtx[l][col] << "\t";
                l+= 1;
            }
        }
        p+=1;
        //std::cout << col <<"\n";
    }

    int mask = 1;
    int count = 0;

    for(int i=0;i<len;i++) {

        if (i + 1 == mask)
            mask <<= 1;

        else {

            for (int j = 0; j < width; j++) {

                if (j == count)
                    gen_mtx[j][i] = 1;
                else
                    gen_mtx[j][i] = 0;

            }

            count += 1;
        }

    }

    return gen_mtx ;
}