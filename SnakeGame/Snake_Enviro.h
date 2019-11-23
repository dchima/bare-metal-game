/*
 * Snake_Enviro.h
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This script contains functions used to setup and drive the environment of an
 * animated snake. It is uses snake.h
 *
 */

#ifndef SNAKE_Enviro_H_
#define SNAKE_Enviro_H_

/* Inclusions */

//inherit snake functions and map
#include "Snake.h"
#include "Snake_Graphics.h"

//define environment limits
#define X_MAX			222
#define X_MIN			14
#define Y_MAX			302
#define Y_MIN			14

/* External Global variables */
//variables for execution timing
extern unsigned int snake_lastTime[2];
extern unsigned int snake_periods[2];
extern unsigned int snake_currentTime;

extern bool snake_newGame;

// Typedef functions for timed executions
typedef void (*s_func)(void); //no inputs

/* Function prototypes */

// Wrapper Function to run the snake game 
void Snake_Game(void);

// Function to run the snake game
void Snake_Enviro(void);

// Function to set a snake in the environment
void Snake_dynamics(void);

// Function to destroy the snake when it hits itself or a wall
void Snake_destruct(void);

// Function to spawn the food
void Snake_food(void);

//these two functions get random x and y coordinates
void get_y(void);
void get_x(void);


//timer execution functions
void snake_execute(s_func p);
void food_execute(s_func p);


#endif /* SNAKE_Enviro_H_ */


