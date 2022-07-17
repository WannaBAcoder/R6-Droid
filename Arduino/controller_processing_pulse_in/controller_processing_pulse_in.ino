#define CH5_PIN 2
#define CH6_PIN 3

#define SLACK 5
#define NO_PRESS 995

//lcd functions
int lcd_max = 0;
int lcd_pulse_width = 0;
bool get_lcd_max = 0;
unsigned long lcd_max_start_time = 0;
unsigned long lcd_max_end_time = 0;

//buttons
int button_max = 0;
int button_pulse_width = 0;
bool get_button_max = 0;
unsigned long button_max_start_time = 0;
unsigned long button_max_end_time = 0;

/*
 * CH5 - LCD functions
*/
#define ACTION1 1008//interlock
#define ACTION2 1020//leg down
#define ACTION3 1042//3 leg mode
#define ACTION4 1064//2 leg mode
#define ACTION5 1085//leg up
#define ACTION6 1110
#define ACTION7 1139
#define ACTION8 1168
#define ACTION9 1198
#define ACTION10 1233

#define SOUND1 1272
#define SOUND2 1319
#define SOUND3 1366
#define SOUND4 1423
#define SOUND5 1484
#define SOUND6 1563
#define SOUND7 1651
#define SOUND8 1748
#define SOUND9 1847
#define SOUND10 1934

/*
 * CH6 - Button functions
*/
#define BUTTON1 1023
#define BUTTON2 1065
#define BUTTON3 1115
#define BUTTON4 1174
#define BUTTON5 1248
#define BUTTON6 1339
#define BUTTON7 1459
#define BUTTON8 1629
#define BUTTON9 1830
#define BUTTON10 1968

void setup() 
{
  Serial.begin(115200);
  pinMode(CH5_PIN, INPUT);
  pinMode(CH6_PIN, INPUT);

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
        
        //Serial.println(lcd_max);
        //Serial.println();
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
        
        //Serial.println(button_max);  
        // Serial.println();
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
