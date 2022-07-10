#ifndef CONSTANTS_H
#define CONSTANTS_H

/* pin Definitions */
#define CH5_PIN 2//LCD actions channel
#define CH6_PIN 4//buttons channel

//Active low signals to drive relay H-bridge circuits
#define LEFT_A_PIN 8 //3-leg mode
#define LEFT_B_PIN 9//2-leg mode
#define RIGHT_A_PIN 10//3-leg mode
#define RIGHT_B_PIN 11//2-leg mode
#define CENTER_UP_PIN 12//2-leg mode
#define CENTER_DOWN_PIN 7//3-leg mode

//button limits
/*
 * CH6
 * 0 - 988 - 996
 * 10 -
 * 20 -
 * 30 -
 * 40 -
 * 50 -
 * 60 -
 * 70 -
 * 80 -
 * 90 -
 * 100 -
 * 110 -
 * 120 - 1



/* PWM Channels */
#define FLAP1 0
#define FLAP2 1
#define FLAP3 2
#define FLAP4 3
#define SIDE_FLAP 4

#define LFS_BASE 5
//..
#define LFS_DISH 14
#define PROJECTOR 15

/* Servo positions */
#define FLAP_OPEN 2300
#define FLAP_CLOSED 1200

#define SIDE_FLAP_OPEN 2000
#define SIDE_FLAP_CLOSED 400

#define PROJECTOR_UP 500
#define PROJECTOR_DOWN 1300

#define LFS_LEFT 2400
#define LFS_MIDDLE 1400
#define LFS_RIGHT 400

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

#endif


/*
 * CH6
 * 0 - 988 - 996
 * 10 - 1004 - 1013
 * 20 - 1019 - 1028
 * 30 - 1037 - 1048
 * 40 - 1058 - 1071
 * 50 - 1083 - 1097
 * 60 - 1108 - 1125
 * 70 - 1139 - 1159
 * 80 - 1175 - 1198
 * 90 - 1219 - 1243
 * 100 - 1270 - 1292
 * 110 - 1333 - 1354
 * 120 - 1409 - 1451
 *
 * 130 - 1509 - 1533
 * 140 - 1627 - 1649
 * 150 - 1732 - 1753
 * 160 - 1816 - 1845
 * 170 - 1878 - 1903
 * 180 - 1935 - 1951
 * 190 - 1959 - 1966
 * 200 - not sny higher???
 * 210
 * 220
 * 230
 * 240
 * 250
 *
 *
 */
