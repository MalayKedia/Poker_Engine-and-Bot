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
    deal_cards(5, board_cards);
    total_bet_on_board=0;

}