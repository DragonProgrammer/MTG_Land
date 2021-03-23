#ifndef ACTION_H
#define ACTION_H
#include "card.h"
#include <vector>

class actions{
	vector<card> hand;

	vector<string> field;

	vector<card> deck;

	vector<char> usable_mana;

	int Land_per_turn_flag = 0;

	int turn_counter = 0;

	public:

	void set_deck(vector<card> from_main);

	void set_initial_Hand();

	void new_turn();

	int draw_card();

	int play_Land();

	void draw_all_Mana();

	card biggest_thing_playable();

	int play_biggest_thing(card Big_thing);

	void turn_report();

	int check_remaining_cards();

	int game_loop();
};

#endif
