
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

#define FILENAMEMAXLEN 256
char filename[FILENAMEMAXLEN] = "savedarray.txt";

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

