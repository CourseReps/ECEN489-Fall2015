#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

double median(vector<double> temp);
//double mode(vector<double> temp);
double variance(vector<double> temp, double mean);


int main() {

    vector<double> vec;
    double e = exp(1);
    for(int i = 0; i < 50; i++){
        vec.push_back(int(e)%10);
        e = e - int(e)%10;
        e = e * 10;
    }
    for (int i = 0; i< 50; i++)
        cout << vec[i];

    double sum = accumulate(vec.begin(), vec.end(), 0.0);
    double mean = sum/vec.size();

    cout << endl << "The mean is " << mean << endl << "The median is " << median << endl << "The mode is " << endl << "The unbiased sample variance is " << variance(vec, mean) << endl;

    return 0;
}

/*double mode(vector<double> temp){
//cycle through vector with loop
//use counter to count values 0-9
    return mode;
}*/

double variance(vector<double> temp, double mean){
    double var = 0;
    for(double x:temp)
        var += pow((x-mean),2);
    var /= temp.size();
    return var;
}

double median(vector<double> temp) {
    double median;
    size_t size = temp.size();
    sort(temp.begin(),temp.end());
    if (size % 2 == 0 ){
        median = (temp[size / 2 - 1] + temp[size / 2]) / 2;
    }
    else{
        median = temp[size / 2];
    }
    return median;
}