#include "CONSTANTS.h"


void setup() 
{
  Serial.begin(115200);
  pinMode(CH5_PIN, INPUT);
  pinMode(CH6_PIN, INPUT);

  //set up head PWM controllers
  head.begin();
  head.setOscillatorFrequency(27000000);
  head.setPWMFreq(50);

  arms.begin();
  arms.setOscillatorFrequency(27000000);
  arms.setPWMFreq(50);

  //intialize all head servos
  //need to initialize all servo positions in setup before running everything
  for(int i = 0; i < 4; i++)
    head.writeMicroseconds(i, TOP_FLAPS_CLOSED[i]);
    
  head.writeMicroseconds(SIDE_FLAP, SIDE_FLAP_CLOSED);
  head.writeMicroseconds(PROJECTOR, PROJECTOR_DOWN);
  head.writeMicroseconds(LFS_DISH, LFS_HOME);
  head.writeMicroseconds(LFS_BASE, LFS_OFF);

  arms.writeMicroseconds(UPPER_ARM, UPPER_ARM_CLOSED);
  arms.writeMicroseconds(LOWER_ARM, LOWER_ARM_CLOSED);

  //set relay pins to output, and disable them 
  for(int i = 7; i < 13; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  //initalize lights
  //play a startup sound?
  

}

void loop() 
{  
  lcd_pulse_width = pulseIn(CH5_PIN, HIGH, 50000); // Read the pulse width of 
  button_pulse_width = pulseIn(CH6_PIN, HIGH, 50000); // each channel
  
  unsigned long current_millis = millis();

  //determine if we have a valid action pulse width, and arent responding to an existing one again
  if(lcd_pulse_width >= NO_PRESS && !get_lcd_max)
  {
    if(current_millis - lcd_max_end_time >= 500)
    {
      get_lcd_max = 1;//start recording max for this transmit period
      lcd_max_start_time = current_millis;//record start of transmit period
    }
  }

  //determine if we have a valid sound pulse width, and arent responding to an existing one again
  if(button_pulse_width >= NO_PRESS && !get_button_max)
  {
    if(current_millis - button_max_end_time >= 500)
    {
      get_button_max = 1;//start recording max for this transmit period
      button_max_start_time = current_millis;//record start of transmit period
    }
  }
  
  if(get_lcd_max)//determine max pulse width for transmit period
  {
    if(current_millis - lcd_max_start_time >= 750)//time to compute, react
    {      
      int lcd_function = lcd_function_check(lcd_max);//figure out which button was pressed

      if(lcd_function > 0 && lcd_function <= 20)//validate button
      {
        //do action
        Serial.print("LCD function: ");
        Serial.println(lcd_function);  

        //function to start 2-3-2 mode
        if(lcd_function == 1 && interlock <=1)
        {
          if(interlock == 1)//exit interlock mode if button is pressed while in interlock
          {
            interlock = 0;
            Serial.println("Interlock released");
          }
          else  
          {
            interlock = 1;//enable interlock
            Serial.println("Interlock set");
          }
          //setall(80,0,0);
          lcd_function = 0;
        }

        // 2 leg mode
        if(lcd_function == 2 && interlock == 1)
        {
          Serial.println("Moving to 2 leg mode");
          digitalWrite(LEFT_A_PIN, HIGH);
          digitalWrite(RIGHT_A_PIN, HIGH);
          digitalWrite(LEFT_B_PIN, LOW);
          digitalWrite(RIGHT_B_PIN, LOW);
          lcd_function = 0;
          interlock = 2;
        }

        //leg up
        if(lcd_function == 3 && interlock == 2)
        {
          Serial.println("Moving leg up");
          digitalWrite(CENTER_DOWN_PIN, HIGH);
          digitalWrite(CENTER_UP_PIN, LOW);  
          lcd_function = 0;
          interlock = 3;
        }

        //leg down
        if(lcd_function == 4 && interlock == 3)
        {
          Serial.println("Moving leg down");
          digitalWrite(CENTER_UP_PIN, HIGH);
          digitalWrite(CENTER_DOWN_PIN, LOW);
          lcd_function = 0;
          interlock = 4;
        }

        //3-leg mode
        if(lcd_function == 5 && interlock == 4)
        {
          Serial.println("Moving to 3 leg mode");
          digitalWrite(LEFT_B_PIN, HIGH);
          digitalWrite(RIGHT_B_PIN, HIGH);
          digitalWrite(LEFT_A_PIN, LOW);
          digitalWrite(RIGHT_A_PIN, LOW);
          lcd_function = 0;
          interlock = 1;
        }

        if(lcd_function == 6)
        {
          timed_arms = !timed_arms;
          Serial.println("timed arms");
          lcd_function = 0;
        }

        if(lcd_function == 7)
        {
          timed_projector = !timed_projector;
          Serial.println("timed projector");
          lcd_function = 0;
        }

        if(lcd_function == 8)
        {
          timed_beep= !timed_beep;
          Serial.println("timed beep");
          lcd_function = 0;
        }
        
        if(lcd_function == 9)
        {
          timed_flaps= !timed_flaps;
          Serial.println("timed flaps");
          lcd_function = 0;
        }
       
       if(lcd_function == 10)
        {
          timed_LFS= !timed_LFS;
          Serial.println("timed LFS");
          lcd_function = 0;
        } 
      }

      get_lcd_max = 0;//finsih getting max
      lcd_max = 0;
      lcd_max_end_time = current_millis;//record time to avoid double couting same transmit
    }
    else//compute the max until for transmit period
    {
      if(lcd_pulse_width > lcd_max)
        lcd_max = lcd_pulse_width;
    }
  } 

  if(get_button_max)//determine max pulse width for transmit period
  {
    if(current_millis - button_max_start_time >= 750)//time to compute, react
    {      
      int button_function = button_check(button_max);//figure out which button was pressed

      if(button_function > 0 && button_function <= 10)
      {
        //do action
        Serial.print("button function: ");
        Serial.println(button_function);  

        if(button_function == 1 && !periscope_running)
          periscope_running = 1;
        else if(button_function == 2 && !LFS_running)
          LFS_running = 1;
        else if(button_function == 3 && !top_flaps_running)
          top_flaps_running = 1;
        else if(button_function == 4 && !side_flap_running)
          side_flap_running = 1;
        else if(button_function == 5 && !projector_running)
          projector_running = 1;  
        else if(button_function == 6 && !arms_running)
          arms_running = 1;  
        //else
        //handle other actions right here
      }
      get_button_max = 0;//finsih getting max
      button_max = 0;
      button_max_end_time = current_millis;//record time to avoid double couting same transmit
    }
    else//compute the max until for transmit period
    {
      if(button_pulse_width > button_max)
        button_max = button_pulse_width;
    }
  }
  
  if(periscope_running)
  {
    if(!periscope_waiting)
    {
      //turn on neopixel for projector?
      //trigger i2c command to aux arudino to run periscope sequence
      //Wire.beginTransmission(PERISCOPE_ADDR);
       //Wire.write(PERISCOPE_ACTIVATE);
      //Wire.endTransmission();
       periscope_waiting = 1;
       periscope_start_time = millis();
       Serial.println("Periscope Activated");
    }
      
    if(millis() - periscope_start_time >= 8000)
    {
      periscope_waiting = 0;
      periscope_running = 0;
      Serial.println("Periscope Deactivated");
    }
  }
 
    //life form scanner. Enters state machine. Can't activate other functions during this
  if(LFS_running)
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
        Serial.println("LFS up");   
      break;
      case LOOK:
        if(millis() - LFS_up_start >= 3700 && LFS_looking == 0)//time limit to run continuous rot. servo up
        {          
          head.writeMicroseconds(LFS_BASE, LFS_OFF);
          update_head_servo(LFS_DISH, 0, LFS_LEFT, LFS_HOME);
          LFS_look_start = millis();
          LFS_looking = 1;
          Serial.println("LFS look");  
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
        LFS_running = 0;
        Serial.println("LFS down"); 
      }
    break;
    }
  }
    //top flaps
  if(top_flaps_running)
  {
    if(!top_flaps_waiting)
    {
      for(int i = 0; i < 4; i++)//open each flap sequentially
        update_head_servo(i, 0, TOP_FLAPS_OPEN[i], TOP_FLAPS_CLOSED[i]);
      top_flap_start_time = millis();  
      top_flaps_waiting = 1;
      Serial.println("Flaps up");
    }

    else
    {
      if(millis() - top_flap_start_time >= 1000)
      {
        for(int i = 0; i < 4; i++)//close each flap sequentially
          update_head_servo(i, 1, TOP_FLAPS_OPEN[i], TOP_FLAPS_CLOSED[i]);
        top_flaps_running = 0;
        top_flaps_waiting = 0;
        Serial.println("Flaps down");
      }
    }
  }

  if(side_flap_running)
  {
    //side flap
    if(!side_flap_waiting)
    {
      update_head_servo(SIDE_FLAP, 0, SIDE_FLAP_OPEN, SIDE_FLAP_CLOSED);
      side_flap_start_time = millis();
      side_flap_waiting = 1;
      Serial.println("Side flap open");
    }
    
    if(millis() - side_flap_start_time >= 1000)
    {
      update_head_servo(SIDE_FLAP, 1, SIDE_FLAP_OPEN, SIDE_FLAP_CLOSED);
      side_flap_waiting = 0;
      side_flap_running = 0;
      Serial.println("Side flap closed");
    }
  }

  if(projector_running)
  {
    if(!projector_waiting)
    {
      //turn on neopixel for projector?
      update_head_servo(PROJECTOR, 1, PROJECTOR_DOWN, PROJECTOR_UP);
      projector_waiting = 1;
      projector_start_time = millis();
      Serial.println("Projector up");
    }
    
    if(millis() - projector_start_time >= 1000)
    {
      update_head_servo(PROJECTOR, 0, PROJECTOR_DOWN, PROJECTOR_UP);
      projector_waiting = 0;
      projector_running = 0;
      Serial.println("Projector down");
    }
  }

  if(arms_running)
  {
    if(!arms_waiting)
    {
      update_arm(UPPER_ARM, 1, UPPER_ARM_OPEN, UPPER_ARM_CLOSED);
      update_arm(LOWER_ARM, 1, LOWER_ARM_OPEN, LOWER_ARM_CLOSED);
      arms_waiting = 1;
      arm_start_time = millis();
      Serial.println("Arms open");
    }
       if(millis() - arm_start_time >= 1000)
    {
      update_arm(UPPER_ARM, 0, UPPER_ARM_OPEN, UPPER_ARM_CLOSED);
      update_arm(LOWER_ARM, 0, LOWER_ARM_OPEN, LOWER_ARM_CLOSED);
      
      arms_waiting = 0;
      arms_running = 0;
      Serial.println("Arms closed");
    }
  }
}

