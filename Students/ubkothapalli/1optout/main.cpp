#include <iostream>
#include <vector>
#include <tgmath.h>
#include <ctime>
#include <cstdlib>

using namespace std;

int main() {


    // creating a matrix creation function
    typedef vector<int> Row;
    typedef vector<Row> Matrix;

    // defining varialbes
    int i, j;

    // taking the numberof parity bits from the user
    int numParBit;
    cout << "please enter the number of parity bits used: ";
    cin >> numParBit;
    cout << "you have entered the parity bit number as: " << numParBit;
    cout << "\n";

    // creating the Parity check Matrix
    int row_P = numParBit;
    int col_P = pow(2, numParBit) - 1;

    Row P_temp(7) ;
    P_temp[0]=1;
    P_temp[1]=2;
    P_temp[2]=4;
    P_temp[3]=6;
    P_temp[4]=5;
    P_temp[5]=3;
    P_temp[6]=7;




    Matrix Par_mat(row_P, Row(col_P));
    for (i = 0; i < col_P; i++) {

        Par_mat[2][i] = P_temp[i ] / 4; //(pow(2,row_P-1));  // P1
        Par_mat[1][i] = (P_temp[i ] % 4 )/ 2;// (pow(2,row_P-1)))/(pow(2,row_P-2));   //P2
        Par_mat[0][i] = P_temp[i ] % 2; //(pow(2,row_P-2));   //P3


    }
    cout << "PArity Check Matrix \n";

    for (i = 0; i < Par_mat.size(); i++) {
        for (j = 0; j < Par_mat[i].size(); j++) {
            cout << Par_mat[i][j] << "\t";

        }
        cout << "\n";

    }
    cout << "Creating a Gen MAtrix";
    cout << "\n ..........";

    //Creating the Generation matrix
    int col_G = col_P;
    int row_G = col_G - numParBit;
    // cout << row_G;
    cout << "\n";

    Matrix Gen_mat(row_G, Row(col_G));


    // we take the elements of the generative matrix from the parity matrix
    // a row in the parity matrix represents the Partiy bit from P(numParBit) to P(1)

    Matrix temp_P_mat(row_G, Row(row_P));
    for (i = 0; i < row_P; i++) {
        temp_P_mat[0][i] = Par_mat[i][2];
        temp_P_mat[1][i] = Par_mat[i][4];
        temp_P_mat[2][i] = Par_mat[i][5];
        temp_P_mat[3][i] = Par_mat[i][6];
    }


   /* for (i = 0; i < temp_P_mat.size(); i++) {
        for (j = 0; j < temp_P_mat[i].size(); j++) {
            cout << temp_P_mat[i][j] << "\t";

        }
        cout << "\n";

    } */
// P1 - P3

    for (i = 0; i < numParBit; i++) {
        for (j = 0; j < row_G; j++) {
            Gen_mat[j][i] = temp_P_mat[j][i];
        }
    }
// assigning the rest values of the data bits in Gen matrix
    // Check for generalizing later
// Generqting Gen Matrix with P1, P2, P3, D1, D2, D3, D4
    Gen_mat[0][3] = 1;
    Gen_mat[1][4] = 1;
    Gen_mat[2][5] = 1;
    Gen_mat[3][6] = 1;

    cout << "Generative MAtrix \n";

    for (i = 0; i < Gen_mat.size(); i++) {
        for (j = 0; j < Gen_mat[i].size(); j++) {
            cout << Gen_mat[i][j] << "\t";

        }
        cout << "\n";

    }


    int db = col_G-numParBit;

    Row data_code(db);
    cout << "Randomly generated message bits: ";
    srand((unsigned int) time(NULL));
    for (i=0;i<db;i++) {

        data_code[i] = rand()%2;
        cout << data_code[i] <<"\t";
    }
    cout << "\n";
    cout << "Encoding the message: \n";
    // Encoding

    Row enCode(col_G);

    for (i=0;i<7;i++){

        for (j=0;j<4;j++){
            enCode[i] += data_code[j]*Gen_mat[j][i];
        }
        enCode[i]=enCode[i]%2;
    }


    cout << "The Encoded message is: ";
    for (i=0;i<enCode.size();i++) {

        cout << enCode[i] << "\t";

    }

    cout << "\n";
    cout << "Now Decoding: \n";

    //Hamming Decoding

    // For decoding we have to remove the parity bits which were placed in 0,1,3 indeces
    Row deCode(db);
    deCode[0]=enCode[3];
    deCode[1]=enCode[4];
    deCode[2]=enCode[5];
    deCode[3]=enCode[6];
    cout << "The decoded message is: ";
    for (i=0;i<deCode.size();i++) {

        cout << deCode[i] << "\t";

    }
    // Checking and Correcting Errors

    cout <<"\n Error Checking: \n";

    Row errorVec(numParBit);



    for (j=0;j<row_P;j++){

        for (i=0;i<col_G;i++){
            errorVec[j] += enCode[i]*Par_mat[j][i];
        }

    }
    //cout << "printing the error check vector: ";

    // Error matrix
    /*for (i=0;i<errorVec.size();i++) {

        cout << errorVec[i] % 2  << "\t";

    } */

     // findig o the errors and correcting
    if ((errorVec[0]==1)|| (errorVec[1]==1) || (errorVec[2]==1)){


        if ((errorVec[0]==1)&& (errorVec[1]==1) && (errorVec[2]==1)){
            cout << "\n Error occured in the fourth data bit \n";
            deCode[3]=(deCode[3]+1)%2;
        }
        if (errorVec[0]==1){
            if(errorVec[1]==1){
                cout << "\n Error occured in the third data bit \n";
              deCode[2]=  (deCode[2]+1)%2;
            }
        }
        if (errorVec[0]==1) {
            if (errorVec[2] == 1) {
                cout << "\n Error occured in the  second data bit \n";
                deCode[1] = (deCode[1] + 1) % 2;
            }
        }
        if (errorVec[1]==1) {
            if (errorVec[2] == 1) {
                cout << "\n Error occured in the  first data bit \n";
                deCode[0] = (deCode[0] + 1) % 2;
            }
        }
        cout << "Error occured in the transmitted bits";
        cout << "\n Corrected received bits are:  \n";
        for (i=0;i<deCode.size();i++) {

            cout << deCode[i] << "\t";

        }
    return 0;
    }

return 1;

}

