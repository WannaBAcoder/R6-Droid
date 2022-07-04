#ifndef CONSTANTS_H
#define CONSTANTS_H

/* pin Definitions */
#define CH5_PIN 2
#define CH6_PIN 4

//Active low signals to drive relay H-bridge circuits
#define LEFT_A_PIN 8 //3-leg mode
#define LEFT_B_PIN 9//2-leg mode
#define RIGHT_A_PIN 10//3-leg mode
#define RIGHT_B_PIN 11//2-leg mode
#define CENTER_UP_PIN 12//2-leg mode
#define CENTER_DOWN_PIN 7//3-leg mode

/* Remote signal states */
#define ACT10_MIN 1004
#define ACT10_MAX 1013
#define ACT20_MIN 1019
#define ACT20_MAX 1028
#define ACT30_MIN 1037
#define ACT30_MAX 1048
#define ACT40_MIN 1058
#define ACT40_MAX 1071
#define ACT50_MIN 1083
#define ACT50_MAX 1097

/* PWM Channels */
#define FLAP1 0
#define FLAP2 1
#define FLAP3 2
#define FLAP4 3
#define SIDE_FLAP 4

#define LFS_BASE 8
#define LFS_DISH 14

#define PROJECTOR 15

/* Servo positions */
int TOP_FLAPS_OPEN[4] = {2300, 2050, 2400, 2300};
int TOP_FLAPS_CLOSED[4] = {1450, 1200, 1800, 1400};


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
