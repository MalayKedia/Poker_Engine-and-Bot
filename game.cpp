#include "declarations.h"

int rand_int_v_u(int v, int u=0){           //This function returns random integer from u to v-1
    return int(u+ (v-u)*rand()/(1.0+RAND_MAX));
}

void reorder_players_in_game(int starting_player_index){       //reorders the players in game st it starts from player we want
    vector<player*> new_list;
    for (int i=0; i<players_in_game.size(); i++){
        new_list.push_back(players_in_game[(starting_player_index+i)%players_in_game.size()]);
    }
    players_in_game=new_list;
}

void bubbleSort_descending(const int inputArr[], int sortedArr[], int n) {
    // Copy the elements from the input array to the new array
    for (int i = 0; i < n; ++i) {
        sortedArr[i] = inputArr[i];
    }

    // Bubble Sort algorithm for descending order
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            // Swap elements if they are in the wrong order
            if (sortedArr[j] < sortedArr[j + 1]) {
                // Swap sortedArr[j] and sortedArr[j + 1]
                int temp = sortedArr[j];
                sortedArr[j] = sortedArr[j + 1];
                sortedArr[j + 1] = temp;
            }
        }
    }
}

void set_up_game_environment(){
    int seed=time(NULL); srand(seed);
    cout<<"Starting the game\nThe seed value is: "<<seed<<endl<<endl;

    //setting undealed card deck to standard 52 deck
    for (int i=0; i<13;i++){
       for (int j=0; j<4; j++){
            undealed_cards.add(card(j,i));
        }
    }

    //asking for the number of players
    cout<<"Enter the number of players playing: ";
    cin>>no_of_players;
}

void set_up_game_environment_cont(){
    cout<<"You are playing limt hold'em\n";
    cout<<"The small blind is "<<small_blind<<" and large blind is "<<large_blind<<endl;

    starting_player_index=rand_int_v_u(no_of_players);
    reorder_players_in_game(starting_player_index);
    cout<<endl<<"The game starts at player "<<players_in_game[0]->player_name<<endl<<endl;

    pot_amount=0;
}

void collect_small_and_large_blind(int starting_player_index=0){  
    //starts the pre-flop round by taking default set small and large blind values from players at starting_player_index and next (if possible)
    current_bet=small_blind;
    if (!(players_in_game[starting_player_index]->collect_bet(small_blind))){
            players_in_game[starting_player_index]->fold();
            
            collect_small_and_large_blind(starting_player_index);
    }
    else {
        cout<<"The current bet now is "<<current_bet<<endl<<endl;
        collect_large_blind(starting_player_index);
    }
}

void collect_large_blind(int starting_player_index){
        current_bet=large_blind;
        if(!(players_in_game[(starting_player_index+1)%(players_in_game.size())]->collect_bet(large_blind))){
            players_in_game[(starting_player_index+1)%(players_in_game.size())]->fold();

            collect_large_blind(starting_player_index);
        }
        else cout<<"The current bet now is "<<current_bet<<endl<<endl;
}

void initialise_round(){
    vector<player*>:: iterator plr = players_in_game.begin();
    for (int i=0; i<players_in_game.size(); i++) plr[i]->bet_in_round=0;
    current_bet=0;
}

bool betting_round_over(){
    bool over=true;
    for (int i=0; i<players_in_game.size(); i++) over=over&&(players_in_game[i]->bet_in_round==current_bet);
    return over;
}

void initiate_betting_preflop(){
    int start_id=players_in_game[2%players_in_game.size()]->player_ID;
    int id;

    for(id=start_id; (id<no_of_players && start_id!=players_in_game.size()-2)|| id<no_of_players-1 ; id++){
        if(all_players[id]->in_game) {
            all_players[id]->play_move();
            cout<<endl;
        }
    }
    for(id=0; id<start_id-1; id++){
        if(all_players[id]->in_game) {
            all_players[id]->play_move();
            cout<<endl;
        }
    }

    while (!betting_round_over()){     
        if(all_players[id]->in_game) {
            all_players[id]->play_move();
            cout<<endl;
        }
        if (id!=no_of_players-1) id++;
        else id=0;
    }
}

