/* 
 * File:   Game.cpp
 * Author: Benjamin Kwon
 * Created on April 7, 2019, 8:00 PM
 * Purpose: Implementation of Game class
 */

//System Libraries
#include <algorithm>    //Algorithms (ex: reverse, sort, etc)
#include <chrono>       //For time functions
#include <cstdlib>      //rand()
#include <fstream>      //For file I/O
#include <iomanip>      //Formatting
#include <iostream>     //Terminal I/O
#include <iterator>     //Iterators
#include <list>         //List
#include <sstream>      //Stringstream
#include <string>       //String
#include <unordered_set>//Set (unordered)
#include <utility>      //Pair
using namespace std;

//User Libraries
#include "Deck.h"
#include "Game.h"

//Constructor without argument
Game::Game() {
    nPlayer = 0; //Number of players
    rounds = 0; //Total turns
    showOut = true;
    players = nullptr;
    setNVal();
}

//Constructor 2 with argument (number of players)
Game::Game(int n) {
    nPlayer = n; //Number of players
    rounds = 0; //Total turns
    showOut = true;
    players = nullptr;
    setNVal();
}

//Destructor
Game::~Game() {
    if (nPlayer > 0) {
        for (int i = 0; i < nPlayer; i++) {
            delete players[i];
        }
        delete []players;
    }
}

//Run game
void Game::run() {
    menu();
}

//Initialize game (Create players, make, shuffle, and distribute deck)
void Game::init() {
    //Create pointer to array of players
    players = new Player*[nPlayer];
    for (int i = 0; i < nPlayer; i++) {
        //Can get string input here from user for player names if needed
        //Otherwise, name Player1,2,..etc
        //Use sstream to make player name
        stringstream ss;
        ss << "Player" << i + 1;
        string name;
        ss >> name;
        //Create each player
        players[i] = new Player(name);
    }

    //Declare, initialize variable for number of cards
    const int size = 52;

    Deck deck(size); //Create a deck (sorted)
    deck.shuffle(); //Shuffle deck recursively

    //Sort deck using either method (to show sort works)
    deck.sortStk();
    //deck.sortMrg();

    deck.shuffle(); //Shuffle again

    if (showOut) {
        //Print deck
        deck.prntDck();
    }

    //Recursively deal cards from deck to players
    dealAll(deck, 0);

    if (showOut) {
        //Output players cards
        cout << "\nDistributing cards:\n";
        for (int i = 0; i < nPlayer; i++) {
            players[i]->prtHand();
        }
        cout << endl;
    }
}

//Deal cards to players
void Game::dealAll(Deck &d, int n) {
    if (d.isEmpty()) return;
    if (n >= nPlayer) n = 0;
    Card card = d.deal();
    players[n]->addCard(card);
    dealAll(d, n + 1);
}

