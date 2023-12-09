#include "declarations.h"

bot::bot(): player()
{   
    player_name="Bot "+to_string(player_ID);
    unseen_cards=undealed_cards;
    
    while (true){
        cout<<"Nature of "<<player_name<<" (confident 'c' / underconfident 'u' / overconfident 'o'/ random 'r'): ";
        cin>>bot_nature;
        if (bot_nature=='c' || bot_nature=='u' || bot_nature=='o' || bot_nature=='r') break;
        else cout<<"Invalid, not a valid bot type\n";
    }
    player_name=string("Bot(") + bot_nature + ") - " + to_string(player_ID);
    
    if(bot_nature=='r'){
        int nature=rand_int_v_u(5);
        if (nature==0) bot_nature='u';
        else if (nature==1) bot_nature='o';
        else bot_nature='c';
    }

    while (true){
        cout<<"Money in account of "<<player_name<<" : ";
        cin>>money_in_hand;
        if (money_in_hand>=large_blind) break;
        else cout<<"Invalid, player must start with more than large blind\n";
    }
}


void bot::play_move(int round_no) //just matches the current bet as of now
{
    assert(unseen_cards.card_list.size()==52);
    if (!played_first_move_in_round) calculate_probability(round_no);

    if (round_no==0)
    {
        sleep_for(10s);
        play_move_preflop();
        played_first_move_in_round=true;
    }
    else if (round_no==1) 
    {
        if (played_first_move_in_round) sleep_for(10s);
        play_move_round_1();
        played_first_move_in_round=true;   
    }
    else if (round_no==2) 
    {
        sleep_for(10s);
        play_move_round_2();
        played_first_move_in_round=true;
    }
    else if (round_no==3) 
    {
        sleep_for(10s);
        play_move_round_3();
        played_first_move_in_round=true;
    }
}

/*
    if (money_in_hand<current_bet-bet_in_round) fold();
    else if (current_bet==0) check();
    else call();
*/

int bot::strength_preflop()
{
    if (player_hand[0].value==player_hand[1].value){
        int value=player_hand[0].value;
        if (value>=5) return 1;
        else if (value>=3) return 2;
        else return 3;
    }
    if (player_hand[0].suit==player_hand[1].suit){
        int high=max(player_hand[0].value,player_hand[1].value), low=min(player_hand[0].value,player_hand[1].value);
        switch (high){
            case 12: {
                if (low>=8) return 1;
                else if (low>=4) return 2;
                else return 3;
            }
            case 11:{
                if (low>=8) return 1;
                else if (low>=7) return 2;
                else return 3;
            }
            case 10:{
                if (low>=8) return 1;
                else if (low>=6) return 2;
                else if (low>=5) return 3;
                else return 4;
            }
            case 9:{
                if (low>=7) return 1;
                else if (low>=6) return 2;
                else if (low>=5) return 3;
                else return 4;
            }
            case 8:{
                if (low>=8) return 1;
                else if (low>=6) return 2;
                else if (low>=5) return 3;
                else return 4;
            }
            case 7:{
                if (low>=6) return 2;
                else if (low>=4) return 3;
                else return 4;
            }
            case 6:{
                if (low>=4) return 3;
                else return 4;
            }
            case 5:{
                if (low>=3) return 3;
                else return 4;
            }
            case 4:{
                if (low>=3) return 3;
                else return 4;
            }
            case 3:{
                if (low>=2) return 3;
                else return 4;
            }
            default: return 4;
        }
    }
    else{
        int high=max(player_hand[0].value,player_hand[1].value), low=min(player_hand[0].value,player_hand[1].value);
        switch (high){
            case 12: {
                if (low>=8) return 1;
                else if (low>=6) return 2;
                else if (low>=4) return 3;
                else return 4;
            }
            case 11:{
                if (low>=9) return 1;
                else if (low>=8) return 2;
                else if (low>=6) return 2;
                else return 4;
            }
            case 10:{
                if (low>=8) return 2;
                else if (low>=6) return 3;
                else return 4;
            }
            case 9:{
                if (low>=8) return 2;
                else if (low>=6) return 3;
                else return 4;
            }
            case 8:{
                if (low>=6) return 3;
                else return 4;
            }
            case 7:{
                if (low>=5) return 3;
                else return 4;
            }
            case 6:{
                if (low>=5) return 3;
                else return 4;
            }
            default: return 4;
        }
    }
}

