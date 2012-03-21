//*******************stats.c***************
//Alex Ford and Andy Pande 10/15/10
//Lab 09: Software for Cycling Computer
//TA: Raffaele Cetrul
//Date fo last Revision: 11/05/10
//
// Description: These are the actual numbers (performance statics) that will
// be displayed on the LCD. They are calculated and then dispalyed.
#include <mc9s12dp512.h>     /* derivative information */

#include "stats.h"
#include "lcd.h"
#include "fixed.h"



//---------------------Get_Stopwatch---------------------
// outputs the current time to the LCD display
// Input: none
// Output: none
void Get_Stopwatch(short hour, short minute, short second, short msecond){
//  unsigned char SaveCCR;
//  asm tpa  
//  asm staa SaveCCR 
//  asm sei          // make atomic 
   
  LCD_OutChar(NUM_TO_CHAR+(hour/10)); 
  LCD_OutChar(NUM_TO_CHAR+(hour%10));
  LCD_OutChar(0x3A);                              // ASCII Colon :
  LCD_OutChar(NUM_TO_CHAR+(minute/10)); 
  LCD_OutChar(NUM_TO_CHAR+(minute%10));
  LCD_OutChar(0x3A);                              // ASCII Colon :
  LCD_OutChar(NUM_TO_CHAR+(second/10)); 
  LCD_OutChar(NUM_TO_CHAR+(second%10));
  LCD_OutChar(0x3A);                              // ASCII Colon :
  LCD_OutChar(NUM_TO_CHAR+(msecond/10)); 
  LCD_OutChar(NUM_TO_CHAR+(msecond%10));
  
//  asm ldaa SaveCCR
//  asm tap          // end critical section
//  asm cli
}


//***************Get_Distance******************
//Calculates the distance traveled based on wheel rotations and displays info on LCD
//Input: Number of wheel rotations
//Output: none
void Get_Distance(unsigned long rotations, unsigned char units){
  unsigned long distance;
  /*
  unsigned char SaveCCR;
  asm tpa  
  asm staa SaveCCR 
  asm sei          // make atomic 
  */
  if(units == KM)
    distance = (rotations*WHEEL_CIRCUM_KM);            //distance currently in milimeters
  else distance = (rotations*WHEEL_CIRCUM_MI);
  distance = distance/1000;
  
  Fixed_ulDecOut2(distance);
  
  LCD_OutChar(0x6B);
  LCD_OutChar(0x6D);
  /*
  asm ldaa SaveCCR
  asm tap          // end critical section
  asm cli
  */
}

//***************Get_Countdown******************
//Displays the time you have been riding in hours, minutes, seconds, miliseconds
//Input: the hour, minute, second and milisecond to display on the LCD
//Output: none
void Get_Countdown(short hour, short minute, short second, short msecond){
  /*
  unsigned char SaveCCR;
  asm tpa  
  asm staa SaveCCR 
  asm sei          // make atomic 
  */
   
  LCD_OutChar(NUM_TO_CHAR+(hour/10)); 
  LCD_OutChar(NUM_TO_CHAR+(hour%10));
  LCD_OutChar(0x3A);                              // ASCII Colon :                          
  LCD_OutChar(NUM_TO_CHAR+(minute/10)); 
  LCD_OutChar(NUM_TO_CHAR+(minute%10));
  LCD_OutChar(0x3A);                              // ASCII Colon :
  LCD_OutChar(NUM_TO_CHAR+(second/10)); 
  LCD_OutChar(NUM_TO_CHAR+(second%10));
  //LCD_OutChar(0x3A);                              // ASCII Colon :
  //LCD_OutChar(NUM_TO_CHAR+(msecond/10)); 
  //LCD_OutChar(NUM_TO_CHAR+(msecond%10));
  /*
  asm ldaa SaveCCR
  asm tap          // end critical section
  asm cli
  */
}

//***************Get_Speed******************
//Displays the speed at which you are travelling
//Input: the total rotation count and the total rotation count 1 second ago
//Output: none
void Get_Speed(unsigned short *buffer, unsigned char units, unsigned long time){
  char i;
  unsigned long speed = 0;
  unsigned long rotations = 0;
  
  unsigned char SaveCCR;
  asm tpa  
  asm staa SaveCCR 
  asm sei          // make atomic
  
 // The for loop has the 10 
  for(i=0; i<10; i++){
    rotations = rotations + *buffer;
    buffer++;   
  }
  rotations = rotations*60;
  if(time < 60){
    rotations = (rotations*60)/time;
  }

  //rotations currently carries the total number of rotations done in one hour
  if(units == KM)
    speed = ((rotations*WHEEL_CIRCUM_KM));           //speed currently in mm/hr
  else speed = ((rotations*WHEEL_CIRCUM_MI));
  speed = (speed)/1000;                              //speed now in meters/hour
  
  Fixed_ulDecOut2(speed);
  
  LCD_OutString(" km/hr");
  
  asm ldaa SaveCCR
  asm tap          // end critical section
  asm cli                                      
}

