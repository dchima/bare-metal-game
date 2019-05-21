/*
 * AvengersEnv.c
 *
 *  Created on: Mar 21, 2018
 *      Author: Azeem
 */

#include "AvengersEnv.h"

// Global Variables 

bool avengers_exit = false;                         // To indicate an exit from the game - Currently unused
unsigned int avengers_selector = AVENGERS_IRON;    // To move through characters in the selection scene
unsigned int avengers_selection = 0;        // To indicate a selection - Currently unsused
unsigned int avengers_audSamp;      // Current audio sample
unsigned int avengers_totAudSamp;   // Total audio samples

/* This program displays the launch scene and music*/
void Avengers_launchScene (void) {
    unsigned int currentTimerValue, lastTimeValue;
    const unsigned int timePeriod = 200000000;     // Slide show image interval. less than a second
    unsigned int img = 0;         // Variable to move through images

    avengers_totAudSamp = sizeof(avengers_Audio1)/sizeof(avengers_Audio1[0]); // Total samples in launch audio file
    avengers_audSamp = 0;       // Initialize current sample to zero
    LT24_clearDisplay(LT24_BLACK);
    ResetWDT();
    // Now we attach the launch audio player function to timer0 interrupt.
    // First we disable the timer, load it with our desired period, and then enable the timer
    // The timer operates at 100MHz - The functions used are in driver HPS_Timers
    Timer0_disable();       
    Timer0_load(2083);      // 2083 acheives a frequency of 48KHz (audio sampling frequency)
    Timer0_enable();
    // We now attach our function to timer0 IRQ
    HPS_IRQ_registerHandler(HPS_TIMER0_IRQ, Avengers_launchAudio);
    ResetWDT();
    lastTimeValue = Timer_currentValue();
    // While the audio is still playing, do the slide show
    while (avengers_audSamp < avengers_totAudSamp) {
        // This is to break from the loop when the exit button is presses
        // No exit button yet
        if (avengers_exit) {
            HPS_IRQ_unregisterHandler(HPS_TIMER0_IRQ);      //stop the music*/
            break;                                          // leave this loop
            // other instructions to come
        }                
        currentTimerValue = Timer_currentValue();   // Get current timer value
        // Check if it is time to display next image
        if ((lastTimeValue - currentTimerValue) >= timePeriod) {
            // Display the image indicated by variable img
            switch (img) {
                case 0:
                    LT24_copyFrameBuffer(avengers_Limage0,0,90,240,134);
                    break;
                case 1:
                    LT24_copyFrameBuffer(avengers_Limage1,0,90,240,134);
                    break;
                case 2:
                    LT24_copyFrameBuffer(avengers_Limage2,0,90,240,134);
                break;
                case 3:
                    LT24_copyFrameBuffer(avengers_Limage3,0,90,240,134);
                break;
                case 4:
                    LT24_copyFrameBuffer(avengers_Limage4,0,90,240,134);
                break;
                case 5:
                    LT24_copyFrameBuffer(avengers_Limage5,0,90,240,134);
                break;
                default:
                    LT24_copyFrameBuffer(avengers_Limage6,0,0,240,320);
            }            
            // increment img only when it is less than 6. We want it to stay at the last image
            if(img < 6) img++;
            lastTimeValue = lastTimeValue - timePeriod;
        }

        // If the timer interrupt flag is set, clear the flag
        Timer_clearInterrupt();
        ResetWDT();
    }

}

// Play the launch audio. This function has been attached to timer0 interrupt
// and will run at 48KHz frequency or 1/48000 seconds
void Avengers_launchAudio (void) {
    // We don't want it to repeat the audio. So once the current sample
    // has reached the total samples, unregister the interrupt.
    // This will detach the audio function from the timer0 interrupt, making it available for further use
    if (avengers_totAudSamp == avengers_audSamp) {
        HPS_IRQ_unregisterHandler(HPS_TIMER0_IRQ);
    }
    double ampl = 1000000000.0;     //Music Amplitude
    // Write the audio files to the fifos if there is space
    // See Audio driver for more details
    if (Audio_writeSpace()) {
        // write the sample to the channels 
        // The audio is a stereo file so has two channels. Mono will have just one
        Audio_writeToLeft((signed int)(ampl*avengers_Audio1[avengers_audSamp][0])); //col 0 left channel
        Audio_writeToRight((signed int)(ampl*avengers_Audio1[avengers_audSamp][1]));//col 1 right channel
        avengers_audSamp++; // Increment the sample counter
    }
    Timer0_clearInterrupt();        // Must clear the interrupt flag
}

