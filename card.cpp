
#ifndef DEBUG
#define DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#define DB_FLAG 0
#define DB(x, level)        \
	if (DB_FLAG > level) do { \
			cerr << x << endl;    \
	} while (0)
#else
#define DB(x, level)        \
	if (DB_FLAG > level) do { \
	} while (0)
#endif

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

int card::check_Enters() {
	if (Enters != 'U' && Enters != 'T') {
		//  DB("Wrong Value in Enters   " << Enters);
		return -1;
	}
	return 0;
}

vector<char> card::parse_Cost() {
	vector<char> costs;
	int spot = 0;
	while (spot < Cost.length()) {
		costs.push_back(Cost[spot]);
		spot++;
	}
	return costs;
}

//string card::get_ID(){return ID;}

vector<mana> card::parse_Produces() {
	vector<mana> from_source;
	int spot = 0;
	if(Produces.find('_')>-1){
		mana produced(Produces, ID);
		from_source.push_back(produced);
		return from_source;
	}


	while (spot < Produces.length()) {
		mana produced(to_string(Produces[spot]), ID);
		
		from_source.push_back(produced);
		spot++;
	}
		return from_source;
}

char card::get_ECost() { return effect_cost; }
string card::get_Effect() { return effect; }
int card::get_CMC() { return Cost.length(); }
char card::get_Type() { return Type; }
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
	cout << ID + " " + Type + " ";
	vector<char> cost = parse_Cost();
	for (auto a : cost) {
		cout << to_string(a);
	}
	cout << " " + Cost + "     ";
}

ostream& operator<<(ostream& os, const card& cd) {
	return (os << cd.ID + " " + cd.Type + " " + cd.Cost + " " + cd.effect_cost +
	                  " " + cd.effect + " " + cd.Mode + "    ");
}
