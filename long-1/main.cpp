/*#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>*/
#include "standartlibs.h"
#include "exception.h"
#include "rational_number.h"
#include "vector.h"
#include "matrix.h"

#define EXCEPTION
#define RATIONALNUMBER
#define VECTOR
#define MATRIX

using namespace std;

int main() {
	try {
		cout << Rational_number("-1/2") << endl;
		
		Matrix m1, m2, m3;
		m1.read("tests/mat_3.txt");
		m2.read("tests/mat_4.txt");
		cout << m1 << "\n\n" << endl;
		cout << m2 << "\n\n" << endl;
		m3 = m1 * m2;
		cout << m3 << "\n\n" << endl;
		Matrix m4;
		m4.printDict();
		m4.read("tests/mat_2.txt");
		m4.printDict();
		m4 = m4^2;
		m4.printDict();
		cout << m4 << "\n\n\n" << endl;
		m4.printDict();
		cout << (~m4) << endl;
		cout << "olololo" << endl;
		cout << m4 << endl;
		m4.printDict();
		cout << "\n\n\n" << endl;
		m4(1, 1) = Rational_number(0); 
		cout << m4 << endl;
		m4.printDict();
		Matrix m5 = Matrix(3, 3, Rational_number(1), true);
		cout << "\n\n\n" << m5 << endl;
		m5.printDict();
		cout << "Proxy test \n\n\n" << endl;

		Matrix::Proxy p = m5(1,1);
		p = Rational_number(3);
		cout << "\n\n" << m5 << endl;
        m5(2,1)=12;

        m5.printDict();


	}
	catch (Exception ex) {
		cout << "ERROR: " << ex.get_code() << " in " << ex.get_msg() << endl;
	}
	return 0;
}


