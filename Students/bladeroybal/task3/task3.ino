void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  //Adding ASCII to Binary Conversion from online converter
  String aggie = "01100001 01100111 01100111 01101001 01100101";
  int stringlength = aggie.length();
  for(int i=0;i<stringlength;i++){
    if (aggie[i]=='1'){
      digitalWrite(13,HIGH);
      delay(100);
    }
    if (aggie[i]=='0'){
      digitalWrite(13,LOW);
      delay(100);
    }
  }
}
