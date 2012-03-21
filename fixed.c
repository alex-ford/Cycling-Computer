// filename  ***************  fixed.c ******************************
// Alex Ford and Andy Pande 09/08/10
// Converts an unsigned or signed deicmal or binary number into the
// fixed point representation with a given resolution
// Lab #01
// TA: Rafael
// Last revised on 9/10/10

#include "fixed.h"
#include "lcd.h"

//------------------Fixed_ulDecOut2-----------------
//Handles a unsigned 32 bit decimal number from 0 to 2^32
// and converts it to a unsigned 32 bit decimal fixed point 
// with a .01 resolution.
//Input: Unsigned 32 bit decimal
//Output: none
void Fixed_ulDecOut2(unsigned long fixedOut){
  if(fixedOut<999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/100));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut)%10));
  }
  else if(fixedOut<9999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/1000));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
  } 
  else if(fixedOut<99999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/10000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
  }
  else if(fixedOut<999999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/100000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
  }                                               
  else if(fixedOut<9999999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/1000000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
  }
  else if(fixedOut<99999999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/10000000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
  }
}

//------------------Fixed_ulDecOut1-----------------
//Handles a unsigned 32 bit decimal number from 0 to 2^32
// and converts it to a unsigned 32 bit decimal fixed point 
// with a .1 resolution.
//Input: Unsigned 32 bit decimal
//Output: none
void Fixed_ulDecOut1(unsigned long fixedOut){
  if(fixedOut<999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/100));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut)%10));
  } 
  else if(fixedOut<9999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/1000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut)%10));
  }
  else if(fixedOut<99999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/10000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut)%10));
  }                                               
  else if(fixedOut<999999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/100000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1)%10));
  }
  else if(fixedOut<9999999){
    LCD_OutChar(NUM_TO_CHAR+(fixedOut/1000000));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1000)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/100)%10));
    LCD_OutChar(0x2E);                              // ASCII Period .
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/10)%10));
    LCD_OutChar(NUM_TO_CHAR+((fixedOut/1)%10));
  }
}