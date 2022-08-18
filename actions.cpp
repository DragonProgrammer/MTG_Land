#include "Debug.h"
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
#include "mana.h"
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
	hand.clear();
	initial_hand.clear();
	
	for (int i = 0; i < 7; i++) {
		hand.push_back(deck[0]);
		deck.erase(deck.begin());
	}
	initial_hand = hand;
}


//-----------------------------------------------------
// Simulate untap step
// 	passes by referance in_play cards to make everything untapped
// Empties mana pool
// 	TODO once phases are implemented this should be its own function
// increments turn counter
// sets per turn flags to 0
// -------------------------------------------------------
void actions::new_turn(vector<card>& in_play) {
	for (int i = 0; i < int(in_play.size()); i++) {
		card temp = in_play[i];
		temp.set_Mode('U');
		in_play[i] = temp;
	}
	turn_counter++;
	Land_per_turn_flag = 0;
	usable_mana.clear();
	DB("\nTurn " + to_string(turn_counter), 7);
}

int actions::draw_card() {
	if (deck.size() > 0) {
		hand.push_back(deck[0]);
		deck.erase(deck.begin());
		return 0;
	} else
		return -4;
}


//----------------------------------------------------------------
//This function checks if a given effect is on a card
//--------------------------------------------------------------

//TODO












//------------------------------------------------------------------
//this function checks to see if there is a land search option avalable
//	land search could come from:
//		 lands like Evolving wilds  - curren implementation only so far
//		 artifacts like travelers amulet  TODO
//		 spells like rampent growth  TODO
//		 creature ETB effects  TODO
//----------------------------------------------------------------

//int actions::search_available(){
	







//-----------------------------------------------------------------------------
//this function simulates terimorphic expance
//TODO split function so there is an effect search that looks for terimorfic and other sac to find land(s) and the actual search
//
//TODO possibly turn into land_search( char flag )  to determine if i am doing land search from hand or from field
//
//--------------------------------------------------------------------------
//Return Values
//   1 = no land search found
//   0 = all lands found
//  -1 = no lands to find
//  -2 = error happened in card removal from deck
//  -3 = error happened in card removal from field //TODO
//  -4 = found under the alotted lands //TODO  this should have some more reporting added to it
//
//-----------------------------------------------------------------------------
// this function updated for new card style
//-----------------------------------------------------------------------------
int actions::land_search() {
	int search_found_flag = 0;
	string search_Target;  // this might become vector string
	string land_Endpoint;
	string land_State;
	int Num_lands;

	effect search_Effect;

	card land_searcher;

	for (auto card_on_field : field) {
		//see if there is a search effect on the field, and that the cost is payable
		//TODO add in mana cost
		if (card_on_field.get_Effect().get_Eff_Type() == "Search" &&
		    card_on_field.get_Effect().get_Eff_Cost().find("Tap") != string::npos &&
		    card_on_field.get_Mode() == 'U') 
		{
			DB("found Land search", 0);
			search_found_flag = 1;
			land_searcher = card_on_field;  //at this point is search_avalability function either add this card to found vector or just return this card


		//set up variable to determine effects of search
			search_Effect = card_on_field.get_Effect();

			search_Target = search_Effect.get_Eff_Target_Type();
			land_Endpoint = search_Effect.get_Eff_Endpoint();
			land_State = search_Effect.get_Eff_State();
			Num_lands = search_Effect.get_Eff_Target_Numeric();
			
		}

	}
	if (search_found_flag == 0 )
		return 1; // no land search found


			//determine if I am seraching for basic land
			//TODO iplement panaramas and shard convergence
	if(search_Target.find("Basic") != string::npos){
				//create a vector of basic land cards from deck
		vector<card> lands_in_deck;
		for (auto card_in_deck : deck) {
			if (card_in_deck.is_of_Type(search_Target))
				lands_in_deck.push_back(card_in_deck);
		}
		if (lands_in_deck.size() > 0) {

				//TODO add found_land vector to implement Cultivate
				
			//	for(int l = 0; l < num_lands; l++){
			int found_number = 0; // to make sure i find only Num_lands
			while( lands_in_deck.size() > 0) {
				card found_land = find_land(lands_in_deck);
					
						
				found_number++;
				if(land_Endpoint == "Field"){
					field.push_back(found_land);
				       	if( land_State == "Tapped" )
						field[field.size() - 1].set_Mode('T');
					else
						field[field.size() - 1].set_Mode('U');
					}
				if(land_Endpoint == "Hand" ){
					hand.push_back(found_land);
				}
				if (remove_card(found_land, deck) == -3) {
					return -3;
				}  // somethuing happeded that remved land from deck premachurly

				if (found_number == Num_lands)
					break;
			}
		}
		if( remove_card(land_searcher, field) == -3 )
			return -3; //TODO difenciate this from the land removal
		return 0; //all lands found
	} else  // if size == 0
		return -1;  // no lands of target in deck
}


