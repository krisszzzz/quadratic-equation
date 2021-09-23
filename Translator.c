// Translator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include<stdio.h>
#include<ctype.h>
#include <stdlib.h>
#include<assert.h>

#define WINDOWS
typedef long long long_int;

enum
{
	IGNORE = -2 // We use -2, because EOF = -1
};

void make_file(char* buffer, long_int file_size);

char* read_to_f_buffer(FILE* source, long_int file_size);

long_int find_file_size(FILE* source);

void make_correct_file_end(char* buffer, long_int file_size);

void ignore_empty_first_line(char* buffer, long_int file_size);

void correct_lines(char* buffer, long_int file_size);

void make_file(char* buffer, long_int file_size);

void write_to_file(char* file_buffer, FILE* destination);

int main()
{
	FILE* source = fopen("Onegin.txt", "rb");
	assert(source);

	long_int file_size = find_file_size(source);
	assert(file_size > 0);

	char* file_buffer = read_to_f_buffer(source, file_size);

	make_file(file_buffer, file_size);

	FILE* destination = fopen("Translated_Onegin.txt", "wb");
	assert(destination);

	write_to_file(file_buffer, destination);
	
	fclose(source);
	fclose(destination);
}

char* read_to_f_buffer(FILE* source, long_int file_size)
{
	char* f_buffer = calloc(file_size + 3, sizeof(char));
	assert(f_buffer);

	long_int num_of_readed_sym = fread(f_buffer, sizeof(char), file_size, source);

	assert(num_of_readed_sym == file_size);

	f_buffer[file_size] = '\0';

	return f_buffer;
}
long_int find_file_size(FILE* source)
{
	fseek(source, 0, SEEK_END);
	long_int file_size = ftell(source);
	fseek(source, 0, SEEK_SET);

	return file_size;
}
void make_correct_file_end(char* buffer, long_int file_size)
{
	if (buffer[file_size - 1] != '\n')
	{
		#ifdef WINDOWS

		buffer[file_size] = '\r';
		buffer[file_size + 1] = '\n';
		buffer[file_size + 2] = '\0';

		#elif

		buffer[file_size] = '\n';
		buffer[file_size + 1] = '\0';

		#endif
	}
}


void ignore_empty_first_line(char* buffer, long_int file_size)
{
	long_int i;
	for(i = 0; isspace(buffer[i]); ++i)
		buffer[i] = IGNORE;
	
	if(buffer[i] == '\n')
		buffer[i] = IGNORE;
	
}
void correct_lines(char* buffer, long_int file_size)
{
	long_int index = 0;
	while (buffer[index] != '\0')
	{
		if (buffer[index] <= 0)
				buffer[index] = IGNORE;

		if (buffer[index] == '\n')
		{
			
			while (isspace(buffer[++index]))
				buffer[index] = IGNORE;

			if(buffer[index] == '\n')
				buffer[index] = IGNORE;

		}
		++index;
	}
}

void make_file(char* buffer, long_int file_size)
{
	make_correct_file_end(buffer, file_size);

	ignore_empty_first_line(buffer, file_size);

	correct_lines(buffer, file_size);

}
void write_to_file(char* file_buffer, FILE* destination)
{
	for(int i = 0; file_buffer[i] != '\0'; ++i)
	{
		if(file_buffer[i] != IGNORE)
			fputc(file_buffer[i], destination);
	}
}