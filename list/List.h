#ifndef __list
#define __list

template <typename type> class List {
	struct Node {
		type data;
		Node *next;
		Node *prev;
	};
	size_t len;
	Node *head;
public:
	// Конструкторы
	List();				// ready
	List(const List<type> &a);	// ready
	~List();			// ready

	// Операторы
	type operator [](int n); 	// ready	// Забрать со смещением с сохраненим объекта в кольце
	// Методы
	void push(type a);		// ready
	void rotation_right(int n);	// ready	// Вращение кольца по часовой
	void rotation_left(int n);	// ready	// Вращение кольца против часовой
	void insert(type a);		// ready	// Вставка в начало
	type get_top();			// ready	// Забрать с вершины 
	type get_at(int n);		// ready	// Забрать со смещением на n (отрицательным и положительным) без сохранения объекта в еольце
	void print();			// ready	// Печть себя на экран
	type search_max();		// ready		// извлечёт из кольца максимальный элемент и начало кольца, установит на элемент находящийся перед ним, если в кольце был один элемент, то сделает кольцо пустым, если кольцо было пустое, то бросит исключение
	size_t get_len() { return len; }
		
};

#endif