void bot::calculate_probability(int round_no)
{
    unseen_cards.remove(player_hand+community_cards);
    switch (round_no){
        case 0: break;
        case 1: calculate_prob_round_1(); break;
        case 2: calculate_prob_round_2(); break;
        case 3: calculate_prob_round_3(); break;
        default: assert(false);
    }
    unseen_cards.add(player_hand+community_cards);
}

void bot::calculate_prob_round_1()
{
    int cases_bot_wins=0, cases_all=0;

    assert(unseen_cards.card_list.size()==47);
    for (int comm1=0; comm1<47; comm1++){
        for (int comm2=comm1+1; comm2<47; comm2++){
            vector<int> self_score=(community_cards+player_hand+unseen_cards[comm1]+unseen_cards[comm2]).score_seven_cards();
            deck unseen_cards_now = unseen_cards-unseen_cards[comm1]-unseen_cards[comm2];

            for (int other1=0; other1<45; other1++){
                for (int other2=other1+1; other2<45; other2++){
                    vector<int> bot_score= (community_cards+unseen_cards[comm1]+unseen_cards[comm2]+unseen_cards_now[other1]+unseen_cards_now[other2]).score_seven_cards();
                    
                    cases_all++;
                    if (!lexicographical_compare(self_score.begin(), self_score.end(), bot_score.begin(), bot_score.end())) cases_bot_wins++;
                }
            }
        }
    }

    //cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
    prob_not_losing_against_player=double(cases_bot_wins)/cases_all;
}

void bot::calculate_prob_round_2()
{
    int cases_bot_wins=0, cases_all=0;

    assert(unseen_cards.card_list.size()==46);
    for (int comm1=0; comm1<46; comm1++){
        vector<int> self_score=(community_cards+player_hand+unseen_cards[comm1]).score_seven_cards();
        deck unseen_cards_now = unseen_cards-unseen_cards[comm1];

        for (int other1=0; other1<45; other1++){
            for (int other2=other1+1; other2<45; other2++){
                vector<int> bot_score= (community_cards+unseen_cards[comm1]+unseen_cards_now[other1]+unseen_cards_now[other2]).score_seven_cards();
                    
                cases_all++;
                if (!lexicographical_compare(self_score.begin(), self_score.end(), bot_score.begin(), bot_score.end())) cases_bot_wins++;
            }
        }
    }

    //cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
    prob_not_losing_against_player=double(cases_bot_wins)/cases_all;
}

void bot::calculate_prob_round_3()
{
    int cases_bot_wins=0, cases_all=0;
        
    assert(unseen_cards.card_list.size()==45);
    vector<int> self_score=(community_cards+player_hand).score_seven_cards();

    for (int other1=0; other1<45; other1++){
        for (int other2=other1+1; other2<45; other2++){
            vector<int> bot_score= (community_cards+unseen_cards[other1]+unseen_cards[other2]).score_seven_cards();
                    
            cases_all++;
            if (!lexicographical_compare(self_score.begin(), self_score.end(), bot_score.begin(), bot_score.end())) cases_bot_wins++;
        }
    }
    
    //cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
    prob_not_losing_against_player=double(cases_bot_wins)/cases_all;
}

