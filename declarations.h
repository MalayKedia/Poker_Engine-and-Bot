#ifndef POKER_H
#define POKER_H

#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <assert.h>
#include <iostream>
using namespace std;

#define small_blind 10
#define large_blind 20
#define max_no_of_raises 3

struct card
{
    int suit, value;                          //suit lies between 0 and 3, and value lies between 0 and 12

    card(int, int);                           //constructor for card
};

ostream & operator<<(ostream &ost, card const &c);
bool operator==(card const &c1, card const &c2);

struct deck //contains a list of cards as a vector
{
    vector<card> card_list;

    deck();                                  //default constructor
    deck(deck const &d);                     //copy constructor

    card operator[](int i) const;

    bool present(card const &c) const;       //function to tell if card c is present in the deck
    void add(card const &c);                 //function to add a card c to deck
    void add(deck const &d);                 //function to add a deck d to deck
    bool remove (card const &c);             //fuction returns false if card c is not in deck, and if c is in deck, it removes it and returns true
    void remove (int i);                     //function to remove card at i th position in vector
    void remove (deck const &d);             //function to remove any cards presents in deck d from deck on which operated

    void deal_random_cards(int number);      //adds 'number' of random caards to deck from the undealed_cards deck
    void submit();                           //empties the deck and adds all cards to undealed_cards deck

    deck operator+(card const &c) const;
    deck operator-(card const &c) const;     //card c must be in deck, or program terminates
    deck operator+(deck const &d) const;

    void print();                           //prints whole deck

    vector<int> score_five_cards();   //operates on a deck of 5 cards and returns a vector which can be lexicographically compared to score any two decks of 5 cards 
    vector<int> score_seven_cards();   //operates on deck of 7 cards and returns highest possible score
};

deck operator+(card const &c1, card const &c2);
ostream & operator<<(ostream &ost, vector<int> const &v);
ostream & operator<<(ostream &ost, deck const &d);


struct player //It is assumed that there is no element uniquely in class player, it must be either in subclass user or in bot
{
    int player_ID;                    //unique id for the player, set by default
    string player_name;               //name to be printed on the screen while refering to this player
    deck player_hand;                 //cards in hand of player, always a deck of size 2
    bool in_game;                     //false if player has folded or is out of game
    bool played_first_move_in_round;  //true after placing first bet in each round
    int money_in_hand;                //money in hand of player
    int bet_in_round;                 //amount bet by player in round till the moment
    
    static int no_of_players_static;  //counter to assign player_ID on call of constructor

    player();                          //sets in game variables to default values and adds player to player lists 
    ~player();                         

    bool collect_bet(int amount);      //returns false if player doesnt have enough balance to raise by amount, else returns true, deducts the amount from players balance and adds it to pot

    virtual void play_move(int round_no) {};
    
    void check();                       //allowed only if current bet is zero
    bool raise(int raise_amount);       //returns false if player doesnt have enough balance to raise by amount, else returns true, and collects the raise and any leftover sum
    void fold();                        //changes in_game to false and removes player from list players_in_game
};
int player::no_of_players_static=0;     

int no_of_players, starting_player_index;
//no_of_players is number of players with which the game start, it isnt changed throughout the code
//starting_player_index is the ID of player who played the small blind in any game
vector<player*> players_in_game, all_players;
//players_in_game is list of all players who havent folded and are still in game
//all_players is a list in which index of list= player id
deck undealed_cards, community_cards;               
//undealed_cards initially holds all 52 cards, any card dealed to player hand or community card must be removed from it
//community_cards are visible to all players at each point in time. They are dealed so that they contain only revealed cards at any moment
int pot_amount;                        
//pot_amt is money available in pot at the moment
int current_bet, no_of_raises; 
//current_bet is bet every player has to match to stay in the game
//no_of_raises is the no of raises made in that round
int bet_amount;
//is equal to large_blind during first 2 rounds and after round 2 equals large blind*2

//global variable declaration
extern deck undealed_cards, community_cards;        
extern int pot_amount, current_bet, no_of_raises, no_of_players, starting_player_index;
extern vector<player*> players_in_game, all_players;

struct bot: public player       //subclass of player, autoplayed by computer
{
    bot();    
    void play_move(int round_no) override;
    deck unseen_cards;
    double prob_not_losing_against_player;

    void calculate_prob_round_1();
    void calculate_prob_round_2();
    void calculate_prob_round_3();
    void play_move_round_1();
    void play_move_round_2();
    void play_move_round_3();

};

struct user: public player      //subclass of player, played by user
{
    user();
    void play_move(int round_no) override;
        
};

//Declarations of all functions defined:

int rand_int_v_u(int v, int u=0);                              //This function returns random integer from u to v-1
void bubbleSort_descending(const int inputArr[], int sortedArr[], int n);
void reorder_players_in_game(int starting_player_index);       //circularly reorders the list players_in_game st it starts from player at index given

void set_up_game_environment();                                //sets undealed card deck to standard 52 deck, asks for no. of players, initialises game variables
void select_starting_player();                                 //randomly selects the player which starts the game and circularly reorders list so that it appears first in list 
 
void collect_small_and_large_blind();                          //starts the pre-flop round by taking default set small and large blind values from players 0 and 1                          
//by design of game, every player in game must be capable of paying the large blind
void deal_cards_to_players();                                  //randomly distributes two cards each to all players in list players_in_game
void initialise_round();                                       //sets current bet and round bets of each player to be zero before start of betting 
bool betting_round_over();                                     //returns true if bet in round of each player in game matches current bet
void initiate_betting_preflop();                               //continues betting round until round isnt over in preflop round
void initiate_betting(int round_no);                                       //continues betting round until round isnt over

void showdown(vector<player*> &winning_players);               //returns the vector having winning players
void end_game(vector<player*> &winning_players);                //prints the name of winning players and divides the pot among them

bool ask_if_continue();                                        //asks user if they want to play another game and returns true if user says yes
bool reset_for_next_game();                                    //returns true if next game is possible, if possible, redeals all cards and reinitialises internal variables

#endif