#include <SoftwareSerial.h>
boolean stringComplete = false;

SoftwareSerial BTSerial(11,10);

String bits = "";

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  bits.reserve(200);
  BTSerial.begin(9600);
}

class convert{
  public:
    String int_to_bin(int);
    String reverse(String);
};

String convert::int_to_bin(int number) {
  static String result;
  static int level = 0;
  level++;
  if (number > 0) {
    if (number % 2 == 0) {
      result.concat("0");
    }
    else {
      result.concat("1");
    }
    int_to_bin(number / 2);
    level--;
  }
  if (level == 1) return reverse(result);
  return result;
}

String convert::reverse(String to_reverse) {
  String result;
  for (int i = to_reverse.length() - 1; i >= 0; i--)
    result += to_reverse[i];
  return result;
}

void loop(){
  serialEvent();
  if(stringComplete){
    bits="";
    stringComplete = false;
  }
}

void serialEvent(){
  int input;
  convert conversion;
  while(BTSerial.available()){
    Serial.write(BTSerial.read());
    char output[30];
    Serial.println("Start function: "+ bits+'\n');
    input = int(BTSerial.read());
    Serial.println(input);
    if((char)input == '#'){
      delay(500);
      bits="";
      stringComplete = true;
      break;
    }
    Serial.println(input, DEC);
    bits = conversion.int_to_bin(input);
    Serial.println("Bits after conversion: "+ bits+'\n');
    bits.toCharArray(output,sizeof(output));
    for(int i = 0; i <= bits.length(); i++){
      Serial.write(output[i]);
      if(output[i] == '1'){
        digitalWrite(13, HIGH);
        delay(300);
      }
      if(output[i] == '0'){
        digitalWrite(13, LOW);
        delay(300);
      }
      else
        continue;
    }
    Serial.println("dd");
    delay(500);
  }
  Serial.flush();
}

