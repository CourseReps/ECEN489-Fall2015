#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int main() {
    double e [50] = {2, 7, 1, 8, 2, 8, 1, 8, 2, 8, 4, 5, 9, 0, 4, 5, 2, 3, 5, 3, 6, 0, 2, 8, 7, 4, 7, 1, 3, 5, 2, 6, 6,
                     2, 4, 9, 7, 7, 5, 7, 2, 4, 7, 0, 9, 3, 6, 9, 9, 9};
    size_t size = 50;
    vector <double> vdbl(size);
    double median;
    double mode;

    for (int i =0; i<size; ++i)
    {
        vdbl[i] = e[i];

    }
    for (int dsp =0; dsp<size; ++dsp)
    {
        cout <<vdbl[dsp]<< endl;
    }
    double sum = accumulate(vdbl.begin(), vdbl.end(), 0.0);
    double mean = sum / vdbl.size();
    cout << "The mean of the vector is: "<< mean << endl;
    sort(vdbl.begin(), vdbl.end());

    if (size  % 2 == 0)
    {
        median = (vdbl[size / 2 - 1] + vdbl[size / 2]) / 2;
    }
    else
    {
        median = vdbl[size / 2];
    }
    cout << "The Median of the vector is: "<< median << endl;
    vector <double> histogram(9,0);
    for(int m =0; m< 50; ++m)
    {
        ++histogram[ vdbl[m]];
    }
    mode = max_element(histogram.begin(), histogram.end()) - histogram.begin();
    cout << "The Mode of the vector is: "<< mode << endl;
    double variance = 0;
    for(int i = 0;i<size;++i)
    {
        variance += (vdbl[i]-mean)*(vdbl[i]-mean)/size;
    }
    cout << "The Variance of the vector is: "<< variance << endl;
}