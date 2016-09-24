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
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <time.h>

// коды ошибок
#define ERROR_NONE      0x0000
#define ERROR_MEMORY_ERR   0x0001

const wchar_t RuWeekDays[8][3] = {L"ПН",L"ВТ",L"СР",L"ЧТ",L"ПТ",L"СБ",L"ВС",L""};
const wchar_t EnWeekDays[8][3] = {L"MO",L"TU",L"WE",L"TH",L"FR",L"SA",L"SU",L""};
// {
//     Mon = 0x01,
//     Tue = 0x02,
//     Wed = 0x04,
//     Thu = 0x08,
//     Fri = 0x10,
//     Sat = 0x20,
//     Sun = 0x40
// };

struct Array
{
    int len;
    // int use;
    struct Item ** data;
};

struct Item
{
    wchar_t* train_num;
    wchar_t* dst;
    unsigned char w_days;
    struct tm* arrival_time; //время HH:MM\0 тип time
    struct tm* station_time; //время HH:MM\0
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
int ItemToString(struct Item*, wchar_t*, size_t);
struct Item* InitFromStr(wchar_t*);
int ItemToBlok(struct Item* item, unsigned char* buf, int buf_size);
struct Item* InitFromBlok(char*, int size);

int ShowItem(struct Item*, FILE*);
int DeleteItem(struct Item*);

// функции для рабаты с днями недели
unsigned char StrToWDays(wchar_t*);
unsigned char isWeekDay(wchar_t*);
void ShowWDays(unsigned char, FILE*);
int WDaysToStr(unsigned char wd, wchar_t* str, int len);

//Работа со временем
int TimeFromStr(struct tm*, wchar_t*);
// int StrToTime(char*);

// вспомогательные функции

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.utf8");
    // struct Array * A = (struct Array*)malloc(sizeof(struct Array));
    // err = ArrayInit(A, 0, 10);
    // free(A);
    struct Item* t = InitFromStdin();
    ShowItem(t, stdout);

    wchar_t* buf = (wchar_t*)malloc(sizeof(wchar_t)*1024);
    ItemToString(t, buf, 1024);
   
    wprintf(L"%ls",buf);

    struct Item* n =InitFromStr(buf);
    ShowItem(t, stdout);

    DeleteItem(t);
    DeleteItem(n);
    free(buf);
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
    int err;
    struct Item *I = (struct Item*)malloc(sizeof(struct Item));
    int len;
    int buf_len = 1024;
    wchar_t* buf = (wchar_t*)malloc(sizeof(wchar_t)*buf_len);
    err = 1;
    while (err!=0)
    {
        fputws(L"введите номер поезда: ", stdout);
        fgetws(buf, buf_len, stdin);
        if (buf[0]==L'\n')
            fputws(L"не введен номер поезда\n", stdout);
        else
            err = 0;

    }
    len = wcslen(buf);
    for (; buf[len]!=L'\n'; len--);
    I->train_num = (wchar_t*)malloc(sizeof(wchar_t)*(len+1));
    wcsncpy(I->train_num, buf, len);
    
    err =-1;
    while (err!=0)
    {
        fputws(L"введите пунтк назначения: ", stdout);
        fgetws(buf, buf_len, stdin);
        if (buf[0]==L'\n')
            fputws(L"не введен пунтк назначения\n", stdout);
        else
            err = 0;
    }
    len = wcslen(buf);
    for (; buf[len]!=L'\n'; len--);
    I->dst = (wchar_t*)malloc(sizeof(wchar_t)*(len+1));
    wcsncpy(I->dst, buf, len);
    
    err = -1;
    while(err!=0)
    {
        fputws(L"введите дни следования: ", stdout);
        fgetws(buf, buf_len, stdin);
        len = wcslen(buf);
        for (; buf[len]!=L'\n'; len--);
        buf[len]=L'\0';
        I->w_days = StrToWDays(buf);
        if (I->w_days == 0)
        {
            fputws(L"не правильно введенs дни следования\n", stdout);
        }
        else
            err=0;
    }

    int hour;
    int minute;
    I->arrival_time = (struct tm*)malloc(sizeof(struct tm));
    err = 1;
    while (err!=0)
    {
        fputws(L"введите время прибытия (HH:MM): ", stdout);
        fgetws(buf, buf_len, stdin);
        err = TimeFromStr(I->arrival_time, buf);
        if (err)
            fputws(L"\nне правильно введено время\n", stdout);
    }

    I->station_time = (struct tm*)malloc(sizeof(struct tm));
    err = 1;
    while (err!=0)
    {
        fputws(L"введите время стоянки (MM): ", stdout);
        fwscanf(stdin, L"%d", &minute);
        if (minute<=0)
            err = -1;
        else
        {
            hour = minute/60;
            if (hour>23)
                err = -1;
            else
                minute %=60;
            err = 0;

        }
        if (err)
            fputws(L"\nне правильно введено время\n", stdout);
        else
        {
            I->station_time->tm_hour = hour;
            I->station_time->tm_min = minute;
        }

    }

    free(buf);
    return I;
}

