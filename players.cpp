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

bool player::collect_bet(int amount){   
    if (money_in_hand<amount){
        cout<<"Error, not enough balance in acc of "<<player_name<<" to pay "<<amount<<endl;
        return false;
    }
    money_in_hand-=amount;
    bet_in_round+=amount;
    pot_amount+=amount;
    cout<<"Collecting bet of "<<amount<<" from "<<player_name<<", money left is "<<money_in_hand<<endl;
    cout<<"The pot now is "<<pot_amount<<" and current bet is "<<current_bet<<endl;

    return true;
}

bool player::raise(int raise_amount){
    return (this->collect_bet(current_bet+raise_amount- bet_in_round));
}

void player::fold(){
    cout<<"Player "<<player_name<<" has folded and is now out of game\n";
    in_game=false;

    int index;                        
    for (int i=0; i<players_in_game.size(); i++) if (players_in_game[i]==this) index=i; 
    players_in_game.erase(players_in_game.begin()+index);
}