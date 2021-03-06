#include <stdlib.h>	// malloc(), free().
#include <stdio.h>	// fprintf()


// структура элемента списка свободных участков памяти.
struct MemElem
{
	void* ptr;	// указатель на начало свободного участка памяти
	unsigned int size;	// размер свободного участка памяти
	struct MemElem* next; // указатель на следующий свободный участок
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
// возвращает:	указатель на выделенную память или NULL если выделить память не удалось
void* myMalloc(unsigned int size, struct MemDump); 

// функция освобождения памяти
// принимает:	указатель на выделеный участок памяти.
// в случае ошибки аварийно завершает работу программы
void myFree(void* ptr, struct MemDump);

// функция для инициализации кучи
// принимает:	размер кучи
// возвращает:	структуру кучи
struct MemDump memInit(unsigned int size);

// функция для удаления кучи
// принимает:	структуру кучи
void memFree(struct MemDump);

