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
unsigned int snake_totalAudio;
unsigned int snake_audio;


/* Function to play the game background music and shot audio */
void Snake_backAudio (void) {
    // This function is attached to timer0 interrupt to run at 
    // 48KHz or 1/48000 seconds interval which is the audio sampling rate.
    // It loads the background music audio samples to the audio codec.
    // If a shot is fired, it mixes the back ground music with the shot audio
    
    double ampl = 1000000000.0;     //Music Amplitude

    // We want the background music to run in a loop. So once we get to the last sample
    // of the back ground music, we start again from sample 0.    
    if (snake_audio >= snake_totalAudio) snake_audio = 0;
    
    // Check if there is space in the left and right audio fifos.
    // See WM8731_AudioEngine.c for more details
    if (Audio_writeSpace()) {
       // The audio file is a stereo file and thus has two channels
        // write the current sample to the left and right channels
        Audio_writeToLeft((signed int)(ampl*snake_Audio[snake_audio][0])); //col 0 left channel
        Audio_writeToRight((signed int)(ampl*snake_Audio[snake_audio][1]));//col 1 right channel
        snake_audio++;                  // Increment the sample counter
    }
    // Clear Timer0 interrupt. See HPS_Timers.c for more details
    Timer0_clearInterrupt();
}
