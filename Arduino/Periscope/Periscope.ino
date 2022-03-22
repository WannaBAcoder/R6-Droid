#include <Wire.h>
#include <Servo.h>
#include <Adafruit_NeoPixel.h>

#define UP_PIN 2
#define DOWN_PIN 3

#define MOVE_PIN 5
#define LOOK_PIN 6

#define NEO_PIN 11

//continuous rotation servo control in microseconds
#define UP 2000
#define DOWN 1000
#define OFF 1500

//standard servo control in degrees
#define LEFT 0
#define RIGHT 180
#define MIDDLE 90

#define NUM_PIXELS 4

#define LOOK_TIME 1500//milliseconds between position changes

Servo move_servo; 
Servo look_servo; 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

enum states{waiting, go_up, look_around, go_down};
uint8_t current_state = waiting;

void setup() 
{
  Serial.begin(115200);
  pinMode(UP_PIN, INPUT_PULLUP);
  pinMode(DOWN_PIN, INPUT_PULLUP);

  move_servo.attach(MOVE_PIN);
  look_servo.attach(LOOK_PIN);

  //initiallize servo positions
  move_servo.writeMicroseconds(OFF);
  look_servo.write(MIDDLE);

  //initialize LED's to off
  pixels.begin();
  for(int i = 0; i < NUM_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  pixels.show();

  //set up limit switch interrupts
  attachInterrupt(UP_PIN, up_isr, FALLING);
  attachInterrupt(DOWN_PIN, down_isr, FALLING);
  
  //set up as i2c slave
  Wire.begin(0x55);// join i2c bus
  Wire.onReceive(receive);
}

void loop() 
{
  switch(current_state)
  {
    case waiting:
    break;
    
    case go_up:
      move_up();
    break;
    
    case look_around:
      look();
    break;
    
    case go_down:
      move_down();
    break;
  }
}        

void look(void)
{
  //turn on neopixels
  for(int i = 0; i < NUM_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(138,66,245));
  pixels.show();
  
  //look left, look right,look left, look right
  look_servo.write(LEFT);
  delay(LOOK_TIME);
  look_servo.write(RIGHT);
  delay(LOOK_TIME);
  look_servo.write(LEFT);
  delay(LOOK_TIME);
  look_servo.write(RIGHT);
  delay(LOOK_TIME);

  //return to rest position
  look_servo.write(MIDDLE);
  delay(LOOK_TIME);

  //turn off neopixels
  for(int i = 0; i < NUM_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  pixels.show();

  current_state = go_down;
  return;
}

void move_up(void)
{
  move_servo.writeMicroseconds(UP);
  return;
}

void move_down(void)
{
  move_servo.writeMicroseconds(DOWN);
  return;
}

//triggered when upper limit switch is reached
void up_isr(void)
{
  //turn off the servo
  move_servo.writeMicroseconds(OFF);
  current_state = look_around;
}

void down_isr(void)
{
  //turn off the move_servo
  move_servo.writeMicroseconds(OFF);
  current_state = waiting;
}

void receive(void) 
{
  int val = Wire.read();    // receive byte as an integer
  Serial.println(val);         // print the integer
  
  if(val == 0xAA && current_state == waiting)
    current_state = go_up;
}
       
