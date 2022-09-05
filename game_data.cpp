#include "Debug.h"
#include "game_data.h"
#include "card.h"
#include "effect.h"
#include "GameState.h"
#include "mana.h"

#include <vector>

using std::endl;
using std::string;
using std::vector;


Game_Data::Game_Data(int run){
	game_run = run;
	
	//reset data
	number_of_turns = 0;
	turns_no_land_was_played.clear();
	number_of_turns_without_land_draop = 0;
	mana_record.clear();
	game_play_order.clear();
	original_hand.clear();
	card_draw_order.clear();

}

void Game_Data::record_no_land(int turn){
	turns_no_land_was_played.push_back(turn);
	number_of_turns_without_land_drop++;
}

//----------------------------------------------
//THis function creates the mana report
//	The mana report contains:
//		All mana available from Single Sources
//		All Mana available from optional sources
//		The Turn That the report was made
//		The last action that was taken - AKA the reason that the pool changed
//			These Will be:
//				Start of turn
//				Played Land: <land.ID>
//				Played Spell: <Card.ID> <Card.Cost>
//				Activated Ability: <Card.ID> <Card.Effect.Text> <Card.Effect.Cost>
//				End of turn
//		The phase the report was made <-- phases not yet implemented

mana_report Game_Data::create_mana_report( vector<mana> single, vector<mana> optional, int turn, string action){
	mana_report temp;
	temp.mana_from_single_sources = single;
	temp.mana_from_optional_sources = optional;
	temp.total_available = temp.mana_from_single_sources.size + temp.mana_from_optional_sources.size();
	temp.turn_count = turn;
	temp.action = action;
	return temp;
}

string Game_Data::report_mana(mana_report to_report){
	string mana_report_string = "";
	mana_report_string = mana_report_string + "On Turn: " + to_string(to_report.turn_count) + 
		" after performing: " to_report.ation + " the is " +
		to_string(to_report.mana_from_single_sources.size() + to_report.mana_from_optional_sources.size())
		+ " mana.\n";

	mana_report_string = mana_report_string + "From single sources there is:\n";
	for( auto m : to_report.mana_from_single_sources){
		mana_report_string = mana_report_string + m.get_produced() + "   ";
	}
	mana_report_string = mana_report_string + "\n\nOptional Sources and what they can produce are:\n";
	for( auto m : to_report.mana_from_optional_sources{
		mana_report_string = mana_report_string + m.get_source + ": " + m.get_options() + "   ";
	}

	return mana_report_string;
	}



