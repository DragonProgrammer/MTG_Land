#ifndef DEBUG
#define DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#define DB_FLAG 1
#define DB(level, x)       \
	do {                     \
		if (DB_FLAG > level) { \
			cerr << x << endl;   \
		}                      \
	} while (0)
#define VDB(level, x)      \
	do {                     \
		if (DB_FLAG > level) { \
			for (auto h : x) {   \
				cerr << h << "  "; \
			}                    \
			cerr << endl;        \
		}                      \
	} while (0)
#else
#define DB(level, x)       \
	do {                     \
		if (DB_FLAG > level) { \
		}                      \
	} while (0)
#define VDB(level, x)      \
	do {                     \
		if (DB_FLAG > level) { \
		}                      \
	} while (0)
#endif
#include <stdio.h>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "actions.h"
#include "card.h"
#include "cost.h"
using std::abs;
using std::cout;
using std::default_random_engine;
using std::endl;
using std::random_shuffle;
using std::sort;
using std::sprintf;
using std::srand;
using std::string;
using std::time;
using std::to_string;
using std::toupper;
using std::vector;

vector<card> hand;

vector<card> field;

vector<card> deck;

vector<mana> usable_mana;  // mana avalable to cast spells

int Land_per_turn_flag = 0;

int turn_counter = 0;

int mana_sources =0;

vector<card> actions::get_deck() { return deck; }

/************************************************************
 * Game Actions
 * 	The part of the game the is seen
 ***********************************************************/

void actions::set_deck(vector<card> from_main) {
	//	srand(unsigned(time(0)));

	//	random_shuffle(from_main.begin(), from_main.end());
	default_random_engine rng(
	    std::chrono::system_clock::now().time_since_epoch().count());
	shuffle(from_main.begin(), from_main.end(), rng);

	deck = from_main;
}

void actions::set_initial_Hand() {
	for (int i = 0; i < 7; i++) {
		hand.push_back(deck[0]);
		deck.erase(deck.begin());
	}
	initial_hand = hand;
}

void actions::new_turn(vector<card>& in_play) {
	for (int i = 0; i < in_play.size(); i++) {
		card temp = in_play[i];
		temp.set_Mode('U');
		in_play[i] = temp;
	}
	turn_counter++;
	Land_per_turn_flag = 0;
	usable_mana.clear();
	DB(7, "\nTurn " + to_string(turn_counter));
}

int actions::draw_card() {
	if (deck.size() > 0) {
		hand.push_back(deck[0]);
		deck.erase(deck.begin());
		return 0;
	} else
		return -1;
}

int actions::land_search() {
	for (auto card_on_field : field) {
		if (card_on_field.get_ECost() == 'S' &&
		    card_on_field.get_Effect() == "Search" &&
		    card_on_field.get_Mode() == 'U') {
			DB(0, "found Land search");
			vector<card> lands_in_deck;
			for (auto card_in_deck : deck) {
				if (card_in_deck.get_Type() == 'L')
					lands_in_deck.push_back(card_in_deck);
			}
			if (lands_in_deck.size() > 0) {
				card found_land = find_land(lands_in_deck);
				field.push_back(found_land);
				field[field.size() - 1].set_Mode('T');
				if (remove_card(found_land, deck) == -1) {
					return -2;
				}  // somethuing happeded that remved land from deck premachurly
				return 0;
			} else  // if size == 0
				return -1;
		}
	}
	return 1;  // no land search found
}

card actions::find_land(vector<card> lands) {
	vector<string> deciding_vector = compute_need();
	card played_land;
	for (auto mana_need : deciding_vector) {
		char land_color = mana_need[1];
		for (auto selected_card : lands) {
			if (selected_card.get_Enters() == 'U' &&
			    selected_card.get_Produces().find(land_color)>-1) {
				played_land = selected_card;
				return played_land;
			}
		}

		for (auto selected_card : lands) {
			if (selected_card.get_Enters() == 'T' &&
			    selected_card.get_Produces().find(land_color)>-1) {
				played_land = selected_card;
				return played_land;
			}
		}
	}

	for (auto selected_card : lands) {
		if (selected_card.get_Enters() == 'U') {
			played_land = selected_card;
			return played_land;
		}
	}

	return lands[0];
}

