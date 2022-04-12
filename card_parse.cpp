#include "Debug.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <boost/algorithm/string/trim.hpp>
#include "card.h"

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


int main() {

// make the file streams	
	ifstream deck("deck.txt");  // deck list
	ifstream ifs("AtomicCards.json");  // card database
	ofstream list("list.txt");  // output log


// get the json
	json mtg = json::parse(ifs);
	json mtgcards = mtg["data"];

//create a  card list
//
//Variable declarations

	card cdata;

	string deck_line;
	string card_name;
	string amount_str;

	int deck_amount;

	card_listing card_for_deck;
//	vector<card_listing> deck_list;
	
	vector<card_listing> card_list;
	vector<card> deck_test;

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

		cout << "card name = " << card_name << "; Amount = " << amount_str << endl;

	
		boost::algorithm::trim(card_name);

//		cout << "trim test: |" << card_name <<"|"<<endl;
		try{
		deck_amount = stoi(amount_str);
		}
		catch(const std::exception&){
			cout << amount_str << " contains non-integers" <<endl;
			continue;
		}

//		cout << "Amount check: " << deck_amount << endl << endl;
	
		card_for_deck.name = card_name;
		card_for_deck.count = deck_amount;


		card_list.push_back(card_for_deck);
	}

// get card data


	for(unsigned int c = 0; c < card_list.size(); c++){

		if( !mtgcards.contains( card_list[c].name ) ){   //input sanitation: spelling error
			cout << endl << card_list[c].name << "  is not a card" << endl;
			continue;
		}

		json mtgcard = mtgcards[ card_list[c].name ];
//print out the data
		

	//card element variable declaration

		string name;
		float mana_value;
		string cost;
		string oracle;
		string p;
		string t;
		vector<string> types;

		cout <<  endl <<  "Name:  "  << mtgcard[0]["name"] << endl;



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



		cout << endl << "CMC:     " << mtgcard[0]["manaValue"] <<endl;
		cout << "Mana Cost:     "   << mtgcard[0]["manaCost"] <<endl;

		types = mtgcard[0]["types"].get<std::vector<string>>();
	//	cout << "Types:   ";
	//	for(unsigned int t = 0; t < types.size(); t++)
	//		cout << types[t] << "     ";
	//	cout << endl;
		cout << "Text:   "  << mtgcard[0]["text"] << endl;
	//	cout << endl << "Power;    " << mtgcard[0]["power"] << endl;
	//	cout << "Toughness:    " << mtgcard[0]["toughness"] << endl;

		DB("recorded CMC: " +to_string(mana_value), -5);
		DB("recorded cost: " + cost, -5);

		int mana_to_int = int(mana_value);

		cdata.set_card(name, mana_to_int, cost, types, oracle, p, t);

	//	cout << cdata;

		for(int dc = 1; dc <= card_list[c].count; dc++){
			cdata.set_ID(dc);
			deck_test.push_back(cdata);
		}


		cout << "Number in deck: " << card_list[c].count << endl;

	}

	for(unsigned int d = 0; d < deck_test.size(); d++)
		cout << deck_test[d] << endl;
	return 0;
}
