#include "declarations.h"

player::player(): player_ID(no_of_players_static), in_game(true), played_first_move_in_round(false) , bet_in_round(0)
{
    players_in_game.push_back(this);
    all_players.push_back(this);
    no_of_players_static++;
}

player::~player()
{
  no_of_players_static--;
  player_hand.submit();
}

void player::collect_bet(int amount) //deducts the amount from players balance and adds it to pot
{   
    money_in_hand-=amount;
    assert(money_in_hand>=0);
    bet_in_round+=amount;
    pot_amount+=amount;
    cout<<"Collecting bet of "<<amount<<" from "<<player_name<<", money left in account of "<<player_name<<" is "<<money_in_hand<<endl;
    cout<<"The pot now is "<<pot_amount<<endl;
}

void player::check() //allowed only if current bet is zero
{
    cout<<player_name<<" chose to check, passing turn to next player\n";
}

void player::open()
{
    assert(money_in_hand>=bet_amount);

    cout<<player_name<<" has chosen to open\n";
    current_bet=bet_amount;
    collect_bet(bet_amount);
    cout<<"The current bet now is "<<current_bet<<endl;
}

void player::call()
{
    assert(money_in_hand>=current_bet-bet_in_round);

    cout<<player_name<<" has chosen to call\n";
    collect_bet(current_bet-bet_in_round);
    cout<<"The current bet is still "<<current_bet<<endl;
}

void player::raise() //collects the raise and any leftover sum
{  
    assert(no_of_raises<max_no_of_raises);
    assert(money_in_hand>=current_bet+bet_amount-bet_in_round);

    cout<<player_name<<" has chosen to raise\n";
    current_bet+=bet_amount;
    no_of_raises++;
    collect_bet(current_bet-bet_in_round);
    cout<<"The current bet now is "<<current_bet<<endl;
}

void player::fold() //changes in_game to false and removes player from list players_in_game
{
    cout<<"Player "<<player_name<<" has folded and is now out of game\n";
    in_game=false;

    int index;                        
    for (int i=0; i<players_in_game.size(); i++) if (players_in_game[i]==this) index=i; 
    players_in_game.erase(players_in_game.begin()+index);
}