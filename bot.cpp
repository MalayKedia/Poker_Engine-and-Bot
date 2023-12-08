#include "declarations.h"

bot::bot(): player()
{   
    player_name="Bot "+to_string(player_ID);
    unseen_cards=undealed_cards;
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
    if (round_no==0)
    {
        played_first_move_in_round=true;
        if (!collect_bet(current_bet- bet_in_round)) fold();
        else cout<<"The current bet is still "<<current_bet<<endl;
    }
    else if (round_no==1) 
    {
        unseen_cards.remove(player_hand+community_cards);
        play_move_round_1();
        unseen_cards.add(player_hand+community_cards);
    }
    else if (round_no==2) 
    {
        unseen_cards.remove(player_hand+community_cards);
        play_move_round_2();
        unseen_cards.add(player_hand+community_cards);
    }
    else if (round_no==3) 
    {
        unseen_cards.remove(player_hand+community_cards);
        play_move_round_3();
        unseen_cards.add(player_hand+community_cards);
    }
}

/*
    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
*/

void bot::calculate_prob_round_1()
{
    int cases_bot_wins=0, cases_all=0;

    for (int ci1=0; ci1<47; ci1++){
        for (int ci2=ci1+1; ci2<47; ci2++){
            vector<int> self_score=(community_cards+player_hand+unseen_cards[ci1]+unseen_cards[ci2]).score_seven_cards();
            deck unseen_cards_now=unseen_cards-unseen_cards[ci1]-unseen_cards[ci2];

            for (int ci3=0; ci3<45; ci3++){
                for (int ci4=ci3+1; ci4<45; ci4++){
                    vector<int> bot_score= (community_cards+unseen_cards[ci1]+unseen_cards[ci2]+unseen_cards_now[ci3]+unseen_cards_now[ci4]).score_seven_cards();
                    
                    cases_all++;
                    if (!lexicographical_compare(bot_score.begin(), bot_score.end(), self_score.begin(), self_score.end())) cases_bot_wins++;
                }
            }
        }
    }
    cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
    prob_not_losing_against_player=double(cases_bot_wins)/cases_all;

}

void bot::play_move_round_1()
{
    if (!played_first_move_in_round) calculate_prob_round_1();
    played_first_move_in_round=true;

    
    double prob_not_losing_all_players=pow(prob_not_losing_against_player, players_in_game.size()-1);

    cout<<"Bots hand is\n"<<player_hand;
    cout<<"The prob the bot wins or ties against 1 player is :"<<prob_not_losing_against_player<<endl;
    cout<<"The prob the bot wins or ties against all player is :"<<prob_not_losing_all_players<<endl;

    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;

}

void bot::play_move_round_2()
{
    played_first_move_in_round=true;

    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
}

void bot::play_move_round_3()
{  
    played_first_move_in_round=true;

    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
}