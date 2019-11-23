/*
 * Snake_Enviro.c
 *
 * Created on: 11 Apr 2018
 *  Authors:
 * 
 *      Azeem Oguntola  SID: 201162945
 *      Chima Nnadika   SID: 201077064
 *
 * Description
 * ---------------------------------------------------------------------
 * This script contains functions used to setup and drive the environment of an
 * animated snake. It is uses snake.h
 *
 */

#include "Snake_Enviro.h"
#include "Audio.h"

/* External Global variables */

//variables for execution timing
unsigned int snake_lastTime[2] = {0,0};
unsigned int snake_periods[2] = {9000000,1000000};
unsigned int snake_currentTime = 0;

bool snake_newGame;

bool snake_startgame = true; //runs the begining of the game
unsigned short snake_button [2]; //button states

/* Wrapper Function to run the snake game */
void Snake_Game(void){
    // This function runs the functions that make up the snake game.
    // It runs the reset screen if the game is over
	if(snake_startgame){ //at start of game
		Graphics_printText(colours[4],17,20, " S N A K E  ");
		usleep(2000000);
		snake_newGame = true; //when this is true, it runs the environment instance once
		snake_startgame = false;
	}

	Snake_Enviro(); //environment instance, runs the game until broken by an end of life event

	if(snake_reset){
        //at end of life, show the game over screen
        Graphics_printText(colours[4],19,18, "GAME OVER");
        Graphics_printText(colours[4],13,20, "PRESS KEY2 TO CONTINUE");
        Graphics_printText(colours[4],15,22, "PRESS KEY3 FOR MENU");
		
        if(*BUTN == 0x8) snake_button[0] = 1;
        if(*BUTN == 0x4) snake_button[1] = 1;
        
        if(snake_button[1] & *BUTN != 0x4) { //button 2 runs snake game again
        	//reset button states
        	snake_button[0] = 0;
            snake_button[1] = 0;
            //start new snake game
            snake_newGame = true;
            snake_reset = false;

        } else if (snake_button[0]& *BUTN != 0x8) { //button 3 runs game menu
            game_selection = 0;
            HPS_IRQ_unregisterHandler(HPS_TIMER0_IRQ);
            //reset button states
            snake_button[0] = 0;
            snake_button[1] = 0;
            newMenu = true; //menu screen will run
            snake_startgame = true; //toggle on for when user enters snake game again
            snake_reset = false;
        }
        ResetWDT();
	}

}

/* Function to run the snake game */ 
void Snake_Enviro(void){

	//Creates game environment once
	if(snake_newGame){

		Snake_init(); //call snake instance
		Snake_drawBox(colours[4],0,0,5,5,235,315); //setup environment wall
        
        // set variables to default
		snake_reset = false;
		snake_initialised = false;
		food_isSpawned = false;
		food_isClear = true;
		snake_isPlaying = true;
		snake_newGame = false;

        // Set the background audio sample counter to zero 
        // and get the total audio samples of the background audio       
        snake_audio = 0;
        snake_totalAudio = sizeof(snake_Audio)/sizeof(snake_Audio[0]); // Total samples in audio file
        
        // To play the background music, we attach the Hero_backAudio() function to timer0 
        // IRQ interrupt. This function loads the audio codec with the next audio sample.
        // First we disable the timer, load it with our desired period, and then enable the timer
        // The timer operates at 100MHz. 2083 achieves a frequency of 48KHz (audio sampling frequency)
        // See HPS_Timers.c for more details about the timer functions    
        
        Timer0_disable();       // disable timer0
        Timer0_load(2083);      // load with our desired value
        Timer0_enable();        // enable the timer

        // We now attach our audio function to timer0 IRQ
        HPS_IRQ_registerHandler(HPS_TIMER0_IRQ, Snake_backAudio);
        ResetWDT();
        
		snake_lastTime[0] = Timer_currentValue();
		snake_lastTime[1] = Timer_currentValue();
	}

	//while game is playing the snake dynamics
	// should run (snake body and food)
	if(snake_isPlaying){
		snake_currentTime = Timer_currentValue();
		Snake_grow();
		Snake_destruct();
		snake_execute(&Snake_dynamics);
		food_execute(&Snake_food);
	}
    Timer_clearInterrupt();
    ResetWDT();
}

