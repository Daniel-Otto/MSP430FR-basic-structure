#ifndef MAIN_H
#define MAIN_H



#include "io430.h"
#include "CS.h"
#include "Timer0_A3.h"
#include "GPIO.h"
#include "EUSCI_A0_UART.h"



/**
 *@brief states of the interrupt evaluation state machine
 */
typedef enum{ UART_A0_RX=0,     /**< received data @ UART_A0 */
              UART_A0_TX,       /**< transmitting data @ UART_A0 */
              UART_A1_RX,       /**< received data @ UART_A1 */
              UART_A1_TX,       /**< transmitting data @ UART_A1 */
              NONE              /**< no interrupt is pending */
} irqTask_t;

/**
 *@brief states of the main state machine
 */
typedef enum{ INIT = 0, /**< for initialisation the MCU and the peripherals */
              SLEEP,    /**< the sleep mode where the MCU rest */
              IRQ_EVAL, /**< evaluate the interrupt */
              RST_WD    /**< kick the watchdog */
} mainTask_t;



/**
 *@brief state machine wich elvaluate the main task
 *@param none
 */
void mainStateMachine ( void );

/**
 *@brief initialize the MCU
 *@param none
 */
void initialization ( void );

/**
 *@brief optical recognition boot success
 *@param none
 */
void opticalBootSuccess( void );

/**
 *@brief state machine wich elvaluate the irq task
 *@param none
 */
void irqStateMachine ( void );

#endif //MAIN_H