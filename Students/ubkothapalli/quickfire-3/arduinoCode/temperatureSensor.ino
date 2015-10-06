
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
int ledPin = 13;

int sensorValue = 0;        // value read from the pot
int outputValue = 0; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin,OUTPUT);

}

void sensorWrite(){

  //for(int i=0;i<=10;i++){
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = sensorValue;
  //outputValue = map(sensorValue, 0, 1023, 0, 400);
  // change the analog out value:
  //analogWrite(analogOutPin, outputValue);

  // print the results to the serial monitor:
  //Serial.print("sensor = " );
  //Serial.println(sensorValue);
  //Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  //delay(300);
  
}

void ledSwitchON(){
  digitalWrite(ledPin,HIGH);
}

void ledSwitchOFF(){
  digitalWrite(ledPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() >0){
    char inChar = Serial.read();
   
    // For Initiating the serial write from the client
    if (inChar == 'Y'){
      sensorWrite();
    }

   else if (inChar == '1'){
      ledSwitchON();
    }
   else if (inChar == '0'){
   //   ledSwitchOFF();
   }
 }  

}
