#include "memFunc.h"

void* myMalloc(unsigned int size, struct MemDump dump)
{
	unsigned int* result;
	struct MemElem* elem = dump.freeMem;
	
	while(elem->next != NULL && elem->next->size != size)  // поиск подходящего блока памяти в свободных
		elem = elem->next;
	
	if (elem->next != NULL) // если найдено используем найденный блок памяти
	{
		result = (unsigned int*)(elem->next->ptr);
		*(result-1) = size;
		elem->next->ptr = NULL;
		if (elem->next == dump.freeMem->ptr+dump.freeMem->size)
			dump.freeMem->size += sizeof(struct MemElem);
		elem->next = elem->next->next;
	}
	else // если не найдено выделяем из кучи.
	{
		if (size <= dump.freeMem->size)
		{
			result = (unsigned int*)(dump.freeMem->ptr);
			*(result-1) = size;
			dump.freeMem->ptr = (void*)result + size + sizeof(unsigned int);
			dump.freeMem->size -= sizeof(unsigned int) + size;
		}
		else
		{	// нахватает свободного места
			fprintf(stderr, "myMalloc endOfMemori\n");
			exit(-1);
		}
	}
	return (void*)result;
}

void myFree(void* ptr, struct MemDump dump)
{
	// проверка принадлежности указателя куче
	if (ptr > dump.ptr || ptr < dump.freeMem->ptr)
	{
		unsigned int size = *((unsigned int*)ptr - 1);
		//проверка корректности размера указателя
		if (size == 0 || size > dump.freeMem->ptr - sizeof(unsigned int) - ptr)
		{
			printf("myFree SizeError\n");
			exit(-1);
		}
		else
		{
			struct MemElem* newElem = dump.freeMem-1;
			struct MemElem* endOfDump = dump.freeMem->ptr+dump.freeMem->size;
			// ищем пустое место под новый элемент списка
			for(; newElem->ptr != NULL && newElem >= endOfDump ; newElem--);
			// выделяем место для нового участка свободной памяти
			if (newElem < endOfDump && dump.freeMem->size >= sizeof(struct MemElem))
			{	
				dump.freeMem->size -= sizeof(struct MemElem);
			}
			if (newElem < endOfDump && dump.freeMem->size <= sizeof(struct MemElem))
			{
				fprintf(stderr, "myFree OutOfMemory\n");
				exit(-1);
			}
			newElem->ptr = ptr;
			newElem->size = size;
			newElem->next = dump.freeMem->next;
			dump.freeMem->next = newElem;
		}
	}
	else
		{
			printf("myFree PtrError\n");
			exit(-1);
		}
}

struct MemDump memInit(unsigned int size)
{
	struct MemDump dump;
	dump.ptr = malloc(size);
	if (dump.ptr != NULL)
	{
		dump.size = size;
		dump.freeMem = dump.ptr + dump.size - sizeof(struct MemElem);
		dump.freeMem->ptr = dump.ptr + sizeof(unsigned int);
		dump.freeMem->size = dump.size - sizeof(unsigned int) - sizeof(struct MemElem);
		dump.freeMem->next = NULL;
	}
	return dump;
}

void memFree(struct MemDump dump)
{
	free(dump.ptr);
}
