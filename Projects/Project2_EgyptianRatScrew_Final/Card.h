/* 
 * File:   Card.h
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Specification of Card class
 */

#ifndef CARD_H
#define CARD_H

//System Libraries
#include <string>

class Card {
    private:
        int number;         //Numerical range of cards, used to create card
        char val;           //Value of card
        std::string suit;   //Suit of card
    public:
        //Exception class
        class NegVal {};    //Empty class declaration
        
        //Constructor
        Card(int);
        
        //Getter functions
        int getNum() const {return number;}
        char getVal() const {return val;}
        std::string getSuit() const {return suit;}
        
        //Setter functions
        void setVal();
        void setSuit();
        void setNum();
        void setNum(int);
};
    
#endif /* CARD_H */