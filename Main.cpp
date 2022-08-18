#include <fstream>
#include <iostream>
#include <string>
#include "actions.h"
#include "card.h"
#include "deck.h"

using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::to_string;
int main(int argc, char* argv[]) {
	
	if( argc < 2){ cout << "need file" << endl; }

	 cout << "test" << endl;
	// card TEST;
	vector<card> deck_from_file;
	
	deck trial_deck;

	trial_deck = deck(argv[1]);

	deck_from_file = trial_deck.get_Deck();

	actions Game_run;
	 cout << "test" << endl;
	float average = Game_run.average_for_deck(deck_from_file);
	 cout << "test" << endl;
	 if(average == 0)
		 cout << "All games failed";
	 else
		 cout << "The average for the deck is " << to_string(average) << endl;
	return 0;
}