void bot::play_move_preflop()
{
    int strength=strength_preflop();
    bool bot_can_call=(money_in_hand>=current_bet-bet_in_round), bot_can_raise=(money_in_hand>=current_bet+bet_amount-bet_in_round)&&(no_of_raises<max_no_of_raises);

    if (!bot_can_call) {
        fold();
        return;
    }

    switch (bot_nature){
        case 'c':{
            if (strength==3 || strength==4){
                if (current_bet<=2*bet_amount) call();
                else fold();
            }
            else if (strength==2){
                if (current_bet<=2*bet_amount && bot_can_raise) raise();
                else call();
            }
            else if (strength==1){
                if (current_bet<=3*bet_amount && bot_can_raise) raise();
                else call();
            }
            break;
        }
        case 'u':{
            if (strength==4){
                if (current_bet==bet_amount) call();
                else fold();
            }
            else if (strength==3 || strength==2){
                if (current_bet<=2*bet_amount) call();
                else fold();
            }
            else if (strength==1){
                if (current_bet<=2*bet_amount && bot_can_raise) raise();
                else call();
            }
            break;
        }
        case 'o':{
            if (strength==3 || strength==4) {call();}
            else if (strength==2){
                if (current_bet<=3*bet_amount && bot_can_raise) raise();
                else call();
            }
            else{
                if (current_bet<=4*bet_amount && bot_can_raise) raise();
                else call();
            }
            break;
        }
    }
}

void bot::play_move_round_1()
{
    //cout<<"Bots hand is\n"<<player_hand;

    bool bot_can_call=(money_in_hand>=current_bet-bet_in_round), bot_can_raise=(money_in_hand>=current_bet+bet_amount-bet_in_round)&&(no_of_raises<max_no_of_raises);

    if (!bot_can_call) {
        fold();
        return;
    }

    if (current_bet==0){
        switch (bot_nature){
            case 'c': {
                if (money_in_hand<bet_amount || prob_not_losing_against_player<=0.50) check();
                else open();
                break;
            }
            case 'o': {
                if (money_in_hand<bet_amount || prob_not_losing_against_player<=0.30) check();
                else open();
                break;
            }
            case 'u': {
                if (money_in_hand<bet_amount || prob_not_losing_against_player<=0.70) check();
                else open();
                break;
            }
            default: assert(false);
        }
    }
    else if(no_of_raises==0){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.15, 0.30, 0.60, 0.75, prob_not_losing_against_player, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.05, 0.15, 0.40, 0.60, prob_not_losing_against_player, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.25, 0.45, 0.75, 0.90, prob_not_losing_against_player, bot_can_raise); break;
            default: assert(false);
        }
    }
    else if (no_of_raises==1){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.20, 0.35, 0.80, 0.90, prob_not_losing_against_player, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.10, 0.25, 0.60, 0.80, prob_not_losing_against_player, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.30, 0.50, 0.85, 1.00, prob_not_losing_against_player, bot_can_raise); break;
            default: assert(false);
        }
    }
    else if (no_of_raises==2){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.30, 0.50, 0.85, 0.95, prob_not_losing_against_player, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.20, 0.40, 0.75, 0.90, prob_not_losing_against_player, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.40, 0.65, 0.90, 1.00, prob_not_losing_against_player, bot_can_raise); break;
            default: assert(false);
        }
    }
    else {
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.30, 0.60, 1.00, 1.00, prob_not_losing_against_player, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.20, 0.40, 1.00, 1.00, prob_not_losing_against_player, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.50, 0.75, 1.00, 1.00, prob_not_losing_against_player, bot_can_raise); break;
            default: assert(false);
        }
    }
}

