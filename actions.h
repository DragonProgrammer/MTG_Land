#ifndef ACTIONS_H
#define ACTIONS_H
#include "card.h"
#include <vector>
#include <string>
class actions{
	vector<card> hand;

	vector<card> initial_hand;

	vector<card> field;

	vector<card> deck;

	vector<char> usable_mana;

	int Land_per_turn_flag = 0;

	int turn_counter = 0;

	public:

	vector<card> get_deck();

	void set_deck(vector<card> from_main);

	vector<card> remove_card(card to_remove, vector<card> remove_from);

	void print_selection(vector<card> to_output);

	void set_initial_Hand();

	void new_turn();

	int draw_card();

	int play_Land();

	void draw_all_Mana();

	card biggest_thing_playable();

	int remove_mana(char mana_symbol);

	int play_biggest_thing(card Big_thing);
	
	void end_check();

	void turn_report();

	int check_remaining_cards();

	int game_loop(vector<card> input);
};

#endif
