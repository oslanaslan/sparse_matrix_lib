#include "standartlibs.h"
#include "rational_number.h"
#include "exception.h"
#include "dictv.h"
#include "dictm.h"
#include "vector.h"
#include "matrix.h"

Matrix::Matrix(size_t n, size_t m, const Rational_number a = 0, bool isDiag = false) {
	set_n(n);
	set_m(m);
	if (a == 0) {
		set_n(n);
		set_m(m);
		return;
	}
	if (isDiag) {
		size_t b = n > m ? m : n;
		for (size_t i = 0; i < b; i++) {
			dict.add(i, i, a);
		}
	}
	else {
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < m; j++) {
				dict.add(i, j, a);
			}
		}
	}
}

Matrix::Matrix(Vector vec, bool isVertical = true) {
	if (isVertical) {
		set_m(vec.get_len());
		set_n(1);
		for (size_t i = 0; i < vec.get_len(); i++) {
			set(0, i, vec[i]);
		}
	}
	else {
		set_n(vec.get_len());
		set_m(1);
		for (size_t i = 0; i < vec.get_len(); i++) {
			set(i, 0, vec[i]);
		}
	}
}

Matrix operator +(Matrix &a, Matrix &b) {
	if (a.get_n() != b.get_n() || a.get_m() != b.get_m()) throw Exception(MATRIXLEN_ERR, "Matrix operator +(const Matrix &a, const Matrix &b)");
	Matrix new_matrix = Matrix(a.get_n(), a.get_m());
	for (size_t i = 0; i < a.get_n(); i++) {
		for (size_t j = 0; j < a.get_m(); j++) {
			try {
				Matrix_coords coords;
				coords.set(i, j);
				Rational_number aa = a[coords];
				Rational_number bb = b[coords];
				Rational_number cc = aa + bb;
				new_matrix.set(i, j, cc);
			}
			catch (...) { 
				throw Exception(UNEXPECTED_ERR, "Matrix operator +(const Matrix &a, const Matrix &b)");
			}
		}
	}
	return new_matrix;
}

Matrix operator -(Matrix &a, Matrix &b) {
	if (a.get_n() != b.get_n() || a.get_m() != b.get_m()) throw Exception(MATRIXLEN_ERR, "Matrix operator +(const Matrix &a, const Matrix &b)");
	Matrix new_matrix = Matrix(a.get_n(), a.get_m());
	for (size_t i = 0; i < a.get_n(); i++) {
		for (size_t j = 0; j < a.get_m(); j++) {
			try {
				Matrix_coords coords;
				coords.set(i, j);
				new_matrix.set(i, j, a[coords] - b[coords]);
			}
			catch (...) { 
				throw Exception(UNEXPECTED_ERR, "Matrix operator +(const Matrix &a, const Matrix &b)");
			}
		}
	}
	return new_matrix;

}

Matrix operator *(Matrix &a, Matrix &b) {
	if (a.get_m() != b.get_n()) throw Exception(MATRIXLEN_ERR, "Matrix operator *(Matrix &a, Matrix &b)");
	Matrix new_matrix = Matrix(a.get_n(), b.get_m());
	for (size_t i = 0; i < a.get_n(); i++) {
		for (size_t j = 0; j < b.get_m(); j++) {
			Rational_number rat = Rational_number(0);
			for (size_t k = 0; k < a.get_m(); k++) {
				Matrix_coords c2 = Matrix_coords(i, k);
				Matrix_coords c3 = Matrix_coords(k, j);
				rat += a[c2] * b[c3];
			}
			new_matrix.set(i,j,rat);
		}
	}
	return new_matrix;
}

Matrix Matrix::operator -() {
	Matrix new_matrix = Matrix(get_n(), get_m());
	for (size_t i = 0; i < get_n(); i++) {
		for (size_t j = 0; j < get_m(); j++) {
			Matrix_coords c = Matrix_coords(i, j);
			new_matrix.set(i, j, (*this)[c] * Rational_number(-1));
		}
	}
	return new_matrix;
}

Matrix Matrix::operator ~() {
	Matrix new_matrix = Matrix(get_m(), get_n());
	for (size_t i  = 0; i < get_n(); i++) {
		for (size_t j = 0; j < get_m(); j++) {
			Matrix_coords c1 = Matrix_coords(i, j);
			new_matrix.set(j, i, (*this)[c1]);
		}
	}
	return new_matrix;
}

Matrix Matrix:: operator ^(const uint32_t power) {
	if (power == 0) throw Exception(UNEXPECTED_ERR, "Matrix Matrix::operator ^(const uint32_t power): power == 0");
	if (n != m) throw Exception(UNEXPECTED_ERR, "Matrix Matrix::operator ^(const uin32_t power): n != m");
	Matrix new_matrix = (*this);
	for (uint32_t l = 0; l < power-1; l++) {
		new_matrix = new_matrix * new_matrix;
	}
	return new_matrix;
}

