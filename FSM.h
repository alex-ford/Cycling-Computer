//*******************FSM.h***************
//Alex Ford and Andy Pande 10/15/10
//Lab 09: Software for Cycling Computer
//TA: Raffaele Cetrul
//Date fo last Revision: 11/05/10

#define STOPWATCH         0x01
#define COUNTDOWN         0x02
#define DISTANCE          0x04
#define CADENCE           0x08
#define SPEED             0x10
#define AVGSPEED          0x20
#define CALBURN           0x40
#define WEIGHT            0x80
#define SETCOUNTDOWN      0x03
#define UNITS             0x05
#define CYCLE_NEXT_STATE  1
#define CYCLE_PREV_STATE  0
#define PLAY              0x10
#define PAUSE             0xA0

//**************Get_State****************
//Input: none
//Output: the current state you are in is returned, see #defines in FSM.h
char Get_State(void);

//**************FSM_Init******************
//Initializes the FSM
//Input: none
//Output: none
void FSM_Init(void);

//**************Get_String******************
//Looks at the current state, and returns a pointer to the string output of that state
//Input: none
//Output: pointer to the first character of the string
char* Get_String(void);

//**************FSM_NextState******************
//Sets to FSM pointer to the next state based on the index
//Input: number of the next state
//Output: none
void FSM_NextState(short index);