//***************Get_AvgSpeed******************
//Displays the average speed over the whole trip
//Input: the total time spent on the trip (in seconds), and the total rotations done over the whole trip
//Output: none
void Get_AvgSpeed(unsigned long time, unsigned long rotations, unsigned char units){
  unsigned long distTraveled;
  
  unsigned char SaveCCR;
  asm tpa  
  asm staa SaveCCR 
  asm sei          // make atomic
  
  if(units == KM)
    distTraveled = rotations*WHEEL_CIRCUM_KM;         //distanceTraveled currently in mm
  else distTraveled = rotations*WHEEL_CIRCUM_MI;         //distanceTraveled currently in miles/1E6
   
  distTraveled = distTraveled/1000;                 //distanceTravleed now in meters
  distTraveled = ((distTraveled*3600)/(time));      //distanceTravled now in meters/hr

if(distTraveled<9999){
    LCD_OutChar(NUM_TO_CHAR+(distTraveled/1000));
    LCD_OutString(".");
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10)%10));
  } 
  else if(distTraveled<99999){
    LCD_OutChar(NUM_TO_CHAR+(distTraveled/10000));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled)%10));
    LCD_OutString(".");
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10)%10));
  }
  else if(distTraveled<999999){
    LCD_OutChar(NUM_TO_CHAR+(distTraveled/100000));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/1000)%10));
    LCD_OutString(".");
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10)%10));
  }                                               
  else if(distTraveled<9999999){
    LCD_OutChar(NUM_TO_CHAR+(distTraveled/1000000));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100000)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/1000)%10));
    LCD_OutString(".");
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10)%10));
  }
  else if(distTraveled<99999999){
    LCD_OutChar(NUM_TO_CHAR+(distTraveled/10000000));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/1000000)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100000)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/1000)%10));
    LCD_OutString(".");
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((distTraveled/10)%10));
    
  }
  LCD_OutString(" km/hr");
    
  asm ldaa SaveCCR
  asm tap          // end critical section
  asm cli  
}

//***************Get_Cadence******************
//Displays the time you have been riding in hours, minutes, seconds, miliseconds
//Input: the hour, minute, second and milisecond to display on the LCD
//Output: none
void Get_Cadence(unsigned short *buffer, unsigned long time){
  char i;
  unsigned short rotations = 0;
  
  unsigned char SaveCCR;
  asm tpa  
  asm staa SaveCCR 
  asm sei          // make atomic 

  // The for loop has the 10 
  for(i=0; i<12; i++){
    rotations = rotations + *buffer;
    buffer++;   
  }
  //rotations now has the number of rotations that have been made
  if(time < 60){
    rotations = ((600/time)*rotations)/10;
  }
  LCD_OutChar(NUM_TO_CHAR + (rotations/100));
  LCD_OutChar(NUM_TO_CHAR + ((rotations/10)%10));
  LCD_OutChar(NUM_TO_CHAR + (rotations%10));
  
  asm ldaa SaveCCR
  asm tap          // end critical section
  asm cli
}

//***************Get_Weight******************
//Displays the users weight in lbs
//Input: the weight that the user is inputting
//Output: none
void Get_Weight(unsigned short weight){
  unsigned char SaveCCR;
  asm tpa  
  asm staa SaveCCR 
  asm sei          // make atomic 
  
  LCD_GoTo(0,0);
  LCD_OutString("    lbs");
  LCD_GoTo(0,0);
  LCD_OutChar(NUM_TO_CHAR + (weight/100));
  LCD_OutChar(NUM_TO_CHAR + ((weight/10)%10));
  LCD_OutChar(NUM_TO_CHAR + (weight%10));
  
  asm ldaa SaveCCR
  asm tap          // end critical section
  asm cli
}

//2 crank rotations equal .1 Cals burned
//20 crank rotations equal 1 calorie burned
void Get_Cals(unsigned long crankRotations){
  Fixed_ulDecOut1(crankRotations/2);
  LCD_OutString(" Cals");    
}


