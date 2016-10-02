// Лабораторная работа 3

// Разработать две функции,
// 	одна из которых вводит с клавиатуры набор данных в произвольной последовательности
// 	и размещает в памяти в переменном формате.
// 	Другая функция читает  эти  данные  и выводит на экран.
// Программа запрашивает и размещает в памяти несколько наборов данных при помощи первой 
// функции, а затем читает их и выводит на экран при помощи второй.
// Размещение данных производить в статическом массиве байтов фиксированной размерности  
// с  контролем  его переполнения.

// 14. Область памяти представляет собой строку.
// Если в ней встречается символ "%", то сразу же за ним находится указатель на другую (обычную) строку.
// Все сроки располагаются в той же области памяти вслед за основной строкой.

#include <stdio.h>      // fputs(), fgets(), fputc(), fprintf(), fwrite(), fflush(), stdout, stdin
#include <stdlib.h>     // mblen() 
#include <string.h>     // strlen(), memmove()
#include <locale.h>     // setlocale(LC_ALL, "ru_RU.utf8")


// функция ввода структуры данных со стандартного вввода
// принимает:   указатель на первый байт структуры
//              максимальный размер структуры
// возвращает:  размер структуры данных или -1 если 
//              ввод структуры данных завершился неудачей
int InputData(char*, int);

// функция вывода структуры данных на стандартный вывод
// принимает:   указатель на первый байт структуры
//              максимальный размер структуры
void ShowData(char*);

// вспомогательные функции

// функция поиска символа в массиве символов
// принимает:   искомый символ
//              указатель на символ с которого начинать поиск
//              указатель на символ на котором завершать поиск
// возвращает:  указатель на первый найденный символ '%'
//              или NULL если символ не найден
char* FindChar(char, char*, char*);

//выводит много байтовый символ в файл
//принимает:    указатель на первый байт символа
//возвращает:   длину выведенного символа или
//              -1 если символ не соответствует категории LC_CTYPE
int fputUTFchar(char*, FILE*);

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.utf8");

    const int BUFFER_SIZE = 64;
    char buf[BUFFER_SIZE];
    int len = InputData(buf, BUFFER_SIZE);
    if (len > 0)
        ShowData(buf);
    return 0;
}


int InputData(char* buf, int buf_size)
{
    char* bc = buf;         // первый символ структуры данных
    char* tc = bc;          // текущий символ структуры данных
    char* ec = bc+buf_size; // указатель на конец выделенного места
    int psize = sizeof(char*); // размер указателя на символ
    //вводим основную строку
    fputs("Введите основную строку: ", stdout);
    fgets(tc, buf_size, stdin);
    tc += strlen(tc);
    for (; *tc!='\n' && tc>=bc; tc--);
    if (*tc=='\n') 
        *tc = '\0';
    
    //считаем количество '%'
    int count = 0;
    for (tc = bc; *tc!='\0'; tc++)
        if (*tc == '%') count++;
    
    if (count > 0)
    {
        char* dc = tc + count*psize; // указатель конец основной строки.
            // если есть место хотя бы на 1 символ для каждой дополнительной строки
            
        if (tc+count*psize + count*sizeof(char)< ec)
        {
            
            // выделяем место под указатели в строке
            char* p;
            for (int i = count; i > 0; i--)
            {
                p = FindChar('%', tc-1, bc);
                memmove(p+psize*i+1, p+1, tc-p);
                tc = p;
            }

            tc = dc;
            *(tc++) = '\0';
            
            // вводим дополнительные строки
            fprintf(stdout, "Введите дополнительные строки (%d штук)\n", count);
            p = bc;
            for (int i = count; i > 0 && tc <= ec-i*sizeof(char); i--)
            {
                p = FindChar('%', p, ec)+1;
                if (tc < ec-i*sizeof(char))
                    fprintf(stdout, "%d (осталось %ld байт): ",count+1-i,ec-tc-i*sizeof(char));
                fgets(tc, ec-tc-i*sizeof(char)+1, stdin);
                *(char**)(p) = tc;
                tc += strlen(tc);
                for (dc = tc; *tc!='\n' && tc>*(char**)p; tc--);
                if (*tc=='\n') 
                    *(tc++) = '\0';
                else
                    tc = dc+1;
                p+=psize;
            }
        }
        else
        {
            fprintf(stderr, "не хватает места для указателей и дополнительны строк.\n");
            return -1;
        }
    }
    else
    {
        fprintf(stdout, "Ввод дополнительных строк не требуется.\n");
    }
    return tc-bc;
}


void ShowData(char* tc)
{
    //char* ec = tc+buf_size;
    while(*tc!='\0')
    {
        if (*tc=='%')
        {   // выводим дополнительную строку
            tc++;
            fputs(*(char**)tc, stdout);
            tc += sizeof(char**);
        }
        else
        {   // выводим символ основной строки
            int l = fputUTFchar(tc, stdout);
            if (l > 0)
                tc += l;
            else
                tc++;
        }

    }
    fputc('\n', stdout);

}



char* FindChar(char ch, char* p, char* end)
{
    int way = (end > p)? 1:-1;
    while(p != end && *(p)!=ch) p+=way;
    if (*p==ch)
        return p;
    return NULL;
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
