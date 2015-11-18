#include <math.h>
#include <string>
#include <iostream>
using namespace std;
class Watersystem
{
public:
    #define Pi 3.1416
    #define Fi 0.95
    #define YITA 0.0008
    #define G 9.8
    #define D1 0.010
    #define D2 0.010
    #define D3 0.008
    #define R1 0.04
    #define R2 0.04
    #define T1 0.01
    #define T2 1
    #define L 0.05
    #define STONE_HEIGHT 0.05
    #define ONEHIGH -1
    #define TWOHIGH 1
    
    double h1,h2;
    double pumpvolt1,pumpvolt2;
    double flowrate1,flowrate2;
    double Qin1,Qin2,Qout1,Qout2,Q3;
    
    Watersystem(double,double);
    double volt_to_flowrate(double);
    void compute();
    void printheight();
    void put_item(int);
    
private:

    double dQ1,dQ2;
    double Vol1,Vol2;
};
