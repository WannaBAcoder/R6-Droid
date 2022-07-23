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
#define MIDDLE 80

#define NUM_PIXELS 4

#define LOOK_TIME 1500//milliseconds between position changes

Servo move_servo; 
Servo look_servo; 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);

bool running = 0;


void setup() 
{
  Serial.begin(115200);
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

  
  //set up as i2c slave
  Wire.begin(0x55);// join i2c bus
  Wire.onReceive(receive);
}

void loop() 
{
  if(running)
  {
    move_servo.writeMicroseconds(UP);
    delay(1750);
    move_servo.writeMicroseconds(OFF);
    look();
    delay(250);
    move_servo.writeMicroseconds(DOWN);
    delay(1500);
    move_servo.writeMicroseconds(OFF);
    running = 0;
  }
}
  

void look(void)
{
  //turn on neopixels
  for(int i = 0; i < NUM_PIXELS; i++)
    pixels.setPixelColor(i, pixels.Color(138,66,245));
  pixels.show();
  delay(500);
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
  
  return;
}

void receive(void) 
{
  int val = Wire.read();    // receive byte as an integer
  Serial.println(val);         // print the integer
  
  if(val == 0xAA && !running)
    running = 1;
}
