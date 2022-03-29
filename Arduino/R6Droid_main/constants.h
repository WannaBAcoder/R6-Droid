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

#define LFS_BASE 5
//..
#define LFS_DISH 14
#define PROJECTOR 15

/* Servo positions */
#define FLAP_OPEN 180
#define FLAP_CLOSED 90

//can the above constants work?
#define SIDE_FLAP_OPEN 180
#define SIDE_FLAP_CLOSED 90

#define PROJECTOR_UP 180
#define PROJECTOR_DOWN 150

#define LFS_LEFT 180
#define LFS_MIDDLE 90
#define LFS_RIGHT 0

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
