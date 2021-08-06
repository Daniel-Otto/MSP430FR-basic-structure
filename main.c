/*******************************************************************************
 *@author       Daniel Otto
 *@date         06.08.2021
 *@brief        A fundamental code from where to build on. Which includes:
 *                      - a State Machine
 *                      - the Watchdog
 *                      - a Bootloader over UART (OTA / SPI)
 *
 ******************************************************************************/
#include "main.h"
   
mainTask_t mainTask = INIT;     // start the state machine with the initialization

int main( void ){
  
  WDTCTL = WDTPW | WDTHOLD;     // Stop watchdog timer

  while(1){
    mainStateMachine();         // run the state main state machine
  } //while 
} //main 



void mainStateMachine(){
  
  switch( mainTask ){
    
  case INIT:
    initialization();
    mainTask = SLEEP;
    break;
    
  case SLEEP:
    TA0CCTL0 = CCIE;                    // TACCR0 interrupt enabled
    __bis_SR_register(LPM3_bits);       // Enter LPM3
    TA0CCTL0 &= ~CCIE;                  // TACCR0 interrupt disabled
    WDTCTL = WDT_ARST_1000;             // Reset WDT
    asm("NOP");                         // For debugger
    break;
    
  case APP:
    // __bic_SR_register(LPM3_bits); // exits LPM3
    // do something
    mainTask = RST_WDT;
    break;
    
  case RST_WDT:
    WDTCTL = WDT_ARST_1000;             // Reset WDT
    if (1){
      mainTask = SLEEP;                 // switch state of the main s.m.
    }else{
      mainTask = APP;                   // switch state of the main s.m.
    } //if-else
    
    break;
    
  default:
    break;
  } //switch
} //fkt



void initialization (){
  
  ClockSystem_Init();           // Clock System Setup
  Timer0_A3_Init();             // timer for resetting the WDT
  GPIO_Init();                  // Configure GPIO
  
  bootSuccess();                // Boot succeed sequence
  
  WDTCTL = WDT_ARST_1000;       // Start WDT
  
  __bis_SR_register(GIE);       // enable general interrupt
  
} //fkt



void ClockSystem_Init (){
  CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
  CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
  // Set SMCLK = MCLK = DCO     ACLK = VLOCLK
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
  CSCTL0_H = 0;
} //fkt



void Timer0_A3_Init(){
  TA0CCTL0 = CCIE;                      // TACCR0 interrupt enabled
  TA0CCR0 = 9500;
  TA0CTL = TASSEL__ACLK | MC__UP;       // ACLK, UP mode
} //fkt



void GPIO_Init (){
  P1DIR |= BIT0;        // Set P1.0 to output direction
  P9DIR |= BIT7;        // Set P2.9 to output direction
  
  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;
} //fkt



void bootSuccess (){
  P9OUT |= BIT7;
  
  for(int i = 0; i < 5; i++){
    P1OUT |= BIT0;
    __delay_cycles(800000);
    P1OUT &= ~BIT0;
    __delay_cycles(200000);
  } //for
  
  P9OUT &= ~BIT7;
} //fkt



#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
  P1OUT ^= BIT0;                // toggle LED @ P1.0
  WDTCTL = WDT_ARST_1000;       // Reset WDT
} //isr