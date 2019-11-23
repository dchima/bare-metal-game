# HERO / SNAKE GAME ON BARE METAL

## Development
The De1-SoC development board is a hard processor system consisting of a Cyclone V SoC, an FPGA, and various hardware 
peripherals. The aim of this project is to program the board to demonstrate the versatility of the Cyclone V SoC device.

## Environment
The gaming environment was programmed in C language and consists of two games; Hero game and Snake game, and a menu 
from which the user can select a game. 

## Hero Game
The hero game is a one player shooter game designed to test the player’s speed and
reaction time. The player chooses a game avatar and starts the game with five live bars. The avatar is positioned at the 
bottom of the screen and can be moved in the horizontal axis. The game is played by shooting beams towards falling enemy ships. Each hit is recorded as a kill and adds to the player score. If the enemy reaches the bottom of the screen, the player losses a live bar. The game is over when the player runs out of life bars.

## Snake Game
The snake game is a single player game in which the player manoeuvres a constantly moving line within an environment. The game is played by directing the line to a food item on the screen. When the line collides with a food item, it increases in length and a new food item is spawned at a random location on the screen. The game is over when the line collides with an obstacle or itself.

## Features
Features of the interactive gaming environment includes
- Saving of high scores to the SD card
- Audo feedback 
- Imaging via shapes and text on the LCD, display of text on the seven-segment displays, 
- Button perihpery for inputs. 

## Design
Techniques used in the design include;
- peripheral interrupts
- task scheduling
- timer polling
- non-blocking polling of inputs
-use of structs.

## Drivers
All drivers used to connect the perihperals to the SoC was written by team:
- Timer drivers
- LCD drivers
- Graphics engine
- Button drivers
- Hex Led Drivers


## Gear
- Altari DE1-SoC development board. ALl drrivers are included in the repository download.
- Eclipse for altari development


**************************Diaclamer************************
Project development team:
  Chima Daniel Nnadika
  Azeem Oguntola
  
 Project written for the ELEC5620M Module for the University of Leeds summer semester 2018.
 All exernal code with propietary ownership tagged with full owners credentials.
