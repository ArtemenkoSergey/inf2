

// функции для рабаты с днями недели


unsigned char StrToWDays(char*);
unsigned char isWeekDay(wchar_t*);
int WDaysToStr(unsigned char wd, char* str, int len);

//Работа со временем
int TimeFromStr(struct tm*, char*);

// вспомогательные функции

// ввод имени файла со стандартного ввода
// входные параметры:
//      указатель на строку в которую будет записано имя файла
// возвращает: указатель на строку с именем файла
char* InputFileName(char*);

// длинна строки с символах на экране (UTF-8)
// входные параметры:
//      строка
//возвращает: количество символов которое будет отображено на экране
int mbstrlen(const char*);


char* IntToStr(char* str, int num);