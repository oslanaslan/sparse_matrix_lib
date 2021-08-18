#include <cstdio>
#include <cstdlib>
#include "String.cpp"

using namespace std;

int main() {
	String s = String("ololololo");
	const String ss = String("lalalalalal");
	String sss = String(1);
	String aa = s;
	String aao = String(12.0);
	sss = s + ss + aao;
	aao.print();
	aao.slomat(); 
	sss.print();
	aao.print();
	
	cout << "\n\n\n" << endl;

	sss = s + ss + aao;
	sss.print();
	aao.print();
	s.print();
	ss.print();
	return 0;
}