void bot::play_move_round_2()
{
    double prob_adjusted_for_no_of_players=pow(prob_not_losing_against_player, sqrt(double(players_in_game.size()-1)));

    //cout<<"Bots hand is\n"<<player_hand;
   
    bool bot_can_call=(money_in_hand>=current_bet-bet_in_round), bot_can_raise=(money_in_hand>=current_bet+bet_amount-bet_in_round)&&(no_of_raises<max_no_of_raises);

    if (!bot_can_call) {
        fold();
        return;
    }

    if (current_bet==0){
        switch (bot_nature){
            case 'c': {
                if (money_in_hand<bet_amount || prob_adjusted_for_no_of_players<=0.50) check();
                else open();
                break;
            }
            case 'o': {
                if (money_in_hand<bet_amount || prob_adjusted_for_no_of_players<=0.30) check();
                else open();
                break;
            }
            case 'u': {
                if (money_in_hand<bet_amount || prob_adjusted_for_no_of_players<=0.70) check();
                else open();
                break;
            }
            default: assert(false);
        }
    }
    else if(no_of_raises==0){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.15, 0.30, 0.60, 0.75, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.05, 0.15, 0.40, 0.60, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.25, 0.45, 0.75, 0.90, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }
    else if (no_of_raises==1){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.20, 0.35, 0.80, 0.90, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.10, 0.25, 0.60, 0.80, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.30, 0.50, 0.85, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }
    else if (no_of_raises==2){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.30, 0.50, 0.85, 0.95, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.20, 0.40, 0.75, 0.90, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.40, 0.65, 0.90, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }
    else {
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.30, 0.60, 1.00, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.20, 0.40, 1.00, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.50, 0.75, 1.00, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }

}

void bot::play_move_round_3()
{  
    double prob_not_losing_all_players=pow(prob_not_losing_against_player, players_in_game.size()-1);
    double prob_adjusted_for_no_of_players=pow(prob_not_losing_against_player, sqrt( 2*double(players_in_game.size()-1) -1 ));

    //cout<<"Bots hand is\n"<<player_hand;

    bool bot_can_call=(money_in_hand>=current_bet-bet_in_round), bot_can_raise=(money_in_hand>=current_bet+bet_amount-bet_in_round)&&(no_of_raises<max_no_of_raises);

    if (!bot_can_call) {
        fold();
        return;
    }

    if (current_bet==0){
        switch (bot_nature){
            case 'c': {
                if (money_in_hand<bet_amount || prob_adjusted_for_no_of_players<=0.50) check();
                else open();
                break;
            }
            case 'o': {
                if (money_in_hand<bet_amount || prob_adjusted_for_no_of_players<=0.30) check();
                else open();
                break;
            }
            case 'u': {
                if (money_in_hand<bet_amount || prob_adjusted_for_no_of_players<=0.70) check();
                else open();
                break;
            }
            default: assert(false);
        }
    }
    else if(no_of_raises==0){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.15, 0.30, 0.60, 0.75, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.05, 0.15, 0.40, 0.60, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.25, 0.45, 0.75, 0.90, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }
    else if (no_of_raises==1){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.20, 0.35, 0.80, 0.90, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.10, 0.25, 0.60, 0.80, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.30, 0.50, 0.85, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }
    else if (no_of_raises==2){
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.30, 0.50, 0.85, 0.95, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.20, 0.40, 0.75, 0.90, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.40, 0.65, 0.90, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }
    else {
        switch (bot_nature){
            case 'c': play_move_given_thresholds(0.30, 0.60, 1.00, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'o': play_move_given_thresholds(0.20, 0.40, 1.00, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            case 'u': play_move_given_thresholds(0.50, 0.75, 1.00, 1.00, prob_adjusted_for_no_of_players, bot_can_raise); break;
            default: assert(false);
        }
    }

}

void bot::play_move_given_thresholds(double p1, double p2, double p3, double p4, double prob, bool bot_can_raise)
{
    if (prob<p1) fold();
    else if (prob<p2){
        double rand=rand_double_v_u(p2,p1);
        if(rand>prob) fold();
        else call();
    }   
    else if (prob<p3) call();
    else if (prob<p4){
        double rand=rand_double_v_u(p3,p2);
        if(bot_can_raise && rand<=prob) raise();
        else call();
    }
    else{
        if(bot_can_raise) raise();
        else call();
    }
}