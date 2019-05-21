/*
 * AvengersEnv.h
 *
 *  Created on: Mar 21, 2018
 *      Author: Azeem
 */

#ifndef AVENGERSENV_H_
#define AVENGERSENV_H_



#include "../HPS_Watchdog/HPS_Watchdog.h"
#include "../WM8731_AudioEngine/WM8731_AudioEngine.h"
#include "../LT24_GraphicsEngine/LT24_GraphicsEngine.h"

#include "../HPS_IRQ/HPS_IRQ.h"
#include "../HPS_Timers/HPS_Timers.h"

#include <math.h>


/* Audio Files*/
#include "Sounds/avengers_Audio1.h"         // Launch Audio
#include "Sounds/avengers_Audio2.h"         // Back ground music

/* Launch Scene Images */
#include "Images/avengers_Limage0.h"
#include "Images/avengers_Limage1.h"
#include "Images/avengers_Limage2.h"
#include "Images/avengers_Limage3.h"
#include "Images/avengers_Limage4.h"
#include "Images/avengers_Limage5.h"
#include "Images/avengers_Limage6.h"

/* Character Images for selection scene */
#include "images/avengers_sLogo.h"
#include "images/avengers_sTitle.h"
#include "images/avengers_sCap.h"
#include "images/avengers_sIron.h"
#include "images/avengers_sPanther.h"
#include "images/avengers_sThor.h"
#include "images/avengers_sScarlet.h"
#include "images/avengers_sWidow.h"

// Character Names
#include "images/avengers_sCapText.h"
#include "images/avengers_sIronText.h"
#include "images/avengers_sPantherText.h"
#include "images/avengers_sThorText.h"
#include "images/avengers_sScarletText.h"
#include "images/avengers_sWidowText.h"


/* Global Variables */

extern bool avengers_exit;          

extern unsigned int avengers_selector;      
extern unsigned int avengers_selection;     


extern unsigned int avengers_audSamp;       
extern unsigned int avengers_totAudSamp;    

//Error Codes
#define AVENGERS_SUCCESS      0 

// Macros to indicate selection
#define AVENGERS_IRON         1
#define AVENGERS_PANTHER      2
#define AVENGERS_CAP          3
#define AVENGERS_WIDOW        4
#define AVENGERS_THOR         5
#define AVENGERS_SCARLET      6


/* Functions */

// plays the launch scene
void Avengers_launchScene (void);
// Plays the launch audio - Is attached to timer interrupt
void Avengers_launchAudio (void);

// Plays background audio - Is attached to timer interrupt
void Avengers_backAudio (void);

// Plays the character selection scene
void Avengers_selectScene (void);

// Displays the character images in the selection scene
void Avengers_selectCursor (void);

// Navigates through characters - attached to key interrupt
void Avengers_selectNav (void);


#endif /* AVENGERSENV_H_ */
