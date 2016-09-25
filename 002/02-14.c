/*Определить структурированный тип, определить набор функций для работы с массивом структур.
В  структурированной переменной предусмотреть способ отметки ее как не содержащей данных (т.е. "пустой").
Функции должны работать с массивом структур или с отдельной структурой через указатели,
а также при необходимости возвращать указатель на структуру. В перечень функций входят:
    + «очистка» структурированных переменных;
    + поиск свободной структурированной переменной;
    + ввод элементов (полей) структуры с клавиатуры;
    + вывод элементов (полей) структуры с клавиатуры;
    - поиск в массиве структуры и минимальным значением  заданного поля;
    - сортировка массива  структур в порядке возрастания заданного поля
        сортировке можно использовать тот факт, что в Си++ разрешается присваивание структурированных переменных);
    - поиск в массиве структур элемента с заданным значением поля или с наиболее близким к нему по значению. 
    + удаление заданного элемента;
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
#include <ctype.h>



const wchar_t RuWeekDays[8][3] = {L"ПН",L"ВТ",L"СР",L"ЧТ",L"ПТ",L"СБ",L"ВС",L""};
const wchar_t EnWeekDays[8][3] = {L"MO",L"TU",L"WE",L"TH",L"FR",L"SA",L"SU",L""};

#define FILENAMEMAXLEN 256
char filename[FILENAMEMAXLEN] = "savedarray.txt";
#define BUFFER_SIZE 1024
// char buffer[BUFFER_SIZE];


struct Array
{
    int len;
    // int use;
    struct Item ** items;
};

struct Item
{
    char* train_num;
    char* dst;
    unsigned char w_days;
    struct tm* arrival_time; //время HH:MM\0 тип time
    struct tm* station_time; //время HH:MM\0
};

// Функции работы с массивом
int ArrayInit(struct Array* A, int len);
int ArrayClear(struct Array*);
int FindFree(struct Array*);
int AddtoArray(struct Array*, struct Item*);
int ArrayAddFromFile(struct Array* A, FILE* F);
int DeleleFromArray(struct Array*, int index);
void ArrayShow(struct Array*, FILE*);
void ArrayTableShow(struct Array*, FILE*);

int ArraySave(struct Array*, FILE*);
int ArrayInitFromFile(struct Array*, FILE*);

//Функции работы с элементом
struct Item* InitFromStdin();
char* ItemToString(struct Item*, char*, size_t);
struct Item* InitFromStr(char*);

int ShowItem(struct Item*, FILE*);
int DeleteItem(struct Item*);

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

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.utf8");

    // char* str = "фычс";
    // printf("%d %d\n", mbstrlen(str), strlen(str));

    // int colwidth[6] = {1, 0, 0, 2, 5, 5};
    // const char* lineformat = "+-%*s-+-%*s-+-%*s-+-%*s-+-%*s-+-%*s-+\n";
    // printf(lineformat, 5, "-", 5, "-", 5, "", 5, "", 6, "", 6, "");
    
    struct Array* A = (struct Array*)malloc(sizeof(struct Array));
    ArrayInitFromFile(A,NULL);
    // ArrayShow(A, stdout);
    ArrayAddFromFile(A, NULL);
    ArrayTableShow(A, stdout);
    

    ArrayClear(A);
    free(A);
    return 0;
}

int ArrayInit(struct Array* A, int len)
{
    A->len = len;
    A->items = (struct Item**)malloc(sizeof(struct Item*)*len);
    if(A->items)
    {
        for (int i = 0; i < len; ++i)
            A->items[i] = NULL;
        return 0;
    }
    return -1;
}

int ArrayInitFromFile(struct Array* A, FILE* F)
{
    int fileopen = 0;
    int err = 0;

    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    
    if (F==NULL)
    {   
        fileopen = 1;
        F = fopen(InputFileName(filename), "r");
        // printf("file opend %s\n", filename);
    }
    // fgets(buffer,BUFFER_SIZE,F);
    // printf("%s\n", buffer);
    A->len = atoi(fgets(buffer,BUFFER_SIZE,F));
    // printf("%d\n", A->len);

    A->items = (struct Item**)malloc(sizeof(struct Item*)*A->len);
    if(A->items)
    {
        for (int i = 0; i < A->len && !feof(F); ++i)
        {
            fgets(buffer,BUFFER_SIZE,F);
            if(buffer[0]=='\n')
                A->items[i] = NULL;
            else
                A->items[i] = InitFromStr(buffer);
        }   
        err = 0;
    }

    if (fileopen)
        err = fclose(F);

    free(buffer);
    return err;
}
int ArrayClear(struct Array* A)
{
    int e=0;
    for (int i = 0; i < A->len; ++i)
    {
        if (A->items[i])
            e = e & DeleteItem(A->items[i]);
    }
    return e;
}

int FindFree(struct Array* A)
{
    int i = 0;
    while (i < A->len && A->items[i]!=NULL)
        i++;
    if (i < A->len)
        return i;
    else
        return -1;
} 

int AddtoArray(struct Array* A, struct Item* item)
{
    int i = FindFree(A);
    if (i>=0)
        A->items[i] = item;
    return i;
}

int ArrayAddFromFile(struct Array* A, FILE* F)
{
    int fileopen = 0;
    int err = 0;

    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    
    if (F==NULL)
    {   
        fileopen = 1;
        F = fopen(InputFileName(filename), "r");
        // printf("file opend %s\n", filename);
    }
    fgets(buffer, BUFFER_SIZE, F);
    int i;
    while((i=FindFree(A))>=0 && !feof(F))
    {   
        fgets(buffer,BUFFER_SIZE,F);
        if(buffer[0]!='\n')
            A->items[i] = InitFromStr(buffer);
    }  

    err = !feof(F);
    if (fileopen)
        fclose(F); 
    return err;
}

int DeleteFromArray(struct Array* A, int index)
{
    if (A->items[index]!=NULL)
        DeleteItem(A->items[index]);
    else
        return -1;
    return 0;
}
void ArrayShow(struct Array* A, FILE* F)
{
    for (int i = 0; i < A->len; ++i)
    {
        if (A->items[i])
            ShowItem(A->items[i], F);
        else
            fprintf(F, "%02d пустой элемент\n", i);
    }
}

void ArrayTableShow(struct Array* A, FILE* F)
{
    const char* empryitem = "Пустой элемент";
    const char* title[6] = {"", "Номер поезда", "Пункт назначения", "Дни следования", "Время прибытия", "Время стоянки"};
    int cw[6];
    for (int i = 0; i < 6; ++i)
        cw[i] = mbstrlen(title[i]);
    int tmp = 0;
    int len;

    // ширина первого стролбца
    for (int i = A->len-1; i > 0; i/=10, tmp++);
    if (tmp>cw[0])
        cw[0] = tmp;
    for (int i = 0; i < A->len; ++i)
    {
        if (A->items[i]!=NULL)
        {
            tmp = mbstrlen(A->items[i]->train_num);
            if (tmp > cw[1])
                cw[1] = tmp;
            tmp = mbstrlen(A->items[i]->dst);
            if (tmp > cw[2])
                cw[2] = tmp;
            tmp = 0;
            for (int k = 1; k < 0x80; k = k << 1)
                if (k & A->items[i]->w_days)
                    tmp++;
            tmp = tmp*3-1;
            if (tmp>cw[3])
                cw[3] = tmp;
        }
    }

    tmp = cw[1]+cw[2]+3;
    int d = mbstrlen(empryitem);
    if (tmp < d)
    {
        d = d - tmp;
        cw[1] += d/2;
        cw[2] += d/2 + d%2;
    }

    tmp = 5;
    for (int i = 0; i < 6; ++i)
        tmp+=cw[i]+3;
    char* rowseporator = (char*)malloc(sizeof(char)*tmp);
    char* p = rowseporator;
    *(p++) = '+';
    for (int i = 0; i < 6; ++i)
    {
        *(p++) = '-';
        for (int k = 0; k < cw[i]; ++k)
            *(p++) = '-';
        *(p++) = '-';
        *(p++) = '+';
    }
    *(p++) = '\n';
    *(p++) = '\0';

    
    char* empryline = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    p = empryline;
    *(p++) = '|';
    for (int k = 0; k < cw[0]+2; ++k)
            *(p++) = ' ';
    *(p++) = '|';
    *(p++) = ' ';
    tmp = strlen(empryitem);
    strncpy(p, empryitem, tmp);
    p+=tmp;
    for (int k = 0; k < cw[1]+cw[2]-mbstrlen(empryitem)+4; ++k)
            *(p++) = ' ';
    *(p++) = '|';
    for (int i = 3; i < 6; ++i)
    {
        *(p++) = ' ';
        for (int k = 0; k < cw[i]; ++k)
            *(p++) = ' ';
        *(p++) = ' ';
        *(p++) = '|';
    }
    *(p++) = '\n';
    *(p++) = '\0';



    char* rowline = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    // составление троки шапки
    p = rowline;
    *(p++) = '|';
    
    for (int i = 0; i < 6; ++i)
    {
        len = strlen(title[i]);
        tmp = mbstrlen(title[i]);
        d = (cw[i]-tmp)/2 + (cw[i]-tmp)%2;
        for (int k = 0; k < d+1; ++k)
            *(p++) = ' ';
        d -= (cw[i]-tmp)%2;
        strncpy(p, title[i], len);
        p+=len;
        for (int k = 0; k < d+1; ++k)
            *(p++) = ' ';
        *(p++) = '|';
    }
    *(p++) = '\n';
    *(p++) = '\0';     


    // char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    // const char* fline = "| %*d | %*s | %-*s | %*s | %*s | %*s |\n";
    // const char* empryfline = "| %*d | %*s | %*s | %*s | %*s |\n";
    // printf(lineformat, cw[0], "", cw[1], "", cw[2], "", cw[3], "", cw[4], "", cw[5], "");
    char* index = (char*)malloc(sizeof(char)*(cw[0]*2+1));
    char* w_days = (char*)malloc(sizeof(char)*(cw[3]*2+1));
    char* arrival_time = (char*)malloc(sizeof(char)*(cw[4]*2+1));
    char* station_time = (char*)malloc(sizeof(char)*(cw[5]*2+1));

    fputs(rowseporator, F);
    fputs(rowline,F);
    fputs(rowseporator, F);
    for (int i = 0; i < A->len; ++i)
    {   
        IntToStr(index, i);
        if (A->items[i]!=NULL)
        {   
            p = rowline;
            *(p++) = '|';
            *(p++) = ' ';
            len = strlen(index);
            tmp = cw[0] - len;
            for(;tmp>0;tmp--,*(p++)=' ');
            strncpy(p, index, len); 
            p += len;
            
            *(p++) = ' ';
            *(p++) = '|';
            *(p++) = ' ';
            
            len = strlen(A->items[i]->train_num);
            for (int k = 0; k < cw[1]-mbstrlen(A->items[i]->train_num); ++k)
                *(p++) = ' ';
            strncpy(p, A->items[i]->train_num, len);
            p+=len;
            // for (int k = 0; k < cw[1]-mbstrlen(A->items[i]->train_num)+1; ++k)
            //     *(p++) = ' ';
            
            *(p++) = ' ';
            *(p++) = '|';
            *(p++) = ' ';

            len = strlen(A->items[i]->dst);
            strncpy(p, A->items[i]->dst, len);
            p+=len;
            for (int k = 0; k < cw[2]-mbstrlen(A->items[i]->dst)+1; ++k)
                *(p++) = ' ';
            
            *(p++) = '|';
            *(p++) = ' ';


            WDaysToStr(A->items[i]->w_days, w_days, cw[3]*2+1);
            len = strlen(w_days);
            tmp = mbstrlen(w_days);
            d = (cw[3]-tmp)/2 + (cw[3]-tmp)%2;
            for (int k = 0; k < d; ++k)
                *(p++) = ' ';
            d -= (cw[3]-tmp)%2;
            strncpy(p, w_days, len);
            p+=len;
            for (int k = 0; k < d+1; ++k)
                *(p++) = ' ';

            *(p++) = '|';
            *(p++) = ' ';

            strftime(arrival_time, BUFFER_SIZE, "%R", A->items[i]->arrival_time);
            len = strlen(arrival_time);
            tmp = mbstrlen(arrival_time);
            d = (cw[4]-tmp)/2 + (cw[4]-tmp)%2;
            for (int k = 0; k < d; ++k)
                *(p++) = ' ';
            d -= (cw[4]-tmp)%2;
            strncpy(p, arrival_time, len);
            p+=len;
            for (int k = 0; k < d+1; ++k)
                *(p++) = ' ';

            *(p++) = '|';
            *(p++) = ' ';

            strftime(station_time, BUFFER_SIZE, "%R", A->items[i]->station_time);
            len = strlen(station_time);
            tmp = mbstrlen(station_time);
            d = (cw[5]-tmp)/2 + (cw[5]-tmp)%2;
            for (int k = 0; k < d; ++k)
                *(p++) = ' ';
            d -= (cw[5]-tmp)%2;
            strncpy(p, arrival_time, len);
            p+=len;
            for (int k = 0; k < d+1; ++k)
                *(p++) = ' ';

            *(p++) = '|';

            // fprintf(F, fline, cw[0], i, cw[1], A->items[i]->train_num, cw[2], A->items[i]->dst, cw[3], w_days, cw[4], arrival_time, cw[5], station_time);
            *(p++) = '\n';
            *(p++) = '\0';
            fprintf(F, "%s", rowline);
        }
        else
        {            
            // fprintf(F, empryfline, cw[0], i, cw[1]+cw[2]+3, empryitem, cw[3], "", cw[4], "", cw[5], "");
            p = empryline + 2;
            tmp = cw[0] - strlen(index);
            for(;tmp>0;tmp--,*(p++)=' ');
            strncpy(p, index, strlen(index)); 
            fprintf(F, "%s", empryline);
        }
    }

    fputs(rowseporator, F);

    free(w_days);
    free(arrival_time);
    free(station_time);
    free(rowseporator);
    free(rowline);
    free(empryline);
}

int ArraySave(struct Array* A, FILE* F)
{
    int fileopen = 0;
    int err = 0;

    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    
    if (F==NULL)
    {   
        fileopen = 1;
        F = fopen(InputFileName(filename), "w");
        // printf("file opend %s\n", filename);

    }

    
    fprintf(F, "%d\n", A->len);
    for (int i = 0; i < A->len; ++i)
    {
        if (A->items[i])
            fputs(ItemToString(A->items[i], buffer, BUFFER_SIZE), F);
        else
            fputs("\n", F);
    }
    if (fileopen)
        err = fclose(F);
    else
        err = fflush(F);

    free(buffer);
    return err;
}



struct Item* InitFromStdin()
{
    int err;
    struct Item *item = (struct Item*)malloc(sizeof(struct Item));
    int len;
    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    err = 1;
    while (err!=0)
    {
        fputs("введите номер поезда: ", stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        if (buffer[0]=='\n')
            fputs("не введен номер поезда\n", stdout);
        else
            err = 0;

    }
    len = strlen(buffer);
    for (; buffer[len]!='\n'; len--);
    item->train_num = (char*)malloc(sizeof(char)*(len+1));
    strncpy(item->train_num, buffer, len);
    item->train_num[len] = '\0';
    
    err =-1;
    while (err!=0)
    {
        fputs("введите пунтк назначения: ", stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        if (buffer[0]=='\n')
            fputs("не введен пунтк назначения\n", stdout);
        else
            err = 0;
    }
    len = strlen(buffer);
    for (; buffer[len]!='\n'; len--);
    item->dst = (char*)malloc(sizeof(char)*(len+1));
    strncpy(item->dst, buffer, len);
    item->dst[len] = '\0';
    
    err = -1;
    while(err!=0)
    {
        fputs("введите дни следования: ", stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        len = strlen(buffer);
        for (; buffer[len]!='\n'; len--);
        buffer[len]='\0';
        item->w_days = StrToWDays(buffer);
        if (item->w_days == 0)
        {
            fputs("не правильно введенs дни следования\n", stdout);
        }
        else
            err=0;
    }

    // int hour;
    // int minute;
    item->arrival_time = (struct tm*)malloc(sizeof(struct tm));
    err = 1;
    while (err!=0)
    {
        fputs("введите время прибытия (HH:MM): ", stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        err = TimeFromStr(item->arrival_time, buffer);
        if (err)
            fputs("\nне правильно введено время\n", stdout);
    }

    item->station_time = (struct tm*)malloc(sizeof(struct tm));
    err = 1;
    while (err!=0)
    {
        fputs("введите время стоянки (MM): ", stdout);
        // /err = fwscanf(stdin, "%d", &minute);
        fgets(buffer, BUFFER_SIZE, stdin);
        err = TimeFromStr(item->station_time, buffer);
        if (err)
            fputs("\nне правильно введено время\n", stdout);
    }

    free(buffer);
    return item;
}

int DeleteItem(struct Item* item)
{
    free(item->train_num);
    free(item->dst);
    free(item->arrival_time);
    free(item->station_time);
    free(item);
    return 0;
}

int ShowItem(struct Item* item, FILE* F)
{
    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    fputs("\nномер поезда:\t\t", F);
    fputs(item->train_num, F);
    
    fputs("\nпункт назначения:\t", F);
    fputs(item->dst, F);

    fputs("\nдни следования:\t\t", F);
    // ShowWDays(item->w_days, F);
    int err = WDaysToStr(item->w_days, buffer, BUFFER_SIZE);
    fputs(buffer, F);

    fputs("\nвремя прибытия:\t\t", F);
    strftime(buffer, BUFFER_SIZE, "%R", item->arrival_time);
    fputs(buffer, F);

    fputs("\nвремя стоянки:\t\t", F);
    strftime(buffer, BUFFER_SIZE, "%R", item->station_time);
    fputs(buffer, F);

    fputc('\n',F);
    free(buffer);
}

char* ItemToString(struct Item* item, char* str, size_t str_len)
{
    // int pos = 0;
    char* pos = str;
    int len;
    
    char* buffer;
    int buf_size = 22;
    buffer = (char*)malloc(sizeof(char)*buf_size);

    len = strlen(item->train_num);
    memcpy(pos, item->train_num, len);
    pos += len;
    // *(str+pos) = '\t';
    *pos = '\t';
    pos++;

    // printf("%p %d #%s#%d\n", pos, len, str, strlen(str));

    len = strlen(item->dst);
    memcpy(pos, item->dst, len);
    pos += len;
    // *(str+pos) = '\t';
    *pos = '\t';
    pos++;

    // printf("%p %d #%s#%d\n", pos, len, str, strlen(str));

    WDaysToStr(item->w_days, buffer, BUFFER_SIZE);
    len = strlen(buffer);
    strncpy(pos, buffer, len);
    pos += len;
    // *(str+pos) = '\t';
    *pos = '\t';
    pos++;

    // printf("%p %d #%s#%d\n", pos, len, str, strlen(str));

    strftime(buffer, BUFFER_SIZE, "%R", item->arrival_time);
    len = strlen(buffer);
    strncpy(pos, buffer, len);
    pos += len;
    // *(str+pos) = '\t';
    *pos = '\t';
    pos++;

    // printf("%p %d #%s#%d\n", pos, len, str, strlen(str));

    strftime(buffer, BUFFER_SIZE, "%R", item->station_time);
    len = strlen(buffer);
    strncpy(pos, buffer, len);
    pos += len;
    // *(str+pos) = '\n';
    *pos = '\n';
    pos++;

    *pos = '\0';
    free(buffer);

    // printf("%p %d #%s#%d\n", pos, len, str, strlen(str));
    return str;
}

struct Item* InitFromStr(char* str)
{
    struct Item *item = (struct Item*)malloc(sizeof(struct Item));
    int len;
    
    int pos = 0;
    len=0;
    while(str[++len]!='\t');
    item->train_num = (char*)malloc(sizeof(char)*(len));
    strncpy(item->train_num, str, len);
    item->train_num[len++] = '\0';

    pos+=len;
    str+=len;

    len = 0;
    while(str[++len]!='\t');
    item->dst = (char*)malloc(sizeof(char)*(len));
    strncpy(item->dst, str, len);
    item->dst[len++]='\0';

    pos+=len;
    str+=len;

    len = 0;
    while(str[++len]!='\t');
    str[len]='\0';
    item->w_days = StrToWDays(str);
    str[len]='\t';
    pos+=len;
    str+=len;

    len = 0;
    item->arrival_time = (struct tm*)malloc(sizeof(struct tm));
    while(str[++len]!='\t');
    str[len]='\0';
    TimeFromStr(item->arrival_time, str);
    str[len]='\t';

    pos+=len;
    str+=len;

    len = 0;
    item->station_time = (struct tm*)malloc(sizeof(struct tm));
    while(str[++len]!='\n');
    str[len]='\0';
    TimeFromStr(item->station_time, str);
    str[len]='\n';

    return item;
}

// Номер поезда, пункт назначения, дни следования, время прибытия, время стоянки.

unsigned char StrToWDays(char* str)
{
    unsigned char r = 0;
    wchar_t* word = NULL;
    int l = strlen(str);
    wchar_t* ws = (wchar_t*)malloc(sizeof(wchar_t)*(l+1));
    // size_t mbstowcs (wchar_t* dest, const char* src, size_t max);
    mbstowcs(ws, str, l);
    for (wchar_t* p = ws; *p!=L'\0'; p++)
        *p = towupper(*p);
    // size_t wcstombs (char* dest, const wchar_t* src, size_t max);
    // wcstombs(str, ws,l);
    
    // wprintf("###############\n");
    for (wchar_t *p = ws; ; p++)
    {
        // printf("%p \"%1c\" %s\n", p, *p, str);
        
        if (word)
        {
            if (!iswalpha(*p))
            {
                char t = *p;
                *p = '\0';
                r = r | isWeekDay(word);
                *p = t;
                word = NULL;
            }
        }
        else
        {
            if (iswalpha(*p))
                word = p;
        }
        if (*p=='\0') break;
    }

    // wcstombs(str, ws,l);
    free(ws);
    return r;
}

unsigned char isWeekDay(wchar_t* ws)
{
    unsigned char r = 0;
    unsigned char m = 1;
    for (int i = 0; RuWeekDays[i][0]!='\0' && r==0; ++i)
    {
        // fputs(RuWeekDays[i], stdout);
        if (0==wcscmp(ws, RuWeekDays[i]) || 0==wcscmp(ws, EnWeekDays[i]))
        {
            r = r | m;
            // fputs(" OK", stdout);
        }
        m = m << 1;
        // printf("%02X \n",r);
    }
    // printf("is week day %02X \n",r);
    return r;
}

int WDaysToStr(unsigned char wd, char* str, int len)
{
    // char* p = str;
    int l = 0;
    int i = 0;
    wd = wd & (~0x80);
    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    for (unsigned char m = 1; m < 0x80 && len>=3; i++, m=m << 1)
    {
        if (wd & m)
        {
            l = wcslen(RuWeekDays[i]);
            wcstombs(buffer, RuWeekDays[i], l*3);
            l = strlen(buffer);
            strncpy(str, buffer, l);
            str += l;
            *(str++) = ' ';
            len -= l+1;
            wd = wd & (~m);
        }
    }
    *(--str) = '\0';
    free(buffer);
    if (wd)
        return -1;
    else
        return 0;
    
}

int TimeFromStr(struct tm* tm, char* str)
{
    // char tmp[6] = {0};
    int t[2] = {0};
    char* digit = NULL;
    char tmp;
    int c;
    for (c=0; c<2; str++)
    {
        if (digit)
        {
            if (!isdigit(*str))
            {   
                tmp = *str;
                *str = '\0';
                t[c++] = atoi(digit);
                *str = tmp;
                digit = NULL;
            }
        }
        else
        {
            if (isdigit(*str))
                digit = str;    
        }
        if (*str=='\0') break;   
    }
    if (c==1)
    {
        t[1] = t[0];
        t[0] = 0;
    }

    if (c<1 || t[0]<0 || t[0]>23 || t[1]<0 || t[1]>59)
        return -1;
    
    tm->tm_hour = t[0];
    tm->tm_min = t[1];

    return 0;
}

char* InputFileName(char* name)
{
    
    // ClearStdin();
    // printf("1################\n");
    // fgets(buf, FILENAMEMAXLEN, stdin);

    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);

    fprintf(stdout, "введите имя файла (%s): ", name);
    
    fgets(buffer, BUFFER_SIZE, stdin);
    int i = strlen(buffer);
    // printf("%d %02X %s\n", i, buffer[i], buffer);
    while (buffer[i]!='\n' && i>0) i--;
    buffer[i]='\0';
    // printf("%d %s\n", i, buffer);
    if (buffer[0]!='\0')
        strncpy(name, buffer, i);
    
    free(buffer);
    return name;
}

int mbstrlen(const char* str)
{
    int c = 0;
    int l = 1;
    while(*str!='\0' && l>0)
    {
        l = mblen(str, 6);
        str += l;
        c++;
    }
    if (l>0)
        return c;
    else
        return l;
}

char* IntToStr(char* str, int num)
{
    int l = 1;
    char* p = str;
    for (int i = num; i > 10; i/=10, l*=10);
    for  (; l>0 ; l/=10)
    {
        *(p++) = num/l+'0';
         num=num%l;
    }
    *p = '\0';
    return str;
}