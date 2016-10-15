#ifndef ARRAY_H
#define ARRAY_H


#include <stdio.h>		//	FILE, fopen(), fprintf(),
#include <stdlib.h>		//	malloc(), free(), 
#include <string.h>		//
#include <time.h>		//	time_t, mktime()



#include "item.h"
#include "others.h"
#include "table.h"

// структура массива
struct Array
{
    int lenght;
    // int use;
    struct Item ** items;
};



#define	NO_ERROR  0
#define	FILE_IS_NULL_ERROR -1
#define	FILE_WRITE_ERROR -2
#define	ITEM_NOT_FAUND -1
#define	EMPRY_ITEM_DELETE -2


// Функции работы с массивом
struct Array* initArray(int lenght);
void clearArray(struct Array*);
int findFree(struct Array*);
int addToArray(struct Array*, struct Item*);

struct Array* initArrayFromFile(FILE*);
int saveArrayToFile(struct Array*, FILE*);
struct Array* importFromFile(FILE*);

int deleleFromArray(struct Array*, int index);
void showArray(struct Array*, FILE*);
void tableShowArray(struct Array*, FILE*);

void deleteArray(struct Array* A);

// поиск бидайшего по значению
int findNearestTrainNum(struct Array* A, int startPos, const char* str);
int findNearestRoute(struct Array* A, int startPos, const char* str);

int findNearestArrivalTime(struct Array* A, int startPos, const char* str);


// размещает все не пустые элементы в начале массива
void rePosArraiItems(struct Array* A);

// поиск минимального
int findMinTrainNum(struct Array* A, int startPos);
int findMinRoute(struct Array* A, int startPos);
int findMinArrivalTime(struct Array* A, int startPos);
int findMinStationTime(struct Array* A, int startPos);

// сортировка
void sortByTrainNum(struct Array* A);
void sortByRoute(struct Array* A);
void sortByArrivalTime(struct Array* A);


#endif