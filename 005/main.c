#include "memFunc.h"
#include <stdio.h>		//	printf()
#include <stdlib.h>		//	srand() rand()
#include <time.h>		//	clock_t clock() time()

int main(int argc, char const *argv[])
{
	int N = 0;			// количество выделяемых блоков памяти 
	
	// задаем количества обрабатываемых элементов через командную строку
	if (argc > 1)
		N = atoi(argv[1]);
	if (N <= 0)
		N = 1000;

	const int fixSize = 8;			// размер фиксированного блока памяти
	const int maxVariableSize = 16;	// максимальный размер блока памяти переменного размера
	const int minVariableSize = 4;	// минимальный размер блока памяти переменнго размера

	srand(time(NULL)); // установка генератора псевдослучайных чисел

	clock_t sumAllocTime;	// общее время затраченое на выделение N блоков памяти
	clock_t avgAllocTime;	// среднее время на выделение блока памяти
	clock_t minAllocTime;	// минимальное время на выделение блока памяти
	clock_t maxAllocTime;	// максимальное время на выделение блока памяти

	clock_t sumFreeTime;	// общее время затраченое на освобождение N блоков памяти
	clock_t avgFreeTime;	// среднее время на освобождение блока памяти
	clock_t minFreeTime;	// минимальное время на освобождение блока памяти
	clock_t maxFreeTime;	// максимальное время на освобождение блока памяти

	clock_t sT, dT;			// вспомагательные переменные для измерения времени.
	unsigned int count, sum, avg;	// счетчик, накопитель cуммы, хранилище среднего 
	struct MemElem* freeElem;	// и указатель для подсчета количества свободнх элементов и их суммы

	void** ptrArray = malloc(sizeof(void*)*N);		// массив указателей на выделенные блоки памяти

	clock_t startTime = clock();	// начало работы программы
	struct MemDump mem = memInit(N*maxVariableSize*5);
	
	// работаем с блоками фиксиованного размера
	printf("Тестирование с блоками памяти фиксированного размера\n");
	// устанавливаем начальные значения для сбора статистики.
	sumAllocTime = 0;
	avgAllocTime = 0;
	minAllocTime = 100000;
	maxAllocTime = 0;

	// выделяем память под N блоков фиксированного размера
	for (int i = 0; i < N; ++i)
	{
		sT = clock();
		ptrArray[i] = myMalloc(fixSize, mem);
		dT = clock() - sT;
		// собираем статистику
		sumAllocTime += dT;
		minAllocTime = (minAllocTime>dT)?dT:minAllocTime;
		maxAllocTime = (maxAllocTime<dT)?dT:maxAllocTime;
	}
	// вычисление остальной части статистики
	avgAllocTime = sumAllocTime/N;	
	count = 0;
	freeElem = mem.freeMem->next;
	while(freeElem != NULL)
	{
		count++;
		freeElem = freeElem->next;
	}

	printf("Выделенно %d блоков памяти\n", N);
	printf("Общее время выделения N блоков памяти:\t\t %.5f милисекунд\n", ((double)sumAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Среднее время выделения блока памяти:\t\t %.5f милисекунд\n", ((double)avgAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Минимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)minAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Максимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)maxAllocTime/CLOCKS_PER_SEC)*1000);
	printf("количество свободных фрагментов памяти %d\n", count);
	printf("\n");

	// освобождение и повтороное выделение блоков памяти
	printf("Освобождение и повторное выделение блоков памяти фиксированного размера\n");
	// устанавливаем начальные значения для сбора статистики.
	sumAllocTime = 0;
	avgAllocTime = 0;
	minAllocTime = 100000;
	maxAllocTime = 0;

	sumFreeTime = 0;
	avgFreeTime = 0;
	minFreeTime = 100000;
	maxFreeTime = 0;

	int n;
	for (int i = 0; i < N; ++i)
	{
		// выбираем номер блока памяти с которым будет работать 
		n = rand()%N;

		// освобождаем n-й блок памяти
		sT = clock();
		myFree(ptrArray[n], mem);
		dT = clock() - sT;

		// собираем статистику
		sumFreeTime += dT;
		minFreeTime = (minFreeTime>dT)?dT:minFreeTime;
		maxFreeTime = (maxFreeTime<dT)?dT:maxFreeTime;

		// выделяем еще раз память для этого блока памяти
		sT = clock();
		ptrArray[i] = myMalloc(fixSize, mem);
		dT = clock() - sT;
		// собираем статистику
		sumAllocTime += dT;
		minAllocTime = (minAllocTime>dT)?dT:minAllocTime;
		maxAllocTime = (maxAllocTime<dT)?dT:maxAllocTime;
	}
	// вычисление остальной части статистики
	avgAllocTime = sumAllocTime/N;
	avgFreeTime = sumFreeTime/N;

	sum = 0;
	count = 0;
	freeElem = mem.freeMem->next;
	while(freeElem != NULL)
	{
		count++;
		sum += freeElem->size;
		freeElem = freeElem->next;
	}
	avg = (count==0)?0:sum/count;

	printf("Освобожденно и заново выделенно %d блоков памяти\n", N);
	printf("Общее время выделения N блоков памяти:\t\t %.5f милисекунд\n", ((double)sumAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Среднее время выделения блока памяти:\t\t %.5f милисекунд\n", ((double)avgAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Минимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)minAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Максимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)maxAllocTime/CLOCKS_PER_SEC)*1000);
	printf("\n");
	printf("Общее время освобождения N блоков памяти:\t %.5f милисекунд\n", ((double)sumFreeTime/CLOCKS_PER_SEC)*1000);
	printf("Среднее время освобождения блока памяти:\t %.5f милисекунд\n", ((double)avgFreeTime/CLOCKS_PER_SEC)*1000);
	printf("Минимальное время освобождения блока памяти:\t %.5f милисекунд\n", ((double)minFreeTime/CLOCKS_PER_SEC)*1000);
	printf("Максимальное время освобождения блока памяти:\t %.5f милисекунд\n", ((double)maxFreeTime/CLOCKS_PER_SEC)*1000);
	printf("\n");
	printf("Всего выделено %d байт памяти\n", mem.size);
	printf("Размер свободной части кучи %u\n", mem.freeMem->size);
	printf("Количество свободных фрагментов памяти %d\n", count);
	printf("Фрагментация %.3f %%\n", (double)(count)/N*100);
	printf("Общий размер свободных блоков памяти %d байт\n", sum);
	printf("Средний размер свободного блока памяти %d байт\n", avg);
	printf("\n");

	// освобождаем память и создаем кучу заново
	memFree(mem);
	mem = memInit(N*maxVariableSize*5);

	// работаем с блоками переменного размера
	printf("Тестирование с блоками памяти переменного (случайного) размера\n");
	// устанавливаем начальные значения для сбора статистики.
	sumAllocTime = 0;
	avgAllocTime = 0;
	minAllocTime = 100000;
	maxAllocTime = 0;

	// выделяем память под N блоков переменного размера
	for (int i = 0; i < N; ++i)
	{
		// вычисляем размер выделяемого блока
		int blockSize = minVariableSize + rand()%(maxVariableSize - minVariableSize);
		sT = clock();
		ptrArray[i] = myMalloc(blockSize, mem);
		dT = clock() - sT;
		// собираем статистику
		sumAllocTime += dT;
		minAllocTime = (minAllocTime>dT)?dT:minAllocTime;
		maxAllocTime = (maxAllocTime<dT)?dT:maxAllocTime;
	}
	avgAllocTime = sumAllocTime/N;
	count = 0;
	freeElem = mem.freeMem->next;
	while(freeElem != NULL)
	{
		count++;
		freeElem = freeElem->next;
	}

	printf("Выделенно %d блоков памяти\n", N);
	printf("Общее время выделения N блоков памяти:\t\t %.5f милисекунд\n", ((double)sumAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Среднее время выделения блока памяти:\t\t %.5f милисекунд\n", ((double)avgAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Минимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)minAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Максимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)maxAllocTime/CLOCKS_PER_SEC)*1000);
	printf("количество свободных фрагментов памяти %d\n", count);
	printf("\n");

	// освобождение и повтороное выделение блоков памяти
	printf("Освобождение и повторное выделение блоков памяти переменного (случайного) размера\n");
	// устанавливаем начальные значения для сбора статистики.
	sumAllocTime = 0;
	avgAllocTime = 0;
	minAllocTime = 100000;
	maxAllocTime = 0;

	sumFreeTime = 0;
	avgFreeTime = 0;
	minFreeTime = 100000;
	maxFreeTime = 0;

	for (int i = 0; i < N; ++i)
	{
		// выбираем номер блока памяти с которым будет работать 
		n = rand()%N;
		// освобождаем n-й блок памяти
		sT = clock();
		myFree(ptrArray[n], mem);
		dT = clock() - sT;

		// собираем статистику
		sumFreeTime += dT;
		minFreeTime = (minFreeTime>dT)?dT:minFreeTime;
		maxFreeTime = (maxFreeTime<dT)?dT:maxFreeTime;

		// выделяем еще раз память для этого блока памяти
		// вычисляем размер выделяемого блока
		int blockSize = minVariableSize + rand()%(maxVariableSize - minVariableSize);
		sT = clock();
		ptrArray[n] = myMalloc(blockSize, mem);
		dT = clock() - sT;
		// собираем статистику
		sumAllocTime += dT;
		minAllocTime = (minAllocTime>dT)?dT:minAllocTime;
		maxAllocTime = (maxAllocTime<dT)?dT:maxAllocTime;
	}
	avgAllocTime = sumAllocTime/N;
	avgFreeTime = sumFreeTime/N;

	printf("Освобожденно и заново выделенно %d блоков памяти\n", N);
	printf("Общее время выделения N блоков памяти:\t\t %.5f милисекунд\n", ((double)sumAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Среднее время выделения блока памяти:\t\t %.5f милисекунд\n", ((double)avgAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Минимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)minAllocTime/CLOCKS_PER_SEC)*1000);
	printf("Максимальное время выделения блока памяти:\t %.5f милисекунд\n", ((double)maxAllocTime/CLOCKS_PER_SEC)*1000);
	printf("\n");
	printf("Общее время освобождения N блоков памяти:\t %.5f милисекунд\n", ((double)sumFreeTime/CLOCKS_PER_SEC)*1000);
	printf("Среднее время освобождения блока памяти:\t %.5f милисекунд\n", ((double)avgFreeTime/CLOCKS_PER_SEC)*1000);
	printf("Минимальное время освобождения блока памяти:\t %.5f милисекунд\n", ((double)minFreeTime/CLOCKS_PER_SEC)*1000);
	printf("Максимальное время освобождения блока памяти:\t %.5f милисекунд\n", ((double)maxFreeTime/CLOCKS_PER_SEC)*1000);
	printf("\n");
	printf("Всего выделено %d байт памяти\n", mem.size);
	printf("Размер свободной части кучи %u\n", mem.freeMem->size);
	sum = 0;
	count = 0;
	freeElem = mem.freeMem->next;
	while(freeElem != NULL)
	{
		count++;
		sum += freeElem->size;
		freeElem = freeElem->next;
	}
	avg = (count==0)?0:sum/count;
	printf("Количество свободных фрагментов памяти %d\n", count);
	printf("Фрагментация %.3f %%\n", (double)(count)/N*100);
	printf("Общий размер свободных блоков памяти без кучи %d байт\n", sum);
	printf("Средний размер свободного блока памяти без кучи %d байт\n", avg);

	memFree(mem);
	clock_t endTime = clock(); // конец работы программы
	double elapsed=((double)(endTime-startTime))/CLOCKS_PER_SEC;
	printf("время работы программы %.6f секунд\n", elapsed);
	free(ptrArray);
	return 0;
}