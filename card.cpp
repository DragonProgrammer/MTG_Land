
#ifndef DEBUG
#define DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#define DB(x)          \
  do {                 \
    cerr << x << endl; \
  } while (0)
#else
#define DB(x) \
  do {        \
  } while (0)
#endif
//#define D_FLAG 1

#include <iostream>
#include <vector>
#include "card.h"
#include <sstream>
using std::cout;
using std::string;
using std::vector;
using std::stringstream;
using std::to_string;



card::card() {}
void card::set_card(string input){

//		string input = "M L 0 U R";
		vector<string> parts;
		stringstream parse(input);
		string partial;
		while (getline(parse, partial, ' ')) {
			parts.push_back(partial);
		}
		ID = parts[0];
		if(parts[1].size() > 1){
			DB("Type biger then char");
			return;
		}
		Type = toupper(parts[1].at(0)); 
		if (check_Type() != 0) return;
		Cost = parts[2];
		if(parts[3].size() > 1){
			DB("Enters biger then char");
			return;
		}
		Enters = toupper(parts[3].at(0)); 	
		if (check_Enters() != 0) return;
		Produces = parts[4];
		Mode = 'N';  // for none
		DB(print_Card());
}


int card::check_Type() {
	
	if (Type == 'L' || Type == 'P' || Type == 'S') {
 		 return 0;
	}
  	DB("Wrong Value in Type   " << Type); 
    // debug message
    return -1;
  }

int card::check_Enters(){
if (Enters != 'U' && Enters != 'T') {
  DB("Wrong Value in Enters   " << Enters);
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

vector<string> card::parse_Produces() {
  vector<string> mana;
  stringstream parse(Produces);
  string partial;
  while (getline(parse, partial, '_')) {
    mana.push_back(partial);
    // debug spot
  }
  return mana;
}

char card::get_Type() { return Type; }
void card::set_Mode(char New_Mode) { Mode = New_Mode; }
char card::get_Mode() { return Mode; }
string card::print_Card() { return ID + " " + Type + " " + Cost; }

