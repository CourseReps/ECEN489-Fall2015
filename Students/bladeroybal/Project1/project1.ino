volatile int NbTopsFan; //measuring the rising edges of the signal
int Calc;                               
int hallsensor = 2;    //The pin location of the sensor

//PWM Integration
int in = 0;
int fan = 3;
int val = 0;
int PWMval = 0;

void rpm ()     //This is the function that the interupt calls 
{ 
  NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
} 


void setup() //
{ 
  pinMode(hallsensor, INPUT); //initializes digital pin 2 as an input
  Serial.begin(9600); //This is the setup function where the serial port is initialised,

  attachInterrupt(hallsensor, rpm, RISING); //and the interrupt is attached
  
  //Adding PWM stuff
  pinMode(fan, OUTPUT);
} 
// the loop() method runs over and over again,
// as long as the Arduino has power
double value;

void loop ()    
{
  NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
  sei();      //Enables interrupts
  delay (1000);   //Wait 1 second
  cli();      //Disable interrupts
  Calc = (NbTopsFan * 60 / 7.5); //(Pulse frequency x 60) / 7.5Q, = flow rate 

//in L/hour 
  Serial.print (Calc, DEC); //Prints the number calculated above
  Serial.print (" L/hour\r\n"); //Prints "L/hour" and returns a  new line

//in L/minute
  Serial.print (Calc/60, DEC);
  Serial.print (" L/minute\r\n"); //Prints L/min and returns a new line
  
  //adding PWM stuff
  PWMval = 100;  //and to use for PWM
  analogWrite(fan,PWMval);            //sets the PWM for the fan
  delay(15);
}

//double pumpConvertF = 98*Q;