/* Function to set a snake in the environment */
void Snake_dynamics(void){
	Snake_direction();
	Snake_create(colours[1],colours[7]);
	Snake_clear(colours[1],colours[7]);

}

/* Function to destroy the snake when it hits itself or a wall */
void Snake_destruct(void){
    
	//Detect an obstacle in all four directions
	if(snake_dir[0] && snake_map[head_xpose][head_ypose-1] == 8){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}
	else if(snake_dir[1] && snake_map[head_xpose][head_ypose+1] == 8){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}
	else if(snake_dir[2] && snake_map[head_xpose-1][head_ypose] == 8){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}
	else if(snake_dir[3] && snake_map[head_xpose+1][head_ypose] == 8){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}

	//Detect a snake body in all four directions
	else if(snake_dir[0] && snake_map[head_xpose][head_ypose-1] == 1){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}
	else if(snake_dir[1] && snake_map[head_xpose][head_ypose+1] == 1){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}
	else if(snake_dir[2] && snake_map[head_xpose-1][head_ypose] == 1){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}
	else if(snake_dir[3] && snake_map[head_xpose+1][head_ypose] == 1){
		snake_reset = true;
		snake_isPlaying = false;
		usleep(1000000);
	}

}

/* Function to spawn the food  */
void Snake_food(void){
    //this function is to spawn food at certain coordinates in screen
    //it has to be random and not at the points where
    //the snake already exists
	//food coordinates
	unsigned int i, j;

	//check field 3x3 from chosen coordinate for snake
	//or obstacle
	if(!food_isSpawned){
		get_x();
		get_y();
		for(i= food_ypose; i<food_ypose+FOOD_SIZE; i++){
			for(j = food_xpose; j< food_xpose+FOOD_SIZE; j++){
				if(snake_map[j][i] != 0 ){
					food_isClear = false;
				}
			}
		}
	}
	//create the food
	if(food_isClear && !food_isSpawned){
		for(i = food_ypose; i < food_ypose+FOOD_SIZE; i++){
			for(j = food_xpose; j< food_xpose+FOOD_SIZE; j++){
				LT24_drawPixel(colours[0],j,i);
				snake_map[j][i] = 10;
			}
		}
		food_isSpawned = true;
	}

}

//get random x coordinates within environment limits
void get_x(void){
	unsigned int x;
	x = (rand()%(X_MAX-X_MIN)+ X_MIN);

	//we want coordinate to start at an even track
	if(x%2==0){
		food_xpose = x;
	}else{
		x++;
		food_xpose = x;
	}
}

//get random y coordinates within environment limits
void get_y(void){
	unsigned int y;
	y = (rand()%(Y_MAX-Y_MIN) + Y_MIN);

	//we want coordinate to start at an even track
	if(y%2==0){
		food_ypose = y;
	}else{
		y++;
		food_ypose = y;
	}
}
/* Function to exexute Snake_dynamics() */
void snake_execute(s_func p){
	if((snake_lastTime[0] - snake_currentTime) >= snake_periods[0]){
		//This  execution function takes in Snake_dynamics() and runs
		//it at an appropriate rate (9Mcycles/231.25MHz)
		p();

		//Snake_test();
		snake_lastTime[0] -= snake_periods[0];
	}
}

/* Function to exexute Snake_food() */
void food_execute(s_func p){
	if((snake_lastTime[1] - snake_currentTime) >= snake_periods[1]){
		//This execution function takes in Snake_food() and runs
		//at a rate much faster than the snake motion. (1Mcycles/231.25MHz)
		p();

		//Snake_test();
		snake_lastTime[1] -= snake_periods[1];
	}
}
