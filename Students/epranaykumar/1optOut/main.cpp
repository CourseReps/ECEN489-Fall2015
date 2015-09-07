#include <iostream>
#include <vector>


#include <ctime>
#include <cmath>
#include <stdlib.h>

#include "hamming_matrix.h"
#include "hamming_functions.h"

using namespace std;

int main(int argc, char* argv[]) {


     int n ; // length of the hamming code
     int k; // number of data bits
     int r; // number of parity bits
    std::cout << "Please enter the number of parity bits" <<endl;
    std::cin >> r;

    n= (int) pow(2,r)- 1;
    k= n-r;

    std::cout << "\nhamming code length: " <<n<< "\t message length: "<< k<< endl;

  // generate arbitary message of length k

    vector<int> msg(k);
    std::cout << "\n\n===========Message=========\n\n";

    msg={1,0,1,0};

    /* initialize random seed: */
       srand((unsigned int) time(0));

       for (int i=0;i<k;i++){
           msg[i]=(rand()%2) ;
           std::cout << msg[i] << "\t";
       }



    // encode the message
    vector<int> encoded_msg;
    encoded_msg = encoder(msg,n,r);

    std::string modify;
    int inpt;
    std::cout << "want to modify the encoded msg y/n" <<endl;
    std::cin >> modify;
    // get the error index
    if (modify.compare("y")==0){
        std::cout << "enter the modified msg of length " << n <<endl;
        std::cin >> inpt;
        for (int i=n-1;i>=0;i--){
            encoded_msg[i]=(inpt%10) ;
            inpt=inpt/10;

        }
    }



    int error_index;
    error_index = get_errIndex(encoded_msg,r);

    // decode the message
    vector<int> decoded_msg;
    decoded_msg = decoder(encoded_msg,error_index,k);


    return 0;

}

