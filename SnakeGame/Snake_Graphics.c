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



#include "Snake_Graphics.h"

/* Function to draw a Line on the LCD */
signed int Snake_drawLine (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    // This function draws a line of colour from point (x1,y1) to point (x2,y2)
    unsigned int dx, dy;
    
    // Sanity Checks //
    if (!LT24_isInitialised()) return LT24_ERRORNOINIT; //Don't run if not yet initialised
    //Ensure coordinates are in range
    if (x1 >= LT24_WIDTH || x2 >= LT24_WIDTH)   return GRAPHICS_INVALIDCORD; //Invalid coordinates
    if (y1 >= LT24_HEIGHT || y2 >= LT24_HEIGHT) return GRAPHICS_INVALIDCORD; //Invalid coordinates

    // Compute the absolute difference in the x and y cordinates of the points 
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    // Classify the type of line based on the slope
    // If dx > dy, the magnitude of slope |dy/dx| is less than 1 and there are 
    // more unique x points on the line than there are y points. Otherwise, there
    // are more or the same unique y points as there are x points.
    if (dx > dy ) {
    // Call the function to draw the type of line where |slope| < 1
    // swap cordinates if x1 > x2.
        if (x1 > x2) Snake_drawLineMoreXpts (colour, x2, y2, x1, y1);
        else Snake_drawLineMoreXpts (colour, x1, y1, x2, y2);
    } else {
    // Call the function to draw the type of line where |slope| >= 1
    // swap cordinates if y1 > y2.
        if (y1 > y2) Snake_drawLineMoreYpts (colour, x2, y2, x1, y1);
        else Snake_drawLineMoreYpts (colour, x1, y1, x2, y2);
    }
    return LT24_SUCCESS; // return success
}
// This function draws a line whose |slope| <= 1
signed int Snake_drawLineMoreXpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {     
    // Compute the absolute difference in the x and y cordinates of the points
    unsigned int dx = abs(x2 - x1);
    unsigned int dy = abs(y2 - y1);
    unsigned int x, y = y1; // we start at y1
    
    // The for every x point on the line, the y point is given by the
    // equation of a line as y = (dy/dx) * (x - x1) + y1.
    // Since there are more x points, every next point of the line is certainly at x+1
    // The next point of the line is either at (x+1, y) or (x+1, y+1).
    // The difference between the two possible points when applied to the equation
    // of a line is given as Diff = 2*dy - dx    
    
    signed int Diff = (dy << 1) - dx; // Left shift once is multiplying by 2
    signed int yinc = 1;
    // We move from point (x1,y1) to (x2,y2) If y1 is greater, then we decrement y.    
    if (y1 > y2) yinc = -1; 
    // increment x2 to avoid using <= in the for loop there by saving
    // one instruction per loop
    x2++;   
    // Move along the line from x1 to x2
    for (x = x1; x < x2; x++) {
		LT24_drawPixel(colour, x, y);   // draw a pixel
        snake_map[(unsigned short)x][(unsigned short)y] = 8;
        // If the difference > 0, the next y point is y+1 (or y-1)
        // we decrement the difference by 2*dx
        if (Diff > 0) {
            y = y + yinc;
            Diff -= (dx << 1);
        }
        // Increment the difference by 2*dy
        Diff += (dy << 1);      
    }
    return LT24_SUCCESS;        // return success
}
// This function draws a line whose |slope| >= 1 based on the Bresenhem algorithm 
signed int Snake_drawLineMoreYpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    // Compute the absolute difference in the x and y cordinates of the points
    unsigned int dx = abs(x2 - x1);
    unsigned int dy = abs(y2 - y1);
    unsigned int x = x1, y; // we start at x1

    // The for every y point on the line, the x point is given by the
    // equation of a line as x = 1/(dy/dx) * (y - y1) + x1.        
    // Since there are more y points, every next point of the line is certainly at y+1
    // The next point of the line is either at (x, y+1) or (x+1, y+1).
    // The difference between the two possible points when applied to the equation
    // of a line is given as Diff = 2*dx - dy    
    
    signed int Diff = (dx << 1) - dy;
    signed int xinc = 1;
    // We move from point (x1,y1) to (x2,y2) If x1 is greater, then we decrement x.       
    if (x1 > x2) xinc = -1;
    // increment y2 to avoid using <= in the for loop there by saving
    // one instruction per loop
    y2++;
    // Move along the line from y1 to y2
    for (y = y1; y < y2; y++) {
		LT24_drawPixel(colour, x, y);
        snake_map[(unsigned short)x][(unsigned short)y] = 8;
        // If the difference > 0, the next x point is x+1 (or x-1)
        // we decrement the difference by 2*dy
        if (Diff > 0) {
            x = x + xinc;
            Diff -= (dy << 1);
        }
        // Increment the difference by 2*dy
        Diff += (dx << 1);
    }
    return LT24_SUCCESS;    // return success
}

/* Function to draw a Box on the LCD */
signed int Snake_drawBox (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    // This function draws a box line of outline colour with diagonal (x1,y1) to (x2,y2)
    // If hasFill is true, the box is filled with fillColour, other wise,
    // only the outline is plotted.

    // Sanity Checks //
    if (!LT24_isInitialised()) return LT24_ERRORNOINIT; //Don't run if not yet initialised
    //Ensure coordinates are in range
    if (x1 >= LT24_WIDTH || x2 >= LT24_WIDTH)   return GRAPHICS_INVALIDCORD; //Invalid coordinates
    if (y1 >= LT24_HEIGHT || y2 >= LT24_HEIGHT) return GRAPHICS_INVALIDCORD; //Invalid coordinates
    if (x1==x2 || y1 == y2)     return GRAPHICS_INVALIDCORD; //Invalid coordinates

    // If the box is to be filled, fill up with fill colour
    if (hasFill) Snake_fillBox (fillColour, x1, y1, x2, y2);

    // Draw the box outline
    Snake_drawLine (colour, x1, y1, x1, y2);
    Snake_drawLine (colour, x1, y1, x2, y1);
    Snake_drawLine (colour, x2, y2, x1, y2);
    Snake_drawLine (colour, x2, y2, x2, y1);

    return LT24_SUCCESS;    // return success
}
// This function fills a box with the specified colour
signed int Snake_fillBox (unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    // The box is filled by drawing vertical lines inside the box
    unsigned int x;
    // We offset the cordinates by decrementing the greater one and
    // and incrementing the lesser one. This is so as not to overwrite 
    // the outline of the box
    if (y2 > y1) {
        y2--;
        y1++;
    } else {
        y2++;
        y1--;
    }
    // Draw vertical lines for each x cordinate in the box
    // We begin from the lesser x cord to the greater cord.
    // We offset the cordinate just like before to avoid overwriting
    // the outline.
    if (x2 > x1) {
        x1++;
        for (x = x1; x < x2; x++) {
            Snake_drawLine (fillColour, x, y1, x, y2);
        }
    } else {
        x2++;
        for (x = x2; x < x1; x++) {
            Snake_drawLine (fillColour, x, y1, x, y2);
           }
    }
    return LT24_SUCCESS;    // return success    
}