// play lands check needs to be -2
int actions::play_Land() {
	if (Land_per_turn_flag == 0) {
		vector<card> playable_lands;
		for (auto card : hand) {
			//	card.print_Card();
			if (card.get_Type() == 'L') {
				playable_lands.push_back(card);
			}
		}
		VDB(5, playable_lands);
		if (playable_lands.size() < 1) {
			return -1;
		}

		card played_land = find_land(playable_lands);
		Land_per_turn_flag = 1;
		field.push_back(played_land);

		DB(5, "\nRemoving from hand and initial hand");
		if (remove_card(played_land, hand) == -1) {
			return -2;
		};
		VDB(10, hand);
		if (remove_card(played_land, initial_hand) == -1) {
			return -2;
		}
		VDB(10, initial_hand);
		DB(10, "\nfield");
		VDB(10, field);
		return 0;
	}
	return 0;
}

void actions::draw_all_Mana(vector<card>& in_play) {
	for (auto& card_on_field : in_play) {
		if (card_on_field.get_Produces() != "-" &&
		    toupper(card_on_field.get_Mode()) == 'U') {
			DB(1, "Mode change");
			DB(1, card_on_field.get_Mode());
			vector<mana> sourced;
			sourced = card_on_field.parse_Produces();

			string produces = card_on_field.get_Produces();

			if(produces.find('_') == -1){
				for (auto mana_produced : sourced) {
					usable_mana.push_back(mana_produced);
				}
			}

			else{
				mana_from_optional_sources.push_back(sourced[0]);
			}
			card_on_field.set_Mode('T');
			DB(1, card_on_field.get_Mode());
		}
	}
	//		VDB(1, field);
}

int actions::play_biggest_thing(card Big_thing) {
	hand = hand;
	if (Big_thing.get_ID().length() == 0) {
		DB(5, "Nothing More to play");
		return -1;
	}
	vector<char> mana_cost = Big_thing.parse_Cost();
	check_mana(mana_cost, 'P');  //call needs to be updated

	DB(1, "\n\nplaying");
	DB(1, Big_thing);

	DB(5, "\nRemoving from hand and initial hand");
	field.push_back(Big_thing);
	if (remove_card(Big_thing, hand) == -1) {
		return -2;
	};

	DB(10, "\nhand");
	// VDB(10, hand);

	if (remove_card(Big_thing, initial_hand) == -1) {
		return -2;
	};
	DB(10, "\nInitial hand");
	// VDB(10, initial_hand);
	return 0;
}

int actions::remove_card(card to_remove, vector<card>& remove_from) {
	string remove_ID = to_remove.get_ID();
	if (end_check() == -1) {
		return -1;
	}  // need to chang here  - do not think so anymore
	for (int i = 0; i < remove_from.size(); i++) {
		if (remove_from[i].get_ID() == remove_ID) {
			remove_from.erase(remove_from.begin() + i);
			return 0;
		}
	}
	DB(1, "\nNot it selection");
	return 0;
}

int actions::end_check() {
	if (initial_hand.size() < 1) {
		return -1;
	}
	return 0;
}

/***************************************************************
 * Card Selection
 * 	functions that return card objects
 ***************************************************************/
card actions::biggest_in_hand() {
	card biggest;
	for (auto card_in_hand : hand) {
		int CMC = card_in_hand.get_CMC();
		if (CMC > biggest.get_CMC()) {
			biggest = card_in_hand;
		}
	}
	return biggest;
}
// this will need much changing once i add another color

