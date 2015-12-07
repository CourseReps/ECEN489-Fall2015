#include <PID_v1.h>
#include <SoftwareSerial.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint,2.3,5,0, DIRECT);

int MotorPin = 3;
int IRPin = A0;
unsigned long start_t = 0;
unsigned long end_t = 0;
SoftwareSerial BTSerial(9,10);
int solenoidState = 1;
volatile int NbTopsFan;
int Calc; 
int hallsensor = 5;
int steadyCount = 0;
float lastVal = 0.0;
float threshold = 1.0;
float currentVolume = 0.0;
int steadyThreshold = 5;
bool isSteady = false;
bool measureVolume = false;
float volume = 0.0;
float steadyFlowRate = 35.0;
void rpm ()     //This is the function that the interupt calls 
{ 
 NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 

void setup()
{
  //initialize the variables we're linked to
  pinMode(hallsensor, INPUT);
  Input = analogRead(IRPin);
  Setpoint = getAnalogValue(10);
  BTSerial.begin(9600);
  Serial.begin(9600);
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
  attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached

  //turn on LED
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
}

int getHeight()
{
  return analogRead(IRPin);
}

float getAnalogValue(int val)
{
  return (91.16084 + 40.91384*val -7.943432*val*val + 0.6022727*val*val*val);
}

int numLoops = 10;

void loop()
{
  Input = analogRead(IRPin);
  //Serial.println(Input);
  myPID.Compute();
  /*
  sei();            //Enables interrupts
 cli();
 */
 if(numLoops == 10)
 {
 // end_t = millis();
  cli();
  Calc = (NbTopsFan * 60 / 7.5)/(0.5);
  sei();
  //start_t = millis();
  numLoops = 0;
  NbTopsFan = 0; 
  //Serial.println(Calc, DEC);
  if((Calc - lastVal) > threshold) {
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
     steadyCount ++;
     if(steadyCount > steadyThreshold){
       if(!isSteady) {
         //stop counting, send estimated volume
         measureVolume = false;
         currentVolume = volume;
       }
       isSteady = true;
     }
       
   }
   if(measureVolume) {
      volume += (Calc-steadyFlowRate);
   }
    
 }
  //If serial is available, get input and convert to actual value to set
  if(Serial.available())
  {
    int t = Serial.parseInt();
    if(t >=5 && t<=15)
      Setpoint = getAnalogValue(t);
  }
  
  analogWrite(MotorPin,map(Output,0,255,0,120));
  Serial.print(volume);
  Serial.print(" , ");
  Serial.print(Calc,DEC);
  Serial.print(" , ");
  Serial.println( steadyCount);
  //analogWrite(MotorPin,0);
  //Serial.println(map(Output,0,255,0,60));
  numLoops ++;
  delay(50);
}
