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

#include "HPS_IRQ.h"

#include <stdlib.h>


/*
 * Declare various useful pointer addresses in the Cyclone V HPS
 */

// Interrupt controller (GIC) CPU interface(s)
#define MPCORE_GIC_CPUIF     0xFFFEC100                   // PERIPH_BASE + 0x100
#define ICCICR               (0x00/sizeof(unsigned int))  // + to CPU interface control
#define ICCPMR               (0x04/sizeof(unsigned int))  // + to interrupt priority mask
#define ICCIAR               (0x0C/sizeof(unsigned int))  // + to interrupt acknowledge
#define ICCEOIR              (0x10/sizeof(unsigned int))  // + to end of interrupt reg

// Interrupt (INT) controller (GIC) distributor interface(s)
#define MPCORE_GIC_DIST      0xFFFED000                   // PERIPH_BASE + 0x1000
#define ICDDCR               (0x000/sizeof(unsigned int)) // + to distributor control reg
#define ICDISER              (0x100/sizeof(unsigned int)) // + to INT set-enable regs
#define ICDICER              (0x180/sizeof(unsigned int)) // + to INT clear-enable regs
#define ICDIPTR              (0x800/sizeof(unsigned int)) // + to INT processor targets regs
#define ICDICFR              (0xC00/sizeof(unsigned int)) // + to INT configuration regs


/*
 * Global variables for the IRQ driver
 */

bool irq_isInitialised = false;

typedef struct {
    unsigned int interruptID;  //The ID of the interrupt source this handler is for
    isr_handler_func handler; //Function pointer to be called to handle this ID
    bool enabled;
} isr_handler;

unsigned int irq_isr_handler_count;
isr_handler* irq_isr_handlers;

volatile unsigned int* irq_gic_cpuif_ptr = (unsigned int *)MPCORE_GIC_CPUIF;
volatile unsigned int* irq_gic_dist_ptr  = (unsigned int *)MPCORE_GIC_DIST;

/*
 * Here we create a vector table using an inline assembly function. This function is
 * not compiled using the C compiler, but fed directly to the Linker.
 * In the table we have two types of entry:
 *   LDR PC,=<functionName> ; Sets Page Counter equal to function address.
 *   DCD 0 ; will trigger a jump to "Undefined Instruction" vector
 */
__asm __attribute__((section("vector_table"))) void vector_table(void) {
    //Import names for various functions which are "extern"al to the __asm block.
    extern __rt_entry          ; // C Standard Library entry point
    extern __irq_isr           ; // IRQ Interrupt Handler
    //Vector Table Entries
    LDR         PC,=__rt_entry ; // 0x00 Reset
    LDR         PC,=__rt_entry ; // 0x04 Undefined Instructions
    DCD         0              ; // 0x08 Software Interrupts (SWI)
    DCD         0              ; // 0x0C Prefetch Abort
    DCD         0              ; // 0x10 Data Abort
    DCD         0              ; // 0x14 RESERVED
    LDR         PC,=__irq_isr  ; // 0x18 IRQ
    DCD         0              ; // 0x1C FIQ
}

/*
 * Next we need our interrupt service routine for IRQs
 *
 * This will check the interrupt id against all registered handlers
 * and cause a crash if it is an unhandled interrupt (watchdog reset)
 */

__irq void __irq_isr (void) {
    bool isr_handled = false;
    // Read the ICCIAR value to get interrupt ID
    unsigned int int_ID = irq_gic_cpuif_ptr[ICCIAR];
    // Check to see if we have a registered handler
    unsigned int handler;
    for (handler = 0; handler < irq_isr_handler_count; handler++) {
        if (int_ID == irq_isr_handlers[handler].interruptID) {
            //If we have found a handler for this ID
            irq_isr_handlers[handler].handler(); //Call it
            isr_handled = true; //And mark as handled
            break;
        }
    }
    //Check if we have an unhandled interrupt
    if (!isr_handled) while(1); //Die. Watchdog timer will kill us.

    //Otherwise write to the End of Interrupt Register (ICCEOIR) to mark as handled
    irq_gic_cpuif_ptr[ICCEOIR] = int_ID;
    //And done.
    return;
}


/* Setup stack pointer (SP) for IRQ mode using assembler */
__asm void HPS_IRQ_ConfigureIRQStack(void) {
    MSR CPSR_c, #0xD2
    MOV SP, #0xFFFFFFF8
    MSR CPSR_c, #0xD3
    BX LR
}

