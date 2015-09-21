#include <iostream>
#include <vector>
#include <math.h>
#include <numeric>

using namespace std;

int main() {
    vector<double> vec;
    double e = exp(1.0);
    double sum = 0.0;
    double mean, variance, squaresum;
    int vecmode, median;
    for(int i = 0; i < 50; i++){
//        e = e*pow(10,i);
        vec.push_back(int(e)%10);
        e-=int(e)%10;
        e*=10;
    }
    for (int i = 0; i< 50; i++){
        cout <<vec[i]<< " ";
        sum += vec[i];
        squaresum = squaresum + pow(vec[i] - mean, 2);
    }
    mean = sum/vec.size();
    median = vec[vec.size()/2];

    vector<int> histogram(51,0);
    for( int i=0; i<50; ++i )
        ++histogram[ vec[i] ];
    vecmode =  max_element( histogram.begin(), histogram.end() ) - histogram.begin();

    for (int i=0;i<50;i++)
        squaresum = squaresum + pow(vec[i]-mean,2);

    variance = squaresum/vec.size();

    cout <<endl <<"Sum is "<<sum<<endl;
    cout <<"Mean is "<<mean<<endl;
    cout<<"Mode is "<<vecmode<<endl;
    cout<<"Variance is "<<variance<<endl;

    return 0;
}
