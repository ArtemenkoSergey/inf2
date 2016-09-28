// Лабораторная работа 4
// Разработать функцию с переменным количеством параметров. Для извлечения параметров из списка использовать
// технологию программирования областей памяти переменного формата, описанную в 4.4.

// 14. Первый параметр - строка, в которой каждый символ «*n», где n-цифра -  обозначает место включения целого (int), 
// являющегося n+1 параметром. Функция выводит на экран полученный текст, содержащий целые значения.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void Print(char* str, ...);
int printUTFchar(char* ch);
char* IntToStr(char* str, int num);

int main(int argc, char const *argv[])
{
    char str[] = "as *1 df *0 sd";
    int a = 1;
    int b = 2;
    Print(str, a, b);
    return 0;
}

void Print(char* str, ...)
{
    char buf[25] = {0};
    char tmpchar;
    char* p = str;
    char* s = str;
    while (*(p++)!='\0') printf("%p %c %d\n",p,*p,(unsigned char)*p );
    for (char i = 0; i < 16; ++i)
    {
        printf("%p %c %d\n",p+i,*(p+i),(unsigned char)*(p+i) );
    }

    int* A = (int*)p;
    // int* A = &a;

    int isNum = 0;
    for (p=str; *p!='\0'; p++)
    {
        if (isNum)
        {
            printf(" is Num %s\n", p);
            if (isdigit(*p))
            {
                char* t = p;
                while(isdigit(*t)) printf("t-> %c\n", *t), t++;
                tmpchar = *t;
                printf("|%c|\n", tmpchar);
                *t = '\0';
                isNum = atoi(p);
                // if (isNum<0)
                //     strcpy(buf, " ERROR ");
                // else
                //     IntToStr(buf, A[isNum]);
                printf("A %d %X\n", isNum, (unsigned int)A[0]);
                puts(buf);
                *t = tmpchar;
                p = t-1;
            }
            else
                p += printUTFchar(p)-1; 
            isNum = 0;
        }
        else
        {
            printf(" is not Num %s\n", p);
            if (*p=='*')
                isNum = 1;
            else
                p += printUTFchar(p)-1;
        }
    }
}

int printUTFchar(char* ch)
{
    
    int charlen = mblen(ch, 6);
    char buf[7];
    // char t = ch[charlen];
    // ch[charlen] = '\0';
    // puts(ch);
    // ch[charlen] = t;
    memcpy(buf, ch, charlen);
    buf[charlen]='\0';
    puts(buf);
    return charlen;
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