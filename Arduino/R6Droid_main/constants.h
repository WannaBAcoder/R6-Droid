#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <Wire.h>
#include <EEPROM.h>
#include <Adafruit_PWMServoDriver.h>

/* RX pin Definitions */
#define CH5_PIN 2
#define CH6_PIN 3

/* 2-3-2 Drive mode relays */
#define LEFT_A_PIN 8 //3-leg mode
#define LEFT_B_PIN 9//2-leg mode
#define RIGHT_A_PIN 10//3-leg mode
#define RIGHT_B_PIN 11//2-leg mode
#define CENTER_UP_PIN 12//2-leg mode
#define CENTER_DOWN_PIN 7//3-leg mode

/* PWM Channels */
#define FLAP1 0
#define FLAP2 1
#define FLAP3 2
#define FLAP4 3
#define SIDE_FLAP 4

#define LFS_BASE 8
#define LFS_DISH 14

#define PROJECTOR 15

#define SIDE_FLAP_OPEN 2000
#define SIDE_FLAP_CLOSED 400

#define PROJECTOR_UP 1700
#define PROJECTOR_DOWN 2500

#define LFS_UP 1600
#define LFS_DOWN 1400
#define LFS_OFF 1500

#define LFS_LEFT 2400
#define LFS_HOME 1000
#define LFS_RIGHT 200

#define UPPER_ARM 0
#define UPPER_ARM_OPEN 2000
#define UPPER_ARM_CLOSED 400

#define LOWER_ARM 15
#define LOWER_ARM_OPEN 2400
#define LOWER_ARM_CLOSED 400

/* i2c devices */
#define ARM_PWM 0x41
#define HEAD_PWM 0x42
#define PERISCOPE_ADDR 0x55
#define PERISCOPE_ACTIVATE 0xAA

#define SLACK 5
#define NO_PRESS 995

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

#define ARM_INTERVAL 15000
#define PROJECTOR_INTERVAL 1000
#define BEEP_INTERVAL 25000
#define FLAPS_INTERVAL 31000
#define LFS_INTERVAL 120000


/* Servo positions */
int TOP_FLAPS_OPEN[4] = {2300, 2050, 2400, 2300};
int TOP_FLAPS_CLOSED[4] = {1450, 1200, 1800, 1400};

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

Adafruit_PWMServoDriver head = Adafruit_PWMServoDriver(HEAD_PWM);
Adafruit_PWMServoDriver arms = Adafruit_PWMServoDriver(ARM_PWM);

/* Head Function state machine handling */
enum LFS_STATES{UP, LOOK, DOWN, OFF};
int LFS_state = OFF;

bool top_flaps_waiting = 0;
bool side_flap_waiting = 0;
bool projector_waiting = 0;
bool periscope_waiting = 0;
bool arms_waiting = 0;
int LFS_looking = 0;

unsigned long periscope_start_time = 0;
unsigned long projector_start_time = 0;
unsigned long top_flap_start_time = 0;
unsigned long side_flap_start_time = 0;
unsigned long arm_start_time = 0;

unsigned long current_millis = 0;
unsigned long LFS_up_start = 0;
unsigned long LFS_down_start = 0;
unsigned long LFS_look_start = 0;

int lcd_command = 0;
int button_command = 0;
int button_command_running[30] = {0};

bool periscope_running = 0;
bool LFS_running = 0;
bool top_flaps_running = 0;
bool side_flap_running = 0;
bool projector_running = 0;
bool arms_running = 0;

bool timed_arms = 0;
bool timed_projector = 0;
bool timed_flaps = 0;
bool timed_beep = 0;
bool timed_LFS = 0;

unsigned long arm_time = 0;
unsigned long projector_time = 0;
unsigned long top_flaps_time = 0;
unsigned long beep_time = 0;

int interlock = 0;

#endif
