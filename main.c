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
    mainStateMachine();         // run the main state machine
  } //while 
} //main 



void mainStateMachine(){
  
  switch( mainTask ){
    
  case INIT:
    initialization();
    mainTask = SLEEP;
    break;
    
  case SLEEP:
    TA0CTL |= MC__UP;                   // Timer0_A3 Up Mode
    asm("NOP");                         // For debugging
    __bis_SR_register(LPM3_bits);       // Enter LPM3
    TA0CTL |= MC__STOP;                 // Timer0_A3 Stop
    WDTCTL = WDT_ARST_1000;             // Reset WDT
    asm("NOP");                         // For debugging
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
  
  opticalBootSuccess();         // Boot succeed sequence
  
  WDTCTL = WDT_ARST_1000;       // Start WDT
  
  __bis_SR_register(GIE);       // enable general interrupt
  
} //fkt



void opticalBootSuccess (){
  P9OUT |= BIT7;
  
  for(int i = 0; i < 5; i++){
    P1OUT |= BIT0;
    __delay_cycles(800000);
    P1OUT &= ~BIT0;
    __delay_cycles(200000);
  } //for
  
  P9OUT &= ~BIT7;
} //fkt