//Play game
void Game::play() {
    //Variables
    chrono::time_point<chrono::system_clock> start, end; //Time variables
    int crntP = 0; //Store current player, start with player1
    int counter; //Counter
    int collect; //Hold player whose turn is to collect pile
    bool cont; //Flag to signal continuation of logic
    start = chrono::system_clock::now(); //Timer start

    //Loop until winner is found
    do {
        counter = 0; //Counter for number of cards player must place
        cont = true; //Flag to continue or not

        //Check for undefined behavior
        if (pile.size() == 0) {
            //Player places card on pile
            if (players[crntP]->getNCrd() != 0) {
                players[crntP]->addTurn();
                putCard(crntP);
                if (chkFace()) {
                    collect = crntP; //If card placed is face/ace, store player
                }
            }
            //Check if pile meets criteria for a slap
            if (slapOk()) {
                //Find player who won slap
                crntP = slap();
            } else {
                //Change player to next player
                crntP = nextP(crntP);
            }
        }            //If the card in front of the pile is a Jack
        else if (pile.front().getVal() == 'J' && players[crntP]->getNCrd() != 0) {
            facePlay(crntP, collect, counter, cont, 1);
        }            //If the card in front of the pile is a Queen
        else if (pile.front().getVal() == 'Q' && players[crntP]->getNCrd() != 0) {
            facePlay(crntP, collect, counter, cont, 2);
        }            //If the card in front of the pile is a King
        else if (pile.front().getVal() == 'K' && players[crntP]->getNCrd() != 0) {
            facePlay(crntP, collect, counter, cont, 3);
        }            //If the card in front of the pile is an Ace
        else if (pile.front().getVal() == 'A' && players[crntP]->getNCrd() != 0) {
            facePlay(crntP, collect, counter, cont, 4);
        }            //Any other card
        else {
            //Player places card on pile
            if (players[crntP]->getNCrd() != 0) {
                players[crntP]->addTurn();
                putCard(crntP);
                if (chkFace()) {
                    collect = crntP; //If card placed is face/ace, store player
                }
            }
            //Check if pile meets criteria for a slap
            if (slapOk()) {
                //Find player who won slap
                crntP = slap();
            } else {
                //Change player to next player
                crntP = nextP(crntP);
            }
        }

        //Test - See output of each players hand and the pile
        if (showOut) {
            for (int i = 0; i < nPlayer; i++) {
                players[i]->prtHand();
            }
            pntPile();
            cout << endl;
        }
    } while (!chkWin()); //Until winning condition is met

    end = chrono::system_clock::now(); //Timer end
    chrono::duration<float> seconds = end - start; //Get time taken in seconds
    timer = seconds.count()*1000; //Store timer in milliseconds

    dspWin(); //Display winner
    dspStat(); //Display stats
    fileOut(); //Write data to file
    hashInfo(); //Hash player information
    cout << endl;
}

//Gameplay for when a face card is placed
void Game::facePlay(int &crntP, int &collect, int &counter, bool &cont, int tries) {
    do {
        //Current player places card into pile
        if (players[crntP]->getNCrd() != 0) {
            players[crntP]->addTurn();
            putCard(crntP);
            if (chkFace()) {
                collect = crntP; //If card placed is face/ace, store player
            }
        }
        //Check for face/ace card
        if (chkFace()) {
            crntP = nextP(crntP);
            cont = false;
        }
        //Check if pile meets criteria for a slap
        if (slapOk()) {
            //Get player who won slap, now his/her turn
            crntP = slap();
            cont = false;
        }
        counter++;
        //If no face/ace card was placed, player who placed J takes pile
        //If player doesn't have enough cards, allow pile to be taken
        if (cont) {
            if (counter == tries || players[crntP]->noHand()) {
                if (pile.size() != 0) {
                    if (showOut) {
                        cout << "Player " << collect + 1 << " collected the pile after placing a J\n";
                    }
                    getPile(collect);
                }
                crntP = collect;
                cont = false;
            }
        }
    } while (cont);
}

//Place card into pile
void Game::putCard(int crntP) {
    //Remove card from current player and push to front of pile
    pile.push_front(players[crntP]->rmvCard());
}

