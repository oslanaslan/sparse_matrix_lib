#include "syn_analizator.h"

Syn_analizator::Syn_analizator(Lex_analizator anal) {
	iterator = 0;
	lex_vec = anal.get_lex_vec();
}

Lexem Syn_analizator::gc() {
	if (iterator == lex_vec_for_parsing_without_infos.size()) {
		Err err;
		err.pos = iterator-1;
		err.str = "Wrong input";
		throw err;
	}
	Lexem lex = lex_vec_for_parsing_without_infos[iterator++];
	cur_lex = lex;
	return lex;
}

void Syn_analizator::run() {
	remove_separators();
	remove_infos();
	try {
		START();
	}
	catch (Err err) {
		throw;
	}
}

void Syn_analizator::remove_separators() {
	for (uint64_t i = 0; i < lex_vec.size(); i++) {
		if (lex_vec[i].type != SEP || (lex_vec[i].str != " " && lex_vec[i].str != "\t" && lex_vec[i].str != "\n")) {
			lex_vec_for_parsing.push_back(lex_vec[i]);
		}
	}
	return;
}

void Syn_analizator::remove_infos() {
	for (uint64_t i = 0; i < lex_vec_for_parsing.size(); i++) {
		if (lex_vec_for_parsing[i].type == INFO) {
			if (i >= lex_vec_for_parsing.size() - 3) {
				Err err;
				err.pos = lex_vec_for_parsing.size() - 1;
				err.str = "Too few arguments for info";
				throw err;
			}
			if (lex_vec_for_parsing[i+1].str != "(") {
				Err err;
				err.pos = i+1;
				err.str = "Wrong argument for info";
				throw err;
			}
			if (lex_vec_for_parsing[i+2].type != STR) {
				Err err;
				err.pos = i+2;
				err.str = "Wrong argument for info";
				throw err;
			}
			if (lex_vec_for_parsing[i+3].str != ")") {
				Err err;
				err.pos = i + 3;
				err.str = "Wrong argument for info";
				throw err;
			}
			Info info;
			info.str = lex_vec_for_parsing[i+2].str;
			info.pos = lex_vec_for_parsing_without_infos.size();
			infos.push_back(info);
			info_pos.push_back(info.pos);
			i += 3;

		}
		else {
			lex_vec_for_parsing_without_infos.push_back(lex_vec_for_parsing[i]);	
		}
	}
	return;
}

void Syn_analizator::START() {
	Err old_err;
	DECLARE();
	Lexem lex; 
	try {
		try { lex = gc(); } catch (...) { throw; }
	}
	catch (...) {
		throw;
	}
	if (lex.type != SEP || lex.str != ":") {
		Err err;
		err.pos = iterator -1;
		err.str = "There should be ':'";
		throw err;
	}
	uint64_t old_it = iterator;
	try {
		DECLAREPART();
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	PROCESS();
	try {
	try { lex = gc(); } catch (...) { throw; }
	}
	catch(...) {
		throw;
	}

	if (lex.type != SEP || lex.str != ":") {
		Err err;
		err.pos = iterator-1;
		err.str = "There should be ':'";
		throw err;
	}
	try {
		PROCESSPART();
	}
	catch (Err err) {
		throw err;
	}
	END();
}

void Syn_analizator::END() {
		Lexem lex;
		try {
			lex = gc();
		}
		catch (Err err) {
			// Пытаемся получить очередную лексему. Если получилось, значит достигли
			// конца, можно закругляться, все ОК
			return;
		}

		Err err;
		err.pos = iterator - 1;
		err.str = "Why not end?";
		throw err;
}

void Syn_analizator::DECLARE() {
	Lexem lex; 
	try {
		try { lex = gc(); } catch (...) { throw; }
	}
	catch (...) {
		throw;
	}
	if (lex.type != DEC || lex.str != "declare") {
		Err err;
		err.pos = iterator - 1;
		err.str = "There should ne 'declare'";
		throw err;
	}
}

void Syn_analizator::DECLAREPART() {
	DEFINITION();
	uint64_t old_it = iterator;
	try {
		while (1) {
			old_it = iterator;
			DEFINITION();
		}
	}
	catch (...) {
		iterator = old_it;
	}
}

void Syn_analizator::DEFINITION() {
	TYPE();
	Lexem lex; 
	try {
		try { lex = gc(); } catch (...) { throw; }
	}
	catch (...) {
		throw;
	}
	if (lex.type != SEP || lex.str != ":") {
		Err err;
		err.pos = iterator - 1;
		err.str = "There should be ':'";
		throw err;
	}
	PERMDECS();
	try {
		try { lex = gc(); } catch (...) { throw; }
	}
	catch (...) {
		throw;
	}
	if (lex.type != SEP || lex.str != ";") {
		Err err;
		err.pos = iterator -1;
		err.str = "There should be ';'";
		throw err; 
	}
}

void Syn_analizator::PERMDECS() {
	PERMDEC();
	uint64_t old_it = iterator;
	try {
		while (1) {
			old_it = iterator;
			Lexem lex; 
			try {
				try { lex = gc(); } catch (...) { throw; }
			}
			catch (...) {
				throw;
			}
			if (lex.type != SEP || lex.str != ",") {
				Err err;
				err.pos = iterator -1;
				err.str = "There should be ','";
				throw err;
			}
			PERMDEC();
		}
	}
	catch (...) {
		iterator = old_it;
	}
}

void Syn_analizator::PERMDEC() {
	NAME();
	uint64_t old_it =  iterator;
	try {
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != "(") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '('";
			throw err;
		}
		PARAMETR();
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != ")"){
			Err err;
			err.pos = iterator - 1;
			err.str = "There should be ')'";
			throw err;
		}
	}
	catch (...) {
		iterator = old_it;
	}
}

