// Translator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include<stdio.h>
#include<ctype.h>
#include <stdlib.h>
char* read_to_f_buffer(FILE* source, size_t file_size)
{
	char* f_buffer = calloc(file_size + 3, sizeof(char));
	fread(f_buffer, sizeof(char), file_size, source);
	f_buffer[file_size] = '\0';

	return f_buffer;
}
size_t find_file_size(FILE* source)
{
	fseek(source, 0, SEEK_END);
	size_t file_size = ftell(source);
	fseek(source, 0, SEEK_SET);

	return file_size;
}
void make_file(char* buffer,size_t file_size)
{
	int index = 0;
	if (buffer[file_size - 1] != '\n')
	{

		buffer[file_size] = '\r';
		buffer[file_size + 1] = '\n';
		buffer[file_size + 2] = '\0';
	}
	int i;
	for(i = 0; isspace(buffer[i]); ++i)
	{


		buffer[i] = EOF;
	}
	if(buffer[i] == '\n')
	{
		buffer[i] = EOF;
	}
	while (buffer[index] != '\0')
	{
		if (buffer[index] == '\n')
		{
			int temp = index;
			while (isspace(buffer[++index]))
			{
				buffer[index] = EOF;
			}

			if(buffer[index] == '\n')
				buffer[index] = EOF;

		}
		++index;

	}
}
int main()
{
	FILE* source = fopen("onegin.txt", "rb");
	size_t file_size = find_file_size(source);
	char* file_buffer = read_to_f_buffer(source, file_size);
	make_file(file_buffer, file_size);
	FILE* destination = fopen("Translated_Onegin.txt", "wb");
	for(int i = 0; file_buffer[i] != '\0'; ++i)
	{
		if(file_buffer[i] != EOF)
			fputc(file_buffer[i], destination);
	}

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
