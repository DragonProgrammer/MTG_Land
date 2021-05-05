#ifndef MANA_H
#define MANA_H
#include <string>
#include <ostream>
using std::ostream;
using std::string;
class mana{
	string options;
	char produced;
	string source;

	public:
	char get_produced();
	string get_options();
	string get_source();
	mana(string o, string from);
	void set_produced(char symbol);
	bool can_produce(char want);

	friend ostream& operator<<(ostream& os, const mana& m);

};
#endif
