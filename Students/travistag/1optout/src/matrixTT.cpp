
#include <vector>
#include <iostream>
#include "matrixTT.h"
std::vector<std::vector<int> > matrixTT::create2DArray(int r, int c){
  std::vector<std::vector<int> > ret;
  for(int i=0; i<r; i++){
    std::vector<int> newRow;
    for(int j=0; j<c; j++){
      newRow.push_back(0);
    }
    ret.push_back(newRow);
  }
  return ret;
}


void matrixTT::print2DArray(std::vector<std::vector<int> > arr){
  for(int i=0; i<arr.size(); i++){
    for(int j=0; j<arr[i].size(); j++){
      std::cout<<arr[i][j]<<"\t";
    }
    std::cout<<"\n";
  }
}

std::vector<std::vector<int> > matrixTT::matrixMultiply(std::vector<std::vector<int> > m1, std::vector<std::vector<int> > m2){
  std::vector<std::vector<int> > ret;
  if(m1[0].size()!=m2.size()){
    std::cout<<"Matrix dimensions do not match!\n";
    return ret;
  }

  for(int i=0; i<m1.size(); i++){
    std::vector<int> newRow;
    for(int j=0; j<m2[0].size(); j++){
      int sum=0;
      for(int k=0; k<m2.size(); k++){
	sum+=m1[i][k]*m2[k][j];
      }
      newRow.push_back(sum);
    }
    ret.push_back(newRow);
  }
  return ret;
}

std::vector<std::vector<int> > matrixTT::matrixMod2(std::vector<std::vector<int> > mat){
  std::vector<std::vector<int> > ret;
  for(int i=0; i<mat.size(); i++){
    std::vector<int> newRow;
    for(int j=0; j<mat[i].size(); j++){
      newRow.push_back(mat[i][j]%2);
    }
    ret.push_back(newRow);
  }

  return ret;
}

std::vector<std::vector<int> > matrixTT::matrixTranspose(std::vector<std::vector<int> > mat){
  std::vector<std::vector<int> > ret;
  for(int i=0; i<mat[0].size(); i++){
    std::vector<int> newRow;
    for(int j=0; j<mat.size(); j++){
      newRow.push_back(mat[j][i]);
    }
    ret.push_back(newRow);
  }

  return ret;
}
