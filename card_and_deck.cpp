#include "declarations.h"

card::card(int s, int v): suit(s), value(v){}

ostream & operator<<(ostream &ost, card const &c) //tells how to interpret value of card as humans read it
{
        ost<<"\t";
        if (c.value<=8) ost<<c.value+2;
        else if (c.value==9) ost<<"Jack";
        else if (c.value==10) ost<<"Queen";
        else if (c.value==11) ost<<"King";
        else if (c.value==12) ost<<"Ace";
        else ost<<"NA";

        ost<<"\t of \t";

        switch (c.suit){
            case 0: ost<<"Hearts"; break;
            case 1: ost<<"Diamonds"; break;
            case 2: ost<<"Clubs"; break;
            case 3: ost<<"Spades"; break;
            default: ost<<"NA"; break;
        }
        ost<<endl;
        return ost;
}
bool operator==(card const &c1, card const &c2)
{
    if (c1.suit==c2.suit && c1.value==c2.value) return true;
    else return false;
}

deck::deck(){}

deck::deck(deck const &d)
{
    card_list=d.card_list;
}

void deck::add(card c) //function to add a card c to deck
{
    card_list.push_back(c);
}

bool deck::remove (card c) //fuction returns false if card c is not in deck, and if c is in deck, it removes it and returns true
{
    int deck_size=card_list.size();
    int index=deck_size;                        //if we find no such card, index doesnt change and stays deck_size
    for (int i=0; i<deck_size; i++) if (card_list[i]==c) index=i; 
    
    if (index==deck_size) return false;         //if index didnt change it means func returns false
    
    card_list.erase(card_list.begin()+index);
    return true;
}

void deck::remove (int i) //function to remove card at i th position in vector
{
    remove(card_list[i]);
}

void deck::deal_random_cards(int number)  //adds 'number' of random caards to deck from the undealed_cards deck
{
    for (int i=0; i<number; i++){
        bool undealed_card_found;
        do{
            int n=rand_int_v_u(52);
            undealed_card_found=undealed_cards.remove(card(n/13,n%13));
            if (undealed_card_found) add(card(n/13,n%13));
        } while (!undealed_card_found);
    }
}

void deck::submit() //empties the deck and adds all cards to undealed_cards deck
{
    while(card_list.size()!=0){
        undealed_cards.add(card_list[0]);
        this->remove(card_list[0]);
    }
}

deck deck::operator+(deck const d) const{
    deck sum;
    sum.card_list=card_list;

    int size_d=d.card_list.size();
    for (int i=0; i<size_d; i++){
        sum.add(d.card_list[i]);
    }
    return sum;
}

ostream & operator<<(ostream &ost, deck const &d){
    for (int i=0; i<d.card_list.size(); i++) ost<<d.card_list[i];
    return ost;
}

void deck::print() //prints whole deck
{
    for (int i=0; i<card_list.size(); i++) cout<<card_list[i]; 
}

int deck::deck_five_value() //operates on a deck of 5 cards and returns a 5 digit no, highest digit being deck rank, next 2 being high card and next 2 being next high card 
{
    assert(this->card_list.size()==5);
   /*Hundreds value:
    High card- 0, Pair- 1, Two pair- 2, Three of a kind- 3, Straight- 4, Flush- 5, Full House- 6, 4 of a kind- 7, Straight Flush- 8 */
   
    int list_of_card_value[13], list_of_card_suite[4], list_of_card_value_sorted[13];
    for (int i=0; i<13; i++) list_of_card_value[i]=0;
    for (int i=0; i<4; i++) list_of_card_suite[i]=0;

    for (int i=0; i<5; i++){
        list_of_card_suite[card_list[i].suit]++;
        list_of_card_value[card_list[i].value]++;
    }
    bubbleSort_descending(list_of_card_value,list_of_card_value_sorted,13);

    bool straight=false, flush=false;
    for (int i=0; i<4; i++) if(list_of_card_suite[i]==5) flush=true;
    for (int i=0; i<9; i++) if (list_of_card_value[i]==1 && list_of_card_value[i+1]==1 && list_of_card_value[i+2]==1 && list_of_card_value[i+3]==1 && list_of_card_value[i+4]==1) straight=true;

    
    int hundreds_place=0, tens_place=12 ,ones_place=12;
    
    if (straight && flush) {
        hundreds_place=8;
        while (list_of_card_value[tens_place]!=1) tens_place--;
        ones_place=tens_place-1;
    }
    else if (list_of_card_value_sorted[0]==4){
        hundreds_place=7;
        while (list_of_card_value[tens_place]!=4) tens_place--;
        while (list_of_card_value[ones_place]!=1) ones_place--;

    }
    else if (list_of_card_value_sorted[0]==3 && list_of_card_value_sorted[1]==2){
        hundreds_place=6;
        while (list_of_card_value[tens_place]!=3) tens_place--;
        while (list_of_card_value[ones_place]!=2) ones_place--;
    }
    else if (flush) {
        hundreds_place=5;
        while (list_of_card_value[tens_place]=0) tens_place--;
        if (list_of_card_value[tens_place]>1) ones_place=tens_place;
        else{
            ones_place=tens_place-1;
            while (list_of_card_value[ones_place]=0) ones_place--;
        }
    }
    else if (straight){
        hundreds_place=4;
        while (list_of_card_value[tens_place]!=1) tens_place--;
        ones_place=tens_place-1;
    }
    else if (list_of_card_value_sorted[0]==3){
        hundreds_place=3;
        while (list_of_card_value[tens_place]!=3) tens_place--;
        while (list_of_card_value[ones_place]!=1) ones_place--;
    }
    else if (list_of_card_value_sorted[0]==2 && list_of_card_value_sorted[1]==2){
        hundreds_place=2;
        while (list_of_card_value[tens_place]!=2) tens_place--;
        ones_place=tens_place-1;
        while (list_of_card_value[ones_place]!=2) ones_place--;
    }
    else if (list_of_card_value_sorted[0]==2){
        hundreds_place=1;
        while (list_of_card_value[tens_place]!=2) tens_place--;
        while (list_of_card_value[ones_place]!=1) ones_place--;
    }
    else{
        while (list_of_card_value[tens_place]!=1) tens_place--;
        ones_place=tens_place-1;
        while (list_of_card_value[ones_place]!=1) ones_place--;
    }
    
    return hundreds_place*10000+tens_place*100+ones_place;
}

int deck::deck_seven_value() //operates on deck of 7 cards and returns highest possible score
{
    assert(this->card_list.size()==7);
    int val=0;
    for (int i=0; i<7; i++){
        for (int j=i+1; j<7; j++){
            deck d=*this;
            d.remove(j); d.remove(i);
            val=max(val,d.deck_five_value());
        }
    }
    return val;
}