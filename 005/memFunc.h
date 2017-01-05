#include <stdlib.h>	// malloc(), free().
#include <stdio.h>	// fprintf()

enum MemFuncErrors
{
	OK = 0,
	OutOfMemory = 1,
	SizeError = 2,
	PtrError = 3
};

// структура элемента списка свободных участков памяти.
struct MemElem
{
	void* ptr;
	unsigned int size;
	struct MemElem* next;
};

// структура кучи
struct MemDump
{
	void* ptr; // адрес начала кучи.
	unsigned int size; // размер кучи
	struct MemElem* freeMem; // список свободных участков памяти в куче
};

// функция для выделения памяти
// принимает:	размер выделяемого участка.
//				указатель на кучу в которой выделить память.
// возвращает:	указатель на выдленую память или NULL если выделить память не удалось
void* myMalloc(unsigned int size, struct MemDump); 

// функция освобождения памяти
// принимает:	указатель на выделеный участок памяти.
// возвращает:	код ошибки
enum MemFuncErrors myFree(void* ptr, struct MemDump);

// функция для инициализации кучи
// принимает:	размер участка памяти
//				указатель на кучу в которой совободить память.
// возвращает:	структуру кучи
struct MemDump memInit(unsigned int size);

// функция для освобождения кучи ??
// принимает:	структуру кучи
void memFree(struct MemDump);

// функция возвращающая количество свободных учасков памяти
// принимает:	структуру кучи
// возвращает:	соличество свободных фрагментов памяти в куче
// int getFreeBlockCount(struct MemDump);

// void dumpShow(struct MemDump);
