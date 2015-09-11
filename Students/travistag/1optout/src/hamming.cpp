#include <iostream>
#include "matrixTT.h"
#include "hamming.h"
#include <vector>

using namespace std;
	

//Returns the G matrix for the Hamming(7,4) code, which is multiplied by the data to produce the encoded word
std::vector<std::vector<int> > hamming::getHammingGMatrix(){
  std::vector<std::vector<int> > ret = matrixTT::create2DArray(7, 4);
  ret[0][0]=1;
  ret[0][1]=1;
  ret[0][3]=1;
  ret[1][0]=1;
  ret[1][2]=1;
  ret[1][3]=1;
  ret[2][0]=1;
  ret[3][1]=1;
  ret[3][2]=1;
  ret[3][3]=1;
  ret[4][1]=1;
  ret[5][2]=1;
  ret[6][3]=1;

  return ret;
}

//Returns the H matrix for the Hamming(7,4) code, which is multiplied by the encoded word to check for errors
std::vector<std::vector<int> > hamming::getHammingHMatrix(){
  std::vector<std::vector<int> > ret = matrixTT::create2DArray(3, 7);
  ret[0][0]=1;
  ret[0][2]=1;
  ret[0][4]=1;
  ret[0][6]=1;
  ret[1][1]=1;
  ret[1][2]=1;
  ret[1][5]=1;
  ret[1][6]=1;
  ret[2][3]=1;
  ret[2][4]=1;
  ret[2][5]=1;
  ret[2][6]=1;

  return ret;
}

//Returns the R matrix, used for reconstruction
std::vector<std::vector<int> > hamming::getHammingRMatrix(){
  std::vector<std::vector<int> > ret = matrixTT::create2DArray(4, 7);
  ret[0][2]=1;
  ret[1][4]=1;
  ret[2][5]=1;
  ret[3][6]=1;

  return ret;
}

//Takes in an encoded word, and checks for/corrects errors
std::vector<std::vector<int> > hamming::hammingErrorCorrect(std::vector<std::vector<int> > mat){
  std::vector<std::vector<int> > z = matrixTT::matrixMultiply(getHammingHMatrix(), mat);
  z = matrixTT::matrixMod2(z);			//Obtain the z Matrix by multiplying the data by the H matrix
  int error=z[2][0]*4+z[1][0]*2+z[0][0]*1;	//Z matrix tells the error by 
  if(error!=0){
    std::cout<<"Error detected in bit "<<error<<", correcting.\n";
    mat[error-1][0]=1-mat[error-1][0];
  }

  return mat;
}

std::vector<int> hamming::encode4Bits(std::vector<int> word){
  std::vector<std::vector<int> > unencoded;
  unencoded.push_back(word);
  unencoded = matrixTT::matrixTranspose(unencoded);
  std::vector<std::vector<int> > encoded = matrixTT::matrixMultiply(hamming::getHammingGMatrix(), unencoded);
  encoded = matrixTT::matrixTranspose(encoded);
  encoded = matrixTT::matrixMod2(encoded);
  return encoded[0];
}

std::vector<int> hamming::decode7Bits(std::vector<int> word){
  std::vector<std::vector<int> > encoded;
  encoded.push_back(word);
  encoded = matrixTT::matrixTranspose(encoded);
  std::vector<std::vector<int> > decoded = hamming::hammingErrorCorrect(encoded);
  decoded = matrixTT::matrixMultiply(hamming::getHammingRMatrix(), decoded);
  decoded = matrixTT::matrixTranspose(decoded);
  decoded = matrixTT::matrixMod2(decoded);
  return decoded[0];
}
