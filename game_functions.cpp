#include "declarations.h"

ostream & operator<<(ostream &ost, vector<int> const &v)
{
    for (int i=0; i<v.size(); i++) ost<<v[i]<<"  ";
    ost<<endl;
    return ost;
}

int rand_int_v_u(int v, int u) //This function returns random integer from u to v-1, default u is 0
{
    return int(u+ (v-u)*(rand()/(1.0+RAND_MAX)));
}

double rand_double_v_u(int v, int u) //This function returns a double from u to v both inclusive
{
    return (u+ (v-u)*(double(rand())/RAND_MAX));
}
void bubbleSort_descending(const int inputArr[], int sortedArr[], int n) 
{
    for (int i = 0; i < n; ++i) sortedArr[i] = inputArr[i];

    // Bubble Sort algorithm for descending order
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (sortedArr[j] < sortedArr[j + 1]) {
                int temp = sortedArr[j];
                sortedArr[j] = sortedArr[j + 1];
                sortedArr[j + 1] = temp;
            }
        }
    }
}

void reorder_players_in_game(int starting_player_index) //circularly reorders the list players_in_game st it starts from player at index given
{
    vector<player*> new_list;
    for (int i=0; i<players_in_game.size(); i++){
        new_list.push_back(players_in_game[(starting_player_index+i)%players_in_game.size()]);
    }
    players_in_game=new_list;
}

void set_up_game_environment() //sets undealed card deck to standard 52 deck, asks for no. of players, initialises game variables
{
    //int seed=1702128349;
    int seed=time(NULL);
    srand(seed);
    cout<<"Starting the game\nThe seed value is: "<<seed<<endl<<endl;

    cout<<"You are playing limit hold'em\n";
    cout<<"The small blind is "<<small_blind<<" and large blind is "<<large_blind<<endl<<endl;

    //setting undealed card deck to standard 52 deck
    for (int i=0; i<13;i++){
       for (int j=0; j<4; j++){
            undealed_cards.add(card(j,i));
        }
    }
    //asking for the number of players
    while (true){
        cout<<"Enter the number of players playing: ";
        cin>>no_of_players;
        if (no_of_players>1 && no_of_players<=6) break;
        else cout<<"Invalid, number of players must lie between 2 and 6 (both inclusive)\n";
    }
    cout<<endl;
    //initialises game variables
    pot_amount=0;
    no_of_raises=0;
    current_bet=0;
    bet_amount=large_blind;
}

void select_starting_player() //randomly selects the player which starts the game and circularly reorders list so that it appears first in list 
{
    starting_player_index=rand_int_v_u(no_of_players);
    reorder_players_in_game(starting_player_index);
    cout<<endl<<"The game starts at player "<<players_in_game[0]->player_name<<endl<<endl;
}

void collect_small_and_large_blind()  //starts the pre-flop round by taking default set small and large blind values from players 0 and 1
{
    //by design of game, every player in game must be capable of paying the large blind
    current_bet=small_blind;
    cout<<"Player "<<players_in_game[0]->player_name<<" pays the small blind\n";
    players_in_game[0]->played_first_move_in_round=true;
    players_in_game[0]->collect_bet(small_blind);
    cout<<"The current bet now is "<<current_bet<<endl<<endl;

    current_bet=large_blind;
    cout<<"Player "<<players_in_game[1]->player_name<<" pays the large blind\n";
    players_in_game[1]->played_first_move_in_round=true;
    players_in_game[1]->collect_bet(large_blind);
    cout<<"The current bet now is "<<current_bet<<endl<<endl;
}

void deal_cards_to_players() //randomly distributes two cards each to all players in list players_in_game
{
    for (int i=0; i<players_in_game.size(); i++) players_in_game[i]->player_hand.deal_random_cards(2);
    cout<<"Dealing cards to all players\n\n";
}

void initialise_round() //sets current bet and round bets of each player to be zero before start of betting 
{
    vector<player*>:: iterator plr = players_in_game.begin();
    for (int i=0; i<players_in_game.size(); i++) {
        plr[i]->bet_in_round=0;
        plr[i]->played_first_move_in_round=false;
    }
    current_bet=0;
    no_of_raises=0;
}

bool betting_round_over() //returns true if bet in round of each player in game matches current bet
{
    bool over=true;
    for (int i=0; i<players_in_game.size(); i++) over=over&&(players_in_game[i]->bet_in_round==current_bet)&&(players_in_game[i]->played_first_move_in_round);
    return over;
}

