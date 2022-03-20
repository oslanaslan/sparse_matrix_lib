#include "standartlibs.h"
#include "lex_analizator.h"

void Lex_analizator::run(string input_str) {
	first_step(input_str);
	second_step();
	remove_comments();
	return;
}

void Lex_analizator::remove_comments() {
	for (uint64_t i = 0; i < lex_vec.size() - 2; i++) {
		if (lex_vec[i].str == "/" && lex_vec[i+1].str == "*" && lex_vec[i+2].str == "/") {
			lex_vec.erase(lex_vec.begin() + i+2);
			lex_vec.erase(lex_vec.begin() + i+1);
			lex_vec.erase(lex_vec.begin() + i);
			i--;
		}
	}
}

void Lex_analizator::second_step() {
	// Вторичное форматирование и приведение лексем к нужному виду
	Lexem lex;
	for (uint64_t i = 0; i < lex_vec.size()-1; i++) {
		if ((lex_vec[i]).type == INTCONST && (lex_vec[i+1]).type == FLCONST) {
			lex.type = FLCONST;
			lex.str = (lex_vec[i]).str + (lex_vec[i+1]).str;
			lex_vec.erase(lex_vec.begin() + i+1);
			lex_vec.erase(lex_vec.begin() + i);
			lex_vec.insert(lex_vec.begin() + i, lex);
		}
	}

	for (uint64_t i = 0; i < lex_vec.size(); i++) {
		switch ((lex_vec[i]).type) {
			case WORD:
				switch (switch_helper((lex_vec[i]).str)) {
					case 0:
						(lex_vec[i]).type = DECLARE;
						break;
					case 1:
						(lex_vec[i]).type = PROCESS;
						break;
					case 2:
						(lex_vec[i]).type = TYPE;
						break;
					case 3:
						(lex_vec[i]).type = ROTATE;
						break;
					case 4:
						(lex_vec[i]).type = READ;
						break;
					case 5:
						(lex_vec[i]).type = WRITE;
						break;
					case 6:
						(lex_vec[i]).type = PRINT;
						break;
					case 7: 
						(lex_vec[i]).type = ROW;
						break;
					case 8:
						(lex_vec[i]).type = COLUMN;
						break;
					case 9:
						(lex_vec[i]).type = INFO;
						break;
					case 10:
						(lex_vec[i]).type = MAKE_CANONICAL;
						break;
					default:
						(lex_vec[i]).type = NAME;
						break;
				}
				break;
			case WORDINBRAK:
				(lex_vec[i]).type = STRING;
				break;
			case SEPARATOR:
				(lex_vec[i]).type = SEP;
				break;
			case BRAKETS:
				(lex_vec[i]).type = BRAKET;
				break;
			case INTCONST:
				(lex_vec[i]).type = ICONST;
				break;
			case FLCONST:
				(lex_vec[i]).type = FCONST;
				break;
			case OPERATOR:
				(lex_vec[i]).type = OP;
				break;
			default:
				std::cout << "Something went wrong: unknown type = " << (lex_vec[i]).type << std::endl;
				break;
		}

	}
	return;
}

int Lex_analizator::switch_helper(string s) {
	if (s == "declare")
		return 0;
	if (s == "process")
		return 1;
	if (s == "matrix" || s == "vector" || s == "integer" || s == "float" || s == "rational")
		return 2;
	if (s == "rotate") 
		return 3;
	if (s == "read")
		return 4;
	if (s == "write")
		return 5; 
	if (s == "print") 
		return 6;
	if (s == "row")
		return 7;
	if (s == "column")
		return 8;
	if (s == "info")
		return 9;
	if (s == "make_canonical")
		return 10;
	return 11;
}

