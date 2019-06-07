/* 
 * File:   Deck.cpp
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Implementation of Deck class
 */

//System Library
#include <algorithm>    //For shuffle
#include <iostream>
#include <vector>
using namespace std;

//User Library
#include "Deck.h"

//Constructor with 1 argument
Deck::Deck(int n){
    nCards = n;
    current = 0;

    for (int i=0; i<nCards; i++) {
        cards.push(Card(i));
    }
}

//Shuffles deck
void Deck::shuffle() {  
    //Create temp vector
    vector<Card> temp;
    while (!cards.empty()) {
        temp.push_back(cards.top());
        cards.pop();
    }
    //Shuffle deck
    //random_shuffle(temp.begin(), temp.end());
    shuffleRec(temp, 0);    //Shuffle recursively instead
    
    //Push shuffled deck into stack
    for (int i=0; i<nCards; i++) {
        cards.push(temp[i]);
    }
}

//Shuffle deck recursively
void Deck::shuffleRec(vector<Card> &v, int pos) {
    if (v.size() - pos < 2)
        return;
    int rnd = pos + 1 + rand()%(v.size() - pos - 1);
    swap(v[pos], v[rnd]);
    shuffleRec(v, pos+1);
}

//Recursive function to insert item sorted
void Deck::stkInsert(stack<Card> &s, Card x) { 
    // Base case: Either stack is empty or newly inserted 
    // item is greater than top (more than all existing) 
    if (s.empty() || x.getNum() > s.top().getNum()) { 
        s.push(x); 
        return; 
    } 
  
    // If top is greater, remove the top item and recur 
    Card temp = s.top();
    s.pop();
    stkInsert(s, x); 
  
    // Put back the top item removed earlier 
    s.push(temp); 
} 

//Sorts deck
void Deck::stkSort(stack<Card> &crd) {
    //If stack of cards not empty 
    if (!crd.empty()) { 
        //Remove the top item 
        Card x = crd.top();
        crd.pop();
  
        //Sort remaining stack 
        stkSort(crd); 
  
        //Push the top item back in sorted stack 
        stkInsert(crd, x); 
    }
}

void Deck::sortMrg() {
    vector<Card> temp;          //To hold deck of cards temporarily
    int total = cards.size();   //Size of deck
    
    //Push cards to vector
    for (int i=0; i<total; i++) {
        Card crd = cards.top();
        temp.push_back(crd);
        cards.pop();
    }
    
    mrgSort(temp, 0, 52); //Sort the vector
    
    //Push sorted cards to deck
    for (int i=0;i<temp.size();i++) {
        cards.push(temp[i]);
    }
}
  
//Merge function for merge sort
void Deck::merge(vector<Card> &hilow, int beg, int nlow, int nhigh) {
    //Create a merged array
    int span = nhigh - beg; //Span the range to merge
    int cntl = beg, cnth = nlow; //Independent counters to merge
    int *c = new int[span]; //Allocate Memory
    //Fill the array
    for (int i = 0; i < span; i++) {
        if (cntl == nlow) {
            c[i] = hilow[cnth++].getNum();
        } else if (cnth == nhigh) {
            c[i] = hilow[cntl++].getNum();
        } else if (hilow[cntl].getNum() < hilow[cnth].getNum()) {
            c[i] = hilow[cntl++].getNum();
        } else {
            c[i] = hilow[cnth++].getNum();
        }
    }

    //Copy back and deallocate memory
    for (int i = 0; i < span; i++) {
        hilow[beg + i] = c[i];
    }
    delete []c;
}

//Merge sort
void Deck::mrgSort(vector<Card> &a, int beg, int end) {
    int center = (beg + end) / 2;
    if ((center - beg) > 1)
        mrgSort(a, beg, center);
    if ((end - center) > 1)
        mrgSort(a, center, end);
    merge(a, beg, center, end);
}

//Print deck
void Deck::prntDck() const {
    stack<Card> temp = cards;
    cout << "Deck: ";
    while (!temp.empty()) {
        cout << temp.top().getVal() << temp.top().getSuit() << " ";
        temp.pop();
    }
    cout << endl;
}

//Deal a card
Card Deck::deal() {
    //Deal card as long as deck is not empty
    if (!cards.empty()) {
        Card top = cards.top();
        cards.pop();
        return top;
    }
    else {
        cout << "No more cards available.\n";
    }
}

//Checks if deck is empty or not
bool Deck::isEmpty() {
    //Conditional operator example
    bool state;
    (cards.empty()) ? state = true : state = false;
    return state;
}