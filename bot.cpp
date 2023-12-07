#include "declarations.h"

bot::bot(): player()
{   
    player_name="Bot "+to_string(player_ID);
    while (true){
        cout<<"Money in account of "<<player_name<<" : ";
        cin>>money_in_hand;
        if (money_in_hand>=large_blind) break;
        else cout<<"Invalid, player must start with more than large blind\n";
    }
}


void bot::play_move() //just matches the current bet as of now
{
    played_first_move_in_round=true;
    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
    
}