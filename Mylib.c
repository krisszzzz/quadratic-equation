#include "MyLib.h"

int my_puts(const char* str) 
{
	/// @param str my_puts output str to console and end this with new line
	for (int index = 0; str[index] != '\0'; ++index)
	{
		if (putchar(str[index]) == EOF)
			return EOF;
	}
	putchar('\n');
	return 1;
}
char* my_strchr(char* str, int ch)
{
	
	while (*str != '\0' && *str != ch)
		str++;

	if (*str == ch)
		return str;
	else
		return NULL;
}
int my_strlen(const char* str)
{
	int length = 0;

	while (str[length] != '\0')
		length++;
	return length;
}
char* my_strcpy(char* dst, const char* src)
{
	int index = 0;
	while ((dst[index] = src[index]) != '\0')
		++index;
		
	return dst;
}
char* my_strncpy(char* dst, const char* src, int len)
{
	int index;
	for (index = 0; index < len && src[index] != '\0'; ++index)
	{
		dst[index] = src[index];
	}

	while (index < len)
	{
		dst[index] = '\0';
		++index;
	}

	return dst;
}
char* my_strcat(char* destination, const char* append)
{
	int size = my_strlen(destination);

	my_strcpy(&destination[size], append);

	return destination;
}
char* my_strncat(char* destination, const char* append, int n)
{
	int size = my_strlen(destination);

	my_strncpy(&destination[size], append, n);

	return destination;
}

char* my_fgets(char* buffer, int n, FILE* stream)
{
	int i = 0;
	char symb = '\0'; // initial value of symb

	while(i < n - 1) 
	{
		symb = getc(stream);

		if(symb == '\n' || symb == EOF)
			break;
		else
			buffer[i] = symb;

		++i;
	}
	
	if(ferror(stream))
		return NULL;

	if(feof(stream) && i == 0) //the file end up, but no one symbol has been read (number of iteration is 0)
		return NULL;

	buffer[i] = '\0';

	return buffer;	

}
char* my_strdup(char* str)
{
	char *p = (char*) malloc(my_strlen(str) + 1);

	if(p == NULL)
		return NULL;

	for(int n = 0; n < my_strlen(str) + 1; ++n)
		p[n] = str[n];

	return p;
}
int my_getline(char** p_buffer, size_t* buffer_size, FILE* source)
{
	int c = 0;
	fseek(source, 0, SEEK_END);

	int size = ftell(source);

	fseek(source, 0, SEEK_SET);
	
	if(*p_buffer == NULL && *buffer_size != 0)
		return EOF;

	else if(*p_buffer == NULL && *buffer_size == 0)
		{
			char* p_temp = (char*)malloc(size + 1);
			p_buffer = &p_temp;
		}

	else if(*buffer_size <= size)
	{
		char* p_temp = (char*)realloc(*p_buffer, size + 1);
		p_buffer = &p_temp;
	}

	int i;

	for(i = 0; i < size; ++i)
		{
			c = getc(source);

			if(c == EOF)
				return EOF;

			if(c != '\n')
				(*p_buffer)[i] = c;
			else 
				break;
		}

	if(i < size && i > *buffer_size) // delete unused memory
	{
		char* p_temp = (char*)realloc(*p_buffer, i + 1);
		p_buffer = &p_temp;
	}

	(*p_buffer)[i] = '\0';

	return i;
}