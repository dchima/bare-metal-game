/*
 * LT24 Graphics Driver (source file)
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



#include "Graphics.h"
#include "../BasicFont/BasicFont.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../DE1Soc_LT24/DE1Soc_LT24.h"
#include "../HPS_usleep/HPS_usleep.h" //some useful delay routines





/*
 * The LT24 LCD needs to be initialised for the driver to work.
 * This function checks if the LT24 LCD has been initialised, and
 * Initialises it if not.
 */
signed int Graphics_initialise(unsigned int lcd_pio_base, unsigned int lcd_hw_base)
{
	if(LT24_isInitialised() == false){
		LT24_initialise(0xFF200060,0xFF200080);
	}
	return GRAPHICS_SUCCESS;
}



/*
 * This function plots the pixels at x,y coordinates corresponding to a line
 * from x1,y1 to x2,y2 using a simple geometric line equation 
 * 'change in y = slope*change in x'
 */
signed int Graphics_drawLine(unsigned short colour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2)
{
	double slope = 0.0;
	double xin = 0, yin = 0;
	//get change in x and y
	signed int rx = (x2 - x1);
	signed int ry = (y2 - y1);

	//do not run function if LCD is not initialised
	if (!LT24_isInitialised()) return LT24_ERRORNOINIT;
	if ((x1 >= LT24_WIDTH) || (x2 >= LT24_WIDTH) ||(y1 >= LT24_HEIGHT) || (y2 >= LT24_HEIGHT)) return LT24_INVALIDSIZE;
	


	if(y2<y1 || x2<x1 ){
		//Check when there's a negative y range at slope
		if(y2 < y1 && (x2 > x1 || x2 == x1)){
			ry = - (double) ry;
			//get slope
			slope = - (((double)ry)/((double)rx));
			//x range is zero, plot and increment along y
			if(isinf(slope) == 1){ 
				xin = x1;
				for(yin = y2; yin < y1; yin++){
					LT24_drawPixel(colour,xin,yin);
				}
			}
			else{
				//plot the line by incrementing the axis with most points on line
				if(abs(ry) > abs(rx)){
					for(yin = y2; yin < y1; yin++){
						xin = ((yin - y1)/slope) + x1;
						LT24_drawPixel(colour,xin,yin);
					}
				}else{
					for(xin = x1; xin < x2; xin++){
						yin = (slope*(xin - x1)) + y1;
						LT24_drawPixel(colour,xin,yin);
					}
				}
			}
		}
		//Check when there's a negative x range at slope
		if(x2 < x1 && (y2 > y1 || y2 == y1)){
			rx = - (double) rx;
			//get slope
			slope = - (((double)ry)/((double)rx));
			//plot the line by incrementing the axis with most points on line
			if(abs(ry) > abs(rx)){
				for(yin = y1; yin < y2; yin++){
					xin = ((yin - y1)/slope) + x1;
					LT24_drawPixel(colour,xin,yin);
				}
			}else{
				for(xin = x2; xin < x1; xin++){
					yin = (slope*(xin - x1)) + y1;
					LT24_drawPixel(colour,xin,yin);
				}
			}
		}
		//Plot line when both ranges are negative in slope
		if(x2 < x1 && y2 < y1){
			//get slope
			slope = (((double)ry)/((double)rx));
			//plot the line by incrementing the axis with most points on line
			if(abs(ry) > abs(rx)){
				for(yin = y2; yin < y1; yin++){
					xin = ((yin - y1)/slope) + x1;
					LT24_drawPixel(colour,xin,yin);
				}
			}else{
				for(xin = x2; xin < x1; xin++){
					yin = (slope*(xin - x1)) + y1;
					LT24_drawPixel(colour,xin,yin);
				}
			}
		}
	}else{
		//get slope of normal line with positive slope
		slope = (((double)ry)/((double)rx));
		//x range is zero, plot and increment along y
		if(isinf(slope) == 1){ 
			xin = x1;
			for(yin = y1; yin < y2; yin++){
				LT24_drawPixel(colour,xin,yin);
			}
		}
		else{
			//plot the line by incrementing the axis with most points on line
			if(abs(ry) > abs(rx)){
				for(yin = y1; yin < y2; yin++){
					xin = ((yin - y1)/slope) + x1;
					LT24_drawPixel(colour,xin,yin);
				}
			}else{
				for(xin = x1; xin < x2; xin++){
					yin = (slope*(xin - x1)) + y1;
					LT24_drawPixel(colour,xin,yin);
				}
			}

		}
	}
	return GRAPHICS_SUCCESS;
}


