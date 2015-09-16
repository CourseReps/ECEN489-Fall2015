#include <iostream>
#include <vector>
using namespace std;


int main() {
    vector<double> speeds;
    vector<double> speedskph;
    double s;

    cout << "please input any number of speeds terminated with an \"end\"" << endl;
    while (cin >> s){
        speeds.push_back(s);
    }
    cout << "The size of your vector is: "<< speeds.size() << endl;
    cout << " Here is your vector in mph: " << endl;
    for (int x=0; x != speeds.size(); ++x){
        cout << speeds[x] << " ";
    }
    cout << endl;
    for (double i : speeds){
        s=i*1.6094;
        speedskph.push_back(s);
    }
    cout << "The size of your vector is: "<< speedskph.size() << endl;
    cout << " Here is your vector in km/h: " << endl;
    for (int x=0; x != speedskph.size(); ++x) {
        cout << speedskph[x] << " ";
    }
    cout << endl;
    return 0;
}