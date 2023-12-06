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


void user::play_move()
{
    char move;
    while(true){
        if (current_bet==0) cout<<"Possible moves are check (c), open (o), fold(f)\n";
        else if (no_of_raises<max_no_of_raises) cout<<"Possible moves are call (k), raise (r), fold(f)\n";
        else cout<<"Possible moves are call (k), fold(f)\n";
   
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
            cout<<"You have raised by "<<large_blind<<endl;
            if (!(this->raise(large_blind))) this->play_move();
            break;
        }

        case 'o':{
            cout<<"You have opened by "<<large_blind<<endl;
            if (!(this->raise(large_blind))) this->play_move();
            break;
        }

        case 'k':{
            cout<<"You chose to call\n";
            if (collect_bet(current_bet- bet_in_round)) {
                cout<<"The current bet is still "<<current_bet<<endl;
                break;
            }
        }
        case 'f':{
            this->fold();
            break;
        }

        default:{
            cout<<"Invalid Input, Try Again\n";
            this->play_move();
            break;
        }
    }
}