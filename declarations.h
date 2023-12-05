#ifndef POKER_H
#define POKER_H

#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <string>
#include <assert.h>
#include <iostream>
using namespace std;

#define no_of_players 5
#define small_blind 10

struct card{
    int suit, value;
    //suit lies between 0 and 3, and value lies between 0 and 12

    card(int, int);
    //constructor for card

    friend ostream & operator<<(ostream &ost, card const &c);
    friend bool operator==(card const &c1, card const &c2);
};

struct deck{
//This contains a list of cards as a vector and the size of the deck as deck size

vector<card> card_list;
int deck_size;

deck();                    //default constructor
deck(deck const &d);       //copy constructor

void add(card c);           //function to add a card c to deck
bool remove (card c);       //fuction returns false if card c is not in deck, and if c is in deck, it removes it and returns true

void print(int number, int start);      //prints cards from index start to start+number-1 in the deck

friend ostream & operator<<(ostream &ost, deck const &d);

};

struct player{
    int player_ID;                    //unique id for the player
    string player_name;               //name to be printed on the screen while refering to this player
    deck player_hand;                 //cards in hand of player, always a deck of size 2
    bool in_game;                     //returns false if player has folded
    int money_in_hand, bet_in_round;  

    player(int id, string name);
    player(int id);

    bool collect_bet(int amount);

    virtual void play_move(){};
    
    bool raise(int raise_amount);
    void fold();

};

deck undealed_cards, community_cards;               //decks as name suggests
int pot_amount, current_bet;                        //pot amt is total amt betted till the moment, and current bet is bet at that round

//global variable declaration
extern deck undealed_cards, community_cards;        
extern int pot_amount, current_bet;

//list of all players who havent folded
vector<player*> players_in_game;
extern vector<player*> players_in_game;


struct bot: public player       //subclass of player, autoplayed by computer
{
    bot(int id);    
    void play_move() override;

};

struct user: public player      //subclass of player, played by user
{
    user(int id, string name);
    void play_move() override;
        
};

//Declarations of all functions defined:

void reorder_players_in_game(int starting_player_index);       //reorders the players in game st it starts from player we want
void deal_cards(int number, deck &hand);                       //number is number of cards to deal to the deck hand

void begin_game();                                             //sets undealed card deck to standard 52 deck and deals 5 cards to community
   
void collect_small_large_blind(int starting_player_index);
void collect_large_blind(int starting_player_index);

void initialise_round();
bool round_over();
void initiate_betting(int start_index);
#endif