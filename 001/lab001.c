/* Задание:
// 14. Найти в строке последовательности, состоящие из одного повторяющегося символа и заменить его на число символов и один символ (например "aaaaaa" - "5a").*/
#include <stdio.h>
#include <stdlib.h>

//функция получает указатель на строку и заменяет в данной строке последовательность
//из одинаковых символов на десятичное число равное количеству символов и один символ.
void replase(char* str);

int main(int argc, char *argv[])
{
    char *str; //указатель на обрабатываемую строку
    int str_len=0; //длина обрабатываемой строки
    if (argc>1)
    {// ввод строки из аргументов командной строки. 
        for (int i = 1; i < argc; ++i)
            for (char* p = argv[i]; *p!='\0'; ++p)
                str_len++;
        str_len+=argc;
        str = (char*)malloc(sizeof(char) * (str_len));
        char* t = str;
        for (int i = 1; i < argc; ++i)
        {
            for (char* p = argv[i]; *p!='\0'; ++p)
                *(t++) = *p;
            *(t++) = ' ';
        }
        *t = '\0';
    }
    else
    {//ввод строки со стандартного ввода
        char* buf;
        int buf_len = 1024;
        buf = (char*)malloc(sizeof(char) * buf_len+1);
        fgets(buf, buf_len, stdin);
        for (char*p = buf; *p!='\n'; p++, str_len++);
        str = (char*)malloc(sizeof(char) * (++str_len));
        char* dst = str;
        for (char* src = buf; *src!='\n'; src++, dst++)
            *dst = *src;
        *(++dst)='\0';
        free(buf);
    }
    // выводим начальный вариант строки на экран
    // printf("%s\n", str);
    fputs(str, stdout);
    fputc('\n', stdout);
    replase(str);
    // выводим измененный вариант строки на экран
    fputs(str, stdout);
    fputc('\n', stdout);
    free(str);
    return 0;
}

void replase(char* str)
{
    char* src = str;
    char* dst = str;
    int c = 1;
    for (; *src!='\0'; src++)
    {
        if (*src==*(src+1))
            c++;
        else
        {
            if (c!=1)
            {
                int l = 1;
                for (int i = c; i > 10; i/=10, l*=10);
                for  (; l>0 ; l/=10)
                {
                    *(dst++) = c/l+'0';
                    c=c%l;
                }
                c=1;   
            }
            *(dst++) = *src;
        }
    }
    *dst='\0';
}
