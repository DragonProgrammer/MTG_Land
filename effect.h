#ifndef EFFECT_H
#define EFFECT_H

#include <string>
#include <vector>
using std::string;
using std::vector;
using std::ostream;

struct Target_Info{
	int Num_targets;
//	int has_variable_numeric // for X targets wher e X is determined by some factor
	string Target_Type;
};

class effect{
	private:
	string Eff_Cost; //this will be anying before the :

	string Eff_Type; // This will be a general what is the effect, Currently only implementing "Search"

	string Eff_Source; // where do i look first, or where is the stuff coming from
		// "Library" for "draw" and "search" effect types
		// "Grave" for "reanimation" effect types
		// "Hand" for "cheat into play" effect types
		// "This" for everything else
	
	Target_Info Eff_Target; // what is it effecting effecting
		// includes numer (how many targets) and card type/subtype/suppertype data
		//this may end up a vector for multiple types of targets (i.e. shard convergence)

	string Eff_Endpoint; //where targets from source go, or if source is "This" where targets are

	string Eff_State; // What changes are applied to the target, only relevent to things left on field

	public:

	effect();

	effect(string Cost, string Type, string Source, int Numeric, string Target, string Endpoint, string State);

//***************************************************
//GETTERS
//******************************************************

	string get_Eff_Cost();
	string get_Eff_Type();
	string get_Eff_Source();
	int get_Eff_Target_Numeric();
	string get_Eff_Target_Type();
	string get_Eff_Endpoint();
	string get_Eff_State();

//*******************************************************

friend ostream& operator<<(ostream &os, const effect &ef);

};
#endif
		