int DeleteItem(struct Item* I)
{
    free(I->train_num);
    free(I->dst);
    free(I->arrival_time);
    free(I->station_time);
    free(I);
    return 0;
}

int ShowItem(struct Item* I, FILE* F)
{
    wchar_t* buf;
    int buf_size = 22;
    buf = (wchar_t*)malloc(sizeof(wchar_t)*buf_size);
    fputws(L"\nномер поезда:\t\t", F);
    fputws(I->train_num, F);
 
    fputws(L"\nпункт назначения:\t", F);
    fputws(I->dst, F);

    fputws(L"\nдни следования:\t", F);
    // ShowWDays(I->w_days, F);
    int err = WDaysToStr(I->w_days, buf, buf_size);
    fputws(buf, F);

    fputws(L"\nвремя прибытия:\t", F);
    wcsftime(buf, buf_size, L"%R", I->arrival_time);
    fputws(buf, F);

    fputws(L"\nвремя стоянки:\t", F);
    wcsftime(buf, buf_size, L"%R", I->station_time);
    fputws(buf, F);

    fputwc(L'\n',F);
    free(buf);
}

int ItemToString(struct Item* item, wchar_t* str, size_t str_len)
{
    size_t pos = 0;
    int len;
    
    wchar_t* buf;
    int buf_size = 22;
    buf = (wchar_t*)malloc(sizeof(wchar_t)*buf_size);

    len = wcslen(item->train_num);
    wcsncpy(str+pos, item->train_num, len);
    pos += len;
    *(str+pos) = L'\t';
    pos++;

    len = wcslen(item->dst);
    wcsncpy(str+pos, item->dst, len);
    pos += len;
    *(str+pos) = L'\t';
    pos++;

    WDaysToStr(item->w_days, buf, buf_size);
    len = wcslen(buf);
    wcsncpy(str+pos, buf, len);
    pos += len;
    *(str+pos) = L'\t';
    pos++;

    wcsftime(buf, buf_size, L"%R", item->arrival_time);
    len = wcslen(buf);
    wcsncpy(str+pos, buf, len);
    pos += len;
    *(str+pos) = L'\t';
    pos++;

    wcsftime(buf, buf_size, L"%R", item->station_time);
    len = wcslen(buf);
    wcsncpy(str+pos, buf, len);
    pos += len;
    *(str+pos) = L'\n';
    pos++;

    *(str+pos) = L'\0';
    free(buf);
    return pos;
}

struct Item* InitFromStr(wchar_t* str)
{
    struct Item *item = (struct Item*)malloc(sizeof(struct Item));
    int len;
    
    int pos = 0;
    len=0;
    while(str[++len]!=L'\t');
    item->train_num = (wchar_t*)malloc(sizeof(wchar_t)*(len));
    wcsncpy(item->train_num, str, len-1);

    pos+=len;
    str+=len;

    len = 0;
    while(str[++len]!=L'\t');
    item->dst = (wchar_t*)malloc(sizeof(wchar_t)*(len));
    wcsncpy(item->dst, str, len-1);

    pos+=len;
    str+=len;

    len = 0;
    while(str[++len]!=L'\t');
    str[len]=L'\0';
    item->w_days = StrToWDays(str);
    str[len]=L'\t';

    pos+=len;
    str+=len;

    len = 0;
    item->arrival_time = (struct tm*)malloc(sizeof(struct tm));
    while(str[++len]!=L'\t');
    str[len]=L'\0';
    TimeFromStr(item->arrival_time, str);
    str[len]=L'\t';

    pos+=len;
    str+=len;

    len = 0;
    item->station_time = (struct tm*)malloc(sizeof(struct tm));
    while(str[++len]!=L'\n');
    str[len]=L'\0';
    TimeFromStr(item->station_time, str);
    str[len]=L'\n';

    return item;


}
int ItemToBlok(struct Item* item, unsigned char* buf, int buf_size)
{
    // int buf_size = 1024;
    // unsigned char* buf = (char*)malloc(sizeof(char)*buf_size);
    size_t pos = 0;
    size_t mem = 0;
    // место под длинну блока
    size_t len = 0;
    mem += sizeof(size_t);
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, &len, sizeof(size_t));
    pos = mem;

    // сохраняем поле train_num
    len = wcslen(item->train_num);
    mem += sizeof(size_t);
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, &len, sizeof(size_t));
    pos = mem;

    mem += sizeof(wchar_t)*len;
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, item->train_num, sizeof(wchar_t)*len);
    pos = mem;
 
    // сохраняем поле dst
    len = wcslen(item->dst);
    mem += sizeof(size_t);
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, &len, sizeof(size_t));
    pos = mem;

    mem += sizeof(wchar_t)*len;
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, item->train_num, sizeof(wchar_t)*len);
    pos = mem;

    // сохраняем поле w_days
    mem += sizeof(unsigned char); 
    if (mem > buf_size)
        return -1;
    //memcpy(buf+pos, &(*item.w_days), sizeof(unsigned char));
    *(buf+pos) = item->w_days;
    pos = mem;    
    
    // сохраняем поле arrival_time
    time_t t;
    t = mktime(item->arrival_time);
    mem += sizeof(time_t);
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, &t, sizeof(unsigned char));
    pos = mem;

    // сохраняем поле station_time
    t = mktime(item->station_time);
    mem += sizeof(time_t);
    if (mem > buf_size)
        return -1;
    memcpy(buf+pos, &t, sizeof(unsigned char));
    pos = mem;

    memcpy(buf, &pos, sizeof(size_t));
    return pos;
}

