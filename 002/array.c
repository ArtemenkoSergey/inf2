
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



#include "array.h"
#include "item.h"
#include "others.h"


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
        if (A->items[i]==NULL)
        {
            for (i--; i >= 0; --i)
                deleteItem(A->items[i]);
            free(A->items);
            free(A);
            return NULL;
        }
    }
    return A;
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
        return -1;
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
        return -1;
    return 0;
}
void showArray(struct Array* A, FILE* F)
{
    for (int i = 0; i < A->lenght; ++i)
    {
        if (A->items[i])
            showItem(A->items[i], F);
        else
            fprintf(F, "%02d пустой элемент\n", i);
    }
}

// void ArrayTableShow(struct Array* A, FILE* F)
// {
//     const char* nullItem[6] = {"XXXXX", "Пустой элемент", "", "", "", "",};
//     const char* title[6] = {"", "Номер поезда", "Пункт назначения", "Дни следования", "Время прибытия", "Время стоянки"};
//     int colsSize[6];
//     for (int i = 0; i < 6; ++i)
//         colsSize[i] = mbStrLen(title[i]);
//     int tmp = 0;
//     int len;

//     // ширина первого стролбца
//     for (int i = A->length-1; i > 0; i/=10, tmp++);
//     if (tmp>colsSize[0])
//         colsSize[0] = tmp;

//     tmp = tmp*3-1;
//     if (tmp>colsSize[3])
//         colsSize[3] = tmp;

//     // if (colsSize[4] < 5) colsSize[4] = 5;
//     // if (colsSize[5] < 5) colsSize[5] = 5;

//     // выставляем ширину столбцов.
//     for (int i = 0; i < A->len; ++i)
//     {
//         if (A->items[i]!=NULL)
//         {
//             tmp = mbStrLen(A->items[i]->trainNum);
//             if (tmp > colsSize[1])
//                 colsSize[1] = tmp;
            
//             tmp = mbStrLen(A->items[i]->route);
//             if (tmp > colsSize[2])
//                 colsSize[2] = tmp;

//             tmp = 0;
//             for (int k = 1; k < 0x80; k = k << 1)
//                 if (k & A->items[i]->weekDays)
//                     tmp++;
//             tmp = tmp*3-1;
//             if (tmp>colsSize[3])
//                 colsSize[3] = tmp;
//         }
//     }

//     char colsAlignment[] = {1,-1,-1,0,0,0};
//     struct Table* tbl = createTable(6, colsSize, colsAlignment);
// }

int ArraySave(struct Array* A, FILE* F)
{
    // enum FILE_ERRORS err;
    if (F==NULL)
        return FILE_IS_NULL_ERROR;

    if (fwrite(&(A->lenght), sizeof(int), 1, F)!=1) return FILE_WRITE_ERROR;
    for (int i = 0; i < A->lenght; ++i)
        if (itemToBin(A->items[i], F)!=0)
            return FILE_WRITE_ERROR;
    return NO_ERROR;
}

