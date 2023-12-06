#include "declarations.h"

bot::bot(): player()
{   player_name="Bot "+to_string(player_ID);
    
    cout<<"Money in account of "<<player_name<<" : ";
    cin>>money_in_hand;
}


void bot::play_move(){

    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
    
}