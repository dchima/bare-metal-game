/*
 * Snake.h
 *
 *  Created on: 13 Mar 2018
 *      Author: Dan
 */

#ifndef SNAKE_H_
#define SNAKE_H_





#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define SNAKE_SUCCESS		0
#define SNAKE_ERRORNOINIT	-1


#define SNAKE_LENGTH		50

//initialise snake engine
signed int Snake_init(unsigned int lcd_pio_base, unsigned int lcd_hw_base);

//check if snake has been initialised
bool Snake_isinit(void);

//this function updates snake position on LCD and map
void Snake_update(unsigned short snakeColour, unsigned short backgroundColour);

//this function creates the snake in motion
signed int Snake_create(unsigned short snakeColour, unsigned short backgroundColour);


//this function clears the snake in motion
signed int Snake_clear(unsigned short snakeColour, unsigned short backgroundColour);


void Snake_direction(void);

void Snake_test(void);



#endif /* SNAKE_H_ */
