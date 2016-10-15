
#include "array.h"


#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif  



struct Array* initArray(int lenght)
{
    if (lenght >= 0)
    {
        struct Array* A = (struct Array*)malloc(sizeof(struct Array*));
        A->lenght = lenght;
        A->items = (struct Item**)malloc(sizeof(struct Item*)*lenght);
        if(A->items)
        {
            for (int i = 0; i < lenght; ++i)
                A->items[i] = NULL;
            return A;
        }
        free(A);
    }
    return NULL;
}

struct Array* initArrayFromFile(FILE* F)
{
    if (F==NULL || feof(F))
        return NULL;
    
    struct Array * A = (struct Array*)malloc(sizeof(struct Array));
    fread(&(A->lenght), sizeof(int), 1, F);
    A->items = (struct Item**)malloc(sizeof(struct Item*)*A->lenght);
    for (int i = 0; i < A->lenght; ++i)
    {
        A->items[i] = initFromBin(F);
    }
    return A;
}

struct Array* importFromFile(FILE* fd)
{
    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
    
    fgets(buffer, BUFFER_SIZE ,fd);
    int lenght = atoi(buffer);
    struct Array* result = initArray(lenght);
    int i = 0;
    while(i<lenght && !feof(fd))
    {
        fgets(buffer, BUFFER_SIZE, fd);
        if (buffer[0]!='\n')
            result->items[i++] = initFromStr(buffer, ',');
        else
            result->items[i++] = NULL;
    }

    free(buffer);
    return result;
}

void clearArray(struct Array* A)
{
    for (int i = 0; i < A->lenght; ++i)
    {
        if (A->items[i])
            deleteItem(A->items[i]);
    }
}

int findFree(struct Array* A)
{
    int i = 0;
    while (i < A->lenght && A->items[i]!=NULL)
        i++;
    if (i < A->lenght)
        return i;
    else
        return ITEM_NOT_FAUND;
} 

int addToArray(struct Array* A, struct Item* item)
{
    int i = findFree(A);
    if (i>=0)
        A->items[i] = item;
    return i;
}


int deleleFromArray(struct Array* A, int index)
{
    if (A->items[index]!=NULL)
        deleteItem(A->items[index]);
    else
        return EMPRY_ITEM_DELETE;
    return 0;
}
void showArray(struct Array* A, FILE* fd)
{
    for (int i = 0; i < A->lenght; ++i)
    {
        if (A->items[i] != NULL)
        {
            fprintf(fd, "\nэлемент %02d:", i);
            showItem(A->items[i], fd);
        }
        else
            fprintf(fd, "\nэлемент %02d: пустой\n", i);
    }
}

void tableShowArray(struct Array* A, FILE* fd)
{
    const char* nullItem = "Пустой элемент";
    const char* title[6] = {"", "Номер поезда", "Пункт назначения", "Дни следования", "Время прибытия", "Время стоянки"};
    int colsSize[6];
    for (int i = 0; i < 6; ++i)
        colsSize[i] = mbStrLen(title[i]);
    int tmp = 0;

    // ширина первого стролбца
    for (int i = A->lenght-1; i > 0; i/=10, tmp++);
    if (tmp>colsSize[0])
        colsSize[0] = tmp;

    // tmp = tmp*3-1;
    // if (tmp>colsSize[3])
    //     colsSize[3] = tmp;

    // if (colsSize[4] < 5) colsSize[4] = 5;
    // if (colsSize[5] < 5) colsSize[5] = 5;

    // выставляем ширину столбцов.
    for (int i = 0; i < A->lenght; ++i)
    {
        if (A->items[i]!=NULL)
        {
            tmp = mbStrLen(A->items[i]->trainNum);
            if (tmp > colsSize[1])
                colsSize[1] = tmp;
            
            tmp = mbStrLen(A->items[i]->route);
            if (tmp > colsSize[2])
                colsSize[2] = tmp;

            tmp = 0;
            for (int k = 1; k < 0x80; k = k << 1)
                if (k & A->items[i]->weekDays)
                    tmp++;
            tmp = tmp*3-1;
            if (tmp>colsSize[3])
                colsSize[3] = tmp;
        }
    }

    char colsAlignment[] = {1,-1,-1,0,0,0};
    struct Table* tbl = createTable(6, colsSize, colsAlignment);

    fputSeparatorRow(tbl, fd);
    fputRow(tbl, fd, 6, title[0], title[1], title[2], title[3], title[4], title[5]);
    fputSeparatorRow(tbl, fd);

    char weekDays[25];
    char arrivalTime[6];
    char stationTime[6];
    char rowNum[6];
    for (int i = 0; i < A->lenght; ++i)
    {
        intToStr(rowNum, i);
        if (A->items[i] != NULL)
        {
            weekDaysToStr(A->items[i]->weekDays, weekDays);
            strftime(arrivalTime, 6, "%R", A->items[i]->arrivalTime);
            strftime(stationTime, 6, "%R", A->items[i]->stationTime);
            fputRow(tbl, fd, 6, rowNum, A->items[i]->trainNum, A->items[i]->route, weekDays, arrivalTime, stationTime);
        }
        else
            fputRow(tbl, fd, 2, rowNum, nullItem);   
    }
    fputSeparatorRow(tbl, fd);   
}

