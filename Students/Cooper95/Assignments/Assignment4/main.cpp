#include <iostream>
#include <vector>
#include <math.h>
#include <numeric>
#include <algorithm>

using namespace std;

double mode(vector<double> temp);
double variance(vector<double> temp, double mean);
double median(vector<double> temp);

int main() {
    vector<double> vec;
    double e = exp(1);
    for(int i = 0; i < 50; i++){
        vec.push_back(int(e)%10);
        e -= int(e)%10;
        e *= 10;
    }
    for (int i = 0; i< 50; i++)
        cout << vec[i] << " ";
    double sum = accumulate(vec.begin(), vec.end(), 0.0);
    double mean = sum/vec.size();
    cout << endl << "Mean: " << mean << " Mode: " << mode(vec) << " Median: " << median(vec)
    << " Variance: " << variance(vec, mean) << endl;
    return 0;
}

double mode(vector<double> temp){
    int a [] = {0,0,0,0,0,0,0,0,0,0};
    double mode = 0;
    for(double x:temp){
        if (x==0)
            a[0]++;
        else if(x==1)
            a[1]++;
        else if(x==2)
            a[2]++;
        else if(x==3)
            a[3]++;
        else if(x==4)
            a[4]++;
        else if(x==5)
            a[5]++;
        else if(x==6)
            a[6]++;
        else if(x==7)
            a[7]++;
        else if(x==8)
            a[8]++;
        else if(x==9)
            a[9]++;
    }
    int mval = a[0];
    for(int i=1;i<10;i++){
        if(a[i] > mval) {
            mode = i;
            mval = a[i];
        }
    }
    return mode;
}

double variance(vector<double> temp, double mean){
    double var = 0;
    for(double x:temp)
        var += pow((x-mean),2);
    var /= temp.size();
    return var;
}

double median(vector<double> temp){
    sort(temp.begin(),temp.end());
    return temp[temp.size()/2];
}