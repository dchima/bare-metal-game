/*
 * Hero_Enviro.c
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

#include "Hero_Enviro.h"


/* Wrapper function to run the Avengers game */
void Hero_game(void){
    // This is the function called in the main.c

    // If a new launch has been requested, run launch sequence
	if(hero_launch) {
        Hero_launchScene();
        Hero_selectScene();
        Hero_init();
        hero_launch = false;
	    }
    // Run the game
	Hero_inGame();
}

/* Function to run the avengers game */
void Hero_inGame(void){
    // This function runs the avengers game.
    // The HPS private timer is used to run the 
    // game functions at specific intervals

    // If hero_dead is true, run the reset function
	if(hero_dead){
		Hero_reset();
        ResetWDT();
	}else{
    // Update current timer value and run the game functions
    
        hero_currentTime = Timer_currentValue();
        execute_hero(&Hero_motion);
		execute_shooter(&Hero_shooter);
		execute_fleet(&Hero_fleet);
		execute_life(&Hero_life);
	}
    // Clear the timer interrupt and reset the watch dog
    Timer_clearInterrupt();
    ResetWDT();
}

/* Function to play the game launch scene */
void Hero_launchScene (void) {
    // The launch scene is a slide show of images and a launch music. 
    // The HPS private timer is used to slide through the images.
    // The HPS timer0 IRQ interrupt is used to run the music
    
    // local variables
	unsigned int currentTimerValue, lastTimeValue, img = 0;
	const unsigned int timePeriod = 200000000; //slide show image interval. 0.87 secs

	// Get total samples in launch audio file
	hero_totalAudio = sizeof(avengers_Audio1)/sizeof(avengers_Audio1[0]);
	// Initialize current sample counter to zero
	hero_audio = 0;
    // clear the display and reset the watch dog
	LT24_clearDisplay(LT24_BLACK);
	ResetWDT();
    
    // To play the launch music, we attach the Hero_launchAudio() function to timer0 
    // IRQ interrupt. This function loads the audio codec with the next audio sample.
    // First we disable the timer, load it with our desired period, and then enable the timer
	// The timer operates at 100MHz. 2083 achieves a frequency of 48KHz (audio sampling frequency)
	// See HPS_Timers.c for more details about the timer functions
    
	Timer0_disable();       // disable timer0
	Timer0_load(2083);      // load with our desired value
	Timer0_enable();        // enable the timer

	// We now attach our audio function to timer0 IRQ
	HPS_IRQ_registerHandler(HPS_TIMER0_IRQ, Hero_launchAudio);
	ResetWDT();
    
    // Now we begin the slide show and stay at the last image until
    // the launch audio has finished playing. The HPS private timer 
    // is used to periodically update the current image
    
    lastTimeValue = Timer_currentValue();   // Set lasttimevalue to the current timer value
	// While the audio is still playing, run the slide show.
	while (hero_audio < hero_totalAudio) {
        currentTimerValue = Timer_currentValue();   // Get current timer value
        // Check if it is time to display next image
        if ((lastTimeValue - currentTimerValue) >= timePeriod) {
            // Display the slide show image indicated by variable img
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
            // increment img if we are not on the last image. 
            // Otherwise, we want it to stay at the last image
            if(img < 6) img++;
            // update the last time value to when program was last run
            lastTimeValue -= timePeriod; 
        }

        // Clear the private timer interrupt flag
        Timer_clearInterrupt();
        ResetWDT();                
	}
}

/* Function to play the game hero selection scene */
void Hero_selectScene (void) {
    // This function displays images the game characters (heroes),
    // attaches the background music, and allows the player to 
    // choose a character using the keys.
    // The KEYS_IRQ is used to read the player action
    
	// Clear the display to black
	LT24_clearDisplay(LT24_BLACK);
	ResetWDT();
    // Display all character images
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
    
	// Set the background audio sample counter to zero 
    // and get the total audio samples of the background audio
	hero_audio = 0;
	hero_totalAudio = sizeof(avengers_Audio2)/sizeof(avengers_Audio2[0]); // Total samples in audio file
    
    // To play the background music, we attach the Hero_backAudio() function to timer0 
    // IRQ interrupt. This function loads the audio codec with the next audio sample.
    // First we disable the timer, load it with our desired period, and then enable the timer
	// The timer operates at 100MHz. 2083 achieves a frequency of 48KHz (audio sampling frequency)
	// See HPS_Timers.c for more details about the timer functions    
    
	Timer0_disable();       // disable timer0
	Timer0_load(2083);      // load with our desired value
	Timer0_enable();        // enable the timer

	// We now attach our audio function to timer0 IRQ
	HPS_IRQ_registerHandler(HPS_TIMER0_IRQ, Hero_backAudio);
	ResetWDT();

	// Then we run the select cursor function
	Hero_selectCursor();

}

