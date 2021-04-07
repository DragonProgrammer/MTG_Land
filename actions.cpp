#ifndef DEBUG
#define DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#define DB_FLAG 0
#define DB(level, x) do{if(DB_FLAG > level){cerr << x << endl; }}while(0)
#define VDB(level, x) do{if(DB_FLAG > level){for(auto h : x){cerr << h << "  ";} cerr << endl;}}while(0)
#else
#define DB(level, x) do{if(DB_FLAG > level){}}while(0)
#define VDB(level, x) do{if(DB_FLAG > level){}}while(0)
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
#include <cmath>
#include <stdio.h>
#include <chrono>
#include <random>
using std::default_random_engine;
using std::sprintf;
using std::abs;
using std::sort;
using std::string;
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

	vector<char> usable_mana; //mana avalable to cast spells



	int Land_per_turn_flag = 0;

	int turn_counter = 0;

	vector<card> actions::get_deck(){return deck;}


		


		




/************************************************************
 * Game Actions
 * 	The part of the game the is seen
***********************************************************/


	void actions::set_deck(vector<card> from_main){
	//	srand(unsigned(time(0)));	
	
	//	random_shuffle(from_main.begin(), from_main.end());
		default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());
		shuffle(from_main.begin(), from_main.end(), rng);
	
		deck = from_main;

	}

	void actions::set_initial_Hand(){
		for(int i = 0; i < 7; i++){
			hand.push_back(deck[0]);
			deck.erase(deck.begin());
		}
		initial_hand = hand;
	}

	void actions::new_turn(vector<card> &in_play){
		for(int i = 0; i< in_play.size(); i++){
			card temp = in_play[i];
			temp.set_Mode('U');
			in_play[i] = temp;
		}
		turn_counter++;
		Land_per_turn_flag = 0;
		usable_mana.clear();
		DB(7,"\nTurn " + to_string(turn_counter));
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
	
	card actions::find_land(vector<card> lands){
			vector<string> deciding_vector = compute_need();
			card played_land;
			for(auto mana_need : deciding_vector){
				char land_color = mana_need[1];
				for(auto selected_card : lands){
					if(selected_card.get_Enters() == 'U'  && selected_card.get_Produces().find(land_color)){
						played_land = selected_card;
						return played_land;
					}
					}
				
				for(auto selected_card : lands){
					if(selected_card.get_Enters() == 'T'  && selected_card.get_Produces().find(land_color)){
						played_land = selected_card;
						return played_land;

					}
				}
					}
			
			for(auto selected_card : lands){
				if(selected_card.get_Enters() == 'U'){
					played_land = selected_card;
					return played_land;
					}
			}
	
			return lands[0];
	}		

// play lands check needs to be -2
	int actions::play_Land(){
		if(Land_per_turn_flag == 0){
			vector<card> playable_lands;
			for(auto card : hand){
			//	card.print_Card();
				if(card.get_Type() == 'L'){
					playable_lands.push_back(card);
				}
			}
			VDB(5, playable_lands);
			if(playable_lands.size() < 1){
				return -1;
			}
			
			card played_land = find_land(playable_lands);
			Land_per_turn_flag = 1;
			field.push_back(played_land);

			DB(5,"\nRemoving from hand and initial hand");
			if(remove_card(played_land, hand) == -1){ return -2;};
			VDB(10,hand);
			if(remove_card(played_land, initial_hand) == -1){return -2;}
			VDB(10, initial_hand);
			DB(10,"\nfield");
			VDB(10,field);
			return 0;
	}
		return 0;
	}

	void actions::draw_all_Mana(vector<card> &in_play){
		for(auto& card_on_field : in_play){
			if(card_on_field.get_Produces() != "0" && toupper(card_on_field.get_Mode()) == 'U'){
				DB(1, "Mode change");
				DB(1, card_on_field.get_Mode());
			
				vector<char> one_source = card_on_field.parse_Produces();
				for(auto mana_produced : one_source){
					usable_mana.push_back(mana_produced);
				}
				card_on_field.set_Mode('T');
				DB(1, card_on_field.get_Mode());
			}
		}
//		VDB(1, field);
	}
	

	int actions::play_biggest_thing(card Big_thing){
		hand = hand;
		if(Big_thing.get_ID().length() == 0){
			DB(5,"Nothing More to play");
			return -1;}
		vector<char> mana_cost = Big_thing.parse_Cost();
		check_mana(mana_cost, usable_mana, 'P');
		
		DB(1,"\n\nplaying");
		DB(1,Big_thing);
		
		DB(5,"\nRemoving from hand and initial hand");
		field.push_back(Big_thing);
		if(remove_card(Big_thing, hand)== -1){ return -2;};
		
		DB(10,"\nhand");
		//VDB(10, hand);
		
		if(remove_card(Big_thing, initial_hand) == -1){return -2;};
		DB(10,"\nInitial hand");
		//VDB(10, initial_hand);		
		return 0;
		}



	int actions::remove_card(card to_remove, vector<card> &remove_from){
	//	to_remove.print_Card();
		string remove_ID = to_remove.get_ID();
		if(end_check() == -1){ return -1;} //need to chang here
		for( int i = 0; i < remove_from.size(); i++){
			if(remove_from[i].get_ID() == remove_ID){
				remove_from.erase(remove_from.begin()+i);
				return 0;
			}

		}
		DB(1,"\nNot it selection");
		return 0;
	}


	int actions::end_check(){
		if(initial_hand.size() < 1){
			return -1;
		
		}
		return 0;
	}

/***************************************************************
 * Card Selection
 * 	functions that return card objects
 ***************************************************************/
	card actions::biggest_in_hand(){
		card biggest;
		for( auto card_in_hand : hand){
			int CMC = card_in_hand.get_CMC();
			if(CMC > biggest.get_CMC()){
				biggest = card_in_hand;
			}
		}
		return biggest;
	}
// this will need much changing once i add another color

	card actions::biggest_thing_playable(){
		int total_mana_avalable = usable_mana.size();
		card biggest;
		for( auto card_in_hand : hand){
			int CMC = card_in_hand.get_CMC();
			if(CMC <= total_mana_avalable && CMC > biggest.get_CMC()){
				vector<char> mana_cost = card_in_hand.parse_Cost();
				DB(3, "checking ");
				DB(3, card_in_hand);
				if(check_mana(mana_cost, usable_mana, 'C') != -1)
					biggest = card_in_hand;
			}
		}
		return biggest;
	}

/****************************************************************************
 * Mana manipulationn
 * 	things that change the mana pool
 * 	besides initial drawing mana as that tapps things too
******************************************************************************/

// to check if the right mana is avalable, also functions as paying mana

	int actions::check_mana(vector<char> mana_cost, vector<char> &mana_pool, char flag){
		vector<char> temp_pool = mana_pool; // incase of not right mana
		if(flag == 'P'){ //pay mana, actualy remove from pool
			DB(3, "in payment");
			for(auto cost_symbol : mana_cost){
				if(remove_mana(cost_symbol, mana_pool) == -1){
					DB(3,"\nPay failed");
					mana_pool.clear(); // clear out mana vector
					mana_pool = temp_pool; // set it back to what it was at start of function
					return -1;
				}
			}
		}
		else{  //temporarily empty from pool
			DB(1, "in check");
			for(auto cost_symbol : mana_cost){
				if(remove_mana(cost_symbol, mana_pool) == -1){
					DB(3,"\ncheck failed");
					mana_pool.clear(); // clear out mana vector
					mana_pool = temp_pool; // set it back to what it was at start of function
					return -1;
				}
			}
			mana_pool.clear(); // clear out mana vector
			mana_pool = temp_pool; // set it back to what it was at start of function
		}
		DB(3,"mana available");
		return 0;
	}


//removes mana from a given pool
//latter will have ability to remove mana from duel sources

	int actions::remove_mana(char mana_symbol, vector<char> &pool){
		for(int i = 0; i < pool.size(); i++){
			if(toupper(usable_mana[i]) == toupper(mana_symbol)){
				DB(1,string("Removed ") +  usable_mana[i]);
				usable_mana.erase(usable_mana.begin()+i);
				return 0;
			}
		}
		if(toupper(mana_symbol) == 'C'){
//				DB("Removed ");
//				DB(usable_mana[0]);
			pool.erase(usable_mana.begin()); // remove first mana to cover colorless
					//will need revamp when multicolor
			return 0;
			}

		return -1;
		}

/**************************************************************************
 * Decision making
 * 	fuctions that decide what card card to play now
 * 	Currently only determines what land needed next
 * 	Playing biggest thing first and later playing mana producers first is coded in and not decided
****************************************************************************/



	vector<string> actions::compute_need(){
		vector<int> mana_needed_for_biggest = mana_numbers(biggest_thing_in_hand.parse_Cost());
		DB(10, "computed for bigest");
		vector<int> mana_requirements = compute_dif(mana_numbers(usable_mana), mana_needed_for_biggest);
		DB(10, "computed dif");
		vector<string> need_order;
		vector<char> mana_types = {'R', 'W', 'B', 'U', 'G', 'C'};
		for(int i = 0; i < mana_types.size(); i++){
			string need_of_type = to_string(mana_requirements[i]) + mana_types[i];
			need_order.push_back(need_of_type);
		}
		DB(10, "made string vector");
		sort(need_order.begin(), need_order.begin()+5); //sorts R-G, Colorless stays at end
		DB(1, "Mana needs");
		//VDB(1, need_order);
		
		vector<string> descending_need_order;
		for(int i = 4; i > -1; i--){
			descending_need_order.push_back(need_order[i]);
		}


		return need_order;	

	}

	vector<int> actions::mana_numbers(vector<char> mana_vector){
		vector<int> numbers = {0, 0, 0, 0, 0, 0}; // {R, W, B, U, G, C}
		for(auto m : mana_vector){
			switch (toupper(m)){
				case 'R':
					numbers[0]++;
					break;
				case 'W':
					numbers[1]++;
					break;
				case 'B':
					numbers[2]++;
					break;
				case 'U':
					numbers[3]++;
					break;
				case 'G':
					numbers[4]++;
					break;
				case 'C':
					numbers[5]++;
					break;
				default:
					break;
			}
		}
		return numbers;
	}

	vector<int> actions::compute_dif(vector<int> have, vector<int> need){
		DB(1, "in dif");
		vector<int> difs;
		int extra = 0;
		for(int i = 0; i < have.size(); i++){
			int dif = need[i] - have[i];
			DB(5, need[i]);
			DB(5, have[i]);
			DB(5, dif);

			if(dif < 0){
				extra += abs(dif); //extra mana to factor out colorless need
				dif = 0;
			}
			difs.push_back(dif);
		}
		difs[5] -= extra; //factoring out colorles need
		if (difs[5] < 0)
			difs[5] = 0;



		return difs;
	}


/************************************************************************************
 * Output
 * 	Data colection functions
*************************************************************************************/


	void actions::print_selection(vector<card> to_output){
		if(to_output.size() < 1){ return;}
		for( auto card : to_output){
			card.print_Card();
		}
	}
	void turn_report();


/***************************************************************************************
* The Game
* 	The functions that determin the order things happen in
*********************************************************************************************/
	float actions::average_for_deck(vector<card> input){
		int total_runs = 50;
		int total_turns = 0;
		for(int run = 1; run < total_runs; run++){
			int turns = game_loop(input);
			cout << "\nTook " + to_string(turns) + " turns.";
			total_turns += turns;
		}
		DB(1, total_turns);
		float average_turns = float(total_turns)/float(total_runs);
		
		return average_turns;
	}

	int actions::game_loop(vector<card> input){
		turn_counter = 0;
		set_deck(input);
		set_initial_Hand();
		DB(6,"Initial Hand");
		VDB(0, initial_hand);		
		while(1==1){
			new_turn(field);
		if(draw_card() == -1){
		       //end run
			cout << "Drew all cards";
			return -1;
		}
		draw_all_Mana(field);
		DB(6,"\n\nmana before land play");
		VDB(6, usable_mana);

		biggest_thing_in_hand = biggest_in_hand();
		DB(6, "biggest thing in hand");
		DB(6, biggest_thing_in_hand);
		mana_need_to_play_biggest = compute_need();
		DB(6, "\nMana for bigest");
		VDB(6, mana_need_to_play_biggest);
	
		// check if played last card with land
		int end_check = play_Land();
		if(end_check == -1)
		       DB(6, "no lands to play this turn");	
	      	if(end_check == -2){
			return turn_counter;
			}
		
		draw_all_Mana(field);
		DB(6,"\n\nmana after land play");
		VDB(6, usable_mana);
		while((end_check = play_biggest_thing(biggest_thing_playable())) != -1){
			//check if played last card with big thing
			if(end_check == -2){
				return turn_counter;
				}	
			VDB(1, usable_mana);
			}
		}
	}

