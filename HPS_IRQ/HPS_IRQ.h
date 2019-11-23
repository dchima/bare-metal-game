/*
 * Cyclone V HPS Interrupt Controller
 * ------------------------------------
 * Description:
 * Driver for enabling and using the General Interrupt
 * Controller (GIC). The driver includes code to create
 * a vector table, and register interrupts.
 *
 * The code makes use of function pointers to register
 * interrupt handlers for specific interrupt IDs.
 *
 * Company: University of Leeds
 * Author: T Carpenter
 *
 * Change Log:
 *
 * Date       | Changes
 * -----------+----------------------------------
 * 12/03/2018 | Creation of driver
 *
 */

#ifndef HPS_IRQ_H_
#define HPS_IRQ_H_

#include <stdbool.h>

#define HPS_IRQ_SUCCESS        0
#define HPS_IRQ_ERRORNOINIT   -1
#define HPS_IRQ_NOTFOUND      -2
#define HPS_IRQ_NONENOUGHMEM  -4


//Include a list of IRQ IDs that can be used while registering interrupts
#include "HPS_IRQ_IDs.h"

//Function Pointer Type for Interrupt Handlers
typedef void (*isr_handler_func)(void);

//Initialise HPS IRQ Driver
// - Returns HPS_IRQ_SUCCESS if successful.
signed int HPS_IRQ_initialise(void);

//Check if driver initialised
// - Returns true if driver previously initialised
bool HPS_IRQ_isInitialised(void);

//Register a new interrupt ID handler
// - interruptID is the number between 0 and 255 of the interrupt being configured
// - handlerFunction is a function pointer to the function that will be called when IRQ with ID occurs
// - if a handler already exists for the specified ID, it will be replaced by the new one.
// - the interrupt ID will be enabled in the GIC
// - returns HPS_IRQ_SUCCESS on success.
// - returns HPS_IRQ_NONENOUGHMEM if failed to reallocated handler array.
signed int HPS_IRQ_registerHandler(unsigned int interruptID, isr_handler_func handlerFunction);

//Unregister interrupt ID handler
// - interruptID is the number between 0 and 255 of the interrupt being configured
// - the interrupt will be disabled also in the GIC
// - returns HPS_IRQ_SUCCESS on success.
// - returns HPS_IRQ_NOTFOUND if handler not found
signed int HPS_IRQ_unregisterHandler(unsigned int interruptID);

#endif /* HPS_IRQ_H_ */
