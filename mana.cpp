#include "mana.h"
#include <string>
#include <ostream>


	string options;
	char produced;
	string source;

	char mana::get_produced(){ return produced;}
	string mana::get_options(){ return options;}
	string mana::get_source(){ return source;}

	mana::mana(string o, string from){
		options = o;
		source = from;
		if(o.length() == 1)
			produced = o[0];
		else
			produced = '0';
	}

	void mana::set_produced(char symbol){
		produced = symbol;
	}

	bool mana::can_produce(char want){
		if(produced == want)
			return true;
		if(options.find(want) > -1)
			return true;
		return false;
	}

	ostream& operator<<(ostream& os, const mana& m){
		if(options.length() == 1)
			os << produced << " by " << source;
		else
			os << produced << " by " << source << " that produces " << options;
			
		return os;
	}



