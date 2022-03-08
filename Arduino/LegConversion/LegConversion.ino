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

#define CH5_PIN 2
#define CH6_PIN 4

//Active low signals to drive relay H-bridge circuits
#define LEFT_A_PIN 8 //3-leg mode
#define LEFT_B_PIN 9//2-leg mode
#define RIGHT_A_PIN 10//3-leg mode
#define RIGHT_B_PIN 11//2-leg mode
#define CENTER_UP_PIN 12//2-leg mode
#define CENTER_DOWN_PIN 13//3-leg mode

void setup() 
{
  //set relay pins to output, and disable them 
  for(int i = 8; i < 14; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
  Serial.begin(115200);
}

void loop() 
{
  unsigned long currentMillis = millis();
  
  ch5 = pulseIn(CH5_PIN, HIGH, 25000); // Read the pulse width of 
  ch6 = pulseIn(CH6_PIN, HIGH, 25000); // each channel
  
  if (ch5 > 1060 || ch6 > 1060) // dirty hack to allow PWM/analog voltage to stabalise 
  {          
    delay(100);
    ch5 = pulseIn(CH5_PIN, HIGH, 25000); // Read the pulse width of 
    ch6 = pulseIn(CH6_PIN, HIGH, 25000); // each channel
  }  
  
  // -------------------start 2-3-2 mode stuff------------
  else if (ch6 > 1060 && ch6 < 1068)
  {  
    Serial.println("2-3-2 mode started");
    // 2-3-2 interlock
    interlock = 1;
    //setall(80,0,0);
    while (interlock != 0) 
    {
      delay(300); // wait for button press to end
      ch6 = pulseIn(CH6_PIN, HIGH, 25000); // read channel      
      if (ch5 > 1060 || ch6 > 1060) 
      {         // dirty hack to allow PWM/analog voltage to stabalise 
        delay(100);
        ch6 = pulseIn(CH6_PIN, HIGH, 25000);        // read channel / decide what to do
      }

      //this button press exits the 2-3-2 interlock
      if (ch6 > 1060 && ch6 < 1068) 
      {

        //disable all relays
        for(int i = 8; i < 14; i++) 
          digitalWrite(i, HIGH);
          delay(200);
          interlock = 0;  // break out of interlock
       }
       else if (ch6 > 1070 && ch6 < 1082) 
       {  // start putting foot down
          digitalWrite(13,HIGH);              // put foot down
          interlock = 2;                      // set interlock to 3 leg mode lock
       }                                   // end putting foot down
          
       else if (ch6 > 1090 && ch6 < 1099 && interlock == 2)// start shoulder conversion to 3 legs 
       { 
          digitalWrite(10,HIGH);
       }                                                      // end shoulder conversion to 3 legs
  
      else if (ch6 > 1108 && ch6 < 1119) // start conversion to 2 legs
      { 
        digitalWrite(11, HIGH);
        interlock = 3;                      // set interlock to 2 leg lock
      }
  
      else if (ch6 > 1130 && ch6 < 1142 && interlock == 3) // start putting foot up
      {  
        digitalWrite(12,HIGH);  
      }                 
    }
  }
}    
// -------------------end 2-3-2 mode stuff------------             
 
 
       