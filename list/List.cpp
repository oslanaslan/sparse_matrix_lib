#include "List.h"

template<typename type> List<type>::List() {
	head = NULL;
	len = 0;
}

template<typename type> List<type>::List(const List<type> &a) {
	this->head = NULL;
	this->len = 0;
	Node *tmp = a.head;
	for (size_t i = 0; i < a.len; i++) {
		push(tmp->data);
		tmp = tmp->next;
	}
}

template<typename type> List<type>::~List() {
	Node *tmp = head;
	for (size_t i = 0; i < len; i++) {
		tmp = head->next;
		delete head;
		head = tmp;
	}
}

template<typename type> void List<type>::insert(type a) {
	push(a);
	return;
}

template<typename type> type List<type>::operator [](int n) {
	if (len == 0 || head == NULL) throw Exception(EMPTY_LIST);
	if (n >= 0) {
		Node *tmp = head;
		for (int i = 0; i < n; i++) {
			tmp = tmp->next;
		}
		return tmp->data;
	}
	else {
		int m = n*(-1);
		Node *tmp = head;
		for (int i = 0; i < m; i++) {
			tmp = tmp->prev;
		}
		return tmp->data;
	}
}

template<typename type> type List<type>::search_max() {
	if (len == 0 || head == NULL) throw Exception(EMPTY_LIST);
	type max = head->data;
	int n = 0;
	for (size_t i = 0; i < len; i++) {
		if ((*this)[i] > max) {
			max = (*this)[i];
			n = i;
		}
	}
	return get_at(n);
}

template<typename type> void List<type>::rotation_right(int n) {
	if (len == 0 || head == NULL) throw Exception(EMPTY_LIST);
	if (n < 0) {
		(*this).rotation_left(-n);	
		return;
	}
	for (int i = 0; i < n; i++) {
		head = head->prev;
	}
	return;
}

template<typename type> void List<type>::rotation_left(int n) {
	if (len == 0 || head == NULL) throw Exception(EMPTY_LIST);
	if (n < 0) {
		(*this).rotation_right(-n);
		return;
	}
	for (int i = 0; i < n; i ++) {
		head = head->next;
	}
	return;
}

template<typename type> void List<type>::print() {
	Node *tmp = head;
	std::cout << "==========" << std::endl;
	for (size_t i = 0; i < len; i++) {
		std::cout << i << "\t" << tmp->data << std::endl;
		tmp = tmp->next;
	}
	std::cout << "==========" << std::endl;
	return;
}

template<typename type> void List<type>::push(type a) {
	Node *tmp = new Node;
	tmp->data = a;
	if (len == 0) {
		head = tmp;
		head->next = tmp;
		head->prev = tmp;	
		tmp->next = tmp;
		tmp->prev = tmp;
	}
	else {
/*		tmp->next = head;
		tmp->prev = head->prev;
		head->prev->next = tmp;
		head->prev = tmp;
		head = tmp;*/
		tmp->prev = head;
		tmp->next = head->next;
		head->next->prev = tmp;
		head->next = tmp;
	}

	len++;
	return;
}

template<typename type> type List<type>::get_top() {
	type res;
	if (len == 0 || head == NULL) 
		throw Exception(EMPTY_LIST);
	Node *tmp = head;
	res = head->data;
	if (len == 1) {
		head = NULL;
	}
	else {
		head->next->prev = head->prev;
		head->prev->next = head->next;	
		head = head->next;
	}
	len--;
	delete tmp;
	return res;
}

template<typename type> type List<type>::get_at(int n) {
	if (len == 0 || head == NULL) throw Exception(EMPTY_LIST);
	if (n == 0) {
		return get_top();
	}
	Node *tmp = head;
	type res;
	if (n > 0) {
		int m = n % len;
		for (int i = 0; i < m; i++) 
			tmp = tmp->next;
	}
	else {
		int m = n*(-1);
		m = n % len;
		for (int i = 0; i < m; i++) 
			tmp = tmp->prev;
	}
	if (len == 1) {
		head = NULL;
	}
	else {
		tmp->next->prev = tmp->prev;
		tmp->prev->next = tmp->next;
		if (tmp == head)
			head = head->next;
	}
	res = tmp->data;
	delete tmp;
	len--;
	return res;
}
