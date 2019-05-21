/*
 * Snake.c
 *
 *  Created on: 13 Mar 2018
 *      Author: Dan
 */


#include "Snake.h"
#include "../DE1SoC_LT24/DE1SoC_LT24.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_usleep/HPS_usleep.h"
#include "../PrivateTimer/PrivateTimer.h"
#include "../Graphics/Graphics.h"

//initialise peripheral buttons
volatile unsigned int *BUTN = (unsigned int *) 0xFF200050;

//initialise Driver
bool snake_initialised = false;
bool started = true;

/*Global variables*/
//these hold the positions of the snake head and tail
unsigned int start_xpose = 10,start_ypose = 300;
signed int head_xpose = 0,head_ypose = 0;
signed int tail_xpose = 0, tail_ypose = 0;

//this is the map of the snake environment
unsigned short snake_map[240][320];

//this holds the direction memory
//0 to 3 holds current direction while 4 to 7 holds
//previous direction before a direction change
unsigned short snake_dir[8] = {0,0,0,1,0,0,0,0};

//initialise snake with a start position and a
//pixel length of 10.
//Remember to update the snakes memory map position

/*
 * later on, the Game_init will handle initialising the LCD, while this
 * function only cares about the snake body and will take in
 * just the colour of the snake it creates.
 * -- Snake_init(unsigned short snakeColour);--
 */

signed int Snake_init(unsigned int lcd_pio_base, unsigned int lcd_hw_base){

	unsigned int i = 0;
	if(LT24_isInitialised() == false){
		LT24_initialise(0xFF200060,0xFF200080);
	}

	LT24_clearDisplay(LT24_BLACK);

	for(i=0;i<(SNAKE_LENGTH);i++){
		//create a snake with 10 pixel length and update snake memory map
		snake_map[start_xpose+i][start_ypose] = 1;
		LT24_drawPixel(LT24_RED,start_xpose+i,start_ypose);
	}

	//once the game starts, we get the coordinates
	//of the snakes head and tail.
	if(started){
		head_xpose = start_xpose + (SNAKE_LENGTH-1);
		head_ypose = start_ypose;
		tail_xpose = start_xpose;
		tail_ypose = start_ypose;

		//initial positions set, so we can turn this off
		started = false;
	}

	snake_initialised = true;

	return SNAKE_SUCCESS;
}


bool Snake_isinit(){
	return snake_initialised;
}


//updates state of the snake in the environment
void Snake_update(unsigned short snakeColour, unsigned short backgroundColour){
	//create moving snake
	LT24_drawPixel(snakeColour,head_xpose,head_ypose);
	//update snake memory map
	snake_map[head_xpose][head_ypose] = 1;

	//clear snake tail
	LT24_drawPixel(backgroundColour,tail_xpose,tail_ypose);
	//update snake memory map
	snake_map[tail_xpose][tail_ypose] = 0;
}


//This function moves the head of the snake forward
//it should be able also increase the length of the snake
signed int Snake_create(unsigned short snakeColour, unsigned short backgroundColour){

	if(!Snake_isinit()) return SNAKE_ERRORNOINIT;


	//check end of screen
	if(head_xpose == 240) head_xpose = 0;
	if(head_xpose < 0) head_xpose = 239;
	if(head_ypose == 320) head_ypose = 0;
	if(head_ypose < 0) head_ypose = 319;

	//change snake direction
	//the &&(head_xpose%2 != 0) makes sure the snake only
	//turns on an even pixel track
	if(snake_dir[0]){
		if(snake_dir[6] && (head_xpose%2) != 0) head_xpose--;
		else if(snake_dir[7] && (head_xpose%2) != 0) head_xpose++;
		else head_ypose--;
	}
	else if(snake_dir[1]){
		if(snake_dir[6] && (head_xpose%2) != 0) head_xpose--;
		else if(snake_dir[7] && (head_xpose%2) != 0) head_xpose++;
		else head_ypose++;
	}
	else if(snake_dir[2]){
		if(snake_dir[4] && (head_xpose%2) != 0) head_ypose--;
		else if(snake_dir[5] && (head_xpose%2) != 0) head_ypose++;
		else head_xpose--;
	}
	else if(snake_dir[3]){
		if(snake_dir[4] && (head_xpose%2) != 0) head_ypose--;
		else if(snake_dir[5] && (head_xpose%2) != 0) head_ypose++;
		else head_xpose++;
	}

	//update snake state in environment
	Snake_update(snakeColour,backgroundColour);

	return SNAKE_SUCCESS;
}


