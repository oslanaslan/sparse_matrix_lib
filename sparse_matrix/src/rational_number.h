#include "exception.h"

#ifndef RATIONALNUMBER
class Rational_number {
	uint32_t n;
	uint32_t m;
	int z;
	uint32_t k;

public:

	// Конструкторы
	Rational_number(): n(0), m(1), z(1) {}
	Rational_number(const int x);
	Rational_number(const long x);
	Rational_number(const char *str); 
	Rational_number(const long x, const long y);
	Rational_number(const uint32_t x, const uint32_t y);
	Rational_number(const uint32_t x, const uint32_t y, const int a);
	// Операторы
	Rational_number operator =(const Rational_number &a);
	Rational_number operator +() const;
	friend Rational_number operator +(const Rational_number &a, const Rational_number &b);
	Rational_number operator -();
	friend Rational_number operator -(const Rational_number &a, const Rational_number &b); 
	friend Rational_number operator *(const Rational_number &a, const Rational_number &b);
	friend Rational_number operator /(const Rational_number &a, const Rational_number &b);
	bool operator <(const Rational_number &b) const;
	bool operator <=(const Rational_number &b) const;
	bool operator >(const Rational_number &b) const;
	bool operator >=(const Rational_number &b) const;
	bool operator ==(const Rational_number &b) const;
	bool operator !=(const Rational_number &b) const;
	Rational_number operator++(int);
	Rational_number operator--(int);
	Rational_number operator+=(const Rational_number &b);
	Rational_number operator-=(const Rational_number &b);
	Rational_number operator*=(const Rational_number &b);
	Rational_number operator/=(const Rational_number &b);
	friend std::ostream &operator <<(std::ostream &os, const Rational_number &n);

	// Геттеры и сеттеры
	uint32_t get_n() const;
	uint32_t get_m() const;
 	int get_z() const;
	uint32_t get_k() const;
	Rational_number get_number_part() const;
	Rational_number get_fractional_part() const;
	void set_n(const uint32_t a);
	void set_m(const uint32_t a);
	void set_z(const int a);
	void set(const uint32_t a, const uint32_t b, const int c);

	// Функции преобразования
	char *toString() const;
	unsigned short toUshort() const;
	int toInt() const;
	long int toLint() const;
	short toShort() const;
	double toDouble() const;
	uint32_t most_div(uint32_t a, uint32_t b);
	void make_canonical();
};
	
#endif
#define RATIONALNUMBER
