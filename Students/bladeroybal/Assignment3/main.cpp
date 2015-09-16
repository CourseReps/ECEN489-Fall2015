#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<double> input;
    vector<double> kph;

    int speed;
    cout << "Type your speeds in mph and press Enter after each speed: " <<endl;
    cout << "Type '999' when finished" << endl;
    while (speed != 999){
        cin >>speed;
        input.push_back(speed);
    }
    cout << input.size() << "entries" <<endl;

    for (int i = 0; i<input.size(); ++i){
        cout << input[i] << " mph" << endl;
    }

    for (auto i : input){
        speed = i*1.609344;
        kph.push_back(speed);
    }

    cout << kph.size() << "entries" <<endl;

    for (int i = 0; i<kph.size(); ++i){
        cout << kph[i] << " km/h" << endl;
    }

    return 0;
}