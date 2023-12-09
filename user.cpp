#include "declarations.h"

user::user(): player()
{   
    cout<<"Enter your name as a single word: ";
    cin>>player_name;

    while (true){
        cout<<"Money in account of "<<player_name<<" : ";
        cin>>money_in_hand;
        if (money_in_hand>=large_blind) break;
        else cout<<"Invalid, player must start with more than large blind\n";
    }
}


void user::play_move(int round_no)
{
    played_first_move_in_round=true;
    char move;
    //cout<<"Your cards are: \n"<<player_hand;
    while(true){
        if (current_bet==0) cout<<"Possible moves are check (c), open (o), fold (f)\n";
        else if (no_of_raises<max_no_of_raises) cout<<"Possible moves are call (k), raise (r), fold (f)\n";
        else cout<<"Possible moves are call (k), fold (f)\n";
   
        cout<<"Type your move: ";
        cin>>move;

        if (move=='f' || (current_bet==0 && (move=='c' || move=='o')) || (current_bet>0 && (move=='k' || (no_of_raises<max_no_of_raises && move=='r')))) break;
        cout<<"Error, Invalid Input. Try again.\n";
    }

    switch (move){
        case 'c': {
            check();
            break;
        }
        case 'r':{
            if (money_in_hand>=current_bet+bet_amount-bet_in_round) raise();
            else {
                cout<<"Cannot raise due to insufficient balance, play some other move\n";
                play_move(round_no);
            }
            break;
        }

        case 'o':{
            if (money_in_hand>=bet_amount) open();
            else {
                cout<<"Cannot open due to insufficient balance, play some other move\n";
                play_move(round_no);
            }
            break;
        }

        case 'k':{
            if (money_in_hand>=current_bet-bet_in_round) call();
            else {
                cout<<"Cannot call due to insufficient balance, play some other move\n";
                play_move(round_no);
            }
            break;
        }
        case 'f':{
            fold();
            break;
        }

        default:{
            assert(false);
        }
    }
}