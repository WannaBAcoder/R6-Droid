/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 400// This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;
int pin = 15;
void setup() {
  Serial.begin(115200);
  Serial.println("8 channel Servo test!");

  pwm.begin();
  Serial.println("done");
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  delay(10);
  pwm.writeMicroseconds(15, USMAX);
  pwm.writeMicroseconds(0, USMIN);
  while(1);

//  Wire.beginTransmission(0x55);
//  Wire.write(0xAA);
//  Wire.endTransmission();
  
  //while(1);
}

void loop() 
{
  // Drive each servo one at a time using setPWM()
  for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) 
  {
    pwm.writeMicroseconds(pin, microsec);
    Serial.println(microsec);
  }

  delay(500);
  for (uint16_t microsec = USMAX; microsec > USMIN; microsec--)
    pwm.writeMicroseconds(pin, microsec);

    delay(500);
}
