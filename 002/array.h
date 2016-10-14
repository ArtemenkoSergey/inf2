#ifndef ARRAY_H
#define ARRAY_H


#include <stdio.h>		//	FILE, fopen(), fprintf(),
#include <stdlib.h>		//	malloc(), free(), 
#include <string.h>		//
#include <time.h>		//



#include "array.h"
#include "item.h"
#include "others.h"

// структура массива
struct Array
{
    int lenght;
    // int use;
    struct Item ** items;
};

enum FILE_ERRORS
{
	NO_ERROR = 0,
	FILE_IS_NULL_ERROR = -1,
	FILE_WRITE_ERROR = -2

};

// Функции работы с массивом
struct Array* initArray(int lenght);
void clearArray(struct Array*);
int findFree(struct Array*);
int addToArray(struct Array*, struct Item*);

// переделать для бинарного файла
struct Array* initArrayFromFile(FILE*);
int saveArrayToFile(struct Array*, FILE*);
int ArrayInitFromFile(struct Array*, FILE*);
//###################################

int deleleFromArray(struct Array*, int index);
void showArray(struct Array*, FILE*);

void tableShowArray(struct Array*, FILE*);



#endif