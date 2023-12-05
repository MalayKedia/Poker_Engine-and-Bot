#include "declarations.h"

user::user (int id, string name): player(id, name){
    //
}

void user::play_move(){

    char move;
    while(true){
        if (current_bet==0) cout<<"Possible moves are check (c), call (k), raise (r), fold(f)\n";
        else cout<<"Possible moves are call (k), raise (r), fold(f)\n";
   
        cout<<"Type your move: ";
        cin>>move;

        if (move=='k' || move=='r' || move=='f' || (current_bet==0 && move=='c')) break;
        cout<<"Error, Invalid Input. Try again.\n";
    }

    switch (move){
        case 'c': {
            cout<<"You chose to check, passing turn to next player\n";
            break;
        }
        case 'r':{
            cout<<"Enter amount to raise by: ";
            int raise_amount; cin>>raise_amount;
            if (!(this->raise(raise_amount))) {
                cout<<"Cannot raise by "<<raise_amount<<" ,try again.";
                this->play_move();
            }
            break;
        }

        case 'k': {
            if (collect_bet(current_bet- bet_in_round)) break;
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

