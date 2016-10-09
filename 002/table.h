// Работа с таблицей

struct Table
{
	int col_quantity;			// количесто колонок
	char row_fill_char;			// пустой символ
	char col_seporator_char;	// символ разделения колонок
	char row_seporator_char;	// символ раделения строк
	char crossing_char;			// сивлол пересечения разделителей
	int* cols_size;				// массив с размером колонок
	char* cols_alignment;		// выравниание колонок
	int _row_size;				// размер строки таблицы в байтах
	// int ignore_col_size;		// флаг, игрорировать рамер колонок
};

// создание и удаление таблицы
struct Table* createtable(int col_quantity, int* cols_size, char* cols_alignment);
void deletetable(struct Table* tbl);

// возвращает строку таблицы
char* getrowstr(struct Table* tbl, char* str, const char* cols);
// выводит строку таблицы в файл
void fputrow(struct Table* tbl, const char* cols, FILE*);
// возвращает строку разделитель 
char* getseparatorrow(struct Table* tbl, char* str);
// выводит строку разделитель в файл
void fputseparatorrow(struct Table* tbl, FILE* F);
// служебная функция. делает пустую строку
char* _setemprypow(struct Table* tbl, char* str);
// служебная функция. возвращает смещение от разделителя строки в колонке
int _getalignedpos(int col_size, int alignment, const char* str);
// служебная функция. возвращает количество символов которое будет отображено на экране
int _mbstrlen(const char* str);