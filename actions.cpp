#ifndef DEBUG
#define DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#define DB_FLAG 3
#define DB(level, x) do{if(DB_FLAG > level){cerr << x << endl; }}while(0)
#else
#define DB(level, x) do{if(DB_FLAG > level){}}while(0)
#endif

#include "card.h"
#include "actions.h"
#include <vector>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <string>
#include <cstdlib>
#include <cctype>
using std::cout;
using std::endl;
using std::random_shuffle;
using std::srand;
using std::time;
using std::to_string;
using std::vector;
using std::toupper;

	vector<card> hand;

	vector<card> field;

	vector<card> deck;

	vector<string> usable_mana;

	int Land_per_turn_flag = 0;

	int turn_counter = 0;

	vector<card> actions::get_deck(){return deck;}

	void actions::set_deck(vector<card> from_main){
		srand(unsigned(time(0)));	
	
		random_shuffle(from_main.begin(), from_main.end());
		deck = from_main;

		//shuffle test
//		for( auto deck_card : deck){
//			card this_card = deck_card;
//			this_card.print_Card();
//		}
	}
		

	void actions::set_initial_Hand(){
//		DB(to_string(deck.size()));
		for(int i = 0; i < 7; i++){
			hand.push_back(deck[0]);
			deck.erase(deck.begin());
		}
		initial_hand = hand;
//		DB(to_string(deck.size()));
	}

	void actions::new_turn(){
		for(auto cards : field){
			cards.set_Mode('U');
		}
		turn_counter++;
		Land_per_turn_flag = 0;
		usable_mana.clear();
		DB(1,"\nTurn " + to_string(turn_counter));
	}

	int actions::draw_card(){
		if(deck.size()>0){
			hand.push_back(deck[0]);
			deck.erase(deck.begin());
			return 0;
		}
		else
			return-1;
			}
			

	int actions::play_Land(){
		if(Land_per_turn_flag == 0){
			vector<card> playable_lands;
			for(auto card : hand){
			//	card.print_Card();
				if(card.get_Type() == 'L'){
					playable_lands.push_back(card);
				}
			}
		//	for(auto card : playable_lands){
		//		card.print_Card();
		//	}
			if(playable_lands.size() < 1){
				return -1;
			}
			for(auto selected_card : playable_lands){
				if(selected_card.get_Enters() == 'U'){
					card played_land = selected_card;
				       	played_land.set_Mode('U');	
					Land_per_turn_flag = 1;
					field.push_back(played_land);
					DB(5,"\nRemoving from hand");
					hand = remove_card(played_land, hand);
					DB(1,"\nhand");
					DB(1, hand[0]);
//					print_selection(hand);
//					DB("\nRemoving from Initial");
					initial_hand = remove_card(played_land, initial_hand);
//					DB("\nInitial hand");
//					print_selection(initial_hand);
					DB(1,field[0]);
					return 0;

				}
			}

			}
			
		return 0;
	}

	vector<card> actions::remove_card(card to_remove, vector<card> remove_from){
		to_remove.print_Card();
		string remove_ID = to_remove.get_ID();
		end_check();
		for( int i = 0; i < remove_from.size(); i++){
			if(remove_from[i].get_ID() == remove_ID){
				remove_from.erase(remove_from.begin()+i);
				return remove_from;
			}

		}
		DB(1,"\nNot it selection");
		return remove_from;
	}

	void actions::draw_all_Mana(){
		for(auto card_on_field :field){
			if(card_on_field.get_Produces() != "0" && card_on_field.get_Mode() == 'U'){
				vector<char> one_source = card_on_field.parse_Produces();
				for(auto mana_produced : one_source){
					usable_mana.push_back(mana_produced);
				}
				card_on_field.set_Mode('T');
			}
		}
	}

	void actions::end_check(){
		if(initial_hand.size() < 1){
			cout << "It took " + to_string(turn_counter) <<endl;
			exit(0);
		
		}
	}


	void actions::print_selection(vector<card> to_output){
		if(to_output.size() < 1){ return;}
		for( auto card : to_output){
			card.print_Card();
		}
	}

// this will need much changing once i add another color

	card actions::biggest_thing_playable(){
		int total_mana_avalable = usable_mana.size();
		card biggest;
		for( auto card_in_hand : hand){
			int CMC = card_in_hand.get_CMC();
			if(CMC <= total_mana_avalable && CMC > biggest.get_CMC()){
				biggest = card_in_hand;
			}
		}
		return biggest;
	}

	int actions::play_biggest_thing(card Big_thing){
		hand = hand;
		if(Big_thing.get_ID().length() == 0){
			DB(5,"Nothing More to play");
			return -1;}
		vector<char> mana_cost = Big_thing.parse_Cost();
		for(auto cost_symbol : mana_cost){
			if(remove_mana(cost_symbol) == -1){
				DB(5,"\ndo not have the right type of mana");
				return -1;
			}
		}
			DB(1,"\n\nplaying");
		Big_thing.print_Card();
		DB(1,"\nRemoving from hand");
			field.push_back(Big_thing);
		hand = remove_card(Big_thing, hand);
		DB(5,"\nhand");
		print_selection(hand);
		DB(1,"\nRemoving from Initial");
		initial_hand = remove_card(Big_thing, initial_hand);
					DB(1,"\nInitial hand");
					for(auto h : initial_hand){DB(1,h);}
		return 0;
		}


			//we have already determined that i have enough mana to cover cmc
	int actions::remove_mana(char mana_symbol){
		for(int i = 0; i < usable_mana.size(); i++){
			if(toupper(usable_mana[i]) == toupper(mana_symbol)){
//				DB("Removed ");
//				DB(usable_mana[i]);
				usable_mana.erase(usable_mana.begin()+i);
				return 0;
			}
		}
		if(toupper(mana_symbol) == 'C'){
//				DB("Removed ");
//				DB(usable_mana[0]);
			usable_mana.erase(usable_mana.begin()); // remove first mana to cover colorless
					//will need revamp when multicolor
			return 0;
			}

		return -1;
		}

	void turn_report();

	int check_remaining_cards();

	int actions::game_loop(vector<card> input){
		turn_counter = 0;
		set_deck(input);
		set_initial_Hand();
		DB(1,"iInitial Hand");
//		print_selection(initial_hand);
		while(1==1){
			new_turn();
		
		if(draw_card() == -1){
		       //end run
			DB(1,"Drew all cards");
			return -1;
		}
		if(play_Land() == -1){
			DB(1,"no lands to play this turn");
		}
		if(initial_hand.size() == 0){
//			break;
		}
		draw_all_Mana();
		DB(5,"\n\nmana");
		//	for(auto mana : usable_mana){
		//		cout << mana + to_string(mana) << endl;
		//	}
			DB(1," ");
		while(play_biggest_thing(biggest_thing_playable()) != -1){
			for(auto mana : usable_mana){
				DB(5, mana);
			}
		if(initial_hand.size() == 0){
//			break;
		}
		}
		}
		DB(1," ");
		cout << "Took " + to_string(turn_counter) + " turns.";
		return 0;
	}

