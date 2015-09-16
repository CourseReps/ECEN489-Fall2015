#include <iostream>
#include <vector>

using namespace std;

int main() {
    vector<double> input;
    int speed;
    cout << "Type your speeds in mph and press Enter after each speed: " <<endl;
    cout << "Type '999' when finished";
    while (speed != 999){
        cin >>speed;
        input.push_back(speed);
    }
    cout << input.size() << "entries" <<endl;

    for (int i = 0; i<=input.size(); ++i){
        input[i] = input[i]*1.609344;
    }

    cout << input;

    return 0;
}