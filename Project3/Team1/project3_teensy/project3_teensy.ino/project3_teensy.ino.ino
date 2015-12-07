#include <SoftwareSerial.h>
#include <PID_v1.h>

/*
 * Pin 04: Input from flow rate sensor
 * Pin 00: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 01: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 * Pin 14: IR input
 * Pin 20: PWM output for pump
 */
#define ALPHA 0.0042  /* filtering constant */
#define CUPMAX 12 /* CUPMAX defines the height of cup */
#define SEP 0     /* Separation between IR sensor and cup top */
#define IRMIN 2   /* minimum measurable distance by IR */
#define IRMAX 15  /* maximum measurable distance by IR */
//#define IRHEIGHT ((CUPMAX > IRMAX) ? IRMAX : CUPMAX)
#define MINSETPOINT ((IRMAX > CUPMAX) ? 0 : (CUPMAX - (IRMAX - SEP)))
#define MAXSETPOINT (CUPMAX - (IRMIN - SEP))
#define LOOP_TIME_MS 154.6
#define THRES 0.25

#define DEBUG1  //comment this out to enable PID control for tank 2

/* define pin configurations */
#define FlowSens 5
#define IRin A1
#define IRin2 A0
#define PumpPWM 4
#define PumpPWM2 3
#define RX 9
#define TX 10

SoftwareSerial BTSerial(RX, TX);

const String get_data = "get_data";
const String send_data1 = "send_data1";
const String send_data2 = "send_data2";
unsigned long timestamp, last_timestamp;
short unsigned int state;
unsigned long state1_counts;
double sigmaQ;
double objectVolume;
double deltaLevel;

/* PID variables */
double Setpoint, Input, Output;
double Setpoint2, Input2, Output2;
/* PID initialization and tuning parameters */
PID myPID(&Input, &Output, &Setpoint,20,4,2, DIRECT);
PID myPID2(&Input2, &Output2, &Setpoint2,20,4,2, DIRECT);

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FlowSens, INPUT);
  attachInterrupt(FlowSens, rpm, RISING);
  pinMode(PumpPWM, OUTPUT);
  pinMode(IRin, INPUT);
  pinMode(IRin2, INPUT);
  pinMode(PumpPWM2, OUTPUT);

  /* PID Initialize */
  Input = Input2 = 0;
  Setpoint = Setpoint2 = 0;

  timestamp = last_timestamp = 0;
  state = 0;

  myPID.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
}

/* initialize variables */
volatile int edges = 0;
int count = 0;
float freq = 0;
float InFlowRt = 0;
double Qout = 0;
double flow_sense = 0.0;

/* declare functions */
void rpm ();
void shortFilter(double *out, double in, double t);

