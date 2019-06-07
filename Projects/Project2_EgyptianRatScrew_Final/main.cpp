/* 
 * File:   main.cpp
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Simulation of Egyptian Rat Screw card game (Project 2)
 */

//System Libraries
#include <cstdlib>  //srand()

//User Libraries
#include "Game.h"

//Main
int main(int argc, char** argv) {
    //Seed random number generator
    srand(static_cast<unsigned int>(time(0)));
    
    Game game;  //Create game
    game.run(); //Run game
    
    return 0;
}