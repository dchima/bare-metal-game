/*
 * Snake.c
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This script contains functions used to setup and drive the dynamics of
 * a snake animation for the game 'Snake'. It is a child script of the
 * Snake Environment Snake_Enviro.
 */

#include "Snake.h"

/* External Global variables */

//initialise peripheral buttons
volatile unsigned int *BUTN = (unsigned int *) 0xFF200050;

bool snake_initialised = false;
bool started = true;

//these hold the positions of the snake head and tail
unsigned int start_xpose = 10,start_ypose = 50; //default start positions
signed int head_xpose = 0,head_ypose = 0;
signed int tail_xpose = 0, tail_ypose = 0;
signed int food_xpose = 0, food_ypose = 0;

//this is the map of the snake environment
unsigned short snake_map[240][320];
unsigned short colours[8] = {LT24_RED,LT24_YELLOW,LT24_GREEN,LT24_CYAN,LT24_WHITE,LT24_BLUE,LT24_MAGENTA,LT24_BLACK};
//boolean to know when the food has been eaten.
//this is useful to tell the program when to spawn
//new food
bool food_isEaten = false;
//boolean to check if food has been spawned
bool food_isSpawned = false;
//boolean to let us know field is clear
bool food_isClear = true;

// these booleans will let us know when the snake
// hits a wall or it self
bool snake_reset = false;
bool snake_isPlaying = false;

//this holds the direction memory
//0 to 3 holds current direction while 4 to 7 holds
//previous direction before a direction change
unsigned short snake_dir[8] = {0,0,0,1,0,0,0,0};

/* Function to initialise the snake */
void Snake_init(void){
    //This function cares about the snake body and
    //initialises it on the screen.
    //It also resets the LCD and snake map whwnever it is called
    //and places the snake in the default start position with
    //the default snake length.
	if(!snake_initialised){
		unsigned int i = 0, j = 0;
		LT24_clearDisplay(colours[7]);

		//clear and reset snake map
		for (i = 0; i < 240; i++ ) {
			for (j = 0; j < 320; j++) {
				snake_map [i][j] = 0;
			}
		}
		//reset snake default direction
		snake_dir[0] = 0;snake_dir[1] = 0;
		snake_dir[2] = 0;snake_dir[3] = 1;
		snake_dir[4] = 0;snake_dir[5] = 0;
		snake_dir[6] = 0;snake_dir[7] = 0;

		//game i set to start, it is time to make
		//a snake in the environment
		started = true;

		for(i=0;i<(SNAKE_LENGTH);i++){
			//create a snake with 10 pixel length and update snake memory map
			snake_map[start_xpose+i][start_ypose] = 1;
			LT24_drawPixel(LT24_YELLOW,start_xpose+i,start_ypose);
		}

		//once the game starts, we get the coordinates
		//of the snakes head and tail.
		if(started){
			head_xpose = start_xpose + (SNAKE_LENGTH-1);
			head_ypose = start_ypose;
			tail_xpose = start_xpose;
			tail_ypose = start_ypose;

			//initial positions of set, so we can turn this off
			started = false;
		}

		snake_initialised = true; //snake created
	}

}

/* Function to update snake position on LCD and map */
void Snake_update(unsigned short snakeColour, unsigned short backgroundColour){
    //updates state of the snake in the environment
	//create moving snake
	LT24_drawPixel(snakeColour,head_xpose,head_ypose);
	//update snake memory map
	snake_map[head_xpose][head_ypose] = 1;
	//clear snake tail
	LT24_drawPixel(backgroundColour,tail_xpose,tail_ypose);
	//update snake memory map
	snake_map[tail_xpose][tail_ypose] = 0;
}

