//this is a list of TODO and where they are from
actions.h / actions.cpp
	add int i_use_snow = 0; // default zero
	void snow_check(vector<card> deck){
		check mana cost and oracle abilits for mention of snow mana
			if yes i_use_snow = 1;
	}


//--------------------------------------------------
actions::draw_all_mana()
	add check for riders:  
		...
		if(card.get_production().size() > 0){
			vector<mana> card_produced = card.get_production();
			for(auto m : card_produced){
				char check = m.is_specail();
				switch(check):
				case ' ':
					usable_mana.push_back(m);
					break;
				case 'o':
					optional_mana.push_back(m);
					break;
				case 's':
					if(i_use_snow == 1){ //detrmined at deck set up
						snow_pool.push_back(m); //new pool
					}
					else
						usable_mana.push_back(m);
					break;
				case 'r':
					rider_mana_pool.push_back(m); // new pool
			}

//------------------------------------------------------

//
mana.cpp / mana.h
	change constructor to :
		mana(char source_produced, string ID, string rider) // rider will be "" for a long while
		mana(string source_options, string ID, string rider) // rider will be "" for a long while
	//------------------
	add string Mana_riders; // this will be much later addition
	add int snow_flag = 0; // by default not snow //might need to change parce_produces to get his in,
       				//in cost snow rewuirement shall by 'S' for symbol
	add int phyrexian_flag = 0; // default zero, this atribut will be there but not used ever	
	//-------------------
	add char is_special() {
		if(mana_riders != ""){
			return 'r'; 
		}
		if (phyrexian_flag == 1){
			return 'p';
		}
		if (snow_flag == 1){ //or i could have a snow mana pool and deal with this in draw mana
			return 's';
		}
		if (options.size() > 1){
			if( options[1] == '/'){
				return 'h';
			}
			else 
				return 'o';
		}
		return ' '; //space, empty char not seem to work , test in code
	}

//
//
Card.cpp / card.h
	add int Token_flag; // for all cards at creation this is set to 0
	    int Permenent_flag;  // default is 1 (yes permenant)
	add vector<mana> mana_production = {};// default empty set;  will not work if multiple mana production options
	add vector<mana> get_production(){return mana_production;}

	in Card(...) add if (card.is_of_type("Instant") || card.is_of_type("Sorcery"){ Permenent_flag = 0;}
	change int is_of_type() to bool


	parse_cost(...)
		change to vector<mana> so I can use a new mana layout to do Hybrid mana

		struct cost{
			vector<mana> normal_cost;
			vector<mana> specail cost; //phyrexian, manariders and hybrid go here
			}

//*************************************************************
Effect.H / Effect.cpp
//
	make struct effect_numeric{
		int total_targets;
		int targets_of_each_type; // this might always be 1
	};

		/*if we assume targets_of_each_type is 1 we can go back to it being int*/
   //---------------------------
   
	make struct target_limitations{
		string super-type;
		vector<string> card_types;
		vector<string> sub_types;
		//vector(char) colors; 
		//int CMC;
		//int Power;
		//int Toughness;
		...
		}

   	vector<target_limitations> effect_limitations;
   
	effect.Target_type - change to vector <target_limitations>: //AKA effect_limitations
		"search for a land" would be vector size 1 
	
			TYPE		NUMERIC
		   {/*Vector*/       | {
		      {/*struct*/    |	 1;
		   	"";          |	 1;
			{"Land"};    |	}
			{""}         |
			{""}         |
		      }              |
		    }                |


		"Search for a basic land" would become 
	
			TYPE		NUMERIC
		   {/*Vector*/       | {
		      {/*struct*/    |	 1;
		   	"Basic";     |	 1;
			{"Land"};    |	}
			{""}         |
			{""}         |
		      }              |
		    }                |


		'Krosan Verge' "Search for a forest and a plains"
		
			TYPE		NUMERIC
		   {/*Vector*/       | {
		     {/*struct*/     |	 2;
		   	"";          |	 1;
			{"Land"};    |	}
			{"Forest"}   |
			{"G"}        |
		     };    	     |
     		     {/*struct 2*/   |
			"";          |
			{"Land"};    |
			{"Plains"}   |
			{"W"}        |
		      }		     |
		    }                |

	vector<target_limitions>  effect::get_limitations(){return effect_limitations;}


	void effect:parse_effect_target(sting target_string){};

	int effect::effect_target_is_of_type(string query}{}
		need to make a function that check effect target for a type
		mirrored mostly from card::is_of_type()

//*****************************************************
LAND FUNCTIONS
//
//
//
land_search():

	to find a search you need to have:
       		if(card.get_Effect.get_Eff_Type() == "Search" && card.get_Effect.Eff_Target_is_of_type("Land") == 1 && .... ).

	
	string search_Target to vector<effect_limitations>
		search_Target.find("Basic") != string::npos) to search_Target.effect_target_is_of_type("Basic")

find_multiple_lands(){}
	//set this function up to do cultivate, Krosen Verge and find X lands
	
	edit implemention of mana_need_vector to factor in lands in hand (of found already)


//************************************************************	
