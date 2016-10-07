

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void Print(char* str, ...);
int printUTFchar(char* ch);
char* IntToStr(char* str, int num);

struct A
{
    int a;
    long int b;
};

int main(int argc, char const *argv[])
{
    char str[] = "as *1 df *0 sd";
    int a = 0x87654321;
    int b = 0X12345678;
    int c = 0XF0F0F0F0;

    long long int e = 1;
    struct A f;
    f.a = 0x10000001;
    f.a = 0x2000000220000002;
    printf("%p\n", &f);
    printf("a = %08X\nc = %08X\nc = %08X\n\n", a, b, c);
    printf("sizeof() %lu\nsizeof(int) %lu\n", sizeof(struct A), sizeof(int) );
    Print(str, f, b, c);
    return 0;
}

void Print(char* str,...)
{
    printf("str = %p %c %d\n",str,*str,(unsigned char)*str );
    printf("&str = %p \n", &str);
    
    unsigned long int** p = (unsigned long int**)&str;
    printf("\n");
    char i;
    for (i = -1; i < 64; ++i)
    {
        printf("%p\t%p\t%016lX\n",p+i, *(p+i), *(p+i));
    }
    printf("\n");
    unsigned int** o = (unsigned int**)&str;
    for (i = -2; i < 20; ++i)
    {
        printf("%p\t%08X\n",(unsigned int**)((int*)o+i), *(unsigned int**)((int*)o+i));
    }

    

    return;
  
}