/*
 * This function utilises the drawLine function to draw a box on the LCD.
 * The function takes in the boarder colour, coordinates of two points signifying
 * the height and width. The function also takes in a boolean and a fill colour
 * When the boolean is 1, the box is filled up with the specified colour.
 * 
 */
signed int Graphics_drawBox(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2){

	signed int yin = 0;
	//do not run function if LCD is not initialised
	if (!LT24_isInitialised()) return LT24_ERRORNOINIT;
	//check for field overflow
	if (x1 >= LT24_WIDTH || x2 >= LT24_WIDTH ||y1 >= LT24_HEIGHT || y2 >= LT24_HEIGHT) return LT24_INVALIDSIZE;
	if (x1 == x2 || y1 == y2) return GRAPHICS_INVALIDSHAPE;

	//Draw box boarder using drawLine function
	Graphics_drawLine(colour,x1,y1,x1,y2);
	Graphics_drawLine(colour,x1,y2,x2,y2);
	Graphics_drawLine(colour,x2,y2,x2,y1);
	Graphics_drawLine(colour,x2,y1,x1,y1);

	if(hasFill){
		if(x2<x1){ //offset width to retain boarder
			x2+=1;
		}else{
			x1+=1;
		}
		//draw box even if the coordinates are reversed
		if(y2<y1){ 
			y2+=1; //offset height to retain boarder
			for(yin = y2; yin < y1; yin++){
				//fill box with lines
				Graphics_drawLine(fillColour,x1,yin,x2,yin);
			}
		}else{
			y1+=1; //offset height to retain boarder
			for(yin = y1; yin < y2; yin++){
				//fill box with lines
				Graphics_drawLine(fillColour,x1,yin,x2,yin);
			}
		}

	}
	return GRAPHICS_SUCCESS;
}


/*
 * This function utilises the drawLine function to draw a triangle on the LCD.
 * The function inputs the boarder colour, coordinates of 3 points for the 
 * triangle, a boolean and a fill colour.
 * When the boolean is 1, the triangle is filled up with the specified fill colour
 */
