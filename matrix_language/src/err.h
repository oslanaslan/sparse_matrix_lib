#include "standartlibs.h"

#ifndef ERRORS

class Err {
public:
	uint64_t pos;
	string str;
	Err(): pos(0), str("") {}
};

#define ERRORS
#endif
