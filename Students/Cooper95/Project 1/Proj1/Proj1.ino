#include <SoftwareSerial.h>

/*
 * Pin 04: Input from flow rate sensor
 * Pin 09: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 10: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 * Pin 12: Relay control output
 * Pin 14: IR input
 * Pin 20: PWM output for pump
 */

const String get_data = "get_data";
const String send_data = "send_data";

SoftwareSerial BTSerial(9,10);
int FlowSens = 4;
int RelayCtrl = 12;
int IRin = 14;
int PumpPWM = 20;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FlowSens, INPUT);
  attachInterrupt(FlowSens, rpm, RISING);
  pinMode(PumpPWM, OUTPUT);
  pinMode(IRin, INPUT);
  pinMode(RelayCtrl, OUTPUT);
}

//initialize variables 
volatile int edges = 0;
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

  //temp serial input code
  static String SerGet = "";
  if(Serial.available())
  {
    char temp = char(Serial.read());
    if(10 == int(temp))
    {
      temp = 0;
    }
    SerGet += temp;
    if(0 == int(temp))
      {
        //Parse SerGet and put data into OutFlowRt and SolState
        int commaIndex = SerGet.indexOf(',');
        OutFlowRt = SerGet.substring(0,commaIndex).toFloat();
        //Default the solenoid to true (or closed)
        if(SerGet.substring(commaIndex+1) == "false")
          SolState = false;
        else
          SolState = true;
        //set outputs
        SerGet = "";
        analogWrite(PumpPWM, LpmToPWM(OutFlowRt));
        digitalWrite(RelayCtrl, SolState);
      }
  }
  
  count = count + 1;
  if(count > 250){
    cli();
    freq = edges * 4;
    sei();
    //Serial.println(freq);
    count = 0;
    edges = 0;
  }
  delay(1);
  
  //set up BT transactions
  static String BTget = "";
  static unsigned short index = 0;
  
  //Wait for polling command from NUC
  if(BTSerial.available())
  {
    char temp = char(BTSerial.read());
    if(10 == int(temp))
    {
      temp = 0;
    }
    BTget += temp;
    index++;

    /* DEBUG CODE 
    Serial.print(index);
    Serial.print(": ");
    Serial.print(BTget);
    Serial.print(": ");
    Serial.println(int(temp));
    /**/
    if(RecData)
    {
      if(0 == int(temp))
      {
        //Parse BTget and put data into OutFlowRt and SolState
        int commaIndex = BTget.indexOf(',');
        OutFlowRt = BTget.substring(0,commaIndex).toFloat();
        //Default the solenoid to true (or closed)
        if(BTget.substring(commaIndex+1) == "false")
          SolState = false;
        else
          SolState = true;
        //set outputs
        analogWrite(PumpPWM, LpmToPWM(OutFlowRt));
        digitalWrite(RelayCtrl, SolState);
        index = 0;
        BTget = "";
        BTsend = "";
        RecData = false;
      }
    }
    else
    {
      //On line break, check if "Get data" or "Send data" command, if so, send or get data to/from NUC
      if(0 == int(temp))
      {
        if(0 == BTget.compareTo(get_data))
        {
          GatherData();             //congregate the data stream to send over BT
          BTSerial.println(BTsend);
          index = 0;
          BTget = "";
          BTsend = "";
        }
        else if(0 == BTget.compareTo(send_data))
        {
          RecData = true;
        }
        else
        {
          Serial.println("else of compare to");
          index = 0;
          BTget = "";
          BTsend = "";
        }
      }
    }
  }
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

