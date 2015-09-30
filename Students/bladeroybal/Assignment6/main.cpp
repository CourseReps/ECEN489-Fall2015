#include <iostream>

using namespace std;

int swap(int *first, int *second){
    int *temp = first;
    first = second;
    second = temp;

    cout<<"Swapping..."<<endl;
    cout<<*first<<" and "<<*second<<endl;
    cout<<"new addresses..."<<endl;
    cout<<first<<" and "<<second<<endl;
}

int refswap(int &ref1, int &ref2){
    int temp=ref1;
    ref1=ref2;
    ref2=temp;

    cout<<ref1<<" and "<<ref2<<endl;

}

int main() {
    int static x,y; //variables
    int *first; //first pointer
    int *second; //second pointer
    cout << "Enter two integer amounts pressing 'Enter' after each one" << endl;
    cin>>x>>y;
    cout<< "Your two numbers are "<<x<<" and "<<y<<endl;

    first = &x;
    second = &y;

    cout<<"The addresses of these locations are ";
    cout<<first<<" and "<<second<<endl;

    swap(first,second);



    cout<<"Using references"<<endl;
    int& ref1=x;
    int& ref2=y;
    refswap(ref1,ref2);

    return 0;
}
