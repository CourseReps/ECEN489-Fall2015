// reading liquid flow rate using Seeeduino and Water Flow Sensor from Seeedstudio.com
// Code adapted by Charles Gantt from PC Fan RPM code written by Crenn @thebestcasescenario.com
// http:/themakersworkbench.com http://thebestcasescenario.com http://seeedstudio.com

volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensor = 5;    //The pin location of the sensor
int pump = 6;
int irsensor = 7;
int pumprate = 0;
int solenoidstate=0;

void rpm ()     //This is the function that the interupt calls 
{ 
 NbTopsFan++;  //This function measures the rising and falling edge of the 

} 
// The setup() method runs once, when the sketch starts
void setup() //
{ 
 pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
 pinMode(irsensor, INPUT);
 Serial.begin(9600);
 attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached
} 
// the loop() method runs over and over again,
// as long as the Arduino has power

int getFlowRate(){
  NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
   sei();            //Enables interrupts
   delay (1000);      //Wait 1 second
   cli();            //Disable interrupts
   Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 
   return Calc;
}

void setPumpRate(int rate){
  pumprate=rate;
  analogWrite(pump, pumprate);
}

void setSolenoidState(int state){
  solenoidstate=state;
}

int getIRSensor(){
  int ret = digitalRead(irsensor);
  return ret;
}

void loop ()    
{
    int r = 255;
    setPumpRate(r);
    delay(3000);
   for(int i=0; i<5; i++){
     Serial.println("Pump rate: "+String(r)+"\n\n");
     setPumpRate(r);
     int sum=0;
     for(int j=0; j<5; j++){
       int currate = getFlowRate();
       Serial.println(currate);
       sum+=currate;
       //delay(50);
     }
     double avg = sum/5.0;
     Serial.println("Average: "+String(avg)+"\n\n");
     r-=(255/5);
     delay(1000);
   }
   
}
