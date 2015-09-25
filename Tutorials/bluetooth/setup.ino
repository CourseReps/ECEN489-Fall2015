#include <SoftwareSerial.h>

SoftwareSerial BTSerial(11,10);

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

}

void loop() {
  if(BTSerial.available())
    Serial.write(BTSerial.read());
  if(Serial.available())
    BTSerial.write(Serial.read());
}
