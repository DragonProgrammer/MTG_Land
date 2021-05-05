#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "card.h"
#include "actions.h"

class GameState{
	public:

	vector<card> S_hand;
	vector<card> S_field;
	vector<card> S_deck;

	//not implemented yet;
	vector<card> grave;
	vector<card> exile;

	vector<mana> S_usable_mana;

	int S_turn_counter;
	int S_Land_flag;

};
#endif
