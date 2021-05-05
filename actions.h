#ifndef ACTIONS_H
#define ACTIONS_H
#include "card.h"
#include <vector>
#include <string>
#include "mana.h"
#include "GameState.h"
class actions{
	vector<card> hand;

	vector<card> initial_hand;

	vector<card> field;

	vector<card> deck;

	vector<mana> usable_mana;

	vector<mana> mana_from_optional_sources; // you are going to be prblamatic

	vector<GameState> state;



	card biggest_thing_in_hand;

	vector<string> mana_need_to_play_biggest;

	int Land_per_turn_flag = 0;

	int turn_counter = 0;

	public:

	GameState get_state();

	void set_state();



	vector<string> compute_mana_percentages();  //adds the mana types to the source vector and orders
	vector<float> compute_source_vector(); // computes the mana/source percentage for each color
	int compute_sources();

	vector<float> mana_pool_numbers(vector<mana> mana_vector, char Flag); // flag options are 'O' for use optional, 'N' for normal
	vector<int> mana_cost_numbers(vector<char> mana_vector); 



	vector<int> compute_dif(vector<float> have, vector<int> need); // probably to be depricated
	vector<string> compute_need(); //also to be depricated

	card biggest_in_hand();

	vector<card> get_deck();

	int check_mana(vector<char> mana_cost, char flag); //need to update

	void set_deck(vector<card> from_main);

	int remove_card(card to_remove, vector<card> &remove_from);

	void print_selection(vector<card> to_output);

	void set_initial_Hand();

	void new_turn(vector<card> &in_play);

	int draw_card();

	int play_Land();

	void draw_all_Mana(vector<card> &in_play);

	int land_search();

	card find_land(vector<card> lands);

	float average_for_deck(vector<card> input);

	card biggest_thing_playable();

	int remove_mana(char mana_symbol);

	int play_biggest_thing(card Big_thing);
	
	int end_check();

	void turn_report();

	int game_loop(vector<card> input);
};

#endif
