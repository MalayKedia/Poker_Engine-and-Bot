#include "declarations.h"

player::player(): player_ID(no_of_players_static), in_game(true), bet_in_round(0){
    player_hand.deal_random_cards(2);
    players_in_game.push_back(this);
    all_players.push_back(this);
    no_of_players_static++;
}

/*
player::~player(){
  no_of_players_static--;
  player_hand.submit();
  cout<<"\n\n\nDestructor for player called, INVALID\n\n";
}*/

bool player::collect_bet(int amount){   
    if (money_in_hand<amount){
        cout<<"Error, not enough balance in acc of "<<player_name<<" to pay "<<amount<<endl;
        return false;
    }
    money_in_hand-=amount;
    bet_in_round+=amount;
    pot_amount+=amount;
    cout<<"Collecting bet of "<<amount<<" from "<<player_name<<", money left in account of "<<player_name<<" is "<<money_in_hand<<endl;
    cout<<"The pot now is "<<pot_amount<<endl;

    return true;
}

void player::check(){
    cout<<player_name<<" chose to check, passing turn to next player\n";
}

bool player::raise(int raise_amount){
    bool raise_possible= (this->collect_bet(current_bet+raise_amount- bet_in_round));
    if (raise_possible) {
      current_bet+=raise_amount;
      cout<<"Successfully raised by "<<raise_amount<<", the current bet now is "<<current_bet<<endl;
    }
    else cout<<"Cannot raise by "<<raise_amount<<" ,try again\n";
    return raise_possible;
}

void player::fold(){
    cout<<"Player "<<player_name<<" has folded and is now out of game\n";
    in_game=false;

    int index;                        
    for (int i=0; i<players_in_game.size(); i++) if (players_in_game[i]==this) index=i; 
    players_in_game.erase(players_in_game.begin()+index);
}