struct Item* InitFromBlok(char* blok, int size)
{
    struct Item* item = malloc(sizeof(struct Item));
    size_t len, s;
    int mem = 0;
    char* p = blok;

    memcpy(&len, p, sizeof(size_t));
    p += sizeof(size_t);

    // mem += sizeof(size_t);
    memcpy(&len, p, sizeof(size_t));
    p += sizeof(size_t);

    // s = sizeof(wchar_t)*len;
    item->train_num = (wchar_t*)malloc(sizeof(wchar_t)*(len+1));
    memcpy(item->train_num, p, sizeof(wchar_t)*len);
    item->train_num[len] = L'\0';
    p+=sizeof(wchar_t)*len;

    memcpy(&len, p, sizeof(size_t));
    p += sizeof(size_t);
    item->dst = (wchar_t*)malloc(sizeof(wchar_t)*(len+1));
    memcpy(item->dst, p, sizeof(wchar_t)*len);
    item->dst[len] = L'\0';
    p+=sizeof(wchar_t)*len;    

    item->w_days = *p;
    p++;

    time_t t;
    memcpy(&t, p, sizeof(time_t));
    item->arrival_time = localtime(&t);
    p+=sizeof(time_t);

    memcpy(&t, p, sizeof(time_t));
    item->station_time = localtime(&t);
    p+=sizeof(time_t);

    return item;
}
// Номер поезда, пункт назначения, дни следования, время прибытия, время стоянки.

unsigned char StrToWDays(wchar_t* str)
{
    unsigned char r = 0;
    wchar_t* word = NULL;

    // wprintf(L"%ls\n",str);
    // wprintf(L"###############\n");
    for (wchar_t *p = str; ; p++)
    {
        // wprintf(L"%p \"%1lc\" %ls\n", p, *p, str);
        *p = towupper(*p);
        if (word)
        {
            if (!iswalpha(*p))
            {
                wchar_t t = *p;
                *p = L'\0';
                // fputws(word, stdout);
                // fputwc(L' ',stdout);

                r = r | isWeekDay(word);
                // wprintf(L"%02X\n",r);
                *p = t;
                word = NULL;
            }
        }
        else
        {
            if (iswalpha(*p))
                word = p;
        }
        if (*p==L'\0') break;
    }
    return r;
}

unsigned char isWeekDay(wchar_t* ws)
{
    unsigned char r = 0;
    unsigned char m = 1;
    for (int i = 0; RuWeekDays[i][0]!=L'\0' && r==0; ++i)
    {
        // fputws(RuWeekDays[i], stdout);
        if (0==wcscmp(ws, RuWeekDays[i]) || 0==wcscmp(ws, EnWeekDays[i]))
        {
            r = r | m;
            // fputws(L" OK", stdout);
        }
        m = m << 1;
        // wprintf(L"%02X \n",r);
    }
    // wprintf(L"is week day %02X \n",r);
    return r;
}
void ShowWDays(unsigned char wd, FILE* F)
{
    wchar_t str[21]={0};
    wchar_t* p = str;
    int i = 0;
    for (unsigned char m = 1; m < 0x80; i++, m=m << 1)
    {
        if (wd & m)
        {
            wcsncpy(p,RuWeekDays[i],2);
            p += 2;
            *(p++) = L' ';
        }
    }
    *p = L'\0';
    fputws(str, F);
}

int WDaysToStr(unsigned char wd, wchar_t* str, int len)
{
    // wchar_t* p = str;
    int i = 0;
    wd = wd & (~0x80);
    for (unsigned char m = 1; m < 0x80 && len>=3; i++, m=m << 1)
    {
        if (wd & m)
        {
            wcsncpy(str, RuWeekDays[i],2);
            str += 2;
            *(str++) = L' ';
            len -= 3;
            wd = wd & (~m);
        }
    }
    *str = L'\0';
    if (wd)
        return -1;
    else
        return 0;
    
}

int TimeFromStr(struct tm* tm, wchar_t* str)
{
    // char tmp[6] = {0};
    int t[2] = {0};
    wchar_t* digit = NULL;
    wchar_t** p;
    int c;
    for (c=0; c<2; str++)
    {
        if (digit)
        {
            if (!iswdigit(*str))
            {
                t[c++] = wcstoll(digit, p, 10);
                digit = NULL;
            }
        }
        else
        {
            if (iswdigit(*str))
                digit = str;    
        }
        if (*str==L'\0') break;   
    }
    // if (c<2)
    //     return -1;
    if (c<2 || t[0]<0 || t[0]>23 || t[1]<0 || t[1]>59)
        return -1;
    tm->tm_hour = t[0];
    tm->tm_min = t[1];
    return 0;
}