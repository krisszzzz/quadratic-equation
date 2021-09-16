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

static size_t num_of_char_in_file(FILE* source)
{
	fseek(source, 0, SEEK_END);

	size_t length = (size_t)ftell(source);

	fseek(source, 0, SEEK_SET);
	return length;
}

static int buffer_size_setting(char** p_buffer, size_t* buffer_size, size_t char_in_file)
{
	if(*p_buffer == NULL && *buffer_size != 0)
		return EOF;

	else if(*p_buffer == NULL && *buffer_size == 0)
	{
			*p_buffer = (char*)malloc(char_in_file + 1);
			*buffer_size = char_in_file + 1;
			return 1;
	}

	else if(*buffer_size <= char_in_file)
	{
		*p_buffer = (char*)realloc(*p_buffer, char_in_file + 1);
		*buffer_size = char_in_file + 1;
		return 1;
	}
	else
		return 0;
}
static size_t file_read(char** buffer, FILE* source, size_t char_in_file)
{
	int i;
	int c = 0;

	for(i = 0; i < char_in_file; ++i)
		{
			c = getc(source);

			if(c == EOF)
				return EOF;

			(*buffer)[i] = c;

			if(c == '\n')
				break;
		}


	if(i == char_in_file)
	{
		(*buffer)[i] = '\0';

		return char_in_file;
	}
	else
		{
			(*buffer)[++i] = '\0';
			return i;
		}

}

int my_getline(char** p_buffer, size_t* buffer_size, FILE* source)
{

	size_t char_in_file = num_of_char_in_file(source);
	int need_to_set_buffer = buffer_size_setting(p_buffer, buffer_size, char_in_file); // return 1, if buffer size setted by
													//function or 0 if a buffer size is enough and correct, return EOF if error
	if(need_to_set_buffer == EOF)
		return EOF;
	
	size_t num_of_r_elem = file_read(p_buffer, source, char_in_file);

	if(num_of_r_elem == EOF)
		return EOF;


	if(need_to_set_buffer && num_of_r_elem < char_in_file + 1) 

	{
		*p_buffer = realloc(*p_buffer, num_of_r_elem + 1);
		*buffer_size = num_of_r_elem + 1;
	}

	return (int)num_of_r_elem;
}
