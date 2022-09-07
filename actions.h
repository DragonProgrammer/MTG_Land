#ifndef ACTIONS_H
#define ACTIONS_H
#include "card.h"
#include <vector>
#include <string>
#include "mana.h"
#include "GameState.h"
class actions{
	
	// card vectors
	vector<card> hand;

	vector<card> initial_hand;

	vector<card> field;

	vector<card> deck;


	//card objects
	
	card last_card_played;
	
	card biggest_thing_in_hand;

	// mana vectors
	
	vector<mana> usable_mana;

	vector<mana> mana_from_optional_sources; // you are going to be prblamatic

	// mana math varialbes

	vector<float> mana_percentages;

	vector<string> mana_need_to_play_biggest;
	
	
	vector<GameState> state;


	int Land_per_turn_flag = 0;

	int turn_counter = 0;


	// end condtion varialbles
	int turn_limiter;
	
	vector<string> end_condition;
	
	int end_flag = 0;
	
	string end_statement = "";

	public:

//--------------- DECK ------------------------------------

	vector<card> get_deck();

	// shuffle the deck
	void set_deck(vector<card> from_main);

//TODO  void shuffle_deck(vector<card> card_selection);

	int draw_card();

//----------- GAME STATE ---------------------------------

	GameState get_state();

	void set_state();

//-----------  HAND --------------------------------------

	void set_initial_Hand();

//TODO  int mulligan_check();

//TODO  void mulligan_initial_hand(int mulligan_count);
	
	card biggest_in_hand();

//TODO  void discard_n_cards(int n);

//TODO  void discard_for_cleanup();

	int remove_card(card to_remove, vector<card> &remove_from);

//------------- TURN STRUCTURE ------------------------------


	void new_turn(vector<card> &in_play);

//TODO	void new_phase();

//TODO	void end_phase();	

//----------------- MANA POOL ----------------------------------

	void draw_all_Mana(vector<card> &in_play);
	
	int check_mana(vector<char> mana_cost, char flag); //need to update
	



	// print a given vector of cards
	void print_selection(vector<card> to_output);





//***************** MANA CALCULATIONS **************************************
 
	vector<string> compute_mana_percentages();  //adds the mana types to the source vector and orders
	vector<float> compute_source_vector(); // computes the mana/source percentage for each color
	int compute_sources();

	vector<float> mana_pool_numbers(vector<mana> mana_vector, char Flag); // flag options are 'O' for use optional, 'N' for normal
	vector<int> mana_cost_numbers(vector<char> mana_vector); 



	vector<int> compute_dif(vector<float> have, vector<int> need); // probably to be depricated
	vector<string> compute_need(); //also to be depricated

	vector<char> mana_need_trimmer(vector<string> need_vector);


//***************  LAND FUNCTIONS   *************************************

	int play_Land();

	int land_search();

	card find_land(vector<card> lands, string land_flag);

	//--------- LAND SEARCH BY TYPE --------------
	
	card find_basic(vector<card> lands, char mana_want); // flag = B or Basic_subtype

	card find_2_color_land(vector<card> lands, vector<string> mana_needs); // flag = 2

	card find_3_color_land(vector<card> lands, vector<string> mana_needs); // flag = 3

	card find_search_land(vector<card> lands); //flag = S

	card find_land_subtype(vector<card> lands, string subtype, vector<string> mana_need);
//*****************************************************


	card biggest_thing_playable();

	int remove_mana(char mana_symbol);

	int play_biggest_thing(card Big_thing);
	

	void turn_report();
//******************* MAIN GAME LOOP ****************************************

	float average_for_deck(vector<card> input);

	int game_loop(vector<card> input);

//*********************** ENDING THE GAME ********************************
	int end_check();

	int deck_size_check();

	void set_end_condition(vector<string> conditions);

};

#endif
