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
		if (sizeof(unsigned int)+size <= dump.freeMem->size)
		{
			result = (unsigned int*)(dump.freeMem->ptr)+1;
			*(result-1) = size;
			dump.freeMem->ptr = (void*)result + size;
			dump.freeMem->size -= sizeof(unsigned int) + size;
		}
		else
		{	// нехватает свободного места
			fprintf(stderr, "myMalloc endOfMemori\n");
			exit(-1);
		}
	}
	*((char*)result) = 0xAA;
	for(unsigned int i = 1; i<size; i++)
		*((char*)result+i) = 0;
	return (void*)result;
}

enum MemFuncErrors myFree(void* ptr, struct MemDump dump)
{
	enum MemFuncErrors result = OK;
	if (ptr > dump.ptr || ptr < dump.freeMem->ptr)
	{
		unsigned int size = *((unsigned int*)ptr - 1);
		if (size == 0 || size > dump.size)
			// result = SizeError; // ошибка данных
		{
			printf("myFree SizeError\n");
			printf("size %u\n", size);
			exit(-1);
		}
		else
		{
			// слияние с соседним свободным блоком памяти.
			// size += sizeof(unsigned int);
			// ptr = (unsigned int*)ptr - 1;
			// // проверка на свободного соседа
			// for(struct MemElem* elem = dump.freeMem; elem != NULL && ptr != NULL ; elem = elem->next)
			// {
			// 	// сосед с права сободен
			// 	if (ptr+size+sizeof(unsigned int) == elem->ptr)
			// 	{
			// 		elem->ptr = ptr;
			// 		elem->size += size+sizeof(unsigned int);
			// 		ptr = NULL;
			// 	}// сосед с лева свободен
			// 	else if (elem->ptr+elem->size+sizeof(unsigned int) == ptr)
			// 	{
			// 		elem->size += size+sizeof(unsigned int);
			// 		ptr = NULL;
			// 	}
			// }

			if (ptr != NULL)
			{
				struct MemElem* newElem = dump.freeMem-1;
				struct MemElem* endOfDump = dump.freeMem->ptr+dump.freeMem->size;
				// printf("newElem %p, endOfDump %p\n", newElem, endOfDump );
				// ищем пустое место под новый элемент списка
				for(; newElem->ptr != NULL && newElem >= endOfDump ; newElem--);
				// выделяем место для нового участка свободной памяти
				if (newElem < endOfDump && dump.freeMem->size >= sizeof(struct MemElem))
				{	
					dump.freeMem->size -= sizeof(struct MemElem);
				}
				if (dump.freeMem->size <= sizeof(struct MemElem))
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
	}
	else
		{
			printf("myFree PtrError\n");
			exit(-1);
		}
	*((unsigned int*)ptr) = 0xFF;
	return result;
}

struct MemDump memInit(unsigned int size)
{
	struct MemDump dump;
	dump.ptr = malloc(size);
	if (dump.ptr != NULL)
	{
		dump.size = size;
		dump.freeMem = dump.ptr + dump.size - sizeof(struct MemElem);
		dump.freeMem->ptr = dump.ptr;
		dump.freeMem->size = dump.size - sizeof(struct MemElem);
		dump.freeMem->next = NULL;
	}
	return dump;
}

void memFree(struct MemDump dump)
{
	free(dump.ptr);
}
