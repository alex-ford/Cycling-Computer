//filename ********* Main.C **************
//Alex Ford and Andy Pande 10/15/10
//Lab 09: Software for Cycling Computer
//TA: Raffaele Cetrul
//Date fo last Revision: 11/05/10

//Description: The Cycling computer will display 
// on an LCD screen certain statistical information about the cyclists performance.
// These statistics and features include:
// a STOPWATCH
// a COUNTDOWN Timer
// DISTANCE Traveled
// CADENCE of teh rider
// SPEED in both km/h and mph
// AVGSPEED traveled
// Calories burned by the rider
// Weight of the rider
//
// These stats will be displayed using a Finite State Machine
// A user can change the information on the LCD using three swtiches that 
// the user can interact with.
//
// The Reed switches will be connected to the bicycle. PP1 will be on the back wheel
// and PP2 will be connected the the crank (pedals) of the bicycle.




// filename ******** Main.C ************** 
// LCD Display (HD44780) on Port H for the 9S12DP512   
// Jonathan W. Valvano 9/18/09
// TCNT runs at 667ns,

//  This example accompanies the books
//   "Embedded Microcomputer Systems: Real Time Interfacing",
//        Thompson, copyright (c) 2006,
//   "Introduction to Embedded Systems: Interfacing to the Freescale 9S12",
//        Cengage Publishing 2009, ISBN-10: 049541137X | ISBN-13: 9780495411376

// Copyright 2009 by Jonathan W. Valvano, valvano@mail.utexas.edu 
//    You may use, edit, run or distribute this file 
//    as long as the above copyright notice remains 
// Purpose: test program for 4-bit LCD.C driver


/*   
  size is 1*16 
  if do not need to read busy, then you can tie R/W=ground 
  ground = pin 1    Vss
  power  = pin 2    Vdd   +5V
  ground = pin 3    Vlc   grounded for highest contrast
  PH4    = pin 4    RS    (1 for data, 0 for control/status)
  PH5    = pin 5    R/W   (1 for read, 0 for write)
  PH6    = pin 6    E     (enable)
  PH3    = pin 14   DB7   (4-bit data)
  PH2    = pin 13   DB6
  PH1    = pin 12   DB5
  PH0    = pin 11   DB4
16 characters are configured as 2 rows of 8
addr  00 01 02 03 04 05 06 07 40 41 42 43 44 45 46 47

Push Buttons (switches):
  PP0 - LED 
  PP1 - Reed Switch Wheel 
  PP2 - Reed Switch Crank  
  PP3 - Enter/Reset
  PP4 - Input Data
  PP5 - Cycle Data
*/

#include <hidef.h>      /* common defines and macros */
#include <mc9s12dp512.h>     /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12dp512"

#include "LCD.H"
#include "PLL.H"
#include "Timer.h"
#include "stats.h"
#include "FSM.h"

#define  PTP_LED          0x01
#define  PTP_WHEEL        0x02
#define  PTP_CRANK        0x04
#define  PTP_ENTER        0x08
#define  PTP_INPUT_DATA   0x10
#define  PTP_CYCLE_DATA   0x20
#define  SPEED_BUFF_SIZE  60
#define  CRANK_BUFF_SIZE  12
#define  NUM_TO_CHAR      0x30
#define  DEFAULT_WEIGHT   160
#define  UP               1
#define  DOWN             0



unsigned short static volatile Count0;          //interrput counter
unsigned short static volatile NoMovement = 0;
unsigned char static volatile CurrentState;
unsigned char static volatile NewState;
unsigned char static volatile KeyPressed;
unsigned char static volatile  CountDownSet;
unsigned char static volatile  StopwatchSet;
unsigned char static PreferedUnits;

unsigned short static volatile SpeedRotCount;
unsigned short static volatile CrankRotCount;
unsigned short static volatile Weight;
unsigned short static volatile UpdateData;      
unsigned short static volatile Hours;
unsigned short static volatile Minutes;
unsigned short static volatile Seconds;
unsigned short static volatile mSeconds;
unsigned short static volatile CountHours;
unsigned short static volatile CountMinutes;
unsigned short static volatile CountSeconds;
unsigned short static volatile CountmSeconds;

unsigned long static volatile WheelRotCount;
unsigned long static volatile TotalTime;
unsigned long static volatile CadenceTime;
unsigned long static volatile SpeedTime;
unsigned short static SpeedBuffer[SPEED_BUFF_SIZE] = {0};
unsigned short static SpeedBufferTest[SPEED_BUFF_SIZE] = {0,1,2,2,2,2,2,2,2,2};
unsigned short static CrankBuffer[CRANK_BUFF_SIZE] = {0};
unsigned short static volatile SpeedBufferIndex;
unsigned short static volatile CrankBufferIndex;