/* Function to select game character (hero) */
void Hero_selectCursor (void) {
    // This function allows the user to select a character. It displays the
    // character names and the selection cursor as the user moves through characters.
    // The HPS private timer is used to blink the cursor
    
    // local variables
    unsigned int currentTimerValue, lastTimeValue;  
    const unsigned int timePeriod = 80000000;   // 0.35 secs
    unsigned short blinkColour, colour;     // colour values
    bool swap = false;      // To know when to change cursor colour

    // set the hero selection to zero
    hero_selection = 0;
    
	// Attach the navigation fucntion to the KEYS_IRQ.
    // The navigation function allows the user to move 
    // through characters
	HPS_IRQ_registerHandler(KEYS_IRQ, Hero_selectNav);
	ResetWDT();

	// Begin private timer polling
    lastTimeValue = Timer_currentValue();
	// While a selection has not been made, we wait for the user to
    // make a character selection. When a selection is made, the variable
    // hero_selection will no longer be zero and this loop will end.
	while (hero_selection == 0) {
		currentTimerValue = Timer_currentValue();   // Get current timer value
        // Check if is time to run the function
        if ((lastTimeValue - currentTimerValue) >= timePeriod) {
            // Change the cursor colour according to value of swap
            if (swap) blinkColour = LT24_YELLOW;    
            else blinkColour = LT24_GREEN;
            
            // Begin to display chracter names and cursor.
            // We set cursor colour to blinkcolour for the currently selected
            // character. Otherwise, we set it to the background which is black.
            // The cursor is set of lines bordering the character images that blinks
            // by changing from yellow to green and back
            
            // Characters

            // Iron man
            if (hero_selected == HERO_IRON) {
                LT24_copyFrameBuffer(avengers_sIronText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw the cursor
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
            if (hero_selected == HERO_PANTHER) {
                LT24_copyFrameBuffer(avengers_sPantherText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw the cursor
            Graphics_drawLine (colour, 88, 144, 151, 144);
            Graphics_drawLine (colour, 88, 143, 151, 143);
            Graphics_drawLine (colour, 88, 225, 151, 226);
            Graphics_drawLine (colour, 88, 226, 151, 225);
            Graphics_drawLine (colour, 88, 143, 88, 226);
            Graphics_drawLine (colour, 89, 143, 89, 226);
            Graphics_drawLine (colour, 150, 143, 150, 226);
            Graphics_drawLine (colour, 151, 143, 151, 226);
            ResetWDT();

            // Captain America
            if (hero_selected == HERO_CAP) {
                LT24_copyFrameBuffer(avengers_sCapText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw the cursor
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
            if (hero_selected == HERO_WIDOW) {
                LT24_copyFrameBuffer(avengers_sWidowText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw the cursor
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
            if (hero_selected == HERO_THOR) {
                LT24_copyFrameBuffer(avengers_sThorText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw the cursor
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
            if (hero_selected == HERO_SCARLET) {
                LT24_copyFrameBuffer(avengers_sScarletText,60,70,120,60);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw the cursor
            Graphics_drawLine (colour, 163, 234, 226, 234);
            Graphics_drawLine (colour, 163, 233, 226, 233);
            Graphics_drawLine (colour, 163, 315, 226, 316);
            Graphics_drawLine (colour, 163, 316, 226, 315);
            Graphics_drawLine (colour, 163, 233, 163, 316);
            Graphics_drawLine (colour, 164, 233, 164, 316);
            Graphics_drawLine (colour, 225, 233, 225, 316);
            Graphics_drawLine (colour, 226, 233, 226, 316);
            ResetWDT();

            swap = !swap;  // toggle swap to change blink colour
            // update the lastTimeValue to when the program was last run
            lastTimeValue -= timePeriod;
        }

        // Clear the private timer interrupt flag
        Timer_clearInterrupt();
        ResetWDT();                
	}
}

/* Function to navigate through characters */
void Hero_selectNav (void) {
    // This function allows the user to move through characters and select
    // a character. The function is attached to the KEYS_IRQ interrupt
    

    // See IO_Peripherals.c for more details on the IO functions
    // get the key that was pressed
    
	unsigned int keyID = IO_getKeyPressID();
	// use key3 for previous
	if (keyID == 3) {
		if (hero_selected == 1) hero_selected = 6;
		else hero_selected --;
	}
	// use key2 for next
	else if (keyID == 2) {
		if (hero_selected == 6) hero_selected = 1;
		else hero_selected ++;
	}
	// use key1 to select character
	else if (keyID == 0) {
		// assign the selected hero to the selection variable
		hero_selection = hero_selected;
		// Then unregister this function from the KEYS_IRQ
		HPS_IRQ_unregisterHandler(KEYS_IRQ);
	}
}
