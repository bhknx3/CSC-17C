/* 
 * File:   Player.h
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 11:25 PM
 * Purpose: Specification of Player class
 */

#ifndef PLAYER_H
#define PLAYER_H

//System Libraries
#include <queue>

//User Libraries
#include "Card.h"

class Player {
    private:
        std::string name;       //Name of player
        std::queue<Card> hand;  //Cards in hand
        //Data variable per player
        int turns;              //Total amount of cards placed by player
        int slaps;              //Total amount slaps
        int wins;               //Total amount of wins
    public:
        //Constructors
        Player();
        Player(std::string);
        
        //Getter functions
        std::string getName() const {return name;}
        int getNCrd() const {return hand.size();}
        int getTurn() const {return turns;}
        int getSlap() const {return slaps;}
        int getWins() const {return wins;}
        
        //Functions
        void addTurn() {turns++;}   //Add turn counter
        void addSlap() {slaps++;}   //Add slap counter
        void addWins() {wins++;}    //Add win counter
        void addCard(Card);         //Add a card to player hand
        Card rmvCard();             //Remove card from hand
        void prtHand() const;       //Display player hand
        bool noHand();              //Check if there are cards in players hand
};

#endif /* PLAYER_H */