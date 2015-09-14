#include <iostream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

int main(){
    vector<double> speedsMph;
    vector<double> speedsKm;
    double speed = 0;
    const double mphToKm = 1.60934;
    cout << "Enter speeds in mph: ";

    while(cin >> speed) {
        if (speed > 0)
            speedsMph.push_back(speed);
    }
    cout << "There are " << speedsMph.size() << " speeds stored." << endl;
    cout << "You entered: ";
    for(decltype(speedsMph.size()) i = 0; i <= speedsMph.size()-1; ++i)
        cout << speedsMph[i] << '\t';
    for(auto &d : speedsMph)
        d = d * mphToKm;
    cout << "\nYour speeds, after converting to km/h are: " << endl;
    for(decltype(speedsMph.size()) i = 0; i <= speedsMph.size()-1; ++i)
        cout << speedsMph[i] << endl;

}