//Check if the pile/pot is slappable by a player
bool Game::slapOk() {
    //Get size of pile (# of elements in deque)
    int size = pile.size();

    //Check for slapping conditions
    if (size >= 2) {
        //Double (2 consecutive cards of same value)
        if (pile.at(0).getVal() == pile.at(1).getVal()) {
            if (showOut) {
                cout << "Pile has a double! | ";
            }
            return true;
        }
        //Top and Bottom of pile is same card
        if (pile.front().getVal() == pile.back().getVal()) {
            if (showOut) {
                cout << "Pile has the same top and bottom! | ";
            }
            return true;
        }
        //Marriage (Q & K next to each other)
        if ((pile.at(0).getVal() == 'Q' && pile.at(1).getVal() == 'K') ||
                (pile.at(0).getVal() == 'K' && pile.at(1).getVal() == 'Q')) {
            if (showOut) {
                cout << "Pile has a marriage! | ";
            }
            return true;
        }
    }
    if (size >= 3) {
        //Sandwich (Same 2 card values with another card in between the 2)
        if (pile.at(0).getVal() == pile.at(2).getVal()) {
            if (showOut) {
                cout << "Pile has a sandwich! | ";
            }
            return true;
        }
    }
    if (size >= 4) {
        //Four in a row (increasing)
        if (numVal[pile.at(0).getVal()]++ == numVal[pile.at(1).getVal()] &&
                numVal[pile.at(1).getVal()]++ == numVal[pile.at(2).getVal()] &&
                numVal[pile.at(2).getVal()]++ == numVal[pile.at(3).getVal()]) {
            if (showOut) {
                cout << "Pile has a four in a row! | ";
            }
            return true;
        }
        //Four in a row (decreasing)
        if (numVal[pile.at(0).getVal()]-- == numVal[pile.at(1).getVal()] &&
                numVal[pile.at(1).getVal()]-- == numVal[pile.at(2).getVal()] &&
                numVal[pile.at(2).getVal()]-- == numVal[pile.at(3).getVal()]) {
            if (showOut) {
                cout << "Pile has a four in a row! | ";
            }
            return true;
        }
        //Four in a row (cut-offs) - Checked manually
        if ((pile.at(0).getVal() == 'J' && pile.at(1).getVal() == 'Q' &&
                pile.at(2).getVal() == 'K' && pile.at(3).getVal() == 'A') ||
                (pile.at(0).getVal() == 'Q' && pile.at(1).getVal() == 'K' &&
                pile.at(2).getVal() == 'A' && pile.at(3).getVal() == '2') ||
                (pile.at(0).getVal() == 'K' && pile.at(1).getVal() == 'A' &&
                pile.at(2).getVal() == '2' && pile.at(3).getVal() == '3') ||
                (pile.at(0).getVal() == 'A' && pile.at(1).getVal() == 'K' &&
                pile.at(2).getVal() == 'Q' && pile.at(3).getVal() == 'J') ||
                (pile.at(0).getVal() == '2' && pile.at(1).getVal() == 'A' &&
                pile.at(2).getVal() == 'K' && pile.at(3).getVal() == 'Q') ||
                (pile.at(0).getVal() == '3' && pile.at(1).getVal() == '2' &&
                pile.at(2).getVal() == 'A' && pile.at(3).getVal() == 'K')) {
            if (showOut) {
                cout << "Pile has a four in a row! | ";
            }
            return true;
        }
    }
    //Else no conditions that allow slaps
    return false;
}

//Determines which player slapped first, fxn returns said player
int Game::slap() {
    //Create a set that will contain random rolls to determine slap winner
    unordered_set<int> randVal;

    //Loop until players all have a different number rolled
    do {
        //Roll a random number for each player
        for (int i = 0; i < nPlayer; i++) {
            randVal.insert(rand() % 10 + 1);
        }
        //If there are duplicate numbers, clear every value
        if (randVal.size() != nPlayer) {
            randVal.clear();
        } else {
            //Output random rolls
            if (showOut) {
                unordered_set<int>::iterator itr = randVal.begin();
                for (int i = 0; i < nPlayer; i++) {
                    cout << "P" << i + 1 << " rolled: " << *itr << ", ";
                    itr++;
                }
            }
        }
    } while (randVal.size() == 0);

    //Create iterator for set
    unordered_set<int>::iterator high;
    //Use iterator to find highest value in set
    high = max_element(randVal.begin(), randVal.end());
    //Get element corresponding to the highest value in the set
    int element = distance(randVal.begin(), high);

    //Output player who slapped first (Got highest roll)
    if (showOut) {
        cout << "| " << players[element]->getName() << " slapped the pile first!\n";
    }

    players[element]->addSlap(); //Add slap count for player
    getPile(element); //Player takes pile
    return element; //Player who took pile starts next turn
}

