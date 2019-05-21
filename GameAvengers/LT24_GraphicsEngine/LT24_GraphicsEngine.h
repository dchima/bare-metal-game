/*
 * LT24_GraphicsEngine.h
 *
 * Created on: Mar 7, 2018
 * Author: Azeem Oguntola
 * -------------------------------------------------------------------
 * Description
 * -------------------------------------------------------------------
 * This is a Graphics Driver for the LT24 Display.
 * It provides functions for drawing lines, rectangles,
 * triangles, and circles. It also provides a function for 
 * printing text to the LCD.
 */
 
#ifndef LT24_GRAPHICSENGINE_H_
#define LT24_GRAPHICSENGINE_H_

// Required header files
#include <stdbool.h>    // Boolean library for true/false data types
#include <math.h>       // math library for math functions such as abs()


#include "../DE1SoC_LT24/DE1SoC_LT24.h"  // LT24 Display Driver
#include "../BasicFont/BasicFont.h"      // Bitmap for 8x5 characters


// Error code
#define GRAPHICS_INVALIDCORD  -3

// Size of a character
#define GRAPHICS_CHARWIDTH   5
#define GRAPHICS_CHARHEIGHT  8

// Maximum characters that can be displayed on the LCD
#define GRAPHICS_MAXSTRINGWIDTH   48
#define GRAPHICS_MAXSTRINGHEIGHT   40

// Driver Functions //

/* Function to Initialise the LT24 LCD driver */
// - returns 0 if successfull //
signed int Graphics_initialise (unsigned int lcd_pio_base, unsigned int lcd_hw_base);

/* Function to draw a Line on the LCD */
// - returns 0 if successfull //
signed int Graphics_drawLine (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
// Draws a line whose |slope| <= 1.
signed int Graphics_drawLineMoreXpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
// Draws a line whose |slope| >= 1 based on the Bresenhem algorithm 
signed int Graphics_drawLineMoreYpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/* Function to draw a Box on the LCD */
// - returns 0 if successfull //
signed int Graphics_drawBox (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
// Fills the box with the specified colour
signed int Graphics_fillBox (unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);

/* Function to draw a Triangle on the LCD */
// - returns 0 if successfull //
signed int Graphics_drawTriangle (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3);
// Fills the triangle with the specified fill colour
signed int Graphics_fillTriangle (unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3);
// Fills a flat top triangle
signed int Graphics_flatTopTriangle (unsigned short fillColour, signed int V1x, signed int V1y, signed int V2x, signed int V2y, signed int V3x, signed int V3y);
// Fills a flat base triangle
signed int Graphics_flatBaseTriangle (unsigned short fillColour, signed int V1x, signed int V1y, signed int V2x, signed int V2y, signed int V3x, signed int V3y);
// Sorts the triangle vertices
unsigned int *Graphics_sortVertices (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3);
// Obtains the maximum y value
unsigned int Graphics_maxCord (unsigned int value1, unsigned int value2, unsigned int value3);
// Obtains the minimum y value
unsigned int Graphics_minCord (unsigned int value1, unsigned int value2, unsigned int value3);

/* Function to draw Circle on the LCD */
// - returns 0 if successfull //
signed int Graphics_drawCircle (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x, unsigned int y, unsigned int r);
// Fills the circle with the specified colour
signed int Graphics_fillCircle (unsigned short fillColour, unsigned int x, unsigned int y, unsigned int r);

/*Function to print text to the LCD*/
// - returns 0 if successfull //
signed int Graphics_printText (unsigned short colour, unsigned int x, unsigned int y, unsigned char *text);
// Prints a single character on the LCD //
signed int Graphics_printChar (unsigned short colour, unsigned int x, unsigned int y, unsigned char data);

#endif /* LT24_GRAPHICSENGINE_H_ */
