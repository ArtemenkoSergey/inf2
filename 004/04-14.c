// Лабораторная работа 4

#include <stdio.h>      //  printf(), fwrite(), fflush(), stdout
#include <stdlib.h>     //  malloc(), free(), mblen()
#include <ctype.h>      //  isdigit()
#include <stdarg.h>     //  va_start(), va_end(), va_arg()
#include <locale.h>     //  setlocale(LC_ALL, "ru_RU.utf8")

// решение лабораторной работы.
// принимает:   форматную строку и
//              несколько переменных типа int зависимости от содержимого строки.
void VA_Show(char*, ...);

//выводит много байтовый символ в файл
//принимает:    указатель на первый байт символа
//возвращает:   длину выведенного символа или
//              -1 если символ не соответствует категории LC_CTYPE
int fputUTFchar(char*, FILE*);

// преобразует массив символов содержащий внешнее представление числа
// во внутреннее представление
// принимает:   указатель на массив символов
//              длину массива символов
// возвращает:  число во внутреннем представлении.
// НЕ КОНТРОЛИРУЕТ СОДЕРЖАНИЕ МАССИВА СИМВОЛОВ!!!!!
// НЕ ОБРАБАТЫВАЕТ ЗНАК '-'
int my_atoi(char* ch, int len);

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.utf8");

    VA_Show("as *2 df *0 sd", 1, 2, 3);

    return 0;
}

void VA_Show(char* str, ...)
{
    int max_num=0;
    int isNum = 0;
    char* p = str;

    // ищем максимальное число написанное после символа '*'
    while(*p!='\0')
    {
        if (isNum)
        {
            if (isdigit(*p))
            {
                char* t = p;
                while(isdigit(*t)) t++;
                isNum = my_atoi(p, t-p);
                if (isNum > max_num)
                    max_num = isNum;
                p = t;
            }
            else
                p++;; 
            isNum = 0;
        }
        else
        {
            if (*p=='*')
                isNum = 1;
            p++;
        }
    } 

    // для организации произвольного доступа копируем все
    // не обязательные параметры в массив.
    int * A = (int*)malloc(sizeof(int)*max_num+1);
    va_list ap;
    va_start(ap, str);
    for (int i = 0; i <= max_num; ++i)
        A[i] = va_arg(ap, int);
    va_end(ap);


    // выводим строку заменяя "*n" на n+1 параметр функции.
    p = str;
    isNum = 0;
    while(*p!='\0')
    {
        if (isNum)
        {
            if (isdigit(*p))
            {
                char* t = p;
                while(isdigit(*t)) t++;
                isNum = my_atoi(p, t-p);
                printf("%d", A[isNum]);
                p = t;
            }
            else
                p += fputUTFchar(p, stdout); 
            isNum = 0;
        }
        else
        {
            if (*p=='*')
            {
                isNum = 1;
                p++;
            }
            else
                p += fputUTFchar(p, stdout);
        }
    } 
    printf("\n");
    free(A);
}

int my_atoi(char* ch, int len)
{
    char* end = ch + len;
    int result = 0;
    int osn = 10;
    for (; ch<end ; ch++)
    {
        result *= osn;
        result += (*ch-'0');
    }
    return result;
}

int fputUTFchar(char* ch, FILE* F)
{
    int len = mblen(ch, 6);
    char* end = ch + len;
    while (ch < end)
    {
        fwrite(ch, sizeof(char), 1, F);
        ch++;
    }
    fflush(F);
    return len;
}
