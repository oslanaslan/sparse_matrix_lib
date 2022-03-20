#include "standartlibs.h"
#include "exception.h"
#include "rational_number.h"

Rational_number::Rational_number(const int x) {
	n = (uint32_t)(x < 0 ? x*(-1) : x);
	m = 1;
	z = x < 0 ? -1 : 1;
}

Rational_number::Rational_number(const long x) {
	n = (uint32_t)(x < 0 ? x*(-1) : x);
	m = 1;
	z = x < 0 ? -1 : 1;
}

Rational_number::Rational_number(const char *str) {  
	if (str == NULL) throw Exception(NULLPTR_ERR, "Rational_number(const char *str)");
	int nn = 1;
	char *str1 = (char *)malloc(sizeof(char)*(strlen(str)+1));
	char *str2 = (char *)malloc(sizeof(char)*(strlen(str)+1));
	int i = 0;
	bool trigger = true;
	for (i = 0; i < (int)strlen(str); i++) {
		if (str[i] == '/') {
			str1[i] = '\0';
			break;
		}
		else if (str[i] == '-') {
			nn *= -1;
			str1[i] = ' ';
		}
		else if (str[i] >= '0' && str[i] <= '9') {
			str1[i] = str[i];	
			trigger = false;
		}
		else {
			str1[i] = ' ';
		}
	}
	str1[i] = '\0';
	if (trigger) {
		free(str1);
		free(str2);
		throw Exception(BROKENRATNUM_ERR, "Rational_number(const char *str)");
	}
	n = atoi(str1);
	if ((uint64_t)i == strlen(str)) {
		m = 1;
		z = nn >=0 ? 1 : (-1);
		free(str1);
		free(str2);
		return;
	}
	i++;
	trigger = true;
	if (i == (int)strlen(str)) {
		m = 1;
	}
	else {
		int j = i;
		for (; i < (int)strlen(str); i++) {
			if (str[i] == '/') {
				str2[i-j] = '\0';
				break;
			}
			else if (str[i] == '-') {
				nn *= -1;
				str2[i-j] = ' ';
			}
			else if (str[i] >= '0' && str[i] <= '9') {
				str2[i-j] = str[i];
				trigger = false;
			}
			else {
				str2[i-j] = ' ';
			}
		}
		str2[i-j] = '\0';
		if (trigger) {
			free(str1);
			free(str2);
			throw Exception(BROKENRATNUM_ERR, "Rational_number(const char *str)");
		}
		m = atoi(str2);
		if (m == 0) {
			free(str1);
			free(str2);
			throw Exception(ZERODIV_ERR, "Rational_number(const char *str)");
		}
	}
	z = nn >= 0  ? 1 : (-1);
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	free(str1);
	free(str2);
}

Rational_number::Rational_number(const long x, const long y) {
	n = (uint32_t)(x < 0 ? x*(-1) : x);
	if (y == 0) {
		throw Exception(ZERODIV_ERR, "Rational_number(const long x, const long y");
	}
	m = (uint32_t)(y < 0 ? y*(-1) : y);
	z = x*y < 0 ? -1 : 1;
	uint32_t k = most_div(n,m);
	n /= k;
	m /= k;
}

Rational_number::Rational_number(const uint32_t x, const uint32_t y) {
	if (y == 0) { throw Exception(ZERODIV_ERR, "Rational_number(const uint32_t x, const uint32_t y)"); }
	n = x;
	m = y;
	z = 1;
	uint32_t k = most_div(n,m);
	n /= k;
	m /= k;
}

Rational_number::Rational_number(const uint32_t x, const uint32_t y, const int a) {
	if (y == 0) { throw Exception(ZERODIV_ERR, "Rational_number(const uint32_t x, const uint32_t y, const int a)"); }
	n = x;
	m = y;
	z = a;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
}

// Операторы
Rational_number Rational_number::operator =(const Rational_number &a) {
	if (&a == this) return *this;
	return Rational_number(this->n = a.n, this->m = a.m, this->z = a.z >= 0 ? 1 : -1);
}

Rational_number Rational_number::operator +() const {
	return *this;
}

