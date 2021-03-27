#include <iostream>
#include <string>
#include "card.h"
#include <fstream>
#include "actions.h"
using std::fstream;
using std::cout;
using std::endl;
using std::string;
int main(int argc, char *argv[]) {
	fstream deck_input;
	deck_input.open(argv[1]);
	//cout << "test" << endl;
	//card TEST;
	vector<card> deck_from_file;
	string card_line;
	while(getline(deck_input,card_line)){
		card new_card;
		new_card.set_card(card_line);
		deck_from_file.push_back(new_card);
	}
//	for(auto card : deck_from_file){
//		card.print_Card();
//	}


	actions Game_run;
	Game_run.game_loop(deck_from_file);
//	vector<card> shuffled = Game_run.get_deck();
//	cout << shuffled.size() <<endl;
//	for(auto card : shuffled){
//		card.print_Card();
//	}
//	TEST.set_card(test_input );
	return 0;
}