Rational_number Matrix::operator [](Matrix_coords coords) const {
	if (coords.get_x() >= get_n() || coords.get_y() >= get_m()) throw Exception(MATRIXLEN_ERR, "Rational_number Matrix::operator [](Matrix_coords coords)"); 
	size_t a = coords.get_x();
	size_t b = coords.get_y();
	if (dict.search(a, b)) {
		return dict.get(coords.get_x(), coords.get_y());
	}
	else {
		return Rational_number(0);
	}
}

Vector Matrix::operator [](Matrix_row_coords coords) const {
	if (coords.get_x() >= get_n()) throw Exception(MATRIXLEN_ERR, "Vector Matrix::operator [](Matrix_row_coords coords)"); 
	Vector vec;
	for (size_t i = 0; i < get_m(); i++) {
		Matrix_coords c = Matrix_coords(coords.get_x(), i);
		vec.add((*this)[c]);
	}
	return vec;
}

Vector Matrix::operator [](Matrix_column_coords coords) const {
	if (coords.get_y() >= get_m()) throw Exception(MATRIXLEN_ERR, "Vector Matrix::operator [](Matrix_column_coords coords)");
	Vector vec;
	for (size_t i = 0; i < get_n(); i++) {
		Matrix_coords c = Matrix_coords(i, coords.get_y());
		vec.add((*this)[c]);
	}
	return vec;
}

void Matrix::read(const char *name) { 
	int fd = open(name, O_RDWR|O_EXCL);
	if (fd <= 0) throw Exception(UNEXPECTED_ERR, "No such file");
	try {
		read(fd);
	}
	catch (...) {
		char *msg = (char *)malloc(sizeof(char)*(strlen(name) + 30));
		sprintf(msg, "File %s is broken", name);
		throw Exception(FILE_ERR, msg);
	}
	close(fd);
	return;
}

void Matrix::write(const char *name) {
	FILE *fd = fopen(name, "w");
	if (fd == NULL) throw Exception(UNEXPECTED_ERR, "No such file");
	write(fd);
	fclose(fd);
	return;
}

void Matrix::read(const int fd) {	
	uint64_t *a = read_x_y(fd);
	(*this).set_n(a[0]);
	(*this).set_m(a[1]);
	while (true) {
		data_st dst = read_data(fd);
		if (!dst.isEmpty && dst.num != Rational_number(0)) {
			if (dst.x >= a[0] || dst.y >= a[1]) {
				free(a);
				throw Exception(FILE_ERR, "void Matrix::read(const int fd)");
			}
			set(dst.x, dst.y, dst.num);
		}
		else 
			break;
	}
	free(a);
	return;
}

uint64_t* Matrix::read_x_y(int fd) {
	char c;
	char word[10] = "matrix";
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
	uint64_t *res = (uint64_t *)malloc(sizeof(uint64_t)*2);
	res[0] = 0;
	res[1] = 0;
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
			word_x = (char *)realloc(word_x, sizeof(char)*(word_x_ptr + 2));
			word_x[word_x_ptr++] = '\0';
			word_x[word_x_ptr] = '\0';
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
			throw Exception(FILE_ERR, "Matrix: uint64_t *Matrix::read_x_y(int fd)");
		}
	}
	if (word_x_ptr != 0) {
		char *w = word_x;
		size_t i = 0;
		while (::strlen(w) == 0) {
			w = &(word_x[++i]);
			if (i == word_x_ptr) {
				free(word_x);
				return res;
			}
		}
		res[0] = atoi(w);
		w = &(w[::strlen(w)]);
		i = 0;
		while (::strlen(w) == 0) {
			w += sizeof(char);
			if (i == word_x_ptr) {
				free(word_x);
				return res;
			}
		}
		res[1] = atoi(w);
		free(word_x);
		return res;
	}
	else 
		return res;

}

void Matrix::write(FILE *fd) {	
	char *str = (*this).toString();
	size_t k = ::fwrite(str, sizeof(char), strlen(str), fd);
	if (k != sizeof(char)*strlen(str)) throw Exception(UNEXPECTED_ERR, "File is broken!");
	free(str);
	return;
}

Matrix::Proxy Matrix::operator ()(uint64_t x, uint64_t y) {
	if (x >= get_n() || y >= get_m()) throw Exception(MATRIXLEN_ERR, "Proxy Matrix::operator ()(uint64_t x, uint64_t y)");
	Matrix_coords coord = Matrix_coords(x, y);
	return Proxy(this, (*this)[coord], x, y);
}

void Matrix::set(size_t x, size_t y, Rational_number a) {
	if (x >= get_n() || y >= get_m()) throw Exception(MATRIXLEN_ERR, "void Matrix::set(size_t x, size_t y, Rational_number a)");
	if (a == Rational_number(0)) {
		if (dict.search(x, y)) 
			dict.remove(x, y);
		return;
	}
	if (dict.search(x, y)) {
		Rational_number *r = dict.get_ptr(x,y);
		*r = a;
	}
	else {
		dict.add(x, y, a);
	}
}

