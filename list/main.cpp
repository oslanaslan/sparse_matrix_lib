#include <cstdio>
#include <iostream>
#include "Exception.cpp"
#include "List.cpp"
#include <string>

using namespace std;

int main() {
	try {
		List<int> a;
		for (int i = 0; i < 10; i++)
			a.push(i);
		a.print();
		a.rotation_left(18);
		cout << a[-3] << " -3" << endl;
		cout << a.search_max() << " max" << endl;
		a.print();
		List<int> b;
		b.rotation_right(-1);
	}
	catch (Exception &b) {
		cout << "ERROR " << b.code << endl;
	}
	return 0;
}
