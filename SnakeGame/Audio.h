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
 * This file contains the audio function for the snake game.
 * The audio file has been decompressed using matlab and converted
 * to a C-array of the samples.
 *
 */

#ifndef AUDIO_H_
#define AUDIO_H_

/* Inclusions */

// Include the necessary drivers 
#include "../WM8731_AudioEngine/WM8731_AudioEngine.h"   // Audio driver
#include "../HPS_IRQ/HPS_IRQ.h"                         // HPS interrupt driver
#include "../HPS_Timers/HPS_Timers.h"                   // Timer driver

// Include the audio file
#include "Sounds/snake_Audio.h"         // Background music

// External Global Variables
extern unsigned int snake_audio;
extern unsigned int snake_totalAudio;

/* Function prototype */

// Function to play the background audio
void Snake_backAudio (void);


#endif /* AUDIO_H_ */
