#ifndef ITEM_H
#define ITEM_H

#include <stdio.h>		//	FILE, fputs(), fgets(), fread(), fwrite(), fflush();
#include <stdlib.h>		//	malloc(), free(), 
#include <string.h>		//	strlen(), strncpy(), strcpy(), memcpy(),
#include <time.h>		//  struct tm, strftime(), time_t, localtime(),

#include "others.h"


struct Item
{
    char* trainNum;
    char* route;
    unsigned char weekDays;
    struct tm* arrivalTime; //время HH:MM\0 тип time
    struct tm* stationTime; //время HH:MM\0
};

//Функции работы с элементом
struct Item* initFromStdin();
struct Item* initFromStr(const char*, const char);
char* itemToStr(const struct Item*, const char separator);

// загрузка из файла сохранение в файл
int itemToBin(struct Item*, FILE*);
struct Item* initFromBin(FILE*);


void showItem(struct Item*, FILE*);
void deleteItem(struct Item*);

#endif