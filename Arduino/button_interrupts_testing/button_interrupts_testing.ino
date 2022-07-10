#define CH6_PIN 2
volatile unsigned long start_time = 0;
volatile unsigned long current_time = 0;
volatile unsigned long pulses = 0;
volatile int pulse_width = 0;
int counter = 0;
unsigned long button_sample_time = 0;
unsigned long last_press = 0;

/*
 * CH6 - Buttons
 * 0 - 996
 * 1 - 1028
 * 2 - 1068
 * 3 - 1112
 * 4 - 1168
 * 5 - 1236
 * 6 - 1320
 * 7 - 1432
 * 8 - 1584
 * 9 - 1780
 * 10 - 1964
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

//min - 992
//max - 1976
void setup() 
{
  Serial.begin(115200);
  pinMode(CH6_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(CH6_PIN), pulse_timer, CHANGE);
}

int max = 900;
int min = 2000;

void loop() 
{  
  if(millis() - button_sample_time >=100)//throttle button sampling
  {
    cli();//disable interrupts
    
    int button = button_check(pulse_width);//figure out which button was pressed
    
    if(button > 0 && millis() - last_press > 1000)//debounce the button response
    {
      Serial.print("Button: ");
      Serial.println(button);  
      last_press = millis();
    }
    button_sample_time = millis();

    sei();//enable interrupts
  }
}

//function for determining which button was pressed
int button_check(int pulse)
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

void pulse_timer(void)
{
  current_time  = micros();
  if(current_time > start_time)
  {
    pulses = current_time - start_time;
    
    if(pulses < 2000)
      pulse_width = pulses;
      
    start_time = current_time;
  }
}
