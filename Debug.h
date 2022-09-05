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
//  0 - - - - - -
//  	0.25 - Math for averages of deck - average_for_deck() - TODO check against failed runs