void Syn_analizator::PARAMETR() {
	Err old_err;
	uint64_t old_it = iterator;
	try {
		old_it = iterator;
		STRING();
		return;
	}
	catch (Err err) {
		old_err = err;
		iterator = old_it;
	}
	try {
		old_it = iterator;
		ICONST();
		return;
	}
	catch (Err err) {
		old_err = err;
		iterator = old_it;
	}
	try {
		old_it = iterator;
		FCONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		ICONST();
		Lexem lex;
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.type != OP || lex.str != "/") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '/'";
			throw err;
		}
		ICONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.type != OP || lex.str != "-") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '-'";
			throw err;
		}
		ICONST();
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw; 
		}
		if (lex.type != OP || lex.str != "/") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '/'";
			throw err;
		}
		ICONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.type != OP || lex.str != "-") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '-'";
			throw err;
		}
		ICONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		} catch (...) {
			throw;
		}
		if (lex.type != OP || lex.str != "-") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '-'";
			throw err;
		}
		FCONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}

void Syn_analizator::PROCESS() {
	Lexem lex; 
	try {
		try { lex = gc(); } catch (...) { throw; }
	}
	catch (...) {
		throw;
	}

	if (lex.type != PR || lex.str != "process") {
		Err err;
		err.pos = iterator-1;
		err.str = "There should be 'process'";
		throw err;
	}
}

void Syn_analizator::PROCESSPART() {
	uint64_t old_it;
	Err old_err;
	try {
		old_it = iterator;
		OBJ();
		OPS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex;
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != "(") {
			Err err;
			err.pos = iterator - 1;
			err.str = "osds";
			throw err;
		}
		BRAKETS();
		OPS();
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex;
		try {
			lex = gc();
		}
		catch (...) {
			throw;
		}
		if (lex.str != ";") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ';'";
			throw err;
		}
		PROCESSPART();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		END();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}

void Syn_analizator::OPS() {
	uint64_t old_it;
	Err old_err;
	try {
		old_it = iterator;
		OPERATOR();
		PROCESSPART();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}

		if (lex.str != ":") {
			Err err;
			err.pos = iterator -1;
			err.str = "dkjnkdn";
			throw err;
		}
		FUN();
		OPS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}

	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...)  {
			throw;
		}
		if (lex.str != "[") {
			Err err;
			err.pos = iterator -1;
			err.str = "dkjnkdn";
			throw err;
		}
		S();
		OPS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex;
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != ";") {
			Err err;
			err.pos = iterator -1;
			err.str = "ksjnkn";
			throw err;
		}
		PROCESSPART();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}

void Syn_analizator::BRAKETS() {
	uint64_t old_it;
	Err old_err;
	try {
		old_it = iterator;
		OBJ();
		OB();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != "(") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		BRAKETS();
		OB();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}

void Syn_analizator::OB() {
	uint64_t old_it;
	Err old_err;
	try {
		old_it = iterator;
		OPERATOR();
		BRAKETS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != ":") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		FUN();
		OB();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != ")") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}

		if (lex.str != "[") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		S();
		OB();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}

	throw old_err;
}

void Syn_analizator::S() {
	uint64_t old_it;
	Err old_err;
	try {
		old_it = iterator;
		OBJ();
		OS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != "(") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		BRAKETS();
		OS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}

void Syn_analizator::OS() {
	uint64_t old_it;
	Err old_err;
	try {
		old_it = iterator;
		OPERATOR();
		BRAKETS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != ":") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		FUN();
		OS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw; 
		}
		if (lex.str != "]") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != "[") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		S();
		OS();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; 
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != ",") {
			Err err;
			err.pos = iterator - 1;
			err.str = "Kknkn";
			throw err;
		}
		S();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}

	throw old_err;
}


