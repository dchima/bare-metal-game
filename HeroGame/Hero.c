/*
 * Hero.c
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This script contains functions used to setup and drive the dynamics of
 * the hero player and enemy animations for the avengers game
 * It is a child script of Hero_Enviro.
 */

#include "Hero.h"

//initialise peripheral buttons
volatile unsigned int *BUTNS = (unsigned int *) 0xFF200050;

/* External Global Variables */

//variables for execution timing
unsigned int hero_lastTime[5] = {0,0,0,0,0};
unsigned int hero_periods[5] = {900000,1500000,270000000,6000000,900000};
unsigned int hero_currentTime = 0;
// Other externals
unsigned int hero_selection = 0;	//player selection check
unsigned int hero_selected = HERO_IRON; //default player selected
bool hero_launch = true;        // indicates a new launch
bool hero_dead;                 // if player is dead

/* Globals - non external */

bool hero_hit = false;    // if player is hit      
unsigned int hero_xpose, hero_ypose; // player position
unsigned short *hero_active; //pointer to active player image
unsigned short hero_button[4] = {0,0,0,0}; //button states
bool spawnShip = false; //check if enemy ships are spawned
bool isHiScore = false; //Checks if player score is a high score

// scoring variables
unsigned int hero_score;
char hero_scoretxt[5]; //string of score
unsigned int hero_life; //holds number of lives
char hero_HiScoresTxt [5][10]; //string of high score
unsigned int hero_HiScores [5];


// Structure of bullet element
// contains instance properties of bullet positions, active and collision statuse
typedef struct{
	unsigned int beam_x;
	unsigned int beam_y;
	bool active;
	bool collide;
} shooter;

// Structure of enemy ship element
// contains instance properties of ship positions, active and collision statuses
typedef struct{
	unsigned int ship_x;
	unsigned int ship_y;
	bool active;
	bool collide;
} spaceship;

unsigned int shoot_id = 0; //instance id of shooter structures
unsigned int ship_id = 0; //instance id of ship structures

// on a straight line, only 27 beams can fit the screen, so it makes sense
// to make that the instance limit at any given time
shooter beam[27];

// an arbitrary number of ship instance limits on the screen
// (ideally 35 ships will never be on the screen simultaneously... ideally).
spaceship fleet[35];


/* Function to initialise the Hero game */
void Hero_init(void){
    // This function initialises the game variables to the default values
    // and sets up the player character and the game border
	unsigned int i;
	// Clear the display
	LT24_clearDisplay(LT24_BLACK);
	ResetWDT();

	// initialise shooter beam variables
	for(i=0; i<27; i++){
		beam[i].beam_y = 270;
		beam[i].active = false;
		beam[i].collide = false;
	}

	// initialise enemy spaceships
	for(i=0; i<35; i++){
		fleet[i].ship_y = 21;
		fleet[i].active = false;
		fleet[i].collide = false;
	}

	// Setup the player image according to the character selection
	switch (hero_selection) {
	case HERO_IRON:
		hero_active = avengers_pIron;
		break;
	case HERO_PANTHER:
		hero_active = avengers_pPanther;
		break;
	case HERO_CAP:
		hero_active = avengers_pCap;
		break;
	case HERO_WIDOW:
		hero_active = avengers_pWidow;
		break;
	case HERO_THOR:
		hero_active = avengers_pThor;
		break;
	case HERO_SCARLET:
		hero_active = avengers_pScarlet;
		break;
	default:
		hero_active = avengers_pIron;
	}
    
    // Set game variables to default
    hero_dead = false;
    hero_life = 5; //default number of lives
    hero_score = 0;
    //default position of player (uper left side of image position in screen)
    hero_xpose = 95;
    hero_ypose = 284;

	// Draw the Gameplay environment border
	Graphics_drawBox(LT24_RED, false, 0, 4, 20, 215, 315);
	ResetWDT();
	// Draw the Player picture
	LT24_copyFrameBuffer(hero_active, hero_xpose, hero_ypose,30,30);
	ResetWDT();

	//Draw Player life bar
	Graphics_drawBox(LT24_GREEN,1,LT24_GREEN,4,10,20,15);
	Graphics_drawBox(LT24_GREEN,1,LT24_GREEN,24,10,40,15);
	Graphics_drawBox(LT24_GREEN,1,LT24_GREEN,44,10,60,15);
	Graphics_drawBox(LT24_GREEN,1,LT24_GREEN,64,10,80,15);
	Graphics_drawBox(LT24_GREEN,1,LT24_GREEN,84,10,100,15);

    //set lastTime variables for timing executions
	hero_lastTime[0] = Timer_currentValue();
	hero_lastTime[1] = Timer_currentValue();
	hero_lastTime[2] = Timer_currentValue();
	hero_lastTime[3] = Timer_currentValue();
	hero_lastTime[4] = Timer_currentValue();
    
}

