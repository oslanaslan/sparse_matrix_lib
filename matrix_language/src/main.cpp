#include "stdio.h"
#include "standartlibs.h"
#include "lexem.h"
#include "lex_analizator.h"
#include "syn_analizator.h"

int main(int argc, char **argv) {
	if (argc != 2) {
		cout << "Wring args" << endl;
		return 0;
	}

	Lex_analizator analiz;
	string s = "";
	FILE *f = fopen(argv[1], "r");
	while (1) {
		char c;
		int k = fread(&c, sizeof(char), 1, f);
		if (k == 0)
			break;
		s += c;
	}
	fclose(f);
	cout << "Running lexical analyzer ... ";
	try {
		analiz.run(s);
	}
	catch (...) {
		cout << endl <<  "SOMETHING WENT WRONG" << endl;
		return 0;
	}
	cout << "OK" << endl;
	cout << "Ranning syntactical analyzer ... ";
	try {	
		Syn_analizator anal(analiz);
		anal.run();
	}
	catch (Err err) {
		cout << endl << "SOMETHING WENT WRONG" << endl;
		cout << err.str << endl;
		return 0;
	}
	cout << "OK" << endl;
	return 0;
}

