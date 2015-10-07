int tempPin = 14;
int ledPin = 13;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  int thermoVolt;
  
  thermoVolt = analogRead(tempPin);

  if(Serial.available() > 0)
  {
    if (Serial.read() == '1')
    {
      Serial.println(thermoVolt);
    }
  }

  if (thermoVolt < 500)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  delay(50);
}
