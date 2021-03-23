#include <iostream>
#include <string>
#include "card.h"
using std::cout;
using std::endl;
using std::string;
int main() {
	cout << "test" << endl;
	card TEST;
	string test_input = "M L 0 U R";
	TEST.set_card(test_input );
	return 0;
}