/* Function to move player left or right */
void Hero_move(void){
    // This checks if the move left or move right button state is true
    // is pressed and moves the player accordingly
	if(hero_button[0] && hero_xpose > HERO_MAXLEFT){
		LT24_copyFrameBuffer(hero_active, hero_xpose--, hero_ypose,30,30);
		ResetWDT();
		hero_button[0] = 0;
	}
	else if(hero_button[1] && hero_xpose < HERO_MAXRIGHT){
		LT24_copyFrameBuffer(hero_active, hero_xpose++, hero_ypose,30,30);
		ResetWDT();

		hero_button[1] = 0;
	}

}

/* Function to update the button states */
void Hero_dynamics(void){
    // This function handles Button dynamics of the player on the screen
    // Button 0 shoots
    // Button 2 and 3 moves player right and left.
    
	if(*BUTNS & 0x8){
		//hero is moving left
		hero_button[0] = 1;
	}
	if(*BUTNS & 0x4){
		//hero is moving right
		hero_button[1] = 1;
	}
	if(*BUTNS & 0x1){
		//hero is shooting
		hero_button[2] = 1;
	}
	if(*BUTNS & 0x2){
		//hero is blasting
		hero_button[3] = 1;
	}
}

/* Function to activate a shot and move it */
void Hero_shooter(void){
    // This function sets the dynamics of the beam shot by the player.
    // It creates an instance of the bullet object and assigns unique coordinates to it.
    // When the bullet instance hits an enemy or has been exhausted, the function clears
    // that particular instance of it.
    
	unsigned int i, j;

	//when the shooter button is pressed, activate a beam
	if(hero_button[2] && !(*BUTNS & 0X1)){
		beam[shoot_id].active = true;
		beam[shoot_id].beam_x = hero_xpose+10;
		LT24_copyFrameBuffer(avengers_IronShot, beam[shoot_id].beam_x, beam[shoot_id].beam_y,10,10);
		shoot_id++;
		if(shoot_id == 27) shoot_id = 0; //reset the object identifier when all instances are exhausted
        heroShot_audio = 0; //start the shooter audio sequence
		hero_button[2] = 0;
	}

	//all active beams in field should move upwards in field
	for(i=0; i < 27; i++){
		if(beam[i].active){ //if particular instance is active, move it upwards
			LT24_copyFrameBuffer(avengers_IronShot, beam[i].beam_x, beam[i].beam_y--,10,10);
			ResetWDT();
			//check for active beams colliding with a ship
			for(j=0; j < 35; j++){
				if(fleet[j].active){
					if(beam[i].beam_y == fleet[j].ship_y+29){
						if((beam[i].beam_x+8 >= fleet[j].ship_x) && (beam[i].beam_x+1 <= fleet[j].ship_x+29)){
							beam[i].collide = true;
							fleet[j].collide = true;
							Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,150,0,239,19);
							hero_score++; //add score when bullet hits ship
						}
					}
				}

			}

		}
		//beam disappears when it meets the end of the screen
		if(beam[i].beam_y == 21){
			beam[i].collide = true;
		}
		if(beam[i].collide){ //when a collision occurs, this resets the instance of that bullet and resets it
			Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,beam[i].beam_x,beam[i].beam_y,beam[i].beam_x+9,beam[i].beam_y+9);
			beam[i].active = false;
			beam[i].beam_y = 270;
			beam[i].collide = false;
		}
	}
    ResetWDT();
}

/* Function to spawn and move enemy ship */
void Hero_fleet(void){
    // This function executes the dynamics of the enemy fleet.
    // Much like the bullet structures, each enemy ship is an instance of the fleet structure.
    // When the enemy reaches the end of the screen (bottom) or is hit by a bullet, the ship
    // instance is destroyed.
	unsigned int i;
	if(spawnShip){ //it is time for a new ship instance to be spawned on screen
		fleet[ship_id].active = true;
		fleet[ship_id].ship_x = (rand()%(HERO_MAXRIGHT-HERO_MAXLEFT)+ HERO_MAXLEFT);
		LT24_copyFrameBuffer(avengers_enemy1, fleet[ship_id].ship_x, fleet[ship_id].ship_y,30,30);
		ship_id++;

		if(ship_id == 35) ship_id = 0; //reset the object identifier when all instances are exhausted

		spawnShip = false; //toggle this so it is turned on again for a new ship to spawn
	}

	for(i=0; i < 35; i++){

		if(fleet[i].active){ //move ship downwards towards floor when it is on screen
			LT24_copyFrameBuffer(avengers_enemy1, fleet[i].ship_x, fleet[i].ship_y++,30,30);
			ResetWDT();
		}
		if(fleet[i].ship_y == 285){ //ship has reached end of screen.
			fleet[i].collide = true;
			hero_life--; //life of hero reduces when ship reaches the end of screen

		}

		if(fleet[i].collide){ //Reset instance of ship to default when it hits the end of the screen or is hit by a bullet.
			Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,fleet[i].ship_x,fleet[i].ship_y,fleet[i].ship_x+29,fleet[i].ship_y+29);
			fleet[i].active = false;
			fleet[i].ship_y = 21;
			fleet[i].collide = false;
            // redraw the player for persistence even if hit.
            LT24_copyFrameBuffer(hero_active, hero_xpose, hero_ypose,30,30);
            ResetWDT();
		}
	}
}

