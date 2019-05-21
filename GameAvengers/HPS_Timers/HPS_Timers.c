/*
 * HPS_Timers.c
 *
 * Created on: Feb 26, 2018
 * Author: Azeem Oguntola
 * -------------------------------------------------------------------
 * Description
 * -------------------------------------------------------------------
 * This is a Bare Metal Driver for the ARM Timers.
 * 
 * --- Change Log ------
 * Mar 22, 2018 :
 * Renamed from HPS_PrivateTimer to HPS_Timers
 * Updated to include functions for other timers in the DE1-Soc 
 * -------------------------------------------------------------------
 * Timer_ functions are for the Private timer 231.25 MHz
 * Timer0_, Timer1_, Timer2_, and Timer3_ functions refer to the other timers 
 *
 * The private timer is intended to be used for polling and task scheduling
 * Timer0 to 3 are intended to be used for IRQ interrupts
 *
 */

#include "HPS_Timers.h"

//Driver Base Address
volatile unsigned int *timer_base_ptr = 0x0; // Private timer 231.25 MHz
volatile unsigned int *timer0_base_ptr = 0x0; // Timer0       100MHz

//Driver Initialised
bool timer_initialised = false;

//Register Offsets
#define TIMER_LOAD (0x00/sizeof(unsigned int))
#define TIMER_VALUE (0x04/sizeof(unsigned int))
#define TIMER_CONTROL (0x08/sizeof(unsigned int))
#define TIMER_INTERRUPT (0x0C/sizeof(unsigned int))


//Function to initialise the Timers
signed int Timer_initialise(void){

//Initialise base address pointers
    timer_base_ptr = (unsigned int *) 0xFFFEC600;       // Private Timer
    timer0_base_ptr = (unsigned int *) 0xFFC08000;      // Timer0 
    
    
//Ensure timers initialises to disabled
    timer_base_ptr[TIMER_CONTROL] = 0;
    timer0_base_ptr[TIMER_CONTROL] = 0;
    
//Timer now initialised
    timer_initialised = true;
    return TIMER_SUCCESS;
}

//Check if driver initialised
bool Timer_isInitialised(void) {
    return timer_initialised;
}

// Function to set the control register for Private Timer
signed int Timer_setControlRegister (bool ISR, bool auto_overFlow, bool enable) {
    // Check if timer is initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // ISR
    if(ISR) timer_base_ptr[TIMER_CONTROL] |= (1 << 2);
    else timer_base_ptr[TIMER_CONTROL] &= ~(1 << 2);
    // Auto overflow
    if(auto_overFlow) timer_base_ptr[TIMER_CONTROL] |= (1 << 1);
    else timer_base_ptr[TIMER_CONTROL] &= ~(1 << 1);
    // Enable 
    if(enable) timer_base_ptr[TIMER_CONTROL] |= (1 << 0);
    else timer_base_ptr[TIMER_CONTROL] &= ~(1 << 0);
    
    return TIMER_SUCCESS;
}

// Function to set the prescaler value for private timer
signed int Timer_setPrescalerValue (unsigned char prescaler) {
    // Check if timer is initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // set the prescaler
    timer_base_ptr[TIMER_CONTROL] |= (prescaler << 8);
    return TIMER_SUCCESS;
}
// Function to set load value for private timer
signed int Timer_setLoadValue (unsigned int load) {
    // Check if timer is initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // load the timer
    timer_base_ptr[TIMER_LOAD] = load;
    return TIMER_SUCCESS;
}
// Function to get the current timer value for private timer
unsigned int Timer_currentValue (void) {
    // Check if timer is initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // return the timer value
    return timer_base_ptr[TIMER_VALUE];
}
// function to clear the private timer interrupt flag
signed int Timer_clearInterrupt (void) {
    // Check if timer is initialised
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // If flag is set, clear the interrupt flag by writing a 1
    if (timer_base_ptr[TIMER_INTERRUPT]) timer_base_ptr[TIMER_INTERRUPT] = 0x1;
    return TIMER_SUCCESS;
}

// Function to disable timer0.
// Must be disabled before loading.
signed int Timer0_disable (void) {
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // disable the timer0
    timer0_base_ptr[TIMER_CONTROL] = 0;
    return TIMER_SUCCESS;
    
}
// Function to enable timer0.
// Enable timer0 after loading.
signed int Timer0_enable (void) {
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // enable the timer0
    timer0_base_ptr[TIMER_CONTROL] = 0x03;
    return TIMER_SUCCESS;
}

// Function to load timer0. clockfreq = 100MHz
// loadValue = 100e6 * Interrupt interval
signed int Timer0_load (unsigned int loadValue) {
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    // enable the timer0
    timer0_base_ptr[TIMER_LOAD] = loadValue;
    return TIMER_SUCCESS;    
}

// Function to clear timer0 interrupt flag
// must clear interrupt flag in the function attached to the interrupt
signed int Timer0_clearInterrupt (void) {
    unsigned int clear = 0;
    if (!Timer_isInitialised()) return TIMER_ERRORNOINIT;
    //Clear the Timer Interrupt Flag
    //By reading timer end of interrupt register
    clear = timer0_base_ptr[TIMER_INTERRUPT];
    return TIMER_SUCCESS;    
}



