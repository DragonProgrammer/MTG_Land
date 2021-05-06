#include "mana.h"
#include <string>
#include <ostream>

//TODO see if adding a cost condion in is appropriete, i.e. color fixing lands
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
//		if(produced == 'A')  // this presents a problem with color fixing lands verse rupture spire lands
//			return true;
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