//TODO add traits Playable and Needed_to_play to card
//TODO create function that sets each of those traits for cards in hand
//
//
//TODO create a function that creates a need vector based on Needed_to_play
//	give priority to colors from cards that need only 1 more mana, and then number of cards that need that color
//	if tied decide with cards that need 2 manna of a color

// TODO see how to incoperate that into Decision_making functions for land decisions

//-------------------------------------------------------------------------------------------
//this function returns a land card from a  given a card vector (hand or deck) based on a vector of mana need
//TODO add 2 char flags to this for cards that search for only a basic, basic or gate, ext.
//	posibility this sifting may be done in another function whose results are passed to this
//TODO add subtype to card traits
//---------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//TODO this function needs to be updated for new card style
//-----------------------------------------------------------------------------
card actions::find_land(vector<card> lands) {
	vector<string> deciding_vector = compute_need();
	DBV(deciding_vector, -1);
	card played_land;
//	for (auto mana_need : deciding_vector) {
	for(int i = 0; i < 6; i++){
		string mana_need = deciding_vector[i];
		char land_color = mana_need[mana_need.length()-1];// this should return the mana symbol of the float string
		DB("mana need " << land_color, 0);
		for (auto selected_card : lands) {
			mana basic_test = selected_card.parse_Produces()[0]; // this should give me the mana a land could produce if it only proicuded one mana
			DB("\n\nTHis is the basic land mana:  " << basic_test << "\n\n\n", 7);
			if (selected_card.get_Enters() == 'U' &&
			    basic_test.can_produce(land_color)) {
				played_land = selected_card;
				DB("return land optuion: basic", -1);
				return played_land;
			}
		}
		for(auto selected_card : lands) {
			if(selected_card.get_Produces().length() == 5){//can produce 3 different colors
				played_land = selected_card;
				DB("return land optuion: 3 color", -1);
				return played_land;
			}
			}	
		
		string mana_need_2;
		char land_color_2;
		if(i != 5){
			mana_need_2 = deciding_vector[i+1];
			land_color_2 = mana_need_2[mana_need_2.length()-1];// this should return the mana symbol of the float string
		}
		
		int selected_flag = 0; // a card has been selected did not have land_color_2
		for (auto selected_card : lands) {
			vector<mana> bounce_test = selected_card.parse_Produces();
			if(bounce_test.size() == 1){
			mana trial = bounce_test[0]; // this dous not factor in bounce lands
			if (selected_card.get_Enters() == 'T' &&  trial.can_produce(land_color)) {
				played_land = selected_card;
				selected_flag = 1;
				if(trial.can_produce(land_color_2)){
				DB("return land optuion: 2 color good", -1);
					return played_land;
				}
			}
			}
			if(bounce_test.size() ==2){
			//TODO
			}
		}
		if(selected_flag == 1){
			DB("return land optuion: 2 color bad", -1);
			return played_land;
		}
		}
//find me a teramorphic
	for (auto selected_card : lands) {
		if (selected_card.get_Enters() == 'U' && selected_card.get_Effect().get_Eff_Type() == "Search") {
			played_land = selected_card;
			DB("return land optuion: teramorphic", -1);
			return played_land;
		}
	}
//find me a land that does not produce mana
	DB("return land optuion: any", -1);
	return lands[0];
}

//----------------------------------------
//this function plays land from hand
//
//----------------------------------------------
//return values
//
//   0 = succeccfully played land	
//  -1 = no lands in hand
//  -2 = error removeing land from hand
//  -3 = land_per_turn already played TODO
//-----------------------------------------------
//Updated for new format
//------------------------------------------

