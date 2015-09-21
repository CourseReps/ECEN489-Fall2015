#include <iostream>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

double findmean(vector<double> vec)         // Function to find mean.
{   double fmean;
    double sum;
    for(auto it = vec.begin(); it <= vec.end()-1; ++it)
         sum += *it;
    fmean = sum/ vec.size();
    return fmean;
}

double findmedian(vector<double> vec)
{   if(vec.empty())return 0;
    else {
        sort(vec.begin(), vec.end());
        if(vec.size() % 2 == 0)
            return (vec[vec.size()/2-1] + vec[vec.size()/2]) / 2;
        else
            return vec[vec.size()/2];
    }
}

double findmode(vector<double> vec)
{
    double mode;
    double ocount = 0;
    for(auto ito = vec.begin(); ito <= vec.end()-1; ++ito){
        double icount = 0;
        for(auto iti = vec.begin(); iti <= vec.end()-1; ++iti) {
            if (*iti == *ito)
                ++icount;
        }
        if(icount > ocount) {
            ocount = icount;
            mode = *ito;
        }
    }
    return mode;
}

double findvariance(vector<double> vec, double mean)
{
    double variance;
    double expectnum;
    double summand;
    for(auto it = vec.begin(); it <= vec.end()-1; ++it){
        summand = pow((*it - mean), 2);
        expectnum += summand;
    }
    variance = expectnum/(vec.size()-1);
    return variance;
}

int main() {
  string ss;
    ss = "27182818284590452353602874713526624977572470936999";  // I tried calculating e using exp(1) but it wasn't
                                                                // accurate to the correct number of digits.
    vector<double> vec;                                         // Initialize vector to contain doubles.
    for(auto it = ss.begin(); it <= ss.end()-1; ++it){          // Iterates through the string and puts each digit into
        if(*it>+47)                                             // the vector as a double.
            vec.push_back(*it-48);                               // For some unknown reason this iterator gives each digit of 'e' + 48(value)???
    }
    //auto itervec = vec.begin();
    for(auto it = vec.begin(); it <= vec.end()-1; ++it)
        cout << *it;
    cout << endl;
    double mean = findmean(vec);
    double median = findmedian(vec);
    double mode = findmode(vec);
    double variance = findvariance(vec, mean);

    cout << "The mean of vec is: " << mean << endl;
    cout << "The median of vec is: " << median << endl;
    cout << "The mode of vec is: " << mode << endl;
    cout << "The variance of vec is: " << variance << endl;
}