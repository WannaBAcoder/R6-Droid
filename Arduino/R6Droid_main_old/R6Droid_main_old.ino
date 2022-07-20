#include "constants.h"

int pulselen;
int interlock = 0;

int ch5;
int ch6;

int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;
int flag5 = 0;
int flag6 = 0;
int flag7 = 0;
int flag40 = 0;

void setup() 
{
  //set relay pins to output, and disable them 
  for(int i = 7; i < 13; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
  Serial.begin(115200);
}

int min = 2000, max = 0;

void loop() 
{
  unsigned long currentMillis = millis();
  
  ch5 = pulseIn(CH5_PIN, HIGH, 25000); // Read the pulse width of 
  ch6 = pulseIn(CH6_PIN, HIGH, 25000); // each channel

//  if(ch6 > 0)
//  {
//    if(ch6 < min)
//      min = ch6;
//    if(ch6 > max)
//      max = ch6;  
//      
//    Serial.print("CH6 min = ");
//    Serial.println(min);
//    Serial.print("CH6 max = ");
//    Serial.println(max);
//  }
  
  if (ch5 > 1060 || ch6 > 1060) // dirty hack to allow PWM/analog voltage to stabalise 
  {          
    delay(100);
    ch5 = pulseIn(CH5_PIN, HIGH, 25000); // Read the pulse width of 
    ch6 = pulseIn(CH6_PIN, HIGH, 25000); // each channel
  }  
  
  // -------------------start 2-3-2 mode stuff------------
  //action 10
  else if (ch6 > ACT10_MIN && ch6 < ACT10_MAX)
  {  
    Serial.println("2-3-2 mode started");
    
    // 2-3-2 interlock
    interlock = 1;
    //setall(80,0,0);
    while (interlock != 0) 
    {
      delay(300); // wait for button press to end
      ch6 = pulseIn(CH6_PIN, HIGH, 25000); // read channel      
      if (ch5 > 1060 || ch6 > ACT10_MIN) 
      { // dirty hack to allow PWM/analog voltage to stabalise 
        delay(100);
        ch6 = pulseIn(CH6_PIN, HIGH, 25000);        // read channel / decide what to do
      }

      //action 10
      //this button press exits the 2-3-2 interlock
      if (ch6 > ACT10_MIN && ch6 < ACT10_MAX) 
      {
        //disable all relays
        for(int i = 8; i < 14; i++) 
          digitalWrite(i, HIGH);
          delay(200);
          interlock = 0;// break out of interlock
      }
      //foot down, action 20
      else if (ch6 > ACT20_MIN && ch6 < ACT20_MAX) 
      {  
        Serial.println("DOWN button");
        digitalWrite(CENTER_UP_PIN, HIGH);
        digitalWrite(CENTER_DOWN_PIN, LOW);
        interlock = 2;// set interlock to 3 leg mode lock
      }                                   

      //3 leg conversion, action 30
      else if (ch6 > ACT30_MIN && ch6 < ACT30_MAX && interlock == 2)// start shoulder conversion to 3 legs 
      {         
        Serial.println("3 leg button");
        digitalWrite(LEFT_B_PIN, HIGH);
        digitalWrite(RIGHT_B_PIN, HIGH);
        digitalWrite(LEFT_A_PIN, LOW);
        digitalWrite(RIGHT_A_PIN, LOW);
      }                                                   

      //2 leg conversion, action 40
      else if (ch6 > ACT40_MIN && ch6 < ACT40_MAX) 
      { 
        Serial.println("2 leg button");
        digitalWrite(LEFT_A_PIN, HIGH);
        digitalWrite(RIGHT_A_PIN, HIGH);
        digitalWrite(LEFT_B_PIN, LOW);
        digitalWrite(RIGHT_B_PIN, LOW);
        
        interlock = 3;// set interlock to 2 leg lock
      }

      //foot up, action 50
      else if (ch6 > ACT50_MIN && ch6 < ACT50_MAX && interlock == 3)
      {  
        Serial.println("UP button");
        digitalWrite(CENTER_DOWN_PIN, HIGH);
        digitalWrite(CENTER_UP_PIN, LOW);  
      }                 
    }
  }
}    
// -------------------end 2-3-2 mode stuff------------             
 
 
       
