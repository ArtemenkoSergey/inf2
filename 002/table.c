

#include "table.h"


struct Table* createTable(int colQuantity, int* colsSize, char* colsAlignment)
{
	struct Table* tbl = (struct Table*)malloc(sizeof(struct Table));
	tbl->colQuantity = colQuantity;
	tbl->rowFillChar	= ' ';
	tbl->colSeporatorChar = '-';
	tbl->rowSeporatorChar = '|';
	tbl->crossingChar = '+';
	// tbl->ignore_col_size = 0;
	tbl->colsSize = (int*)malloc(sizeof(int)*colQuantity);
	memcpy(tbl->colsSize, colsSize, sizeof(int)*colQuantity);
	tbl->colsAlignment = (char*)malloc(sizeof(char)*colQuantity);
	memcpy(tbl->colsAlignment, colsAlignment, sizeof(int)*colQuantity);
	// размер строки по 2 байта на символ для много байтных символов
	// + 3 байта на каждый разделитеь колонок
	// + 4 байта на границы таблицы + 2 байта '\n'+'\0'
	tbl->_rowSize = 3*(tbl->colQuantity-1)+4+2;
	for (int i = 0; i < tbl->colQuantity; ++i)
		tbl->_rowSize += tbl->colsSize[i]*2;
	return tbl;
}

void deleteTable(struct Table* tbl)
{
	free(tbl->colsSize);
	free(tbl->colsAlignment);
	free(tbl);
}

//отладочная для простотра памяти
void putmem(char* str, int s)
{
	for (int i = 0; i < s; ++i)
	{
		printf("%p %02X %c\n",&str[i], (unsigned char)str[i], str[i]);
	}
}

char* getRow(struct Table* tbl, int colsCount, ...)
{
	char* row = (char*)malloc(sizeof(char)*tbl->_rowSize);
	memset(row, tbl->rowFillChar, tbl->_rowSize);

	va_list ap;
	va_start(ap, colsCount);
	
	char* sp = row; // позиция разделителя столбцов.
	char* colText;

	*sp = tbl->rowSeporatorChar;	
	for (int i = 0; i < colsCount; ++i)
	{
		colText = va_arg(ap, char*);
		sp = _setCol(tbl, i, row, sp, colText);
	}

	if (colsCount < tbl->colQuantity)
	{
		for (int i = colsCount; i < tbl->colQuantity; ++i)
			sp = _setCol(tbl, i, row, sp, "");
	}

	*(++sp) = '\n';
	*(++sp) = '\0';

	va_end(ap);
	return row;
}

char* getSeparatorRow(struct Table* tbl, char* str)
{
	char* p = str;
	*(p++) = tbl->crossingChar;
	for (int i = 0; i < tbl->colQuantity; ++i)
	{
		memset(p, tbl->colSeporatorChar, tbl->colsSize[i]+2);
		p+=tbl->colsSize[i]+2;
		*(p++) = tbl->crossingChar;
	}
	*(p++) = '\n';
	*(p++) = '\0';
	return str;
}


// void fputRow(struct Table* tbl, const char* cols, FILE* F)
// {
// 	char* str = (char*)malloc(sizeof(char)*tbl->_rowSize);
// 	str = getRowStr(tbl, str, cols);
	

// 	fputs(str, F);
// 	free(str);
// }

void fputRow(struct Table* tbl, FILE* fd, int colsCount, ...)
{

	char* row = (char*)malloc(sizeof(char)*tbl->_rowSize);
	memset(row, tbl->rowFillChar, tbl->_rowSize);

	va_list ap;
    va_start(ap, colsCount);

    char* sp = row; // позиция разделителя столбцов.
	char* colText;

	*sp = tbl->rowSeporatorChar;	
	for (int i = 0; i < colsCount; ++i)
	{
		colText = va_arg(ap, char*);
		sp = _setCol(tbl, i, row, sp, colText);
	}

	if (colsCount < tbl->colQuantity)
	{
		for (int i = colsCount; i < tbl->colQuantity; ++i)
			sp = _setCol(tbl, i, row, sp, "");
	}

	*(++sp) = '\n';
	*(++sp) = '\0';

	fputs(row, fd);

    va_end(ap);
    free(row);
}

void fputSeparatorRow(struct Table* tbl, FILE* fd)
{
	char* str = (char*)malloc(sizeof(char)*tbl->_rowSize);
	str = getSeparatorRow(tbl, str);
	fputs(str, fd);
	free(str);
}

int _getAlignedPos(int col_size, int alignment, const char* str)
{
	if (alignment == -1)
		return 2;
	int len = _mbStrLen(str);
	if (len<0) 
		return SYMBOL_TYPE_ERROR;
	if (alignment == 1)
		return col_size-len+2;
	return (col_size-len)/2+(col_size-len)%2+2;
}

char* _setCol(struct Table* tbl, int colNum, char* row, char* sp, const char* colText)
{
	char* p; 		// позиция содержимого колонки в строке.
	int shift;		// сдвиг позиции содержимого колонки от разделителя
	size_t cpySize;	// количество копируемых байт.

	shift = _getAlignedPos(tbl->colsSize[colNum], tbl->colsAlignment[colNum], colText);
	if (shift == SYMBOL_TYPE_ERROR)
	{
		sp += tbl->colsSize[colNum]+3;
	}
	else
	{	
		p = sp + shift;
		// контроль выхода за начало строки таблицы (левую границу)
		if (p < row)
		{
			colText -= p-row;
			p = row;
		}
			// контроль переполнения строки таблицы
		if (p+strlen(colText) > row+tbl->_rowSize)
			cpySize = row+tbl->_rowSize - p;
		else
			cpySize = strlen(colText);
		// printf("%d\n", cpySize);
		strncpy(p, colText, cpySize);
		// printf("%d %d\n", strlen(colText), _mbStrLen(colText) );
		sp += cpySize+(tbl->colsSize[colNum]-_mbStrLen(colText))+3;
		// sp += cpySize+(tbl->colsSize[colNum]-strlen(colText))+3;
	}

	if (p+cpySize < sp)
	{
		*(sp) = tbl->rowSeporatorChar;
	}
	return sp;
}

int _mbStrLen(const char* str)
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