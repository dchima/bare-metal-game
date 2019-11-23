/*
 * HPS_Timers.h
 *
 * Created on: Feb 26, 2018
 * Author: Azeem Oguntola
 * -------------------------------------------------------------------
 * Description
 * -------------------------------------------------------------------
 * This is a Bare Metal Driver for the ARM Timers.
 * 
 */

#ifndef HPS_TIMERS_H_
#define HPS_TIMERS_H_

#include <stdbool.h>
#define TIMER_SUCCESS       0
#define TIMER_ERRORNOINIT   -1



//Function to initialise the Timers
signed int Timer_initialise(void);

//Check if driver initialised
bool Timer_isInitialised(void);

// Function to set the control register for Private Timer
signed int Timer_setControlRegister (bool ISR, bool auto_overFlow, bool enable);

// Function to set the prescaler value for private timer
signed int Timer_setPrescalerValue (unsigned char prescaler);

// Function to set load value for private timer
signed int Timer_setLoadValue (unsigned int load);

// Function to get the current timer value for private timer
unsigned int Timer_currentValue (void);

// function to clear the private timer interrupt flag
signed int Timer_clearInterrupt (void);

// Function to disable timer0.
// Must be disabled before loading.
signed int Timer0_disable (void);

// Function to enable timer0.
// Enable timer0 after loading.
signed int Timer0_enable (void);

// Function to load timer0. clockfreq = 100MHz
// loadValue = 100e6 * Interrupt interval
signed int Timer0_load (unsigned int loadValue);

// Function to clear timer0 interrupt flag
// must clear interrupt flag in the function attached to the interrupt
signed int Timer0_clearInterrupt (void);

#endif
