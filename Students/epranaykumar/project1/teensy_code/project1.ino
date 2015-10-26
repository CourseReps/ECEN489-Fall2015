
#include <SoftwareSerial.h>
int RX=0;
int TX=1;

//input pins
int IRpin = A0; 

int flowpin = 2;

//output pins
int relay = 13; // Pin 12
int pwmpin = 10; // Pin 10
 
//ouput values
double IRRange; 
  
int tempsol; 
bool solenoidState;

double cur_voltage=0;
double prev_voltage=0;

double prev_flowval = 0;
double prev_var = 1 ;    

int relaystate=0;

double flowval = 0; // variable to store the flow rate value
double measured_flowval=0;

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
                   input = Serial.read();  // NOTe: select no line ending option at serial monitor
                                                    
              }
            
                         
             if(input == "0" ){      
                              cur_voltage =0; 
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
                               
                                if( cur_voltage>8){
                                      cur_voltage=0;
                                      relaystate =0;
                                  }
                                  else{
                                       cur_voltage++;
                                      relaystate =1;
                                  }
                                
                                read_data();                              
                                send_data();
             }

             delay(2000);
             prev_voltage = cur_voltage;
               
}
           

void read_data() { 

              Serial.println("begin of read"); 
              IRRange=analogRead(IRpin);
              
  
              digitalWrite(relay, relaystate); // on
              analogWrite(pwmpin, cur_voltage * 21);
              
  
              NbTopsFan = 0;   //Set NbTops to 0 ready for calculations
               
              sei();          //Enables interrupts
              delay (1000);   //Wait 1 second
              cli();          //Disable interrupts
              
              measured_flowval = NbTopsFan*0.053; // 30sec with 250 ticks/sec is required to fill 400ml. so the constant A(in ml/tick) to get flow value in ml/sec is 400/30*250
              flowval= kalman_filter(measured_flowval, (cur_voltage-prev_voltage) );
             

              Serial.println("end of read"); 
             

}

void send_data() {
              Serial.println(" begin of send"); 
             
              Serial1.println(IRRange); 
                           
              Serial1.println(relaystate); 
              
             
              Serial1.println(cur_voltage);               
              Serial1.println(flowval, DEC); 
              
             
              
              Serial1.println ("200000"); //Prints the number calculated above     
             
             
             
              Serial.println("end of send"); 
              
}

double kalman_filter(double measured_flowval, double change_in_pumprate){
  
          double pred_cur_flowval = prev_flowval + change_in_pumprate; // slope of curve is 1
          double pred_cur_var = prev_var + 2; // spread in process model is choosen to be 2

          double kalman_gain = pred_cur_var / (pred_cur_var + 3);  // spread in measurement model is choosen to be 3
          
          double cur_flowval = pred_cur_flowval + kalman_gain * (measured_flowval - pred_cur_flowval);
          double cur_var = (1 - kalman_gain)*pred_cur_var;

          prev_flowval = cur_flowval;
          prev_var = cur_var ;  

          return cur_flowval;
}
 
