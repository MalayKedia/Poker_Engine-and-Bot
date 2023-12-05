#include "declarations.h"

card::card(int s, int v): suit(s), value(v){}

ostream & operator<<(ostream &ost, card const &c){
    //tells how to interpret value of card as humans read it
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
bool operator==(card const &c1, card const &c2){
            if (c1.suit==c2.suit && c1.value==c2.value) return true;
            else return false;
    }

deck::deck(){
    deck_size=0;
}

deck::deck(deck const &d){
    card_list=d.card_list;
    deck_size=d.deck_size;
}

void deck::add(card c){
    card_list.push_back(c);
    deck_size++;
}
bool deck::remove (card c){

    int index=deck_size;                        //if we find no such card, index doesnt change and stays deck_size
    for (int i=0; i<deck_size; i++) if (card_list[i]==c) index=i; 
    
    if (index==deck_size) return false;         //if index didnt change it means func returns false
    
    card_list.erase(card_list.begin()+index);
    deck_size--;
    return true;
}

void deck::print(int number, int start=0) {      //prints cards from index start to start+number-1
    for (int i=start; i<start+number; i++) cout<<card_list[i]; 
}

ostream & operator<<(ostream &ost, deck const &d){
    cout<<"No of cards: "<<d.deck_size<<endl;
    for (int i=0; i<d.deck_size; i++) ost<<d.card_list[i]<<endl;
    return ost;
}