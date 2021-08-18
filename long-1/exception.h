#ifndef EXCEPTION

// Exceptions

#define UNEXPECTED_ERR 0	// Неклассифицированная ошибка
#define OVERFLOW_ERR 1 		// Переполнение
#define MEMORY_ERR 2 		// Ошибка работы с памятью
#define ZERODIV_ERR 3		// Деление на 0 в рациональном числе
#define NULLPTR_ERR 4		// Получен нулевой указатель
#define TRANSFORMATION_ERR 5	// Ошибка преобразования типов
#define VECLEN_ERR 6		// Попытка сложить два вектора разной длины
#define NONEXISTOBJ_ERR 7	// this == NULL
#define BROKENRATNUM_ERR 8	// Неверный формат Rational_number
#define INDEXOUTOFRANGE_ERR 9	// Выход за пределы массива 
#define MATRIXLEN_ERR 10	// Попытка сложить/умножить/вычесть матрицы разных размеров
#define FILE_ERR 11		// Попытка читать/писать пустой/несуществующий/ломаный файл
#define CLASSWRAP_ERR 12	// Ошибка при использовании класса-обертки для матриц

class Exception {
	int code;
	char *msg;
public:
	Exception(int a);
	Exception();
	Exception(int a, const char *b);
	Exception(const Exception &b);
	~Exception();
	int get_code();
	const char *get_msg();
};

#endif
#define EXCEPTION
