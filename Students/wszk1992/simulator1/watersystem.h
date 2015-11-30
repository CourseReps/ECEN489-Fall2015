#include <math.h>
#include <string>
#include <iostream>
#include "macro_value.h"
using namespace std;
class Watersystem
{
public:
    double h1,h2;
    double d1,d2,d3;
    double pumpvolt1,pumpvolt2;
    double flowrate1,flowrate2;
    double Qin1,Qin2,Qout1,Qout2,Q3;

    double dQ1,dQ2;
    double Vol1,Vol2;
    
    Watersystem(double init_h1, double init_h2, double dd1, double dd2, double dd3);
    double volt_to_flowrate(double);
    void compute();
    void printheight();
    int put_item(double volumn);
    
    void set_d1(double dd1);
    void set_d2(double dd2);
    void set_d3(double dd3);
    bool check_volumn(double volumn);
};
