#include<stdlib.h>
#include<stdio.h>
#include<cassert>
#include<string.h>

#define DEBUG_LEVEL 2

enum WARNINGS
{
	STACK_UNDERFLOW,
    INCCORECT_INPUT_TO_CTOR,
	NOT_INITIALIZED_STACK,
	ALREADY_CONSTRUCTED

};
enum ERRORS
{
	MEMORY_ALLOCATED_ERROR = -13,
	INCCORECT_STACK,
	NULL_PTR_STACK

};

const int MIN_SIZE = 8;
const int OFFSET_ON_DELETING = 3;

void switch_errors(int error, const char* FUNCT, int LINE, FILE* LOG_FILE)
{
	assert(LOG_FILE != nullptr || !"Critical error: LOG_FILE is nullptr,"
	 "impossible to switch errors");
	fprintf(LOG_FILE, "switch_error caused by %s, on the line under the number %d\n", FUNCT, LINE);
	if(DEBUG_LEVEL == 2)
	{
		switch(error)
		{
			case NOT_INITIALIZED_STACK:
            case ALREADY_CONSTRUCTED:
			    fprintf(LOG_FILE, "WARNING: NOT_INITIALIZED_STACK or ALREADY_CONSTRUCTED,"
                                   "your stack has been automatically zeroed\n");
				break;

			case STACK_UNDERFLOW:
				fprintf(LOG_FILE, "WARNING: STACK_UNDERFLOW, pop function will return you zeroed stack\n");
				break;
            case INCCORECT_INPUT_TO_CTOR:
                fprintf(LOG_FILE, "WARNING: INCCORECT_INPUT_TO_CTOR, you passed the wrong capacity to the constructor\n");
		}

	}


    switch(error)
    {
        case INCCORECT_STACK:
			    fprintf(LOG_FILE, "ERROR: INCORRECT_STACK, your stack incorrect, the program was stoped\n");
				break;
        case NULL_PTR_STACK:
                fprintf(LOG_FILE, "ERROR: NULL_PTR_STACK, you passed to the function that mentioned before the nullptr-object\n");
                break;


    }


}

void* recalloc(void* block, size_t elem_count, size_t elem_size)
{
	return realloc(block, elem_count * elem_size);
}

#define malloc()
#define realloc()



#define stack_template(elem_type)                                                                                \
struct stack_##elem_type {																					           \
	elem_type* data;																				           \
	ssize_t size;																					           \
	ssize_t capacity;																				           \
                                                                                                               \
};																						   \
																											   \
																											   \
void general_info_stack_##elem_type (stack_##elem_type* stack_t, 											   \
									 FILE* LOG_FILE = nullptr, void (*printer)(elem_type* to_print, FILE* LOG_FILE) = nullptr)\
{                                                                                                              \
    static void (*stat_printer)(elem_type* to_print, FILE* LOG_FILE) = nullptr;                                                \
    if(stack_t == nullptr)																					   \
        switch_errors(NULL_PTR_STACK, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);                                \
    if(printer != nullptr)                                                                                     \
        stat_printer = printer;                                                                                \
																											   \
	if(LOG_FILE != nullptr)                                                                                    \
    {                                                                                                          \
		fprintf(LOG_FILE, "Stack info: stack address = %p, stack type - %s, stack size = %d,"                  \
				" stack capacity = %d, stack data = %p\n", stack_t, #elem_type,        \
				stack_t->size, stack_t->capacity, stack_t->data);                                              \
		for(int elem_of_data = 0; elem_of_data < stack_t->size; ++elem_of_data)                               \
		{                                                                                                      \
           fprintf(LOG_FILE, "&[%d] == %p\n", elem_of_data, &stack_t->data[elem_of_data]);                                   \
           if(stat_printer != nullptr)                                                                         \
           {                                                                                                   \
                fprintf(LOG_FILE, "*[%d] == ", elem_of_data);                                                                \
                stat_printer(&stack_t->data[elem_of_data], LOG_FILE);                                                    \
                fprintf(LOG_FILE, "\n");                                                                       \
           }                                                                                                   \
                                                                                                               \
		}                                                                                                      \
    } 																											\
     																											\
}																												\
																												\
																												\
