#include "declarations.h"

player::player(int id, string name){
    player_ID=id; 
    player_name=name;
    deal_cards(2,player_hand);
    cout<<"Money in account of "<<player_name<<" :";
  //  cin>>money_in_hand;
}

player::player(int id){
    player_ID=id; 
    player_name="Bot "+to_string(id);
    deal_cards(2,player_hand);
    cout<<"Money in account of "<<player_name<<" :";
   // cin>>money_in_hand;
}