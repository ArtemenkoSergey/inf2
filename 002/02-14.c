/*Определить структурированный тип, определить набор функций для работы с массивом структур.
В  структурированной переменной предусмотреть способ отметки ее как не содержащей данных (т.е. "пустой").
Функции должны работать с массивом структур или с отдельной структурой через указатели,
а также при необходимости возвращать указатель на структуру. В перечень функций входят:
    - «очистка» структурированных переменных;
    - поиск свободной структурированной переменной;
    - ввод элементов (полей) структуры с клавиатуры;
    - вывод элементов (полей) структуры с клавиатуры;
    - поиск в массиве структуры и минимальным значением  заданного поля;
    - сортировка массива  структур в порядке возрастания заданного поля
        сортировке можно использовать тот факт, что в Си++ разрешается присваивание структурированных переменных);
    - поиск в массиве структур элемента с заданным значением поля или с наиболее близким к нему по значению. 
    - удаление заданного элемента;
    - изменение (редактирование) заданного элемента.
    - вычисление с проверкой и использованием всех элементов массива по заданному условию и формуле
        (например, общая сумма на всех счетах) -  дается индивидуально.

14. Номер поезда, пункт назначения, дни следования, время прибытия, время стоянки.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// коды ошибок
#define ERROR_NONE      0x0000
#define ERROR_MEMORY_ERR   0x0001

enum WeekDays
{
    Mon = 0x01,
    Tue = 0x02,
    Wed = 0x04,
    Thu = 0x08,
    Fri = 0x10,
    Sat = 0x20,
    Sun = 0x40
};

struct Array
{
    int len;
    // int use;
    struct Item ** data;
};

struct Item
{
    char* train_num;
    char* dst;
    char w_days;
    char arrival_time[6]; //время HH:MM\0 тип time
    char station_time[6]; //время HH:MM\0
    // int arrival_time; //время в секундах
    // int station_time; //время в секундах
};

// Функции работы с массивом
int ArrayInit(struct Array* A, int len);
int ArrayClear(struct Array*);
int FindFree(struct Array*);
int AddtoArray(struct Array*, struct Item*);
int DelfromArray(struct Array*, int index);

int SaveArray(struct Array*, FILE*);
int LoadArray(struct Array*, FILE*);

//Функции работы с элементом
struct Item* InitFromStdin();
struct Item* InitFromString(char*);
char* ItemToString(struct Item*);

int ShowItem(struct Item*, FILE*);
int DeleteItem(struct Item*);

// функции для рабаты с днями недели
int StrToWDays(char*);
void ShowWDays(int, FILE*);

//Работа со временем
// char* TimeToStr(int time);
// int StrToTime(char*);

int main(int argc, char const *argv[])
{
    // struct Array * A = (struct Array*)malloc(sizeof(struct Array));
    // err = ArrayInit(A, 0, 10);
    // free(A);
    struct Item* t = InitFromStdin();
    ShowItem(t, stdout);
    DeleteItem(t);

    return 0;
}

// char* TimeToStr(int time)
// {
//     hour = time/3600;
//     minute = (time-time/3600)%60
// }

int ArrayInit(struct Array* A, int len)
{
    A->len = len;
    A->data = (struct Item**)malloc(sizeof(struct Item*)*len);
    if(A->data)
    {
        for (int i = 0; i < len; ++i)
            A->data[i] = NULL;
        return ERROR_NONE;
    }
    return ERROR_MEMORY_ERR;
}

int ArrayClear(struct Array* A)
{
    int e=0;
    for (int i = 0; i < A->len; ++i)
    {
        if (A->data[i])
            e = e & DeleteItem(A->data[i]);
    }
    return e;
}

int FindFree(struct Array* A)
{
    int i = 0;
    while (i < A->len && A->data[i]!=NULL)
        i++;
    if (i < A->len)
        return i;
    else
        return -1;
} 

int AddtoArray(struct Array* A, struct Item* item)
{
    int i = FindFree(A);
    if (i>0)
        A->data[i] = item;
    return i;
}

struct Item* InitFromStdin()
{
    struct Item *I = (struct Item*)malloc(sizeof(struct Item));
    int len;
    int buf_len = 1024;
    char* buf = (char*)malloc(sizeof(char)*buf_len);
    fputs("введите номер поезда: ", stdout);
    fgets(buf, buf_len, stdin);
   
    len = strlen(buf)+1;
    I->train_num = (char*)malloc(sizeof(char)*len);
    strcpy(I->train_num, buf);
    fputs("введите пунтк назначения: ", stdout);
    fgets(buf, buf_len, stdin);
   
    len = strlen(buf)+1;
    I->dst = (char*)malloc(sizeof(char)*len);
    strcpy(I->dst, buf);
    free(buf);
    return I;
}

int DeleteItem(struct Item* I)
{
    free(I->train_num);
    free(I->dst);
    free(I);
    return 0;
}

int ShowItem(struct Item* I, FILE* F)
{
    fputs("\nномер поезда:\t", F);
    fputs(I->train_num, F);
    fputs("\nпункт назначения:\t", F);
    fputs(I->dst, F);
}
// Номер поезда, пункт назначения, дни следования, время прибытия, время стоянки.

int StrToWDays(char*);
void ShowWDays(int, FILE*);