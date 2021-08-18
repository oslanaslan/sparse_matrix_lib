#include "standartlibs.h"
#include "exception.h"

Exception::Exception() { 
	code = UNEXPECTED_ERR; 
	msg = NULL;
}
Exception::Exception(int a) { 
	code = a; 
	msg = NULL;
}
int Exception::get_code() { 
	return code; 
}
const char *Exception::get_msg() {
	if (msg == NULL) {
		char *res = (char *)malloc(sizeof(char)*(strlen("Unknown") + 1));
		if (res == NULL) throw Exception(MEMORY_ERR);
		std::sprintf(res, "Unknown");
		return (const char *)res;
	}
	else {
		char *res = (char *)malloc(sizeof(char)*(strlen(msg) + 1));
		if (res == NULL) throw Exception(MEMORY_ERR);
		strcpy(res, msg);
		return (const char *)res;
	}
}
Exception::Exception(int a, const char *b) {
	code = a;
	msg = (char *)malloc(sizeof(char)*(strlen(b)+1));
	if (msg == NULL) throw Exception(MEMORY_ERR);
	strcpy(msg, b);
	msg[strlen(b)] = '\0';
}
Exception::~Exception() {
	if (msg != NULL)
		free(msg);
}
Exception::Exception(const Exception &a) {
	if (a.msg != NULL) {
		msg = (char *)malloc(sizeof(char) * (strlen(a.msg) + 1));
		if (msg == NULL) throw Exception(MEMORY_ERR);
		strcpy(msg, a.msg);
	}
	else
		msg = NULL;
	code = a.code;
}
