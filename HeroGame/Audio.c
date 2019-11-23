/*
 * Audio.c
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

#include "Audio.h"

/* External Global Variables */

// Total samples and current sample counter of background music
unsigned int hero_totalAudio;     
unsigned int hero_audio;           

// Total samples and current sample counter of shot audio
unsigned int heroShot_audio = sizeof(avengers_Audio3)/sizeof(avengers_Audio3[0]);
unsigned int heroShot_totalAudio = sizeof(avengers_Audio3)/sizeof(avengers_Audio3[0]);


/* Function to play the launch audio */
void Hero_launchAudio (void) {
    // This function is attached to timer0 interrupt to run at 
    // 48KHz or 1/48000 seconds interval which is the audio sampling rate.
    // It loads audio samples to the audio codec
    
    double ampl = 1000000000.0;     //Music Amplitude
	// We don't want to repeat the audio. So once the current sample equals the total samples,
    // unregister the interrupt, making timer0 available for further use as well.
    if (hero_totalAudio == hero_audio) {
        HPS_IRQ_unregisterHandler(HPS_TIMER0_IRQ);
    }
    // Check if there is space in the left and right audio fifos.
    // See WM8731_AudioEngine.c for more details
    if (Audio_writeSpace()) {
        // The audio file is a stereo file and thus has two channels
        // write the current sample to the left and right channels
        Audio_writeToLeft((signed int)(ampl*avengers_Audio1[hero_audio][0])); //col 0 left channel
        Audio_writeToRight((signed int)(ampl*avengers_Audio1[hero_audio][1]));//col 1 right channel
       hero_audio++; // Increment the sample counter
    }
    // Clear Timer0 interrupt. See HPS_Timers.c for more details
    Timer0_clearInterrupt();
}

/* Function to play the game background music and shot audio */
void Hero_backAudio (void) {
    // This function is attached to timer0 interrupt to run at 
    // 48KHz or 1/48000 seconds interval which is the audio sampling rate.
    // It loads the background music audio samples to the audio codec.
    // If a shot is fired, it mixes the back ground music with the shot audio

    // local Variables
    double ampl1 = 700000000.0;     // Background music amplitude
    double ampl2 = 1200000000.0;    // Shot audio amplitude
    double shotAudio[2];            // sample holder for shot audio
    // initialize sample holder to zero
    shotAudio[0] = 0;       
    shotAudio[1] = 0;

    // We want the background music to run in a loop. So once we get to the last sample
    // of the back ground music, we start again from sample 0.
    if (hero_audio >= hero_totalAudio) hero_audio = 0;
    
    // Check if there is space in the left and right audio fifos.
    // See WM8731_AudioEngine.c for more details
    if (Audio_writeSpace()) {
        
    // If the current sample counter of the shot audio is less than the total
    // samples, we assign the current sample to the sample holder. The sample counter 
    // the shot audio is set to zero when a shot is fired.
        if (heroShot_audio < heroShot_totalAudio) {
            shotAudio[0] = avengers_Audio3[heroShot_audio][0];
            shotAudio[1] = avengers_Audio3[heroShot_audio][1];
            heroShot_audio ++;          // increment the sample counter of the shot audio
        }
        // Then we add the background audio sample and the shot audio sample holder
        // and write them to the left and right channels
        Audio_writeToLeft((signed int)(ampl1*(avengers_Audio2[hero_audio][0])+(ampl2*shotAudio[0]))); //col 0 left channel
        Audio_writeToRight((signed int)(ampl1*(avengers_Audio2[hero_audio][1])+(ampl2*shotAudio[1])));//col 1 right channel
        hero_audio++;                   // Increment the sample counter of the background audio
    }
    // Clear Timer0 interrupt. See HPS_Timers.c for more details
    Timer0_clearInterrupt();
}

