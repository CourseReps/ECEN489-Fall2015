//Cooper Rehn
//9-24-15
//used the two tutorial examples found at https://www.arduino.cc/en/Reference/Servo
#include <Servo.h>
int in = 0;
int fan = 3;
int pot = 14;
int val = 0;
int servoval = 0;
int PWMval = 0;

Servo myservo;  // create servo object to control a servo

void setup() {
  pinMode(fan, OUTPUT);
  pinMode(pot, INPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  val = analogRead(pot);                    // reads the value of the potentiometer (value between 0 and 1023)
  servoval = map(val, 0, 1023, 180, 0);     // scale it to use it with the servo (value between 0 and 180)
  PWMval = map(val, 0, 1023, 0, 255);       // and to use for PWM
  myservo.write(servoval);                  // sets the servo position according to the scaled value
  analogWrite(fan, PWMval);                 // sets the PWM for the fan
  delay(15);
}
  
