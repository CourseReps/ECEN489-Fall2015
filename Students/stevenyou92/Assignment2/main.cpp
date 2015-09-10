#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct SpeedData {
public:
    string scale = "1.60934";
    double speed;
    int year;
} spd;
int main() {
    char inp[256];
    double myspeed, myscale;
    char scl[256];
    strcpy(scl, spd.scale.c_str());
    printf("Please enter a speed: ");
    fgets(inp, 256, stdin );
    myspeed = atof (inp);
    spd.speed = myspeed;
    cout << "Speed in mph: " << spd.speed << " mph" << endl;

    myscale = strtod (scl, NULL);
    spd.speed = myspeed * myscale;
    //cout << "Speed in kmph " << spd.speed << "kmph" << endl;
    fprintf(stdout, "The speed in kmph is: %f kmph \n\n", spd.speed);

    double spd1, spd2;
    char spdin1[256], spdin2[256];
    printf("Please enter speed 1: ");
    fgets(spdin1, 256, stdin);
    spd1 = atof(spdin1);
    printf("\nPlease enter speed 2: ");
    fgets(spdin2, 256, stdin);
    spd2 = atof(spdin2);
    if(spd1 > spd2){
        printf("\nThe smaller speed is: %f", spd2);
    }
    if(spd2 > spd1){
        printf("\nThe smaller speed is: %f", spd1);
    }
    return 0;
}