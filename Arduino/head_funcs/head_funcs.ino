#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "constants.h"

Adafruit_PWMServoDriver head = Adafruit_PWMServoDriver(HEAD_PWM);

enum LFS_STATES{UP, LOOK, DOWN, OFF};
int LFS_state = OFF;

bool top_flaps_waiting = 0;
bool side_flap_waiting = 0;
bool projector_waiting = 0;
int LFS_looking = 0;

unsigned long projector_start_time = 0;
unsigned long top_flap_start_time = 0;
unsigned long side_flap_start_time = 0;
unsigned long current_millis = 0;
unsigned long LFS_up_start = 0;
unsigned long LFS_down_start = 0;
unsigned long LFS_look_start = 0;
int command = 0;

void setup() 
{
  Serial.begin(115200);
  
  head.begin();
  head.setOscillatorFrequency(27000000);
  head.setPWMFreq(50);  // Analog servos run at ~50 Hz updates

  Serial.println("start");

  
  //need to initialize all servo positions in setup before running everything
  for(int i = 0; i < 4; i++)
    head.writeMicroseconds(i, TOP_FLAPS_CLOSED[i]);
    
  head.writeMicroseconds(SIDE_FLAP, SIDE_FLAP_CLOSED);
  head.writeMicroseconds(PROJECTOR, PROJECTOR_DOWN);
  head.writeMicroseconds(LFS_DISH, LFS_HOME);
  head.writeMicroseconds(LFS_BASE, LFS_OFF);

}

void loop() 
{
  current_millis = millis();
  
  if(Serial.available())//need to debounce the trigger conditions for 1s from the controller
  {
    command = Serial.readStringUntil('\n').toInt(); 
    Serial.println(command);
  }
  
  if(command == 1)//side flap
  {
    if(!side_flap_waiting)
    {
      update_head_servo(SIDE_FLAP, 0, SIDE_FLAP_OPEN, SIDE_FLAP_CLOSED);
      side_flap_start_time = millis();
      side_flap_waiting = 1;
    }
    
    if(millis() - side_flap_start_time >= 1000)
    {
      update_head_servo(SIDE_FLAP, 1, SIDE_FLAP_OPEN, SIDE_FLAP_CLOSED);
      side_flap_waiting = 0;
      command = 0;
    }
  }

  if(command == 2)//top flaps, sequentially triggered
  {
    if(!top_flaps_waiting)
    {
      for(int i = 0; i < 4; i++)//open each flap sequentially
        update_head_servo(i, 0, TOP_FLAPS_OPEN[i], TOP_FLAPS_CLOSED[i]);
      top_flap_start_time = millis();  
      top_flaps_waiting = 1;
    }

    else
    {
      if(millis() - top_flap_start_time >= 1000)
      {
        for(int i = 0; i < 4; i++)//close each flap sequentially
          update_head_servo(i, 1, TOP_FLAPS_OPEN[i], TOP_FLAPS_CLOSED[i]);
        command = 0;
        top_flaps_waiting = 0;
      }
    }
  }

  if(command == 3)//projector
  {
    if(!projector_waiting)
    {
      //turn on neopixel for projector?
      update_head_servo(PROJECTOR, 1, PROJECTOR_DOWN, PROJECTOR_UP);
      projector_waiting = 1;
      projector_start_time = millis();
    }
    
    if(millis() - projector_start_time >= 1000)
    {
      Serial.println("here");
      update_head_servo(PROJECTOR, 0, PROJECTOR_DOWN, PROJECTOR_UP);
      projector_waiting = 0;
      command = 0;
    }
  }

  if(command == 4)//periscope
  {
    //trigger i2c command to aux arudino to run periscope sequence
    //Wire.beginTransmission(PERISCOPE_ADDR);
   // Wire.write(PERISCOPE_ACTIVATE);
    //Wire.endTransmission();
    command = 0;
  }

  if(command == 5)//life form scanner
  {
    if(LFS_state == OFF)
      LFS_state = UP;
    
    //states: up, look, down, off
    switch(LFS_state)
    {
      case UP:
        head.writeMicroseconds(LFS_BASE, LFS_UP);
        LFS_state = LOOK;
        LFS_up_start = millis();
      break;
      case LOOK:
        if(millis() - LFS_up_start >= 3700 && LFS_looking == 0)//time limit to run continuous rot. servo up
        {          
          head.writeMicroseconds(LFS_BASE, LFS_OFF);
          update_head_servo(LFS_DISH, 0, LFS_LEFT, LFS_HOME);
          LFS_look_start = millis();
          LFS_looking = 1;
        }
        
        if(LFS_looking == 1)
        {
          if(millis() - LFS_look_start >= 1000)
          {
            //look right
            update_head_servo(LFS_DISH, 1, LFS_LEFT, LFS_RIGHT);
            LFS_look_start = millis();
            LFS_looking = 2;
          }
        }

        if(LFS_looking == 2)
        {
          if(millis() - LFS_look_start >= 1000)
          {
            update_head_servo(LFS_DISH, 0, LFS_HOME, LFS_RIGHT);
            LFS_state = DOWN;
            head.writeMicroseconds(LFS_BASE, LFS_DOWN);
            LFS_down_start = millis();
            LFS_looking = 0;
          }
        }
      break;
      case DOWN:
        if(millis() - LFS_down_start >= 4750)
        {        
          head.writeMicroseconds(LFS_BASE, LFS_OFF);
          LFS_state = OFF;
          command = 0;
        }
      break;
 
    }
  }
  //head functions
  /*
   * i2c to servo controller
   * head flaps - button enables periodic state of opening/closing each of the flaps
   * side flap - button toggles open/closed state of side flap
   * life form scanner - button triggers sequence of up, look, return, down
   * projector - button triggers up/down movement sequence of the projector
   * 
   * periscope - button triggers I2c to aux arudino comand 0xAA to address 0x55 trigger periscope sequence 
   * 
   */

  //utility arms - button triggers opening/closing of the arms
      //different button triggers periodic opening/closing of arms?
      
  //sounds - triggered by action commands
  //two buttons trigger volume up/down?
  //skip track?
  //specific tracks?
}

void update_head_servo(int servo, int state, int opened, int closed)
{
  int var = 2;
  
  if(servo == 15)
    var = 1; 
    
  if(state)
  {
    for (int microsec = opened; microsec > closed; microsec-=var)
      head.writeMicroseconds(servo, microsec); 
  }
  else
  {
    for (int microsec = closed; microsec <= opened; microsec+=var)
      head.writeMicroseconds(servo, microsec);
  }
}