unsigned short static volatile mSeconds;

void resetStopwatch(void){
  Hours = 0;
  Minutes = 0;
  Seconds = 0;
  mSeconds = 0;
}

void resetCountdown(void){
  CountHours = 0;
  CountMinutes = 0;
  CountSeconds = 0;
  CountmSeconds = 0;
  CountDownSet = 0;
}

void resetDistance(void){
  WheelRotCount = 0;
}

void resetTotalTime(void){
  TotalTime = 0;  
}

void setWeight(char direction){
  if(direction == DOWN){
    Weight = Weight - 5;
    if(Weight == 0)
      Weight = DEFAULT_WEIGHT;
  } 
  else{
    Weight = Weight + 5;
    if(Weight == 500)
      Weight = DEFAULT_WEIGHT;  
  }     
}


void setCountdown(void){
  //Increment the minutes to countdown by 5 minutes
  if((CountHours<1)&&(CountMinutes<30)){
    CountMinutes = CountMinutes + 5;  
  } 
  //Increment the minutes to countdown by 5 minutes
  else if((CountHours<1)&&(CountMinutes<60)){
    CountMinutes = CountMinutes + 10;
    if(CountMinutes==60){
      CountHours++;
      CountMinutes = 0;  
    }
  } 
  else if((CountHours<2)&&(CountMinutes<30)){
    CountMinutes = CountMinutes + 15;  
  }
   
  //Increment the minutes to countdown by 30 minutes 
  else{
    CountMinutes = CountMinutes + 30;
    if(CountMinutes == 60){
      CountHours++;
      //If the Countdown Timer is at 11 hours then reset it to 0
      if(CountHours == 11){
        CountHours=0;
      }
      CountMinutes = 0;  
    }
  }
}

interrupt 8 void TOC0handler(void){         // executes at 12 Hz 
  TFLG1 = 0x01;                             // acknowledge OC0
  //Update Time
  UpdateData = 1;
  Count0++;
  if(Count0==60001)
    Count0=0;
  if((Count0%100)==0){
    CadenceTime++;
    SpeedTime++;
    TotalTime++;
  }
  if((Count0%25)==0)
    PTP ^= 0x80;
  
  //Updating Stopwatch Time
  if(StopwatchSet == 1){
    mSeconds++;
    if(mSeconds == 100){
      mSeconds = 0;
      Seconds++;
      if(Seconds == 60){
        Seconds = 0;
        Minutes++;
        if(Minutes == 60){
          Minutes = 0;
          Hours++;
        }
      }
    }
  }
  
  //Updating Countdown Time and Countdown LED
  if(CountDownSet == 1){
    if(!((CountmSeconds==0)&&(CountSeconds==0)&&(CountMinutes==0)&&(CountHours==0))){
      CountmSeconds--;
      if(CountmSeconds == 65535){
        CountmSeconds = 99;
        CountSeconds--;
        if(CountSeconds == 65535){
          CountSeconds = 59;
          CountMinutes--;
          if(CountMinutes == 65535){
            CountMinutes = 59;
            CountHours--;
          }
        }
      }
    }
    else{
      //The Countdown Timer is done, so flash the LED four times per second
      if(Count0%25==0)
        PTP ^= PTP_LED;  
    }
  }
  
  //Updating Cadence: Each element of array CrankBuffer will contain
  //how many rotations are done in 5 seonds
  //The size of the array should be 12 so it adds up to a minute
  if(Count0%500==0){
    CrankBuffer[CrankBufferIndex] = CrankRotCount;
    CrankRotCount=0;
    CrankBufferIndex++;
    if(CrankBufferIndex == CRANK_BUFF_SIZE){
      CrankBufferIndex = 0;  
    }
  }
  //Updating Speed: Each element of the array SpeedBuffer will contain
  // how many rotations are done in 1 second
  if(Count0%100==0){
    SpeedBuffer[SpeedBufferIndex] = SpeedRotCount;
    if(SpeedRotCount == 0)
      NoMovement++;
    else{
      NoMovement = 0;
      SpeedRotCount=0;
    }
    SpeedBufferIndex++;
    if(SpeedBufferIndex == SPEED_BUFF_SIZE){
      SpeedBufferIndex = 0;    
    }
    if(NoMovement == 3){
      char i;
      SpeedTime = 0;
      SpeedBufferIndex = 0;
      for(i=0; i<SPEED_BUFF_SIZE; i++){
        SpeedBuffer[i] = 0;
      }
    }
  }
  
  TC0 = TC0+(15000);                          // interrupts 100 times per second
}

