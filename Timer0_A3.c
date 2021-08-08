#include "io430.h"
#include "Timer0_A3.h"



void Timer0_A3_Init(){
  TA0CCTL0 = CCIE;                      // TACCR0 interrupt enabled
  TA0CCR0 = 9500;                       // count to 9500 @ 10 kHz ACLK < 1 s
  TA0CTL = TASSEL__ACLK | MC__UP;       // ACLK, UP mode
} //fkt