// Работа с таблицей

#ifndef TABLE_H
#define TABLE_H value

#include <stdio.h>		// fputs(), 
#include <stdlib.h>		// malloc(), free(), mblen()
#include <string.h>		// memcpy(), memset(), strlen(), strncpy(), 
#include <stdarg.h>

struct Table
{
	int colQuantity;			// количесто колонок
	char rowFillChar;			// пустой символ
	char colSeporatorChar;	// символ разделения колонок
	char rowSeporatorChar;	// символ раделения строк
	char crossingChar;			// сивлол пересечения разделителей
	int* colsSize;				// массив с размером колонок
	char* colsAlignment;		// выравниание колонок
	int _rowSize;				// размер строки таблицы в байтах
	// int ignore_col_size;		// флаг, игрорировать рамер колонок
};

enum TABLE_ERRORS
{
	NO_ERROR = 0,
	SYMBOL_TYPE_ERROR = -256

};

// создание и удаление таблицы
struct Table* createTable(int colQuantity, int* colsSize, char* colsAlignment);
void deleteTable(struct Table* tbl);

// возвращает строку таблицы
char* getRow(struct Table* tbl, int colsCount, ...);
// выводит строку таблицы в файл
// void fputRow(struct Table* tbl, const char* cols, FILE*);
void fputRow(struct Table* tbl, FILE* fd, int colsCount, ...);
// возвращает строку разделитель 
char* getSeparatorRow(struct Table* tbl, char* str);
// выводит строку разделитель в файл
void fputSeparatorRow(struct Table* tbl, FILE* fd);

// служебная функция. возвращает смещение от разделителя строки в колонке
int _getAlignedPos(int col_size, int alignment, const char* str);
// служебная функция. возвращает количество символов которое будет отображено на экране
int _mbStrLen(const char* str);

// служебная функция. позиционенует и записывает текст колонки в строку таблицы
// возщвращает указатель на следующий разделитель колонок.
char* _setCol(struct Table* tbl, int colNum, char* row, char* sp, const char* colText);
#endif