#define CH5_PIN 3
volatile unsigned long lcd_start_time = 0;
volatile unsigned long lcd_current_time = 0;
volatile unsigned long lcd_pulses = 0;
volatile int lcd_pulse_width = 0;

unsigned long lcd_sample_time = 0;
unsigned long lcd_last_press = 0;
volatile bool get_max = 0;
volatile unsigned long max_start_time = 0;
volatile unsigned long max_end_time = 0;
/*
 * CH5 - LCD functions
*/
#define SLACK 10
#define NO_SCREEN_PRESS 996
#define ACTION1 1008//no press
#define ACTION2 1020//interlock
#define ACTION3 1036//leg down
#define ACTION4 1052//3 leg mode
#define ACTION5 1072//2 leg mode
#define ACTION6 1088//leg up
#define ACTION7 1112
#define ACTION8 1136
#define ACTION9 1160
#define ACTION10 1184

#define SOUND1 1216
#define SOUND2 1240
#define SOUND3 1280
#define SOUND4 1324
#define SOUND5 1376
#define SOUND6 1428
#define SOUND7 1492
#define SOUND8 1568
#define SOUND9 1652
#define SOUND10 1732


void setup() 
{
  Serial.begin(115200);
  pinMode(CH5_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CH5_PIN), lcd_pulse_timer, CHANGE);
}

int max = 0;
int min = 2000;

void loop() 
{  

  if(get_max)//determine max pulse width for transmit period
  {
    if(millis() - max_start_time >= 750)//time to compute, react
    {      
      int lcd_function = lcd_function_check(max);//figure out which button was pressed
    
      Serial.print("LCD functoin: ");
      Serial.println(lcd_function);  
      Serial.println(max);
    
      get_max = 0;//finsih getting max
      max = 0;//reset max for next time
      max_end_time = millis();//record time to avoid double couting same transmit
    }
    else//compute the max until for transmit period
    {
      if(lcd_pulse_width > max)
        max = lcd_pulse_width;
    }
  } 
}

//function for determining which button was pressed
int lcd_function_check(int pulse)
{
  int lcd_val = 0;

  //parse actions
  if(pulse > 997 && pulse < ACTION1)// no button press
    lcd_val = 1;
  else if(pulse > ACTION1-SLACK && pulse < ACTION2)
    lcd_val = 2;
  else if(pulse > ACTION2-SLACK && pulse < ACTION3)
    lcd_val = 3;
  else if(pulse > ACTION3-SLACK && pulse < ACTION4)
    lcd_val = 4;
  else if(pulse > ACTION4-SLACK && pulse < ACTION5)
    lcd_val = 5;
  else if(pulse > ACTION5-SLACK && pulse < ACTION6)
    lcd_val = 6;
  else if(pulse > ACTION6-SLACK && pulse < ACTION7)
    lcd_val = 7;
  else if(pulse > ACTION7-SLACK && pulse < ACTION8)
    lcd_val = 8;
  else if(pulse > ACTION8-SLACK && pulse < ACTION9)
    lcd_val = 9;
  else if(pulse > ACTION9-SLACK && pulse < ACTION10)
    lcd_val = 10;

  //parse sounds
  else if(pulse > ACTION10-SLACK && pulse < SOUND1)
    lcd_val = 11;
  else if(pulse > SOUND1-SLACK && pulse < SOUND2)
    lcd_val = 12;
  else if(pulse > SOUND2-SLACK && pulse < SOUND3)
    lcd_val = 13;
  else if(pulse > SOUND3-SLACK && pulse < SOUND4)
    lcd_val = 14;
  else if(pulse > SOUND4-SLACK && pulse < SOUND5)
    lcd_val = 15;
  else if(pulse > SOUND5-SLACK && pulse < SOUND6)
    lcd_val = 16;
  else if(pulse > SOUND6-SLACK && pulse < SOUND7)
    lcd_val = 17;
  else if(pulse > SOUND7-SLACK && pulse < SOUND8)
    lcd_val = 18;
  else if(pulse > SOUND8-SLACK && pulse < SOUND9)
    lcd_val = 19;
  else if(pulse > SOUND9-SLACK && pulse < SOUND10)
    lcd_val = 20;
  else
    lcd_val = 0;  

  return lcd_val;
}

//calculate LCD function pulse widths
void lcd_pulse_timer(void)
{
  lcd_current_time  = micros();
  if(lcd_current_time > lcd_start_time)
  {
    lcd_pulses = lcd_current_time - lcd_start_time;
    
    if(lcd_pulses < 2000)
      lcd_pulse_width = lcd_pulses;

    //determine if we have a valid action pulse width, and arent responding to an existing one again
    if(lcd_pulse_width > 996 && !get_max)
    {
      if(millis() - max_end_time >= 250 )
      {
        get_max = 1;//start recording max for this transmit period
        max_start_time = millis();//record start of transmit period
      }
    }
      
    lcd_start_time = lcd_current_time;
  }
}
