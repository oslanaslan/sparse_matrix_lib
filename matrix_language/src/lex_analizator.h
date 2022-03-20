#include "standartlibs.h"
#include "lexem.h"

#ifndef LEXANALIZATOR

/* Лексемы 
 * DECLARE 	{ "declare" }
 * PROCESS	{ "process" }
 * TYPE		{ "matrix", "vector", "rational", "integer", "float" }
 * NAME		{ a^n: a in {'a'-'z', 'A'-'Z', '0'-'9', '_'} причем как минимум одна буква}
 * SEP		{ ':', ',', ' ', '\t', '\n', ';'}
 * BRAKET	{ '{', '}', '(', ')', '\'', '"', '[', ']' }
 * STRING	{ "{a^n: a in {<any char>}}"}
 * ICONST	{ a^n: a in {'0'-'9'}}
 * FCONST	{ a^n.b^n: a,b in {'0'-'9'}}
 * OP		{ '=', '+', '*', '-', '/', '^' }
 * INFO 	{ "info" }
 * READ		{ "read" }
 * WRITE 	{ "write" }
 * PRINT	{ "print" }
 * ROTATE	{ "rotate" }
 * ROW		{ "row" }
 * COLUMN	{ "column" }
 * MAKE_CANONICAL { "make_canonical" }
 */

 /* Лексемы нижнего уровня 
  * WORD	{ '0'-'9', 'a'-'z', 'A'-'Z', '_' }
  * WORDINBRAK	{ "<any_char>" }
  * BRAKETS	{ '{', '}', '(', ')', '[', ']', ''' }
  * SEPARATOR	{ ' ', '\t', '\n', ',', ':', ';' }
  * OPERATOR	{ '=', '+', '-', '*', '/', '^' }
  * INTCONST	{ '0'-'9' }
  * FLCONST	{ '.''0'-'9' }
  * COMMENT 	{ '#'<any_char>'\n'}
  * LCOMMENT 	{ \/* <any_char> *\/ }
  */

class Lex_analizator {
	vector<Lexem> lex_vec;
public:
	enum states_1 {
		WORD,
		WORDINBRAK,
		BRAKETS,
		SEPARATOR,
		OPERATOR,
		INTCONST,
		FLCONST,
		COMMENT,
		LCOMMENT,
		UNKNOWN
	};
	enum statees {
		DECLARE,
		PROCESS,
		TYPE,
		NAME,
		SEP,
		BRAKET,
		STRING,
		ICONST,
		FCONST,
		OP,
		INFO,
		READ,
		WRITE,
		PRINT,
		ROTATE,
		ROW,
		COLUMN,
		MAKE_CANONICAL
	};
	Lex_analizator() {}
	void first_step(string input_str);
	void second_step();
	void run(string input_string);
	states_1 state_ident(char c);
	bool isDigit(char c);
	bool isLat(char c);
	vector<Lexem> get_lex_vec() { return lex_vec; }
	void print_lex_vec();
	int switch_helper(string s);
	void remove_comments();
};

#define LEXANALIZATOR
#endif
