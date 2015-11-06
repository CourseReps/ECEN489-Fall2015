
#include <PID_v1.h>

double Setpoint, Input, Output; 
float threshold1 = 7;

const int level1  = A0;
const int level2 = A1;
int motorPin = 6;

float Kp = 5;
float Ki = 3;
float Kd = 2;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup() {
  // put your setup code here, to run once:
  Input = getHeight();
    Setpoint = 1;
    
    

}

int getHeight(){
  float lev1 = analogRead(level1);
  float lev2 = analogRead(level2);
  int height = 0;
  if (lev1 >= threshold1){
    height++; 
    if (lev2>= threshold1){
      height ++;
    }
  }
return height;
}

void loop() {
  Input = getHeight();
  Setpoint = 1;
  myPID.Compute();
  analogWrite(motorPin, Output);
  
  Serial.print(Input);
  Serial.print(Setpoint);
  Serial.println(Output);
  // put your main code here, to run repeatedly:

}
