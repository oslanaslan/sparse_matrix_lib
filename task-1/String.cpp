#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

class String {
	char *str;
	size_t len;
	size_t allm;
	bool isBroken;
	
public:
	String();
	String(const int);
	String(const double);		
	String(const String &a);
	String(const char *);
	~String();
	friend String operator+(const String &a, const String &b);
	String operator =(const String &a);
	friend bool operator ==(const String &a, const String &b);
	friend bool operator !=(const String &a, const String &b);
	char *toString() const;
	void slomat() { isBroken = true; if (str != NULL) { free(str); str = NULL;}}
	void print() const;
};

String::String() {
	str = NULL;
	len = 0;
	allm = 0;
	isBroken = false;
}

void String::print() const {
	char *str = toString();
	if (str == NULL) {
		std::cout << "Object is broken" << std::endl;
	}
	else {
		std::cout << str << std::endl;
		free(str);
	}
	return;
}

String::String(const int a) {
	int n = 0;
	int b = a;
	for (; b != 0; n++, b /= 10);
	str = (char *)malloc(sizeof(char)*(n+2));
	str[0] = '\0';
	if (str == NULL) {
		isBroken = true;
		allm = 0;
		len = 0;
	}
	else {
		allm = sizeof(char)*(n+2);
		sprintf(str, "%d", a);
		len = strlen(str);
		isBroken = false;
	}
}

String::String(const String &a) {
	if (a.isBroken) {
		isBroken = true;
		str = NULL;
		allm = 0;
		len = 0;
	}
	else {
		str = (char *)malloc(sizeof(char)*(a.len + 2));

		if (str == NULL) {
			isBroken = true;
			len = 0;
			allm = 0;
		}
		else {
			str[0]='\0';
			allm = a.allm;
			len = a.len;
			isBroken = false;
			strcpy(str, a.str);
		}
	}
}
	
String::String(const char *n_str) {
	str = (char *)malloc(sizeof(char)*(strlen(n_str)+1));
	if (str == NULL) {
		isBroken = true;
		return;
	}
	isBroken = false;
	strcpy(str, n_str);
	len = strlen(str);
	allm = sizeof(char)*strlen(str);
}
String::~String() {
	if (str != NULL)
		free(str);
	return;
}

String::String(const double a) {
	str = (char *)malloc(sizeof(char)*100);
	if (str == NULL) {
		len = 0;
		isBroken = true;
		allm = 0;
		return;
	}
	str[0] = '\0';
	sprintf(str, "%f", a);
	len = strlen(str);
	allm = len+1;
	isBroken = false;
	return;
}

char *String::toString() const{
	if (isBroken) {
		return NULL;
	}
	char *new_str = (char *)malloc(sizeof(char)*(len+1));
	new_str[0] = '\0';
	if (str != NULL);
		strcpy(new_str, str);
	return new_str;
}
String String::operator =(const String &a) {
	if (this == &a)
		return *this;
	if (a.isBroken) {
		(*this).isBroken = true;
		if ((*this).str != NULL) {
			free((*this).str);
			(*this).str = NULL;
		}
		(*this).allm = 0;
		(*this).len = 0;
		return *this;
	}
	if ((*this).str != NULL)
		free((*this).str);
	(*this).len = a.len;
	(*this).allm = a.allm;
	isBroken = false;
	str = NULL;
	for (size_t i = 0; i < len; i++) {
		str = (char *)realloc(str, sizeof(char)*(i+2));
		if (str == NULL) {
			isBroken = true;
			break;
		}
		str[i] = a.str[i];
	}
	return *this;
}

bool operator ==(const String &a, const String &b) {
	if (a.isBroken == true && b.isBroken == true)
		return true;
	if (a.len == b.len) {
		bool res = true;
		for (size_t i = 0; i < a.len; i++) {
			if (a.str[i] != b.str[i])
				res = false;
		}
		return res;
	}
	else {
		return false;
	}
}
bool operator !=(const String &a, const String &b) {
	if (a == b)
		return false;
	else
		return true;
}

String operator +(const String &a, const String &b) {
	char *ololo = (char *)malloc(sizeof(char)*(a.len + b.len + 1));
	if (a.isBroken || b.isBroken) {
		String s;
		s.slomat();
		free(ololo);
		return s;
	}

	if (ololo == NULL) {
		String s = String(1);
		s.isBroken = true;
		return s;
	}
	for (size_t i = 0; i < a.len; i++) {
		ololo[i] = a.str[i];
	}
	for (size_t i = 0; i < b.len; i++) {
		ololo[a.len + i] = b.str[i];
	}
	ololo[a.len + b.len] = '\0';
	String s =  String(ololo);
	free(ololo);
	return s;
}
