class MatrixProxyClass {
	Matrix *matrix;
	Rational_number number;
	uint64_t x;
	uint64_t y;
public:
	MatrixProxyClass(): matrix(NULL), number(0), x(0), y(0) {}
	void set(const Rational_number &a) { number = a; }
	Rational_number get_number() { return number; }
	Matrix *get_matrix() { return matrix; }
	uint64_t get_x() { return x; }
	uint64_t get_y() { return y; }
};