//Change player to next player
int Game::nextP(int n) {
    int p1 = 0; //Player 1
    int p2 = 1; //Player 2
    int p3 = 2; //Player 3
    int p4 = 3; //Player 4

    //2-Player mode
    if (nPlayer == 2) {
        if (n == p1) {
            return p2;
        } else if (n == p2) {
            return p1;
        }
    }        //3-Player mode
    else if (nPlayer == 3) {
        if (n == p1) {
            return p2;
        } else if (n == p2) {
            return p3;
        } else if (n == p3) {
            return p1;
        }
    }        //4-Player mode
    else if (nPlayer == 4) {
        if (n == p1) {
            return p2;
        } else if (n == p2) {
            return p3;
        } else if (n == p3) {
            return p4;
        } else if (n == p4) {
            return p1;
        }
    }
}

//Player takes pile and places it in their hand
void Game::getPile(int plyrNum) {
    //Reverse pile
    reverse(pile.begin(), pile.end());
    //Create iterator
    deque<Card>::iterator itr = pile.begin();
    //Add pile to player 1's hand by iteration
    while (itr != pile.end()) {
        players[plyrNum]->addCard(*itr);
        itr++;
    }
    //Clear pile
    pile.clear();
    rounds++; //Increase round count every time pile is taken
}

//Check if card placed is a face card or ace card
bool Game::chkFace() {
    //Get value at front of pile
    char value = pile.front().getVal();

    //Check if card is a face or ace card
    if (value == 'J' || value == 'Q' || value == 'K' || value == 'A') {
        return true;
    }
    return false;
}

//Check if the game has a winner
bool Game::chkWin() {
    //Create vector that contains card count for each player
    vector<int> check;
    for (int i = 0; i < nPlayer; i++) {
        check.push_back(players[i]->getNCrd());
    }

    int numZero = 0; //Counter for number of zeroes
    for (int i = 0; i < check.size(); i++) {
        //Check how many players have 0 cards in their hand
        if (check[i] == 0) {
            numZero++;
        }
    }
    //If only one player has more than 0 cards in hand, considered winner
    if (numZero == nPlayer - 1) {
        return true;
    } else {
        check.clear(); //Clear vector contents
        return false;
    }
}

//Find and display winner of game
void Game::dspWin() {
    //List containing the number of cards remaining for each player
    list<int> numCrd;
    for (int i = 0; i < nPlayer; i++) {
        numCrd.push_back(players[i]->getNCrd());
    }

    //Use iterator to find maximum value, which corresponds to the winner
    list<int>::iterator winner;
    winner = max_element(numCrd.begin(), numCrd.end());
    int player = distance(numCrd.begin(), winner);
    players[player]->addWins(); //Add win for player

    //Get player number and output winner
    cout << "Game Over\n";
    cout << "The winner is Player " << player + 1 << "!\n";
}

//Display statistics @ end of simulation
void Game::dspStat() {
    cout << fixed << setprecision(2); //2 decimal places
    cout << "\n--------------------------------\n";
    cout << "\t     Data";
    cout << "\n--------------------------------\n";
    cout << "Number of players: " << nPlayer << endl;
    cout << "Number of rounds : " << rounds << endl << endl;
    for (int i = 0; i < nPlayer; i++) {
        cout << "Number of turns by player " << i + 1 << ": " << players[i]->getTurn() << endl;
        cout << "Number of slaps by player " << i + 1 << ": " << players[i]->getSlap() << endl;
        cout << "Number of wins  by player " << i + 1 << ": " << players[i]->getWins() << endl;
        cout << endl;
    }
    cout << "Time taken to run game: " << timer << "ms" << endl;
    cout << endl;
}

//Print cards in pile - First value shown is top of pile
void Game::pntPile() {
    cout << "Pile: ";
    //Iterate through pile to output card value & suit
    deque<Card>::iterator itr;
    for (itr = pile.begin(); itr != pile.end(); itr++) {
        cout << itr->getVal() << itr->getSuit() << " ";
    }
    cout << endl;
}

