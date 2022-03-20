#define EMPTY_LIST 1

class Exception {
public:
	int code;
	Exception(): code(0) {}
	Exception(int a): code(a) {}
	~Exception() {}	
};
