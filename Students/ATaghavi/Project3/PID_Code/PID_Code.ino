#include <PID_v1.h>
#include <SoftwareSerial.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,80,3,2, DIRECT);

int MotorPin = 3;
int IRPin = 15;
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
  
  Input = getHeightAvg();
  Setpoint = 10;
  BTSerial.begin(9600);
  Serial.begin(9600);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached

  //turn on LED
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

double getHeight2(){
  int rea = getHeight();
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

int getHeight()
{
  return analogRead(IRPin);
}

float getAnalogValue(float val)
{
  return (91.16084 + 40.91384*val -7.943432*val*val + 0.6022727*val*val*val);
}

int numLoops = 10;

void updateHeights()
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
  
  heights[4] = getHeight2();
}

void loop()
{
  updateHeights();
  
  Input = getHeightAvg();
  //Serial.println(Input);
  myPID.Compute();
  
  
  
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
  float heightAvg = getHeightAvg();
  /*
  if( (!isSteady &&(steadyCount == 0 && abs( Input - lastVal) > threshold ) || (steadyCount > 0 && abs(Input - firstSteadyVal) > threshold)  ) || 
      (isSteady && abs(Input - steadyAvg) > threshold) ) {
        */
  if(abs(heightAvg - Setpoint) > threshold) {
    steadyCount = 0;
    
    if(isSteady)
    {
      volume = 0.0;
      currentVolume = 0.0;
      measureVolume = true;
    }
    
    isSteady = false;
   }
   else {
     if(steadyCount == 0)
     {
       firstSteadyVal = heightAvg;
     }
     steadyCount ++;
     measureVolume = false;
     currentVolume = volume/24.5;
     if(steadyCount > steadyThreshold){
       isSteady = true;
     }
      
   }
   if(measureVolume) {
      volume -= (Calc-steadyFlowRate)/2;
   }
   lastVal = heightAvg;
 }
  //If serial is available, get input and convert to actual value to set
  if(Serial.available())
  {
    int t = Serial.parseInt();
    if(t >=5 && t<=15)
      Setpoint = t;
  }
  
  analogWrite(MotorPin,map(Output,0,255,0,150));
  
  Serial.print("Team2");
  Serial.print(",");
  Serial.print(Input);
  Serial.print(",");
  Serial.print(Output);
  Serial.print(",");
  Serial.print( Calc/24.5, DEC );
  Serial.print(",");
  Serial.print( currentVolume);
  Serial.print(",");
  Serial.println(isSteady);
  
  //analogWrite(MotorPin,0);
  //Serial.println(map(Output,0,255,0,60));
  numLoops ++;
  delay(1);
}