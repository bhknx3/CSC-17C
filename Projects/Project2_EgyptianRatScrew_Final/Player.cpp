/* 
 * File:   Player.cpp
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 11:25 PM
 * Purpose: Implementation of Player class
 */

//System Libraries
#include <iostream>
#include <iterator>
using namespace std;

//User Library
#include "Player.h"

//Constructor 1
Player::Player() {
    name = "";
    turns = 0;
    slaps = 0;
    wins = 0;
}

//Constructor 2 w/ argument for name
Player::Player(string n) {
    name = n;
    turns = 0;
    slaps = 0;
    wins = 0;
}

//Add card to players hand
void Player::addCard(Card card) {
    //Add card to hand
    hand.push(card);
}

//Remove card from players hand
Card Player::rmvCard() {
    //Set variable to card in front of hand
    Card temp = hand.front();
    //Delete first card in hand
    hand.pop();
    
    return temp;
 }

//Print players hand
void Player::prtHand() const {
    //Copy elements in queue
    queue<Card> temp = hand;
    //Output cards in hand
    cout << name << "'s hand: ";
    while (!temp.empty()) {
        cout << temp.front().getVal() << temp.front().getSuit() << " ";
        temp.pop();
    }
    cout << endl;
}

//Signals no cards in players hand
bool Player::noHand() {
    if (hand.size() == 0) {
        return true;
    }
    else {
        return false;
    }
}