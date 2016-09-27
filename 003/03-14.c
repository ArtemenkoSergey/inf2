// Лаболаторная работа 3

// Разработать две функции,
// 	одна из которых вводит с клавиатуры набор данных в произвольной последовательности
// 	и размещает в памяти в переменном формате.
// 	Другая функция читает  эти  данные  и выводит на экран.
// Программа запрашивает и размещает в памяти несколько наборов данных при помощи первой функции,
// а затем читает их и выводит на экран при помощи второй.
// Размещение данных производить в статическом массиве байтов фиксированной размерности  с  контролем  его переполнения.

// 14. Область памяти представляет собой строку.
// Если в ней встречается символ "%", то сразу же за ним находится указатель на другую (обычную) строку.
// Все сроки располагаются в той же области памяти вслед за основной строкой.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFER_SIZE 1024

int InputData(char*, int);
void ShowData(char*, int);

// вспомагательные функции
char* FindNextPointer(char*);
char* FindPrevPointer(char*, const char*);
char* FindPointer(char*, const char*, int);

int main(int argc, char const *argv[])
{
    char buf[BUFFER_SIZE];
    int len = InputData(buf, 25);
    if (len > 0)
        ShowData(buf, BUFFER_SIZE);
    return 0;
}

void F(unsigned char* str, int n)
{
    for (int i = 0; i < n; ++i)
    {
        printf(" %c ", str[i]);
    }
    printf("\n");
    for (int i = 0; i < n; ++i)
    {
        printf("%02X ", str[i]);
    }
    printf("\n");
}

int InputData(char* buf, int buf_size)
{
    char* bc = buf;
    char* tc = bc;
    char* ec = bc+buf_size;
    int pos = 0;
    int psize = sizeof(char*);
    // strcpy(tc, "%a%\n");
    //вводим остновную строку
    fputs("Введите основную строку: ", stdout);
    fgets(tc, buf_size, stdin);
    fputs(tc,stdout);
    tc += strlen(tc);
    for (; *tc!='\n' && tc>=bc; tc--);
    *tc = '\0';
    
    
    //считаем количество '%'
    int count = 0;
    for (tc = bc; *tc!='\0'; tc++)
        if (*tc == '%') count++;
    char* dc = tc;
    if (count != 0)
    {
            // если есть место хотябы на 1 символ для каждой дополнительной строки
            
            if (tc+count*psize < ec-count*sizeof(char))
            {
                printf("Введите дополнительные строки (%d штук)\n", count);
                // выделяем место под указатели в середине строки
                char* p;
                for (int i = count; i > 0; i--)
                {
                    // printf("move tc %p %02X %c\n",tc, (unsigned char)*tc, *tc );
                    p = FindPrevPointer(tc-1, bc);
                    if (p!=NULL)
                        memmove(p+psize*i+1, p+1, tc-p),
                        memset(p+psize*(i-1)+1, ' '+i, psize);
                    else
                        break;
                    tc = p;

                    // printf("move  p %p %02X %c\n",p, (unsigned char)*p, *p );
                }

                dc+=count*psize;
                tc = dc;
                *(tc++) = '\0';
                
                // F(bc, tc-bc);            
                // вводим дополнительные строки
                p = bc;
                for (int i = count; i > 0 && tc <= ec; i--)
                {
                    p = FindNextPointer(p)+1;
                    // printf("set  p %p %02X %c\n",p, (unsigned char)*p, *p );
                    printf("%d (осталось %ld байт)", count+1-i, ec-tc-i*sizeof(char));
                    // strcpy(tc, "|B|\n");
                    // printf("%s", tc);
                    fgets(tc, ec-tc-i*sizeof(char)+1, stdin);
                    *(char**)(p) = tc;
                    // printf("set tc %p %02X %c\n",tc, (unsigned char)*tc, *tc );
                    F(bc, ec-bc);            
                    tc += strlen(tc);
                    printf("set  p %p %02X %c\n",*(char**)p, (unsigned char)*p, *p );
                    for (dc = tc; *tc!='\n' && tc>*(char**)p; tc--);
                    printf("1set tc %p %02X %c\n",tc, (unsigned char)*tc, *tc );
                    if (*tc=='\n') 
                        *(tc++) = '\0';
                    else
                        tc = dc+1;
                    printf("2set tc %p %02X %c\n",tc, (unsigned char)*tc, *tc );
                    p+=psize;
                }
            }
            else
            {
                printf("не хватает места для дополнительны строк.\n");
                return -1;
            }
    }
    else
    {
        printf("Ввод дополнительных строк не требуется.\n");
    }
    F(bc, ec-bc);
    return tc-bc;
}

void ShowData(char* str, int buf_size)
{
    char* ec = str+buf_size;
    int psize = sizeof(char*);
    while(*str!='\0' && str<ec)
    {
        if (*str=='%')
        {   // выводим дополнительную строку
            str++;
            fputs(*(char**)str, stdout);
            str += psize;
        }
        else
        {   // выводим символ основной строки
            fputc(*str, stdout);
            str++;
        }

    }
    fputc('\n', stdout);

}


char* FindNextPointer(char* p)
{
    while(*(p)!='%' && *p !='\0') p++;
    if (*p=='%')
        return p;
    return NULL;
}

char* FindPrevPointer(char* p, const char* start)
{
    while(p >= start && *(p)!='%') p--;
    if (*p=='%')
        return p;
    return NULL;
}