//Output menu and get inputs
void Game::menu() {
    int input; //Input for menu option

    //Output menu
    cout << "----------------------------------\n"
            "\tEgyptian Rat Screw\n"
            "----------------------------------\n"
            "1. Run game simulation\n"
            "2. Toggle output (default: on)\n"
            "3. Game rules\n"
            "4. Read game file\n"
            "5. Search hash table\n"
            "6. Exit\n"
            "\nInput: ";

    //Get input w/ input validation
    do {
        cin >> input; //Get input
        cin.clear(); //Clear input buffer
        cin.ignore(10000, '\n');
        if (input < 1 || input > 6) {
            cout << "ERROR: Invalid input. Please enter a value between 1 and 6: ";
        }
    } while (input < 1 || input > 6);

    //Read input from user
    switch (input) {
            //Play game
        case 1: //Get input for number of players
            int num;
            cout << "Select number of players (2-4): ";
            do {
                cin >> num;
                cin.clear(); //Clear input buffer
                cin.ignore(10000, '\n');
                if (num < 2 || num > 4) {
                    cout << "ERROR: Invalid number of players. Try again: ";
                }
            } while (num < 2 || num > 4);
            cout << endl;
            //Assign number of players to variable
            nPlayer = num;
            init();
            play();
            menu();
            break;
            //Toggle output show status
        case 2: toggle();
            menu();
            break;
            //Output instructions
        case 3: cout << "\nGame rules:\n";
            cout << "Players are dealt cards face down from a standard deck of 52 cards.\n"
                    "Players place the top card in their hand in a pile face up until\n"
                    "a face card or an ace card is placed. The next player must then place\n"
                    "a determined number of cards on to the pile (J:1, Q:2, K:3; A:4).\n"
                    "This continues until only one player has cards in his or her hand.\n\n"
                    "But, there is also a slapping rule that overrides all situations.\n"
                    "The first player to correctly slap the pile may take all the cards\n"
                    "and place it on the bottom of their current hand.\n\n"
                    "The following are conditions in which a slap is allowed:\n"
                    "(Type and number of conditions may vary among individuals)\n"
                    "Double     - 2 cards of equivalent value are placed consecutively.\n"
                    "Sandwich   - Any card is between 2 cards of equaivalent value\n"
                    "Top Bottom - First card placed is the same as the last card\n"
                    "4 in a row - When four cards have values in ascending or descending order\n"
                    "Marriage   - When a queen and king is placed next to each other\n\n"
                    "For more information, go to the link below:\n"
                    "https://bicyclecards.com/how-to-play/egyptian-rat-screw/\n\n";
            menu();
            break;
            //Read game file
        case 4: fileIn();
            menu();
            break;
        case 5: lookUp();
            menu();
            break;
            //Exit program
        default: cout << "\nExiting program.\n";
            break;
    }
}

//Toggle output showing each turn in game
//Shows: players hands, pile, random generator for slap mechanic
void Game::toggle() {
    if (showOut == true) {
        showOut = false;
        cout << "Output: OFF\n";
    } else if (showOut == false) {
        showOut = true;
        cout << "Output: ON\n";
    }
    cout << endl;
}

//Set a numeric value for each card (used to calculate slapping condition(s))
void Game::setNVal() {
    //Set map of card
    numVal['A'] = 1;
    numVal['2'] = 2;
    numVal['3'] = 3;
    numVal['4'] = 4;
    numVal['5'] = 5;
    numVal['6'] = 6;
    numVal['7'] = 7;
    numVal['8'] = 8;
    numVal['9'] = 9;
    numVal['T'] = 10;
    numVal['J'] = 11;
    numVal['Q'] = 12;
    //numVal['K'] = 13;
    numVal.insert(pair<char, int>('K', 13)); //Example of pair
}

