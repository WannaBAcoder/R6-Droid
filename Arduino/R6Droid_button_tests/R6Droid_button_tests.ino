#include "constants.h"

int pulselen;
int interlock = 0;

int ch5;
int ch6;

int min = 2000; 
int max = 0;

void setup() 
{  
  pinMode(CH5_PIN, INPUT);
  Serial.begin(115200);
}

void loop() 
{
  unsigned long currentMillis = millis();
  
  ch5 = pulseIn(CH5_PIN, HIGH); // Read the pulse width of 
  ch6 = pulseIn(CH6_PIN, HIGH); // each channel

  if(ch6 > 1000)
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
  
}      
 
 
       
