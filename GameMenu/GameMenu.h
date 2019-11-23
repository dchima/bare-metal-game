/*
 * GameMenu.h
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This file contains functions that run the game main menu scene
 *
 */

#ifndef GAMEMENU_H_
#define GAMEMENU_H_

/* Inclusions */

// Include the necessary drivers
#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../HPS_Timers/HPS_Timers.h"                   // Timer driver for private timer and timer 0
#include "../BasicFont/BasicFont.h"
#include "../LT24_GraphicsEngine/LT24_GraphicsEngine.h" 
#include "../IO_Peripherals/IO_Peripherals.h"             // Functions for KEY interrupt
#include "../HPS_IRQ/HPS_IRQ.h"
#include "../HPS_usleep/HPS_usleep.h"

// Include standard libraries
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Include images
#include "images/menu_title.h"
#include "images/menu_lSnake.h"
#include "images/menu_lAvengers.h"
#include "images/menu_tSnake.h"
#include "images/menu_tAvengers.h"

// Define Macros to indicate game selection
#define HEROGAME       1
#define SNAKEGAME      2

// External Globals
extern unsigned int game_selection;
extern unsigned int game_selected;
extern bool newMenu;

/* Function prototypes */

// Wrapper Function to run the game menu scene
void Game_Menu(void);

// Function to play the game manu scene
void Game_menuScene (void);

// Function to select a game
void Game_selectCursor (void);

// Function to navigate through characters
void Game_selectNav (void);


#endif /* GAMEMENU_H_ */
