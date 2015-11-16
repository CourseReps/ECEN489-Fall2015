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
#define CUPMAX 12 /* CUPMAX defines the height of cup */
#define SEP 2     /* Separation between IR sensor and cup top */
#define IRMIN 2   /* minimum measurable distance by IR */
#define IRMAX 15  /* maximum measurable distance by IR */
//#define IRHEIGHT ((CUPMAX > IRMAX) ? IRMAX : CUPMAX)
#define MINSETPOINT ((IRMAX > CUPMAX) ? 0 : (CUPMAX - (IRMAX - SEP)))
#define MAXSETPOINT (CUPMAX - (IRMIN - SEP))

/* define pin configurations */
#define FlowSens 5
#define IRin1 14
#define IRin2 15
#define PumpPWM1 4
#define PumpPWM2 3
#define TeensyPWR 6
#define RX 9
#define TX 10

SoftwareSerial BTSerial(RX, TX);

const String get_data = "get_data";
const String send_data = "send_data";

/* PID variables */
double Setpoint, Input, Output;
/* PID initialization and tuning parameters */
PID myPID(&Input, &Output, &Setpoint,20,4,2, DIRECT);

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
  pinMode(FlowSens, INPUT);
  attachInterrupt(FlowSens, rpm, RISING);
  pinMode(PumpPWM1, OUTPUT);
  pinMode(PumpPWM2, OUTPUT);
  pinMode(IRin1, INPUT);
  pinMode(IRin2, INPUT);
  /* PID Initialize */
  Input = 0;
  Setpoint = 0;

  myPID.SetMode(AUTOMATIC);
}

/* initialize variables */
volatile int edges = 0;
int count = 0;
float freq = 0;
float InFlowRt = 0;

/* declare functions */
void rpm ();
void shortFilter(double *out, double in, double t);

void loop()
{
  static unsigned int a = 0;
  a++;
  /* Get IR Sensor Value */
  static double IRvalFilt = 0;
  double IRval = (double)analogRead(IRin1);
  /* Filter the IR value */
  shortFilter(&IRvalFilt, IRval, (double)ALPHA);
  digitalWrite(TeensyPWR, HIGH);
  delay(200);
  digitalWrite(TeensyPWR, LOW);
  delay(200);
  /* convert IR values to cm */
  double height = 2770.1 * pow(IRvalFilt, -1.116);
  if (height > MAXSETPOINT)
  {
    height = MAXSETPOINT;
  }
  else if (height < MINSETPOINT)
  {
    height = MINSETPOINT;
  }
  
  Input = (double)CUPMAX - height;

  if(a%500 == 0)
  {
    /*Serial.print("Height: ");
    Serial.println(height);
    Serial.print("Input: ");
    Serial.println(Input);*/
  }

  if(a%500 == 0)
  {
    Serial.print("Setpoint: ");
    Serial.println(Setpoint);
  }
  
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

  /*if(a%500 == 0)
  {
    Serial.print("pumpControl: ");
    Serial.println(pumpControl);
  }*/

  analogWrite(PumpPWM,pumpControl);
  
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

    //On line break, check if "Get data" or "Send data" command, if so, send or get data to/from NUC
    if(0 == int(temp))
    {
      if(0 == BTget.compareTo(get_data))
      {
        //GatherData();             //congregate the data stream to send over BT
        BTSerial.println(Input);
        index = 0;
        BTget = "";
      }
      else if(send_data == BTget.substring(0,9))
      {
        BTget = BTget.remove(0,9);
        Setpoint = (double)BTget.toFloat();
        Serial.println(Setpoint);
        BTSerial.println('1');
        index = 0;
        BTget = "";
      }
      else
      {
        Serial.println("else of compare to");
        index = 0;
        BTget = "";
      }
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

  out1 = ((in - out1) * ALPHA) + out1;

  *out = out1;
}
