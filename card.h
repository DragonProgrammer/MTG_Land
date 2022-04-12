#ifndef CARD_H
#define CARD_H
#include <vector>
#include <string>
#include "mana.h"
using std::string;
using std::vector;
using std::ostream;
class card{
	string ID;
	string Cost;
	char Enters;

	string effect;
	char Mode;

//-----------------------------------------------------------
//     NEW OR CHANGED VARIABLES
// changing card format to make later coding easier and reflect needs of new creation method
// ___________________________________________________________

	string Oracle_text;
	vector<string> Type;
	int CMC;
	string Power;        // will need a function that makes these ints, in string type to deale with */*, */n, n/*
	string Toughness;


	string Produces; // options are '-' for none, 'S_S' for optional, or 'S' for non optional (S is catch all for mana symbols)
	//CHANGES this will be the string from the oracle text that for the mana ability

//	char Type; // depricated

//----------------------------------------------------------------

	public:
	card();




//-----------------------------------------------------
//      NEW FUNCTIONS
// changeing card creation method from string to JSON extraction changes how cards are made
//___________________________________________________________________________


	void set_card(string name, int mana_value, string cost, vector<string> card_types, string oracle, string p, string t);
	void set_ID(int num);  // this will add the card number to the end to diferenciate cards in deck
	
	vector<string> get_Type();

//---------------------------------------------------

//----------------------------------------------------
//   CHANGED FUNCTIONS
//-------------------------------------------------------

	int check_Type();
	int check_Enters();
	vector<char> parse_Cost();
//	vector<mana> parse_Produces();// need to rework for optional ie R or G
	void print_Card();
friend ostream& operator<<(ostream &os, const card &cd);

//----------------------------------------------
//   DEPRICATED FUNCIONS
//--------------------------------------------------

//	void set_card(string input); // depricated
//	string get_Effect();
//	char get_ECost();
//	char get_Type();
//---------------------------------------------------

//--------------------------------------------------------
//   UNCHANGED
//--------------------------------------------------------


	int get_CMC();
	string get_ID();
	string get_Produces();
	char get_Enters();
	void set_Mode(char New_Mode);
	char get_Mode();
	};
#endif