int saveArrayToFile(struct Array* A, FILE* fd)
{
    // enum FILE_ERRORS err;
    if (fd==NULL)
        return FILE_IS_NULL_ERROR;
    int nullItem = 0;
    if (fwrite(&(A->lenght), sizeof(int), 1, fd)!=1)
        return FILE_WRITE_ERROR;
    for (int i = 0; i < A->lenght; ++i)
        if (A->items[i] != NULL)
        {
            if (itemToBin(A->items[i], fd)!=0)
                return FILE_WRITE_ERROR;
        }
        else
            if (fwrite(&(nullItem), sizeof(int), 1, fd)!=1) 
                return FILE_WRITE_ERROR;
    return NO_ERROR;
}

void deleteArray(struct Array* A)
{
    for (int i = 0; i < A->lenght; ++i)
    {
        if (A->items[i] != NULL)
            deleteItem(A->items[i]);
    }
    free(A);
}

void rePosArraiItems(struct Array* A)
{
    for (int i = 0; i < A->lenght-1; ++i)
    {
        if (A->items[i] == NULL)
        {
            for (int j = i; j < A->lenght-1; ++j)
                A->items[j] = A->items[j+1];
        }

    }
}

int findMinTrainNum(struct Array* A, int startPos)
{
    int indexMinItem = startPos;
    while(A->items[indexMinItem] == NULL && indexMinItem < A->lenght) indexMinItem++;
    if (indexMinItem >= A->lenght) return ITEM_NOT_FAUND;

    for (int i = indexMinItem; i < A->lenght; ++i)
    {
        if (A->items[i] != NULL)
            if (strcmp(A->items[indexMinItem]->trainNum, A->items[i]->trainNum) > 0)
                indexMinItem = i;
    }
    return indexMinItem;
}

int findMinRoute(struct Array* A, int startPos)
{
    int indexMinItem = startPos;
    while(A->items[indexMinItem] == NULL && indexMinItem < A->lenght) indexMinItem++;
    if (indexMinItem >= A->lenght) return ITEM_NOT_FAUND;

    for (int i = indexMinItem; i < A->lenght; ++i)
    {
        if (A->items[i] != NULL)
            if (strcmp(A->items[indexMinItem]->route, A->items[i]->route) > 0)
                indexMinItem = i;
    }
    return indexMinItem;
}

int findMinArrivalTime(struct Array* A, int startPos)
{
    int indexMinItem = startPos;
    while(A->items[indexMinItem] == NULL && indexMinItem < A->lenght) indexMinItem++;
    if (indexMinItem >= A->lenght) return ITEM_NOT_FAUND;

    time_t tTime;
    time_t minTime = mktime(A->items[indexMinItem]->arrivalTime);
    for (int i = indexMinItem; i < A->lenght; ++i)
    {
        if (A->items[i] != NULL)
        {
            tTime = mktime(A->items[i]->arrivalTime);
            if (minTime > tTime)
            {
                indexMinItem = i;
                minTime = tTime;
            }
        }
    }
    return indexMinItem;
}

int findMinStationTime(struct Array* A, int startPos)
{
    int indexMinItem = startPos;
    while(A->items[indexMinItem] == NULL && indexMinItem < A->lenght) indexMinItem++;
    if (indexMinItem >= A->lenght) return ITEM_NOT_FAUND;

    time_t tTime;
    time_t minTime = mktime(A->items[indexMinItem]->stationTime);

    for (int i = indexMinItem; i < A->lenght; ++i)
    {
        if (A->items[i] != NULL)
        {
            tTime = mktime(A->items[i]->stationTime);
            if (minTime > tTime)
            {
                indexMinItem = i;
                minTime = tTime;
            }
        }
    }
    return indexMinItem;
}

void sortByTrainNum(struct Array* A)
{
    rePosArraiItems(A);
    int min;
    struct Item* tItem;
    for (int i = 0; i < A->lenght-1; ++i)
    {
        min = findMinTrainNum(A, i);
        if (min == ITEM_NOT_FAUND) return;
        if (min != i)
        {
            tItem = A->items[i];
            A->items[i] = A->items[min];
            A->items[min] = tItem;
        }

    }
}

void sortByRoute(struct Array* A)
{
    rePosArraiItems(A);
    int min;
    struct Item* tItem;
    for (int i = 0; i < A->lenght-1; ++i)
    {
        min = findMinRoute(A, i);
        if (min == ITEM_NOT_FAUND) return;
        if (min != i)
        {
            tItem = A->items[i];
            A->items[i] = A->items[min];
            A->items[min] = tItem;
        }
    }
}

void sortByArrivalTime(struct Array* A)
{
    rePosArraiItems(A);
    int min;
    struct Item* tItem;
    for (int i = 0; i < A->lenght-1; ++i)
    {
        min = findMinArrivalTime(A, i);
        if (min == ITEM_NOT_FAUND) return;
        if (min != i)
        {
            tItem = A->items[i];
            A->items[i] = A->items[min];
            A->items[min] = tItem;
        }
    }
}

int findNearestTrainNum(struct Array* A, int startPos, const char* str)
{
    
}