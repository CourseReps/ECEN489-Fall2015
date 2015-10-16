#include <SoftwareSerial.h>

/*
 * Pin 09: Get data on Teensy from NUC via BC-06. Directly connected to TXD of BC-06
 * Pin 10: Send data from Teensy to NUC via BC-06. Connected using voltage divider circuit.
 */
SoftwareSerial BTSerial(9,10);

const String get_data = "get_data";

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop()
{
  static String BTget = "";
  static unsigned short index = 0;
  
  if(BTSerial.available())
  {
    char temp = char(BTSerial.read());
    if(10 == int(temp))
    {
      temp = 0;
    }
    BTget += temp;
    index++;

    Serial.print(index);
    Serial.print(": ");
    Serial.print(BTget);
    Serial.print(": ");
    Serial.println(int(temp));
    
    if(0 == int(temp))
    {
      if(0 == BTget.compareTo(get_data))
      {
        BTSerial.println("232345,1765,1276523,1276521");
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
