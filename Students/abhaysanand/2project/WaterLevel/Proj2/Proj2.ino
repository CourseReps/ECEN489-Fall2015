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
#define CUPMAX 8 /* CUPMAX defines the height of cup */
#define SEP 1     /* Separation between IR sensor and cup top */
#define IRMIN 2   /* minimum measurable distance by IR */
#define IRMAX 15  /* maximum measurable distance by IR */
//#define IRHEIGHT ((CUPMAX > IRMAX) ? IRMAX : CUPMAX)
#define MINSETPOINT ((IRMAX > CUPMAX) ? 0 : (CUPMAX - (IRMAX - SEP)))
#define MAXSETPOINT (CUPMAX - (IRMIN - SEP))

/* define pin configurations */
#define FlowSens 4
#define IRin 14
#define PumpPWM 23
#define RX 0
#define TX 1

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
  double IRval = (double)analogRead(IRin) * 12 / 1023;
  /* Filter the IR value */
  shortFilter(&IRvalFilt, IRval, (double)ALPHA);

  /* convert IR values to cm */
  double height = 32.222 * pow(IRvalFilt, -1.327);
  if (height > IRMAX)
  {
    height = IRMAX;
  }
  else if (height < IRMIN)
  {
    height = IRMIN;
  }
  
  Input = (double)(CUPMAX - (height - SEP));

  if(Input < 0)
  {
    Input = 0;
  }
  else if(Input > CUPMAX)
  {
    Input = CUPMAX;
  }

  if(a%500 == 0)
  {
    Serial.print("Height: ");
    Serial.println(height);
    Serial.print("Input: ");
    Serial.println(Input);
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
  /*if(a%500 == 0)
  {
    Serial.print("Output: ");
    Serial.println(Output);
  }*/

  /* Write PID controlled PWN value to PumpPWM */
  int pumpControl = (int)(Output * (80-30) / (MAXSETPOINT-MINSETPOINT)) + 30;

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

