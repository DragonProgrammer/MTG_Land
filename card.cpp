#include "Debug.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "card.h"
#include "mana.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;

card::card() {}

void card::set_card(string name, int mana_value, string cost, vector<string> card_types, string oracle, string p, string t){
	ID = name;
	CMC = mana_value;
	Cost = cost;
	Type = card_types;
	Oracle_text = oracle;
	Power =p;
	Toughness = t;
	Mode = 'U';
	Enters = 'U';
	Produces = "-";

	//put in call to parser functions here for 
	//	Enters and Mode
	//	Produces
} 


/**
void card::set_card(string input) {
	//		string input = "M L 0 U R";
	vector<string> parts;
	stringstream parse(input);
	string partial;
	while (getline(parse, partial, ' ')) {
		for(auto & c: partial) c = toupper(c);
		parts.push_back(partial);
	}
	ID = parts[0];
	if (parts[1].size() > 1) {
		//			DB("Type biger then char");
		return;
	}
	Type = toupper(parts[1].at(0));
	if (check_Type() != 0) return;
	Cost = parts[2];
	if (parts[3].size() > 1) {
		//			DB("Enters biger then char");
		return;
	}
	Enters = toupper(parts[3].at(0));
	if (check_Enters() != 0) return;

	Produces = parts[4];
	if (parts.size() < 7) {
		DB("Did not get all parts", 0);
		exit(0);
	}
	if (parts[5].length() < 1) {
		DB("Did not read in effect cost", 0);
		exit(0);
	}
	effect_cost = toupper(parts[5].at(0));
	effect = parts[6];
	Mode = 'N';  // for none
	             //	DB(print_Card());
}

int card::check_Type() {
	if (Type == 'L' || Type == 'P' || Type == 'S') {
		return 0;
	}
	//	DB("Wrong Value in Type   " << Type);
	// debug message
	return -1;
}
*/


int card::check_Enters() {
	if (Enters != 'U' && Enters != 'T') {
		//  DB("Wrong Value in Enters   " << Enters);
		return -1;
	}
	return 0;
}

//TODO  this needs to take into acount that cost is now "{symbol}{symbol}..."


vector<char> card::parse_Cost() {
	vector<char> costs;
//	int spot = 0;
//	while (spot < Cost.length()) {
//		costs.push_back(Cost[spot]);
//		spot++;
//	}
	return costs;
}


//TODO  rework based on oracle text
//TODO figure out why make is giving a "undefined referance to mana::mana(char ..."
// commented out due compiler error and not used at this poitn


/**

vector<mana> card::parse_Produces() {
	vector<mana> from_source;
	unsigned int spot = 0;
//	if(Produces.find('_')>-1){
//		mana produced(Produces, ID);
//		from_source.push_back(produced);
//		return from_source;
//	}


	while (spot < Produces.length()) {
		mana produced(Produces[spot], ID);
		DB("mana made " << produced.get_produced(), -1);	
		from_source.push_back(produced);
		spot++;
	}
		return from_source;
}

**/


//char card::get_ECost() { return effect_cost; }
//string card::get_Effect() { return effect; }

int card::get_CMC() { return CMC; }  //changed

//char card::get_Type() { return Type; }

vector<string> card::get_Type() {return Type;} //changed

void card::set_ID(int num){
	if(ID.find('_') != string::npos){
		int identifier = ID.find('_');
		ID = ID.substr(0,identifier);
	}
	ID = ID + "_" + to_string(num);
}

string card::get_ID() { return ID; }
string card::get_Produces() { return Produces; }
char card::get_Enters() { return Enters; }
void card::set_Mode(char New_Mode) { Mode = New_Mode; }

char card::get_Mode() { return Mode; }
void card::print_Card() {
	if (ID.length() < 1) {
		cout << "ID not there       ";
		return;
	}
	cout << ID + " " + Mode + "    ";
}

ostream& operator<<(ostream& os, const card& cd) {

	string type_line;
	for(unsigned int t = 0; t < cd.Type.size(); t++){
		type_line = type_line + cd.Type[t];
		if( t+1 != cd.Type.size() )
			type_line = type_line + " ";
	}
	if(cd.Power == "")
		return (os << cd.ID + " " + to_string(cd.CMC) + " " + cd.Cost + " " + type_line + " " + cd.Oracle_text + "        ");
	else
		return (os << cd.ID + " " + to_string(cd.CMC) + " " + cd.Cost + " " + type_line + " " + cd.Oracle_text + " " + cd.Power + "/" + cd.Toughness  +  "        ");
}
