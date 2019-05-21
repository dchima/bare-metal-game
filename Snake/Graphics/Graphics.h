/*
 * LT24 Graphics Driver (header file)
 * ------------------------------
 * Description:
 * Graphics Driver for the LT24 Display Controller
 *
 * Company: University of Leeds
 * Author: Nnadika Chima Daniel
 * SID: 201077064
 *
 * Change Log:
 *
 * Date       | Changes
 * -----------+----------------------------------
 * 03/03/2018 | Creation of driver
 * 10/03/2018 | Update driver with triangle function
 * 11/03/2018 | Added a text to LCD function
 *
 */

#ifndef Graphics_H_
#define Graphics_H_

//Include required header files
#include <stdbool.h> //Boolean variable type "bool" and "true"/"false" constants.
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//Error Codes
#define GRAPHICS_SUCCESS    	0
#define GRAPHICS_TEXTERROR		-7
#define GRAPHICS_INVALIDSHAPE	-4

//define text field limits
#define MAX_FIELDX				48
#define MAX_FIELDY				40



//Function to initialise the LCD
//returns 0 if successful
signed int Graphics_initialise(unsigned int lcd_pio_base, unsigned int lcd_hw_base);

//Function to plot line on LCD
//returns 0 if successful
signed int Graphics_drawLine(unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

//Function to draw a box on the LCD
//You can activate hasFill with a true to fill up box
//Returns 0 if successful
signed int Graphics_drawBox(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

//Function to draw a triangle
//You can activate hasFill with a true to fill up triangle
//Return 0 if successful
signed int Graphics_drawTriangle(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3);

//Function to draw a circle
//You can activate hasFill with a true to fill up circle
//Return 0 if successful
signed int Graphics_drawCircle(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x, unsigned int y, unsigned int r);

//Functin to print text on LCD
//Returns 0 if successful
signed int Graphics_text (unsigned short colour, unsigned int x, unsigned int y, const char *text);


#endif
