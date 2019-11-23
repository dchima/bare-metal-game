/*
 * LT24_GraphicsEngine.c
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

#include "LT24_GraphicsEngine.h"

/* Function to Initialise the LT24 LCD driver */
signed int Graphics_initialise (unsigned int lcd_pio_base, unsigned int lcd_hw_base) {

    // If the LT24 display is not initialised, initialise it
    if (!LT24_isInitialised()) LT24_initialise (lcd_pio_base, lcd_hw_base);

    return LT24_SUCCESS;    // return the success code
}

/* Function to draw a Line on the LCD */
signed int Graphics_drawLine (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
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
        if (x1 > x2) Graphics_drawLineMoreXpts (colour, x2, y2, x1, y1);
        else Graphics_drawLineMoreXpts (colour, x1, y1, x2, y2);
    } else {
    // Call the function to draw the type of line where |slope| >= 1
    // swap cordinates if y1 > y2.
        if (y1 > y2) Graphics_drawLineMoreYpts (colour, x2, y2, x1, y1);
        else Graphics_drawLineMoreYpts (colour, x1, y1, x2, y2);
    }
    return LT24_SUCCESS; // return success
}
// This function draws a line whose |slope| <= 1
signed int Graphics_drawLineMoreXpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {     
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
signed int Graphics_drawLineMoreYpts (unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
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
signed int Graphics_drawBox (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
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
    if (hasFill) Graphics_fillBox (fillColour, x1, y1, x2, y2);

    // Draw the box outline
    Graphics_drawLine (colour, x1, y1, x1, y2);
    Graphics_drawLine (colour, x1, y1, x2, y1);
    Graphics_drawLine (colour, x2, y2, x1, y2);
    Graphics_drawLine (colour, x2, y2, x2, y1);

    return LT24_SUCCESS;    // return success
}
// This function fills a box with the specified colour
signed int Graphics_fillBox (unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
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
            Graphics_drawLine (fillColour, x, y1, x, y2);
        }
    } else {
        x2++;
        for (x = x2; x < x1; x++) {
            Graphics_drawLine (fillColour, x, y1, x, y2);
           }
    }
    return LT24_SUCCESS;    // return success    
}

