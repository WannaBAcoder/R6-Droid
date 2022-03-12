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

int min = 2000, max = 0;

void loop() 
{
 

/*
 * CH6
 * 0 - 988 - 996
 * 10 - 1004 - 1013
 * 20 - 1019 - 1028
 * 30 - 1037 - 1048
 * 40 - 1058 - 1071
 * 50 - 1083 - 1097
 * 60 - 1108 - 1125
 * 70 - 1139 - 1159
 * 80 - 1175 - 1198
 * 90 - 1219 - 1243
 * 100 - 1270 - 1292
 * 110 - 1333 - 1354
 * 120 - 1409 - 1451
 * 
 * 130 - 1509 - 1533
 * 140 - 1627 - 1649
 * 150 - 1732 - 1753
 * 160 - 1816 - 1845
 * 170 - 1878 - 1903
 * 180 - 1935 - 1951
 * 190 - 1959 - 1966
 * 200 - not sny higher???
 * 210
 * 220
 * 230
 * 240
 * 250
 * 
 * 
 */
  
  unsigned long currentMillis = millis();
  
  ch5 = pulseIn(CH5_PIN, HIGH, 25000); // Read the pulse width of 
  ch6 = pulseIn(CH6_PIN, HIGH, 25000); // each channel

  if(ch6 > 0)
  {
    if(ch6 < min)
      min = ch6;
    if(ch6 > max)
      max = ch6;  
      
    Serial.print("CH6 min = ");
    Serial.println(min);
    Serial.print("CH6 max = ");
    Serial.println(max);
  }
  /*
  if (ch5 > 1060 || ch6 > 1060) // dirty hack to allow PWM/analog voltage to stabalise 
  {          
    delay(100);
    ch5 = pulseIn(CH5_PIN, HIGH, 25000); // Read the pulse width of 
    ch6 = pulseIn(CH6_PIN, HIGH, 25000); // each channel
  }  
  
  // -------------------start 2-3-2 mode stuff------------
  //action 40
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
      { // dirty hack to allow PWM/analog voltage to stabalise 
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
          interlock = 0;// break out of interlock
      }
      //foot down
      else if (ch6 > 1070 && ch6 < 1082) 
      {  
        Serial.println("DOWN button");
        digitalWrite(CENTER_UP_PIN, HIGH);
        digitalWrite(CENTER_DOWN_PIN, LOW);
        interlock = 2;// set interlock to 3 leg mode lock
      }                                   

      //3 leg conversion
      else if (ch6 > 1090 && ch6 < 1099 && interlock == 2)// start shoulder conversion to 3 legs 
      {         
        Serial.println("3 leg button");
        digitalWrite(LEFT_B_PIN, HIGH);
        digitalWrite(RIGHT_B_PIN, HIGH);
        digitalWrite(LEFT_A_PIN, LOW);
        digitalWrite(RIGHT_A_PIN, LOW);
      }                                                   

      //2 leg conversion
      else if (ch6 > 1108 && ch6 < 1119) 
      { 
        Serial.println("2 leg button");
        digitalWrite(LEFT_A_PIN, HIGH);
        digitalWrite(RIGHT_A_PIN, HIGH);
        digitalWrite(LEFT_B_PIN, LOW);
        digitalWrite(RIGHT_B_PIN, LOW);
        
        interlock = 3;// set interlock to 2 leg lock
      }

      //foot up
      else if (ch6 > 1130 && ch6 < 1142 && interlock == 3)
      {  
        Serial.println("UP button");
        digitalWrite(CENTER_DOWN_PIN, HIGH);
        digitalWrite(CENTER_UP_PIN, LOW);  
      }                 
    }
  }
  */
}    
// -------------------end 2-3-2 mode stuff------------             
 
 
       
