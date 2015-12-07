
volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensor = 4;    //The pin location of the sensor
int pump = 20;
int irsensor = 14;
int pumprate = 0;


void rpm ()     
{ 
 NbTopsFan++;


} 
// The setup() method runs once, when the sketch starts
void setup() //
{ 
 pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
 pinMode(irsensor, INPUT);
 Serial.begin(9600);
 attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached
} 


int getFlowRate(){
  NbTopsFan = 0;      //Set NbTops to 0 ready for calculations
   sei();            //Enables interrupts
   delay (1000);      //Wait 1 second
   cli();            //Disable interrupts
   Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 
   return Calc;
}

/*
void setPumpRate(int rate){
  pumprate=rate;
  analogWrite(pump, pumprate);
}



int getIRSensor(){
  int val = analogRead(irsensor);
  return val;
}
*/

void loop ()    
{
  
  int pumppwm = 10;
  int temprate=0;
  int flow=0;
  if (Serial.available()){
    char comm = Serial.read();
    if (comm == 'y'){ 
  while (pumppwm <= 250){
     analogWrite(pump,pumppwm);
     for (int k=0;k<5;k++){
       temprate = getFlowRate();
       flow = flow + temprate;
     } 
     flow = flow/5;
     Serial.print(pumppwm);
     Serial.print("\t");
     Serial.println(flow);
     pumppwm = pumppwm + 5;
     
  }
    }
  }
   
   
   
   
   
   
}
