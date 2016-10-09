

struct Item
{
    char* train_num;
    char* dst;
    unsigned char w_days;
    struct tm* arrival_time; //время HH:MM\0 тип time
    struct tm* station_time; //время HH:MM\0
};

//Функции работы с элементом
struct Item* InitFromStdin();
char* ItemToString(struct Item*, char*, size_t);
struct Item* InitFromStr(char*);

int ShowItem(struct Item*, FILE*);
int DeleteItem(struct Item*);