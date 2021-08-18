#include "standartlibs.h"
#include "exception.h"
#include "rational_number.h"
#include "dictv.h"
#include "vector.h"
#include "dictm.h"
#include "matrix_coords.h"

#ifndef MATRIX
class Matrix: public DictM {
	DictM dict;
	size_t n;
	size_t m;
public:
	Matrix(): n(0), m(0) {}
	Matrix(size_t n, size_t m, const Rational_number, bool isDiag);
	Matrix(Vector vec, bool isVertical);
	Matrix(const Matrix &a); 
	~Matrix() {}

	// Операторы
	const Matrix operator =(const Matrix &a) {
		if (this ==  &a) 
			return *this;
		dict.clear();
		dict = DictM();
		n = a.get_n();
		m = a.get_m();
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < m; j++) {
				Matrix_coords coords = Matrix_coords(i, j);
				Rational_number aa = a[coords];
				(*this).set(i, j, aa);

			}
		}
		return *this;
	}
	friend Matrix operator +(Matrix &a, Matrix &b);
	friend Matrix operator -(Matrix &a, Matrix &b);
	friend Matrix operator *(Matrix &a, Matrix &b);
	Matrix operator -();
	Matrix operator ~();
	Matrix operator ^(const uint32_t power);
	Rational_number operator [](Matrix_coords coords) const;
	Vector operator [](Matrix_row_coords coords) const;
	Vector operator [](Matrix_column_coords coords) const;
	friend std::ostream &operator<<(std::ostream &os, const Matrix &m);
	class Proxy {
		Matrix *matrix;
		Rational_number num;
		uint64_t x;
		uint64_t y;
	public:
		Proxy(): matrix(NULL), num(0), x(0), y(0) {}
		Proxy(Matrix *a, Rational_number numbr, uint64_t xx, uint64_t yy) {
			matrix = a;
			num = numbr;
			x = xx;
			y = yy;
		}
		Rational_number get_num() { return num; }
		Matrix *get_matrix() { return matrix; }
		uint64_t get_x() { return x; }
		uint64_t get_y() { return y; }		
		Proxy operator =(const Rational_number &a) {
			num = a;
			(*matrix).set(x, y, a);
			return *this;		
		}	
		operator Rational_number() const { return num; }
	};	
//	Rational_number *operator ()(size_t x, size_t y);
	Proxy operator ()(uint64_t x, uint64_t y);

	// Методы
	
	void set(size_t x, size_t y, Rational_number a);
	size_t get_n() const { return n; }
	size_t get_m() const { return m; }
	void set_n(size_t a) { n = a; }
	void set_m(size_t b) { m = b; }
	void write(const char *name);
	void write(FILE *fd);	
	void read(const char *name);
	void read(const int fd);
	char *toString() const;
	void printDict() const;
	void make_canonical();
protected:
	struct data_st {
		bool isEmpty;
		size_t x;
		size_t y;
		Rational_number num;
	};
	uint64_t *read_x_y(int fd);
	data_st read_data(int fd);
};

#endif
#define MATRIX
