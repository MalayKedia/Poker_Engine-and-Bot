#include "declarations.h"
#include "game.cpp"
#include "cards.cpp"
#include "players.cpp"
#include "bot.cpp"
#include "user.cpp"

int main(){

    begin_game();
    //I assume no of players in game is 5
    user u;
    bot b[no_of_players-1];

    cout<<endl<<"Round one of betting begins (pre-flop): \n";
    cout<<"The small blind is "<<small_blind<<" and large blind is "<<2*small_blind<<endl;
    int starting_player_index=no_of_players*(rand()/(1.0+RAND_MAX));
    reorder_players_in_game(starting_player_index);

    cout<<"Starting at player "<<players_in_game[0]->player_name<<endl<<endl;
    collect_small_and_large_blind();

    cout<<"Your cards are :\n";
    u.player_hand.print();  
    cout<<endl;

    player* player_current_turn=players_in_game[2%no_of_players];
    initiate_betting(2);

    if (players_in_game.size()>1){
        cout<<"Round two of betting begins: \n\nDealing the flop:\n";
        community_cards.print(3);    
        initialise_round();
        initiate_betting();

        if (players_in_game.size()>1){
            cout<<"Round three of betting begins: \n\nDealing the turn:\n";
            community_cards.print(4); 
        
            initialise_round();
            initiate_betting();

            if (players_in_game.size()>1){
                cout<<"Round four of betting begins: \n\nDealing the river:\n";
                community_cards.print(5); 
        
                initialise_round();
                initiate_betting();
            }
        }
    }

    vector<player*> winning_players;
    if (players_in_game.size()==1) winning_players.push_back(players_in_game[0]);
    else void showdown(vector<player*> winning_players);

    end_game(winning_players);
}