interrupt 56 void Keyhandler(void){
  KeyPressed = PTP;
  //wait for bounce to stop
  mwait(10);
  
  //Distance Measurement
  if((KeyPressed&PTP_WHEEL)==PTP_WHEEL){      //if the wheel does one full rotation
    while((PTP&=PTP_WHEEL) == PTP_WHEEL){
      mwait(10);
    }
    //UPdate data for Distance Calculations
    WheelRotCount++;
    //Update Data for Speed Calculations 
    SpeedRotCount++;
    PIFP = 0x3E;                         //clear flag
  }
  //Crank Measurement 
  else if((KeyPressed&PTP_CRANK)==PTP_CRANK){      //if the Crank does one full rotation 
    //wait for bounce to stop 
    while((PTP&=PTP_CRANK) == PTP_CRANK){
      mwait(10);
    }
    CrankRotCount++;
    PIFP = 0x3E;                         //clear flag 
  }
   
  else if((KeyPressed&PTP_ENTER)==0){
    //wait for bounce to stop
    while((PTP&=PTP_ENTER) == 0){
      mwait(10);
    }
     
    if(CurrentState == STOPWATCH){
      if(StopwatchSet == 1){
        StopwatchSet = 0;
      } 
      else StopwatchSet = 1;
    } 
    else if(CurrentState == SETCOUNTDOWN){
      setCountdown();  
    }
    else if(CurrentState == COUNTDOWN){
      if(CountDownSet == 1){
        CountDownSet = 0;
      } 
      else CountDownSet = 1;
    }
    else if(CurrentState == WEIGHT){
      setWeight(DOWN);  
    } 
    else if(CurrentState == UNITS){
      PreferedUnits = MI;
      FSM_NextState(CYCLE_NEXT_STATE);
    }
//  else if(CurrentState == COUNTDOWN){;}
//  else if(CurrentState == COUNTDOWN){;}

    PIFP = 0x3E;                         //clear flag    
  }
   
  else if((KeyPressed&PTP_INPUT_DATA)==0){
    //wait for bounce to stop
    while((PTP&PTP_INPUT_DATA) == 0){
      mwait(10);
    }
    if(CurrentState == SETCOUNTDOWN){
      resetCountdown();  
    } 
    else if(CurrentState == STOPWATCH){
      resetStopwatch();  
    } 
    else if(CurrentState == COUNTDOWN){
      FSM_NextState(CYCLE_PREV_STATE);
      resetCountdown();
    }
    else if(CurrentState == DISTANCE){
      resetDistance();
      resetTotalTime();
    } 
    else if(CurrentState == WEIGHT){
      setWeight(UP);  
    }
    else if(CurrentState == UNITS){
      PreferedUnits = KM;
      FSM_NextState(CYCLE_NEXT_STATE);
    }
    
    PIFP = 0x3E;
  } 
  else if((KeyPressed&PTP_CYCLE_DATA)==0){
    //wait for bounce to stop
    while((PTP&PTP_CYCLE_DATA) == 0){
      mwait(10);
    }
    if(CurrentState == UNITS){
      PreferedUnits = KM;
    } 
    else if((CurrentState == STOPWATCH)&&(CountDownSet == 0))
      FSM_NextState(CYCLE_PREV_STATE);
    else if((CurrentState == SETCOUNTDOWN)&&(CountDownSet == 0)){
      if((CountHours + CountMinutes) > 0){
        CountDownSet = 1;
        FSM_NextState(CYCLE_NEXT_STATE);  
      } 
      else
        FSM_NextState(CYCLE_PREV_STATE);
    }
    else
      FSM_NextState(CYCLE_NEXT_STATE);
    
    
    PIFP = 0x3E;
  }
  
  PIFP = 0x3E;
}

//---------------------OC_Init0---------------------
// arm output compare 0 for 100Hz periodic interrupt
// Input: none
// Output: none 
void OC_Init0(){
  DDRP |= 0x81;       //PP7 and PP0 are outputs
  UpdateData = 0;
  WheelRotCount = 10000;
  CountDownSet = 0;
  StopwatchSet = 0;
  Count0 = 0;
  TotalTime = 0;
  CadenceTime = 0;
  SpeedTime = 0;
  CrankRotCount = 0;
  SpeedRotCount = 0;
  //CurrentState = 0;
  SpeedBufferIndex = 0;
  CrankBufferIndex = 0;
  CadenceTime = 0;
  Weight = DEFAULT_WEIGHT;
  //SpeedBuffer = 0;   
  TIOS |= 0x01;   // activate TC0 as output compare
  TIE  |= 0x01;   // arm OC0
  TC0  = TCNT+50; // first interrupt right away
}

