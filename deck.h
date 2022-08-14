#ifndef DECK_H
#define DECK_H

#include <vector>
#include "card.h"

using std::vector;

struct card_listing {
	string name;
	int count;
};

class deck{

	vector<card_listing> class_deck_list;
	vector<card> class_deck;

	public:

	vector<card> get_Deck();
	void create_Deck( vector<card_listing> deck_list);
	void make_Deck_List(string deck_file);
	vector<card_listing> get_Deck_List();

	deck();
	deck(string file_name);
};
#endif