/* Function to create the snake in motion */
void Snake_create(unsigned short snakeColour, unsigned short backgroundColour){

    //This function moves the head of the snake forward

	//check end of screen
	if(head_xpose == 240) head_xpose = 0;
	if(head_xpose < 0) head_xpose = 239;
	if(head_ypose == 320) head_ypose = 0;
	if(head_ypose < 0) head_ypose = 319;

	//change snake direction
	//the &&(head_xpose%2 != 0) makes sure the snake only
	//turns on an even pixel track. This is for error checking purposes
	//and to make sure the snake is always visible on the track
	// (it never touches itself except at the head)
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

}

/* Function to clear the snake in motion */
void Snake_clear(unsigned short snakeColour, unsigned short backgroundColour){
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

}

/* Function to grow the snake when a food is eaten */
void Snake_grow(void){

	unsigned int i = 0, j = 0;

	//if it sees food ahead of it, reset the food pixels
	//The code runs four times to accommodate for the four directions
	//the snake is moving as it finds a food to eat

	//snake is moving up and sees food
	if(snake_dir[0] && snake_map[head_xpose][head_ypose-2] == 10){

		for(i = food_ypose; i<food_ypose+FOOD_SIZE; i++){
			for(j = food_xpose; j< food_xpose+FOOD_SIZE; j++){
				LT24_drawPixel(colours[7],j,i);
				snake_map[j][i] = 0;
			}
		}

		//grow the snake
		for(i=head_ypose; i>head_ypose-10; i--){
			LT24_drawPixel(colours[1],head_xpose,i);
			snake_map[head_xpose][i] = 1;
		}
		head_ypose = head_ypose - 9;
		food_isSpawned = false;
	}
	//snake is moving down and sees food
	else if(snake_dir[1] && snake_map[head_xpose][head_ypose+2] == 10){

		for(i = food_ypose; i<food_ypose+FOOD_SIZE; i++){
			for(j = food_xpose; j< food_xpose+FOOD_SIZE; j++){
				LT24_drawPixel(colours[7],j,i);
				snake_map[j][i] = 0;
			}
		}

		//grow the snake in this direction
		for(i=head_ypose; i<head_ypose+10; i++){
			LT24_drawPixel(colours[1],head_xpose,i);
			snake_map[head_xpose][i] = 1;
		}
		head_ypose = head_ypose + 9;
		food_isSpawned = false;
	}
	//snake is moving left and sees food
	else if(snake_dir[2] && snake_map[head_xpose-2][head_ypose] == 10){

		for(i = food_ypose; i<food_ypose+FOOD_SIZE; i++){
			for(j = food_xpose; j< food_xpose+FOOD_SIZE; j++){
				LT24_drawPixel(colours[7],j,i);
				snake_map[j][i] = 0;
			}
		}

		for(i=head_xpose; i>head_xpose-10; i--){
			LT24_drawPixel(colours[1],i,head_ypose);
			snake_map[i][head_ypose] = 1;
		}
		head_xpose = head_xpose - 9;
		food_isSpawned = false;
	}
	//snake is moving right and sees food
	else if(snake_dir[3] && snake_map[head_xpose+2][head_ypose] == 10){

		for(i = food_ypose; i<food_ypose+FOOD_SIZE; i++){
			for(j = food_xpose; j< food_xpose+FOOD_SIZE; j++){
				LT24_drawPixel(colours[7],j,i);
				snake_map[j][i] = 0;
			}
		}

		for(i=head_xpose; i<head_xpose+10; i++){
			LT24_drawPixel(colours[1],i,head_ypose);
			snake_map[i][head_ypose] = 1;
		}
		head_xpose = head_xpose + 9;
		food_isSpawned = false;
	}
}

/* Function to change the snake direction */
void Snake_direction(void){
    //This function runs all the time to keep track of the button
    //pressed to change direction of the snake.
    //The snake uses its current direction to then know how its moving
    //The function also saves the previous direction for error purposes in motion
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

/* Debugging Function to observe the snake map */
void Snake_test(void){
    // This function prints out the snake map in the console
    // It is not part of the game
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
