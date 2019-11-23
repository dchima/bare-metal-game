/*
 * Audio.h
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This file contains the audio functions for the avengers game.
 * The audio files have been decompressed using matlab and converted
 * to a C-array of the samples.
 *
 */

#ifndef AUDIO_H_
#define AUDIO_H_

/* Inclusions */

// Include the necessary drivers 
#include "../WM8731_AudioEngine/WM8731_AudioEngine.h"   // Audio driver
#include "../HPS_IRQ/HPS_IRQ.h"                         // HPS interrupt driver
#include "../HPS_Timers/HPS_Timers.h"                   // Timer driver for private timer and timer 0

// Include the audio files
#include "Sounds/avengers_Audio1.h"         // Launch Scene audio
#include "Sounds/avengers_Audio2.h"         // Game background audio
#include "Sounds/avengers_Audio3.h"         // shot audio effect

// External Global Variables
extern unsigned int heroShot_audio;
extern unsigned int heroShot_totalAudio;
extern unsigned int hero_audio;
extern unsigned int hero_totalAudio;

/* Function prototypes */

// Function to play the launch scene audio
void Hero_launchAudio (void);

// Function to play the background audio and shot audio effect
void Hero_backAudio (void);


#endif /* AUDIO_H_ */
