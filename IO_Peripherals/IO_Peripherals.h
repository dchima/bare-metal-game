#ifndef IO_Peripherals_H_
#define IO_Peripherals_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define IO_SUCCESS       0
#define IO_ERRORNOINIT   -1
#define HEX_MAX_COUNT     6
#define HEX_INVALID_ID    -2
#define LED_MAX_COUNT     10
#define LED_INVALID_ID    -3


signed int IO_initialise(void);
bool IO_isInitialised(void);
unsigned int IO_getKeyPressID (void);

/*
signed int LED_ByID (unsigned int LED_id, bool value);

signed int LED_ByValue (unsigned int value);
bool SwitchIsOn (unsigned int switchID);
void ButtonFlag (void);
bool ButtonIsPressed (unsigned int buttonID);
signed int Hex_writeHexadecimal (int hex_id, unsigned char hexDigit);
*/

#endif
