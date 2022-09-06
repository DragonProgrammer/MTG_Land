#define DEBUG
#ifdef DEBUG
#include <iostream>
#include <vector>
using std::cerr;
using std::endl;
using std::vector;
typedef vector< vector<float> > vec;
#define DB_FLAG vector<int>({0,12})
#define DBA(x)do{cerr << x << endl;}while(0)
#define DBVA(x) do{for(auto h : x) { cerr << h << "  ";}cerr << endl; } while(0)
//#define DB(x, level) if(DB_FLAG == level)do{cerr << x << endl;}while(0)
#define DB(x, level) do{for(auto flag_m : DB_FLAG){if(flag_m == level){cerr << x << endl;}}}while(0)
//#define DBV(x, level) if(DB_FLAG == level)do{for(auto h : x) { cerr << h << "  ";}cerr << endl; } while(0)
#define DBV(x, level) do{for(auto flag_v : DB_FLAG){if(flag_v == level){for(auto h : x) { cerr << h << "  ";}cerr << endl; } } }while(0)
#else
#define DBA(x) do{}while(0)
#define DBVA(x) do{}while(0)
//#define DB(x, level) if(DB_FLAG == level)do{}while(0)
#define DB(x, level) do{}while(0)
//#define DBV(x, level) if(DB_FLAG == level)do{ } while(0)
#define DBV(x, level) do{} while(0)
#endif

// Debug levels
//
// DBA :
// 	Average_for_deck():
// 		Anmounce game fail
// 		State number of turns the game took
// 		State how many games failed out of the number of full runs
// 		State the average for the deck
//
//	Game_loop():
//		Multiple calls to play_land()
//		Error removing cads from location
//
//
//  0 - - - - - -
//  	0.25 - Math for averages of deck - average_for_deck() - TODO check against failed runs
//  	0.5  - Creation of end condtions - set_end_conditions()
//
//  1 - - - - - - 
//  	1    - turn counter - game_loop()
//  	1.1  - initial hand - game_loop()
//  	1.25 - card draw    - draw_card()
//	
//  2 - - - - - - - MANA REPORTS	
//	2.1  - start of turn report  - game_loop()
//	2.2  - after land report     - game_loop() TODO
//	2.3  - during card play loop - game_loop() TODO
//	2.4  - end of turn report    - game_loop() TODO
//
//  3 - - - - - - - LAND PLAY
//  	3    - Playing the land      - play_Land()
//  	3.2  - No lands to play      - game_loop()
//
//  4 - - - - - - - CAST SPELL
//  	4    - Declare Cast	     - Game_loop()
//  	4.1  - Casting details       - Cast_biggest_thing() TODO
//  	4.25 - Field change in loop  - Game_loop()
//  	4.3  - pre-cast nameing      - Game_loop()
//  	4.4  - ran out of mana       - Game_loop()
