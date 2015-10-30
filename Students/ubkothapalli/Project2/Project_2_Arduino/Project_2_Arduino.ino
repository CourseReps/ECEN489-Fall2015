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
//int Level1 = 16;
//int Level2 = 17;
int FlowSens = 4;
int RelayCtrl = 15;
int IRin = 14;
int PumpPWM = 20;
int powerLED = 23;


double Setpoint, Input;
double Output; 
float Kp = 2;
float Ki = 3;
float Kd = 1;

//const int sampleRate = 250;
PID myPID(&Input,&Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FlowSens, INPUT);
  attachInterrupt(FlowSens, rpm, RISING);
  //pinMode(Level1, OUTPUT);
  //pinMode(Level2, OUTPUT);
  pinMode(PumpPWM, OUTPUT);
  pinMode(IRin, INPUT);
  pinMode(RelayCtrl, OUTPUT);
  myPID.SetMode(AUTOMATIC);
  //myPID.SetSampleTime(sampleRate);
  Setpoint = 1;
  
  myPID.SetOutputLimits(0, 250); 
  
  pinMode (powerLED,OUTPUT);

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
double voltToLevel(int levelSetpoint);
void loop() {
  
  digitalWrite(powerLED,HIGH);
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
    
    double IRtemp = map(IRvolt,0,650,0,400);    //Serial.print(IRvolt);
    //Serial.print("\t");
      delay (10);

    int commandSetpoint=6.5;
    
    Setpoint = voltToLevel(commandSetpoint);
    
    if (BTSerial.available()){
    
      char testCommand = BTSerial.read();
      if (testCommand == 'y'){
        BTSerial.write('K');
      }
    }
    
    /*
    if (Serial.available()){
  
      int TempSetpoint = Serial.parseInt();
      if (TempSetpoint == 0){
        Setpoint = 0;
      }
      else if (TempSetpoint == 1){
        Setpoint = 1;
      }
      else if (TempSetpoint==2){
       Setpoint = 2;
      }
      else if (TempSetpoint == 3){
        Setpoint = 3;
      }
      else if (TempSetpoint==4){
       Setpoint = 4;
      }
      else if (TempSetpoint == 5){
        Setpoint = 5;
      }
      else if (TempSetpoint==6){
       Setpoint = 6;
      }
      else if (TempSetpoint == 7){
        Setpoint = 7;
      }
      else if (TempSetpoint==8){
       Setpoint = 8;
      }
	else if (TempSetpoint==9){
       Setpoint = 9;
      }
      else if (TempSetpoint==10){
       Setpoint = 10;
      }
      else if (TempSetpoint==11){
       Setpoint = 11;
      }
      else if (TempSetpoint==12){
       Setpoint = 12;
      }

      //level = tempinput;
    
        }
        
	else { 
	Setpoint = 4; 
	}
*/
    Input = IRtemp;
   
    
    
     //Serial.print(level);
    //Serial.print("\t");
    myPID.Compute();
    Serial.print(Output);
    Serial.print("\t");
    int Output1 = map(Output,0,250,0,100);
    OutFlowRt = Output1;
  
    Serial.print(Input);
    Serial.print("\t");
    Serial.print((Setpoint));
    Serial.print("\t");
    
    //Serial.print(FlowSens);
    //Serial.print("\t");
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


double voltToLevel(int levelSetpoint){
  double dist;
  //int level1 = 0;
  dist = 1193.8* pow(levelSetpoint,-0.905);
  //dist = 5.3041308916 * pow(voltage,-1.1563920336);
  //Serial.print(dist);
  // Serial.print("\t");
  /*
  if (dist >= 14.0 ){
    level1 = 12;
    
  }
  else if (dist >= 12.0 && dist < 13.0){
      level1 = 11;
     
  }
  
  else if (dist >= 11.0 && dist < 12.0){
      level1 = 10;
     
  }
  
  else if (dist >= 10.0 && dist < 11.0){
      level1 = 9;
     
  }
  
  else if (dist >= 9.0 && dist < 10.0){
      level1 = 8;
     
  }

  else if (dist >= 8.0 && dist < 9.0){
      level1 = 7;
     
  }
  else if (dist >= 7.0 && dist < 8.0){
      level1 = 6;
     
  }
  else if (dist >= 6.0 && dist < 7.0){
      level1 = 5;
     
  }
  else if (dist >= 5.0 && dist < 6.0){
      level1 = 4;
     
  }
  else if (dist >= 4.0 && dist < 5.0){
      level1 = 3;
     
  }
  else if (dist >= 3.0 && dist < 4.0){
      level1 = 2;
     
  }
  else if (dist >= 2.0 && dist < 3.0){
      level1 = 1;
     
  }

  else if (dist < 2.0 ){
      level1 = 0;
   
  }
  */
  dist = map(dist,0,650,0,400);
  return dist;
}

