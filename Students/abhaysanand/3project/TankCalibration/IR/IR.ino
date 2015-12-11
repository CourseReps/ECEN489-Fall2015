int IR = 15;
int PumpPWM = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR, INPUT);
  pinMode(PumpPWM, OUTPUT);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int IRvalue = analogRead(IR);

  Serial.println(IRvalue);
  delay(50);
  static float pwm1 = 0;
  static String SerGet = "";
  
  if (Serial.available())
  {
  	char temp = char(Serial.read());
  	SerGet += temp;
  	if(temp == '\n')
  	{
      pwm1 = SerGet.toFloat();
  		SerGet = "";
  	}
  }
  //Serial.println(pwm1);
  analogWrite(PumpPWM, pwm1);
}
