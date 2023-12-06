#include "declarations.h"

user::user(): player()
{   cout<<"Enter your name as a single word: ";
    cin>>player_name;

    cout<<"Money in account of "<<player_name<<" : ";
    cin>>money_in_hand;
}


void user::play_move(){

    char move;
    while(true){
        if (current_bet==0) cout<<"Possible moves are check (c), open (o), fold(f)\n";
        else cout<<"Possible moves are call (k), raise (r), fold(f)\n";
   
        cout<<"Type your move: ";
        cin>>move;

        if (move=='f' || (current_bet==0 && (move=='c' || move=='o')) || (current_bet>0 && (move=='k' || move=='r'))) break;
        cout<<"Error, Invalid Input. Try again.\n";
    }

    switch (move){
        case 'c': {
            check();
            break;
        }
        case 'r':{
            cout<<"Enter amount to raise by: ";
            int raise_amount; cin>>raise_amount;
            if (!(this->raise(raise_amount))) this->play_move();
            break;
        }

        case 'o':{
            cout<<"Enter amount you want to bet: ";
            int bet_amount; cin>>bet_amount;
            if (!(this->raise(bet_amount))) this->play_move();
            break;
        }

        case 'k': {
            cout<<"You chose to call\n";
            if (collect_bet(current_bet- bet_in_round)) {
                cout<<"The current bet is still "<<current_bet<<endl;
                break;
            }
        }
        case 'f': {
            this->fold();
            break;
        }

        default: {
            cout<<"InValid Input, Try again\n";
            this->play_move();
        }
    }
}