void initiate_betting_preflop() //continues betting round until round isnt over in the preflop round
{
    int id=players_in_game[2%players_in_game.size()]->player_ID;
    while (!betting_round_over()){     
        if(all_players[id]->in_game) {
            cout<<"Turn moves to player "<<all_players[id]->player_name<<"\n";
            all_players[id]->play_move(0);
            cout<<endl;
        }
        if (id!=no_of_players-1) id++;
        else id=0;
    }
}

void initiate_betting(int round_no) //continues betting round until round isnt over
{
    int id=players_in_game[0]->player_ID;
    while (!betting_round_over()){     
        if(all_players[id]->in_game) {
            cout<<"Turn moves to player "<<all_players[id]->player_name<<"\n";
            all_players[id]->play_move(round_no);
            cout<<endl;
        }
        if (id!=no_of_players-1) id++;
        else id=0;
    }
}

void showdown(vector<player*> &winning_players) //returns the vector having winning players
{
    cout<<"Starting the showdown: \n\n";
    cout<<"The community cards were: \n";
    community_cards.print();

    vector<int> scores;
    for (vector<player*> ::iterator plr=players_in_game.begin(); plr<players_in_game.end(); plr++){
        deck seven_cards=plr[0]->player_hand + community_cards;
        scores.push_back(seven_cards.score_seven_cards());
        cout<<"cards of "<<plr[0]->player_name<<" are:\n"<<plr[0]->player_hand;
    }
    int max_score=scores[0];
    for (int index=1; index<players_in_game.size(); index++){
        max_score=max(max_score,scores[index]);
    }
    for (int index=0; index<players_in_game.size(); index++){
        if (scores[index]==max_score){
            winning_players.push_back(players_in_game[index]);
        }
    }
}

void end_game(vector<player*> &winning_players) //prints the name of winning players and divides the pot among them
{
    cout<<endl<<"The player(s) which won is (are): ";
    for (vector<player*> ::iterator plr=winning_players.begin(); plr<winning_players.end(); plr++) cout<<plr[0]->player_name<<",  ";
    
    if (winning_players.size()==1) cout<<endl<<"The pot of "<<pot_amount<<" is awarded to him\n";
    else cout<<endl<<"The pot of "<<pot_amount<<" is divided among them, awarding each player "<<pot_amount/winning_players.size()<<endl;

    for (vector<player*> ::iterator plr=winning_players.begin(); plr<winning_players.end(); plr++) plr[0]->money_in_hand += pot_amount/winning_players.size();

    cout<<"\nThe final balance in accounts of every player are: \n";
    for (vector<player*> ::iterator plr=all_players.begin(); plr<all_players.end(); plr++) cout<<plr[0]->player_name<<" = "<<plr[0]->money_in_hand<<endl;
}

bool ask_if_continue() //asks user if they want to play another game and returns true if user says yes
{
    while(true){
        cout<<"Do you want to continue? ('y' / 'n'): ";
        char input; cin>>input;
        if (input=='y') return true;
        else if (input=='n') return false;
        cout<<"Error, invalid input. Try again\n";
    }
}

bool reset_for_next_game() //returns true if next game is possible, if possible, redeals all cards and reinitialises internal variables
{
    cout<<"You chose to continue the game\n";

    starting_player_index +=1;
    players_in_game.clear();
    community_cards.submit();

    for (int i=0; i<no_of_players; i++){
        all_players[i]->player_hand.submit();

        if(all_players[i]->money_in_hand>=large_blind) all_players[i]->in_game=true;
        else all_players[i]->in_game=false;
    }
    for (int i=0; i<no_of_players; i++){ 
        if (all_players[(i+starting_player_index)%no_of_players]->in_game) {
            players_in_game.push_back(all_players[(i+starting_player_index)%no_of_players]);            
        }
    }

    if (players_in_game.size()<=1) {
        cout<<"It is not possible to play more games\n";
        return false;
    }

    cout<<"The players playing this game will be: ";
    for (int i=0; i<no_of_players; i++){
        if (all_players[i]->in_game) cout<<all_players[i]->player_name<<" , ";
    }
    cout<<endl<<"The first turn will be played by "<<players_in_game[0]->player_name<<endl<<endl;
    starting_player_index=players_in_game[0]->player_ID;

    pot_amount=0;
    bet_amount=large_blind;
    initialise_round();
    return true;
}