signed int Graphics_drawTriangle(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, unsigned int x3, unsigned int y3){

	signed int rx1 = 0, ry1 = 0, rx2 = 0, ry2 = 0, rx3 = 0, ry3 = 0;
	double s1 = 0, s2 = 0, s3 = 0;
	double yina = 0, yinb = 0, xin = 0;
	signed int xmax = 0,xmin = 0;

	//do not run function if LCD is not initialised
	if (!LT24_isInitialised()) return LT24_ERRORNOINIT;

	//check for field overflow
	if ((x1 >= LT24_WIDTH) || (x2 >= LT24_WIDTH) || (x3 >= LT24_WIDTH))   return LT24_INVALIDSIZE;
	if ((y1 >= LT24_HEIGHT) || (y2 >= LT24_HEIGHT) || (y3 >= LT24_HEIGHT)) return LT24_INVALIDSIZE;
	if ((y1==y2 && y2==y3)||(x1==x2 && x2==x3)) return GRAPHICS_INVALIDSHAPE;

	//Draw triangle boarder using drawLine function
	Graphics_drawLine(colour,x1,y1,x2,y2);
	Graphics_drawLine(colour,x2,y2,x3,y3);
	Graphics_drawLine(colour,x3,y3,x1,y1);

	if(hasFill){
		//Get ranges of each line of the triangle
		ry1 = (y2 - y1);
		rx1 = (x2-x1);
		ry2 = (y3 - y2);
		rx2 = (x3-x2);
		ry3 = (y3 - y1);
		rx3 = (x3-x1);

		//Calculate the slope of each line in the triangle.
		s1 = ((double)ry1)/((double)rx1);
		s2 = ((double)ry2)/((double)rx2);
		s3 = ((double)ry3)/((double)rx3);


		//Get minimum and maximum x points of triangle.
		//This lets us know the longest line of the triangle
		xmin = x1;
		xmax = x1;
		if((x2<x1)&&(x2<=x3)) xmin = x2;
		else if((x3<x1)&&(x3<=x2)) xmin = x3;
		if((x2>=x3)&&(x2>x1)) xmax = x2;
		else if((x3>=x2)&&(x3>x1)) xmax = x3;

		/*
		 * The triangle algorithm works under the assumption that after finding
		 * the longest line between two points of the triangle, the third point
		 * of the triangle lies either in-between these points or at the same x points
		 * of either of the two max and min points. 
		 * 
		 * Therefore, only specific conditions have to be met to fill the triangle. 
		 * The slope of the largest line remains constant, and for every x point at
		 * that slope, there is a corresponding x points at the slope of either line
		 * from the third (middle) point to the maximum or to the minimum points.
		 * 
		 * This divides the triangle to two parts at the third point, and fills up the
		 * triangle by using the drawLine function to draw a line from the left line slope
		 * to the maximum slope (by finding the corresponding y points for each x increment),
		 * and doing the same for the right line slope and the maximum line slope.
		 * 
		 * When the third x points equal either the maximum or minimum x points, then we do not
		 * have to break the triangle into two, as we simply match the slopes of the two
		 * available lines (to the left or right) and find their y coordinates 
		 * for each incrementing x coordinate. 
		 * 
		 */

		//if two x points are equal, then we only need to
		//find the y coordinates of two slopes as we increment x
		if(x1==x2){ 
			for(xin=(xmin+1); xin<xmax;xin++){
				yina =(s2*(xin-x2))+y2;
				yinb =(s3*(xin-x1))+y1;
				//offset of line plot to fill triangle within boarder
				//if point is above largest line, we flip the increment/decrement of y
				if(y1<y2) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
				else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
			}
		}
		else if(x1==x3){
			for(xin=(xmin+1); xin<xmax;xin++){
				yina =(s2*(xin-x2))+y2;
				yinb =(s1*(xin-x1)) +y1;
				//offset of line plot to fill triangle within boarder	
				//if point is above largest line, we flip the increment/decrement of y			
				if(y1<y3) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
				else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
			}
		}
		else if(x2==x3){
			for(xin=(xmin+1); xin<xmax;xin++){
				yina =(s1*(xin-x1)) +y1;
				yinb =(s3*(xin-x1))+y1;
				//fill triangle within boarder with line plot
				//if point is above largest line, we flip the increment/decrement of y
				if(y2<y3) Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
				else Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
			}
		}

		//For third point in triangle at x1, if it is greater than the minimum x and larger than the maximum x,
		//we split the triangle into two triangles (One to the left of the third point and the other to the right).
		//These two triangles are then filled by finding y coordinates of both triangles as x increments, and drawing
		//a line between them. 
		else if((xmin<x1)&&(xmax>x1)){
			for(xin = (xmin+1); xin<xmax;xin++){
				//for left triangle before the  x1
				if(xin<x1){
					yina =(s2*(xin-x2))+y2;
					if(xmin==x2){
						yinb =(s1*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y1<y2) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}else{
						yinb =(s3*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y1<y3) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}
				//for right triangle after the midpoint	x1
				}else{
					yina =(s2*(xin-xmin))+y2;
					if(xmin==x2){
						yinb =(s3*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y1<y3) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}else{
						yinb =(s1*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y1<y2) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}
				}
			}
		}
		
		//The previous if statement is simply repeated for x2 and x3 as the points
		//between the maximum and minimum x points.
		else if((xmin<x2)&&(xmax>x2)){
			for(xin = (xmin+1); xin<xmax;xin++){
				if(xin<x2){
					yina =(s3*(xin-x1))+y1;
					if(xmin==x1){
						yinb =(s1*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y2<y1) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}else{
						yinb =(s2*(xin-x2)) +y2;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y2<y3) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}
				}else{
					yina =(s3*(xin-x1))+y1;
					if(xmin==x1){
						yinb =(s2*(xin-x2)) +y2;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y2<y3) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}else{
						yinb =(s1*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y2<y1) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}
				}
			}
		}
		else if((xmin<x3)&&(xmax>x3)){
			for(xin=(xmin+1);xin<xmax;xin++){
				if(xin<x3){
					yina =(s1*(xin-x1))+y1;
					if(xmin==x2){
						yinb =(s2*(xin-x2)) +y2;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y3<y2) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}else{
						yinb =(s3*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y3<y1) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}
				}else{
					yina =(s1*(xin-x1))+y1;
					if(xmin==x2){
						yinb =(s3*(xin-x1)) +y1;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y3<y1) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}else{
						yinb =(s2*(xin-x2)) +y2;
						//fill triangle within boarder with line plot
						//if point is above largest line, we flip the increment/decrement of y
						if(y3<y2) Graphics_drawLine(fillColour,xin,yina-1,xin,yinb+1);
						else Graphics_drawLine(fillColour,xin,yina+1,xin,yinb-1);
					}
				}
			}
		}
	}

	return GRAPHICS_SUCCESS;
}



