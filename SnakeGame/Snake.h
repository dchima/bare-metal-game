/*
 * Snake.h
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

#ifndef SNAKE_H_
#define SNAKE_H_


/* Inclusions */

// Include the necessary drivers
#include "../DE1SoC_LT24/DE1SoC_LT24.h"
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_usleep/HPS_usleep.h"
#include "../WM8731_AudioEngine/WM8731_AudioEngine.h"
#include "../HPS_IRQ/HPS_IRQ.h"
#include "../HPS_Timers/HPS_Timers.h"
#include "../BasicFont/BasicFont.h"
#include "../GameMenu/GameMenu.h"

// Include standard libraries
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


//define the default snake length at start
//and the size of the food at start
#define SNAKE_LENGTH		50
#define FOOD_SIZE			4

/* External Global variables */
//These variables are declared in the source file and are visible/used/altered
//anywhere this h file is called
extern unsigned short snake_map[240][320];
extern unsigned short colours[8];
extern bool snake_initialised;
extern volatile unsigned int *BUTN;
extern bool snake_reset;
extern bool snake_isPlaying;
extern signed int head_xpose,head_ypose;
extern signed int tail_xpose, tail_ypose;
extern unsigned short snake_dir[8];
extern signed int food_xpose, food_ypose;
extern bool food_isSpawned;
extern bool food_isClear;
extern bool food_isEaten;

/* Function prototypes */

// Function to initialise the snake
void Snake_init(void);

// Function to update snake position on LCD and map
void Snake_update(unsigned short snakeColour, unsigned short backgroundColour);

// Function to create the snake in motion
void Snake_create(unsigned short snakeColour, unsigned short backgroundColour);

// Function to clear the snake in motion
void Snake_clear(unsigned short snakeColour, unsigned short backgroundColour);

// Function to grow the snake when a food is eaten
void Snake_grow(void);

// Function to change the snake direction
void Snake_direction(void);

// Debugging Function to observe the snake map *Not part of the game
void Snake_test(void);



#endif /* SNAKE_H_ */