//Write file containing recorded game data
void Game::fileOut() {
    //Filename
    string fileN = "Output.dat";
    string fileN2 = "OutputFormatted.dat";

    //Create and open file
    ofstream file;
    ofstream file2;
    file.open(fileN.c_str(), ios::out);
    file2.open(fileN2.c_str(), ios::out);

    //If file is valid
    if (file) {
        //Formatting
        file << fixed << setprecision(2);
        //Write to file
        file << nPlayer << endl; //Number of players
        file << rounds << endl; //Number of rounds
        for (int i = 0; i < nPlayer; i++) {
            file << players[i]->getTurn() << endl; //Player turns (cards put down)
            file << players[i]->getSlap() << endl; //Player slaps
            file << players[i]->getWins() << endl; //Player wins
        }
        file << timer;
        //Close file
        file.close();
        //Output message confirming data writing
        cout << "Data successfully written to " << fileN << endl;
    }        //If file is invalid
    else {
        cout << "Could not open " << fileN << " for output.\n\n";
    }

    //If file2 is valid
    if (file2) {
        //Formatting
        file2 << fixed << setprecision(2);
        //Write to file
        file2 << "Number of Players: " << nPlayer << endl; //Number of players
        file2 << "Number of rounds : " << rounds << endl; //Number of rounds
        file2 << endl;
        file2 << "           Turns   Slaps   Wins\n";
        for (int i = 0; i < nPlayer; i++) {
            file2 << "Player " << i + 1;
            file2 << setw(7) << players[i]->getTurn(); //Player turns
            file2 << setw(7) << players[i]->getSlap(); //Player slaps
            file2 << setw(7) << players[i]->getWins(); //Player wins
            file2 << endl;
        }
        file2 << "\nTime taken to compute game: " << timer << "ms";
        //Close file
        file2.close();
        //Output message confirming data writing
        cout << "Data successfully written to " << fileN2 << endl;
    }        //If file2 is invalid
    else {
        cout << "Could not open " << fileN2 << " for output.\n\n";
    }
}

//Read in file and output contents
void Game::fileIn() {
    ifstream file; //Declare file
    string fileN; //Filename
    int input; //To hold input
    float fInput; //To hold input as float

    cout << "\n(Default game file: Output.dat)"
            "\nEnter name of file to open: ";
    getline(cin, fileN, '\n');

    //Open file
    file.open(fileN.c_str(), ios::in);

    if (file) { //If file is valid
        //Read and display record
        file >> input; //Get number of players
        cout << "\nNumber of players: " << input << endl;
        int n = input; //Store number of players
        file >> input; //Get number of rounds
        cout << "Number of rounds : " << input << endl << endl;
        for (int i = 0; i < n; i++) {
            file >> input; //
            cout << "Player " << i + 1 << " number of turns: " << input << endl;
            file >> input;
            cout << "Player " << i + 1 << " number of slaps: " << input << endl;
            file >> input;
            cout << "Player " << i + 1 << " number of wins : " << input << endl;
            cout << endl;
        }
        file >> fInput;
        cout << "Time taken to compute game: " << fInput << "ms" << endl;
        //Close file
        file.close();
        cout << "\nData successfully read.\n\n";
    } else {
        cout << "Could not open " << fileN << " for input.\n\n";
    }
}

//Put into hash table
void Game::hashInfo() {
    for (int i = 0; i < nPlayer; i++) {
        string str = "player";
        str += to_string(i + 1);
        hashTbl[str] = players[i];
    }
    //    //Iterator debug
    //    unordered_map<string, Player*>:: iterator itr; 
    //    cout << "\nAll Elements : \n"; 
    //    for (itr = hashTbl.begin(); itr != hashTbl.end(); itr++) { 
    //        //itr->first  stores key
    //        //itr->second stroes value
    //        cout << itr->first << "  " << itr->second->getTurn() << endl; 
    //    } 
}

//Function to search key in hash table
void Game::lookUp() {
    string key;
    cout << "\nEnter hash key (\"player1\", \"player2\", etc): ";
    cin >> key;
    cout << endl;

    if (hashTbl.find(key) == hashTbl.end()) { //Key found
        cout << key << " not found\n\n";
    } else { //Key not found
        cout << "Found " << key << "!\n";
        cout << "Last played game statistics for " << key << ":\n";
        cout << "Wins : " << hashTbl[key]->getWins() << endl;
        cout << "Turns: " << hashTbl[key]->getTurn() << endl;
        cout << "Slaps: " << hashTbl[key]->getSlap() << endl << endl;
    }
}