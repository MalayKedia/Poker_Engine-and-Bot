#include "declarations.h"

void deal_cards(int number, deck &hand){ //number is number of cards to deal to the deck hand 

    for (int i=0; i<number; i++){
        bool undealed_card_found;
        do{
            int n=52*(rand()/(1.0+RAND_MAX));
            undealed_card_found=undealed_cards.remove(card(n/13,n%13));
            if (undealed_card_found) hand.add(card(n/13,n%13));
        } while (!undealed_card_found);
    }
}

void begin_game(){

    srand(time(NULL));

    for (int i=0; i<13;i++){
       for (int j=0; j<4; j++){
            undealed_cards.add(card(j,i));
        }
    }
    deal_cards(5, community_cards);
    pot_amount=0;

}

void collect_bet(player &p, int amount){
    if (p.money_in_hand<amount){
        cout<<"Error, not enough balance in acc of "<<p.player_name<<" to pay "<<amount<<endl;
        return;
    }
    p.money_in_hand-=amount;
    p.bet_in_round+=amount;
    pot_amount+=amount;
    cout<<"Collecting bet of "<<amount<<" from "<<p.player_name<<", money left is "<<p.money_in_hand<<endl;
    cout<<"The pot now is "<<pot_amount<<" and current bet is "<<current_bet<<endl;
}

void collect_small_large_bet(int starting_player_index){
    current_bet=small_blind;
    collect_bet(*players_in_game[starting_player_index], small_blind);
    current_bet*=2;
    collect_bet(*players_in_game[(starting_player_index+1)%no_of_players], small_blind*2);
    cout<<endl;
}

void initialise_round(){
    vector<player*>:: iterator plr = players_in_game.begin();
    for (int i=0; i<players_in_game.size(); i++) plr[i]->bet_in_round=0;
    current_bet=0;
}

void initiate_betting(){
    
}