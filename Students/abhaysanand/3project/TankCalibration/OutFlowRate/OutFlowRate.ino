#include <SoftwareSerial.h>
#include <PID_v1.h>

/*
 * Pin 04: Input from flow rate sensor
 * Pin 00: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 01: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 * Pin 14: IR input
 * Pin 20: PWM output for pump
 */
//#define ALPHA 0.0042  /* filtering constant */
#define CUPMAX 13.5 /* CUPMAX defines the height of cup */
#define SEP 0     /* Separation between IR sensor and cup top */
#define IRMIN 2   /* minimum measurable distance by IR */
#define IRMAX 15  /* maximum measurable distance by IR */
//#define IRHEIGHT ((CUPMAX > IRMAX) ? IRMAX : CUPMAX)
#define MINSETPOINT ((IRMAX > CUPMAX) ? 0 : (CUPMAX - (IRMAX - SEP)))
#define MAXSETPOINT (CUPMAX - (IRMIN - SEP))

/* define pin configurations */
#define FlowSens 5
#define IRin 14
#define PumpPWM 4
#define RX 0
#define TX 1

#define X3 0.5859
#define X2 -5.7348
#define X1 25.967
#define X0 103.48

SoftwareSerial BTSerial(RX, TX);

const String get_data = "get_data";
const String send_data = "send_data";

/* PID variables */
double Setpoint, Input, Output;
/* PID initialization and tuning parameters */
PID myPID(&Input, &Output, &Setpoint,4.8,1.5,0, DIRECT);

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
  myPID.SetOutputLimits(30,100);
}

/* initialize variables */
volatile int edges = 0;
int count = 0;
float freq = 0;
float InFlowRt = 0;

/* declare functions */
void rpm ();
//void shortFilter(double *out, double in, double t);

void loop()
{
  static unsigned int a = 0;
  a++;
  /* Get IR Sensor Value */
  double IRval = (double)analogRead(IRin);

  /* convert IR values to cm */
  double Input = IRval;


  if(a%500 == 0)
  {
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

  /* Write PID controlled PWN value to PumpPWM */
  int pumpControl = (Output);
  //pumpControl = map(pumpControl,0,255,30,100);

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
      double temp1 = SerGet.toFloat();
      Setpoint = (X3*pow(temp1,3)) + (X2*pow(temp1,2)) + (X1*temp1) + (X0);
    }
  }
}

//Define functions
void rpm()
{
  edges = edges + 1;
}

