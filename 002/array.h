

// структура массива
struct Array
{
    int len;
    // int use;
    struct Item ** items;
};

// Функции работы с массивом
int ArrayInit(struct Array* A, int len);
int ArrayClear(struct Array*);
int FindFree(struct Array*);
int AddtoArray(struct Array*, struct Item*);
int ArrayAddFromFile(struct Array* A, FILE* F);
int DeleleFromArray(struct Array*, int index);
void ArrayShow(struct Array*, FILE*);
void ArrayTableShow(struct Array*, FILE*);

int ArraySave(struct Array*, FILE*);
int ArrayInitFromFile(struct Array*, FILE*);