#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using namespace std;

int main() {
    double median;
    double mode;
    double e [50] = {2, 7, 1, 8, 2, 8, 1, 8, 2, 8, 4, 5, 9, 0, 4, 5, 2, 3, 5, 3, 6, 0, 2, 8, 7, 4, 7, 1, 3, 5, 2, 6, 6, 2, 4, 9, 7, 7, 5, 7, 2, 4, 7, 0, 9, 3, 6, 9, 9, 9};
    size_t size = 50;
    vector <double> a(size);

    for (int i =0; i<size; ++i)
    {
        a[i] = e[i];
        cout << a[i] << " ";
    }
    double sum = accumulate(a.begin(), a.end(), 0.0);
    double mean = sum / a.size();
    cout << "\nThe mean of the vector is: "<< mean << endl;
    sort(a.begin(), a.end());

    if (size  % 2 == 0)
    {
        median = (a[size / 2 - 1] + a[size / 2]) / 2;
    }
    else
    {
        median = a[size / 2];
    }
    cout << "The median of the vector is: "<< median << endl;


    int number = a[0];
    int mode2 = number;
    int count = 1;
    int countMode = 1;

    for (int i=1; i<size; i++)
    {
        if (a[i] == number)
        {
            count++;
        }
        else
        {
            if (count > countMode)
            {
                countMode = count;
                mode2 = number;
            }
            count = 1;
            number = a[i];
        }
    }

    cout << "The mode of the vector is: " << mode2 << endl;

    double variance = 0;
    for(int i = 0;i<size;++i)
    {
        variance += (a[i]-mean)*(a[i]-mean);
    }
    variance = variance/size;
    cout << "The Variance of the vector is: "<< variance << endl;
}