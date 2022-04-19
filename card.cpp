#include "Debug.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp> // for split function
#include "card.h"
#include "mana.h"
#include "effect.h"
using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::to_string;
using std::vector;
using std::find;

card::card() {}

void card::set_card(string name, int mana_value, string cost, vector<string> card_types, vector<string> card_super_types, vector<string> card_sub_types, string oracle, string p, string t){
	ID = name;
	CMC = mana_value;
	Cost = cost;
	Super_Type = card_super_types;
	Sub_Type = card_sub_types;
	Type = card_types;
	Oracle_text = oracle;
	Power =p;
	Toughness = t;
	Mode = 'U';
	Enters = 'U';
	Produces = "-";
	Card_effect = effect("-","-","-",0,"-","-","-");

	//put in call to parser functions here for 
	//	Enters and Mode
	//	Produces
} 


//---------------------------------------------
//create a vector of mana cost from the cards Cost variable
//	does not work for hybrid mana costs
//	** UPDATED FOR NEW FORMAT **
//--------------------------------------------------------
vector<char> card::parse_Cost() {
	vector<char> costs;
	unsigned int spot = 0;
	while (spot < Cost.length()) {
		if(Cost[spot] != '{' && Cost[spot] != '}') //bypass the symbol seperaters
		costs.push_back(Cost[spot]);
		spot++;
	}
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




void card::set_ID(int num){
	if(ID.find('_') != string::npos){
		int identifier = ID.find('_');
		ID = ID.substr(0,identifier);
	}
	ID = ID + "_" + to_string(num);
}

void card::set_Mode(char New_Mode) { Mode = New_Mode; }
//******************************************
// Data returning functions
// ******************************************


string card::get_ID() { return ID; }
string card::get_Produces() { return Produces; }
char card::get_Enters() { return Enters; }
int card::get_CMC() { return CMC; }  //changed

char card::get_Mode() { return Mode; }

vector<string> card::get_Type() {return Type;} //changed
vector<string> card::get_Super_Type() {return Super_Type;} 
vector<string> card::get_Sub_Type() {return Sub_Type;} 

//----------------------------------------
//gets the card name from the ID string
//----------------------------------------
string card::get_Name(){
	string Name;
	if(ID.find('_') != string::npos){
		int identifier = ID.find('_');
		Name = ID.substr(0,identifier);
	}
	return Name;
}
	

effect card::get_effect(){return Card_effect;}


//************************************************************
//Parsing functions
//******************************************************************

void card::parse_Oracle(){
	parse_text_enters();
	trim_oracle();
	parse_text_produces();
	trim_oracle();
	parse_text_land_search();
	trim_oracle();
}

//-------------------------------------------------------------------
//THis function parses the Oracle for land search
//	it curently only does "rampant growth" and "evolving wilds"
//-------------------------------------------------------------------	
void card::parse_text_land_search(){
	string effect_string = "Search your library for a basic land card, put it onto the battlefield tapped, then shuffle.";
	string cost_line = "{T}, Sacrifice " + get_Name() + ":";

	int search_flag = 0;
	
//declaration of variable to mach effect class
	string E_Type = "-";
	string E_Source;
	string E_Targets;
	int E_Numeric;
	string E_Endpoint;
	string E_State;
	string E_Cost;	


	size_t effect_start = Oracle_text.find(effect_string);  // needs to be of size_t in order to have npos
	
	DB("Oracle Text for " + ID + "      " + Oracle_text, -5);

	if( effect_start != string::npos){
		DB("In Search effect set-up", -5);
		DB("", -5);
		E_Type = "Search";
       		E_Source = "Library";
		E_Targets = "Basic Land";
		E_Numeric = 1;
		E_Endpoint = "Field";
		E_State = "Tapped";
		search_flag = 1;
		DB("After declaration the recoreded state is: " + E_State, -4);  // had + not = as declaration
	}

	unsigned int cost_len = cost_line.length();
	if( size_t cost_start = Oracle_text.find( cost_line) != string::npos  && E_Type != "-"){
		DB("Cost start: " + to_string(cost_start), -4);
		DB("Comparison COst start: " + to_string(effect_start - cost_len),-4);
		if (cost_start == (effect_start-cost_len) ){
			E_Cost = "Tap Sacrifice This";
			DB("Same effect", -4);
		}
		//TODO add in mana cost check
	}
	DB("the recoreded state is: " + E_State, -4);
	if( search_flag == 1 )
		Card_effect = effect(E_Cost, E_Type, E_Source, E_Numeric, E_Targets, E_Endpoint, E_State);
	else
		Card_effect = effect("-","-","",0,"","","");
	
}




//-----------------------------------------------
//trimes the new line char from the begining of edited oracle text
//	als trims reminder text braccets if nothing between
//-------------------------------------------------------------
void card::trim_oracle(){
	if(Oracle_text[0] == '\n'){
		Oracle_text= Oracle_text.substr(1);
		DB("Trimmed text: " + Oracle_text, -4);
	}
	if(int reminder = Oracle_text.find("()") != string::npos){
		Oracle_text.replace(reminder-1, 2, "");
		DB("Trimmed text: " + Oracle_text, -4);
	}
		
}


//--------------------------------------------------
//checks the card to see if it should enter tapped
//	searches the cards text for "card_name enters the battlefield tapped"
//
//	does not work with conditional enters TODO
//-----------------------------------------------
void card::parse_text_enters(){
	string to_find = get_Name() + " enters the battlefield tapped.";
	DB(to_find, -4);
	if (Oracle_text.find(to_find) != string::npos){
		Enters = 'T';
		DB("Enters tapped", -4);
		int len = to_find.length();
		DB(to_find + "is char in len: " + to_string(len), -4);
		int start = Oracle_text.find(to_find);
		DB("Old Oracle text: " + Oracle_text,-4);
		Oracle_text.replace(start, len, "");
		DB("New Oracle text: " + Oracle_text, -4);
		//TODO add function that removes text from oracle
	}
}

//-------------------------------------------------------------
//checks card for mana producing abilities
//	search the card text for "T: Add"
//	then looks for the position of next period
//	the text between is the card's Produces string
//------------------------------------------------------------
void card::parse_text_produces(){
	string to_find = "{T}: Add ";
	DB(to_find, -2);
	if (Oracle_text.find(to_find) != string::npos){
		int ability_start = Oracle_text.find(to_find);
		int fixxer_check = ability_start-1;
		//check for "is first thing", or if the char before it is new line or indicatoer of reminder text
		if (ability_start == 0 || Oracle_text[fixxer_check] == '\n' || Oracle_text[fixxer_check] == '(' ){
			DB("in produces check", -4);
			string sub_Oracle = Oracle_text.substr(ability_start);
			DB(sub_Oracle,-4);
			int period_at = sub_Oracle.find(".");
			DB("Period found: " + to_string(period_at), -4);
			string mana_string = sub_Oracle.substr(9,period_at-9);
			Produces = mana_string;
			DB(mana_string, -4);
// remove string from oracle
			int len = 9 + mana_string.length() + 1;
			DB("Old Oracle text: " + Oracle_text,-4);
			Oracle_text.replace(ability_start, len, "");
			DB("New Oracle text: " + Oracle_text,-4);
		

		}
		else
			DB("Fixxer found", -4);
	}
}

//*************************************************************
//Comparison Functions
//	These functions see if a card maches a given mechanic
//		is_of_type
//		is_castable_CMC_based //TODO
//************************************************************
//
//
int card::is_of_Type(string query){
	vector<string> query_types;
       	boost::split(query_types, query, boost::is_any_of(" "));
	DB("Query types Vector:", -5);
	DBV(query_types, -5);

	unsigned int full_query_flag = 0;

	for(auto t : query_types){
		
		if( find(Super_Type.begin(), Super_Type.end(), t)  != Super_Type.end() ){
			full_query_flag++;
			continue;
		}
		if( find(Type.begin(), Type.end(), t)  != Type.end() ){
			full_query_flag++;
			continue;
		}
		if( find(Sub_Type.begin(), Sub_Type.end(), t)  != Sub_Type.end() )
			full_query_flag++;
	}
	if (full_query_flag == query_types.size()){
		DB( ID + " is a " + query, -5);
		return 1;
	}
	DB( ID + " is not a " + query, -5);
	return 0;
}

	       			       






//*************************************************
//Data output functions
//	these print card data
//******************************************************i


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
