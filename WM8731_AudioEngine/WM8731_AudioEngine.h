/*
 * WM8731_AudioEngine.h
 *
 * Created on: Mar 20, 2018
 * Author: Azeem Oguntola
 * -------------------------------------------------------------------
 */
 
#ifndef WM8731_AUDIOENGINE_H_
#define WM8731_AUDIOENGINE_H_

//Include required header files
#include <stdbool.h> //Boolean variable type "bool" and "true"/"false" constants.

//Error Codes
#define WM8731_SUCCESS      0
#define WM8731_ERRORNOINIT -1

//FIFO Space Offsets
#define WM8731_RARC 0
#define WM8731_RALC 1
#define WM8731_WSRC 2
#define WM8731_WSLC 3

//Initialise Audio Controller
// - base_address is memory-mapped address of audio controller
// - returns 0 if successful
signed int Audio_initialise (void);

//Check if driver initialised
bool WM8731_isInitialised(void);


//Clears FIFOs
// - returns true if successful
signed int Audio_clearFIFO( bool adc, bool dac);

// Check if the Output FIFO space.
// Use this fucntion to check if audio files can be written
// We can only write audio if this function returns a true
bool Audio_writeSpace (void);

/*
// Check if the Input FIFO space.
// Use this function to check if audio files are available to be read
// We can only read audio if this function returns a true
bool Audio_readSpace (void);
*/

// Functiion to write audio to the left channel
// Check Audio_writeSpace before calling this function
// - returns 0 if successful
signed int Audio_writeToLeft (signed int audio_data);

// Functiion to write audio to the right channel
// Check Audio_writeSpace before calling this function
// - returns 0 if successful
signed int Audio_writeToRight (signed int audio_data);

// Function to set output volume
// 0 = mute. Volume levels are 1 to 10
// - returns 0 if successful
signed int Audio_outVolume (unsigned int level);

#endif /* WM8731_AUDIOENGINE_H_ */
