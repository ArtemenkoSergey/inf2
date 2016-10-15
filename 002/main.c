/*Определить структурированный тип, определить набор функций для работы с массивом структур.
В  структурированной переменной предусмотреть способ отметки ее как не содержащей данных (т.е. "пустой").
Функции должны работать с массивом структур или с отдельной структурой через указатели,
а также при необходимости возвращать указатель на структуру. В перечень функций входят:
    + «очистка» структурированных переменных;
    + поиск свободной структурированной переменной;
    + ввод элементов (полей) структуры с клавиатуры;
    + вывод элементов (полей) структуры с клавиатуры;
    + поиск в массиве структуры и минимальным значением  заданного поля;
    + сортировка массива  структур в порядке возрастания заданного поля
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
#include "item.h"




int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "ru_RU.utf8");
    // int colQuantity = 5;
    // int colsSize[] = {6,6,6,6,6};
    // char colsAlignment[] = {1,1,0,-1,-1};

    // struct Table* tbl = createTable(colQuantity, colsSize, colsAlignment);
    
    // char* str = getRow(tbl, 2, "Ф","a");

    // fputSeparatorRow(tbl, stdout);
    // printf("%s", str);
    // fputSeparatorRow(tbl, stdout);
    // free(str);

    //struct Array* A = initArray(10);
    FILE* fd = fopen("test.bin","rb");
    // FILE* fd = fopen("raspisanie.txt","r");
    struct Array* A = initArrayFromFile(fd);
    // struct Array* A = importFromFile(fd);
    fclose(fd);

    // fd = fopen("test.bin","wb");
    // printf("%d\n", saveArrayToFile(A, fd));

    // fclose(fd);
    tableShowArray(A, stdout);
    printf("sort sortByTrainNum\n");
    sortByTrainNum(A);
    tableShowArray(A, stdout);

    // int t = findMinTrainNum(A,0);
    // printf("%d\n", t);
    // showItem(A->items[t], stdout);

    printf("sort sortByRoute\n");
    sortByRoute(A);
    tableShowArray(A, stdout);


    printf("sort sortByArrivalTime\n");
    sortByArrivalTime(A);
    tableShowArray(A, stdout);

    deleteArray(A);


    return 0;
}
