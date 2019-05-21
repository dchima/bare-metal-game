/*
 * Private Timer Driver for the HPS (source file)
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

#include "PrivateTimer.h"

volatile unsigned int *timer_base_ptr = 0x0;

bool timer_initialised = false;


//offsets for data size (32 bit register sizes
#define TIMER_LOAD 			(0x00/sizeof(unsigned int))
#define TIMER_VALUE			(0x04/sizeof(unsigned int))
#define TIMER_CONTROL		(0X08/sizeof(unsigned int))
#define TIMER_INTERRUPT		(0x0C/sizeof(unsigned int))


//Initialise the Private timer
signed int Timer_initialise(unsigned int base_address){

	//set base address
	timer_base_ptr = (unsigned int *) base_address;
	timer_base_ptr[TIMER_CONTROL] = 0; //set controls to zero
	timer_initialised = true;


	return TIMER_SUCCESS;
}

bool Timer_isInitialised(void){
	return timer_initialised;

}

// set load value of timer oad register
//normally set to its maximum 0xFFFFFFFF
//timer counts down from this value
signed int Timer_setLoad(unsigned int load){
	//check if timer has been initialised
	if(!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_LOAD] = load;

	return TIMER_SUCCESS;
}

//this function returns the current value
//of the timer from the current value register
signed int Timer_readValue(void){
	//check if timer has been initialised
	if(!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	return timer_base_ptr[TIMER_VALUE];
}

//Here we set the control booleans
signed int Timer_setControl(bool I, bool A, bool E){
	//check if timer has been initialised
	if(!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	if(I) timer_base_ptr[TIMER_CONTROL] |= (1<<2);
	else timer_base_ptr[TIMER_CONTROL] &= ~(1<<2);
	if(A) timer_base_ptr[TIMER_CONTROL] |= (1<<1);
	else timer_base_ptr[TIMER_CONTROL] &= ~(1<<1);
	if(E) timer_base_ptr[TIMER_CONTROL] |= (1<<0);
	else timer_base_ptr[TIMER_CONTROL] &= ~(1<<0);

	return TIMER_SUCCESS;
}

//this function sets the prescaler value of the timer
signed int Timer_setPrescaler(unsigned char prescaler){
	//check if timer has been initialised
	if(!Timer_isInitialised()) return TIMER_ERRORNOINIT;
	timer_base_ptr[TIMER_CONTROL] |= (prescaler<<8);
	return TIMER_SUCCESS;
}

//Read the state of the interupt
bool Timer_readInterrupt(void){
	//check if timer has been initialised
	if(!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	return (bool) timer_base_ptr[TIMER_INTERRUPT];
}


//this function clears the timer interrupt
signed int Timer_clearInterrupt(void){
	//check if timer has been initialised
	if(!Timer_isInitialised()) return TIMER_ERRORNOINIT;

	timer_base_ptr[TIMER_INTERRUPT] = 1;

	return TIMER_SUCCESS;
}