/* Function to draw a Triangle on the LCD */
signed int Graphics_drawTriangle (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3) {
    // This function draws a triangle of outline colour at corners x1

    // Sanity Checks //
    if (!LT24_isInitialised()) return LT24_ERRORNOINIT; //Don't run if not yet initialised
    //Ensure end coordinates are in range
    if (x1 >= LT24_WIDTH || x2 >= LT24_WIDTH || x3 >= LT24_WIDTH)   return GRAPHICS_INVALIDCORD; //Invalid coordinates
    if (y1 >= LT24_HEIGHT || y2 >= LT24_HEIGHT || y3 >= LT24_HEIGHT) return GRAPHICS_INVALIDCORD; //Invalid coordinates
    // Ensure cordinates are appropriate for triangle
    if ((x1==x2 && x1 == x3) || (y1 == y2 && y1 ==y3)) return GRAPHICS_INVALIDCORD; //Invalid cordinates for box

    // If the triangle is to be filled, fill up
    if (hasFill) Graphics_fillTriangle (fillColour, x1, y1, x2, y2, x3, y3);
    
    // Draw triangle outline
    Graphics_drawLine (colour, x1, y1, x2, y2);
    Graphics_drawLine (colour, x2, y2, x3, y3);
    Graphics_drawLine (colour, x3, y3, x1, y1);
    
    return LT24_SUCCESS;    // return success
}
// This function fills the triangle with the specified fill colour
signed int Graphics_fillTriangle (unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3) {
    // sort the triangle vertices in ascending order of y value such
    // that Vy1 <= Vy2 <= Vy3
    // Vxy[0] = Vx1, Vxy[1] = Vy1, Vxy[2] = Vx2, Vxy[3] = Vy2
    // Vxy[4] = Vx3, Vxy[5] = Vy3
    unsigned int *Vxy = Graphics_sortVertices (x1, y1, x2, y2, x3, y3);
    unsigned int V4y, V4x;

    // The triangle is classified into three types.
    // A flat base triangle, a flat top triangle, and a regular triangle
    unsigned int TriangleType = 0;  // Default to regular triangle
    
    if (Vxy[1] == Vxy[3]) TriangleType = 1; // if y1 = y2 flatTopTriangle
    if (Vxy[3] == Vxy[5]) TriangleType = 2; // if y2 = y3 flatBaseTriangle
    
    // Call functions to fill the triangle based on the type
    if (TriangleType == 1) {    // flat top triangle
        Graphics_flatTopTriangle (fillColour, Vxy[0], Vxy[1], Vxy[2], Vxy[3], Vxy[4], Vxy[5]);
    } else if (TriangleType == 2) {  // flat base triangle
        Graphics_flatBaseTriangle (fillColour, Vxy[0], Vxy[1], Vxy[2], Vxy[3], Vxy[4], Vxy[5]);
    } else {
        // If regular triangle, divide into two parts; a flat top and a flat base
        // obtain the fourth vertice
        V4y = Vxy[3];
        V4x = (unsigned int)(Vxy[0] + ((float)(Vxy[3] - Vxy[1])/(float)(Vxy[5] - Vxy[1]))*((float)Vxy[4] - (float)Vxy[0]));
        // fill both parts seperately
        Graphics_flatBaseTriangle (fillColour, Vxy[0], Vxy[1], Vxy[2], Vxy[3], V4x, V4y);
        Graphics_flatTopTriangle (fillColour, V4x, V4y, Vxy[2], Vxy[3], Vxy[4], Vxy[5]);
    }            
    return LT24_SUCCESS;    // return success
}
// Function to fill a flat top triangle
signed int Graphics_flatTopTriangle (unsigned short fillColour, signed int V1x, signed int V1y, signed int V2x, signed int V2y, signed int V3x, signed int V3y) {
    // The vertices passed to this function have been ordered such that 
    // Vy1 <= Vy2 <= Vy3
    // For a flat base triangle, Vy1 = Vy2 and both are less than Vy3
    // To fill up the triangle, we transverse both legs of the triangle in the
    // y direction and draw horizontal lines parallel to the top between 
    // both legss.
    unsigned int yi;
    // Get the slopes of both legs of the triangle
    float invSlopeV23 = (float)(V3x - V2x)/(float)(V3y - V2y);
    float invSlopeV13 = (float)(V3x - V1x)/(float)(V3y - V1y);    
   // Set the starting point to the top of the triangle
    float V23xi = V3x, V13xi = V3x;
    V3y--;    
    LT24_drawPixel (fillColour, V3x, V3y);

    // move along the y direction and calculate the corresponding x points  
    for (yi = V3y; yi > V1y; yi-- ) {
        // Get the new x points
        V23xi -= invSlopeV23;
        V13xi -= invSlopeV13;
        // Draw horizontal line between both points
        Graphics_drawLine (fillColour, (unsigned int)V23xi, yi, (unsigned int)V13xi, yi);
    }
    return LT24_SUCCESS;    // return success
}
// Function to fill a flat base triangle
signed int Graphics_flatBaseTriangle (unsigned short fillColour, signed int V1x, signed int V1y, signed int V2x, signed int V2y, signed int V3x, signed int V3y) {
    // The vertices passed to this function have been ordered such that 
    // Vy1 <= Vy2 <= Vy3
    // For a flat base triangle, Vy2 = Vy3 and both are greater than Vy1
    // To fill up the triangle, we transverse both legs of the triangle in the
    // y direction and draw horizontal lines parallel to the base between 
    // both legss.
    unsigned int yi;
    // Get the slopes of both legs of the triangle
    float invSlopeV12 = (float)(V2x - V1x)/(float)(V2y - V1y); //slope of line V1 to V2
    float invSlopeV13 = (float)(V3x - V1x)/(float)(V3y - V1y); //slope of line V1 to V3
    // Set the starting point to the top of the triangle
    float V12xi = V1x, V13xi = V1x;
    V1y++;
    V2y++;
    LT24_drawPixel (fillColour, V1x, V1y);
    // move along the y direction and calculate the corresponding x points
    for (yi = V1y; yi < V2y; yi++ ) {
        // Get the new x points
        V12xi += invSlopeV12;       
        V13xi += invSlopeV13;            
        // Draw horizontal line between both points
        Graphics_drawLine (fillColour, (unsigned int)V12xi, yi, (unsigned int)V13xi, yi);
    }
    return LT24_SUCCESS;    // return success
}
// Function to sort the triangle vertices
unsigned int *Graphics_sortVertices (unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3) {
    // This function sorts the triangle vertices in ascending order
    // of y and returns the sorted vertices as an array.
    // Sort order V1y <= V2y <= v3y

    unsigned int yMax, yMin;
    //array to store sorted vertices
    // Vxy[0] = Vx1, Vxy[1] = Vy1, Vxy[2] = Vx2, Vxy[3] = Vy2
    // Vxy[4] = Vx3, Vxy[5] = Vy3
    unsigned int static Vxy [6];    
    yMax = Graphics_maxCord (y1, y2, y3);     // Get the maximum y value
    yMin = Graphics_minCord (y1, y2, y3);     // Get the minimum y value

    // If y1 is the maximum value, set y1 as V3 and check the other
    // two vertices
    if (yMax == y1) {
            Vxy[4] = x1;    //Vx3
            Vxy[5] = y1;    //Vy3        
        if (yMin == y2) {
            Vxy[0] = x2;    // Vx1
            Vxy[1] = y2;    // Vy1
            Vxy[2] = x3;    // Vx2
            Vxy[3] = y3;    // Vy2
        } else if (yMin == y3) {
            Vxy[0] = x3;
            Vxy[1] = y3;
            Vxy[2] = x2;
            Vxy[3] = y2;
        }
    } else if (yMax == y2) {
        // if y2 is the maximum, then set y2 as V3 and check the other
        // vertices
            Vxy[4] = x2;
            Vxy[5] = y2;            
        if (yMin == y1) {
            Vxy[0] = x1;
            Vxy[1] = y1;
            Vxy[2] = x3;
            Vxy[3] = y3;
        } else if (yMin == y3) {
            Vxy[0] = x3;
            Vxy[1] = y3;
            Vxy[2] = x1;
            Vxy[3] = y1;
        }
    } else if (yMax == y3) {
        // else if y3 is the maximum, set y3 as V3
            Vxy[4] = x3;
            Vxy[5] = y3;            
        if (yMin == y1) {
            Vxy[0] = x1;
            Vxy[1] = y1;
            Vxy[2] = x2;
            Vxy[3] = y2;
        } else if (yMin == y2) {
            Vxy[0] = x2;
            Vxy[1] = y2;
            Vxy[2] = x1;
            Vxy[3] = y1;
        }
    }
//  Sort vertices end
    return Vxy;     // return the sorted vertices    
}
// Function to obtain the maximum y value
unsigned int Graphics_maxCord (unsigned int value1, unsigned int value2, unsigned int value3) {
    unsigned int maxValue = value1; // assume value 1 is the maximum
    // Compare with the other values and set to maximum if greater than maximum
    if (value2 > maxValue) maxValue = value2;
    if (value3 > maxValue) maxValue = value3;
    return maxValue;    // return the maximum value
}
// Function to obtain the minimum y value
unsigned int Graphics_minCord (unsigned int value1, unsigned int value2, unsigned int value3) {
    unsigned int minValue = value1; // assume value 1 is the minimum
    // Compare with the other values and set to minimum if less than minimum
    if (value2 < minValue) minValue = value2;
    if (value3 < minValue) minValue = value3;        
    return minValue;    // return the minimum value
}

