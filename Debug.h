#define DEBUG
#ifdef DEBUG
#include <iostream>
#include <vector>
using std::cerr;
using std::endl;
using std::vector;
typedef vector< vector<float> > vec;
#define DB_FLAG -4
#define DB(x, level) if(DB_FLAG > level)do{cerr << x << endl;}while(0)
#define DBV(x, level) if(DB_FLAG > level)do{for(auto h : x) { cerr << h << "  ";}cerr << endl; } while(0)
#define DBM(vec, level) if(DB_FLAG> level)do{for(int r =0; r < 4; r++){ for(int c = 0; c <4; c++) cerr << "point " << r << " " << c << " " << vec[r][c] << "     "; cerr << endl;} } while(0)
#else
#define DB(x, level) if(DB_FLAG > level)do{}while(0)
#define DBM(vec, level) if(DB_FLAG > level)do{}while(0)
#define DBV(x, level) if(DB_FLAG > level)do{ } while(0)
#endif
