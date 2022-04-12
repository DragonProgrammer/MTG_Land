#define DEBUG
#ifdef DEBUG
#include <iostream>
#include <vector>
using std::cerr;
using std::endl;
using std::vector;
typedef vector< vector<float> > v;
#define DB_FLAG -3
#define DB(x, level) if(DB_FLAG > level)do{cerr << x << endl;}while(0)
#define DBV(x, level) if(DB_FLAG > level)do{for(auto h : x) { cerr << h << "  ";}cerr << endl; } while(0)
#define DBM(v, level) if(DB_FLAG> level)do{for(int r =0; r < 4; r++){ for(int c = 0; c <4; c++) cerr << "point " << r << " " << c << " " << v[r][c] << "     "; cerr << endl;} } while(0)
#else
#define DB(x, level) if(DB_FLAG > level)do{}while(0)
#define DBM(v, level) if(DB_FLAG > level)do{}while(0)
#define DBV(x, level) if(DB_FLAG > level)do{ } while(0)
#endif
