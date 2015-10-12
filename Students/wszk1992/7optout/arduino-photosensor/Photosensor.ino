//Using photoresistance to control 2 LEDs.

const int analogInPin = A5;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  delay(1000);
}
