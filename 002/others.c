

#include "others.h"


#define BUFFER_SIZE 1024


const wchar_t ruWeekDays[8][3] = {L"ПН",L"ВТ",L"СР",L"ЧТ",L"ПТ",L"СБ",L"ВС",L""};
const wchar_t enWeekDays[8][3] = {L"MO",L"TU",L"WE",L"TH",L"FR",L"SA",L"SU",L""};

unsigned char strToWeekDays(char* str)
{
    unsigned char r = 0;
    wchar_t* word = NULL;
    int l = strlen(str);
    wchar_t* ws = (wchar_t*)malloc(sizeof(wchar_t)*(l+1));
    // size_t mbstowcs (wchar_t* dest, const char* src, size_t max);
    mbstowcs(ws, str, l);
    for (wchar_t* p = ws; *p!=L'\0'; p++)
        *p = towupper(*p);
    // size_t wcstombs (char* dest, const wchar_t* src, size_t max);
    // wcstombs(str, ws,l);
    
    // wprintf("###############\n");
    for (wchar_t *p = ws; ; p++)
    {
        // printf("%p \"%1c\" %s\n", p, *p, str);
        
        if (word)
        {
            if (!iswalpha(*p))
            {
                char t = *p;
                *p = '\0';
                r = r | isWeekDay(word);
                *p = t;
                word = NULL;
            }
        }
        else
        {
            if (iswalpha(*p))
                word = p;
        }
        if (*p=='\0') break;
    }

    // wcstombs(str, ws,l);
    free(ws);
    return r;
}

unsigned char isWeekDay(wchar_t* ws)
{
    unsigned char r = 0;
    unsigned char m = 1;
    for (int i = 0; ruWeekDays[i][0]!='\0' && r==0; ++i)
    {
        // fputs(ruWeekDays[i], stdout);
        if (0==wcscmp(ws, ruWeekDays[i]) || 0==wcscmp(ws, enWeekDays[i]))
        {
            r = r | m;
            // fputs(" OK", stdout);
        }
        m = m << 1;
        // printf("%02X \n",r);
    }
    // printf("is week day %02X \n",r);
    return r;
}

char* weekDaysToStr(unsigned char weekDays, char* buffer)
{
    // char* p = str;
    // int l = sizeof(ruWeekDays)/sizeof(ruWeekDays[0]);
    // int size = 0; // считаем максимальную длинну строки с днями недели.
    // printf("l=%d\n", l);
    // for (int i = 0; i < l; ++i)
    //     size += wcslen(ruWeekDays[i])+1;
    // char* buffer = (char*)malloc(sizeof(char)*size);
    char* pos = buffer;
    *pos = '\0';
    int i = 0;
    int l;
    weekDays = weekDays & (~0x80); // убираем лишний бит если он есть

    for (unsigned char mask = 1; mask < 0x80; i++, mask <<= 1)
    {
        if (weekDays & mask)
        {
            l = wcslen(ruWeekDays[i]);
            wcstombs(pos, ruWeekDays[i], l*3);
            l = strlen(pos);
            pos += l;
            *(pos++) = ' ';
            weekDays = weekDays & (~mask);
        }
    }
    if (pos != buffer)
        *(--pos) = '\0';

    // if (weekDays !=0 )
    // {
    //     free(buffer);
    //     return NULL;
    // }
    
    // size = pos - buffer;
    // char* result = (char*)malloc(sizeof(char)*size);
    // strcpy(result, buffer);

    // free(buffer);
    return buffer;
}

struct tm* strToTime(const char* str)
{
    // char tmp[6] = {0};
    struct tm* tm = (struct tm*)malloc(sizeof(struct tm));
    int t[2] = {0};
    const char* digit = NULL;
    int c;
    for (c=0; c<2; str++)
    {
        if (digit != NULL)
        {
            if (!isdigit(*str))
            {   
                t[c++] = atoi(digit);
                digit = NULL;
            }
        }
        else
        {
            if (isdigit(*str))
                digit = str;    
        }
        if (*str=='\0') break;   
    }
    if (c==1)
    {
        t[1] = t[0];
        t[0] = 0;
    }

    if (c<1 || t[0]<0 || t[0]>23 || t[1]<0 || t[1]>59)
    {
        free(tm);
        return NULL;
    }
    tm->tm_hour = t[0];
    tm->tm_min = t[1];

    return tm;
}

char* inputFileName(char* name)
{
    
    // ClearStdin();
    // printf("1################\n");
    // fgets(buf, FILENAMEMAXLEN, stdin);

    char* buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);

    fprintf(stdout, "введите имя файла (%s): ", name);
    
    fgets(buffer, BUFFER_SIZE, stdin);
    int i = strlen(buffer);
    // printf("%d %02X %s\n", i, buffer[i], buffer);
    while (buffer[i]!='\n' && i>0) i--;
    buffer[i]='\0';
    // printf("%d %s\n", i, buffer);
    if (buffer[0]!='\0')
        strncpy(name, buffer, i);
    
    free(buffer);
    return name;
}

int mbStrLen(const char* str)
{
    int c = 0;
    int l = 1;
    while(*str!='\0' && l>0)
    {
        l = mblen(str, 6);
        str += l;
        c++;
    }
    if (l>0)
        return c;
    else
        return l;
}

char* intToStr(char* s, int n)
{
    int i, sign;
    if ((sign = n) < 0)    
        n = -n;                
    i = 0;
    do 
    {               
        s[i++] = n % 10 + '0';   
    } while ((n /= 10) > 0);     
    if (sign < 0)
         s[i++] = '-';
    s[i--] = '\0';

    int j = 0;
    char c;
    while(j<i)
    {
        c = s[j];
        s[j] = s[i];
        s[i] = c;
        j++;
        i--;
    }


    return s;
}