//Initialise HPS IRQ Driver
signed int HPS_IRQ_initialise( ) {

    /* Configure Global Interrupt Controller (GIC) */
    //Disable interrupts before configuring
    __asm("cpsid if ;");  // mask IRQ and FIQ (we will not re-enable FIQ)

    // Set Interrupt Priority Mask Register (ICCPMR)
    // Enable interrupts of all priorities
    irq_gic_cpuif_ptr[ICCPMR] = 0xFFFF;

    // Set CPU Interface Control Register (ICCICR)
    // Enable signalling of interrupts
    irq_gic_cpuif_ptr[ICCICR] = 0x1;

    // Configure the Distributor Control Register (ICDDCR)
    // Send pending interrupts to CPUs
    irq_gic_dist_ptr[ICDDCR] = 0x1;

    //Configure the IRQ mode stack
    HPS_IRQ_ConfigureIRQStack();

    //Initially no handlers
    irq_isr_handler_count = 0;
    irq_isr_handlers = 0x0;

    //Enable interrupts again
    __asm("cpsie i ;");  // unmask IRQ

    //Mark as initialised
    irq_isInitialised = true;
    //And done
    return HPS_IRQ_SUCCESS;
}

//Check if driver initialised
// - returns true if initialised
bool HPS_IRQ_isInitialised() {
    return irq_isInitialised;
}

signed int HPS_IRQ_registerHandler(unsigned int interruptID, isr_handler_func handlerFunction) {
    isr_handler* new_handler;
    unsigned int handler;
    volatile unsigned char* diptr;
    if (!HPS_IRQ_isInitialised()) return HPS_IRQ_ERRORNOINIT;
    //First check if a handler already exists (we can overwrite it if it does)
    for (handler = 0; handler < irq_isr_handler_count; handler++) {
        if (irq_isr_handlers[handler].interruptID == interruptID) {
            //Found an existing one. We'll just overwrite it, instead of making new one.
            break;
        }
    }
    //Before changing anything we need to mask interrupts temporarily while we change the handlers
    __asm("cpsid i ;");  // mask IRQ and FIQ
    if (handler == irq_isr_handler_count) {
        //If we failed to find a match, we need to reallocated our handler array to gain more space
        new_handler = realloc(irq_isr_handlers, (irq_isr_handler_count + 1)*sizeof(isr_handler) );
        //We should be sure to check that reallocation was a success
        if (new_handler == 0x0) {
            //If realloc returned null, then reallocation failed, cannot register new handler.
            return HPS_IRQ_NONENOUGHMEM;
        }
        //If we were successful in making space, update our global pointer
        irq_isr_handlers = new_handler;
        //Update the count of available handlers
        irq_isr_handler_count = irq_isr_handler_count + 1;
    }
    //Add our new handler to the end
    irq_isr_handlers[handler].handler = handlerFunction;
    irq_isr_handlers[handler].interruptID = interruptID;
    irq_isr_handlers[handler].enabled = true;
    //Then we need to enable the interrupt in the distributer
    irq_gic_dist_ptr[ICDISER + (interruptID / 32)] = 1 << (interruptID % 32);
    //And set the affinity to CPU0
    diptr = (unsigned char*)&(irq_gic_dist_ptr[ICDIPTR + interruptID / 4]);
    diptr[interruptID % 4] = 0x1;
    //Finally we unmask interrupts to resume processing.
    __asm("cpsie i ;");  // unmask IRQ and FIQ
    //And done.
    return HPS_IRQ_SUCCESS;
}

signed int HPS_IRQ_unregisterHandler(unsigned int interruptID) {
    unsigned int handler;
    if (!HPS_IRQ_isInitialised()) return HPS_IRQ_ERRORNOINIT;
    //See if we can find the requested handler
    for (handler = 0; handler < irq_isr_handler_count; handler++) {
        if (irq_isr_handlers[handler].interruptID == interruptID) {
            //Found it.
            //Before changing anything we need to mask interrupts temporarily while we change the handlers
            __asm("cpsid i ;");  // mask IRQ
            //Clear the handler pointer, and mark as disabled
            irq_isr_handlers[handler].handler = 0x0;
            irq_isr_handlers[handler].enabled = false;
            //Then we need to disable the interrupt in the distributer
            irq_gic_dist_ptr[ICDICER + (interruptID / 32)] = 1 << (interruptID & 31);
            //Finally we unmask interrupts to resume processing.
            __asm("cpsie i ;");  // unmask IRQ
            //And done
            return HPS_IRQ_SUCCESS;
        }
    }
    //Woops, handler doesn't exist.
    return HPS_IRQ_NOTFOUND;
}


