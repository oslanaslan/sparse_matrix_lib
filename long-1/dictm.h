#include "rational_number.h"

#ifndef DICTM
struct node {
	size_t x;
	size_t y;
	Rational_number num;
	node *next;
	node *prev;
};

class DictM {
	node *head;
	node *tail;
	size_t len;
public:
	DictM(): head(NULL), tail(NULL), len(0) {}
	DictM(const DictM &d);
	
	void add(size_t x, size_t y, Rational_number n);
	Rational_number get(size_t x, size_t y) const;
	Rational_number *get_ptr(size_t x, size_t y);
	bool search(size_t x, size_t y) const;
	void remove(size_t x, size_t y);

	void set_head(node *nd) { head = nd; }
	void set_tail(node *nd) { tail = nd; }
	void set_len (size_t l) { len = l; }
	node *get_head() const { return head; }
	node *get_tail() const { return tail; }
	size_t get_len() const { return len; }
	void clear();

	~DictM();
};
#endif
#define DICTM
