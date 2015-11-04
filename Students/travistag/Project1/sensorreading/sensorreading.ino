#include <PID_v1.h>

// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensor = 5;    //The pin location of the sensor
int pump = 20;
int irsensor = 14;
int solenoid = 2;
int pumprate = 0;
int solenoidstate=1;
double qval = 50;
double Setpoint, Input, Output;
bool justread = false;
PID myPID(&Input, &Output, &Setpoint,3,5,1, DIRECT);

double getHeight(){
  int rea = analogRead(irsensor);
  //Serial.println(rea);
  if(rea>0){
    double re = rea/1.0;
    double hei = 8283.4*pow(re, -1.334);
    return 17-hei;
  }
  else
    return 0;
}

int pumpval = 0;

void rpm ()     //This is the function that the interupt calls 
{ 
 NbTopsFan++;  //This function measures the rising and falling edge of the 

} 
// The setup() method runs once, when the sketch starts
void setup() //
{ 
 pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
 pinMode(irsensor, INPUT);
 pinMode(solenoid, OUTPUT);
 Serial2.begin(9600); //This is the setup function where the serial port is 
 Serial.begin(9600);
 attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached
 Input = getHeight();
  Setpoint = 0;
  //turn the PID on
  myPID.SetMode(AUTOMATIC);
} 
// the loop() method runs over and over again,
// as long as the Arduino has power

int getFlowRate(){
  NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
   sei();            //Enables interrupts
   delay (1000);      //Wait 1 second
   cli();            //Disable interrupts
   Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 
   return Calc/60;
}

void setPumpRate(int rate){
  pumprate=rate;
  analogWrite(pump, pumprate);
}

void setSolenoidState(int state){
  solenoidstate=state;
  if(state==0)
    digitalWrite(solenoid, LOW);
  else
    digitalWrite(solenoid, HIGH);
}

int getIRSensor(){
  int ret = digitalRead(irsensor);
  return ret;
}

String getDataString(){
  String s = "";
  int ir = getIRSensor();
  setSolenoidState(ir);
  s+=String(ir);
  s+=", ";
  s+=String(pumprate);
  s+=", ";
  s+=String(0);
  s+=", ";
  s+=String(solenoidstate);
  return s;
}

void loop ()    
{
  Input = getHeight();
 
  myPID.Compute();
  setPumpRate(Output/2);
   if(Serial2.available()){
     if(justread ==false){
       int p = Serial2.parseInt();
       Setpoint=p;
       Serial.println("Setpoint to "+String(p));
       justread = true;
     }
     else{
       justread = false;
       Serial2.parseInt();
     }
   }
   String dat = getDataString();
   Serial2.println(dat);
   //Serial.println(dat);
}
