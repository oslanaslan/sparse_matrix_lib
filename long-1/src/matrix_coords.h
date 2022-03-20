#ifndef MATRIXCOORDS

class Matrix_coords {
	size_t x;
	size_t y;

public:

	Matrix_coords(): x(0), y(0) {}
	Matrix_coords(size_t a, size_t b): x(a), y(b) {}
	void set(size_t i, size_t j) { x = i; y = j; }
	size_t get_x() { return x; }
	size_t get_y() { return y; }
	void set_x(size_t a) { x = a; }
	void set_y(size_t a) { y = a; }
};

class Matrix_row_coords {
	size_t x;

public:

	Matrix_row_coords(): x(0) {}
	Matrix_row_coords(size_t a): x(a) {}
	void set(size_t a) { x = a; }
	size_t get_x() { return x; }
	void set_x(size_t a) { x = a; }
};

class Matrix_column_coords {
	size_t y;

public:

	Matrix_column_coords(): y(0) {}
	Matrix_column_coords(size_t a): y(a) {}
	void set(size_t a) { y = a; }
	size_t get_y() { return y; }
	void set_y(size_t a) { y = a; }
};

#endif 
#define MATRIXCOORDS