void initiate_betting(){
    int start_id=players_in_game[0]->player_ID;
    int id;

    for(id=start_id; id<no_of_players ; id++){
        if(all_players[id]->in_game) {
            all_players[id]->play_move();
            cout<<endl;
        }
    }
    for(id=0; id<start_id; id++){
        if(all_players[id]->in_game) {
            all_players[id]->play_move();
            cout<<endl;
        }
    }

    while (!betting_round_over()){     
        if(all_players[id]->in_game) {
            all_players[id]->play_move();
            cout<<endl;
        }
        if (id!=no_of_players-1) id++;
        else id=0;
    }
}

void showdown(vector<player*> &winning_players){     //returns the vector having winning players
    cout<<"Starting the showdown: \n\n";

    vector<int> scores;
    for (vector<player*> ::iterator plr=players_in_game.begin(); plr<players_in_game.end(); plr++){
        deck seven_cards=plr[0]->player_hand + community_cards;
        scores.push_back(seven_cards.deck_seven_value());
        cout<<"cards of "<<plr[0]->player_name<<" are:\n"<<plr[0]->player_hand;
    }
    int max_score=0;
    for (int index=0; index<players_in_game.size(); index++){
        max_score=max(max_score, scores[index]);
    }
    for (int index=0; index<players_in_game.size(); index++){
        if (scores[index]==max_score){
            winning_players.push_back(players_in_game[index]);
        }
    }
}

void end_game(vector<player*> winning_players){
    cout<<endl<<"The player(s) which won is (are): ";
    for (vector<player*> ::iterator plr=winning_players.begin(); plr<winning_players.end(); plr++) cout<<plr[0]->player_name<<",  ";
    
    if (winning_players.size()==1) cout<<endl<<"The pot of "<<pot_amount<<" is awarded to him\n";
    else cout<<endl<<"The pot of "<<pot_amount<<" is divided among them, awarding each player "<<pot_amount/winning_players.size()<<endl;

    for (vector<player*> ::iterator plr=winning_players.begin(); plr<winning_players.end(); plr++) plr[0]->money_in_hand += pot_amount/winning_players.size();

    cout<<"\nThe final balance in accounts of every player are: \n";
    for (vector<player*> ::iterator plr=all_players.begin(); plr<all_players.end(); plr++) cout<<plr[0]->player_name<<" = "<<plr[0]->money_in_hand<<endl;
}

char ask_if_continue(){
    cout<<"Do you want to continue? ('y' / 'n' ): ";
    char input; cin>>input;
    if (input=='y' || input=='n') return input;
    else {
        cout<<"Error, invalid input. Try again\n";
        return ask_if_continue();
    }
}

void reset_for_next_game(){
    cout<<"You chose to continue the game\n";

    starting_player_index=(starting_player_index+1)%no_of_players;
    all_players.clear();
    community_cards.submit();

    cout<<"The players playing this game will be: ";
    for (int i=0; i<no_of_players; i++){
        all_players[i]->player_hand.submit();

        if(all_players[i]->money_in_hand>=large_blind) {
            all_players[i]->in_game=true;
            cout<<all_players[i]->player_name<<" , ";
        }
        else{
            all_players[i]->in_game=false;
        }

        if (all_players[(i+starting_player_index)%no_of_players]->in_game) players_in_game.push_back(all_players[(i+starting_player_index)%no_of_players]);
    }
    for (int i=0; i<no_of_players; i++){
        if (all_players[i]->in_game) all_players[i]->player_hand.deal_random_cards(2);
    }

    cout<<endl<<"The first turn will be played by "<<players_in_game[0]<<endl<<endl;

    pot_amount=0;
    initialise_round();

}