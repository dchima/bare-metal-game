
#include "IO_Peripherals.h"

//Peripherals Base Addresses


volatile unsigned int *HEX3_0_ptr = 0x0;
volatile unsigned int *HEX5_4_ptr = 0x0;
//  KEY buttons base address
volatile unsigned int *KEY_ptr  = 0x0;
//  Switches base address
volatile unsigned int *SW_ptr = 0x0;
//  LED base address
volatile unsigned int *LEDR_ptr = 0x0;

unsigned char *hex3_0_pt, *hex5_4_pt;

    
bool IO_initialised = false;


int segValues [17] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
                        0x7F, 0x67, 0x77, 0x7C, 0x39, 0x58, 0x79, 0x71, 0};
int valSelect;
                        

//Register Offsets
#define HEX0_4  (0x00/sizeof(unsigned char))
#define HEX1_5   (0x01/sizeof(unsigned char))
#define HEX2 (0x02/sizeof(unsigned char))
#define HEX3   (0x03/sizeof(unsigned char))



//Function to initialise the IO_Peripherals
signed int IO_initialise(void) {

    // HEX Base addresses
    HEX3_0_ptr = (unsigned int *) 0xFF200020;
    HEX5_4_ptr = (unsigned int *) 0xFF200030;
    // KEY buttons base address
    KEY_ptr  = (unsigned int *) 0xFF200050;
    // Switches base address
    SW_ptr = (unsigned int *) 0xFF200040;
    // LED base address
    LEDR_ptr = (unsigned int *) 0xFF200000;

    // Clear the Hex displays and the LEDs
    *HEX3_0_ptr = 0;
    *HEX5_4_ptr = 0;
    *LEDR_ptr = 0;

    hex3_0_pt = (unsigned char*) HEX3_0_ptr;
    hex5_4_pt = (unsigned char*) HEX5_4_ptr;
  
    // Configure Push Buttons to interrupt on press
    KEY_ptr[2] = 0xF;     // Enable interrupts for all four KEYs


  
//Timer now initialised
    IO_initialised = true;
    return IO_SUCCESS;
}

//Check if driver initialised
bool IO_isInitialised(void) {
    return IO_initialised;
}

// Function to read the keys interrupt register
// returns 0, 1, 2, or 3 according to the key pressed
// This function should be called only once in a function attached to the key interrupt
unsigned int IO_getKeyPressID () {
    unsigned int press, keyPressed;    
    //Read the Push-button interrupt register
    press = KEY_ptr[3];
    //Then clear the interrupt flag by writing the value back
    KEY_ptr[3] = press;
    if (press == 0x1) keyPressed = 0;
    else if (press == 0x2) keyPressed = 1;
    else if (press == 0x4) keyPressed = 2;
    else if (press == 0x8) keyPressed = 3;
    
    
    
    // return the value of the key pressed
    return keyPressed;
    
   
    
    
    
}

