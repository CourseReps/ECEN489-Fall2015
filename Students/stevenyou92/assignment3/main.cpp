#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>


using namespace std;

int main() {
    double scale = 1.60934;
    double size, speed;
    char numspd[256];


    printf("How many speed inputs?: ");
    fgets(numspd, 256, stdin );
    size = atof (numspd);
    vector <double> v(size);
    for( int i=0; i< size; i++)
    {
        cout <<"Please enter speed " << i+1 << ": " ;
        cin >> speed;
        v[i] = speed;
    }
    cout <<"The size of the vector is: " << size << endl;
    cout <<"The contents of the vector are: "<< endl;
    for( int j=0; j< size; j++)
    {
        cout <<v[j] << " mph" << endl;
    }
    for( int k=0; k< size; k++)
    {
       v[k] =  v[k] * scale;
    }
    cout <<"The size of the vector is: " << size << endl;
    cout <<"The contents of the vector are: "<< endl;
    for( int l=0; l< size; l++)
    {
        cout <<v[l] << " kmph" << endl;
    }
    return 0;
}