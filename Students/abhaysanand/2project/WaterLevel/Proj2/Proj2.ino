#include <SoftwareSerial.h>
#include <PID_v1.h>

/*
 * Pin 04: Input from flow rate sensor
 * Pin 00: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 01: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 * Pin 14: IR input
 * Pin 20: PWM output for pump
 */
#define ALPHA 0.0018  /* filtering constant */
#define CUPMAX 12 /* CUPMAX defines the height of cup below IR sensor*/
#define IRMIN 2   /* minimum measurable distance by IR */
#define IRMAX 15  /* maximum measurable distance by IR */
#define IRHEIGHT ((CUPMAX > IRMAX) ? IRMAX : CUPMAX)
#define MINSETPOINT ((CUPMAX > IRMAX) ? (CUPMAX - IRMAX) : 0)
#define MAXSETPOINT (CUPMAX - IRMIN)

/* define pin configurations */
#define FlowSens 4
#define IRin 14
#define PumpPWM 20
#define RX 0
#define TX 1

SoftwareSerial BTSerial(RX, TX);

const String get_data = "get_data";
const String send_data = "send_data";

/* PID variables */
double Setpoint, Input, Output;
/* PID initialization and tuning parameters */
PID myPID(&Input, &Output, &Setpoint,30,2,2, DIRECT);

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FlowSens, INPUT);
  attachInterrupt(FlowSens, rpm, RISING);
  pinMode(PumpPWM, OUTPUT);
  pinMode(IRin, INPUT);

  /* PID Initialize */
  Input = 0;
  Setpoint = 0;

  myPID.SetMode(AUTOMATIC);
}

/* initialize variables */
volatile int edges = 0;
int count = 0;
float freq = 0;
float OutFlowRt = 0;
float InFlowRt = 0;
bool RecData = false;
static String BTsend = "";
//int up = 1;

/* define calibration constants */
const float Ap = -9.896452;
const float Bp =  53.94933;
const float Cp = -.7948750;
const float Dp =  15.38008;

/* declare functions */
//int LpmToPWM(float LPerMin);
void GatherData();
void rpm ();
void shortFilter(double *out, double in, double t);

void loop()
{
  static unsigned int a = 0;
  a++;
  /* Get IR Sensor Value */
  static double IRvalFilt = 0;
  double IRval = (double)analogRead(IRin);
  /* Filter the IR value */
  shortFilter(&IRvalFilt, IRval, (double)ALPHA);

  /* convert IR values to cm */
  double height = 1.4 * 2770.1 * pow(IRvalFilt, -1.116);
  if (height > IRHEIGHT)
  {
    height = IRHEIGHT;
  }
  else if (height < IRMIN)
  {
    height = IRMIN;
  }
  
  Input = (double)CUPMAX - height;

  if(a%500 == 0)
  {
    /*Serial.print("Height: ");
    Serial.println(height);*/
    Serial.print("Input: ");
    Serial.println(Input);
  }

  /* temp serial input code */
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
        /* Get Setpoint data from SerialRead */
        Setpoint = SerGet.toFloat();
        if(Setpoint > MAXSETPOINT)
        {
          Setpoint = MAXSETPOINT;
        }
        else if(Setpoint < MINSETPOINT)
        {
          Setpoint = MINSETPOINT;
        }
        
        SerGet = ""; /* clear SerGet buffer */
      }
  }
  /*if(a%500 == 0)
  {
    Serial.print("Setpoint: ");
    Serial.println(Setpoint);
  }*/
  
  /* Compute the PID output value */
  myPID.Compute();
  if(Output > MAXSETPOINT)
  {
    Output = MAXSETPOINT;
  }
  else if(Output < MINSETPOINT)
  {
    Output = MINSETPOINT;
  }
  if(a%500 == 0)
  {
    Serial.print("Output: ");
    Serial.println(Output);
  }

  /* Write PID controlled PWN value to PumpPWM */
  int pumpControl = (int)(Output * (80-35) / (MAXSETPOINT-MINSETPOINT)) + 35;

  if(a%500 == 0)
  {
    Serial.print("pumpControl: ");
    Serial.println(pumpControl);
  }

  analogWrite(PumpPWM,pumpControl);
  
  count = count + 1;
  if(count > 250){
    cli();
    freq = edges * 4;
    sei();
    //Serial.println(freq);
    count = 0;
    edges = 0;
    /*
    if (OutFlowRt > 2)
      OutFlowRt = 0.1;
    else
      OutFlowRt = OutFlowRt + .01;
    /**/
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
        /*if(BTget.substring(commaIndex+1) == "false")
          SolState = false;
        else
          SolState = true;*/
        //set outputs
        //analogWrite(PumpPWM, LpmToPWM(OutFlowRt));
        //digitalWrite(RelayCtrl, SolState);
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
}

//Define functions
void rpm()
{
  edges = edges + 1;
}

/* Convert from Liters per Minute to PWM value based on calibrated eqn. */
/*int LpmToPWM(float LPerMin)
{
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
}*/

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
  //BTsend += String(SolState);
  Serial.println(BTsend);
}

void shortFilter(double *out, double in, double t)
{
  double out1 = *out;

  out1 = ((in - out1) * ALPHA) + out1;

  *out = out1;
}