/*void Syn_analizator::PROCESSPART() {
	Err old_err;
	uint64_t old_it = iterator;
	try {
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.type != SEP || lex.str != ";") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ';'";
			throw err;
		}
		try {
			while (1) {
				old_it = iterator;
				PROCESSPART();
			}
		}
		catch (Err err) {
			iterator = old_it;
			old_err = err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}

	try {
		old_it = iterator;
		COMMAND();
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.type != SEP || lex.str != ";") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ';'";
			throw err;
		}
		try {
			while (1) {
				old_it = iterator;
				PROCESSPART();
			}
		}
		catch (Err err) {
			iterator = old_it;
			old_err = err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}

void Syn_analizator::COMMAND() {
	Err old_err;
	uint64_t old_it = iterator;
	try {
		old_it = iterator;
		OBJ();
		OPERATOR();
		COMMAND();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "(") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '('";
			throw err;
		}
		OBJ();
		OPERATOR();
		COMMAND();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != ")") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '/'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	} */
/*	try {
		old_it = iterator;
		COMMAND();
		COMMAND();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}*/
/*	try {
		old_it = iterator;
		OBJ();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}

	throw old_err;
}
*/
void Syn_analizator::OBJ() {
	uint64_t old_it = iterator;
	Err old_err;
	try {
		old_it = iterator;
		NAME();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		ICONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		FCONST();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}
/*
void Syn_analizator::OBJ_TYPE_1() {
	uint64_t old_it = iterator;
	Err old_err;
	try {
		old_it = iterator;
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "[") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '['";
			throw err;
		}
		ICONST();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != ",") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ','";
			throw err;
		}
		ICONST();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "]") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ']'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "[") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '['";
			throw err;
		}
		ICONST();
		if (lex.str != "]") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ']'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;
}
*/
void Syn_analizator::FUN() {
	Err old_err;
	uint64_t old_it = iterator;
	try {
		old_it = iterator;
		READ();
		Lexem lex;
		try {
			try { lex = gc(); } catch (...) { throw; }
		}
		catch (...) {
			throw;
		}
		if (lex.str != "(") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '('";
			throw err;
		}
		STRING();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != ")") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ')'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		WRITE();
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "(") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '('";
			throw 1;
		}
		STRING();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != ")") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ')'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		PRINT();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		ROTATE();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		ROW();
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "(") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '('";
			throw err;
		}
		ICONST();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != ")") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ')'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		COLUMN();
		Lexem lex; try { lex = gc(); } catch (...) { throw; }
		if (lex.str != "(") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be '('";
			throw err;
		}
		ICONST();
		try { lex = gc(); } catch (...) { throw; }
		if (lex.str != ")") {
			Err err;
			err.pos = iterator-1;
			err.str = "There should be ')'";
			throw err;
		}
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	try {
		old_it = iterator;
		MAKE_CANONICAL();
		return;
	}
	catch (Err err) {
		iterator = old_it;
		old_err = err;
	}
	throw old_err;

}

void Syn_analizator::TYPE() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != TY) {
		Err err;
		err.pos = iterator -1;
		err.str = "It is not a type";
		throw err;
	}
	return;
}

void Syn_analizator::NAME() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != NA) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a name";
		throw err;
	}
	return;
}

void Syn_analizator::STRING() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != STR) {
		Err err;
		err.pos = iterator -1;
		err.str = "It is not a string";
		throw err;
	}
	return;
}

void Syn_analizator::ICONST() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != IC) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not an int const";
		throw err;
	}
	return;
}

void Syn_analizator::FCONST() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != FC) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a float const";
		throw err;
	}
	return;
}

void Syn_analizator::OPERATOR() {
	Lexem lex;
	try {
		try { lex = gc(); } catch (...) { throw; }
	}
	catch (...) {
		throw;
	}
//	cout << lex.str << " " << lex.type<< endl;
	if (lex.type != OP) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a operator";
		throw err;
	}
	return;
}

void Syn_analizator::READ() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != RD) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a read";
		throw err;
	}
	return;
}

void Syn_analizator::WRITE() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != WR) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a write";
		throw err;
	}
	return;
}

void Syn_analizator::PRINT() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != PRT) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a print";
		throw err;
	}
	return;
}

void Syn_analizator::ROTATE() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != RO) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a rotate";
		throw err;
	}
	return;
}

void Syn_analizator::COLUMN() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != CMN) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a column";
		throw err;
	}
	return;
}

void Syn_analizator::ROW() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != RW) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a row";
		throw err;
	}
	return;
}

void Syn_analizator::MAKE_CANONICAL() {
	Lexem lex; try { lex = gc(); } catch (...) { throw; }
	if (lex.type != MC) {
		Err err;
		err.pos = iterator - 1;
		err.str = "It is not a make_canonical";
		throw err;
	}
	return;
}

