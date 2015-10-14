#include <SoftwareSerial.h>
#include <FreqMeasure.h>

/*
 * Pin 03: Input from flow rate sensor
 * Pin 04: Unusable with analogWrite (from freqMeasure)
 * Pin 09: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 10: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 * Pin 12: Relay control output
 * Pin 14: IR input
 * Pin 23: PWM output for pump
 */

const String get_data = "get_data";
const String send_data = "send_data";

SoftwareSerial BTSerial(9,10);
int RelayCtrl = 12;
int IRin = 14;
int PumpPWM = 23;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTSerial.begin(9600);
  FreqMeasure.begin();
  pinMode(PumpPWM, OUTPUT);
  pinMode(IRin, INPUT);
  pinMode(RelayCtrl, OUTPUT);
}

//define variables 
int FreqIndex = 0;
int count = 0;
float sum = 0;
float freqs[30];
float freq = 0;
float OutFlowRt = 0;
float InFlowRt = 0;
int SolState = 0;
bool RecData = false;
static String BTsend = "";

//define calibration constants
float Ap = -9.896452;
float Bp =  53.94933;
float Cp = -.7948750;
float Dp =  15.38008;

//declare functions
int LpmToPWM(float LPerMin);
void GatherData();

void loop() {
  
  //measure instantanious freq., put into array holding the last 30 freq measurements
  if (FreqMeasure.available()){
    freqs[FreqIndex] = FreqMeasure.read();
    //increment or roll over FreqIndex
    if (FreqIndex < 30)
      FreqIndex++;
    else
      FreqIndex = 0;
    //count until freqs is full of last 30 measurements
    if (count < 30)
      count = count + 1;
  }
  
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
    */
    if(RecData)
    {
      if(0 == int(temp))
      {
        //Parse string recieved and put data into OutFlowRt and SolState
        //set outputs
        analogWrite(PumpPWM, LpmToPWM(OutFlowRt));
        digitalWrite(RelayCtrl, SolState);
      }
    }
    else
    {
      //On line break, check if "Get data" command, if so, send data
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
  return (int) (Ap*pow(lpm,3) + Bp*pow(lpm,2) + Cp*lpm + Dp); 
}

void GatherData(){
  //BTsend should be in the order: IR range, Pump rate, Flow rate, Solenoid state
  
  //measure IR sensor
  if(analogRead(IRin) > 200)
    BTsend += '1';
  else
    BTsend += '0';
  BTsend += ',';
  
  //current output to pump
  BTsend += OutFlowRt;
  BTsend += ',';
  
  //get average of last 30 frequencies for flow rate
  if (count == 30){
    for (int i = 0; i < 30; i++){
      sum += freqs[i];
    }
    freq = FreqMeasure.countToFrequency(sum/30);
  } else
    freq = 0;
  //Flow rate = frequency / 33 Hz/lpm
  InFlowRt = freq / 33;
  BTsend += InFlowRt;
  BTsend += ',';

  //current output to solenoid
  BTsend += SolState;
}

