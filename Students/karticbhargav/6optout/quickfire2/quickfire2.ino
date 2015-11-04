void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

double code;
double celsius;
double fahrenheit;
double A = 0.001593;
double B = 0.0001076;
double C = 0.00000058127;
double R = 32500;
int i=0;
//double Rt = 31500;
//Rt at 0 = 8400
//Rt at 150 = 440


void loop() {
  // put your main code here, to run repeatedly:
  code = analogRead(14);
  
  double vin=5.15;
  double vout = code*3.3/1023;
  
  double Rt = vout*R/(vin-vout);
  celsius = (1/(A+B*log(Rt)+C*pow(log(Rt),3)))-273;
  //celsius = 25 + (code - 512) / 11.3;
  fahrenheit = celsius * 1.8 + 32;
  //Serial.print("temperature: ");
  Serial.println(int(celsius));
  
  //Serial.print(" Celsius, ");
//  Serial.print(fahrenheit);
//  Serial.println(" Fahrenheit");
  delay(1000);
}
