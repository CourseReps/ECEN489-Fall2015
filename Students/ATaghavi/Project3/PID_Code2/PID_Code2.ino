#include <PID_v1.h>
#include <SoftwareSerial.h>

//Define Variables we'll be connecting to
double Setpoint1, Input1, Output1;
double Setpoint2, Input2, Output2;

//Specify the links and initial tuning parameters
PID myPID(&Input1, &Output1, &Setpoint1,80,3,2, DIRECT);
PID myPID(&Input2, &Output2, &Setpoint2,80,3,2, DIRECT);

int MotorPin1 = 3;
int MotorPin2 = 4;
int IRPin1 = 15;
int IRPin2 = 14;
unsigned long start_t = 0;
unsigned long end_t = 0;
SoftwareSerial BTSerial(9,10);
int solenoidState = 1;
volatile int NbTopsFan;
int Calc; 
int hallsensor = 5;
int steadyCount = 0;
float lastVal = 0;
float threshold = 0.5;
float currentVolume = 0.0;
float steadyAvg = 0.0;
int steadyThreshold = 10;
bool isSteady = false;
bool measureVolume = false;
float volume = 0.0;
float steadyFlowRate = 850.0;
float firstSteadyVal = 0.0;

float heights[5];

void rpm ()     //This is the function that the interupt calls 
{ 
 //Serial.println("*");
 NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 

void setup()
{
  //initialize the variables we're linked to
  pinMode(hallsensor, INPUT);
  
  Input1 = getHeight2();
  Input2 = getHeight2();
  Setpoint1 = 10;
  Setpoint2 = 10;
  BTSerial.begin(9600);
  Serial.begin(9600);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached

  //turn on LED
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

double getHeight2(int a){
  int rea = getHeight(a);
  //Serial.println(rea);
  if(rea>0){
    double re = rea/1.0;
    double hei = 8283.4*pow(re, -1.334);
    return 17-hei;
  }
  else
    return 0;
}

float getHeightAvg() {
  
  float sum = 0.0;
  int high = 0;
  int low = 10000;
  for(int i=0; i<5; i++)
  {
    if(heights[i] > high)
      high = heights[i];
    if(heights[i] < low)
      low = heights[i];
    sum+=heights[i];
  }
  
  //sum -= high+low;
  return (float)sum/5.0;
}

int getHeight(int a)
{
  if(a==1)
    return analogRead(IRPin1);
  if(a==2)
    return analogRead(IRPin2);
}

float getAnalogValue(float val)
{
  return (91.16084 + 40.91384*val -7.943432*val*val + 0.6022727*val*val*val);
}

int numLoops = 10;

void updateHeights(int a)
{
  /*
  heights[0] = heights[1];
  heights[1] = heights[2];
  heights[2] = heights[3];
  heights[3] = heights[4];
  */
  
  for(int i=0; i<4; i++) {
    heights[i] = heights[i+1];
  }
  
  heights[4] = getHeight2(a);
}

void loop()
{
  updateHeights(1);
  updateHeights(2);
  
  Input1 = getHeightAvg(1);
  Input2 = getHeightAvg(2);
  //Serial.println(Input);
  myPID1.Compute();
  myPID2.Compute();
  
  
  
 if(numLoops >= 500 )
 {
 // end_t = millis();
  cli();
  //delay(1000);
  Calc = (NbTopsFan * 60 / 7.5)*2;
  sei();
  Calc = (NbTopsFan * 60 / 7.5)*2;
  //Serial.println(Calc,DEC);
  //start_t = millis();
  numLoops = 0;
  NbTopsFan = 0; 
  //Serial.println(Calc, DEC);
  float heightAvg = getHeightAvg(1);
  /*
  if( (!isSteady &&(steadyCount == 0 && abs( Input - lastVal) > threshold ) || (steadyCount > 0 && abs(Input - firstSteadyVal) > threshold)  ) || 
      (isSteady && abs(Input - steadyAvg) > threshold) ) {
        */
   lastVal = heightAvg;
 }
  //If serial is available, get input and convert to actual value to set
  if(Serial.available())
  {
    int t = Serial.parseInt();
    if(t >=5 && t<=15)
      Setpoint = t;
  }
  
  analogWrite(MotorPin1,map(Output1,0,255,0,150));
  analogWrite(MotorPin2,map(Output2,0,255,0,150));
  
  Serial.print("Team2a");
  Serial.print(",");
  Serial.print(Input1);
  Serial.print(",");
  Serial.print(Output1);
  Serial.print(",");
  Serial.println( Calc/24.5, DEC );
  
  Serial.print("Team2b");
  Serial.print(",");
  Serial.print(Input2);
  Serial.print(",");
  Serial.print(Output2);
  Serial.print(",");
  Serial.println( Calc/24.5, DEC );

  
  //analogWrite(MotorPin,0);
  //Serial.println(map(Output,0,255,0,60));
  numLoops ++;
  delay(1);
}
