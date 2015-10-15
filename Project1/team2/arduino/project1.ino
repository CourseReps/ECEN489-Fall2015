
#include <SoftwareSerial.h>
int RX=0;
int TX=1;

//input pins
int IRpin = A0;
//int Solpin = A3; 

int flowpin = 2;

//output pins
int relay = 13; // Pin 12
int pwmpin = 10; // Pin 10
 
//ouput values
double IRRange; 
  
int tempsol; 
bool solenoidState;

int voltage=0;
int relaystate=0;
int flowval = 0; // variable to store the flow rate value
int dutyCycle = 1; // PWM cycle, value is from 0-12 volts;
volatile int NbTopsFan; //measuring the rising edges of the signal


void setup() {
   
         pinMode(relay, OUTPUT); // Set pin 13 as digital out
         pinMode(pwmpin, OUTPUT);
          
         pinMode(IRpin,INPUT);
         
         pinMode(flowpin,INPUT);
        
       
  
        attachInterrupt(digitalPinToInterrupt(flowpin), rpm, RISING); //and the interrupt is attached
        
        Serial.begin(9600); 
        Serial1.begin(9600);
  }

  void rpm () { //This is the function that the interupt calls
    
    NbTopsFan++;  //This function measures the rising and falling edge of the hall effect sensors signal
    
  } 


void loop() {

             String input = "" ;
             String state = "" ;
             
              
              
             if (Serial.available())
              {               
                    input = Serial.read();  // NOTe: no line ending option at serial monitor
                       
                                    
              }
                         
              if(input == "0" ){      
                              voltage =0; 
                              relaystate =0;
                              read_data();
                              send_data(); 
                              
                              Serial.println("stopped");   
                                while (!Serial.available())
                                {               
                                    // resume when input is provided            
                                                      
                                }                                     
             }

             else{
                               
                                if(voltage>11){
                                  voltage=0;
                                  relaystate =0;
                                  }
                                  else{
                                     voltage++;
                                     relaystate =1;
                                  }
                                
                                read_data();
                                send_data();
              }

              delay(2000);

             
       
    }
           

void read_data() { 

              Serial.println("begin of read"); 
              IRRange=analogRead(IRpin);
              
  
              digitalWrite(relay, relaystate); // on
              analogWrite(pwmpin, voltage * 21);
              
  
              NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
               
              sei();          //Enables interrupts
              delay (1000);   //Wait 1 second
              cli();          //Disable interrupts
              
              flowval = (NbTopsFan * 60 )/ 73; //(Pulse frequency x 60) / 7.5Q, = flow rate in L/hour 

               Serial.println("end of read"); 
             

}

void send_data() {
              Serial.println(" begin of send"); 
             
              Serial1.println(IRRange); 
                           
              Serial1.println(relaystate); 
              
             
              Serial1.println(voltage);               
              Serial1.println(flowval, DEC); 
              
             
              
              Serial1.println ("200000"); //Prints the number calculated above     
             
             
             
               Serial.println("end of send"); 
              
}
 
