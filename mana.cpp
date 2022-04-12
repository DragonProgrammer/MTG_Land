#include "Debug.h"
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

	mana::mana(string source_options, string from){
		options = source_options;
		source = from;
		DB("options size " << source_options.length(), -1);
		DB("Options check " << options, -1);
		if(source_options.length() == 1)
			produced = source_options[0];
		else
			produced = '0';
	}
	
	mana::mana(char source_produced, string from){
		options = source_produced;
		source = from;
		DB("Options check " << options, -1);
		produced = source_produced;
	}

	void mana::set_produced(char symbol){
		produced = symbol;
	}

	bool mana::can_produce(char want){
//		if(produced == 'A')  // this presents a problem with color fixing lands verse rupture spire lands
//			return true;
		if(produced == want)
			return true;
		if(options.find(want) != string::npos)
			return true;
		return false;
	}

	ostream& operator<<(ostream& os, const mana& m){
		if(options.length() == 1)
			os << m.produced << " by " << m.source;
		else
			os << m.produced << " by " << m.source << " that produces " << m.options;
			
		return os;
	}



