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
        if (!played_first_move_in_round) calculate_prob_round_1();
        played_first_move_in_round=true;
        play_move_round_1();
        unseen_cards.add(player_hand+community_cards);
    }
    else if (round_no==2) 
    {
        unseen_cards.remove(player_hand+community_cards);
        if (!played_first_move_in_round) calculate_prob_round_2();
        played_first_move_in_round=true;
        play_move_round_2();
        unseen_cards.add(player_hand+community_cards);
    }
    else if (round_no==3) 
    {
        unseen_cards.remove(player_hand+community_cards);
        if (!played_first_move_in_round) calculate_prob_round_3();
        played_first_move_in_round=true;
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

    cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
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

    cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
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
    
    cout<<"Out of "<<cases_all<<" cases, bot wins in "<<cases_bot_wins<<endl;
    prob_not_losing_against_player=double(cases_bot_wins)/cases_all;
}

void bot::play_move_round_1()
{
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
    double prob_not_losing_all_players=pow(prob_not_losing_against_player, players_in_game.size()-1);

    cout<<"Bots hand is\n"<<player_hand;
    cout<<"The prob the bot wins or ties against 1 player is :"<<prob_not_losing_against_player<<endl;
    cout<<"The prob the bot wins or ties against all player is :"<<prob_not_losing_all_players<<endl;

    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
}

void bot::play_move_round_3()
{  
    double prob_not_losing_all_players=pow(prob_not_losing_against_player, players_in_game.size()-1);

    cout<<"Bots hand is\n"<<player_hand;
    cout<<"The prob the bot wins or ties against 1 player is :"<<prob_not_losing_against_player<<endl;
    cout<<"The prob the bot wins or ties against all player is :"<<prob_not_losing_all_players<<endl;

    if (current_bet==0) check();
    else if (!collect_bet(current_bet- bet_in_round)) fold();
    else cout<<"The current bet is still "<<current_bet<<endl;
}