void ctor_stack_##elem_type(stack_##elem_type* to_ctor, size_t capacity = 8,								\
                            FILE* LOG_FILE = nullptr, void (*printer)(elem_type* to_print, FILE* LOG_FILE) = nullptr)		\
{																									        \
     if(to_ctor == nullptr)                                                                                 \
        switch_errors(NULL_PTR_STACK, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);                    \
                                                                                                                \
	if(capacity < 0)\
		switch_errors(INCCORECT_INPUT_TO_CTOR, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);\
	if(DEBUG_LEVEL == 2)                                                                                    \
	{                                                                                                       \
		stack_##elem_type null_initialized = {0};                                                           \
		if(memcmp(to_ctor, &null_initialized, sizeof(elem_type)) != 0)                                      \
			switch_errors(NOT_INITIALIZED_STACK, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);					\
		*to_ctor = null_initialized; 																		\
	}                                                                                                       \
                                                                                                            \
	if(capacity < MIN_SIZE)																				    \
	{																								        \
		to_ctor->capacity = MIN_SIZE;																        \
		to_ctor->data = (elem_type*)calloc(MIN_SIZE, sizeof(elem_type));                                    \
	}																								        \
	else {																							        \
	to_ctor->capacity = capacity;																		    \
	to_ctor->data = (elem_type*)calloc(capacity, sizeof(elem_type));                                        \
	}																								        \
                                                                                                            \
	if(to_ctor->data == nullptr) {\
		switch_errors(MEMORY_ALLOCATED_ERROR, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);                     \
		assert(to_ctor->data != nullptr);                                                                   \
	}\
	general_info_stack_##elem_type(to_ctor, LOG_FILE, printer);                                             \
}																									        \
                    																				        \
                                                                                                            \
int invalid_stack_##elem_type(stack_##elem_type* stack_t, FILE* LOG_FILE = nullptr)					        \
{																									        \
	if(stack_t == nullptr)\
		switch_errors(NULL_PTR_STACK, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);\
		return 1;\
	if(stack_t->size >= stack_t->capacity || stack_t->size < 0)\
		switch_errors(INCCORECT_STACK, __PRETTY_FUNCTION__, __LINE__, LOG_FILE);\
		assert(!"STACK INCCORECT, THE PROGRAMM WAS STOPPED");\
		return 1;\
	\
	return 0;\
}																									        \
                                                                                                            \
void stack_push_##elem_type(stack_##elem_type* stack_t, elem_type* value, FILE* LOG_FILE = nullptr)		    \
{																									        \
	if (stack_t->size < stack_t->capacity)\
		stack_t->data[stack_t->size++] = *value;\
	else																							        \
	{																								        \
		stack_t->data = (elem_type*)recalloc(stack_t->data, 2*stack_t->capacity, sizeof(elem_type));  \
		stack_t->capacity *= 2;\
		stack_t->data[stack_t->size++] = *value;\
		assert(stack_t->data != nullptr);															        \
	}\
	general_info_stack_##elem_type(stack_t, LOG_FILE);\
}                                                                                                           \
                                                                                                            \
int is_empty_stack_##elem_type(stack_##elem_type* stack_t)											        \
{																									        \
	assert(stack_t != nullptr);																		        \
	return (stack_t->size = 0) ? 1 : 0;																        \
}																									        \
                                                                                                            \
elem_type stack_pop_##elem_type(stack_##elem_type* stack_t, FILE* LOG_FILE = nullptr)					        \
{																									        \
	if(!is_empty_stack_##elem_type(stack_t))														        \
	{																								        \
		if(stack_t->size <= stack_t->capacity / 2 - OFFSET_ON_DELETING && stack_t->capacity/ 2 > MIN_SIZE)\
        {\
			stack_t->data = (elem_type*)recalloc(stack_t->data, stack_t->capacity / 2, sizeof(elem_type));  \
			stack_t->capacity /= 2;                                                                         \
        }                                                                                                   \
		if(DEBUG_LEVEL == 2)   {                                                                            \
			elem_type to_return =  stack_t->data[stack_t->size];                                            \
			elem_type null_initialized = {0};                                                               \
			stack_t->data[stack_t->size--] = null_initialized;                                              \
			return to_return;                                                                               \
                                                                                                            \
		}                                                                                                   \
		else                                                                                                \
			return stack_t->data[stack_t->size--];                                                          \
                                                                                                            \
	}																								        \
	else																							        \
	{																								        \
	    elem_type null_initialized = {0};																	\
		return null_initialized;																			\
    }																										\
}																											\
void dtor_stack_##elem_type(stack_##elem_type* stack_t, FILE* LOG_FILE = nullptr)							\
{																											\
    stack_t->size = stack_t->capacity = 0;\
    free(stack_t->data);\
    stack_t->data = nullptr;																				\
}


stack_template(int)
void printer(int* to_pr, FILE* log)
{
    fprintf(log, "%d", *to_pr);
}

int main()
{
    int a = -5, b = -1, c = 3, d = 5;
    FILE* log = fopen("log_file.txt", "w");
	stack_int A = {};
	ctor_stack_int(&A, 16, log, printer);
	int* p = &a;
	stack_push_int(&A, &a, log);
	p = &b;
	stack_push_int(&A, p, log);
}
