/*
 * DE1-SoC Audio Example
 *
 * Generates a sinusoidal tone waveform
 * Writes stereo audio to the CODEC (LINE OUT)
 *
 *  Created on: 03 Feb 2018
 */

#include "AvengersGame/AvengersGame.h"
#include "HPS_Watchdog/HPS_Watchdog.h"
#include "HPS_Timers/HPS_Timers.h"  // Timer Driver
#include "WM8731_AudioEngine/WM8731_AudioEngine.h"
#include "HPS_IRQ/HPS_IRQ.h"
//Include Floating Point Math Libraries
#include <math.h>


void exitOnFail(signed int status, signed int successStatus);

//
// Main Function
// =============
int main(void) {


//    const unsigned int timePeriod [3] = {4817, 20000000, 2000000};
//    unsigned int currentTimerValue, lastTimeValue [3];

// Initialise the timers // private timer and timer0       
	exitOnFail(
			Timer_initialise (),
			TIMER_SUCCESS);  //Exit if not successful
    // Configure, load and set prescaler for private timer
    // Configure the timer
	exitOnFail(
			Timer_setControlRegister (false, true, true),
			TIMER_SUCCESS);
    // Load the timer
	exitOnFail(
			Timer_setLoadValue (0xFFFFFFFF),
			TIMER_SUCCESS);
    // Set the prescaler value to 0
	exitOnFail(
			Timer_setPrescalerValue (0),
			TIMER_SUCCESS);
    ResetWDT();
    // Graphics driver
    exitOnFail(
            Graphics_initialise(0xFF200060,0xFF200080),
            LT24_SUCCESS);                          
    ResetWDT();

    //Initialise the Audio Codec.
    exitOnFail(
            Audio_initialise(),  //Initialise Audio Codec
            WM8731_SUCCESS);                //Exit if not successful
    //Clear both FIFOs
    Audio_clearFIFO(true,true);
    ResetWDT();
    // Initialise IRQ
    HPS_IRQ_initialise();
    ResetWDT();
    Audio_outVolume (8);          // Set audio volume to level 8



// Main run loop
while (1) {
        
        // Run avengers game
        Avengers_main();
/*
        currentTimerValue = Timer_currentValue();
        if ((lastTimeValue[0] - currentTimerValue) >= timePeriod[0]) {

        //Always check the FIFO Space before writing or reading left/right channel pointers
        if (Audio_writeSpace()) {
            //If there is space in the write FIFO for both channels:
            //Increment the phase
            
       //     phase = phase + inc;
            //Ensure phase is wrapped to range 0 to 2Pi (range of sin function)
     //       while (phase >= PI2) {



   //             phase = phase - PI2;
 //           }
            // Calculate next sample of the output tone.
//            audio_sample = (signed int)( ampl * sin( phase ) );
            // Output tone to left and right channels.
//            *audio_left_ptr = audio_sample;
//            *audio_right_ptr = audio_sample;

            Audio_writeToLeft((signed int)(ampl*AvengersLaunch2[i][0]));
            Audio_writeToRight((signed int)(ampl*AvengersLaunch2[i][1]));
            i++;
//            i1++;
            if (i >= rowBack) i = 0;
//            if (i1 >= row1) i1 = 0;


        }
        lastTimeValue[0] = lastTimeValue[0] - timePeriod[0];
        }
        if (Timer_interrupt()) {
            // If the timer interrupt flag is set, clear the flag
        	Timer_clearInterrupt();
        }
        //Debugging - display FIFO space on red LEDs.
        //Finally reset the watchdog.
        */
        ResetWDT();
    }
}





void exitOnFail(signed int status, signed int successStatus){
    if (status != successStatus) {
        exit((int)status); //Add breakpoint here to catch failure
    }
}
