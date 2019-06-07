/* 
 * File:   Deck.h
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Specification of Deck class
 */

#ifndef DECK_H
#define DECK_H

//System Library
#include <stack>
#include <vector>

//User Library
#include "Card.h"

class Deck {
    private:
        int nCards;                 //Total number of cards
        std::stack<Card> cards;     //Stack to store cards
        int current;                //Current card on top of deck
        
        //Sorts using recursion
        void stkSort(std::stack<Card>&);
        void stkInsert(std::stack<Card>&, Card);
        void mrgSort(std::vector<Card>&, int, int);
        void merge(std::vector<Card>&, int, int, int);
        
        //Shuffle
        void shuffleRec(std::vector<Card>&, int); //Shuffle recursively
        
    public:
        //Constructor
        Deck(int);
        
        //Getter functions
        int getNCrd() const {return nCards;}
        
        //Functions
        void shuffle();         //Shuffle deck
        Card deal();    //Deal card
        void prntDck () const;  //Print deck
        bool isEmpty();         //Flag for empty state
        
        //Sorting
        void sortStk() {stkSort(this->cards);}
        void sortMrg();
};

#endif /* DECK_H */