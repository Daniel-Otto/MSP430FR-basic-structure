#include "main.h"



void Timer0_A3_Init(){
  TA0CCTL0 = CCIE;                      // TACCR0 interrupt enabled
  TA0CCR0 = 9500;                       // count to 9500 @ 10 kHz ACLK < 1 s
  TA0CTL = TASSEL__ACLK | MC__UP;       // ACLK, UP mode
} //fkt



#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
  P1OUT ^= BIT0;                // toggle LED @ P1.0
  WDTCTL = WDT_ARST_1000;       // Reset WDT
} //isr