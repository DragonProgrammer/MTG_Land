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
#include "game_data.h"

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


Game_Data game_play_record();

vector<Game_Data> deck_loop_data;

int Land_per_turn_flag = 0;

int turn_counter = 0;

int mana_sources =0;

/***********************************************************
 *  DECK FUNCTIONS
 *  ********************************************************/

vector<card> actions::get_deck() { return deck; }


//This will shuffle a selection of cards and make them the deck
void actions::set_deck(vector<card> from_main) {
	default_random_engine rng(
	    std::chrono::system_clock::now().time_since_epoch().count());
	shuffle(from_main.begin(), from_main.end(), rng);

	deck = from_main;
}


int actions::draw_card() {
	if (deck.size() > 0) {
		DB("Draw " + deck[0].get_Name(), 1);
		hand.push_back(deck[0]);
		deck.erase(deck.begin());
		return 0;
	} else
		return -4;
}



/***********************************************************
 *  GAME STATE FUNCTIONS
 *  ********************************************************/

GameState get_state(){ return state[state.size() -1]; }

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

/************************************************************
 * HAND FUNCTIONS
 * 	These functions work with the hand
 * 		Set initial hand,
 * 		Mulligan
 * 		Discarad
 * 		removing card from slection (can work with any vector<card>
 * 		finding biggest card
 ***********************************************************/


void actions::set_initial_Hand() {
	hand.clear();
	initial_hand.clear();
	
	for (int i = 0; i < 7; i++) {
		hand.push_back(deck[0]);
		deck.erase(deck.begin());
	}
	initial_hand = hand;
}


	/***************************************************************
 	* Card Selection
 	* 	functions that return card objects
 	***************************************************************/

//---------------------------------------------------------
// This function returns the Biggest CMC in hand
// 	If 1 or more cards have the same biggest CMC returns the first found
// 	returns an empty card if no card in hand has a CMC
//------------------------------------
card actions::biggest_in_hand() {
	card biggest_Hand;
	for (auto card_in_hand : hand) {
		int CMC = card_in_hand.get_CMC();
		if (CMC > biggest_Hand.get_CMC()) {
			biggest_Hand = card_in_hand;
		}
	}
	DB("Found biggest in biggest_in_hand   " << biggest_Hand, 14);
	return biggest_Hand;
}



//---------------------------------------------------------
// This function returns the Biggest card in hand that can be played
// 	If 1 or more cards meet the requirements it returns the first found
// 	returns an empty card if no card in hand can be cast
//------------------------------------
card actions::biggest_thing_playable() {
	int total_mana_avalable = usable_mana.size() + mana_from_optional_sources.size();
	
	card biggest_Playable;
	DB("This is biggest at start of function " << biggest_Playable,14);
	DB("Total usable mana   " + to_string(total_mana_avalable), 14);
	for (auto card_in_hand : hand) {
		if (card_in_hand.is_of_Type("Land") == 1){
			continue;
		}
		int CMC = card_in_hand.get_CMC();
		DB("checking " << card_in_hand << " for biggest", 14);
		if(biggest_Playable.get_CMC() == 0){
			DB("There is no card yet", 14);
		}
		else
			DB("The biggest playable CMC is " + to_string(biggest_Playable.get_CMC()),14);
		if (CMC <= total_mana_avalable && CMC > biggest_Playable.get_CMC()) {
			DB("This CMC is bigger and in the amount avalable", 14);
			vector<char> mana_cost = card_in_hand.get_Cost();
			DB("its cost is", 14);
			DBV(mana_cost,14);
			DB("checking " << card_in_hand, 13);
			DBV(mana_cost,13);
			if (check_mana(mana_cost, 'C') != -1)  
				biggest_Playable = card_in_hand;
			else
				DB("do not have mana for it",14);
		}
	}
	DB("Found biggest in biggest_thing_playable   " << biggest_Playable, 14);
	return biggest_Playable;
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
		DBA("Tried Playing " + to_remove.get_ID() + " after emptying initial hand");
		return -2; //was -1, changed to go along with what other function arros ar looking for
	}  // need to chang here  - do not think so anymore
	for (int i = 0; i < int(remove_from.size()); i++) {
		if (remove_from[i].get_ID() == remove_ID) {
			remove_from.erase(remove_from.begin() + i);
			return 0;
		}
	}
	DB("\nNot it selection", 10);
	return -1; //was 0 changed to go along with what other function errors are looking for
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
	DB("\nTurn " + to_string(turn_counter), 0
			);
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
			DB("found Land search", 3.1);
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
//TODO add a forward looking check that sees "If i have a multicolored/teramorfic/tap land and a basic, If I play the basic will still not be able to play a card. If so play nonbasic" 
//---------------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//TODO this function needs to be updated for new card style
//-----------------------------------------------------------------------------
card actions::find_basic(vector<card> lands, char mana_want){
	for (auto selected_card : lands) {
		vector <mana> basic_test;	
		if(selected_card.is_of_Type("Basic")){
		basic_test = selected_card.parse_Produces(); // this should give me back one man 
		if( basic_test.size() > 1){
			DBA(selected_card.get_Name() + " is a Basic, but produces more then 1 mana"); //TODO incoperate mana doublers
		}
		DB("\n" + selected_card.get_ID() + " is a basic land that produces  " + basic_test[0].get_produced() + "\n", 3.2);
		if (selected_card.get_Enters() == 'U' && basic_test[0].can_produce(land_color)) {
			played_land = selected_card;
			DB(played_land.get_ID() + " is returned as Basic from find_land\n", 3.2);
			return played_land;
			}
		}
	}
	return card no_basic;
}

