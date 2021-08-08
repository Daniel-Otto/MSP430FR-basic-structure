#include "io430.h"
#include "CS.h"



void ClockSystem_Init (){
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
  // Set SMCLK = MCLK = DCO     ACLK = VLOCLK
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
  CSCTL0_H = 0;
} //fkt