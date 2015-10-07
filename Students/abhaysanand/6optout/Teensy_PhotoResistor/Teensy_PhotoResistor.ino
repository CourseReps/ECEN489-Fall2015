int lightPin = 14;
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
  int photoVolt;
  
  photoVolt = analogRead(lightPin);

  Serial.println(photoVolt);

  if (photoVolt < 500)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
  delay(50);
}