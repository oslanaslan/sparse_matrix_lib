#include "standartlibs.h"
#include "exception.h"
#include "rational_number.h"
#include "dictm.h"

DictM::DictM(const DictM &d) {
	set_head(NULL);
	set_tail(NULL);
	node *tnd = get_tail();
	node *nd = get_head();
	while (nd != tnd) {
		add(nd->x, nd->y, nd->num);
	}
}

void DictM::add(size_t x, size_t y, Rational_number n) {
	node *nd = (node *)malloc(sizeof(node));
	nd->x = x;
	nd->y = y;
	nd->num = n;
	if (get_head() == NULL) {
		nd->next = NULL;
		nd->prev = NULL;
		set_head(nd);
		set_tail(nd);
	}
	else {
		nd->next = NULL;
		nd->prev = get_tail();
		get_tail()->next = nd;
		set_tail(nd);
	}
	set_len(get_len()+1);
	return;
}

Rational_number DictM::get(size_t x, size_t y) const {
	if (get_head() == NULL) return Rational_number("0/1");
	node *nd = get_head();
	while (nd != NULL) {
		if (nd->x == x && nd->y == y) 
			return nd->num;
		nd = nd->next;
	}
	return Rational_number("0/1");
}

Rational_number *DictM::get_ptr(size_t x, size_t y) {
	if (get_head() == NULL) return NULL;
	node *nd = get_head();
	while (nd != NULL) {
		if (nd->x == x && nd->y == y)
			return &(nd->num);
		nd = nd->next;
	}
	return NULL;
}

void DictM::remove(size_t x, size_t y) {
	node *nd = get_head();
	while (nd != NULL) {
		if (nd->x == x && nd->y == y) {
			if (nd == get_head())
				set_head(nd->next);
			if (nd == get_tail()) 
				set_tail(nd->prev);
			nd->prev->next = nd->next;
			nd->next->prev = nd->prev;
			free(nd);
			set_len(get_len()-1);
			return;
		}
		else {
			nd = nd->next;
		}
	}
	return;
}

bool DictM::search(size_t x, size_t y) const {
	node *nd = get_head();
	while (nd != NULL) {
		if (nd->x == x && nd->y == y) 
			return true;
		nd = nd->next;
	}
	return false;
}

DictM::~DictM() {
	node *nd = get_head();
	for (uint64_t i = 0; i < get_len(); i++) {
		node *nnd = nd->next;
		free(nd);
		nd = nnd;
	}
	return;
}

void DictM::clear() {
	node *nd = get_head();
	for (uint64_t i = 0; i < get_len(); i++) {
		node *nnd = nd->next;
		free(nd);
		nd = nnd;
	}
	return;

}


