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

struct card{
    int suit, value;
    //suit lies between 0 and 3, and value lies between 0 and 12

    card(int, int);

    friend ostream & operator<<(ostream &ost, card const &c);
    friend bool operator==(card const &c1, card const &c2);
};

struct deck{
//This contains a list of cards
vector<card> card_list;
int deck_size;

deck();
deck(deck const &d);

void add(card c);
bool remove (card c);

friend ostream & operator<<(ostream &ost, deck const &d);

};

struct player{
    int player_ID;
    string player_name;
    deck player_hand;
    double money_in_hand;

    player(int id, string name);
    player(int id);
};


deck undealed_cards, board_cards;
extern deck undealed_cards, board_cards;        //global variable

//double total_bet_on_board=0, bet_amount;
//extern double total_bet_on_board, bet_amount;

#endif