/* Function to keep track of player life*/
void Hero_life(void){
    // This function keeps track of the player life
    // It draws a life bar and keeps the player scores whenever it
    // destroys a ship.
    // Print the score
    sprintf(hero_scoretxt, "%d", hero_score);
	Graphics_printText(LT24_GREEN,36,0, hero_scoretxt);

	if(hero_life == 4) Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,84,10,100,15);
	else if(hero_life == 3) Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,64,10,80,15);
	else if(hero_life == 2) Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,44,10,60,15);
	else if(hero_life == 1) Graphics_drawBox(LT24_BLACK,1,LT24_BLACK,24,10,40,15);
	else if(hero_life == 0){// when player is dead, run end of life routine
		hero_dead = true;
        Hero_highScores(); //run high score check and save it to file
		LT24_clearDisplay(LT24_BLACK);
	    usleep(1000000);
        Hero_printOver(); //print end of life sequence
		ResetWDT();
	}

}

/* Function to save highscores to SD card */
void Hero_highScores (void) {
    // This function reads the high scores from the SD card,
    // Checks if the player score is a high score, and updates the high scores
    // and writes them back to the sd card
    // Five high scores are saved
    
    FIL fil;    // Create file pointer
    unsigned int i = 0;
    char buffer[200];   // String to store the high scores

    ResetWDT();
    // Open the high score file and read the values into
    // hero_HiScores array. 
    f_open(&fil, "scores1.txt", FA_READ);
    ResetWDT();
    while (f_gets(buffer, sizeof buffer, &fil)) {
    	hero_HiScores[i] = atoi (buffer);   //atoi converts the string to integer
        if (i < sizeof (hero_HiScores)) i++;
    }
    f_close(&fil);  // close the file
    ResetWDT();    
    isHiScore = true;   // assume player has a highscore

    // Check if the player's score is a high score and update the
    // High score array accordingly
    if (hero_score > hero_HiScores[0]) {
    	hero_HiScores [4] = hero_HiScores [3];
    	hero_HiScores [3] = hero_HiScores [2];
    	hero_HiScores [2] = hero_HiScores [1];
    	hero_HiScores [1] = hero_HiScores [0];
    	hero_HiScores [0] = hero_score;
    }
    else if (hero_score > hero_HiScores[1]) {
    	hero_HiScores [4] = hero_HiScores [3];
    	hero_HiScores [3] = hero_HiScores [2];
    	hero_HiScores [2] = hero_HiScores [1];
    	hero_HiScores [1] = hero_score;
    }
    else if (hero_score > hero_HiScores[2]) {
    	hero_HiScores [4] = hero_HiScores [3];
    	hero_HiScores [3] = hero_HiScores [2];
    	hero_HiScores [2] = hero_score;
    }
    else if (hero_score > hero_HiScores[3]) {
    	hero_HiScores [4] = hero_HiScores [3];
    	hero_HiScores [3] = hero_score;
    }
    else if (hero_score > hero_HiScores[4]) hero_HiScores [4] = hero_score;
    else isHiScore = false; // If not a high score, set isHiScore to false

    // Create a new high score file and write the updated high scores to it
    f_open(&fil, "scores1.txt", FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
    ResetWDT();
    f_printf(&fil, "%d\n%d\n%d\n%d\n%d\n", hero_HiScores[0], hero_HiScores[1], hero_HiScores[2], hero_HiScores[3], hero_HiScores[4]);
    ResetWDT();
    f_close(&fil);  // close the file
    
    // Convert the high scores to strings so the can be printed on the LCD
	sprintf(hero_HiScoresTxt[0], "%d", hero_HiScores[0]);
	sprintf(hero_HiScoresTxt[1], "%d", hero_HiScores[1]);
	sprintf(hero_HiScoresTxt[2], "%d", hero_HiScores[2]);
	sprintf(hero_HiScoresTxt[3], "%d", hero_HiScores[3]);
	sprintf(hero_HiScoresTxt[4], "%d", hero_HiScores[4]);
    
}

/* Function to print game over texts */
void Hero_printOver (void) {
    // This function prints the end of game text
	Graphics_printText(LT24_WHITE,19,5, "GAME OVER");
    
    if (isHiScore) {
        Graphics_printText(LT24_WHITE,16,7, "CONGRATULATIONS");
        Graphics_printText(LT24_WHITE,13,9, "YOU HAVE A HIGH SCORE");                
    } else {
        Graphics_printText(LT24_CYAN,22,7, "OOPS!");
        Graphics_printText(LT24_CYAN,10,9, "YOU DON'T HAVE A HIGH SCORE");
    }
    ResetWDT();

    Graphics_printText(LT24_CYAN,15,11, "YOUR SCORE = ");
    Graphics_printText(LT24_CYAN,28,11, hero_scoretxt);
    ResetWDT();
    Graphics_printText(LT24_CYAN,18,13, "HIGH SCORES");
    Graphics_printText(LT24_CYAN,18,15, "1.");
    Graphics_printText(LT24_CYAN,22,15, hero_HiScoresTxt[0]);
    Graphics_printText(LT24_CYAN,18,17, "2.");
    Graphics_printText(LT24_CYAN,22,17, hero_HiScoresTxt[1]);
    Graphics_printText(LT24_CYAN,18,19, "3.");
    Graphics_printText(LT24_CYAN,22,19, hero_HiScoresTxt[2]);
    Graphics_printText(LT24_CYAN,18,21, "4.");
    Graphics_printText(LT24_CYAN,22,21, hero_HiScoresTxt[3]);
    Graphics_printText(LT24_CYAN,18,23, "5.");
    Graphics_printText(LT24_CYAN,22,23, hero_HiScoresTxt[4]);
    ResetWDT();
        
    Graphics_printText(LT24_CYAN,13,26, "PRESS KEY2 TO CONTINUE");
    Graphics_printText(LT24_CYAN,15,28, "PRESS KEY3 FOR MENU");
    
    ResetWDT();    
    
}

/* Function to play the game reset scene */
void Hero_reset(void){
    // This function polls the key inputs and either restarts the game
    // or goes back to the main menu

    // Poll key inputs
	if(*BUTNS == 0x8) hero_button[0] = 1;
	if(*BUTNS == 0x4) hero_button[1] = 1;

    // Use key 2 to restart the game
	if(hero_button[1] & *BUTNS != 0x4) {
		hero_button[1] = 0;
		hero_button[0] = 0;
		Hero_init();
    // use key 3 to go to main menu
	} else if (hero_button[0] & *BUTNS != 0x8) {
		game_selection = 0;
        // unregister background music function from timer0 IRQ
		HPS_IRQ_unregisterHandler(HPS_TIMER0_IRQ); 
		hero_button[0] = 0;
		hero_button[1] = 0;
		newMenu = true;
		hero_launch = true;

	}
	ResetWDT();

}

/* Wrapper function for the player motion functions */
void Hero_motion(void){
	Hero_dynamics();
	Hero_move();
}

/* Function to execute Hero_shooter function at specified intervals */
void execute_shooter(h_func p){
	if((hero_lastTime[0] - hero_currentTime) >= hero_periods[0]){
		//This function takes in Hero_shooter and runs it at the
		//rate of period[0]
		p();

		hero_lastTime[0] -= hero_periods[0];
	}
}

/* Function to execute Hero_motion() function at specified intervals */
void execute_hero(h_func p){
	if((hero_lastTime[1] - hero_currentTime) >= hero_periods[1]){
		//This function takes in Hero_motion() and runs it at the
		//rate of period[1]
		p();


		hero_lastTime[1] -= hero_periods[1];
	}
}

/* Function to execute fleet function at specified intervals */
void execute_fleet(h_func p){
	if((hero_lastTime[2] - hero_currentTime) >= hero_periods[2]){
		//This is the rate at which the fleets are spawned on the screen
		//period[2]
		spawnShip = true;

		hero_lastTime[2] -= hero_periods[2];
	}

	if((hero_lastTime[3] - hero_currentTime) >= hero_periods[3]){
		//This function takes in Hero_fleet() and runs it at the
		//rate of period[3]
		p();

		hero_lastTime[3] -= hero_periods[3];
	}

}

/* Function to execute Hero_life() function at specified intervals */
void execute_life(h_func p){
	if((hero_lastTime[4] - hero_currentTime) >= hero_periods[4]){
		//This function takes in Hero_life() and runs it at the
		//rate of period[4]
		p();

		hero_lastTime[4] -= hero_periods[4];
	}
}
