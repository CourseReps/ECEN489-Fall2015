#define IRin A0
#define ALPHA 0.0042

void setup()
{
  pinMode(IRin, INPUT);
  Serial.begin(9600);
}

void shortFilter(double *out, double in, double t)
{
  double out1 = *out;

  out1 = ((in - out1) * t) + out1;

  *out = out1;
}

void loop()
{
  static unsigned int a = 0;
  a++;
  static double IRvalFilt = 0;

  double IRval = (double)analogRead(IRin) * 12 / 1023;
  
  shortFilter(&IRvalFilt, IRval, (double)ALPHA);

  if(a%100 == 0)
    Serial.println(IRvalFilt);
    
  delay(3);
}
