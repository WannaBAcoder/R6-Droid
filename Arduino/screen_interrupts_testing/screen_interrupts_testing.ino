#define CH5_PIN 3
volatile unsigned long lcd_start_time = 0;
volatile unsigned long lcd_current_time = 0;
volatile unsigned long lcd_pulses = 0;
volatile int lcd_pulse_width = 0;

unsigned long lcd_sample_time = 0;
unsigned long lcd_last_press = 0;

/*
 * CH5 - LCD functions
 * 
 * Actions
 * 0 - 992
 * 1 - 1004 - interlock enable
 * 2 - 1020 - leg down
 * 3 - 1044 - 3 leg mode
 * 4 - 1064 - 2 leg mode
 * 5 - 1132 - leg up
 * 6 - 1108 
 * 7 - 1140
 * 8 - 1164
 * 9 - 1196
 * 10 - 1224
 *  
 * Sounds
 * 1 - 1260
 * 2 - 1300
 * 3 - 1348
 * 4 - 1400
 * 5 - 1456
 * 6 - 1528
 * 7 - 1604
 * 8 - 1696
 * 9 - 1784
 * 10 - 1868
*/
#define NO_PRESS 996
#define BUTTON1 1028
#define BUTTON2 1068
#define BUTTON3 1112
#define BUTTON4 1168
#define BUTTON5 1236
#define BUTTON6 1320
#define BUTTON7 1432
#define BUTTON8 1584
#define BUTTON9 1780
#define BUTTON10 1964

void setup() 
{
  Serial.begin(115200);
  pinMode(CH5_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CH5_PIN), lcd_pulse_timer, CHANGE);
}

int max = 900;
int min = 2000;

void loop() 
{  
  //if(lcd_pulse_width > 992)
  //{
    if(lcd_pulse_width > max)
    {
      max = lcd_pulse_width;
    }
    Serial.println(max);
  //}


 // Serial.println(lcd_pulse_width);  
//  if(millis() - lcd_sample_time >= 100)//throttle button sampling
//  {
//    cli();//disable interrupts
//    
//    int lcd_function = lcd_function_check(lcd_pulse_width);//figure out which button was pressed
//    
//    if(lcd_function > 0 && millis() - lcd_last_press > 1000)//debounce the button response
//    {
//      Serial.print("LCD functoin: ");
//      Serial.println(lcd_function);  
//      lcd_last_press = millis();
//    }
//    lcd_sample_time = millis();
//
//    sei();//enable interrupts
//  }
}

//function for determining which button was pressed
int lcd_function_check(int pulse)
{
  int button_val = 0;
 
  if(pulse > 997 && pulse < BUTTON1)// no button press
    button_val = 1;
  else if(pulse > BUTTON1 && pulse < BUTTON2)// no button press
    button_val = 2;
  else if(pulse > BUTTON2 && pulse < BUTTON3)// no button press
    button_val = 3;
  else if(pulse > BUTTON3 && pulse < BUTTON4)// no button press
    button_val = 4;
  else if(pulse > BUTTON4 && pulse < BUTTON5)// no button press
    button_val = 5;
  else if(pulse > BUTTON5 && pulse < BUTTON6)// no button press
    button_val = 6;
  else if(pulse > BUTTON6 && pulse < BUTTON7)// no button press
    button_val = 7;
  else if(pulse > BUTTON7 && pulse < BUTTON8)// no button press
    button_val = 8;
  else if(pulse > BUTTON8 && pulse < BUTTON9)// no button press
    button_val = 9;
  else if(pulse > BUTTON9 && pulse < BUTTON10)// no button press
    button_val = 10;
  else
    button_val = 0;  
    
  return button_val;
}

void lcd_pulse_timer(void)
{
  lcd_current_time  = micros();
  if(lcd_current_time > lcd_start_time)
  {
    lcd_pulses = lcd_current_time - lcd_start_time;
    
    if(lcd_pulses < 2000)
      lcd_pulse_width = lcd_pulses;
      
    lcd_start_time = lcd_current_time;
  }
}