//function for determining which button was pressed
int lcd_function_check(int pulse)
{
  int lcd_val = 0;

  //parse actions
  if(pulse > NO_PRESS && pulse <= ACTION1+SLACK)// no button press
    lcd_val = 1;
  else if(pulse > ACTION1-SLACK && pulse <= ACTION2+SLACK)
    lcd_val = 2;
  else if(pulse > ACTION2-SLACK && pulse <= ACTION3+SLACK)
    lcd_val = 3;
  else if(pulse > ACTION3-SLACK && pulse <= ACTION4+SLACK)
    lcd_val = 4;
  else if(pulse > ACTION4-SLACK && pulse <= ACTION5+SLACK)
    lcd_val = 5;
  else if(pulse > ACTION5-SLACK && pulse <= ACTION6+SLACK)
    lcd_val = 6;
  else if(pulse > ACTION6-SLACK && pulse <= ACTION7+SLACK)
    lcd_val = 7;
  else if(pulse > ACTION7-SLACK && pulse <= ACTION8+SLACK)
    lcd_val = 8;
  else if(pulse > ACTION8-SLACK && pulse <= ACTION9+SLACK)
    lcd_val = 9;
  else if(pulse > ACTION9-SLACK && pulse <= ACTION10+SLACK)
    lcd_val = 10;

  //parse sounds
  else if(pulse > ACTION10-SLACK && pulse <= SOUND1+SLACK)
    lcd_val = 11;
  else if(pulse > SOUND1-SLACK && pulse <= SOUND2+SLACK)
    lcd_val = 12;
  else if(pulse > SOUND2-SLACK && pulse <= SOUND3+SLACK)
    lcd_val = 13;
  else if(pulse > SOUND3-SLACK && pulse <= SOUND4+SLACK)
    lcd_val = 14;
  else if(pulse > SOUND4-SLACK && pulse <= SOUND5+SLACK)
    lcd_val = 15;
  else if(pulse > SOUND5-SLACK && pulse <= SOUND6+SLACK)
    lcd_val = 16;
  else if(pulse > SOUND6-SLACK && pulse <= SOUND7+SLACK)
    lcd_val = 17;
  else if(pulse > SOUND7-SLACK && pulse <= SOUND8+SLACK)
    lcd_val = 18;
  else if(pulse > SOUND8-SLACK && pulse <= SOUND9+SLACK)
    lcd_val = 19;
  else if(pulse > SOUND9-SLACK && pulse <= SOUND10+SLACK)
    lcd_val = 20;
  else
    lcd_val = 0;  
  
  return lcd_val;
}

