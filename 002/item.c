#include "item.h"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif

struct Item* initFromStdin()
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
    item->trainNum = (char*)malloc(sizeof(char)*(len+1));
    strncpy(item->trainNum, buffer, len);
    item->trainNum[len] = '\0';
    
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
    item->route = (char*)malloc(sizeof(char)*(len+1));
    strncpy(item->route, buffer, len);
    item->route[len] = '\0';
    
    err = -1;
    while(err!=0)
    {
        fputs("введите дни следования: ", stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        len = strlen(buffer);
        for (; buffer[len]!='\n'; len--);
        buffer[len]='\0';
        item->weekDays = strToWeekDays(buffer);
        if (item->weekDays == 0)
        {
            fputs("не правильно введенs дни следования\n", stdout);
        }
        else
            err=0;
    }

    // int hour;
    // int minute;
    item->arrivalTime = NULL;
    while (item->arrivalTime == NULL)
    {
        fputs("введите время прибытия (HH:MM): ", stdout);
        fgets(buffer, BUFFER_SIZE, stdin);
        item->arrivalTime = strToTime(buffer);
        if (item->arrivalTime == NULL)
            fputs("\nне правильно введено время\n", stdout);
    }

    item->stationTime = NULL;
    while (item->stationTime == NULL)
    {
        fputs("введите время стоянки (MM): ", stdout);
        // /err = fwscanf(stdin, "%d", &minute);
        fgets(buffer, BUFFER_SIZE, stdin);
        item->stationTime = strToTime(buffer);
        if (item->stationTime == NULL)
            fputs("\nне правильно введено время\n", stdout);
    }

    free(buffer);
    return item;
}

void deleteItem(struct Item* item)
{
    free(item->trainNum);
    free(item->route);
    free(item->arrivalTime);
    free(item->stationTime);
    free(item);
}

void showItem(struct Item* item, FILE* F)
{
    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    fputs("\nномер поезда:\t\t", F);
    fputs(item->trainNum, F);
    
    fputs("\nпункт назначения:\t", F);
    fputs(item->route, F);

    fputs("\nдни следования:\t\t", F);
    // ShowWDays(item->weekDays, F);
    char* weekDays = weekDaysToStr(item->weekDays);
    fputs(weekDays, F);
    free(weekDays);

    fputs("\nвремя прибытия:\t\t", F);
    strftime(buffer, BUFFER_SIZE, "%R", item->arrivalTime);
    fputs(buffer, F);

    fputs("\nвремя стоянки:\t\t", F);
    strftime(buffer, BUFFER_SIZE, "%R", item->stationTime);
    fputs(buffer, F);

    fputc('\n',F);
    free(buffer);
}

char* itemToStr(const struct Item* item, const char separator)
{
    char* buffer;
    int bufferSize = 25;
    buffer = (char*)malloc(sizeof(char)*bufferSize);

    // подсчитываем длинну строки.
    int size = 0;

    size += strlen(item->trainNum) + 1;
    size += strlen(item->route)+1;
    
    char* weekDays = weekDaysToStr(item->weekDays);

    size += strlen(weekDays)+1;

    strftime(buffer, BUFFER_SIZE, "%R", item->arrivalTime);
    int lenght = strlen(buffer);
    *(buffer+lenght) = separator;
    strftime(buffer+lenght+1, BUFFER_SIZE, "%R", item->stationTime);

    size += strlen(buffer)+1;

    char* result = (char*)malloc(sizeof(char)*size);
    char* pos = result;

    strcpy(pos, item->trainNum);
    pos += strlen(pos);
    *(pos++) = separator;

    strcpy(pos, item->route);
    pos += strlen(pos);
    *(pos++) = separator;

    strcpy(pos, weekDays);
    pos += strlen(pos);
    *(pos++) = separator;

    strcpy(pos, buffer);
    pos += strlen(pos);
    *(pos++) = '\0';

    free(weekDays);
    free(buffer);
    return result;
}     

struct Item* initFromStr(const char* str, const char separator)
{
    struct Item *item = (struct Item*)malloc(sizeof(struct Item));
    int len;
    
    //int pos = 0;
    len=0;
    while(str[++len]!=separator);
    item->trainNum = (char*)malloc(sizeof(char)*(len));
    strncpy(item->trainNum, str, len);
    item->trainNum[len++] = '\0';

    //pos+=len;
    str+=len;

    len = 0;
    while(str[++len]!=separator);
    item->route = (char*)malloc(sizeof(char)*(len));
    strncpy(item->route, str, len);
    item->route[len++]='\0';

    //pos+=len;
    str+=len;

    len = 0;
    while(str[++len]!=separator);
    char* buffer = (char*)malloc(sizeof(char)*(BUFFER_SIZE));
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    item->weekDays = strToWeekDays(buffer);
   
    // pos+=len;
    str+=len;

    len = 0;
    item->arrivalTime = (struct tm*)malloc(sizeof(struct tm));
    while(str[++len]!=separator);
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    item->arrivalTime = strToTime(str);
    

    //pos+=len;
    str+=len;

    len = 0;
    item->stationTime = (struct tm*)malloc(sizeof(struct tm));
    while(str[++len]!='\n');
    strncpy(buffer, str, len);
    buffer[len] = '\0';
    item->stationTime = strToTime(str);
    
    free(buffer);
    return item;
}

struct Item* initFromBin(FILE* F)
{
    size_t readCount = 0;

    struct Item* item = (struct Item*)malloc(sizeof(struct Item));
    int size;

    readCount += fread(&size, sizeof(int), 1, F);
    item->trainNum = (char*)malloc(sizeof(char)*(size+1));
    readCount += fread(item->trainNum, size, 1, F);
    item->trainNum[size] = '\0';

    readCount += fread(&size, sizeof(int), 1, F);
    item->route = (char*)malloc(sizeof(char)*(size+1));
    readCount += fread(item->route, size, 1, F);
    item->route[size] = '\0';

    readCount += fread(&(item->weekDays), sizeof(unsigned char), 1, F);

    item->arrivalTime = (struct tm*)malloc(sizeof(struct tm));
    time_t tTime = 0;
    readCount += fread(&tTime, sizeof(time_t), 1, F);
    memcpy(item->arrivalTime, localtime(&tTime), sizeof(struct tm));

    tTime = 0;
    item->stationTime = (struct tm*)malloc(sizeof(struct tm));
    readCount += fread(&tTime, sizeof(time_t), 1, F);
    memcpy(item->stationTime, localtime(&tTime), sizeof(struct tm));

    if (readCount == 7)
        return item;

    deleteItem(item);
    return NULL;

}

int itemToBin(struct Item* item, FILE* F)
{
    int writeCount = 0;
    
    int size = strlen(item->trainNum);
    writeCount += fwrite(&size, sizeof(int), 1, F);
    writeCount += fwrite(item->trainNum, size, 1, F);


    size = strlen(item->route);
    writeCount += fwrite(&size, sizeof(int), 1, F);
    writeCount += fwrite(item->route, size, 1, F);

    writeCount += fwrite(&(item->weekDays), sizeof(unsigned char), 1, F);

    time_t tTime = mktime(item->arrivalTime);
    writeCount += fwrite(&tTime, sizeof(time_t), 1, F);

    tTime = mktime(item->stationTime);
    writeCount += fwrite(&tTime, sizeof(time_t), 1, F);
    fflush(F);
    return (writeCount==7);
}   