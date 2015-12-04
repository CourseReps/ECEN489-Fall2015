int IR = 14;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(IR, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(IR));
  delay(250);
}
