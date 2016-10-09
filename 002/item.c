#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "others.h"
#include "item.h"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif

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
    WDaysToStr(item->w_days, buffer, BUFFER_SIZE);
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