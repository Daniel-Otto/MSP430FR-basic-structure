/*******************************************************************************
 * @author      Daniel Otto
 * @date        06.08.2021
 * @version     0.1a
 * @brief       A fundamental code from where to build on. Which includes e.g.:
 *                      - a State Machine
 *                      - the Watchdog
 *                      - communication over UART
 *                      - a Bootloader over UART (OTA / SPI)
 *              Please read the README.md for more information.
 * @copyright   GNU Public License
 ******************************************************************************/

#include "main.h"


#define version 0x1A            // Firmware Version -> 0.1A(lpha)



mainTask_t mainTask = INIT;     // start main state machine with initialization task
irqTask_t irqTask = NONE;       // start irq evalutaion state machine with none task



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
    //mcuMode = LPM;
    asm("NOP");                         // For debugging
    __bis_SR_register(LPM3_bits);       // Enter LPM3
    TA0CTL |= MC__STOP;                 // Timer0_A3 Stop
    /** need to reset the timer counter??? */
    WDTCTL = WDT_ARST_1000;             // Reset WDT
    asm("NOP");                         // For debugging
    break;
    
  case IRQ_EVAL:
    // 
    irqStateMachine();                  // do something
    mainTask = RST_WD;
    break;
    
  case RST_WD:
    WDTCTL = WDT_ARST_1000;             // Reset WDT
    
    if (irqTask == NONE){
      mainTask = SLEEP;                 // switch state of the main s.m.
    }else{
      mainTask = IRQ_EVAL;              // switch state of the main s.m.
    } //if-else
    
    break;
    
  default:
    break;
  } //switch
} //fkt



void irqStateMachine (){
  
  switch( irqTask ){
    
  case UART_A0_RX:
    while(!(UCA0IFG&UCTXIFG));
    UCA0TXBUF = UCA0RXBUF;
    __no_operation();
    irqTask = NONE;
    break;
    
  case UART_A0_TX:
    break;
      
  case UART_A1_RX:
    break;
    
  case UART_A1_TX:
    break;
    
  case NONE:
    break;
    
  default:
    break;
  } //switch  
} //fkt



void initialization (){
  
  ClockSystem_Init();           // Clock System Setup
  UART_A0_Init();
  Timer0_A3_Init();             // timer for resetting the WDT
  GPIO_Init();                  // Configure GPIO
  
  opticalBootSuccess();         // Boot succeed sequence
  
  WDTCTL = WDT_ARST_1000;       // Start WDT
  
  __bis_SR_register(GIE);       // enable general interrupt
  
} //fkt



void opticalBootSuccess (){
  P9OUT |= BIT7;                // turn on LED @ P9.7 (green)
  
  for(int i = 0; i < 5; i++){   // toggle LED @ P1.0 4 times (red)
    P1OUT |= BIT0;
    __delay_cycles(800000);
    P1OUT &= ~BIT0;
    __delay_cycles(200000);
  } //for
  
  P9OUT &= ~BIT7;               // turn off LED @ P9.7 (green)
} //fkt



#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void){
  P1OUT ^= BIT0;                        // toggle LED @ P1.0
  WDTCTL = WDT_ARST_1000;               // Reset WDT
  irqTask = UART_A0_RX;
} //isr



#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void)
{
  switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
  {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
      mainTask = IRQ_EVAL;
      irqTask = UART_A0_RX;
      break;
    case USCI_UART_UCTXIFG: break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
  }
  __bic_SR_register(LPM3_bits); // exits LPM3
}