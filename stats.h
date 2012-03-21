//*******************stats.h***************
//Alex Ford and Andy Pande 10/15/10
//Lab 09: Software for Cycling Computer
//TA: Raffaele Cetrul
//Date fo last Revision: 11/05/10
//
// Description: These are the actual numbers (performance statics) that will
// be displayed on the LCD. They are calculated and then dispalyed.

#include <stdio.h>

#define WHEEL_CIRCUM_KM   2095 //actually 2.095 meters
#define WHEEL_CIRCUM_MI   1301
#define MI               1
#define KM               0
#define WORK_MODERATE_MI  259  //.259 calories /lb/mi
#define WORK_MODERATE_KM  161  //.161 calories /lb/km


//***************Get_Distance******************
//Calculates the distance traveled based on wheel rotations and displays info on LCD
//Input: Number of wheel rotations
//Output: none
void Get_Distance(unsigned long rotations, unsigned char units);

//***************Get_Power******************
//Calculates the Power done by rider based on Cadence and weight. Displays info on LCD
//Input: Number of crank rotations, weight lbs
//Output: none
void Get_Power(long rotations, short weight);

//***************Get_Stopwatch******************
//Displays the time you have been riding in hours, minutes, seconds, miliseconds
//Input: the hour, minute, second and milisecond to display on the LCD
//Output: none
void Get_Stopwatch(short hour, short minute, short second, short msecond);

//***************Get_Countdown******************
//Displays the time you have been riding in hours, minutes, seconds, miliseconds
//Input: the hour, minute, second and milisecond to display on the LCD
//Output: none
void Get_Countdown(short hour, short minute, short second, short msecond);

//***************Get_Speed******************
//Displays the speed at which you are travelling
//Input: the total rotation count and the total rotation count 1 second ago
//Output: none
void Get_Speed(unsigned short *buffer, unsigned char units, unsigned long time);

//***************Get_AvgSpeed******************
//Displays the average speed over the whole trip
//Input: the total time spent on the trip, and the total rotations done over the whole trip
//Output: none
void Get_AvgSpeed(unsigned long time, unsigned long rotations, unsigned char units);

void Get_Cadence(unsigned short *buffer, unsigned long time);

void Get_Weight(unsigned short weight);

void Get_Cals(unsigned long crankRotations);

