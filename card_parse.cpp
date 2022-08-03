#include "Debug.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <boost/algorithm/string/trim.hpp>
#include "card.h"
#include "effect.h"

using std::string;
using json = nlohmann::json;
using std::ifstream;
using std::cout;
using std::endl;
using std::ofstream;
using std::to_string;
using std::array;
using std::vector;
using std::find;
using std::stoi;

// this is for setting up the decks
// 	it contains the name of a card and the number in the deck
struct card_listing {
	string name;
	int count;
};


vector<card_listing> make_deck_list(string deck_file){

// make the file streams	
	ifstream deck(deck_file);  // deck list
//create a  card list
//
//Variable declarations


	string deck_line;
	string card_name;
	string amount_str;

	int deck_amount;

	card_listing card_for_deck;
//	vector<card_listing> deck_list;
	
	vector<card_listing> card_list;

	while( getline( deck, deck_line ) ) {
		if(deck_line.length() < 2 ) // intput sanitation: empty line
			continue;

//input splitting
		string delimiter = ";";
		size_t split_loc = deck_line.find(delimiter);

// input sanitation: not splitable
// 	this will later have code for deck names
		if (split_loc == string::npos){ 
			cout << endl <<"Can not split: " << deck_line << endl;
			continue;
		}
		
		card_name = deck_line.substr(0, split_loc);  // extract name
		amount_str = deck_line.substr(split_loc + 1);  //extract number

		boost::algorithm::trim(card_name);  //input sanitation: trimming

		try{
		deck_amount = stoi(amount_str);
		}
		catch(const std::exception&){
			cout << amount_str << " contains non-integers" <<endl;
			continue;
		}

//make card list item
		card_for_deck.name = card_name;
		card_for_deck.count = deck_amount;

		card_list.push_back(card_for_deck); // add to decklist
	}

	return card_list;
}

vector<card> create_deck( vector<card_listing> deck_list ){

//create json stream
	ifstream ifs("AtomicCards.json");  // card database

// get the json
	json mtg = json::parse(ifs);
	json mtgcards = mtg["data"];

	vector<card> working_deck;
	card cdata;

	for(unsigned int c = 0; c < deck_list.size(); c++){

		if( !mtgcards.contains( deck_list[c].name ) ){   //input sanitation: spelling error
			cout << endl << deck_list[c].name << "  is not a card" << endl;
			continue;
		}

		json mtgcard = mtgcards[ deck_list[c].name ];
		

	//card element variable declaration

		string name;
		float mana_value;
		string cost;
		string oracle;
		string p;
		string t;
		vector<string> types;
		vector<string> super;
		vector<string> sub;

//data assigning

		if (mtgcard[0]["name"] != NULL)
			name = mtgcard[0]["name"];
		DB("Added name", -4);		
		
		mana_value = mtgcard[0]["manaValue"].get<float>();
		DB("Added CMC", -4);		
		if (!mtgcard[0]["manaCost"].is_null())
			cost = mtgcard[0]["manaCost"].get<string>();
		DB("Added cost", -4);		
		if (!mtgcard[0]["text"].is_null())
			oracle = mtgcard[0]["text"];
		DB("Added oracle", -4);		
		if (!mtgcard[0]["power"].is_null())
			p = mtgcard[0]["power"];
		DB("Added power", -4);		
		if (!mtgcard[0]["toughness"].is_null())
			t = mtgcard[0]["toughness"];
		DB("Added toughness", -4);		
		types = mtgcard[0]["types"].get<std::vector<string>>();
		super = mtgcard[0]["supertypes"].get<std::vector<string>>();
		sub = mtgcard[0]["subtypes"].get<std::vector<string>>();


//create card
		cdata.set_card(name, mana_value, cost, types, super, sub, oracle, p, t);

//add amount of cards to deck
		for(int dc = 1; dc <= deck_list[c].count; dc++){
			cdata.set_ID(dc);
			working_deck.push_back(cdata);
		}

	}

	return working_deck;
}

/**
int main() {

	string deck_file = "deck.txt";
	vector<card_listing> sample_list = make_deck_list(deck_file);
	vector<card> sample_deck = create_deck(sample_list);

	for(unsigned int c = 0; c < sample_deck.size(); c++ ){
		sample_deck[c].parse_Oracle();
		cout << sample_deck[c] << endl;
		sample_deck[c].is_of_Type("Basic Land");
	//	if(sample_deck[c].get_effect().get_Eff_Type() != "-")
	//		cout << " Card Effect is:  "  << sample_deck[c].get_effect();


	}

	return 0;
}
**/
// get card data
