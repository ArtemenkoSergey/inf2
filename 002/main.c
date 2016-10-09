/*Определить структурированный тип, определить набор функций для работы с массивом структур.
В  структурированной переменной предусмотреть способ отметки ее как не содержащей данных (т.е. "пустой").
Функции должны работать с массивом структур или с отдельной структурой через указатели,
а также при необходимости возвращать указатель на структуру. В перечень функций входят:
    + «очистка» структурированных переменных;
    + поиск свободной структурированной переменной;
    + ввод элементов (полей) структуры с клавиатуры;
    + вывод элементов (полей) структуры с клавиатуры;
    - поиск в массиве структуры и минимальным значением  заданного поля;
    - сортировка массива  структур в порядке возрастания заданного поля
        сортировке можно использовать тот факт, что в Си++ разрешается присваивание структурированных переменных);
    - поиск в массиве структур элемента с заданным значением поля или с наиболее близким к нему по значению. 
    + удаление заданного элемента;
    - изменение (редактирование) заданного элемента.
    - вычисление с проверкой и использованием всех элементов массива по заданному условию и формуле
        (например, общая сумма на всех счетах) -  дается индивидуально.

14. Номер поезда, пункт назначения, дни следования, время прибытия, время стоянки.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include <time.h>
#include <ctype.h>


#include "array.h"
#include "table.h"

#define BUFFER_SIZE 1024
// char buffer[BUFFER_SIZE];



int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.utf8");

    // char* str = "фычс";
    // printf("%d %d\n", mbstrlen(str), strlen(str));

    // int colwidth[6] = {1, 0, 0, 2, 5, 5};
    // const char* lineformat = "+-%*s-+-%*s-+-%*s-+-%*s-+-%*s-+-%*s-+\n";
    // printf(lineformat, 5, "-", 5, "-", 5, "", 5, "", 6, "", 6, "");
    
    // struct Array* A = (struct Array*)malloc(sizeof(struct Array));
    // ArrayInitFromFile(A,NULL);
    // // ArrayShow(A, stdout);
    // ArrayAddFromFile(A, NULL);
    // ArrayTableShow(A, stdout);
    

    // ArrayClear(A);
    // free(A);
    char cols[5][100] = {"колонка 1","aa", "колонка 1","col3","dsweqdeqwdqw123456789"};
    // char cols[5][100] = {"col1","col2","col3","col4","dnewnceofhnaufcihbeauiwffcbiuewghifuwegiacv123sasw"};
    int colqty = 5;
    int cols_size[] = {5, 5, 5, 5, 5};
    char col_agl[] = {-1, -1, 0, 1, -1};

    struct Table* tbl = createtable(colqty, cols_size, col_agl);

    char* str = (char*)malloc(sizeof(char)*tbl->_row_size+50);
    char* p = str;
    int s = 0;
    for (int i = 0; i < 5; ++i)
    {
        strcpy(p,cols[i]);
        s += strlen(p)+1;
        printf("%s len %d\n",p, strlen(p));
        p+=strlen(cols[i])+1;
    }

    printf("s %d\n", s);
    printf("_row_size %d\n",tbl->_row_size );
    fputseparatorrow(tbl, stdout);
    fputrow(tbl, str, stdout);
    fputseparatorrow(tbl, stdout);
    deletetable(tbl);
    free(str);
    return 0;
}
