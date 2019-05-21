/*
 * Private Timer Driver for the HPS (header file)
 * ------------------------------
 * Description:
 * HPS Private Timer Driver
 *
 * Company: University of Leeds
 * Author: Nnadika Chima Daniel
 * SID: 201077064
 *
 * Change Log:
 *
 * Date       | Changes
 * -----------+----------------------------------
 * 26/02/2018 | Creation of driver
 *
 */

#ifndef PrivateTimer_H_
#define PrivateTimer_H_

#include <stdbool.h>


#define TIMER_SUCCESS		0
#define TIMER_ERRORNOINIT  -1

//initialise the private timer
//return 0 if successful
signed int Timer_initialise(unsigned int base_Address);

//check if timer is initialised
bool Timer_isInitialised(void);

//set load value of timer
//return 0 if successful
signed int Timer_setLoad(unsigned int load);

//read current value of timer
//return 0 if successful
signed int Timer_readValue(void);

//this function sets the controls of timer
//return 0 if successful
signed int Timer_setControl(bool I, bool A, bool E);

//this function sets the prescaler value of timer
//return 0 if successful
signed int Timer_setPrescaler(unsigned char prescaler);

//read interrupt value
bool Timer_readInterrupt(void);

//this function clears the interrupt value
//returns 0 if successful
signed int Timer_clearInterrupt(void);

#endif
