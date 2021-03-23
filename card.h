#ifndef CARD_H
#define CARD_H
#include <vector>
#include <string>
using std::string;
using std::vector;
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
	vector<string> parse_Produces();
	char get_Type();
	void set_Mode(char New_Mode);
	char get_Mode();
	string print_Card();

	};
#endif
