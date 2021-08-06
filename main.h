#ifndef main_h
#define main_h



#include "io430.h"



/**
 *@brief states of the state machine
 */
typedef enum{ INIT = 0, /**< for initialisation the MCU and the peripherals */
              SLEEP,    /**< the sleep mode where the MCU rest */
              APP,      /**< the main task of this programm*/
              RST_WDT   /**< reset the watchdog timer*/
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
 *@brief initialize the CS module
 *@param none
 */
void ClockSystem_Init ( void );

/**
 *@brief initialize the timer0_A3
 *@param none
 */
void Timer0_A3_Init ( void );

/**
 *@brief initialize the GPIO's
 *@param none
 */
void GPIO_Init ( void );

/**
 *@brief optical recognition boot success
 *@param none
 */
void bootSuccess( void );

#endif