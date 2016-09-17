// Задание:
// 14. Найти в строке последовательности, состоящие из одного повторяющегося символа и заменить его на число символов и один символ (например "aaaaaa" - "5a").
#include <iostream>
using namespace std;

void replase(char* str);

int main(int argc, char *argv[])
{
    char *str;
    int str_len=0;
    if (argc>1)
    {
        for (int i = 1; i < argc; ++i)
            for (char* p = argv[i]; *p!='\0'; ++p)
                str_len++;
        str = new char[++str_len];
        char* t = str;
        for (int i = 1; i < argc; ++i)
            for (char* p = argv[i]; *p!='\0'; ++p)
                *t++ = *p;
        *t = '\0';
    }
    else
    {
        char* buf;
        int buf_len = 1024;
        buf = new char[buf_len];
        cin.getline(buf, buf_len);
        for (char*p = buf; *p!='\0'; p++, str_len++);
        str = new char[++str_len];
        char* dst = str;
        for (char* src = buf; *src!='\0'; src++, dst++)
            *dst = *src;
        dst++;
        *dst='\0';
        delete [] buf;
    }
    cout << str << endl;
    replase(str);
    cout << str << endl;
    delete [] str;
    return 0;
}

void replase(char* str)
{
    char* src = str;
    char* dst = str;
    int c = 1;
    for (; *src!='\0'; src++)
    {
        if (c==1)
            if (*src==*(src+1))
            {
                c++;
            }
            else
            {
                *dst++ = *src;
            }
        else
        {
            if (*src==*(src+1))
            {
                c++;
            }
            else
            {
                int l = 0;
                for (int i = c; i > 0; i/=10, l++);
                char* num =  new char[l+1];
                // *(num+l--) = '\0';
                char *t = num+l;
                *t-- = '\0';
                for (; c > 0; t--, c/=10)
                {
                    *t = c%10+'0';
                }
                for(t = num; *t!='\0'; t++)
                    *dst++ = *t;
                delete [] num;
                *dst++ = *src;
                c=1;
            }
        }
    }
    *dst='\0';
}