char *Matrix::toString() const {
	char *res = (char *)malloc(sizeof(char)*2);
	if (res == NULL) throw Exception(MEMORY_ERR, "char *Matrix::toString()");
	res[0] = '[';
	res[1] = '\0';
	size_t k = 1;
	for (size_t i = 0; i < get_n(); i++) {
		for (size_t j = 0; j < get_m(); j++) {
			Matrix_coords c = Matrix_coords(i, j);
			Rational_number num = (*this)[c];
			char *str = num.toString();
			res = (char *)realloc(res, sizeof(char)*(strlen(res) + strlen(str) + 2));
			for (size_t l = 0; l < 	strlen(str); l++) {
				res[k++] = str[l];
			}
			res[k++] = ',';
			res[k] = '\0';	
			free(str);
		}
		res[k-1] = ']';
		if (i != get_n()-1) {
			res = (char *)realloc(res, sizeof(char)*(strlen(res)+3));
			res[k++] = '\n';
			res[k++] = '[';
			res[k] = '\0';
		}
	}
	return res;	
}

Matrix::data_st Matrix::read_data(int fd) {	// Not tested
	data_st st;
	char *word = (char *)malloc(sizeof(char));
	size_t word_ptr = 0;
	word[0] = '\0';
	char c = '\0';

	// Читаем первые лишние пробелы и пустые строки до первого числа или конца файла
	while (true) {
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

	// Читаем первую координату
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
			throw Exception(FILE_ERR, "Matrix: Matrix::data_st Matrix::read_data(int fd)");
		}
	}
	st.x = atoi(word);
	free(word);
	word_ptr = 0;

	// Читаем пробелы после первой координаты
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
			throw Exception(FILE_ERR, "Matrix: Matrix::data_st Matrix::read_data(int fd)");
		}
	}
	word = (char *)malloc(sizeof(char)*2);
	word[0] = c;
	word[1] = '\0';
	word_ptr = 1;

	// Читаем вторую координату
	while (true) {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0) {
			st.isEmpty = true;
			free(word);
			return st;	
		}
		if (c >= '0' && c <= '9') {
			word = (char *)realloc(word, sizeof(char)*(word_ptr + 1));
			word[word_ptr++] = c;
			word[word_ptr] = '\0';
		}
		else if (c == ' ' || c == '\t' || c == '-') {
			break;
		}
		else {
			free(word);
			throw Exception(FILE_ERR, "Matrix: Matrix::data_st Matrix::read_data(int fd)");
		}
	}
	st.y = atoi(word);	
	free(word);	
	word = (char *)malloc(sizeof(char));	
	word_ptr = 0;	

	if (c == '-') {
		word = (char *)realloc(word, sizeof(char)*(word_ptr + 2));
		word[word_ptr++] = c;
		word[word_ptr] = '\0';
	}
	else {
	}

	while (c != '\n') {
		int k = ::read(fd, &c, sizeof(char));
		if (k == 0 || c == '\n')
			break;
		word = (char *)realloc(word, sizeof(char)*(word_ptr + 2));
		word[word_ptr++] = c;
		word[word_ptr] = '\0';
	}

	st.isEmpty = false;
	try {
		st.num = Rational_number(word);
	}
	catch (...) {
		free(word);
		throw Exception(FILE_ERR, "Matrix: Matrix::data_st Matrix::read_data(int fd)");
	}
	free(word);
	return st;
}

Matrix::Matrix(const Matrix &a) {
	n = a.get_n();
	m = a.get_m();
	for (size_t i = 0 ; i < a.get_n(); i++) {
		for (size_t j = 0; j < a.get_m(); j++) {
			Matrix_coords coords = Matrix_coords(i, j);
			Rational_number aa = a[coords];
			if (aa != Rational_number(0)) {
				(*this).set(i, j, aa);
			}
		}
	}
}

void Matrix::printDict() const {
	node *head = dict.get_head();
	std::cout << "Dict List:" << std::endl;
	while (head != NULL) {
		char *output_string = (head->num).toString();
		std::cout << "(" << head->x << ", " << head->y << ", " << output_string << ")" << std::endl;
		free(output_string);
		head = head->next;
	}
}

std::ostream &operator<<(std::ostream &os, const Matrix &m) {
	char *str = m.toString();
	os << m.get_n() << "x" << m.get_m() << "\n" << str;
	free(str);
	return os;
}

void Matrix::make_canonical() {
	Rational_number *ptr;
	for (uint64_t i = 0; i < n; i++) {
		for (uint64_t j = 0; j < m; j++) {
			ptr = dict.get_ptr(i, j);
			(*ptr).make_canonical();
		}
	}
}
