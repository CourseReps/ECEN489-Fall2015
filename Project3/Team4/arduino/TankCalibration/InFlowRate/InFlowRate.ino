int FlowSens = 5;
int PumpPWM = 4;

void setup()
{
	Serial.begin(9600);
	pinMode(FlowSens, INPUT);
	attachInterrupt(FlowSens, rpm, RISING);
	pinMode(PumpPWM, OUTPUT);
}

volatile int edges = 0;
float freq = 0;

void rpm()
{
	edges = edges + 1;
}

void loop()
{
  int i;
	static int pumpRate = 0;
  static char start;
  static String SerGet = "";
	cli();
  freq = edges * 4;
  sei();
	
  if (Serial.available())
  {
    char temp = char(Serial.read());
    SerGet += temp;
    if(temp == '\n')
    {
      pumpRate = SerGet.toFloat();
      SerGet = "";
    }
  }

  analogWrite(PumpPWM, pumpRate);
  float tempFreq =0;
    
  //for(i=0;i<=9;i++)
  //{
  //  tempFreq = tempFreq + (freq/33);
  //  delay(10);
  //}
  //tempFreq /= 10;
  
  Serial.print(pumpRate);
  Serial.print("\t");
  Serial.println(freq/88);
  edges = 0;

    
  delay(250);
}
