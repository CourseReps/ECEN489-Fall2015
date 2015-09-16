#include <iostream>
#include <vector>

using namespace std;

int main() {
    cout << "Enter a list of speeds in mph, stop by typing -1:" << endl;
    const double kmh = 1.60934;
    double speed;
    vector<double> speeds;
    while (speed != -1) {
        cin >> speed;
        speeds.push_back(speed);
    }
    cout << "The size of speed you enter is: " << speeds.size()-1 << endl;
    cout << "The sets of speed you enter in mph are: " << endl;
    for(unsigned i = 0; i < speeds.size() - 1; i++)
        cout << speeds[i] << "  ";
    cout << endl << "The sets of speed in km/h:" << endl;
    for(unsigned i = 0; i < speeds.size() - 1; i++)
        cout << speeds[i] * kmh << "  ";
    cout << endl;
    return 0;
}