Rational_number operator +(const Rational_number &a, const Rational_number &b) {
	if (a.m == 0 || b.m == 0) throw Exception(ZERODIV_ERR, "friend Rational_number operator +(const Rational_number &a, const Rational_number &b)");
	Rational_number c;
	long long int aa;
	aa = (long long int)(a.n*b.m)*a.z + (long long int)(a.m*b.n)*b.z;
	c.z = aa >= 0 ? 1 : -1;
	aa = aa >= 0 ? aa : aa * (-1);
	if (aa > UINT_MAX) throw Exception(OVERFLOW_ERR, "friend Rational_number operator +(const Rational_number &a, const Rational_number &b)");
	if (a.m*b.m > UINT_MAX) throw Exception(OVERFLOW_ERR, "friend Rational_number operator +(const Rational_number &a, const Rational_number &b)");
	c.n = aa;
	c.m = a.m*b.m;
	uint32_t k = c.most_div(c.n, c.m);
	c.n /= k;
	c.m /= k;
	return c;
}

Rational_number Rational_number::operator -() {
	return Rational_number(n, m, z * -1);
}

Rational_number operator -(const Rational_number &a, const Rational_number &b) { 
	Rational_number c = Rational_number(b.get_n(), b.get_m(), b.get_z() * (-1));
	Rational_number d = a + c;
	return d;
}

Rational_number operator *(const Rational_number &a, const Rational_number &b) {
	Rational_number c;
	if (a.get_n() * b.get_n()> UINT_MAX) throw Exception(OVERFLOW_ERR, "friend Rational_number operator *(const Rational_number &a, const Rational_number &b)");
	c.n = a.get_n() * b.get_n();
	if (a.get_m() * b.get_m() > UINT_MAX) throw Exception(OVERFLOW_ERR, "friend Rational_number operator *(const Rational_number &a, const Rational_number &b)");
	c.m = a.get_m() * b.get_m();
	c.z = a.get_z() * b.get_z();
	uint32_t k = c.most_div(c.get_n(), c.get_m());
	c.n /= k;
	c.m /= k;
	return c;
}

Rational_number operator /(const Rational_number &a, const Rational_number &b) {
	Rational_number c;
	if (a.n * b.m > UINT_MAX) throw Exception(OVERFLOW_ERR, "friend Rational_number operator /(const Rational_number &a, const Rational_number &b)");
	if (a.m * b.n > UINT_MAX) throw Exception(OVERFLOW_ERR, "friend Rational_number operator /(const Rational_number &a, const Rational_number &b)");
	c.n = a.n * b.m;
	c.m = a.m * b.n;
	c.z = a.z * b.z;
	uint32_t k = c.most_div(c.n, c.m);
	c.n /= k;
	c.m /= k;
	return c;
}

bool Rational_number::operator <(const Rational_number &b) const {
	int32_t aa, bb;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	return aa < bb ? true : false;
}

bool Rational_number::operator <=(const Rational_number &b) const {
	int32_t aa,bb;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	return aa <= bb ? true : false;
}

bool Rational_number::operator >(const Rational_number &b) const {
	int32_t aa,bb;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	return aa > bb ? true : false;
}

bool Rational_number::operator >=(const Rational_number &b) const {
	int32_t aa,bb;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	return aa >= bb ? true : false;
}

bool Rational_number::operator ==(const Rational_number &b) const {
	int32_t aa,bb;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	return aa == bb ? true : false;
}

bool Rational_number::operator !=(const Rational_number &b) const {
	int32_t aa, bb;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	return aa != bb ? true : false;
}

Rational_number Rational_number::operator++(int) {
	int32_t aa = n*z + m;
	n = aa >= 0 ? aa : (-1)*aa;
	z = aa >= 0 ? 1 : -1;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	return Rational_number(n,m,z);	
}

Rational_number Rational_number::operator--(int) {
	int32_t aa = n*z - m;
	n = aa >= 0 ? aa : (-1)*aa;
	z = aa >= 0 ? 1 : -1;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	return Rational_number(n,m,z);
}

Rational_number Rational_number::operator+=(const Rational_number &b) {
	int32_t aa = n*b.m*z;
	int32_t bb = b.n*m*b.z;
	int32_t cc = aa + bb;
	n = cc >= 0 ? cc : (-1)*cc;
	m = m*b.m;
	z = cc >= 0 ? 1 : -1;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	return Rational_number(n, m, z);
}