/*
 * The Draw Circle function uses the popular Mid Point circle algorithm, where 8
 * quadrants of the circle is plotted simultaneously, with an error to check that
 * the points being incremented to by the new x and y coordinates being
 * calculated are within the boundaries of the circle line.
 * 
 * To fill up the circle, the function recursively calls itself and reduces the 
 * radius by one pixel with each new function call. 
 */
signed int Graphics_drawCircle(unsigned short colour, bool hasFill, unsigned short fillColour, unsigned int x, unsigned int y, unsigned int r){

	signed int xin=0, yin=r;
	signed int rx=1, ry = 1;
	signed int err= ry - (2*r);
	unsigned int i;

	//do not run function if LCD is not initialised
	if (!LT24_isInitialised()) return LT24_ERRORNOINIT;
	//check for field overflow
	if ((x >= LT24_WIDTH) || (x + r >= LT24_WIDTH) || (x - r >= LT24_WIDTH))   return LT24_INVALIDSIZE;
	if ((y >= LT24_HEIGHT) || (y + r >= LT24_HEIGHT) || (y - r >= LT24_HEIGHT)) return LT24_INVALIDSIZE;

	 while (yin >= xin) {
		//Plot pixels symmetrically in 8 quadrants
		LT24_drawPixel(colour, x+xin, y+yin);
		LT24_drawPixel(colour, x+xin, y-yin);
		LT24_drawPixel(colour, x-xin, y+yin);
		LT24_drawPixel(colour, x-xin, y-yin);
		LT24_drawPixel(colour, x+yin, y+xin);
		LT24_drawPixel(colour, x+yin, y-xin);
		LT24_drawPixel(colour, x-yin, y+xin);
		LT24_drawPixel(colour, x-yin, y-xin);

		//check limits of circle points (if err > 0), point is not on circle
		//so we decrement the coordinate plot
		if (err > 0) {
			yin--;
			ry+=2;
			err+= ry-(2*r);
		} else {
			xin++;
			err+=rx;
			rx+=2;
		}
		}
	if(hasFill){

		for(i=(r-1);i!=0;i--){
			Graphics_drawCircle(fillColour,0, 0, x, y, i);
		}
		LT24_drawPixel(fillColour, x, y);
	}
	return GRAPHICS_SUCCESS;
}


/*
 * The text function utilises the Font Bitmap h file to turn on the configuration of pixels that
 * correspond to the input characters to the function.
 * The x and y coordinates placed into the function are actually divisions of the total pixels in
 * x,y axis (240x320) by the bitmap character x,y axis (5x8).
 *
 * This means the texts possible to fill the entire LCD has an x,y of 48x40 characters.
 */ 
signed int Graphics_text (unsigned short colour, unsigned int x, unsigned int y, const char *text) {
	//take the length of string input per character
    unsigned int textLength = strlen(text), i = 0,k = 0;

    //do not run function if LCD is not initialised
    if (!LT24_isInitialised()) return LT24_ERRORNOINIT;
    //Check if text field limits overflow
    if(x >= MAX_FIELDX || y >= MAX_FIELDY) return GRAPHICS_TEXTERROR;
    //for each character, print the pixel bitmap onto the LCD
    for(i = 0; i < textLength; i++){
		for(k = 0; k < 8; k++){
			//bitmap is 5x8 pixels, so we set the 5 pixels on the x axis 8 times
			//along the y axis
			if (BF_fontMap[text[i]-' '][0] & (1 << k)) LT24_drawPixel (colour, (5*(x+i)),(8*y)+k);
			if (BF_fontMap[text[i]-' '][1] & (1 << k)) LT24_drawPixel (colour, (5*(x+i)+1),(8*y)+k);
			if (BF_fontMap[text[i]-' '][2] & (1 << k)) LT24_drawPixel (colour, (5*(x+i)+2),(8*y)+k);
			if (BF_fontMap[text[i]-' '][3] & (1 << k)) LT24_drawPixel (colour, (5*(x+i)+3),(8*y)+k);
			if (BF_fontMap[text[i]-' '][4] & (1 << k)) LT24_drawPixel (colour, (5*(x+i)+4),(8*y)+k);
		}
    }
	return GRAPHICS_SUCCESS;
}

