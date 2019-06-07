/* 
 * File:   Card.h
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Implementation of Card class
 */

//System Libraries
#include <array>
using namespace std;

//User Libraries
#include "Card.h"

//Constructor
Card::Card(int num) {
    //Validate number
    if (num>51) {
        number = num%52;
    }
    else if (num>=0 && num<52) {
        number = num;
    }
    else {
        throw NegVal();
    }
    setSuit();  //Set suit based on number
    setVal();  //Set face based on number
}

//Set suit value based on number member variable
void Card::setSuit() {
    array<string,4> symbols = { "\xe2\x99\xa5", "\xe2\x99\xa3", 
                                "\xe2\x99\xa6", "\xe2\x99\xa0" };
    suit = symbols[number/13];
}

//Set card value based on number member variable
void Card::setVal() {
    array<char,13> values = { '2', '3', '4', '5', '6', '7', '8', 
                              '9', 'T', 'J', 'Q', 'K', 'A' };
    val = values[number%13];
}

//Set card value based on number member variable
void Card::setNum(int num) {
    number = num;
    this->setSuit();
    this->setVal();
}