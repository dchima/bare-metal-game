/*
 * main.c
 *
 *  Created on: 12 Mar 2018
 *      Author: Dan
 */


#include "DE1SoC_LT24/DE1SoC_LT24.h"
#include "HPS_Watchdog/HPS_Watchdog.h"
#include "HPS_usleep/HPS_usleep.h"
#include "PrivateTimer/PrivateTimer.h"
#include "Graphics/Graphics.h"
#include "Snake_Engine/Snake_Enviro.h"






void exitOnFail(signed int status, signed int successStatus){
    if (status != successStatus) {
        exit((int)status); //Add breakpoint here to catch failure
    }
}



int main(void){

	unsigned short colours[8] = {LT24_RED,LT24_YELLOW,LT24_GREEN,LT24_CYAN,LT24_WHITE,LT24_BLUE,LT24_MAGENTA,LT24_BLACK};

	//creates snake instance
	exitOnFail(
			Snake_init(0xFF200060,0xFF200080), //Initialise LCD
			SNAKE_SUCCESS);
	ResetWDT();


	while(1){

		//move snake
		Snake_direction();
		Snake_create(colours[0],colours[7]);
		Snake_clear(colours[0],colours[7]);
		//Snake_test();
		usleep(50000);

		ResetWDT();
	}


}
