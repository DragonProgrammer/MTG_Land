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

	vector<string> mana_from_duel_sources; // you are going to be prblamatic

	card biggest_thing_in_hand;

	string mana_need_to_play_biggest;

	int Land_per_turn_flag = 0;

	int turn_counter = 0;

	public:

	string compute_need();

	card biggest_in_hand();

	vector<card> get_deck();

	int check_mana(vector<char> mana_cost, vector<char> &mana_pool, char flag);

	void set_deck(vector<card> from_main);

	vector<card> remove_card(card to_remove, vector<card> remove_from);

	void print_selection(vector<card> to_output);

	void set_initial_Hand();

	void new_turn();

	int draw_card();

	int play_Land();

	void draw_all_Mana();

	card biggest_thing_playable();

	int remove_mana(char mana_symbol, vector<char> &pool);

	int play_biggest_thing(card Big_thing);
	
	void end_check();

	void turn_report();

	int game_loop(vector<card> input);
};

#endif
