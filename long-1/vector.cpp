#include "standartlibs.h"
#include "exception.h"
#include "rational_number.h"
#include "dictv.h"
#include "vector.h"

Vector::Vector(const Vector &a) {
		vec = DictV(a.vec);
		len = a.len;
	}
Vector::Vector(const uint32_t size, Rational_number parametr) {
		len = size;
		if (parametr != Rational_number((long)0)) {
			for (uint32_t i = 0; i < len; i++) 
				vec.add(i, parametr);
		}

	}
Vector operator +(Vector &a, Vector &b) {
	if (a.len != b.len) throw Exception(VECLEN_ERR);
	Vector c;
	c.len = a.len;
	Rational_number aa, bb;
	for (uint32_t i = 0; i < c.len; i++) {
		aa = a[i];
		bb = b[i];
		c.set(i, aa + bb);
	}
	return c;
}
Vector operator +(Vector &a, Rational_number &b) {
	Vector c;
	c.len = a.len;
	Rational_number aa;
	for (uint32_t i = 0; i < c.len; i++) {
		aa = a[i];
		c.set(i, aa + b);
	}
	return c;
}

Vector operator -(Vector &a, Vector &b) {
	if (a.len != b.len) throw Exception(VECLEN_ERR);
	Vector c;
	c.len = a.len;
	Rational_number aa, bb;
	for (uint32_t i = 0; i < c.len; i++) {
		aa = a[i];
		bb = b[i];
		c.set(i, aa - bb);
	}
	return c;
}
Vector operator -(Vector &a, Rational_number &b) {
	Vector c;
	c.len = a.len;
	Rational_number aa;
	for (uint32_t i = 0; i < c.len; i++) {
		aa = a[i];
		c.set(i, aa - b);
	}
	return c;

}
Rational_number operator *(Vector &a, Vector &b) { // Скалярное произведение
	if (a.len != b.len) throw Exception(VECLEN_ERR);
	Rational_number c = Rational_number((long)0);
	Rational_number aa, bb;
	for (uint32_t i = 0; i < a.len; i++) {
		aa = a[i];
		bb = b[i];
		c += aa * bb;
	}
	return c;
}
Vector operator *(Vector &a, Rational_number &b) {
	if (b == Rational_number((long)0)) throw Exception(ZERODIV_ERR);
	Vector c;
	Rational_number aa;
	c.len = a.len;
	for (uint32_t i = 0; i < c.len; i++) {
		aa = a[i];
		c.set(i, aa * b);
	}
	return c;

}
Vector operator /(Vector &a, Rational_number &b) {
	if (b == Rational_number((long)0)) throw Exception(ZERODIV_ERR);
	Vector c;
	Rational_number aa;
	c.len = a.len;
	for (uint32_t i = 0; i < c.len; i++) {
		aa = a[i];
		c.set(i, aa / b);
	}
	return c;
}
/*Vector::operator =(const Vector &a) {
	if (&a == this) return *this;
	(*this).vec = DictV(a.vec);
	(*this).len = a.len;
	return *this;

}*/
bool operator ==(const Vector &a, const Vector &b) {
	if (b.len != a.len) return false;
	bool res = true;
	for (uint32_t i = 0; i < b.len; i++) 
		if ((*(b.vec.get(i))) != (*(a.vec.get(i)))) 
			res = false;
	return res;

}
bool operator !=(const Vector &a, const Vector &b) {
	return b == a ? false : true;
}
Vector Vector::operator -() {
	Vector c;
	Rational_number *aa;
	c.len = len;
	for (uint32_t i = 0; i < len; i++) {
		aa = vec.get(i);
		if (aa != NULL) c.vec.add(i, (*aa)*(-1));
	}
	return c;

}
Rational_number Vector::operator [](const uint32_t index) {
	if (index >= len) throw Exception(INDEXOUTOFRANGE_ERR, "Rational_number operator [](const uint32_t index)");
	Rational_number *a = vec.get(index);
	if (a == NULL)
		return Rational_number((long)0);
	else
		return *a;
}
void Vector::operator ()(Rational_number &a, const uint32_t index) {
	if (index >= len) throw Exception(INDEXOUTOFRANGE_ERR, "void operator ()(Rational_number &a, const uint32_t index)");
	vec.set(index, a);
	return;
}
std::ostream &operator <<(std::ostream &os, Vector &v) {
	char *str = v.toString();
	os << str;
	free(str);
	return os;
}
void Vector::write(FILE *fd) {
	char *str1 = (*this).toString();
	int k = ::fwrite(str1, sizeof(char), strlen(str1), fd);
	if ((uint64_t)k < (uint64_t)strlen(str1)*sizeof(char)) throw Exception(UNEXPECTED_ERR, "void write(const char *name)");
	return;
		
}
void Vector::write(const char *name) {
	FILE *fd = fopen(name, "w");
	if (fd == NULL) throw Exception(UNEXPECTED_ERR, "Such file exists: Vector.write");
	write(fd);
	fclose(fd);
	return;
}
void Vector::read(const char *name) {
	int fd = open(name, O_RDWR|O_EXCL);
	if (fd <= 0 ) throw Exception(UNEXPECTED_ERR, "Such file doesn't exist: Vector.read");
	try {
		read(fd);
	}
	catch (...){
		char *msg = (char *)malloc(sizeof(char)*(strlen(name)+30));
		::sprintf(msg, "File %s is broken", name);
		throw Exception(FILE_ERR, msg);
	}
	close(fd);
	return;	
}
void Vector::read(const int fd) {		// not tested
	char *num = (char *)malloc(sizeof(char));
	num[0] = '\0';
	uint64_t a = read_x(fd);
	(*this).set_n(a);
	while (true) {
		data_st dst = read_data(fd);
		if (!dst.isEmpty && dst.num != Rational_number(0)) {
			if (dst.x >= a)
				throw Exception(FILE_ERR, "Vector: void read(const int fd)");
			set(dst.x, dst.num);
		}
		else 
			break;
	}
	return;
}
uint64_t Vector::read_x(int fd) {
	char c;
	char word[10] = "vector";
	uint64_t vec_ptr = 0;
	bool flag = false;
	while (vec_ptr != strlen(word)) {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0) {
			flag = true;
			break;
		}
		if (c == '#') {
			while (c != '\n' && k != 0)
				k = ::read(fd, &c, sizeof(char));
		}
		else if ((c == '\n' || c == '\t' || c == ' ') && vec_ptr == 0) {
			continue;
		}
		else if (c == word[vec_ptr]) {
			vec_ptr++;
		}
		else {
			flag = true;
			break;
		}	
	}	
	uint64_t res = 0;
	if (flag) {
		return res;
	}
	char *word_x = (char *)malloc(sizeof(char));
	size_t word_x_ptr = 0;
	word_x[word_x_ptr] = '\0';
	while (c != '\n') {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0) {
			break;
		}
		else if (c == ' ' || c == '\t') {
			continue;
		}
		else if (c >= '0' && c <= '9') {
			word_x = (char *)realloc(word_x, sizeof(char)*(word_x_ptr + 2));
			word_x[word_x_ptr++] = c;
			word_x[word_x_ptr] = '\0';
		}
		else if (c == '\n') {
			break;
		}
		else {
			throw Exception(FILE_ERR, "Vector: uint64_t read_x(int fd)");
		}
	}
	if (word_x_ptr != 0) {
		return atoi(word_x);
	}
	else 
		return 0;
}
Vector::data_st Vector::read_data(int fd) {
	data_st st;
	char *word = (char *)malloc(sizeof(char));
	size_t word_ptr = 0;
	word[0] = '\0';
	char c = '\0';
	while (true) {	// Читаем первые лишние пробелы и пустые строки до первого числа или конца файла
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0) {
			st.isEmpty = true;
			st.x = 0;
			st.num = Rational_number(0);
			free(word);
			return st;
		}
		if (c >= '0' && c <= '9')
			break;
		else if (c == '#') {
			while (c != '\n') {
				k = ::read(fd, &c, sizeof(char));
				if (k == 0) {
					st.isEmpty = true;
					free(word);
					return st;
				}
			}
		}
	}
	word = (char *)realloc(word, sizeof(char)*(word_ptr + 2));
	word[word_ptr++] = c;
	word[word_ptr] = '\0';
	while (true) {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0) {
			st.isEmpty = true;
			free(word);
			return st;
		}
		if (c >='0' && c <= '9') {
			word = (char *)realloc(word, sizeof(char)*(word_ptr + 2));
			word[word_ptr++] = c;
			word[word_ptr] = '\0';
		}
		else if (c == ' ' || c == '\t') {
			break;
		}
		else {
			free(word);
			throw Exception(FILE_ERR, "Vector: data_st read_data(int fd)");
		}
	}
	while (true) {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0) {
			st.isEmpty = true;
			free(word);
			return st;
		}
		if ((c >= '0' && c <= '9') || c == '-')
			break;
		else if (c != ' ' && c != '\t') {
			free(word);
			throw Exception(FILE_ERR, "Vector: data_st read_data(int fd)");
		}
	}
	word = (char *)realloc(word, sizeof(char)*(word_ptr + 3));
	word_ptr += 2;
	word[word_ptr-1] = c;
	word[word_ptr] = '\0';
	char *word_1 = word;
	char *word_2 = &(word[word_ptr-1]);

	while (c != '\n') {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0)
			break;
		word = (char *)realloc(word, sizeof(char)*(word_ptr + 2));
		word[word_ptr++] = c;
		word[word_ptr] = '\0';
	}
	st.isEmpty = false;
	st.x = atoi(word_1);
	try {
		st.num = Rational_number(word_2);
	}
	catch (...) {
		free(word);
		throw Exception(FILE_ERR, "Vector: data_st read_data(int fd)");
	}
	return st;
}
char *Vector::toString() {
	uint32_t ll = 0;
	char *num;
	bool trigger = false;	// Чтобы исключить лишнюю ',' в начале строки	
	char *res = (char *)malloc(sizeof(char));
	res[0] = '[';
	ll = 1;
	if (res == NULL) throw Exception(MEMORY_ERR);
	for (uint32_t i = 0; i < len; i++) {
		num = ((*this)[i]).toString();
		if (num == NULL) {
			res = (char *)realloc(res, sizeof(char)*(ll+2));
			ll += 2;
			if (res == NULL) throw Exception(MEMORY_ERR);
			if (trigger) {
				res[ll-2] = ',';
			} 
			else {
				trigger = true;
				res[ll-2] = ' ';
			}
			res[ll-1] = '0';
		}
		else {
			res = (char *)realloc(res, sizeof(char)*(ll + 1 + strlen(num)));
			if (trigger) {
				res[ll] = ',';
			}
			else {
				trigger = true;
				res[ll] = ' ';
			}
			uint32_t aa = ll;
			ll++;
			for (; ll < aa + 1 + strlen(num); ll++) {
				res[ll] = num[ll - aa - 1];
			}
		}
	}
	res = (char *)realloc(res, sizeof(char)*(ll + 2));
	if (res == NULL) throw Exception(MEMORY_ERR);
	res[ll++] = ']';
	res[ll++] = '\0';
	return res;
}
void Vector::add(Rational_number a) {
	vec.add(len, a);
	len++;
	return;
}
void Vector::set(uint64_t x, Rational_number a) {
	if (x >= len) throw Exception(INDEXOUTOFRANGE_ERR, "void set(uint64_t x, Rational_number)");
	vec.set(x, a);
	return;			
}
void Vector::set_n(uint64_t n) {
	len = n;	
}
size_t Vector::get_len() {
	return len;
}
void Vector::make_canonical() {
	Rational_number *num;
	for (uint64_t i = 0; i < len; i++) {
		num = vec.get(i);
		if (num) {
			(*num).make_canonical();
		}
	}
}
