#include "standartlibs.h"

#ifndef LEXEM
class Lexem {
public:
	int type;
	string str;
	Lexem(): type(0), str("") {}
	int get_type() { return type; }
	string get_str() { return str; }
	void set_type(int a) { type = a; }
	void set_str(string a) { str = a; }
};

#define LEXEM
#endif