/* Function to draw Circle on the LCD */
signed int Graphics_drawCircle (unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x, unsigned int y, unsigned int r) {
    // This function draws a circle of outline colour, centered at (x,y),
    // with radius r. If hasFill is specified, the circle will be filled
    // with fillColour.
    signed int xi, yi, dx, dy, Diff;

    // Sanity Checks//
    if (!LT24_isInitialised()) return LT24_ERRORNOINIT; //Don't run if not yet initialised
    //Ensure coordinates are in range
    if (x >= LT24_WIDTH || x + r >= LT24_WIDTH || x - r >= LT24_WIDTH)   return GRAPHICS_INVALIDCORD; //Invalid coordinates
    if (y >= LT24_HEIGHT || y + r >= LT24_HEIGHT || y - r >= LT24_HEIGHT) return GRAPHICS_INVALIDCORD; //Invalid coordinates
   
    // The circle is drawn based on the Midpoint algorithm, optimized for 
    // integer based computations. The objective is to find pixel points 
    // that come as close as possible to satisfying the equation of a 
    // circle that x^2 + y^2 = r^2
    
    // With the center at (x,y), it is certain that points (x+r,y), (x, y+r)
    // (x-r, y) and (x, y-r) are on the circle.
    
    // Starting at (x, y+r), the next point on the circle is
    // either at (x, y+r-1), or (x+1, y+r). We compute the difference between 
    // these two points to determine which is the next point on the circle.
    // If this difference is greater than zero, the next point is at 
    // (x, y+r-1), otherwise, it is at (x+1, y+r).
    
    // For every point on the circle, we compute the new difference to 
    // determine the next point whether to move on step in the
    // y direction or one step in the x direction
    xi = 0;         // start at x
    yi = r;        // start at y+r
    dx = 1;         
    dy = 1;
    // Left shift for multiplication by 2
    Diff = dy - (r << 1); // D = dy - 2*r

    // Since a circle is symmetrical, we split up the circle into 8 octants
    // We find the points on one octant and mirror the cordinates for the
    // remaining octants    
    // stop when we get to point (x+r/2, y+r/2)
    while (yi >= xi) {
        // draw pixels at the 8 octants //
        LT24_drawPixel(colour, x+xi, y+yi);     
        LT24_drawPixel(colour, x+xi, y-yi);
        LT24_drawPixel(colour, x-xi, y+yi);
        LT24_drawPixel(colour, x-xi, y-yi);
        LT24_drawPixel(colour, x+yi, y+xi);
        LT24_drawPixel(colour, x+yi, y-xi);
        LT24_drawPixel(colour, x-yi, y+xi);
        LT24_drawPixel(colour, x-yi, y-xi);
        // Check the difference value and act accordingly
        if (Diff > 0) {
            yi--;       // move one step in the y axis
            dy += 2;
            Diff += dy - (r << 1);  // recompute the difference
        } else {   
            xi++;       // move one step in the x axis
            Diff += dx; // recompute the difference    
            dx +=2;
        }
    }    
    // If the circle is to be filled, fill up with fillColour
    if (hasFill) Graphics_fillCircle (fillColour, x, y, r-1);
    return LT24_SUCCESS; // return success
}
// This functions fills the circle with the specified colour
signed int Graphics_fillCircle (unsigned short fillColour, unsigned int x, unsigned int y, unsigned int r) {
    // by drawing concentric circles.
    unsigned int i = 0;
    //draw concentric circles with radius r-1 to 1
    for (i = r; i != 0; i--) {
        Graphics_drawCircle (fillColour, false, 0, x, y, i);
    }
    LT24_drawPixel(fillColour, x, y);	// draw a pixel at the centre of the circle
    return LT24_SUCCESS;    // return success
}

