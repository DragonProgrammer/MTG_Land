// an object to record when a card was played and with what mana
// ------------------------------------------
// TO ADD as implemented
//	phase_played
//	resolved_?  -- for when counter splells are implementred
//	target --  this might need to be in a struct of ists own to draw from card.Effect data

struct card_play{
	card card_played;
	vector <mana> used_mana;
	int turn_payed;
};


struct mana_report{
	vector <mana> mana_from_single_sources;
	vector <mana> mana_from_optional_sources;

	int total_available;
	int turn_count;
	
	string action;
	// string phase;   phases not yet implemented
};


//A structucture for tracking activated abilies
//	this might need to be folded into gamestate
//	---
//	it would be nice to have 


class Game_Data{

	
	int game_run; // Game idetification
	int number_of_turns;  // For averaging deck
	
	// Track Land effectiveness
	// 	for evaluation of the effect of land count changes
	
	vector<int> turns_no_land_was_played; //what turns did i not play lands
	int number_of_turns_without_land_drop;

	//functions on deck_data will give me output for how many lands are in the deck, the percentages for needed mana by color and the percentages of producability by color
	
	vector<mana_report> mana_record;


	vector <card_play> game_play_order;

	vector <card> original_hand;
	vector <card> card_draw_order;	
	

	//END STATE DATA

	vector<card> end_field;
	vector<card> end_deck;
	vector<card> end_grave;
	vector<card> end_hand;


	public:
	Game_Data(int run);

	void recornd_no_land(int turn);

	mana_report create_mana_report( vector<mana> single, vector<mana> optional, int turn, string action);

	string report_mana(mana_report to_report);
};
