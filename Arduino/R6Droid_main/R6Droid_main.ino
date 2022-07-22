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

  //run this loop once on first programming
  if(EEPROM.read(0) == 255)
  {
    for(int i = 0; i < 5; i++)
      EEPROM.write(i, 0);
  }
  
  //read saved settings for timed actions
  timed_arms = EEPROM.read(0);
  timed_projector = EEPROM.read(1);
  timed_beep = EEPROM.read(2);
  timed_flaps = EEPROM.read(3);
  timed_LFS = EEPROM.read(4);

  sound_board.begin(9600);
/*
  if (!sfx.reset()) 
  {
    Serial.println(F("Not found"));
    while (1);
  }
*/
  strip.begin();// INITIALIZE NeoPixel strip object (REQUIRED)
  for(int i = 0; i < LED_COUNT; i++)
  {
    if(i < 36)
      strip.setPixelColor(i, strip.Color(0,0,0));
    else if(i >=36 && i < 61)
      strip.setPixelColor(i, strip.Color(0,0,255));  
    else
      strip.setPixelColor(i, strip.Color(128,128,128));  
  }
 
  strip.show();/// Turn OFF all pixels ASAP
  
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
        Serial.print(F("LCD function: "));
        Serial.println(lcd_function);  

        //function to start 2-3-2 mode
        if(lcd_function == 1 && interlock <=1)
        {
          if(interlock == 1)//exit interlock mode if button is pressed while in interlock
          {
            interlock = 0;
            setall(0,0,0);
            Serial.println(F("Interlock released"));
          }
          else  
          {
            interlock = 1;//enable interlock
            Serial.println(F("Interlock set"));
            setall(80,0,0);
          }
          
          lcd_function = 0;
        }

        // 2 leg mode
        if(lcd_function == 2 && interlock == 1)
        {
          Serial.println(F("Moving to 2 leg mode"));
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
          Serial.println(F("Moving leg up"));
          digitalWrite(CENTER_DOWN_PIN, HIGH);
          digitalWrite(CENTER_UP_PIN, LOW);  
          lcd_function = 0;
          interlock = 3;
        }

        //leg down
        if(lcd_function == 4 && interlock == 3)
        {
          Serial.println(F("Moving leg down"));
          digitalWrite(CENTER_UP_PIN, HIGH);
          digitalWrite(CENTER_DOWN_PIN, LOW);
          lcd_function = 0;
          interlock = 4;
        }

        //3-leg mode
        if(lcd_function == 5 && interlock == 4)
        {
          Serial.println(F("Moving to 3 leg mode"));
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
          if(EEPROM.read(0) != timed_arms)
            EEPROM.write(0, timed_arms);
            
          Serial.println(F("timed arms"));
          lcd_function = 0;
        }

        if(lcd_function == 7)
        {
          timed_projector = !timed_projector;
          if(EEPROM.read(1) != timed_projector)
            EEPROM.write(1, timed_projector);
          Serial.println(F("timed projector"));
          lcd_function = 0;
        }

        if(lcd_function == 8)
        {
          timed_beep =!timed_beep;
          if(EEPROM.read(2) != timed_beep)
            EEPROM.write(2, timed_beep);
          Serial.println(F("timed beep"));
          lcd_function = 0;
        }
        
        if(lcd_function == 9)
        {
          timed_flaps= !timed_flaps;
          if(EEPROM.read(3) != timed_flaps)
            EEPROM.write(3, timed_flaps);
          Serial.println(F("timed flaps"));
          lcd_function = 0;
        }
       
       if(lcd_function == 10)
        {
          timed_LFS= !timed_LFS;
          if(EEPROM.read(4) != timed_LFS)
            EEPROM.write(4, timed_LFS);
          Serial.println(F("timed LFS"));
          lcd_function = 0;
        } 

        if(lcd_function > 10 && lcd_function < 21)
          sfx.playTrack((uint8_t)lcd_function-10);
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
        Serial.print(F("button function: "));
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
        else if(button_function == 7)//play random sound
          sfx.playTrack(random(1, 12));
        else if(button_function == 8)//top sound
           sfx.stop();
        else if(button_function == 9)//sound vol+
          sfx.volUp();
        else if(button_function == 10)//sound vol-
          sfx.volDown();
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

  //timed actions
  if(timed_arms && current_millis - arm_time >= ARM_INTERVAL)
  {
    arms_running = 1;
    arm_time = millis();
  }

  if(timed_projector &&current_millis - projector_time >= PROJECTOR_INTERVAL)
  {
    projector_running = 1;
    projector_time = millis();
  }

  if(timed_flaps && current_millis - flaps_time >= FLAPS_INTERVAL)
  {
    top_flaps_running = 1;
    flaps_time = millis();
  }

  if(timed_beep && current_millis - beep_time >= BEEP_INTERVAL)
  {
    //do beep here
    Serial.println(F("beep"));
    sfx.playTrack(11);//play sound 11, which is the r2d2 beep
    beep_time = millis();
  }

  if(timed_LFS &&current_millis - LFS_time >= LFS_INTERVAL)
  {
    LFS_running = 1;
    LFS_time = millis();
  }

  //start of main action handling
  if(periscope_running)
  {
    if(!periscope_waiting)
    {
      //turn on neopixel for projector?
      //trigger i2c command to aux arudino to run periscope sequence
       Wire.beginTransmission(PERISCOPE_ADDR);
       Wire.write(PERISCOPE_ACTIVATE);
       Wire.endTransmission();
       periscope_waiting = 1;
       periscope_start_time = millis();
       Serial.println(F("Periscope Activated"));
    }
      
    if(millis() - periscope_start_time >= 8000)
    {
      periscope_waiting = 0;
      periscope_running = 0;
      Serial.println(F("Periscope Deactivated"));
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
        Serial.println(F("LFS up"));   
      break;
      case LOOK:
        if(millis() - LFS_up_start >= 3700 && LFS_looking == 0)//time limit to run continuous rot. servo up
        {          
          head.writeMicroseconds(LFS_BASE, LFS_OFF);
          update_head_servo(LFS_DISH, 0, LFS_LEFT, LFS_HOME);
          LFS_look_start = millis();
          LFS_looking = 1;
          Serial.println(F("LFS look"));  
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
        Serial.println(F("LFS down")); 
      }
    break;
    }
  }
    //top flaps
  if(top_flaps_running)
  {
    if(!top_flaps_waiting)
    {
      if(current_millis - flap_time >=500)
      {
        if(flap_counter <= 3)
        {
          Serial.println(flap_counter);
          update_head_servo(flap_counter, 0, TOP_FLAPS_OPEN[flap_counter], TOP_FLAPS_CLOSED[flap_counter]);
          flap_counter++; 
        }
       
        else
        {     
          if(flap_counter == 4)
            flap_counter = 3;
          top_flap_start_time = millis();  
          top_flaps_waiting = 1;
          Serial.println(F("Flaps up"));
        }
        
        flap_time = current_millis; 
      }
    }

    else
    {
      if(current_millis - flap_time >=500)
      {
        if(flap_counter >= 0)
        {
          update_head_servo(flap_counter, 1, TOP_FLAPS_OPEN[flap_counter], TOP_FLAPS_CLOSED[flap_counter]);
          flap_counter--;
        }
        else
        {
          top_flaps_running = 0;
          top_flaps_waiting = 0;
          Serial.println(F("Flaps down"));
        }
        flap_time = current_millis;
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
      Serial.println(F("Side flap open"));
    }
    
    if(millis() - side_flap_start_time >= 1000)
    {
      update_head_servo(SIDE_FLAP, 1, SIDE_FLAP_OPEN, SIDE_FLAP_CLOSED);
      side_flap_waiting = 0;
      side_flap_running = 0;
      Serial.println(F("Side flap closed"));
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
      Serial.println(F("Projector up"));
    }
    
    if(millis() - projector_start_time >= 1000)
    {
      update_head_servo(PROJECTOR, 0, PROJECTOR_DOWN, PROJECTOR_UP);
      projector_waiting = 0;
      projector_running = 0;
      Serial.println(F("Projector down"));
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
      Serial.println(F("Arms open"));
    }
       if(millis() - arm_start_time >= 1000)
    {
      update_arm(UPPER_ARM, 0, UPPER_ARM_OPEN, UPPER_ARM_CLOSED);
      update_arm(LOWER_ARM, 0, LOWER_ARM_OPEN, LOWER_ARM_CLOSED);
      
      arms_waiting = 0;
      arms_running = 0;
      Serial.println(F("Arms closed"));
    }
  }

  if(current_millis - previousPixelMillis >= pixelTime && interlock == 0) 
  {  // logic Neopixels
    strip.setPixelColor(pixel, strip.Color(R,G,B));
    strip.setPixelColor((pixel-3), strip.Color(R,G,B));
    strip.setPixelColor((pixel-9), strip.Color(50,50,50));
    strip.setPixelColor((pixel-15), strip.Color(R,G,B));
    strip.setPixelColor(((pixel-2)), strip.Color(0,0,0));
    strip.setPixelColor(((pixel-12)), strip.Color(0,0,0));

    strip.show(); // This sends the updated pixel color to the hardware.
    pixel=pixel+1;
    
    if(flag1 == 1 || flag2 == 2)
    {
      R = 30;
      B = 0;
      G = 0;
    }
    else if(flag3 == 1 || flag3 == 2 || flag3 == 4) 
    {
      B = 30;
      R = 0;
      G = 0;
    }
    else if(flag4 != 0) 
    {
      R = 0;
      G = 30;
      B = 0;
    }
    else 
    {    
      R = 40;
      G = 30;
      B = 0;
    }    
    if(pixel==36) 
       pixel = 0;
       
    previousPixelMillis = current_millis;
  }
 
  if (flag40 == 1 && (current_millis - previousCascadeMillis >= cascadeTime)) 
  {
    flag1 = 1;
    flag40 = 2;   
    previousCascadeMillis = current_millis;
  }
  else if (flag40 == 2 && (current_millis - previousCascadeMillis >= cascadeTime)) 
  {
    flag2 = 1;
    flag40 = 0;   
    previousCascadeMillis = current_millis;
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
  if(state)
    head.writeMicroseconds(servo, closed); 
 
  else
    head.writeMicroseconds(servo, opened);
}

void update_arm(int arm, int state, int opened, int closed)
{    
  if(state)
    arms.writeMicroseconds(arm, closed); 
  else
    arms.writeMicroseconds(arm, opened);
}

int setall(int R, int G, int B) 
{  /// set all logics to one RGB colour
   for (int i = 0; i <= 35; i++) 
   {
     strip.setPixelColor(i, strip.Color(R,G,B));
     strip.show();
   }
}
     
