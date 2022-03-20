#include "standartlibs.h"
#include "lexem.h"
#include "lex_analizator.h"
#include "info.h"
#include "err.h"

#ifndef SYNANALIZATOR

class Syn_analizator {
	Lexem cur_lex;
	vector<Lexem> lex_vec;
	vector<Lexem> lex_vec_for_parsing;
	vector<Lexem> lex_vec_for_parsing_without_infos;
	vector<Info> infos;
	vector<uint64_t> info_pos;
	uint64_t iterator;
	enum states {
		DEC,	// 0 
		PR,  	// 1
		TY, 	// 2
		NA, 	// 3
		SEP, 	// 4
		BRAKET, // 5
		STR, 	// 6
		IC, 	// 7
		FC,  	// 8
		OP, 	// 9
		INFO, 	// 10
		RD, 	// 11
		WR, 	// 12
		PRT, 	// 13
		RO, 	// 14
		RW, 	// 15
		CMN, 	// 16
		MC 	// 17
		};
public:
	Syn_analizator(): iterator(0) {};
	Syn_analizator(Lex_analizator);
	Lexem gc();
	void run();
	void remove_separators();
	void remove_infos();
	void START();
	void DECLARE();
	void DECLAREPART();
	void PROCESS();
	void PROCESSPART();
	void DEFINITION();
	void PERMDECS();
	void PERMDEC();
	void PARAMETR();
	void COMMAND();
	void OBJ();
	void OBJ_TYPE_1();
	void OBJ_TYPE_2();
	void TYPE();
	void NAME();
	void STRING();
	void ICONST();
	void FCONST();
	void OPERATOR();
	void READ();
	void WRITE();
	void PRINT();
	void ROTATE();
	void COLUMN();
	void ROW();
	void MAKE_CANONICAL();
	void BRAKETS();
	void OPS();
	void FUN();
	void S();
	void OB();
	void OS();
	void END();
};


#define SYNANALIZATOR
#endif