void Lex_analizator::first_step(string input_str) {
	states_1 st = WORD;
	int state = st;
	state = state_ident(input_str[0]);
	uint64_t ptr = 1;
	Lexem lex;
	string str = "";
	str += input_str[0];
	if (ptr == input_str.length()) {
		lex.type = state;
		lex.str = str;
		lex_vec.push_back(lex);
		return;
	}
	bool flg = false;
	bool cflg = false;
	while (1) {
		switch (state) {
			case 0:	// WORD
				if (isDigit(input_str[ptr]) || isLat(input_str[ptr]) || input_str[ptr] == '_') {
					str += input_str[ptr];
					ptr++;
					if (ptr == input_str.length()) {
						lex.type = state;
						lex.str = str;
						lex_vec.push_back(lex);
						return;
					}
				}
				else {
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					lex.type = 0;
					str = input_str[ptr];
					state = state_ident(input_str[ptr]);
					ptr++;
					if (ptr == input_str.length()) {
						lex.type = state;
						lex.str = str;
						lex_vec.push_back(lex);
						return;
					}
				}
				break;
			case 1: //WORDINBRAK
				// Если кавычка и не экранированная
				if (!flg && input_str[ptr] == '"') {
					str += input_str[ptr];
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					lex.type = 0;
					lex.str = "";
					ptr++;
					if (ptr == input_str.length()) {
						return;
					}
					state = state_ident(input_str[ptr]);
					str = "";
					str += input_str[ptr];
					ptr++;
					if (ptr == input_str.length()) {
						lex.type = state;
					lex.str = str;
						lex_vec.push_back(lex);
						return;
					}
					flg = false;
				}
				else {
					if (input_str[ptr] == '\\')	
						flg = true;
					else 
						flg = false;
					str += input_str[ptr];
					ptr++;
					if (ptr == input_str.length()) {
						lex.type = state;
						lex.str = str;
						lex_vec.push_back(lex);
						return;
					}
				}	
				break;
			case 2: //BRAKETS
				lex.type = state;
				lex.str = str;
				lex_vec.push_back(lex);
				str = "";
				state = state_ident(input_str[ptr]);
				str += input_str[ptr];
				ptr++;
				if (ptr == input_str.length()) {
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					return;
				}
				break;
			case 3: //SEPARATOR
				lex.type = state;
				lex.str = str;
				lex_vec.push_back(lex);
				str = "";
				state = state_ident(input_str[ptr]);
				str += input_str[ptr];
				ptr++;
				if (ptr == input_str.length()) {
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					return;
				}
				break;
			case 4: //OPERATOR
				if (str == "/" && ptr != input_str.length() && input_str[ptr] == '*') {
					while (!(input_str[ptr] == '*' && input_str[ptr+1] == '/')) {
						if (ptr == input_str.length() -1) {
							state = UNKNOWN;
							break;
						}
						ptr++;
					}
				}
				lex.type = state;
				lex.str = str;
				str = "";
				lex_vec.push_back(lex);
				state = state_ident(input_str[ptr]);
				str += input_str[ptr];
				ptr++;
				if (ptr == input_str.length()) {
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					return;
				}
				break;
			case 5: //INTCONST:
			case 6://FLCONST:
				if (isDigit(input_str[ptr])) {
					str += input_str[ptr];
					ptr++;
					if (ptr == input_str.length()) {
						lex.type = state;
						lex.str = str;
						lex_vec.push_back(lex);
						return;
					}
				}
				else {
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					state = state_ident(input_str[ptr]);
					str = "";
					str += input_str[ptr];
					ptr++;
					if (ptr == input_str.length()) {
						lex.type = state;
						lex.str = str;
						lex_vec.push_back(lex);
						return;
					}
				}
				break;
			case 7: //COMMENT:
				while (input_str[ptr] != '\n') {
					if (ptr >= input_str.length())
						return;
					ptr++;
				}
				ptr++;
				str = "";
				if (ptr == input_str.length())
					return;
				str += input_str[ptr];
				state = state_ident(input_str[ptr]);
				ptr++;
				if (ptr == input_str.length()) {
					lex.type = state;
					lex.str = str;
					lex_vec.push_back(lex);
					return;
				}
				break;
			case 8: //LCOMMENT:
				if (str.length() == 1 && input_str[ptr] != '*') {
					state = OPERATOR;
				}
				else if (str.length() > 2 && input_str[ptr] == '*') {
					cflg = true;
				}
				else if (cflg && input_str[ptr] == '/') {
					ptr++;
					if (ptr == input_str.length())
						return;
					state = state_ident(input_str[ptr]);
					str = "";
					str += input_str[ptr];
				}
				else {
					cflg = false;
				}
				break;
			case 9: //UNKNOWN:
				// TODO Error manager
				std::cout << "error" << std::endl;
				return;			
		}
	}
}

Lex_analizator::states_1 Lex_analizator::state_ident(char c) {
	if (c >= '0' && c <= '9') 
		return INTCONST;
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_')
		return WORD;
	switch (c) {
		case '.':
			return FLCONST;
		case '#':
			return COMMENT;
		case '/':
			return LCOMMENT;
		case '"':
			return WORDINBRAK;
		case '{':
		case '}':
		case '(':
		case ')':
		case '[':
		case ']':
		case '\'':
			return BRAKETS;
		case ' ':
		case '\t':
		case '\n':
		case ',':
		case ':':
		case ';':
			return SEPARATOR;
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case '^':
			return OPERATOR;
		default:
			return UNKNOWN;
	}
}

bool Lex_analizator::isDigit(char c) {
	if (c >= '0' && c <= '9') 
		return true;
	else 
		return false;
}

bool Lex_analizator::isLat(char c) {
	if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') 
		return true;
	else
		return false;
}

void Lex_analizator::print_lex_vec() {
	string s;
	for (int i = 0; i < lex_vec.size(); i++) {
		switch((lex_vec[i]).type) {
			case DECLARE:
				s = "DECLARE";
				break;
			case PROCESS:
				s = "PROCESS";
				break;
			case TYPE:
				s = "TYPE";
				break;
			case NAME:
				s = "NAME";
				break;
			case SEP:
				s = "SEP";
				break;
			case BRAKET:
				s = "BRAKET";
				break;
			case STRING:
				s = "STRING";
				break;
			case ICONST:
				s = "ICONST";
				break;
			case FCONST:
				s = "FCONST";
				break;
			case OP:
				s = "OP";
				break;
			case INFO:
				s = "INFO";
				break;
			case READ:
				s = "READ";
				break;
			case WRITE:
				s = "WRITE";
				break;
			case PRINT:
				s = "PRINT";
				break;
			case ROTATE:
				s = "ROTATE";
				break;
			case ROW:
				s = "ROW";
				break;
			case COLUMN:
				s = "COLUMN";
				break;
			case MAKE_CANONICAL:
				s = "MAKE_CANONICAL";
				break;
			default:
				s = "UNKNOWN";
				break;
		}
		std::cout << s << "\t\t" << (lex_vec[i]).str << std::endl;
	}
	return;
}
