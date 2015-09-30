
    
int led = 13; // Pin 2
int val = 0;
 

void setup()
{
    pinMode(led, OUTPUT); // Set pin 13 as digital out
     
    // Start up serial connection
    Serial.begin(9600); // baud rate
    Serial.flush();
}
     
void loop()
{
    String incomingByte = "" ;
    unsigned char bytecount = 0;
    int i;
    
    // wait for input from C++ interface before sending the analog reading
    while (bytecount<1) // wait until one byte is read
    {
      if (Serial.available() > 0)  // use 'if' not 'while' as we want to read only one byte and then process
         {   
               
              incomingByte = Serial.read(); // reads one byte from the buffer
                   
              if (incomingByte == "1")
              {
                  digitalWrite(led, HIGH);
                  //delay(500);
              }
              else if (incomingByte == "0")
              {
                 digitalWrite(led, LOW);
                 //delay(1000);
              }
              bytecount++;
              i=0;
          }
    } 
   
 
   while(i<5){
      val = analogRead(led);
      Serial.print(val);//prints the integer value to the usb buffer and returns 
      Serial.print(" ");
      delay(1000);  
      i++;     
    }
     Serial.print(2000);//for breaking the loop in client code
     Serial.print(" ");
    
     digitalWrite(led, LOW);
    

}
