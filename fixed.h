// filename  ***************  fixed.h ******************************
// Alex Ford and Andy Pande 09/08/10
// Converts an unsigned or signed deicmal or binary number into the
// fixed point representation with a given resolution
// Lab #01
// TA: Rafael
// Last revised on 11/26/10

#define NUM_TO_CHAR 0x30

//------------------Fixed_ulDecOut2-----------------
//Handles a unsigned 32 bit decimal number from 0 to 2^32
// and converts it to a unsigned 32 bit decimal fixed point 
// with a .01 resolution.
//Input: Unsigned 32 bit decimal
//Output: none
void Fixed_ulDecOut2(unsigned long fixedOut);