void loop()
{
  static unsigned int a = 0;
  a++;
  
  /*** Compute the PID1 output value ***/
  /* Get IR Sensor Value */
  static double IRvalFilt = 0;
  double IRval = (double)analogRead(IRin) * 12 / 1023;
  /* Filter the IR value */
  shortFilter(&IRvalFilt, IRval, (double)ALPHA);
  /* Convert to cm */
  double height = .0293*pow(IRvalFilt,5) - 0.5663*pow(IRvalFilt,4) + 4.4136*pow(IRvalFilt,3) - 17.7419*pow(IRvalFilt,2) + 38.7313*IRvalFilt - 28.8534; //Poly fit
  Input = height;
  myPID.Compute();
  if(Output > MAXSETPOINT)
  {
    Output = MAXSETPOINT;
  }
  else if(Output < MINSETPOINT)
  {
    Output = MINSETPOINT;
  }
  /* Write PID controlled PWN value to PumpPWM */
  int pumpControl = (int)(Output * (80-30) / (MAXSETPOINT-MINSETPOINT)) + 30;
  analogWrite(PumpPWM,pumpControl);

#ifndef DEBUG1
  /*** Compute PID2 output value ***/
  /* Get IR Sensor Value */
  static double IRvalFilt2 = 0;
  double IRval2 = (double)analogRead(IRin2) * 12 / 1023;
  /* Filter the IR value */
  shortFilter(&IRvalFilt2, IRval2, (double)ALPHA);
  /* Convert to cm */
  double height2 = .0293*pow(IRvalFilt2,5) - 0.5663*pow(IRvalFilt2,4) + 4.4136*pow(IRvalFilt2,3) - 17.7419*pow(IRvalFilt2,2) + 38.7313*IRvalFilt2 - 28.8534; //Poly fit
  Input2 = height2;
  myPID2.Compute();
  if(Output2 > MAXSETPOINT)
  {
    Output2 = MAXSETPOINT;
  }
  else if(Output2 < MINSETPOINT)
  {
    Output2 = MINSETPOINT;
  }
  /* Write PID controlled PWN value to PumpPWM */
  int pumpControl2 = (int)(Output2 * (80-30) / (MAXSETPOINT-MINSETPOINT)) + 30;
  analogWrite(PumpPWM2,pumpControl2);
#else
  int pumpControl2 = 0; //for comms purposes
  Input2 = 0;
#endif
  
  count = count + 1;
  if(count > 250){
    cli();
    freq = edges * 4;
    sei();
    count = 0;
    edges = 0;
  }
  flow_sense = (double)freq/33; //flow meter reading in l/min
  
  if(a%100 == 0)  //decreased from 500 to 100 to improve counter performance for volume calculation
  {
    //for calibration
    last_timestamp = timestamp;
    timestamp = micros();

    deltaLevel = abs(height - Setpoint);
    Qout = -.00215*pow(height,4)+0.0699*pow(height,3)-0.8287*pow(height,2)+4.4372*height-5.2091;
    //steady state detection
    switch(state)
    {
        case 0: if(deltaLevel > THRES)      //still unstable, maintain state 0
                  break;
                else                        //error is within threshold, begin wait for PID steady state (state 1)
                {
                  state = 1;
                  state1_counts = 0;
                  break;
                }
        case 1: if(deltaLevel < THRES)      //still within threshold
                {
                  if(state1_counts == 40)   //accumulated enough time in wait state, acceptable PID steady state found, go to state 2
                  {
                    state = 2;
                    break;
                  }
                  state1_counts++;          //implied else, increment count
                }
                else                        //we've exceeded the threshold, not steady after all, return to start state
                {
                  state = 0;
                  break;
                }
        case 2: if(deltaLevel < THRES)      //hold here as long as still within threshold
                  break;
                else                        //object detected by error going out of bounds, transition to state 3
                {
                  Serial.println("Object drop detected!");
                  sigmaQ = 0;
                  state = 3;
                  break;
                }
        case 3: if(deltaLevel > THRES)      //sum up Qout until we're back inside the threshold
                {
                  sigmaQ += Qout-flow_sense;//Qout due to object is delta between expected Q at current height (Qin from flow meter) and actual (from calibration)
                  break;
                }
                else                        //we've fallen back within the threshold, stop summing and do the volume calculation
                {
                  state = 0;
                  objectVolume = sigmaQ * LOOP_TIME_MS / 60000.0; //mulitply summation of flow (l/min) by time in minutes (ms to minute conversion = ms/60000)
                  Serial.print("Object Volume = ");
                  Serial.print(objectVolume*1000);
                  Serial.println(" cm^3");
                  break;
                }
         default:
                Serial.println("ERROR IN STATE MACHINE"); //should never ever get here
    }
  }
  if(a%500 == 0)
  {
    //debug statements
    Serial.print("Delta T (ms): ");
    Serial.println((double)(timestamp-last_timestamp)/1000.0);
    Serial.print("Height(cm from bottom): ");
    Serial.println(Input);
    Serial.print("Flow(l/min): ");
    Serial.println(flow_sense);
    Serial.print("Output flow(l/min): ");
    Serial.println(Qout);
    //Serial.print("Pump PWM(0-255): ");
    //Serial.println(pumpControl);
    Serial.print("Setpoint(cm): ");
    Serial.println(Setpoint);
    Serial.println();
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

    //On line break, check if "Get data" or "Send data" command, if so, send or get data to/from NUC
    if(0 == int(temp))
    {
      if(0 == BTget.compareTo(get_data))
      {
        BTSerial.print(Input); BTSerial.print(",");
        BTSerial.print(Input2); BTSerial.print(",");
        BTSerial.print(pumpControl); BTSerial.print(",");
        BTSerial.print(pumpControl2); BTSerial.print(",");
        BTSerial.println(flow_sense);
        Serial.print("get_data!");
        
        index = 0;
        BTget = "";
      }
      else if(send_data1 == BTget.substring(0,10))
      {
        BTget = BTget.remove(0,10);
        Setpoint = (double)BTget.toFloat();
        
        Serial.println(Setpoint);
        BTSerial.println('1');
        index = 0;
        BTget = "";
      }
      else if(send_data2 == BTget.substring(0,10))
      {
        BTget = BTget.remove(0,10);
        Setpoint2 = (double)BTget.toFloat();
        
        Serial.println(Setpoint2);
        BTSerial.println('1');
        index = 0;
        BTget = "";
      }
      else
      {
        Serial.println("else of compare to: ");
        Serial.println(BTget);
        index = 0;
        BTget = "";
      }
    }
  }
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
      Setpoint = SerGet.toFloat();
      Setpoint2 = SerGet.toFloat();
      SerGet = "";
    }
  }
}

//Define functions
void rpm()
{
  edges = edges + 1;
}

void shortFilter(double *out, double in, double t)
{
  double out1 = *out;

  out1 = ((in - out1) * t) + out1;

  *out = out1;
}

