/*
 * GameMenu.c
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This file contains functions that run the game main menu scene
 *
 */
#include "GameMenu.h"

// External Globals

bool newMenu = true;
unsigned int game_selection;
unsigned int game_selected;

/* Wrapper Function to run the game menu scene */
void Game_Menu(void){
    // This function runs the game menu scene if newMenu is requested
	if(newMenu) {
        game_selection = 0;
        game_selected = HEROGAME;

	    Game_menuScene();
		LT24_clearDisplay(LT24_BLACK);
		ResetWDT();
	    newMenu = false;
	}
}

/* Function to play the game manu scene */
void Game_menuScene (void) {
    // This function displays the title of the game environment
    // and allows the user to select between the available games

	// Clear display and display the game logos
	LT24_clearDisplay(LT24_BLACK);
	ResetWDT();
	LT24_copyFrameBuffer(menu_title,20,10,200,50);
	ResetWDT();
	LT24_copyFrameBuffer(menu_lAvengers,25,150,90,90);
	ResetWDT();
	LT24_copyFrameBuffer(menu_lSnake,125,150,90,90);
	ResetWDT();
    
    usleep(500000); // wait a while
	// Play the cursor select function
	Game_selectCursor();
}

/* Function to select a game */
void Game_selectCursor (void) {
    // This function allows the user to select a game. It displays the
    // game names and the selection cursor as the user moves through the games.
    // The HPS private timer is used to blink the cursor
    
    // local variables
    unsigned int currentTimerValue, lastTimeValue;  
    const unsigned int timePeriod = 80000000;   // 0.35 secs
    unsigned short blinkColour, colour;     // colour values
    bool swap = false;      // To know when to change cursor colour

    game_selection = 0;
	// Attach the navigation fucntion to the KEYS_IRQ.
    // The navigation function allows the user to move 
    // through the games
	HPS_IRQ_registerHandler(KEYS_IRQ, Game_selectNav);
	ResetWDT();
	// Begin private timer polling
    lastTimeValue = Timer_currentValue();
	// While a selection has not been made, we wait for the user to
    // make a selection. When a selection is made, the variable
    // game_selection will no longer be zero and this loop will end.

	while (game_selection == 0) {
		currentTimerValue = Timer_currentValue();   // Get current timer value
        // Check if is time to run the function
        if ((lastTimeValue - currentTimerValue) >= timePeriod) {
            // Change the cursor colour according to value of swap
            if (swap) blinkColour = LT24_YELLOW;    
            else blinkColour = LT24_GREEN;
            
            // Begin to display game names and cursor.
            // We set cursor colour to blinkcolour for the currently selected
            // character. Otherwise, we set it to the background which is black.
            // The cursor is box bordering the images that blinks
            // by changing from yellow to green and back
            
            // Herogame
            if (game_selected == HEROGAME) {
                LT24_copyFrameBuffer(menu_tAvengers,45,80,150,30);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw cursor
            Graphics_drawBox(colour,false,0,23,148,116,241);
            Graphics_drawBox(colour,false,0,24,149,115,240);
            ResetWDT();

            // snake game
            if (game_selected == SNAKEGAME) {
                LT24_copyFrameBuffer(menu_tSnake,45,80,150,30);
                ResetWDT();
                colour = blinkColour;
            } else {
                colour = LT24_BLACK;
            }
            // draw cursor
            Graphics_drawBox(colour,false,0,123,148,216,241);
            Graphics_drawBox(colour,false,0,124,149,215,240);
            ResetWDT();

            swap = !swap;       // toggle swap
            // update the lastTimeValue to when the program was last run
            lastTimeValue -= timePeriod;
        }

        // Clear the private timer interrupt flag
        Timer_clearInterrupt();
        ResetWDT();                
	}
}

/* Function to navigate through characters */
void Game_selectNav (void) {
    // This function allows the user to move through games and select
    // a game. The function is attached to the KEYS_IRQ interrupt
    
    // See IO_Peripherals.c for more details on the IO functions
	// get the key that was pressed
	unsigned int keyID = IO_getKeyPressID();

	// use key3 for previous
	if (keyID == 3) {
		game_selected = 1;
	}
	// use key2 for next
	else if (keyID == 2) {
		game_selected = 2;
	}
	// use key0 to select game
	else if (keyID == 0) {
		// unregister this function from the interrupts
		HPS_IRQ_unregisterHandler(KEYS_IRQ);
		// assign the selected game to the selection variable
		game_selection = game_selected;
	}
}
