#include <PID_v1.h>

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


double Setpoint, Input;
double Output; 
float Kp = 20;
float Ki = 5;
float Kd = 2;

const int sampleRate = 250;
PID myPID(&Input,&Output, &Setpoint, Kp, Ki, Kd, DIRECT);

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
  myPID.SetMode(AUTOMATIC);
  myPID.SetSampleTime(sampleRate);
  Setpoint = 1;
  
  myPID.SetOutputLimits(0, 250); 
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
int voltToLevel(double voltage);
void loop() {
  
  //count = count + 1;
  //if(count > 1000){
   /* if(analogRead(Level2) > 7){
      level = 2;
    } else if (analogRead(Level1) > 7){
      level = 1;
    } else {
      level = 0;
    }
    */
    
    double IRvolt = analogRead(IRin);
    level = voltToLevel(IRvolt);
    
    Setpoint = 4;
    if (Serial.available()){
  
      char TempSetpoint = Serial.read();
      if (TempSetpoint == '0'){
        Setpoint = 0;
      }
      else if (TempSetpoint == '1'){
        Setpoint = 1;
      }
      else if (TempSetpoint=='2'){
       Setpoint = 2;
      }
      else if (TempSetpoint == '3'){
        Setpoint = 3;
      }
      else if (TempSetpoint=='4'){
       Setpoint = 4;
      }
      else if (TempSetpoint == '5'){
        Setpoint = 5;
      }
      else if (TempSetpoint=='6'){
       Setpoint = 6;
      }
      else if (TempSetpoint == '7'){
        Setpoint = 7;
      }
      else if (TempSetpoint=='8'){
       Setpoint = 8;
      }
      //level = tempinput;
    
        }
    Input = level;
    
    
     Serial.print(level);
    Serial.print("\t");
    myPID.Compute();
    int Output1 = map(Output,0,350,10,250);
    OutFlowRt = Output1;
  
    
    Serial.print((Setpoint));
    Serial.print("\t");
    Serial.print(Input);
    Serial.print("\t");
    Serial.print(FlowSens);
    Serial.print("\t");
    Serial.println(OutFlowRt);
    cli();
    freq = edges * 4;
    sei();
    count = 0;
    edges = 0;
  //}
  delay(500);
    analogWrite(PumpPWM, (OutFlowRt));

  //analogWrite(PumpPWM, LpmToPWM(OutFlowRt));
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


int voltToLevel(double voltage){
  double dist;
  int level1 = 0;
  dist = 5.3041308916 * pow(voltage,-1.1563920336);
  if (dist >= 14.0 ){
    level1 = 0;
    
  }
  else if (dist >= 12.0 && dist < 14.0){
      level1 = 2;
     
  }
  
  else if (dist >= 10.0 && dist < 12.0){
      level1 = 3;
     
  }
  
  else if (dist >= 8.0 && dist < 10.0){
      level1 = 4;
     
  }
  
  else if (dist >= 6.0 && dist < 8.0){
      level1 = 5;
     
  }

  else if (dist >= 4.0 && dist < 6.0){
      level1 = 6;
     
  }
  else if (dist >= 2.0 && dist < 4.0){
      level1 = 7;
     
  }
  else if (dist < 2.0 ){
      level1 = 8;
   
  }
  return level1;
}

