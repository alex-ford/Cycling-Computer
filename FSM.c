//*******************FSM.c***************
//Alex Ford and Andy Pande 10/15/10
//Lab 09: Software for Cycling Computer
//TA: Raffaele Cetrul
//Date fo last Revision: 11/05/10

#include <mc9s12dp512.h>     /* derivative information */
#include "FSM.h"
#include "lcd.h"

//These strings will dispaly what is being read by the LCD
char watchString[17] = {
  PLAY,'/',PAUSE,'s','w',' ',' ','R','E','S','E','T',' ',' ','-','>','\0'
};
//Play/Pause Symbol 0x10,'/',0xA0,' '
char countString[17] = {
  PLAY,'/',PAUSE,'c','n','t',' ','R','E','S','E','T',' ',' ','-','>','\0'
};

char setCountString[17] = {
  ' ','U','P',' ',' ','R','E','S','E','T',' ',' ',' ','S','E','T','\0'
};

char calString[17] = {
  ' ','c','a','l','o','r','i','e','s',' ','b','u','r','n','e','d','\0'
};

char speedString[17] = {
  'S','P','E','E','D',' ',' ',' ',' ',' ',' ',' ',' ',' ','-','>','\0'
};

char avgspeedString[17] = {
  'A','V','G',' ','S','P','E','E','D',' ',' ',' ',' ',' ','-','>','\0'
};

char distString[17] = {
  ' ',' ',' ',' ',' ','R','E','S','E','T',' ',' ',' ',' ','-','>','\0'
};

char cadString[17] = {
  'c','a','d','e','n','c','e',' ',' ',' ',' ',' ',' ',' ','-','>','\0'
};

char String[17] = {
  's','t','r','i','n','g',' ',' ',' ',' ',' ',' ',' ',' ','-','>','\0'
};
char unitsString[17] = {
  'M','I','L','E','S',' ',' ','K','M','E','T','E','R','S',' ',' ','\0'
};

char weightString[17] = {
  'D','O','W','N',' ',' ',' ','U','P',' ',' ','E','N','T','E','R','\0'
};                          

const struct State{
  unsigned char Out;            // Output that says the state we are in
  char *OutString;              // Points to String that outputs to LCD
  const struct State *Next[2];  // Next state based on switches press PP5-3
};

typedef const struct State StateType;
typedef StateType *	StatePtr;

#define S0 &fsm[0]
#define S1 &fsm[1]
#define S2 &fsm[2]
#define S3 &fsm[3]
#define S4 &fsm[4]
#define S5 &fsm[5]
#define S6 &fsm[6]
#define S7 &fsm[7]
#define S8 &fsm[8]
#define S9 &fsm[9]

StateType fsm[10]={
	{STOPWATCH,&watchString[0],          {S8,S1}},  
	{COUNTDOWN,&countString[0],          {S8,S2}},        
	{DISTANCE,&distString[0],            {S2,S3}},   
	{CADENCE,&cadString[0],              {S3,S4}},      
	{SPEED,&speedString[0],              {S4,S5}},  
	{AVGSPEED,&avgspeedString[0],        {S5,S6}},  
	{CALBURN,&calString[0],              {S6,S0}},
	{WEIGHT,&weightString[0],            {S7,S0}},
	{SETCOUNTDOWN,&setCountString[0],    {S2,S1}}, 
	{UNITS,&unitsString[0],              {S7,S7}}
};

StatePtr PtFsm;  // Current State

//**************Get_State****************
//Input: none
//Output: the current state you are in is returned, see #defines in FSM.h
char Get_State(void){
  return(PtFsm->Out);  
}

//**************Get_String******************
//Looks at the current state, and returns a pointer to the string output of that state
//Input: none
//Output: pointer to the first character of the string
char* Get_String(void){
  return(PtFsm->OutString);  
}

//**************FSM_Init******************
//Initializes the FSM
//Input: none
//Output: none
void FSM_Init(void){
  PtFsm = S9;
}

//**************FSM_NextState******************
//Sets to FSM pointer to the next state based on the index
//Input: number of the next state
//Output: none
void FSM_NextState(short index){
  PtFsm = PtFsm->Next[index];  
}