//function for determining which button was pressed
int button_check(int pulse)
{
  int button_val = 0;
 if(pulse > NO_PRESS && pulse <= BUTTON1+SLACK)// no button press
    button_val = 1;
  else if(pulse > BUTTON1-SLACK && pulse <= BUTTON2+SLACK)// no button press
    button_val = 2;
  else if(pulse > BUTTON2-SLACK && pulse <= BUTTON3+SLACK)// no button press
    button_val = 3;
  else if(pulse > BUTTON3-SLACK && pulse <= BUTTON4+SLACK)// no button press
    button_val = 4;
  else if(pulse > BUTTON4-SLACK && pulse <= BUTTON5+SLACK)// no button press
    button_val = 5;
  else if(pulse > BUTTON5-SLACK && pulse <= BUTTON6+SLACK)// no button press
    button_val = 6;
  else if(pulse > BUTTON6-SLACK && pulse <= BUTTON7+SLACK)// no button press
    button_val = 7;
  else if(pulse > BUTTON7-SLACK && pulse <= BUTTON8+SLACK)// no button press
    button_val = 8;
  else if(pulse > BUTTON8-SLACK && pulse <= BUTTON9+SLACK)// no button press
    button_val = 9;
  else if(pulse > BUTTON9-SLACK && pulse <= BUTTON10+SLACK)// no button press
    button_val = 10;
  else
    button_val = 0;  

  return button_val;
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

void update_arm(int arm, int state, int opened, int closed)
{    
  if(state)
  {
    for (int microsec = opened; microsec > closed; microsec-=5)
      arms.writeMicroseconds(arm, microsec); 
  }
  else
  {
    for (int microsec = closed; microsec <= opened; microsec+=5)
      arms.writeMicroseconds(arm, microsec);
  }
}
