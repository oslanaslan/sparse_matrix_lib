#include "rational_number.h"

#ifndef DICTV

class DictV {
	uint32_t len;
	struct node {
		uint32_t x;
		Rational_number val;
		node *next;
		node *prev;
	};
	node *head;
	node *tail;
public:

	// Конструкторы
	DictV();
	~DictV();
	DictV(const DictV &d);

	// Методы
	void add(const uint32_t key, const Rational_number &v);
	void set(const uint32_t key, const Rational_number &v);
	Rational_number *get(const uint32_t x) const;
	node *search(const Rational_number &a) const;
	bool in(const Rational_number &a) const;
	uint32_t get_len();
	void remove(uint32_t key);
};

#endif
#define DICTV
