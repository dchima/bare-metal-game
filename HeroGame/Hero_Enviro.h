/*
 * Hero_Enviro.h
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This file contains functions that setup the environment
 * for the avengers game and synchronize the game play. 
 * It uses functions from hero.h and inherits its external variables.
 *
 */
 
#ifndef HERO_ENVIRO_H_
#define HERO_ENVIRO_H_

/* Inclusions */

#include "Hero.h"

// Include the launch scene images
#include "Images/avengers_Limage0.h"
#include "Images/avengers_Limage1.h"
#include "Images/avengers_Limage2.h"
#include "Images/avengers_Limage3.h"
#include "Images/avengers_Limage4.h"
#include "Images/avengers_Limage5.h"
#include "Images/avengers_Limage6.h"

// Include hero images for selection scene
#include "images/avengers_sLogo.h"
#include "images/avengers_sTitle.h"
#include "images/avengers_sCap.h"
#include "images/avengers_sIron.h"
#include "images/avengers_sPanther.h"
#include "images/avengers_sThor.h"
#include "images/avengers_sScarlet.h"
#include "images/avengers_sWidow.h"

// Include hero title images
#include "images/avengers_sCapText.h"
#include "images/avengers_sIronText.h"
#include "images/avengers_sPantherText.h"
#include "images/avengers_sThorText.h"
#include "images/avengers_sScarletText.h"
#include "images/avengers_sWidowText.h"

/* Function Prototypes */

// Wrapper function to run the Avengers game
void Hero_game(void);

// Function to run the avengers game
void Hero_inGame(void);

// Function to play the game launch scene
void Hero_launchScene (void);

// Function to play the game hero selection scene
void Hero_selectScene (void);

// Function to select game character (hero)
void Hero_selectCursor (void);

// Function to navigate through characters
void Hero_selectNav (void);


#endif /* HERO_ENVIRO_H_ */
