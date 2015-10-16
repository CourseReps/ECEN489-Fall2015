
#include <SoftwareSerial.h>
// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensor = 3;    //The pin location of the sensor
int motorPin = 6;
int IRPin = A2;
int valvePin =11 ;

double getFlowRate(int volt)
{
  return 441.81098901099+3.5575091575092*volt;
}

SoftwareSerial BTSerial(9,10);

void rpm ()     //This is the function that the interupt calls 
{ 
 NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 
// The setup() method runs once, when the sketch starts

String solenoidState = "ON";

void setup() //
{ 
 pinMode(valvePin, OUTPUT);
 pinMode(IRPin, INPUT);
 pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
 Serial.begin(9600); //This is the setup function where the serial port is initialised,
 BTSerial.begin(57600);
 attachInterrupt(digitalPinToInterrupt(hallsensor), rpm, RISING); //and the interrupt is attached
} 
// the loop() method runs over and over again,
// as long as the Arduino has power
int i=0;
int voltage = 0;
bool ignore = false;
void loop ()    
{

 if(Serial.available())
 {
   int x = Serial.parseInt();
   if(!ignore)
   {
     voltage = x;
     ignore = true;
   }
   else
   {
     ignore = false;
   }
 }

 analogWrite(motorPin, map(voltage,0,1000,0,255));
 NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
 sei();            //Enables interrupts
 //digitalWrite(valvePin, HIGH);
 delay(500);
 //digitalWrite(valvePin, LOW);
 delay(500);
 cli();            //Disable interrupts
 
 Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 

Serial.print("5,");
Serial.print(analogRead(IRPin));
Serial.print(",");
//Serial.print(getFlowRate(voltage));
Serial.print(voltage);
Serial.print(",");
Serial.print(Calc, DEC);
Serial.print(",");
Serial.println(solenoidState);

i++;
}
