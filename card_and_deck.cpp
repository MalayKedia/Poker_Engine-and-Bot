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

card deck::operator[](int i) const
{
    return card_list[i];
}

bool deck::present(card const &c) const //function to tell if card c is present in the deck
{
    int deck_size=card_list.size();
    int index=deck_size;                        //if we find no such card, index doesnt change and stays deck_size
    for (int i=0; i<deck_size; i++) if (card_list[i]==c) index=i; 
    
    if (index==deck_size) return false;         //if index didnt change it means func returns false
    else return true;
}

void deck::add(card const &c) //function to add a card c to deck
{
    card_list.push_back(c);
}

void deck::add(deck const &d) //function to add a deck d to deck
{
    int n=d.card_list.size();
    for (int i=0; i<n; i++){
        card_list.push_back(d[i]);
    }
}

bool deck::remove (card const &c) //fuction returns false if card c is not in deck, and if c is in deck, it removes it and returns true
{
    int deck_size=card_list.size();
    int index=deck_size;                        //if we find no such card, index doesnt change and stays deck_size
    for (int i=0; i<deck_size; i++) if (card_list[i]==c) index=i; 
    
    if (index==deck_size) return false;         //if index didnt change it means func returns false
    
    card_list.erase(card_list.begin()+index);
    return true;
}

void deck::remove (deck const &d) //function to remove any cards presents in deck d from deck on which operated
{
    for (int i=0; i<d.card_list.size(); i++){
        remove(d[i]);
    }
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

deck operator+(card const &c1, card const &c2)
{
    deck sum;
    sum.add(c1);
    sum.add(c2);
    return sum;
}

deck deck::operator+(card const &c) const
{
    deck sum;
    sum.card_list=card_list;
    sum.add(c);
    return sum;
}

deck deck::operator+(deck const &d) const
{
    deck sum;
    sum.card_list=card_list;

    int size_d=d.card_list.size();
    for (int i=0; i<size_d; i++){
        sum.add(d[i]);
    }
    return sum;
}

deck deck::operator-(card const &c) const
{
    deck sum;
    sum.card_list=card_list;
    assert(sum.remove(c));
    return sum;
}

ostream & operator<<(ostream &ost, deck const &d){
    for (int i=0; i<d.card_list.size(); i++) ost<<d[i];
    return ost;
}

void deck::print() //prints whole deck
{
    for (int i=0; i<card_list.size(); i++) cout<<card_list[i]; 
    cout<<endl;
}

vector<int> deck::score_five_cards()   //operates on a deck of 5 cards and returns a vector which can be lexicographically compared to score any two decks of 5 cards 
{
    assert(this->card_list.size()==5);
   /*First value:
    High card- 0, Pair- 1, Two pair- 2, Three of a kind- 3, Straight- 4, Flush- 5, Full House- 6, 4 of a kind- 7, Straight Flush- 8 */
   
    vector<int> score;

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

    if (straight && flush) {
        score.push_back(8);
        int highest_card=12;
        while (list_of_card_value[highest_card]!=1) highest_card--;
        score.push_back(highest_card);
    }
    else if (list_of_card_value_sorted[0]==4){
        score.push_back(7);
        int quadruple_card=12, single_card=12;
        while (list_of_card_value[quadruple_card]!=4) quadruple_card--;
        while (list_of_card_value[single_card]!=1) single_card--;
        score.push_back(quadruple_card);
        score.push_back(single_card);
    }
    else if (list_of_card_value_sorted[0]==3 && list_of_card_value_sorted[1]==2){
        score.push_back(6);
        int triple_card=12, double_card=12;
        while (list_of_card_value[triple_card]!=3) triple_card--;
        while (list_of_card_value[double_card]!=2) double_card--;
        score.push_back(triple_card);
        score.push_back(double_card);
    }
    else if (flush) {
        score.push_back(5);
        for(int i=12; i>=0; i--)  if (list_of_card_value[i]==1) score.push_back(i);
    }
    else if (straight){
        score.push_back(4);
        int highest_card=12;
        while (list_of_card_value[highest_card]!=1) highest_card--;
        score.push_back(highest_card);
    }
    else if (list_of_card_value_sorted[0]==3){
        score.push_back(3);
        int triple_card=12;
        while (list_of_card_value[triple_card]!=3) triple_card--;
        score.push_back(triple_card);
        for(int i=12; i>=0; i--)  if (list_of_card_value[i]==1) score.push_back(i);
    }
    else if (list_of_card_value_sorted[0]==2 && list_of_card_value_sorted[1]==2){
        score.push_back(2);
        for(int i=12; i>=0; i--)  if (list_of_card_value[i]==2) score.push_back(i);
        for(int i=12; i>=0; i--)  if (list_of_card_value[i]==1) score.push_back(i);
    }
    else if (list_of_card_value_sorted[0]==2){
        score.push_back(1);
        int double_card=12;
        while (list_of_card_value[double_card]!=2) double_card--;
        score.push_back(double_card);
        for(int i=12; i>=0; i--)  if (list_of_card_value[i]==1) score.push_back(i);
    }
    else{
        score.push_back(0);
        for(int i=12; i>=0; i--)  if (list_of_card_value[i]==1) score.push_back(i);
    }
    return score;
}

vector<int> deck::score_seven_cards()   //operates on deck of 7 cards and returns highest possible score
{
    assert(this->card_list.size()==7);
    deck d=*this;
    vector<int> highest_score=(d-d[6]-d[5]).score_five_cards();
    for (int i=0; i<5; i++){
        for (int j=i+1; j<7; j++){
            vector<int> deck_score=(d-d[i]-d[j]).score_five_cards(); 

            if (lexicographical_compare(highest_score.begin(), highest_score.end(), deck_score.begin(), deck_score.end())) highest_score=deck_score;
//std::lexicographical_compare compares the elements of the two vectors lexicographically. The function returns true if the first range is lexicographically less than the second range and false otherwise
        }
    }
    return highest_score;
}