// Plays the background music
// This might be where i'll add the sound effects for when a shot is fired.
// By averaging the two audio files. I dunno yet.
void Avengers_backAudio (void) {
    double ampl = 1000000000.0;     //Music Amplitude
    // We want this to loop so set the sample variable to reset every time it reaches the end
    if (avengers_totAudSamp == avengers_audSamp) avengers_audSamp = 0;
    if (Audio_writeSpace()) {
    // Write the audio files to the fifos if there is space
        // write the sample to the channels 
        Audio_writeToLeft((signed int)(ampl*avengers_Audio2[avengers_audSamp][0])); //col 0 left channel
        Audio_writeToRight((signed int)(ampl*avengers_Audio2[avengers_audSamp][1]));//col 1 right channel
        avengers_audSamp++; // Increment the sample counter
    }
    Timer0_clearInterrupt();        // Clear interrupts flag
}

// Plays the selection scene
void Avengers_selectScene (void) {
    // This is for key interrupt. I will work on making it a driver later
    volatile unsigned int * KEY_ptr     = (unsigned int *) 0xFF200050;    
    // Clear display and display all the character images as well as the A logo
    LT24_clearDisplay(LT24_BLACK);
    ResetWDT();
    LT24_copyFrameBuffer(avengers_sLogo,0,0,70,60);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sTitle,70,0,170,60);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sIron,15,145,60,80);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sPanther,90,145,60,80);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sCap,165,145,60,80);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sWidow,15,235,60,80);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sThor,90,235,60,80);
    ResetWDT();        
    LT24_copyFrameBuffer(avengers_sScarlet,165,235,60,80);
    ResetWDT();
    // Set the audio sample counter to zero and get the total samples of the back ground music
    avengers_audSamp = 0;
    avengers_totAudSamp = sizeof(avengers_Audio2)/sizeof(avengers_Audio2[0]); // Total samples in audio file
    // As before, disable the timer, load it, then enable
    Timer0_disable();
    Timer0_load(2083);      //100MHz, 2083 load value for 1/48000 seconds
    Timer0_enable();
    // Attach our background music to the timer0 interrupt
    // Timer0 and the audio codec can't be used while this is still attached.
    // unless we detach the interrupt or overwrite this function.
    HPS_IRQ_registerHandler(HPS_TIMER0_IRQ, Avengers_backAudio);
    ResetWDT();
    
    // Configure Push Buttons to interrupt on press - This will be later moved to a driver
    KEY_ptr[2] = 0xF;     // Enable interrupts for all four KEYs

    // Play the cursor select function
    Avengers_selectCursor();
    
}

