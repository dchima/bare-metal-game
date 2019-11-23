/*
 * Hero.c
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
 * the hero player and enemy animations for the avengers game
 * It is a child script of Hero_Enviro.
 */

#ifndef HERO_H_
#define HERO_H_

/* Inclusions */

// Include the necessary drivers
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_usleep/HPS_usleep.h"
#include "../HPS_Timers/HPS_Timers.h"                   // Timer driver for private timer and timer 0
#include "../BasicFont/BasicFont.h"
#include "../FatFS/diskio.h"                                // File system functions to access SD card
#include "../FatFS/ff.h"                                 // File system functions to access SD card
#include "../LT24_GraphicsEngine/LT24_GraphicsEngine.h" 
#include "../IO_Peripherals/IO_Peripherals.h"             // Functions for KEY interrupt

// Include other header files
#include "../GameMenu/GameMenu.h"
#include "Audio.h"

// Include standard libraries
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Include hero images for game play
#include "images/avengers_pIron.h"
#include "images/avengers_pPanther.h"
#include "images/avengers_pCap.h"
#include "images/avengers_pWidow.h"
#include "images/avengers_pThor.h"
#include "images/avengers_pScarlet.h"

// Include enemy and bullet images
#include "images/avengers_IronShot.h"
#include "images/avengers_enemy1.h"

// Define Macros to indicate character selection
#define HERO_IRON         1
#define HERO_PANTHER      2
#define HERO_CAP          3
#define HERO_WIDOW        4
#define HERO_THOR         5
#define HERO_SCARLET      6

// Define maximum and minimum player position on the LCD
#define HERO_MAXLEFT      5
#define HERO_MAXRIGHT     185

/* External Global variables */

// Variables for execution timing
extern unsigned int hero_lastTime[5];
extern unsigned int hero_periods[5];
extern unsigned int hero_currentTime;

// Other external globals
extern bool hero_launch;
extern bool hero_dead;
extern unsigned int hero_selected;
extern unsigned int hero_selection;

// Typedef functions for timed executions
typedef void (*h_func)(void); //no inputs

/* Function Prototypes */

// Function to initialise the Hero game
void Hero_init(void);

// Function to move player left or right
void Hero_move(void);

// Function to update the button states
void Hero_dynamics(void);

// Function to activate a shot and move it
void Hero_shooter(void);

// Function to spawn and move enemy ship
void Hero_fleet(void);

// Function to keep track of player life
void Hero_life(void);

// Function to save highscores to SD card
void Hero_highScores (void);

// Function to print game over texts
void Hero_printOver (void);

// Function to play the game reset scene
void Hero_reset(void);

// Wrapper function for the player motion functions
void Hero_motion(void);

// Function to execute Hero_shooter function at specified intervals
void execute_hero(h_func p);

// Function to execute Hero_motion() function at specified intervals
void execute_shooter(h_func p);

// Function to execute fleet function at specified intervals
void execute_fleet(h_func p);

// Function to execute Hero_life() function at specified intervals
void execute_life(h_func p);
#endif /* HERO_H_ */