Rational_number Rational_number::operator-=(const Rational_number &b) {
	int32_t aa, bb, cc;
	aa = n*b.m*z;
	bb = b.n*m*b.z;
	cc = aa - bb;
	n = cc >= 0 ? cc : (-1)*cc;
	m = m*b.m;
	z = cc >= 0 ? 1 : -1;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	return Rational_number(n, m, z);
}

Rational_number Rational_number::operator*=(const Rational_number &b) {
	n *= b.n;
	m *= b.m;
	z *= b.z;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	return Rational_number(n,m,z);
}

Rational_number Rational_number::operator/=(const Rational_number &b) {
	n *= b.m;
	m *= b.n;
	z *= b.z;
	uint32_t k = most_div(n, m);
	n /= k;
	m /= k;
	return Rational_number(n, m, z);
}

std::ostream& operator <<(std::ostream &os, const Rational_number &n) {
	char *str = n.toString();
	os << str;
	free(str);
	return os;
}

// Геттеры и сеттеры
uint32_t Rational_number::get_n() const { return n; }
uint32_t Rational_number::get_m() const { return m; }
int Rational_number::get_z() const { return z; }
uint32_t Rational_number::get_k() const { return k; }

Rational_number Rational_number::get_number_part() const {
	uint32_t res = n;
	uint32_t i = 0;
	for (; res >= m; i++, res -= m);
	return Rational_number(i, 1, z);
}

Rational_number Rational_number::get_fractional_part() const {
	uint32_t res = n;
	uint32_t i = 0;
	for (; res >= m; i++, res -= m);
	return Rational_number(res, m, z);
}

void Rational_number::set_n(const uint32_t a) { n = a; }
void Rational_number::set_m(const uint32_t a) { m = a; }
void Rational_number::set_z(const int a) { z = a >= 0 ? 1 : -1; }
void Rational_number::set(const uint32_t a, const uint32_t b, const int c) { n = a; m = b, z = c; } 

// Функции преобразования
char * Rational_number::toString() const {
	if (m == 0) throw Exception(ZERODIV_ERR);
	int nn = 0;
	uint32_t a, b;
	a = n;
	b = m;
	for (; a != 0; nn++, a = a / 10);
	for (; b != 0; nn++, b /= 10);
	char *res = (char *)malloc(sizeof(char)*(nn+3));
	if (res == NULL) {
		throw Exception(MEMORY_ERR);
	}
	if (z >= 0)
		sprintf(res, "%d/%d", n, m);
	else 
		sprintf(res, "-%d/%d", n, m);
	return res;
}

unsigned short Rational_number::toUshort() const {
	unsigned short res;
	if ((uint32_t)(n/m) > USHRT_MAX) throw Exception(TRANSFORMATION_ERR);
	res = (unsigned short)(n / m);
	return res;
}

int Rational_number::toInt() const {
	int res;
	if ((int32_t)(z * n / m) > INT_MAX || (int32_t)(z * n / m) < INT_MIN) throw Exception(TRANSFORMATION_ERR, "int Rational_number::toInt()");
	res = (int)(z * n / m);
	return res;
}

long int Rational_number::toLint() const {
	long int res;
	if ((int64_t)(z * n / m) > LONG_MAX || (int64_t)(z * n / m) < LONG_MIN) throw Exception(TRANSFORMATION_ERR, "long int Rational_number::toLint()");
	res = (long int)(z * n / m);
	return res;
}

short Rational_number::toShort() const {
	short res;
	if ((int64_t)(z * n / m) > SHRT_MAX || (int64_t)(z * n / m) < SHRT_MIN) throw Exception(TRANSFORMATION_ERR, "short Rational_number::toShort()");
	res = (short)(z * n / m);
	return res;
}

double Rational_number::toDouble() const {
	return (double)(z*n/m);
}

uint32_t Rational_number::most_div(uint32_t a, uint32_t b) {
	if (b == 0) 
		return 	a;
	return most_div(b, a % b);
}

void Rational_number::make_canonical() {
	uint32_t o = most_div(n,m);
	if (n % o != 0 || m % o != 0)
		std::cout << "make_canonical for rational_number failed" << std::endl;
	n /= o;
	m /= o;
}
