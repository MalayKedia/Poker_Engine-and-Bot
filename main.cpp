#include "declarations.h"
#include "card_and_deck.cpp"
#include "player.cpp"
#include "bot.cpp"
#include "user.cpp"
#include "game_functions.cpp"

int main(){

    set_up_game_environment();
    user u; bot b[no_of_players-1];
    select_starting_player();
    
    bool next_game_possible;
    do{
        cout<<"Round one of betting begins (pre-flop): \n\n";
        collect_small_and_large_blind();

        deal_cards_to_players();
        cout<<"Your cards are :\n"<<u.player_hand<<endl;
        initiate_betting_preflop();

        if (players_in_game.size()>1){
            cout<<"Round two of betting begins: \n\nDealing the flop:\n";
            community_cards.deal_random_cards(3);
            community_cards.print();    
            
            initialise_round();
            initiate_betting(1);

            if (players_in_game.size()>1){
                bet_amount*=2;
                cout<<"Round three of betting begins: \n\nDealing the turn:\n";
                community_cards.deal_random_cards(1);
                community_cards.print();
        
                initialise_round();
                initiate_betting(2);

                if (players_in_game.size()>1){
                    cout<<"Round four of betting begins: \n\nDealing the river:\n";
                    community_cards.deal_random_cards(1);
                    community_cards.print(); 
        
                    initialise_round();
                    initiate_betting(3);
                }
            }
        }
        vector<player*> winning_players;
        if (players_in_game.size()==1) winning_players.push_back(players_in_game[0]);
        else showdown(winning_players);
        end_game(winning_players);

        next_game_possible=false;
        if (ask_if_continue()) next_game_possible=reset_for_next_game();
    }
    while(next_game_possible);
    cout<<"\nThank you for playing\n";
}