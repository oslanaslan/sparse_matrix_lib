#include "dictv.h"

#ifndef VECTOR
class Vector {
	uint32_t len;
	DictV vec;
public:
	// Конструкторы
	Vector(): len(0) {}
	Vector(const Vector &a);
	Vector(const uint32_t size, Rational_number parametr);
	~Vector() {}
	// Операторы
	friend Vector operator +(Vector &a, Vector &b);
	friend Vector operator +(Vector &a, Rational_number &b);
	friend Vector operator -(Vector &a, Vector &b);
	friend Vector operator -(Vector &a, Rational_number &b);
	friend Rational_number operator *(Vector &a, Vector &b); // Скалярное произведение
	friend Vector operator *(Vector &a, Rational_number &b);
	friend Vector operator /(Vector &a, Rational_number &b);
	Vector operator =(const Vector &a) {
	if (&a == this) return *this;
		(*this).vec = DictV(a.vec);
		(*this).len = a.len;
		return *this;

	}

	friend bool operator ==(const Vector &a, const Vector &b);
	friend bool operator !=(const Vector &a, const Vector &b);
	Vector operator -();
	Rational_number operator [](const uint32_t index);
	void operator ()(Rational_number &a, const uint32_t index);
	friend std::ostream &operator <<(std::ostream &os, Vector &v);

	// Методы
	void write(FILE *fd);
	void write(const char *name);
	void read(const char *name);
	struct data_st {
		bool isEmpty;
		uint64_t x;
		Rational_number num;
	};
	void read(const int fd);
	uint64_t read_x(int fd);
	data_st read_data(int fd);
	char *toString();
	void add(Rational_number a);
	void set(uint64_t x, Rational_number a);
	void set_n(uint64_t n);
	size_t get_len();
	void make_canonical();
};


#endif
#define VECTOR
