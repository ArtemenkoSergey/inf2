#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <time.h>
#include <ctype.h>

#include "others.h"

#ifndef BUFFER_SIZE
    #define BUFFER_SIZE 1024
#endif  

const wchar_t RuWeekDays[8][3] = {L"ПН",L"ВТ",L"СР",L"ЧТ",L"ПТ",L"СБ",L"ВС",L""};
const wchar_t EnWeekDays[8][3] = {L"MO",L"TU",L"WE",L"TH",L"FR",L"SA",L"SU",L""};

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