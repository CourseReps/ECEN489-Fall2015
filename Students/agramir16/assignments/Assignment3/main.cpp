#include <iostream>
#include <vector>

using namespace std;

int main() {

    vector<double> mph;
    vector<double> kph;
    double speed;

    cout << "Enter a speed then press enter. You may enter as many speeds as you would like." << endl
            << "Once your desired number of speeds have been inputed, enter 'mph'" << endl;
    while (cin >> speed){
        mph.push_back(speed);
    }
    cout << "The mph vector size is : "<< mph.size() << endl;

    for (double i : mph){
        speed=i*1.6094;
        kph.push_back(speed);
    }
    cout << "The km/h vector size is: "<< kph.size() << endl;

    for (int x=0; x != kph.size(); ++x) {
        cout << mph[x] << " mph = " << kph[x] << " km/h" << endl;
    }
    cout << endl;

    return 0;

}