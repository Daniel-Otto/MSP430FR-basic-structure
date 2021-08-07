#include "main.h"



void GPIO_Init (){
  P1DIR |= BIT0;        // Set P1.0 to output direction
  P9DIR |= BIT7;        // Set P2.9 to output direction
  
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
} //fkt