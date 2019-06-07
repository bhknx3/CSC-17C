/* 
 * File:   Game.h
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Specification of Game class
 */

#ifndef GAME_H
#define GAME_H

//System Libraries
#include <deque>
#include <map>
#include <unordered_map>

//User Libraries
#include "Player.h"
#include "Deck.h"

class Game {
    private:
        std::deque<Card> pile;      //Pot pile where players place their cards
        int nPlayer;                //Number of players in game
        int rounds;                 //Total number of turns in game (cards placed)
        float timer;                //Show time taken
        Player **players;           //Pointer to players
        std::map<char,int> numVal;  //Store numerical value of cards
        bool showOut;               //Toggle showing each output in game
        std::unordered_map<std::string,Player*> hashTbl; //Hash table
        
        //Private member functions
        void init();        //Initialize & setup game
        void play();        //Play game - each player places cards
        void menu();        //Menu
        void toggle();      //Toggle for output in menu
        
        //Game-related functions
        void dealAll(Deck &, int);   //Deal cards to players
        bool chkWin();      //Check if game has a winner
        bool slapOk();      //Check if pile is slappable
        bool chkFace();     //Check if top of pile is a face/ace card
        int slap();         //Player slap
        void getPile(int);  //Player takes pile
        void putCard(int);  //Player places card
        int nextP(int);     //Get next player
        void setNVal();     //Set numerical values of cards
        void facePlay(int&, int&, int&, bool&, int);    //Gameplay logic when a face card is placed
        
        //Output functions
        void dspWin();      //Display winning statements
        void dspStat();     //Display statistics
        void pntPile();     //Print contents of pile
        void hashInfo();    //Hash information
        void lookUp();      //Look up hashed information
        
        //File operations
        void fileOut();     //Write data to file
        void fileIn();      //Read data from file
        
    public:
        //Constructors
        Game();
        Game(int);
        
        //Destructor
        ~Game();

        //Getter functions
        int getNumP() const {return nPlayer;}
        int getRnd() const {return rounds;}

        //Functions - operation
        void run();         //Runs game
};

#endif /* GAME_H */