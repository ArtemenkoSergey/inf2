#ifndef OTHERS_H
#define OTHERS_H 

#include <stdio.h>		// fprintf(), fgets()
#include <stdlib.h>		// malloc(), free(), mbstowcs(), wcstombs(), atoi(), mblen(), 
#include <string.h>		// strlen(), strcpy(), strncpy(),
#include <wchar.h>		// wchar_t, towupper(), wcscmp(), wcslen()
#include <wctype.h>		// iswalpha()
#include <time.h>		// struct tm, 
#include <ctype.h>		// isdigit()

// функции для рабаты с днями недели


unsigned char strToWeekDays(char*);
unsigned char isWeekDay(wchar_t*);

//ПЕРЕДЕЛАТЬ в char* weekDayToStr(unsigned char weekDay);
char* weekDaysToStr(unsigned char weekDays);

//Работа со временем
struct tm* strToTime(const char* str);

// вспомогательные функции

// ввод имени файла со стандартного ввода
// входные параметры:
//      указатель на строку в которую будет записано имя файла
// возвращает: указатель на строку с именем файла
char* inputFileName(char*);

// длинна строки с символах на экране (UTF-8)
// входные параметры:
//      строка
//возвращает: количество символов которое будет отображено на экране
int mbStrLen(const char*);


char* intToStr(char* str, int num);
#endif