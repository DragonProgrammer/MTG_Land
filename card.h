#ifndef CARD_H
#define CARD_H
#include <vector>
#include <string>
using std::string;
using std::vector;
using std::ostream;
class card{
	string ID;
	string Cost;
	char Enters;
	string Produces;
	char Mode;

	char Type;
	public:
	card();
	void set_card(string input);

	int check_Type();
	int check_Enters();
	vector<char> parse_Cost();
	vector<char> parse_Produces();// need to rework for optional ie R or G
	int get_CMC();
	char get_Type();
	string get_ID();
	string get_Produces();
	char get_Enters();
	void set_Mode(char New_Mode);
	char get_Mode();
	void print_Card();
friend ostream& operator<<(ostream &os, const card &cd);
	};
#endif
