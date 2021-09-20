// Onegin.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define WINDOWS
#pragma warning (disable: 4996)

enum 
{
	TRANSLATE_INDEX_TO_SIZE = 1
};
struct string
{
	size_t line_size;
	char* line;
};

size_t find_file_size(FILE* source);

char* read_to_f_buffer(FILE* source, size_t file_size);

size_t count_strings(char* buffer, size_t buffer_size);

void read_strings(char* buffer, struct string* str, size_t num_of_string);

char* strndup(char* src, size_t size);

void my_qsort(void* first, int num, int size, int (*compar)(void*, void*));

int comparator(struct string* first, struct string* second);

void swap(void* a, void* b, int size);

void output(struct string* ptr_string, size_t num_of_string, FILE* dst);

void free_memory(char* f_buffer, struct string* ptr_string, size_t num_of_string);

int main()
{

	FILE* source = fopen("Translated_Onegin.txt", "rb");

	size_t file_size = find_file_size(source);

	char* f_buffer = read_to_f_buffer(source, file_size);

	size_t num_of_strings = count_strings(f_buffer, file_size);

	struct string* ptr_string = calloc(num_of_strings, sizeof(struct string));

	read_strings(f_buffer, ptr_string, num_of_strings);

	my_qsort(ptr_string, num_of_strings, sizeof(struct string), comparator);

	FILE* destination = fopen("True_Onegin.txt", "wb");
	output(ptr_string, num_of_strings, destination);

	free_memory(f_buffer, ptr_string, num_of_strings);
}
int comparator(struct string* first, struct string* second)
{
	while (!isalpha(*(first->line)))
		++(first->line);

	while (!isalpha(*((*second).line)))
		++(second->line);

	for (int i = 0; tolower(first->line[i]) == tolower(second->line[i]); ++i)
		if (second->line[i] == '\0')
			return 0;

	return tolower(*((*first).line)) - tolower(*((*second).line));
}
size_t find_file_size(FILE* source)
{
	fseek(source, 0, SEEK_END);
	size_t file_size = ftell(source);
	fseek(source, 0, SEEK_SET);

	return file_size;
}
char* read_to_f_buffer(FILE* source, size_t file_size)
{
	char* f_buffer = calloc(file_size + 1, sizeof(char));
	fread(f_buffer, sizeof(char), file_size, source);
	f_buffer[file_size] = '\0';

	return f_buffer;
}
size_t count_strings(char* buffer, size_t buffer_size)
{
	size_t i = 0;
	size_t num_of_strings = 0;
	while(i < buffer_size)
	{
		if(buffer[i] == '\n')
			num_of_strings++;
		++i;
	}
	return num_of_strings;
}

void read_strings(char* buffer, struct string* str, size_t num_of_string)
{
	#ifdef WINDOWS
		int num_of_ignored_char = 2;
	#elif
		int num_of_ignored_char = 1;
	#endif
	
	int string_index = 0;
	size_t readed_string_size = 0;
	char c;
	size_t size_of_string = 0;
	
	for(size_t buf_index = 0; (c =  buffer[buf_index]) != '\0'; ++buf_index)
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
char* strndup(char* src, size_t size)
{
	size_t i = 0;

	char* dst = calloc(size + 1, sizeof(char));

	while(i < size)
	{
		dst[i] = src[i];
		++i;
	}

	dst[size] = '\0';
	return dst;
}

void my_qsort(void* first, int number, int size, int (*compar)(void*, void*))
{
	int gap, i, j;

 	for (gap = number/2; gap > 0; gap /= 2)
 		for (i = gap; i < number; i++)
 			for (j = i - gap; j >= 0 && (*compar)((char*)first + j*size, (char*)first + (j+gap)*size) > 0; j -= gap) {
 				swap( (char*)first + j*size, (char*)first + (j+gap)*size, size);
		    }

}


void swap(void* a, void* b, int size)
{
	char* tmp = (char*)malloc(size);
	memcpy(tmp, a, size);
	memcpy(a, b, size);
	memcpy(b, tmp, size);
	free(tmp);
}
void output(struct string* ptr_string, size_t num_of_string, FILE* dst)
{
	for (size_t i = 0; i < num_of_string; ++i)
		fprintf(dst, "%s\n", ptr_string[i].line);
}
void free_memory(char* f_buffer, struct string* ptr_string, size_t num_of_string)
{
	free(f_buffer);
	for (size_t i = 0; i < num_of_string; ++i)
		free(ptr_string[i].line);
}
