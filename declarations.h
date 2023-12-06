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

#define small_blind 10
#define large_blind 10

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

    deck();                    //default constructor
    deck(deck const &d);       //copy constructor

    void add(card c);           //function to add a card c to deck
    void remove (int i);        //function to remove card of index i
    bool remove (card c);       //fuction returns false if card c is not in deck, and if c is in deck, it removes it and returns true

    void deal_random_cards(int number);      //number is number of cards to deal from the undealed deck
    void submit();                           //all cards are removed from deck and added to undealed cards deck

    deck operator+(deck const d) const;
    friend ostream & operator<<(ostream &ost, deck const &d);
    void print(int number);                 //prints cards from index 0 to number-1 in the deck
    void print();                           //prints whole deck

    int deck_five_value();            //It operates on a deck of 5 cards and returns a 5 digit no, highest digit being deck rank, next 2 being high card and next 2 being next high card 
    int deck_seven_value();
};

struct player{
    int player_ID;                    //unique id for the player
    string player_name;               //name to be printed on the screen while refering to this player
    deck player_hand;                 //cards in hand of player, always a deck of size 2
    bool in_game;                     //returns false if player has folded
    int money_in_hand, bet_in_round;  
    static int no_of_players_static;

    player();
 //   ~player();

    bool collect_bet(int amount);      //

    virtual void play_move() {};
    
    void check();
    bool raise(int raise_amount);
    void fold();

};
int player::no_of_players_static=0;

deck undealed_cards, community_cards;               //decks as name suggests
int pot_amount, current_bet;                        //pot amt is total amt betted till the moment, and current bet is bet at that round
int no_of_players, starting_player_index;

//global variable declaration
extern deck undealed_cards, community_cards;        
extern int pot_amount, current_bet;
extern int no_of_players, starting_player_index;

//list of all players who havent folded
vector<player*> players_in_game, all_players;
extern vector<player*> players_in_game, all_players;


struct bot: public player       //subclass of player, autoplayed by computer
{
    bot();    
    void play_move() override;

};

struct user: public player      //subclass of player, played by user
{
    user();
    void play_move() override;
        
};

//Declarations of all functions defined:

void reorder_players_in_game(int starting_player_index);       //reorders the players in game st it starts from player we want
void bubbleSort_descending(const int inputArr[], int sortedArr[], int n);

void begin_game();                                             //sets undealed card deck to standard 52 deck, asks for no. of players and deals 5 cards to community

//starts the pre-flop round by taking default set small and large blind values from players at starting_player_index and next (if possible)
void collect_small_and_large_blind(int starting_player_index); 
void collect_large_blind(int starting_player_index);

void initialise_round();                                       //sets current bet and round bets of each player to be zero before start of betting 
bool betting_round_over();                                     //returns true if bet in round of each player in game matches current bet
void initiate_betting(int start_index);                        //continues betting round until round isnt over


#endif