// This function displays the names and the selection cursor
// as the user moves through characters
void Avengers_selectCursor (void) {
    unsigned int currentTimerValue, lastTimeValue;  
    const unsigned int timePeriod = 100000000;      // We refresh the name image and blink cursor at about half a sec
    unsigned short blinkColour, colour;
    bool swap = false;      // To know when to swap cursor colour
    // Attach navigation function to key
    HPS_IRQ_registerHandler(KEYS_IRQ, Avengers_selectNav);
    ResetWDT();
    // Begin timer polling
    lastTimeValue = Timer_currentValue();
    // While a selection has not been made, remain here
    // Future work here:
    // When a selection is made by pressing another key, it will be passed to avengers_selection variable
    // This variable will no longer be zero and then this loop will end.
    // The exit variable will be inserted later on to break out of this loop
    while (avengers_selection == 0) {
        currentTimerValue = Timer_currentValue();   // Get current timer value
        if ((lastTimeValue - currentTimerValue) >= timePeriod) {
            // Change the curso colour according to value of swap
            if (swap) blinkColour = LT24_YELLOW;    
            else blinkColour = LT24_GREEN;
            
            // Display chracter names and cursor
            // We set cursor colour to blinkcolour if this is the current selection
            // Otherwise, we set it to the background which is black.
            // The cursor is just supposed to be a line bordering the character images that blinks
            // by changing from yellow to white and back
            
            // Iron man
            if (avengers_selector == AVENGERS_IRON) {
                LT24_copyFrameBuffer(avengers_sIronText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            Graphics_drawLine (colour, 13, 144, 76, 144);            
            Graphics_drawLine (colour, 13, 143, 76, 143);
            Graphics_drawLine (colour, 13, 225, 76, 226);
            Graphics_drawLine (colour, 13, 226, 76, 225);
            Graphics_drawLine (colour, 13, 143, 13, 226);
            Graphics_drawLine (colour, 14, 143, 14, 226);
            Graphics_drawLine (colour, 75, 143, 75, 226);
            Graphics_drawLine (colour, 76, 143, 76, 226);
            ResetWDT();
            
            // Black panther
            if (avengers_selector == AVENGERS_PANTHER) {
                LT24_copyFrameBuffer(avengers_sPantherText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            Graphics_drawLine (colour, 88, 144, 151, 144);            
            Graphics_drawLine (colour, 88, 143, 151, 143);
            Graphics_drawLine (colour, 88, 225, 151, 226);
            Graphics_drawLine (colour, 88, 226, 151, 225);
            Graphics_drawLine (colour, 88, 143, 88, 226);
            Graphics_drawLine (colour, 89, 143, 89, 226);
            Graphics_drawLine (colour, 150, 143, 150, 226);
            Graphics_drawLine (colour, 151, 143, 151, 226);
            ResetWDT();
            
            // Cappo
            if (avengers_selector == AVENGERS_CAP) {
                LT24_copyFrameBuffer(avengers_sCapText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            Graphics_drawLine (colour, 163, 144, 226, 144);            
            Graphics_drawLine (colour, 163, 143, 226, 143);
            Graphics_drawLine (colour, 163, 225, 226, 226);
            Graphics_drawLine (colour, 163, 226, 226, 225);
            Graphics_drawLine (colour, 163, 143, 163, 226);
            Graphics_drawLine (colour, 164, 143, 164, 226);
            Graphics_drawLine (colour, 225, 143, 225, 226);
            Graphics_drawLine (colour, 226, 143, 226, 226);
            ResetWDT();
            
            // Blackwidow
            if (avengers_selector == AVENGERS_WIDOW) {
                LT24_copyFrameBuffer(avengers_sWidowText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            Graphics_drawLine (colour, 13, 234, 76, 234);            
            Graphics_drawLine (colour, 13, 233, 76, 233);
            Graphics_drawLine (colour, 13, 315, 76, 316);
            Graphics_drawLine (colour, 13, 316, 76, 315);
            Graphics_drawLine (colour, 13, 233, 13, 316);
            Graphics_drawLine (colour, 14, 233, 14, 316);
            Graphics_drawLine (colour, 75, 233, 75, 316);
            Graphics_drawLine (colour, 76, 233, 76, 316);
            ResetWDT();

            // Lord of thunder
            if (avengers_selector == AVENGERS_THOR) {
                LT24_copyFrameBuffer(avengers_sThorText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            Graphics_drawLine (colour, 88, 234, 151, 234);            
            Graphics_drawLine (colour, 88, 233, 151, 233);
            Graphics_drawLine (colour, 88, 315, 151, 316);
            Graphics_drawLine (colour, 88, 316, 151, 315);
            Graphics_drawLine (colour, 88, 233, 88, 316);
            Graphics_drawLine (colour, 89, 233, 89, 316);
            Graphics_drawLine (colour, 150, 233, 150, 316);
            Graphics_drawLine (colour, 151, 233, 151, 316);
            ResetWDT();
            
            // Scarlet witch
            if (avengers_selector == AVENGERS_SCARLET) {
                LT24_copyFrameBuffer(avengers_sScarletText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            Graphics_drawLine (colour, 163, 234, 226, 234);            
            Graphics_drawLine (colour, 163, 233, 226, 233);
            Graphics_drawLine (colour, 163, 315, 226, 316);
            Graphics_drawLine (colour, 163, 316, 226, 315);
            Graphics_drawLine (colour, 163, 233, 163, 316);
            Graphics_drawLine (colour, 164, 233, 164, 316);
            Graphics_drawLine (colour, 225, 233, 225, 316);
            Graphics_drawLine (colour, 226, 233, 226, 316);
            ResetWDT();

            swap = !swap;       // toggle swap
            lastTimeValue = lastTimeValue - timePeriod;
        }
        Timer_clearInterrupt();
        ResetWDT();
    }

}

// This function allows the user to navigate through
// Characters by pressing key0
void Avengers_selectNav (void) {
    volatile unsigned int * KEY_ptr     = (unsigned int *) 0xFF200050;    
    unsigned int press;
    //Read the Push-button interrupt register
    press = KEY_ptr[3];
    //Then clear the interrupt flag by writing the value back
    KEY_ptr[3] = press;
        // If key0 was pressed, increment the selector variable
        // go back to begining if at max
    if (press & 0x1) {
        if (avengers_selector == 6) avengers_selector = 1;
        else avengers_selector ++;
    }    
}