card actions::find_multi_color_land(vector<card> lands, vector<string> mana_need, char mana_want){
	for(int second_want = 0; second_want < mana_need.size(); second_want++){
		mana_want_2 = mana_need[second_want];



card actions::find_land(vector<card> lands) {
	vector<string> deciding_vector = compute_need();
	DB("this is the need vector when finding lands", 3.2);
	DBV(deciding_vector, 3.2);
	card played_land;
//	for (auto mana_need : deciding_vector) {
	for(int i = 0; i < 6; i++){
		string mana_need = deciding_vector[i];
		char land_color = mana_need[mana_need.length()-1];// this should return the mana symbol of the float string
		DB("Mana looking for " << land_color, 0);
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
	vector<char> mana_cost = Big_thing.get_Cost();
	check_mana(mana_cost, 'P');  //call needs to be updated TODO<<--- does it ?

	DB("\n\nplaying " <<  Big_thing, 14);

	DB( "\nRemoving from hand and initial hand", 10);

	field.push_back(Big_thing);  // enter field state check TODO
	if (remove_card(Big_thing, hand) == -3) {
		return -3;
	};

	DB( "\nhand after play", 14444);
	DBV(hand, 10);

	if (remove_card(Big_thing, initial_hand) == -3) {
		return -3;
	};
	DB("\nInitial hand", 10);
	DBV(initial_hand, 10);
	return 0;
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
			DB("the cost i am checking is " << cost_symbol,14);
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
		DB("The mana I am comparing it to is  " << usable_mana[i].get_produced() , 14);
		if(usable_mana[i].can_produce(toupper(mana_symbol)))
		{
			DB("I found it can produce " << usable_mana[i], 14);
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
		DB("for C " << usable_mana[0], 14);
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
	    mana_cost_numbers(biggest_thing_in_hand.get_Cost());
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
// DEBUG level
//
// DBA - Always report the results for each game,  Always report averages for deck ---- TODO add this to a Deck report function
//
// 0.25  == Averaging math for decks
// -------------------------------------------------------------------------------------------

float actions::average_for_deck(vector<card> input) {
	deck_loop_data.clear();
	
	int total_runs = 1; //TODO later make this changeable by input
	int total_turns = 0;
	int failed_games = 0; //TODO posiblby make this an extern so i can output it in report
	
	DB("In Average function", 1);
	for (int run = 1; run <= total_runs; run++) {
		int turns = game_loop(input, run);
		if(turns == 0){
			failed_games++;
			DBA("Game " << to_string(run) << " failed");
			continue;
			}
		DBA("\nGame " + to_string(run) + "    took " + to_string(turns) + " turns.\n\n");
		total_turns += turns;
	}

	DBA("This deck resulted in " + to_string(failed_games) + " failed games, out of " + to_string(total_runs) + " games");
	if(total_runs-failed_games == 0)
		return 0;

	float average_turns = float(total_turns) / float(total_runs-failed_games);

	DB("Between " + to_string(total_runs) + " games this deck had " + to_string(failed_games) + ";" 
			+ "\nit had a total of " + to_string(total_turns) + " turns;"
			+ "\n resulting in " + to_string(average_turns) + " turns on average.", 0.25); 
			//TODO check the results of average function
	return average_turns;
}



/***********************************************************
 * Loop of game 
 * *****************/
 // DEBUG levels:
 //
 // DBA - Always report field at end of game,  Always state if you draw all cards, ALways report erros from removing cards
 // 
 // 0 -  Initial state
 //
 // 1 - mana report		



int actions::game_loop(vector<card> input, int run) {
	game_play_data.clear;
	game_play_data = new Game_Data(run);
	mana_report newest_mana_report;

	turn_counter = 0;

	turns_without_land_drop = 0;

	set_deck(input);
	set_initial_Hand();
	field.clear();
	set_state(); // game state imnitializer
	DB("Initial Hand", 14);
	DBV(initial_hand, 14);
	while (1 == 1) {
		new_turn(field);
		DB("This is turn:   " + to_string(turn_counter) + "\n", 14);
		int loop_statement = draw_card();
		if (loop_statement == -4) {
			// end run
			DBA("Drew all cards in " + to_string(turn_counter) + " turns");
			set_state();// allows me to make report off of failed run
			DBA("This game had " + to_string(turns_without_land_drop) + " turns with no lands to play");
			DBA("Field at end of game");
			DBVA(field);
			return 0; //indicates failed run
		}
		draw_all_Mana(field);
		
		newest_mana_report = game_play_data.create_mana_report(usable_mana, mana_from_optional_sources, turn_counter, "Start of turn");
		
		DB( report_mana(newest_mana_report) , 1);

		biggest_thing_in_hand = biggest_in_hand();
		
		DB(biggest_thing_in_hand,5);
		
		mana_need_to_play_biggest = compute_need();

		set_state(); // first set of options

		// check if played last card with land
		int land_options = land_search();
		if(land_options == -1){
			DBA("no lands of search target found");
		}//TODO Add in other DBs for 0 = no search found and 1 = all lands found
	
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
			// game limiter for testing
		if(turn_counter > 15){
			DBA("Field at end of game");
			DBVA(field);
			set_state();// allows me to make report off of  run
			return 0;
		}
		draw_all_Mana(field);// adds mana from new land
		
		set_state();
		
		while(0==0){
			int mana_available_check = usable_mana.size() + mana_from_optional_sources.size();
			if (mana_available_check == 0){
				DBA("Done casting things");
				break;
			}

			card thing_to_play = biggest_thing_playable();
			string name_thing_to_play = thing_to_play.get_ID();
			if(name_thing_to_play == "-"){
				DB("THis is blank card", 14);
				break;
			}
			DB("\nThe card to play is:  " + name_thing_to_play, 14);
			if(name_thing_to_play.size() == 0)
				loop_statement = -5;
			if (loop_statement == -5){
				DBA("Cant play anything");
				break;
			}
			loop_statement = play_biggest_thing(thing_to_play);
			// check if played last card with big thing
			
			if (loop_statement == -3) DBA( "Error removing biggest_thing from hand" );
			set_state(); // each spell makes a new gamestate


			DB("THe field after play", 14);
			DBV(field, 14);
			

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


//********************************************************
//GAME END FUNCTIONS
//	These functions check if I met a predeturmined kill point or a state based action that kills me
//	Predetermined end points are:
//		"Empty Initial Hand"
//		"Deal Lethal Damage" TODO
//		"Deal Lethal Damage + X" TODO
//		"Get to Turn X" // Implemented in new_turn
//		"State Based Actions" //implemented where relevent - always assumed to be looked for
//	These functions will change the end_flag from 0 to 1
//		And set end_statement to the termination condition
//*************************************************************
//
// Return conditions are:
// 	0 - Condition not met
// 	-1 - Condition met
//************************************************************


int actions::end_check() {
	if (end_flag ==1) {
		DBA("I am already done. CAUSE: " + end_statement);
		return -1
	}
	if(find(end_conditions.begin(), end_conditions.end(), "Empty Initial Hand") != v.end()){
		if (initial_hand.size() == 0){
			end_statement = "End Condition Met --- I emptied my initial hand ---";
			end_flag = 1;
			return -1;
		}
	if(find(end_conditions.begin(), end_conditions.end(), "Get to Turn") != v.end()){
		if(turn_limiter > 0 && turn_counter == turn_limiter){
			end_statement = "End Condition Met --- I got to Turn " + to_string(turn_limiter) + " ---";
			end_flag = 1;
			return -1;
		}	
	}
	return 0;
}


int actions::deck_size_check(){
	if (end_flag ==1) {
		DBA("I am already done. CAUSE: " + end_statement);
		return -1
	}
	if(deck.size() = 0){
		end_statement = "I LOSE --- I can not draw any more cards ---";
		end_flag = 1;
		return -1;
	}
	return 0;
}

//-----------------------------------
//This functions sets the end condtions from a given set of strings
//	only 2 implemented not
//----------------------------------------------
//TODO test Get to turn X

void set_end_condition(vector<string> conditions){
	
	if (conditions.size() > 0){ 
		for ( int c = 0; c < conditions.size(); c++){
			if(conditions[c] == "Empty Initial Hand"){
				DB("Found end conditon initial hand", 0.5);
				end_conditions.push_back("Empty Initial Hand");
			}
			if(find(conditions[c].begin(), conditions[c].end(), "Get to Turn ") != conditions[c].end()){
				end_conditions.push_back("Get to Turn");
				string turn_string = condition[c].substr(12);
				DB("Found Turn limit of: " + turn_string, 0.5);
				int turn_limit = stoi(turn_string);
				turn_limiter = turn_limit;
			}

		}
	}
}
	

