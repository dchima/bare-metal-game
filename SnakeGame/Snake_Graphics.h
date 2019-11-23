/*
 * Snake_Graphics.c
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This is a modified version of the LT24_Grapics engine customized for the snake_map
 * it updates the snak map as it draws a pixel
 */
 
#ifndef Graphics_H_
#define Graphics_H_

//Include required header files
#include "Snake.h"


//Error Codes
#define GRAPHICS_SUCCESS    	0
#define GRAPHICS_TEXTERROR		-7
#define GRAPHICS_INVALIDSHAPE	-4

//define text field limits
#define MAX_FIELDX				48
#define MAX_FIELDY				40


//Function to plot line on LCD
//returns 0 if successful
signed int Snake_drawLine(unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

//Function to draw a box on the LCD
//You can activate hasFill with a true to fill up box
//Returns 0 if successful
signed int Snake_drawBox(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

signed int Snake_fillBox (unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
signed int Snake_drawLineMoreXpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

signed int Snake_drawLineMoreYpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);




#endif