card actions::biggest_thing_playable() {
	int total_mana_avalable = usable_mana.size() + mana_from_optional_sources.size();
	card biggest;
	for (auto card_in_hand : hand) {
		int CMC = card_in_hand.get_CMC();
		if (CMC <= total_mana_avalable && CMC > biggest.get_CMC()) {
			vector<char> mana_cost = card_in_hand.parse_Cost();
			DB(3, "checking ");
			DB(3, card_in_hand);
			if (check_mana(mana_cost, 'C') != -1) //check line need update 
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

int actions::check_mana(vector<char> mana_cost, char flag) {
	vector<mana> temp_pool = usable_mana;	// incase of not right mana
	vector<mana> temp_optional = mana_from_optional_sources;

	if (flag == 'P') {                   // pay mana, actualy remove from pool
		DB(3, "in payment");
		for (auto cost_symbol : mana_cost) {
			if (remove_mana(cost_symbol) == -1) {
				DB(3, "\nPay failed");
				usable_mana.clear();  // clear out mana vector
				usable_mana = temp_pool;  // set it back to what it was at start of function
				mana_from_optional_sources.clear();
				mana_from_optional_sources = temp_optional;
				return -1;
			}
		}
	} else {  // temporarily empty from pool
		DB(1, "in check");
		for (auto cost_symbol : mana_cost) {
			if (remove_mana(cost_symbol) == -1) {
				DB(3, "\ncheck failed");
				usable_mana.clear();  // clear out mana vector
				usable_mana =  temp_pool;  // set it back to what it was at start of function
				mana_from_optional_sources.clear();
				mana_from_optional_sources = temp_optional;
				return -1;
			}
		}
		usable_mana.clear();      // clear out mana vector
		usable_mana = temp_pool;  // set it back to what it was at start of function
		mana_from_optional_sources.clear();
		mana_from_optional_sources = temp_optional;
	}
	DB(3, "mana available");
	return 0;
}


int actions::remove_mana(char mana_symbol) {
	for (int i = 0; i < usable_mana.size(); i++) {
//		if (toupper(usable_mana[i].get_produced) == toupper(mana_symbol))
		if(usable_mana[i].can_produce(toupper(mana_symbol)))
		{
			DB(1, usable_mana[i]);
			usable_mana.erase(usable_mana.begin() + i);
			return 0;
		}
	}
		for (int i = 0; i < mana_from_optional_sources.size(); i++) {
			if (mana_from_optional_sources[i].can_produce(toupper(mana_symbol))){
				DB(1, mana_from_optional_sources[i]);
				mana_from_optional_sources.erase(mana_from_optional_sources.begin() + i);
				return 0;
			}
		}
		//remove first mana from a pool to cover colorless
		if (toupper(mana_symbol) == 'C') {
			if(usable_mana.size()>0){
			DB(1,"for C " << usable_mana[0]);
				usable_mana.erase(usable_mana.begin());  
			}	
		else if(mana_from_optional_sources.size() > 0){
				DB(1, "for C " << mana_from_optional_sources[0]);
			mana_from_optional_sources.erase(mana_from_optional_sources.begin()); 
	}       
		return 0;
	}

	return -1;
}

/**************************************************************************
 * Decision making
 * 	fuctions that decide what card card to play now
 * 	Currently only determines what land needed next
 * 	Playing biggest thing first and later playing mana producers first is coded
 *in and not decided
 ****************************************************************************/

vector<string> actions::compute_mana_percentages() {
	vector<float> mana_percentages = compute_source_vector();
	DB(10, "computed percentages");
	vector<string> need_order;
	vector<char> mana_types = {'R', 'W', 'B', 'U', 'G', 'C'};
	for (int i = 0; i < mana_types.size(); i++) {
		string need_of_type = to_string(mana_percentages[i]) + mana_types[i];
		need_order.push_back(need_of_type);
	}
	DB(10, "made string vector");
	sort(need_order.begin(),
	     need_order.begin() + 5);  // sorts R-G, Colorless stays at end
	DB(1, "Mana needs");
	VDB(1, need_order);

	vector<string> descending_need_order;
	for (int i = 4; i > -1; i--) {
		descending_need_order.push_back(need_order[i]);
	}
	DB(1, "Mana needs decending");
	VDB(1, descending_need_order);

	return need_order;
}

//older function, for computing need for mono type only mana sources, will be debricated
vector<string> actions::compute_need() {
	vector<int> mana_needed_for_biggest =
	    mana_cost_numbers(biggest_thing_in_hand.parse_Cost());
	DB(10, "computed for bigest");
	vector<int> mana_requirements =
	    compute_dif(mana_pool_numbers(usable_mana, 'N'), mana_needed_for_biggest);
	DB(10, "computed dif");
	vector<string> need_order;
	vector<char> mana_types = {'R', 'W', 'B', 'U', 'G', 'C'};
	for (int i = 0; i < mana_types.size(); i++) {
		string need_of_type = to_string(mana_requirements[i]) + mana_types[i];
		need_order.push_back(need_of_type);
	}
	DB(10, "made string vector");
	sort(need_order.begin(),
	     need_order.begin() + 5);  // sorts R-G, Colorless stays at end
	DB(1, "Mana needs");
	// VDB(1, need_order);

	vector<string> descending_need_order;
	for (int i = 4; i > -1; i--) {
		descending_need_order.push_back(need_order[i]);
	}

	return need_order;
}

// Parses the cost of a spell into what is needed
vector<int> actions::mana_cost_numbers(vector<char> mana_vector) {
	vector<int> numbers = {0, 0, 0, 0, 0, 0};  // {R, W, B, U, G, C}
	for (auto m : mana_vector) {
		switch (toupper(m)) {
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


// tallys how many mana sources there are, should be called before anything played
// 	possible thought making this number part of gameState 
int actions::compute_sources(){
	mana_sources = 0;
	for(auto c : field){
		if(c.get_Produces() != "-" && c.get_Mode() == 'U'){
			mana_sources++;
		}
	}
	return mana_sources;
}


// returns the percent of sources that can produce a given color of mana
//    anther number for game_state?
vector<float> actions::compute_source_vector(){
	vector<float> available_mana = mana_pool_numbers(usable_mana, 'O');
	float source_number = compute_sources();
	vector<float> mana_percentage;
	for(auto m : available_mana){
		float mana_math = m/source_number;
		mana_percentage.push_back(mana_math);
	}
	return mana_percentage;
}


		
			



// to get teoretical avalibility factoring in option sources
vector<float> actions::mana_pool_numbers(vector<mana> mana_vector, char Flag) {
	vector<float> numbers = {0, 0, 0, 0, 0, 0};  // {R, W, B, U, G, C}
	for (auto m : mana_vector) {
		char check = m.get_produced();
		switch (toupper(check)) {
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
	if(Flag == 'O'){ // O for optional
		for(auto m : mana_from_optional_sources){
				if(m.can_produce('R'))
					numbers[0]++;
				if(m.can_produce('W'))
					numbers[1]++;
				if(m.can_produce('B'))
					numbers[2]++;
				if(m.can_produce('U'))
					numbers[3]++;
				if(m.can_produce('G'))
					numbers[4]++;
				if(m.can_produce('C'))
					numbers[5]++;
			}
		}
	
	return numbers;
}
vector<int> actions::compute_dif(vector<float> have, vector<int> need) {
	DB(1, "in dif");
	vector<int> difs;
	int extra = 0;
	for (int i = 0; i < have.size(); i++) {
		int dif = need[i] - have[i];
		DB(5, need[i]);
		DB(5, have[i]);
		DB(5, dif);

		if (dif < 0) {
			extra += abs(dif);  // extra mana to factor out colorless need
			dif = 0;
		}
		difs.push_back(dif);
	}
	difs[5] -= extra;  // factoring out colorles need
	if (difs[5] < 0) difs[5] = 0;

	return difs;
}

/************************************************************************************
 * Output
 * 	Data colection functions
 *************************************************************************************/

void actions::print_selection(vector<card> to_output) {
	if (to_output.size() < 1) {
		return;
	}
	for (auto card : to_output) {
		card.print_Card();
	}
}
void turn_report();

/***************************************************************************************
 * The Game
 * 	The functions that determin the order things happen in
 *********************************************************************************************/
float actions::average_for_deck(vector<card> input) {
	int total_runs = 50; //TODO later make this changeable by input
	int total_turns = 0;
	int failed_games = 0; //TODO posiblby make this an extern so i can output it in report
	for (int run = 0; run < total_runs; run++) {
		int turns = game_loop(input);
		if(turns == 0)
			failed_games++;
		DB(1, "\nTook " + to_string(turns) + " turns.");
		total_turns += turns;
	}
	DB(1, total_turns);
	float average_turns = float(total_turns) / float(total_runs-failed_games);
	return average_turns;
}

int actions::game_loop(vector<card> input) {
	turn_counter = 0;
	set_deck(input);
	set_initial_Hand();
	set_state(); // game state imnitializer
	DB(6, "Initial Hand");
	VDB(1, initial_hand);
	while (1 == 1) {
		new_turn(field);
		if (draw_card() == -1) {
			// end run
			cout << "Drew all cards";
			set_state();// allows me to make report off of failed run
			return 0; //indicates failed run
		}
		draw_all_Mana(field);
		DB(6, "\n\nmana before land play");
		VDB(6, usable_mana);

		biggest_thing_in_hand = biggest_in_hand();
		DB(6, "biggest thing in hand");
		DB(6, biggest_thing_in_hand);
		mana_need_to_play_biggest = compute_need();
		DB(6, "\nMana for bigest");
		VDB(6, mana_need_to_play_biggest);

		set_state(); // first set of options

		// check if played last card with land
		int land_options = land_search();
		int end_check = play_Land();
		if (end_check == -1) DB(6, "no lands to play this turn");
		if (end_check == -2) {
			return turn_counter;
		}

		draw_all_Mana(field);// adds mana from new land
		set_state();
		DB(6, "\n\nmana after land play");
		VDB(6, usable_mana);
		while ((end_check = play_biggest_thing(biggest_thing_playable())) != -1) {
			// check if played last card with big thing
			
			set_state(); // each spell makes a new gamestate

			if (end_check == -2) {
				return turn_counter;
			}
			VDB(1, usable_mana);
		}
	}
}

/*************************************GAME STATE ******************************************/

void actions::set_state(){
	GameState temp;
	temp.S_hand = hand;
	temp.S_field= field;
	temp.S_deck = deck;
	temp.S_usable_mana = usable_mana;
	temp.S_turn_counter = turn_counter;
	temp.S_Land_flag = Land_per_turn_flag;
	state.push_back(temp);
}

