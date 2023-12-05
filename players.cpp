#include "declarations.h"

player::player(int id, string name): player_ID(id), player_name(name), in_game(true), bet_in_round(0)
{   deal_cards(2,player_hand);
    cout<<"Money in account of "<<player_name<<" :";
    cin>>money_in_hand;
    players_in_game.push_back(this);
}

player::player(int id): player_ID(id), in_game(true), bet_in_round(0)
{   player_name="Bot "+to_string(id);
    deal_cards(2,player_hand);
    cout<<"Money in account of "<<player_name<<" :";
    cin>>money_in_hand;
    players_in_game.push_back(this);
}