// play lands check needs to be -2  TODO<--- TO FIGURE THIS OUT
int actions::play_Land() {
	if (Land_per_turn_flag == 0) {
		vector<card> playable_lands;
		for (auto card : hand) {
			//	card.print_Card();
			if (card.is_of_Type("Land") == 1) {
				playable_lands.push_back(card);
			}
		}
		DBV(playable_lands, -1);
		if (playable_lands.size() < 1) {
			DB("no lands in hand", -1);
			return -1;
		}

		card played_land = find_land(playable_lands);
		DB("played land = " << played_land,12);
		Land_per_turn_flag = 1;
		field.push_back(played_land);  //TODO create put on field function that checks if enters taped

		DB("\nRemoving from hand and initial hand", 10);
		if (remove_card(played_land, hand) == -3) {
			return -3;
		};
		DBV(hand, 10);
		if (remove_card(played_land, initial_hand) == -3) {
			return -3;
		}
		DB("\ninitial Hand", 10);
		DBV(initial_hand, 10);
		DB("\nfield", 10);
		DBV( field, 10);
		return 0;
	}
	return 0; //TODO make this diferenciated so that it lets me know if play_land is called with land_per_tern flag triggered
}

//-----------------------------------------------------
//Fills mana pull with all available mana
//-----------------------------------------------------
//
//--------------------------------------------
//Needs to be updated for new format
//------------------------------
void actions::draw_all_Mana(vector<card>& in_play) {
	if(in_play.size() > 0) {
	for (auto& card_on_field : in_play) {
		DB( "the  " << card_on_field << "  produces  " << card_on_field.get_Produces(), 13);
		if (card_on_field.get_Produces() != "-" &&
		    toupper(card_on_field.get_Mode()) == 'U') {
			DB("Mode change", 3);
			DB(card_on_field.get_Mode(), 3);
			vector<mana> sourced;
			sourced = card_on_field.parse_Produces();

			string produces = card_on_field.get_Produces();

			if(int(produces.find('_')) -1){
				for (auto mana_produced : sourced) {
					usable_mana.push_back(mana_produced);
				}
			}

			else{
				mana_from_optional_sources.push_back(sourced[0]);
			}
			card_on_field.set_Mode('T');
			DB(card_on_field.get_Mode(), 3);
		}
	}
	}
	else{
		DB("No mana sources", -1);
	}
	//		VDB(1, field);
}

//--------------------------------------------
//Plays the biggest thing, or a given card
//
//TODO add in function to check about enting tapped
//
//-----------------------------------------
//return values
//
//   0 = success
//  -1 = no card passed to function
//  -2 = error with removal form hand
//  -3 = error with removal from initail hand TODO
//-------------------------------------------- 


int actions::play_biggest_thing(card Big_thing) {
	hand = hand;

	if (Big_thing.get_ID().length() == 0) {
		DBA("Nothing More to play");
		return -5;
	}
	vector<char> mana_cost = Big_thing.parse_Cost();
	check_mana(mana_cost, 'P');  //call needs to be updated TODO<<--- does it ?

	DB("\n\nplaying " <<  Big_thing, 12);

	DB( "\nRemoving from hand and initial hand", 10);

	field.push_back(Big_thing);  // enter field state check TODO
	if (remove_card(Big_thing, hand) == -3) {
		return -3;
	};

	DB( "\nhand", 10);
	DBV(hand, 10);

	if (remove_card(Big_thing, initial_hand) == -3) {
		return -3;
	};
	DB("\nInitial hand", 10);
	DBV(initial_hand, 10);
	return 0;
}

//-------------------------------------------------
//removes a card from a given vector
//
//--------------------------------------------------
//return values
//
// 0 = success
// -1 = not in selection Error
// -2 = game should end, nothing in intial hand TODO figure out why here
// ----------------------------------------------------------------------
// TODO Test with changes 
// --------------------------------------------


int actions::remove_card(card to_remove, vector<card>& remove_from) {
	string remove_ID = to_remove.get_ID();
	if (end_check() == 1) {
		return 1; //was -1, changed to go along with what other function arros ar looking for
	}  // need to chang here  - do not think so anymore
	for (int i = 0; i < int(remove_from.size()); i++) {
		if (remove_from[i].get_ID() == remove_ID) {
			remove_from.erase(remove_from.begin() + i);
			return 0;
		}
	}
	DB("\nNot it selection", 10);
	return -333; //was 0 changed to go along with what other function errors are looking for
}

