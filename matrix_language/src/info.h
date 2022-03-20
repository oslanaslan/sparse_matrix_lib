#include "standartlibs.h"
#include "lexem.h"

#ifndef INFOLIB

class Info {
public:
	string str;
	uint64_t pos;
	Info(): str(""), pos(0) {}
};

#define INFOLIB
#endif
