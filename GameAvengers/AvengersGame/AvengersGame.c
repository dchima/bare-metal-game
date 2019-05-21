/*
 * AvengersGame.c
 *
 *  Created on: Mar 21, 2018
 *      Author: Azeem
 */

#include "AvengersGame.h"
#include "AvengersEnv.h"



bool avengers_newLaunch = true; // bool to decide if we need to load the launch media

// This is intended to be the Avengers_Game function that will just be called in the main.c
signed int Avengers_main (void) {

    // If newLaunch is requested, Start from launch screen and set new launch to false
    if(avengers_newLaunch) {
        Avengers_launchScene();
        Avengers_selectScene();
        avengers_newLaunch = false;
    }
    return AVENGERS_SUCCESS;
}

