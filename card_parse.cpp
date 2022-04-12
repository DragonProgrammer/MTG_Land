#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <boost/algorithm/string/trim.hpp>


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
	string deck_line;
	string card_name;
	string amount_str;

	int deck_amount;

	card_listing card;
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
	
		card.name = card_name;
		card.count = deck_amount;


		card_list.push_back(card);
	}

// get card data

	for(unsigned int c = 0; c < card_list.size(); c++){

		if( !mtgcards.contains( card_list[c].name ) ){   //input sanitation: spelling error
			cout << endl << card_list[c].name << "  is not a card" << endl;
			continue;
		}

		json mtgcard = mtgcards[ card_list[c].name ];
//print out the data		
		cout <<  endl <<  "Name:  "  << mtgcard[0]["name"] << endl;
	
		cout << endl << "CMC:     " << mtgcard[0]["manaValue"] <<endl;
		cout << "Mana Cost:     "   << mtgcard[0]["manaCost"] <<endl;

		vector<string> types = mtgcard[0]["types"].get<std::vector<string>>();
		cout << "Types:   ";
		for(unsigned int t = 0; t < types.size(); t++)
			cout << types[t] << "     ";
		cout << endl;
		cout << "Text:   "  << mtgcard[0]["text"] << endl;
		cout << endl << "Power;    " << mtgcard[0]["power"] << endl;
		cout << "Toughness:    " << mtgcard[0]["toughness"] << endl;

		cout << "Number in deck: " << card_list[c].count << endl;

	}
	return 0;
}