//This function clears the tail of the moving snake
signed int Snake_clear(unsigned short snakeColour, unsigned short backgroundColour){

	if(!Snake_isinit()) return SNAKE_ERRORNOINIT;



	//update snake state in environment
	Snake_update(snakeColour,backgroundColour);


	//check end of screen
	//check for the snakes last known tail position
	//and move towards that point
	if(tail_xpose > -1 && tail_ypose > -1 && tail_xpose < 240 && tail_ypose < 320){
		//this corrects a positioning error in the map i didn't understand
		if(tail_ypose == 319) snake_map[tail_xpose-1][319] = 0;
		if(tail_ypose == 0) snake_map[tail_xpose+1][0] = 0;

		//Allows the function follow the snake trail on the map
		if(snake_map[tail_xpose+1][tail_ypose] == 1) tail_xpose++;
		else if(snake_map[tail_xpose-1][tail_ypose] == 1) tail_xpose--;
		else if(snake_map[tail_xpose][tail_ypose+1] == 1) tail_ypose++;
		else if(snake_map[tail_xpose][tail_ypose-1] == 1) tail_ypose--;
	}

	//correct for edge looping
	if(tail_xpose == 240) tail_xpose = 0;
	if(tail_xpose == -1) tail_xpose = 239;
	if(tail_ypose == 320) tail_ypose = 0;
	if(tail_ypose == -1) tail_ypose = 319;




	return SNAKE_SUCCESS;
}

void Snake_direction(void){

	unsigned int i=0;

	if(*BUTN & 0xF){
		//save previous direction for state check
		for (i=0;i<4;i++){
			snake_dir[i+4] = snake_dir[i];
		}
	}

	if(*BUTN & 0x8){
		//snake is moving up
		if(snake_dir[1]){
			//but not when it's moving down
			snake_dir[0] = 0;
		}else{
			snake_dir[0] = 1;snake_dir[1] = 0;
			snake_dir[2] = 0;snake_dir[3] = 0;
		}
	}
	if(*BUTN & 0x4){
		//snake is moving down
		if(snake_dir[0]){
			//but not when it's moving up
			snake_dir[1] = 0;
		}else{
			snake_dir[0] = 0;snake_dir[1] = 1;
			snake_dir[2] = 0;snake_dir[3] = 0;
		}
	}
	if(*BUTN & 0x2){
		//snake is moving left
		if(snake_dir[3]){
			//but not when it's moving right
			snake_dir[2] = 0;
		}else{
			snake_dir[0] = 0;snake_dir[1] = 0;
			snake_dir[2] = 1;snake_dir[3] = 0;
		}
	}
	if(*BUTN & 0x1){
		//snake is moving right
		if(snake_dir[2]){
			//but not when moving left
			snake_dir[3] = 0;
		}else{
			snake_dir[0] = 0;snake_dir[1] = 0;
			snake_dir[2] = 0;snake_dir[3] = 1;
		}
	}

}

//I couldn't see what was going on in the map
//so I made this function to print a pseudo map
//for me
void Snake_test(void){
	signed int h1, h2, h3, h4;
	signed int t1, t2, t3, t4;
	h1 = head_xpose - 1; t1 = tail_xpose - 1;
	h2 = head_xpose + 1; t2 = tail_xpose + 1;
	h3 = head_ypose - 1; t3 = tail_ypose - 1;
	h4 = head_ypose + 1; t4 = tail_ypose + 1;

	printf("Head Position: \n ------------ \n");
	printf("x: %d, y: %d \n", head_xpose, head_ypose);
	printf("%d   %d  %d  \n", snake_map[h1][h3],snake_map[head_xpose][h3],snake_map[h2][h3]);
	printf("%d   %d  %d  \n", snake_map[h1][head_ypose],snake_map[head_xpose][head_ypose],snake_map[h2][head_ypose]);
	printf("%d   %d  %d   \n", snake_map[h1][h4],snake_map[head_xpose][h4],snake_map[h2][h4]);
	printf("------------\n \n");
	printf("Tail Position: \n ------------ \n");
	printf("x: %d, y: %d \n", tail_xpose, tail_ypose);
	printf("%d   %d  %d  \n", snake_map[t1][t3],snake_map[tail_xpose][t3],snake_map[t2][t3]);
	printf("%d   %d  %d  \n", snake_map[t1][tail_ypose],snake_map[tail_xpose][tail_ypose],snake_map[t2][tail_ypose]);
	printf("%d   %d  %d   \n", snake_map[t1][t4],snake_map[tail_xpose][t4],snake_map[t2][t4]);



}

