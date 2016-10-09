#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>     //  va_start(), va_end(), va_arg()

#include "table.h"

#ifndef SYMBOL_TYPE_ERROR
	#define SYMBOL_TYPE_ERROR -128
#endif

struct Table* createtable(int col_quantity, int* cols_size, char* cols_alignment)
{
	struct Table* tbl = (struct Table*)malloc(sizeof(struct Table));
	tbl->col_quantity = col_quantity;
	tbl->row_fill_char	= ' ';
	tbl->col_seporator_char = '|';
	tbl->row_seporator_char = '-';
	tbl->crossing_char = '+';
	// tbl->ignore_col_size = 0;
	tbl->cols_size = (int*)malloc(sizeof(int)*col_quantity);
	memcpy(tbl->cols_size, cols_size, sizeof(int)*col_quantity);
	tbl->cols_alignment = (char*)malloc(sizeof(char)*col_quantity);
	memcpy(tbl->cols_alignment, cols_alignment, sizeof(int)*col_quantity);
	// размер строки по 2 байта на символ для много байтных символов
	// + 3 байта на каждый разделитеь колонок
	// + 4 байта на границы таблицы + 2 байта '\n'+'\0'
	tbl->_row_size = 3*(tbl->col_quantity-1)+4+2;
	for (int i = 0; i < tbl->col_quantity; ++i)
		tbl->_row_size += tbl->cols_size[i]*2;
	return tbl;
}

void deletetable(struct Table* tbl)
{
	free(tbl->cols_size);
	free(tbl->cols_alignment);
	free(tbl);
}

void putmem(char* str, int s)
{
	for (int i = 0; i < s; ++i)
	{
		printf("%p %02X %c\n",&str[i], (unsigned char)str[i], str[i]);
	}
}

char* getrowstr(struct Table* tbl, char* str, const char* cols)
{
	char* p = str;
	char* sp = str;
	// putmem(cols, tbl->_row_size);
	// char* end = str + tbl->_row_size;
	memset(str, tbl->row_fill_char, tbl->_row_size);
	*(sp) = tbl->col_seporator_char;
	// p = sp+1;
	// printf("end %p\n", str + tbl->_row_size);
	for (int i = 0; i < tbl->col_quantity; ++i)
	{
		int len = strlen(cols);
		// printf("len %d ", len);
		int shift, cpysize;
		if (len > 0 )
		{
			// printf("cols %p ", cols);
			shift = _getalignedpos(tbl->cols_size[i], tbl->cols_alignment[i], cols);
			// printf("shift %d\n", shift);
			if (shift != SYMBOL_TYPE_ERROR)
			{
				p = sp + 2 + shift;
				if (len > str + tbl->_row_size - p - 3)
					cpysize = str + tbl->_row_size - p - 3;
				else
					cpysize = len;
				// printf("%d cpysize %d\n",i, cpysize );
				strncpy(p, cols, cpysize);
				p += cpysize;
				*p = '\0';
				sp += cpysize+(tbl->cols_size[i]-_mbstrlen(cols))+3;
				*p = tbl->row_fill_char;
				if (tbl->cols_alignment == 0)
					sp += shift + (tbl->cols_size[i]-_mbstrlen(cols))%2;
				
			}
			else
				sp += tbl->cols_size[i]+3;
		}
		else
		{
			sp += tbl->cols_size[i]+3;
		}
		cols += len+1;
		// printf("p %p sp %p c %c %02X\n", p, sp, *cols, (unsigned char)*cols);
		if (p < sp)
		{
			*(sp) = tbl->col_seporator_char;
		}
	}
	if (p > sp)
		sp = p;
	*(++sp) = '\n';
	*(++sp) = '\0';
	// putmem(str, tbl->_row_size);
	return str;
}

char* getseparatorrow(struct Table* tbl, char* str)
{
	char* p = str;
	*(p++) = tbl->crossing_char;
	for (int i = 0; i < tbl->col_quantity; ++i)
	{
		memset(p, tbl->row_seporator_char, tbl->cols_size[i]+2);
		p+=tbl->cols_size[i]+2;
		*(p++) = tbl->crossing_char;
	}
	*(p++) = '\n';
	*(p++) = '\0';
	return str;
}


void fputrow(struct Table* tbl, const char* cols, FILE* F)
{
	char* str = (char*)malloc(sizeof(char)*tbl->_row_size);
	str = getrowstr(tbl, str, cols);
	fputs(str, F);
	free(str);
}

void fputseparatorrow(struct Table* tbl, FILE* F)
{
	char* str = (char*)malloc(sizeof(char)*tbl->_row_size);
	str = getseparatorrow(tbl, str);
	fputs(str, F);
	free(str);
}

char* _setemprypow(struct Table* tbl, char* str)
{
	char* p = str;
	*(p++) = tbl->row_seporator_char;
	for (int i = 0; i < tbl->col_quantity; ++i)
	{
		memset(p, tbl->row_fill_char, tbl->cols_size[i]+2);
		p+=tbl->cols_size[i]+2;
		*(p++) = tbl->row_seporator_char;
	}
	*(p++) = '\n';
	*(p++) = '\0';
	return str;
}

int _getalignedpos(int col_size, int alignment, const char* str)
{
	if (alignment == -1)
		return 0;
	int len = _mbstrlen(str);
	if (len<0) 
		return SYMBOL_TYPE_ERROR;
	if (alignment == 1)
		return col_size-len;
	return (col_size-len)/2;
}

int _mbstrlen(const char* str)
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