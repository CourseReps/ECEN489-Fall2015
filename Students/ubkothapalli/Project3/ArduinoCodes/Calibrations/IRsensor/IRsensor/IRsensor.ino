

int IRpin = 14;    // select the input pin for the potentiometer

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(IRpin, INPUT);  
}

float getreading(){
  float sensorTemp = analogRead(IRpin);
  return sensorTemp;
}

void loop() {
  // read the value from the sensor:
     
  // turn the ledPin on
  if(Serial.available()){
      char tempC = Serial.read();
      if (tempC == 'y'){
        int i = 0;
        float val =0;
        while(i<=4){
          
          val = val + getreading();
          delay(10);
          i++;
          
        }
        Serial.println(val/5);
      
      }
    
  }    
}
