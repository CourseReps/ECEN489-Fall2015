#include <iostream>
#include <vector>

using namespace std;

int main() {
    cout << "Enter a list of speeds in mph, terminated with a 'q':" << endl;
    const double kpm = 1.60934;
    string in;
    vector<double> speeds;
    do{
        cin >> in;
        try{
            speeds.push_back(stod(in));
        }
        catch(...){}
    } while (in != "q");
        cout << "You entered " << speeds.size() << " speeds:" << endl;
    for(double i: speeds)
        cout << i << " ";
        cout << endl << "Here are the same speeds in km/h:" << endl;
    for(double i: speeds)
        cout << i * kpm << " ";
        cout << endl;
    return 0;
}