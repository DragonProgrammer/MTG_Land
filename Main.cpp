#include <fstream>
#include <iostream>
#include <string>
#include "actions.h"
#include "card.h"
using std::cout;
using std::endl;
using std::fstream;
using std::string;
using std::to_string;
int main(int argc, char* argv[]) {
	fstream deck_input;
	deck_input.open(argv[1]);
	 cout << "test" << endl;
	// card TEST;
	vector<card> deck_from_file;
	string card_line;
	while (getline(deck_input, card_line)) {
		card new_card;
		new_card.set_card(card_line);
		deck_from_file.push_back(new_card);
	}
		for(auto card : deck_from_file){
//			card.print_Card();
		}

	actions Game_run;
	 cout << "test" << endl;
	float average = Game_run.average_for_deck(deck_from_file);
	 cout << "test" << endl;
	cout << "The average for the deck is " << to_string(average) << endl;
	return 0;
}