void Key_Init(void){
 DDRP &= ~0x3E;     // PP5-1 is an input switch (Wakeup Key)
 
 PERP &= ~0x38;     // no pull down on PP5-3
 PERP |= 0x06;     // pull down on PP3-1
 PPSP &= ~0x38;      // falling edge active
 PPSP |= 0x06;      // risign edge active
 
 PIEP |= 0x3E;      // arm PP5-1
 PIFP = 0x3E;       // clear flags
}

void IC_Init0(void){
  TIOS &= ~0x3E;
  DDRP &= ~0x3E;     // PP5-1 is an input switch (Wakeup Key)
  TCTL3 = 0x05;
  TCTL4 = 0x54;
  TFLG1 = 0x3E;
  TIE |= 0x3E;
}


void main(void) {
  PLL_Init();       // set E clock to 24 MHz
  Timer_Init();      // enable Port T
  FSM_Init();
  LCD_Open();
  LCD_Clear();
  Key_Init();
  //IC_Init0();
  OC_Init0();       // initialize interrupt0 and LED Flash (Heartbeat PP7)
  //PreferedUnits = KM;
  asm cli           // allows debugger to run
  CurrentState = Get_State();
  LCD_GoTo(0,0);
  LCD_OutString("Prefered Units");
  LCD_GoTo(1,0);
  LCD_OutString(Get_String());
  
  for(;;) {
    //Check if we are in a new state
    //If state is new, then establish CurrentState variable
    if(CurrentState != (NewState=Get_State())){
      CurrentState = NewState;
      LCD_GoTo(1,0);
      LCD_OutString(Get_String());
      if(CurrentState != UNITS){
        LCD_GoTo(0,0);
        blank();
        LCD_GoTo(0,0);
      }
    }

    //Assume we are in the state that displays weight
    if((UpdateData == 1) && (CurrentState == WEIGHT) && (Count0%25==0)){
      Get_Weight(Weight); 
      UpdateData = 0;
    }
    else if((UpdateData == 1) && (CurrentState == STOPWATCH)){
      LCD_GoTo(0,0);
      Get_Stopwatch(Hours, Minutes, Seconds, mSeconds);
      UpdateData=0; 
    }
    //Assume we are in the state that dispalys distance
    else if((UpdateData == 1) && (CurrentState == DISTANCE) && (Count0%25==0)){     //if the wheel has made a rotation
      LCD_GoTo(0,0);
      blank();
      LCD_GoTo(0,0);
      Get_Distance(WheelRotCount, PreferedUnits);
      LCD_GoTo(0,13);
      LCD_OutChar(NUM_TO_CHAR + WheelRotCount);
      UpdateData = 0;
    }
    //Check if we are in the Countdown state
    else if((UpdateData == 1) && ((CurrentState == COUNTDOWN)||(CurrentState==SETCOUNTDOWN))){
      LCD_GoTo(0,0);
      Get_Countdown(CountHours,CountMinutes,CountSeconds,CountmSeconds);
      UpdateData = 0;
    }
    //Check if we are in the Speed state
    else if((UpdateData == 1) && (CurrentState == SPEED) && ((Count0%100)==0)){
      LCD_GoTo(0,0);
      blank();
      LCD_GoTo(0,0);
      Get_Speed(SpeedBuffer, PreferedUnits, SpeedTime);
      UpdateData = 0;
    }
    //Check if we are in the AVGSpeed state
    else if((UpdateData == 1) && (CurrentState == AVGSPEED) && ((Count0%100)==0)){
      LCD_GoTo(0,0);
      blank();
      LCD_GoTo(0,0);
      Get_AvgSpeed(TotalTime, WheelRotCount, PreferedUnits);
      UpdateData = 0;  
    }
    //Check if we are in the Cadence state
    else if((UpdateData == 1) && (CurrentState == CADENCE) && ((Count0%100)==0)){
      LCD_GoTo(0,0);
      blank();
      LCD_GoTo(0,0);
      Get_Cadence(CrankBuffer, CadenceTime);
      LCD_GoTo(0,4);
      LCD_OutString(" RPM");
      UpdateData = 0;  
    }
    //Check if we are in the CaloriesBurned state
    else if((UpdateData == 1) && (CurrentState == CALBURN) && ((Count0%100)==0)){
      LCD_GoTo(0,0);
      blank();
      LCD_GoTo(0,0);
      Get_Cals(CrankRotCount);
      LCD_GoTo(0,8);
      LCD_OutString("calories");
      UpdateData = 0;  
    }
  }
}

void main2(void){
  PLL_Init();       // set E clock to 24 MHz
  Timer_Init();      // enable Port T
  DDRP |= 0x80;
  DDRP &= ~0x20;
  for(;;){
    if(PTP&0x20){
      PTP |= 0x80;  
    } 
    else PTP &= ~0x80;
  }    
}