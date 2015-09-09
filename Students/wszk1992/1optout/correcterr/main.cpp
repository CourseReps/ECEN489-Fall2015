#include <iostream>
#include"correcterr.h"
using namespace std;

int main() {
    int p_trsmtcode[7] = {0,1,1,0,1,1,1};
    int p_synvec[3] = {0};
    int i;
    int n;
    bool deco;
    cout<<"trsmtcode:";
    for (i = 0; i < 7; i++) {
        cout<<p_trsmtcode[i];
    }
    deco = correct(p_trsmtcode, p_synvec);
    cout <<endl<< "-----correct----->"<<endl;
    if(deco)
        cout<<"right code!";
    else {
        cout << "wrong code!"<<endl;
        n=4*p_synvec[0]+2*p_synvec[1]+p_synvec[2];
        cout<<"No. "<<n<<" is wrong"<<endl;
        p_trsmtcode[n-1]=1- p_trsmtcode[n-1];
        cout<<"correct code is ";
        for (i = 0; i < 7; i++) {
            cout<<p_trsmtcode[i];
        }
        }
    cout << endl;
}