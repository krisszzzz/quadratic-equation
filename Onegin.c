// Onegin.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define WINDOWS

#include "Onegin_functions.h"

int main()
{
	
	FILE* LOG_FILE = fopen("LOG_FILE.txt", "w");

	FILE* source = fopen("Translated_Onegin.txt", "rb");
	ptr_status(source, "Error: the file isn't open\n", LOG_FILE);

	long_int file_size = find_file_size(source);

	char* f_buffer = read_to_f_buffer(source, file_size);
	ptr_status(f_buffer, "Error: incorrect file reading\n", LOG_FILE);

	long_int num_of_strings = count_strings(f_buffer, file_size);

	string* ptr_string = calloc(num_of_strings, sizeof(string));
	ptr_status(ptr_string, "Incorrect memory allocation for string structure\n", LOG_FILE);

	read_strings(f_buffer, ptr_string, num_of_strings);

	FILE* destination = fopen("True_Onegin.txt", "wb");

	//qsort(ptr_string, num_of_strings, sizeof(string), (int(*)(const void*, const void*))comparator);

	my_qsort(ptr_string, num_of_strings, sizeof(string), (int(*)(void*, void*))comparator);
	output(ptr_string, num_of_strings, destination);
	
	make_indent(destination);

	my_qsort(ptr_string, num_of_strings, sizeof(string), (int(*)(void*, void*)) end_comparator);
	output(ptr_string, num_of_strings, destination);

	fclose(source);
	fclose(destination);
	fclose(LOG_FILE);

	free_memory(f_buffer, ptr_string, num_of_strings);

}


void ptr_status(void* ptr, const char* error, FILE* log_file)
{
	if (ptr == NULL)
	{
		fprintf(log_file, error);
		assert(ptr != NULL);
	}
}

int comparator(string* first, string* second)
{
	assert(first->line != NULL);
	assert(second->line != NULL);

	char* p_first = first->line;
	char* p_second = second->line;
	int is_equal = 1;

	while(*first->line != '\0' && *second->line != '\0')
	{
		if (!isalpha(*first->line))
			++first->line;
		if(!isalpha(*second->line))
			++second->line;

		if (isalpha(*first->line) && isalpha(*second->line))
		{
			if (*first->line != *second->line)
				break;
			else
			{
				++first->line;
				++second->line;
			}
		}
	}

	int c = tolower(*first->line) - tolower(*second->line);

	first->line = p_first;
	second->line = p_second;

	return c;
}
int end_comparator(string* first, string* second)
{
	
	char* p_first = first->line;
	char* p_second = second->line;

	first->line += first->line_size - 1;
	second->line += second->line_size - 1;


	while (first->line != p_first && second->line != p_second)
	{
		if (!isalpha(*first->line))
			--first->line;
		if (!isalpha(*second->line))
			--second->line;

		if (isalpha(*first->line) && isalpha(*second->line))
		{
			if (*first->line != *second->line)
				break;
			else
			{
				--first->line;
				--second->line;
			}
		}
	}
	int c = tolower(*first->line) - tolower(*second->line);

	first->line = p_first;
	second->line = p_second;

	return c;
}

long_int find_file_size(FILE* source)
{
	fseek(source, 0, SEEK_END);
	long_int file_size = ftell(source);
	fseek(source, 0, SEEK_SET);

	return file_size;
}
char* read_to_f_buffer(FILE* source, long_int file_size)
{
	char* f_buffer = calloc((size_t)file_size + 1, sizeof(char));

	if (f_buffer != NULL)
	{
		fread(f_buffer, sizeof(char), file_size, source);
		f_buffer[file_size] = '\0';
	}
	else 
		return NULL;

	return f_buffer;
}
long_int count_strings(char* buffer, long_int buffer_size)
{
	long_int i = 0;
	long_int num_of_strings = 0;
	while(i < buffer_size)
	{
		if(buffer[i] == '\n')
			num_of_strings++;

		++i;
	}
	return num_of_strings;
}

void read_strings(char* buffer, string* str, long_int num_of_string)
{
	#ifdef WINDOWS
		int num_of_ignored_char = 2;
	#elif
		int num_of_ignored_char = 1;
	#endif
	
	long_int string_index = 0;
	long_int readed_string_size = 0;
	char c;
	long_int size_of_string = 0;
	
	for(long_int buf_index = 0; (c =  buffer[buf_index]) != '\0'; ++buf_index)
	{
		if(c == '\n')
		{
			
			size_of_string = (buf_index + TRANSLATE_INDEX_TO_SIZE) - readed_string_size - num_of_ignored_char;  
			// num_of_ignored_char show what special symbol we ignore. For Windows we ignore 2 symbol \r and \n
			str[string_index].line = strndup(buffer + readed_string_size, size_of_string);
			str[string_index].line_size = size_of_string;

			readed_string_size = buf_index + TRANSLATE_INDEX_TO_SIZE; // because index start from zero we add 1 to
			// get a length of readed string
			string_index++;

		}
	}
}

char* strndup(char* src, long_int size)
{
	long_int i = 0;

	char* dst = calloc((size_t)size + 1, sizeof(char));

	assert(dst != NULL);

	while(i < size)
	{
		dst[i] = src[i];
		++i;
	}

	dst[size] = '\0';
	return dst;
}


void my_qsort(void* first, long_int number, size_t size, int (*compar)(void*, void*))
{
	long_int gap, i, j;

 	for (gap = number/2; gap > 0; gap /= 2)
 		for (i = gap; i < number; i++)
 			for (j = i - gap; j >= 0 && (*compar)((char*)first + j*size, (char*)first + (j+gap)*size) > 0; j -= gap) {
 				swap( (char*)first + j*size, (char*)first + (j+gap)*size, size);
		    }

}

void swap(void* a, void* b, size_t size)
{
	char* tmp = (char*)malloc(size);

	assert(tmp != NULL);
	assert(a != NULL);
	assert(b != NULL);

	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
	free(tmp);
}


void output(string* ptr_string, long_int num_of_string, FILE* dst)
{
	for (long_int i = 0; i < num_of_string; ++i)
		fprintf(dst, "%s\n", ptr_string[i].line);
}

void make_indent(FILE* dst)
{
	for(int i = 0; i < 50; ++i)
		fputc('\n', dst);
}

void free_memory(char* f_buffer, string* ptr_string, long_int num_of_string)
{

	free(f_buffer);
	for (size_t i = 0; i < num_of_string; ++i)
		free(ptr_string[i].line);
}
