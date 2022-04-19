#include <string>
#include "effect.h"

using std::to_string;

effect::effect() {};

effect::effect( string Cost, string Type, string Source, int Numeric, string Target, string Endpoint, string State){
	Eff_Cost = Cost;
	Eff_Type = Type;
	Eff_Source = Source;
	Eff_Endpoint = Endpoint;
	Eff_State = State;
	Eff_Target.Num_targets = Numeric;
	Eff_Target.Target_Type = Target;
}

//*****************************************
//Getters
//******************************************


string effect::get_Eff_Cost(){ return Eff_Cost;}
string effect::get_Eff_Type(){ return Eff_Type;}
string effect::get_Eff_Source(){ return Eff_Source;}
string effect::get_Eff_Target_Type(){ return Eff_Target.Target_Type;}
string effect::get_Eff_Endpoint(){ return Eff_Endpoint; }
string effect::get_Eff_State(){ return Eff_State;}
int effect::get_Eff_Target_Numeric(){ return Eff_Target.Num_targets; }

//***********************************************************





ostream& operator<<(ostream& os, const effect& ef){
	return( os << "Cost: " + ef.Eff_Cost + "\nType: " + ef.Eff_Type + "\nSource: " + ef.Eff_Source +
			"\nTarget: " + ef.Eff_Target.Target_Type + "   " + to_string(ef.Eff_Target.Num_targets) +
			"\nEndpoint: " + ef.Eff_Endpoint + "  State:  " + ef.Eff_State +"\n");
}