//---------------------------------------------
//THis function checks for end of game
//	only one ending implemented now
//	   Current end of game is no more cards left from opening hand
//
//--------------------------------------------------------
//return values
//
//   0 = end conditions not met, continue
//  -1 = end condition met, stop loop 


int actions::end_check() {
	if (initial_hand.size() < 1) {
		return 1;
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
	DB("Found biggest in biggest_in_hand   " << biggest, 12);
	return biggest;
}

card actions::biggest_thing_playable() {
	int total_mana_avalable = usable_mana.size() + mana_from_optional_sources.size();

	card biggest;
	DB("Total usable mana   " + to_string(total_mana_avalable), 13);
	for (auto card_in_hand : hand) {
		int CMC = card_in_hand.get_CMC();
		DB("checking " << card_in_hand << " for biggest", 12);
		if (CMC <= total_mana_avalable && CMC > biggest.get_CMC()) {
			vector<char> mana_cost = card_in_hand.parse_Cost();
			DB("checking " << card_in_hand, 12);
			DB(card_in_hand, 13); // will want to check in course of mathing optional lands
			if (check_mana(mana_cost, 'C') != -1)  
				biggest = card_in_hand;
		}
	}
	DB("Found biggest in biggest_thing_playable   " << biggest, 12);
	return biggest;
}

/****************************************************************************
 * Mana manipulationn
 * 	things that change the mana pool
 * 	besides initial drawing mana as that tapps things too
 ******************************************************************************/


//------------------------------------------------------------------------------
// to check if the right mana is avalable, also functions as paying mana based on flag input
//
//TODO move flag check so we have 1 for loop, need to make sure optional mana works first
//TODO work in way to do hybrid mana, possibly from vector<char> to vector<string> or vector<mana>
//
//--------------------------------------------------------------------------------
//return values
//
//  0 = mana avalable OR mana paid
// -1 = mana not available OR mana cant be paid
//
//
//-----------------------------------------------------------------------------
//TODO this function needs to be updated for new card style
//-----------------------------------------------------------------------------
int actions::check_mana(vector<char> mana_cost, char flag) {
	vector<mana> temp_pool = usable_mana;	// incase of not right mana
	vector<mana> temp_optional = mana_from_optional_sources;

	if (flag == 'P') {                   // pay mana, actualy remove from pool
		DB("in payment", 2); // will want to check in coure of mathing optional lands
		for (auto cost_symbol : mana_cost) {
			if (remove_mana(cost_symbol) == -1) {
				DB( "\nPay failed", 2);
				usable_mana.clear();  // clear out mana vector
				usable_mana = temp_pool;  // set it back to what it was at start of function
				mana_from_optional_sources.clear();
				mana_from_optional_sources = temp_optional;
				return -1;
			}
		}
	} else {  // temporarily empty from pool
		DB("in check", 2);
		for (auto cost_symbol : mana_cost) {
			if (remove_mana(cost_symbol) == -1) {
				DB("\ncheck failed", 2);
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
	DB("mana available", 2);
	return 0;
}

//-----------------------------------------------------------------
//remove mana from pool
//
//TODO for latter reporting ability differentiate returns so I can tell from what pool
//-----------------------------------------------------
//return values
//
//  0 = sucessfuly removed
// -1 = could not remove
// --------------------------------------------------------


int actions::remove_mana(char mana_symbol) {
	for (int i = 0; i < int(usable_mana.size()); i++) {
//		if (toupper(usable_mana[i].get_produced) == toupper(mana_symbol))
		if(usable_mana[i].can_produce(toupper(mana_symbol)))
		{
			DB(usable_mana[i], 5);
			usable_mana.erase(usable_mana.begin() + i);
			return 0;
		}
	}
	for (int i = 0; i < int(mana_from_optional_sources.size()); i++) {
		if (mana_from_optional_sources[i].can_produce(toupper(mana_symbol))){
			DB(mana_from_optional_sources[i], 0);
			mana_from_optional_sources.erase(mana_from_optional_sources.begin() + i);
			return 0;
		}
	}
	//remove first mana from a pool to cover colorless
	//TODO change for generic mana sources ,, see if it alread gets rid of colorless first
	if (toupper(mana_symbol) == 'C') {
		if(usable_mana.size()>0){
		DB("for C " << usable_mana[0], 3);
			usable_mana.erase(usable_mana.begin());  
		}	
		else if(mana_from_optional_sources.size() > 0){
			DB( "for C " << mana_from_optional_sources[0], 3);
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


//TODO look up where these are called to get full understanding of what is happening, and what is still used



//----------------------------------------
//compute the percentage
//
//TODO rename variable for better understanding, Determine from where it is called if these make sence
//----------------------------------------------------------------------------------
vector<string> actions::compute_mana_percentages() {
	vector<float> functions_mana_percentages = compute_source_vector();
	DB("computed percentages", -1);
	vector<string> need_order;
	vector<char> mana_types = {'R', 'W', 'B', 'U', 'G', 'C'};
	for (int i = 0; i < int(mana_types.size()); i++) {
		string need_of_type = to_string(functions_mana_percentages[i]) + mana_types[i];
		need_order.push_back(need_of_type);
	}
	DB("made string vector", 0);
	sort(need_order.begin(),
	     need_order.begin() + 5);  // sorts R-G, Colorless stays at end
	DB("Mana needs", 0);
	DBV(need_order, 0);

	vector<string> descending_need_order;
	for (int i = 4; i > -1; i--) {
		descending_need_order.push_back(need_order[i]);
	}
	DB("Mana needs decending", 0);
	DBV(descending_need_order, 0);

	return need_order;
}

//------------------------------------------------------------------------------------------
//older function, for computing need for mono type only mana sources, will be debricated
//

vector<string> actions::compute_need() {
	vector<int> mana_needed_for_biggest =
	    mana_cost_numbers(biggest_thing_in_hand.parse_Cost());
	DB("computed for bigest", -1);
	vector<int> mana_requirements =
	    compute_dif(mana_pool_numbers(usable_mana, 'N'), mana_needed_for_biggest);
	DB("computed dif", -1);
	vector<string> need_order;
	vector<char> mana_types = {'R', 'W', 'B', 'U', 'G', 'C'};
	for (int i = 0; i < int(mana_types.size()); i++) {
		string need_of_type = to_string(mana_requirements[i]) + mana_types[i];
		need_order.push_back(need_of_type);
	}
	DB( "made string vector", 5);
	sort(need_order.begin(),
	     need_order.begin() + 5);  // sorts R-G, Colorless stays at end
	DB( "Mana needs", 5);
	// VDB(1, need_order);

	vector<string> descending_need_order;
	for (int i = 4; i > -1; i--) {
		descending_need_order.push_back(need_order[i]);
	}
	descending_need_order.push_back(need_order[5]);

	return descending_need_order;
}

//-----------------------------------------------------------------------
// Parses the cost of a spell into what is needed
// 	does not currently work with hybrid mana
//
// given a vector<char> representing the mana costs
// returns a vector<int> of mana needs by color
// 	order R, W, B, U, G, C
//------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------
// tallys how many mana sources there are, should be called before anything played
// 	possible thought making this number part of gameState 
// ---------------------------------------------------------------------------------	
int actions::compute_sources(){
	mana_sources = 0;
	for(auto c : field){
		if(c.get_Produces() != "-" && c.get_Mode() == 'U'){
			mana_sources++;
		}
	}
	return mana_sources;
}


//----------------------------------------------------------------------------------
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

//--------------------------------------------
//this returns the vector<float> of the difference between what i need of a color and what i have
//	any eccess have is used to reduce colorless need
//-----------------------------------------------------------------------------


vector<int> actions::compute_dif(vector<float> have, vector<int> need) {
	DB("in dif", -1);
	vector<int> difs;
	int extra = 0;
	for (int i = 0; i < int(have.size()); i++) {
		int dif = need[i] - have[i];
//		DB(5, need[i]);
//		DB(5, have[i]);
//		DB(5, dif);

		if (dif < 0) {
			extra += abs(dif);  // extra mana to factor out colorless need
			dif = 0;
		}
		difs.push_back(dif);
	}
	difs[5] -= extra;  // factoring out colorles need
	if (difs[5] < 0) difs[5] = 0;  //if i have more mana then needed 

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
	int total_runs = 10; //TODO later make this changeable by input
	int total_turns = 0;
	int failed_games = 0; //TODO posiblby make this an extern so i can output it in report
	
	DB("In Average function", 1);
	for (int run = 1; run <= total_runs; run++) {
		int turns = game_loop(input);
		if(turns == 0){
			failed_games++;
			DBA("Game " << to_string(run) << " failed");
			continue;
			}
		DBA("\nGame " + to_string(run) + "    took " + to_string(turns) + " turns.\n\n");
		total_turns += turns;
	}
	DB(total_turns, 10); // will have to check with failed games math later
	DBA("This deck resulted in " + to_string(failed_games) + " out of " + to_string(total_runs) + " games");
	if(total_runs-failed_games == 0)
		return 0;
	float average_turns = float(total_turns) / float(total_runs-failed_games);
	return average_turns;
}

int actions::game_loop(vector<card> input) {
	turn_counter = 0;
	set_deck(input);
	set_initial_Hand();
	field.clear();
	set_state(); // game state imnitializer
	DB("Initial Hand", 12);
	DBV(initial_hand, 12);
	while (1 == 1) {
		new_turn(field);
		DB("This is turn:   " + to_string(turn_counter) + "\n", 13);
		int loop_statement = draw_card();
		if (loop_statement == -4) {
			// end run
			DBA("Drew all cards in " + to_string(turn_counter) + " turns");
			set_state();// allows me to make report off of failed run
			DBA("Field at end of game");
			DBVA(field);
			return 0; //indicates failed run
		}
		draw_all_Mana(field);
		DB("\nFIeld 1: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana
		DBV( field, -1);
		DB("\n\nmana before land play", 13);  //TODO update these DB statements to include optional mana
		DBV( usable_mana, 13);

		biggest_thing_in_hand = biggest_in_hand();
		DB("biggest thing in hand", 5);
		DB("\nFIeld 2: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana
		DB(biggest_thing_in_hand,5);
		mana_need_to_play_biggest = compute_need();
		DB("\nMana for bigest",5);
		DB("\nFIeld 3: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana
		DBV( mana_need_to_play_biggest, 5);

		set_state(); // first set of options
		DB("\nFIeld 4: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana

		// check if played last card with land
	//	int land_options = land_search();
		DB("\nFIeld 5: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana
	
	//play lands section
		loop_statement = play_Land();
		if (loop_statement == -1) DBA( "no lands to play "<< to_string(turn_counter) );
		if (loop_statement == -2) DBA( "Multiple calls to play_land() this turn" );
		if (loop_statement == -3) DBA( "Error removing land from hand" );
		if (loop_statement == 1) {
			DBA("Field at end of game");
			DBVA(field);
			set_state();// allows me to make report of run
			return turn_counter;
		}

		draw_all_Mana(field);// adds mana from new land
		DB("\nFIeld 6: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana
		set_state();
		DB("\n\nmana after land play", 13);
		DB("\nFIeld 7: " + to_string(field.size()), -1);  //TODO update these DB statements to include optional mana
		DBV(usable_mana, 13);
		while(0==0){
			int mana_available_check = usable_mana.size() + mana_from_optional_sources.size();
			if (mana_available_check == 0){
				DBA("Done casting things");
				break;
			}

			card thing_to_play = biggest_thing_playable();
			string name_thing_to_play = thing_to_play.get_ID();
			DB("The card to play is:  " + name_thing_to_play, 14);
			if(name_thing_to_play.size() == 0)
				loop_statement = -5;
			if (loop_statement == -5){
				DBA("Cant play anything");
				break;
			}
			loop_statement = play_biggest_thing(biggest_thing_playable());
			// check if played last card with big thing
			
			if (loop_statement == -3) DBA( "Error removing biggest_thing from hand" );
			set_state(); // each spell makes a new gamestate

			loop_statement = end_check();
			if (loop_statement == 1) {
				DBA("Field at end of game");
				DBVA(field);
				set_state();// allows me to make report off of  run
				return turn_counter;
			}
			DBV(usable_mana, 5);
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

