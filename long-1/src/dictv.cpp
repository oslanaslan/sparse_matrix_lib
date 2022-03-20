#include "standartlibs.h"
#include "exception.h"
#include "rational_number.h"
#include "dictv.h"

DictV::DictV() {
	len = 0;
	head = NULL;
	tail = NULL;

}
DictV::~DictV() {
	if (head != NULL) {
		node *nd;
		nd = head;
		while (head != tail) {
			nd = head;
			head = head->next;
			free(nd);
		}
		free(head);
	}

}

DictV::DictV(const DictV &d) {
	len = d.len;
	if (d.head != NULL) {
		node *nd = d.head;
		node *new_nd, *prev;
		new_nd = NULL;
		prev = NULL;
		while (nd != NULL) {	
			if (nd == d.head) 
				new_nd = (node *)malloc(sizeof(node));
			else {
				new_nd->next = (node *)malloc(sizeof(node));
				new_nd = new_nd->next;
			}
			new_nd->x = nd->x;
			new_nd->val = nd->val;
			if (nd == d.head) {
				new_nd->prev = NULL;
				head = new_nd;
			}
			else
				new_nd->prev = prev;
			if (nd == d.tail) {
				new_nd->next = NULL;
				tail = new_nd;
			}
			nd = nd->next;
		}
	}
	else {
		head = NULL;
		tail = NULL;
	}

}

void DictV::add(const uint32_t key, const Rational_number &v) {
	if (v == Rational_number(0)) {
		len++;
		return;
	}
	Rational_number a(v);
	node *nd = tail;
	if (tail == NULL) {
		nd = (node *)malloc(sizeof(node));
	}
	else {
		nd->next = (node *)malloc(sizeof(node));
		nd = nd->next;
	}
	nd->x = key;
	nd->val = a;
	nd->prev = tail;
	nd->next = NULL;
	tail = nd;
	if (head == NULL) {
		head = nd;
	}
	len++;
	return;

}

void DictV::set(const uint32_t key, const Rational_number &v) {
	node *nd = head;
	while (nd != NULL && nd->x != key) 
		nd = nd->next;
	if (nd == NULL)
		add(key, v);
	else 
		nd->val = Rational_number(v);
	return;
}

Rational_number *DictV::get(const uint32_t x) const {
	node *nd = head;
	while (nd != NULL && nd->x != x) 
		nd = nd->next;
	if (nd == NULL) return NULL;
	else return &(nd->val);

}

DictV::node *DictV::search(const Rational_number &a) const {
	node *nd = head;
	while (nd != NULL && nd->val != a) 
		nd = nd->next;
	if (nd == NULL) return NULL;
	else return nd;

}

bool DictV::in(const Rational_number &a) const {
	if (search(a) == NULL) return false;
	else return true;

}

uint32_t DictV::get_len() {
	return len;

}

void DictV::remove(uint32_t key) {
	node *nd = head;
	while (nd != NULL && nd->x != key) 
		nd = nd->next;
	if (nd != NULL) {
		if (nd == head)
			head = nd->next;
		if (nd == tail)
			tail = nd->prev;
		if (nd->prev != NULL)
			nd->prev->next = nd->next;
		if (nd->next != NULL)
			nd->next->prev = nd->prev;
		free(nd);
	}

}
