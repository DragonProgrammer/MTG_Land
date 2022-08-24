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
//  o = land search, land play
// -1 = mana stuff
// -4 = parse text functions, card creation functions
// -5 = search effect set up, text trim, type querying
//
//  1 - function declation - in average
//  2 = checking mana cost, 
//  3 = mode checks, playing biggest thing,  removing mana for colorless,
//  5, removing colored mana, mana in game loop, lagest thing in game loop
//  7 = turn counter
//
//  12 = game loop stats
//  13 - card in hand checks
//  10 - removing cards from list
// 20 = Empty/ NO DB
//
//  7 = mana function calls
