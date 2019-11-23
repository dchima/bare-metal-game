/*
 * main.c
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This is an interactive gaming environment
 * It consists of two games; Hero (avengers) game and snake game
 * the user can select which game to play
 *
 * Features of the game include
 * - Saving of high scores to SD Card
 * - Playing of audio
 *
 */

// Include required header files

#include "HeroGame/Hero_Enviro.h"   // Include hero game
#include "SnakeGame/Snake_Enviro.h" // include snake game

// Exit on fail function prototype
void exitOnFail(signed int status, signed int successStatus);

// Create FATFS file system for accessing the SD card
FATFS FatFs;

// Main function
int main(void){
    
    // mount file system
    f_mount(&FatFs, "", 0);
    ResetWDT();

    // Initialise the LCD graphics engine
	exitOnFail(
			Graphics_initialise(0xFF200060,0xFF200080),
			LT24_SUCCESS);
	ResetWDT();

	// Initialise the Private timer and timer0
	exitOnFail(
			Timer_initialise(),
			TIMER_SUCCESS);
    // configure the private timer
	exitOnFail(
			Timer_setControlRegister (false, true, true),
			TIMER_SUCCESS);
    // Load the private timer
	exitOnFail(
			Timer_setLoadValue (0xFFFFFFFF),
			TIMER_SUCCESS);
    // Set the prescaler value to 0
	exitOnFail(
			Timer_setPrescalerValue (0),
			TIMER_SUCCESS);
    ResetWDT();

    // Initialise the audio codec
    exitOnFail(
            Audio_initialise(),
            WM8731_SUCCESS);
    //Clear both FIFOs
    Audio_clearFIFO(true,true);
    ResetWDT();
    // Initialise the KEYS driver for IRQ
    IO_initialise();
    // Initialise IRQ
    HPS_IRQ_initialise();
    ResetWDT();
    Audio_outVolume (8);          // Set audio volume to level 8
    ResetWDT();

    // Main run loop
	while(1){

        // See GameMenu, HeroGame and SnakeGame folders for function details. 
    
        // Run the game menu function
        Game_Menu();
        // if the hero game was selected, run the hero game
        if (game_selection == HEROGAME) Hero_game();
        // else if the snake game was selected, run the snake game
        else if (game_selection == SNAKEGAME) Snake_Game();
        ResetWDT(); // reset watchdog
	}


}


// Exit on fail function
void exitOnFail(signed int status, signed int successStatus){
	if (status != successStatus) {
		exit((int)status); //Add breakpoint here to catch failure
	}
}