/*Function to print text to the LCD*/
signed int Graphics_printText (unsigned short colour, unsigned int x, unsigned int y, char *text) {
    // This function prints a text (data) starting at point (x,y)
    // to the LCD. The function is able to handle over flow of text
    // to the next line.        
    unsigned int len, i, j, line1Max, numLines, nxtStart, nxtEnd;

    // Sanity Checks//
    if (!LT24_isInitialised()) return LT24_ERRORNOINIT; //Don't run if not yet initialised
    //Ensure coordinates are in range
    if (x >= GRAPHICS_MAXSTRINGWIDTH)   return GRAPHICS_INVALIDCORD; //Invalid size
    if (y >= GRAPHICS_MAXSTRINGHEIGHT) return GRAPHICS_INVALIDCORD; //Invalid size

    // Each character is 5 pixels wide and 8 pixels high (5 x 8)
    // Thus the LCD can only print text 48 characters wide and 40
    // characters high
    
    len = strlen(text);     // Get the length of the string
    // Get the maximum characters that can be printed on the first line
    // Based on the specified starting point
    line1Max = GRAPHICS_MAXSTRINGWIDTH - x;
    // Check if the string will fit on one line
    if (line1Max > len ) {
    // If the the string fits the first line, print every character
    // of the string.
        for (i = 0; i < len; i ++) {
            Graphics_printChar (colour, x+i, y, text [i]); 
        }
    // Otherwise, we break up the string 
    } else {
        // Determine the number of lines needed to print the string
        numLines = 1 + (len - line1Max)/GRAPHICS_MAXSTRINGWIDTH;

        // Print the first line
        for (i = 0; i < line1Max; i ++) {
            Graphics_printChar (colour, x+i, y, text [i]); 
        }
        // Print the other lines
        for (j = 0; j < numLines; j++) {
            // obtain the character at the start of the next line
            nxtStart = line1Max + j * GRAPHICS_MAXSTRINGWIDTH;
            
            // If this is the last line. Set the end point to the last
            // character of the string, otherwise, set to the maximum width
            if (numLines == j+1) nxtEnd = len;
            else nxtEnd = nxtStart + GRAPHICS_MAXSTRINGWIDTH;
            
            // Print each next line
            for (i = nxtStart; i < nxtEnd; i++) {                
                Graphics_printChar (colour, i-nxtStart, y+j+1, text [i]);
            }                        
        }
    }    
    return LT24_SUCCESS; // return success
}
// Function to print a single character on the LCD //
signed int Graphics_printChar (unsigned short colour, unsigned int x, unsigned int y, char data) {
    // This function uses the BasicFont bitmap
    // Each character is 5 pixels wide and 8 pixels high (5 x 8)
    // LCD can take 48 chars wide and 40 chars high
    unsigned int j;
    // Convert character coordinates to pixel coordinates
    unsigned int xp = x * GRAPHICS_CHARWIDTH;   
    unsigned int yp = y * GRAPHICS_CHARHEIGHT;
    // There are 5 colums and 8 rows.
    // For optimization, we unroll the loop for the row and instead
    // write the code to print each row
    // Read the character pixel map and begin printing
    for (j=0; j < 5; j++) {
        // Check every row of each coloumn and print if the value is set
        // We write the code to check each row instead of using a loop.
        // This saves the processor from checking loop conditions 8 times
        if (BF_fontMap[data-' '][j] & (1 << 0)) LT24_drawPixel (colour, xp+j, yp);
        if (BF_fontMap[data-' '][j] & (1 << 1)) LT24_drawPixel (colour, xp+j, yp+1);
        if (BF_fontMap[data-' '][j] & (1 << 2)) LT24_drawPixel (colour, xp+j, yp+2);
        if (BF_fontMap[data-' '][j] & (1 << 3)) LT24_drawPixel (colour, xp+j, yp+3);
        if (BF_fontMap[data-' '][j] & (1 << 4)) LT24_drawPixel (colour, xp+j, yp+4);
        if (BF_fontMap[data-' '][j] & (1 << 5)) LT24_drawPixel (colour, xp+j, yp+5);
        if (BF_fontMap[data-' '][j] & (1 << 6)) LT24_drawPixel (colour, xp+j, yp+6);
        if (BF_fontMap[data-' '][j] & (1 << 7)) LT24_drawPixel (colour, xp+j, yp+7);
    }
    return LT24_SUCCESS; // Return success
}

