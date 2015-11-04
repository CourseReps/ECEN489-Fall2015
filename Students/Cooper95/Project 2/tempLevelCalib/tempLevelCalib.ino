#include <SoftwareSerial.h>

/*
 * Pin 04: Input from flow rate sensor
 * Pin 09: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 10: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 * Pin 14: IR input
 * Pin 15: Relay control output
 * Pin 20: PWM output for pump
 */

SoftwareSerial BTSerial(9,10);
int Level1 = 16;
int Level2 = 17;
int FlowSens = 4;
int RelayCtrl = 15;
int IRin = 14;
int PumpPWM = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FlowSens, INPUT);
  attachInterrupt(FlowSens, rpm, RISING);
  pinMode(Level1, OUTPUT);
  pinMode(Level2, OUTPUT);
  pinMode(PumpPWM, OUTPUT);
  pinMode(IRin, INPUT);
  pinMode(RelayCtrl, OUTPUT);
}

//initialize variables 
volatile int edges = 0;
int level = 0;
int count = 0;
float freq = 0;
float OutFlowRt = 0;
float InFlowRt = 0;
int SolState = 0;
bool RecData = false;
static String BTsend = "";

//define calibration constants
const float Ap = -9.896452;
const float Bp =  53.94933;
const float Cp = -.7948750;
const float Dp =  15.38008;

//declare functions
int LpmToPWM(float LPerMin);
void GatherData();
void rpm ();

void loop() {
  
  count = count + 1;
  if(count > 1000){
    if(analogRead(Level2) > 7){
      level = 2;
    } else if (analogRead(Level1) > 7){
      level = 1;
    } else {
      level = 0;
    }
    Serial.println(level);
    cli();
    freq = edges * 4;
    sei();
    count = 0;
    edges = 0;
  }
  delay(1);
  
  analogWrite(PumpPWM, LpmToPWM(OutFlowRt));
  digitalWrite(RelayCtrl, SolState);
}
//Define functions
void rpm(){
  edges = edges + 1;
}

//Convert from Liters per Minute to PWM value based on calibrated eqn.
int LpmToPWM(float LPerMin){
  float lpm;
  if (LPerMin > 3.5)
    lpm = 3.5;
  else if (LPerMin < 0)
    lpm = 0;
  else
    lpm = LPerMin;
  if(lpm ==0)
    return 0;
  else
    return (int) (Ap*pow(lpm,3) + Bp*pow(lpm,2) + Cp*lpm + Dp); 
}

void GatherData(){
  //BTsend should be in the order: IR range, Pump rate, Flow rate, Solenoid state
  //measure IR sensor
  //Serial.println(analogRead(IRin));
  if(analogRead(IRin) > 500)
    BTsend += '0';
  else
    BTsend += '1';
  BTsend += ',';
  
  //current output to pump
  BTsend += String(OutFlowRt);
  BTsend += ',';
  
  //Flow rate = frequency / 33 Hz/lpm
  InFlowRt = freq / 33;
  BTsend += String(InFlowRt);
  BTsend += ',';
  
  //current output to solenoid
  BTsend += String(SolState);
  Serial.println(BTsend);
}

