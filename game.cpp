#include "declarations.h"

void reorder_players_in_game(int starting_player_index){       //reorders the players in game st it starts from player we want
    vector<player*> new_list;
    for (int i=0; i<players_in_game.size(); i++){
        new_list.push_back(players_in_game[(starting_player_index+i)%players_in_game.size()]);
    }
    players_in_game=new_list;
}

void deal_cards(int number, deck &hand){                        //number is number of cards to deal to the deck hand 

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

    //setting undealed card deck to standard 52 deck
    for (int i=0; i<13;i++){
       for (int j=0; j<4; j++){
            undealed_cards.add(card(j,i));
        }
    }
    //dealing 5 cards to community cards
    deal_cards(5, community_cards);
    pot_amount=0;

}

void collect_small_large_blind(int starting_player_index=0){  //collects the small blind and large blind
    current_bet=small_blind;
    if (!(players_in_game[starting_player_index]->collect_bet(small_blind))){
            players_in_game[starting_player_index]->fold();
            
            collect_small_large_blind(starting_player_index);
    }
    else collect_large_blind(starting_player_index);
}

void collect_large_blind(int starting_player_index){
        current_bet=2*small_blind;
        if(!(players_in_game[(starting_player_index+1)%no_of_players]->collect_bet(small_blind*2))){
            players_in_game[(starting_player_index+1)%no_of_players]->fold();

            collect_large_blind(starting_player_index);
        }
        cout<<endl;
}

void initialise_round(){
    vector<player*>:: iterator plr = players_in_game.begin();
    for (int i=0; i<players_in_game.size(); i++) plr[i]->bet_in_round=0;
    current_bet=0;
}

bool round_over(){
    bool over=true;
    for (int i=0; i<players_in_game.size(); i++) over=over&&(players_in_game[i]->bet_in_round=current_bet);
    return over;
}

void initiate_betting(int start_index=0){
    vector<player*>:: iterator plr = players_in_game.begin()+start_index%players_in_game.size();

    for(; plr<=players_in_game.end(); plr++){
        plr[0]->play_move();
    }    

    while (!round_over()){
        player* player_move=*plr;
        player_move->play_move();

        if (plr!=players_in_game.end()) plr++;
        else plr=players_in_game.begin();
    }
}
