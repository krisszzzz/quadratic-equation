#include<stdlib.h>
#include<stdio.h>
#include<assert.h>

#pragma warning (disable: 4996)

enum ERRORS
{
	STACK_UNDERFLOW
};

const int MIN_SIZE = 8;
const int OFFSET_ON_DELETING = 3;



void* recalloc(void* block, size_t elem_count, size_t elem_size)
{
	return realloc(block, elem_count * elem_size);
}

#define malloc()
#define realloc()



#define stack_template(elem_type)																	        \
typedef struct {																					        \
	elem_type* data;																				        \
	size_t size;																					        \
	size_t capacity;																				        \
                                                                                                            \
} stack_##elem_type;																						\
																											\
\
void general_info_stack_##elem_type (stack_##elem_type* stack_t, 											\
									 void (*printer)(elem_type* to_print) = nullptr,  FILE* LOG_FILE = nullptr)\
{                                                                                                            \
    static void (*stat_printer)(elem_type* to_print) = nullptr;                                                \
	int is_name_remembered = 0;                                                                               \
    if(stack_t == nullptr)																					   \
        return;                                                                                               \
    if(printer != nullptr)                                                                                     \
        stat_printer = printer;                                                                                \
																											   \
	if(LOG_FILE != nullptr)                                                                                    \
    {                                                                                                         \
		fprintf(LOG_FILE, "Stack info: stack address = %p, stack type - %s, stack size = %d,"                  \
				" stack capacity = %d, stack data = %p\n, address of elements - ", stack_t, #elem_type,        \
				stack_t->size, stack_t->capacity, stack_t->data);                                              \
		for(int elem_of_data = 0; elem_of_data <= stack_t->size; ++elem_of_data)                               \
		{                                                                                                      \
           fprintf(LOG_FILE, "&[%d] == %p\n", elem_of_data, &stack_t->data);                                   \
           if(stat_printer != nullptr)                                                                         \
           {                                                                                                   \
                fprintf(LOG_FILE, "*[%d] == ");                                                                \
                stat_printer(&stack_t->data[elem_of_data]);                                                    \
                fprintf(LOG_FILE, "\n");                                                                       \
           }                                                                                                   \
                                                                                                               \
		}                                                                                                      \
    } \
     \
}	\
	\
\
void ctor_stack_##elem_type_(stack_##elem_type* to_ctor, size_t capacity = 8, char* elem_name, int* err = nullptr, FILE* LOG_FILE = nullptr)	\
{																									        \
                                                                                                            \
	to_ctor->size = 0;																				        \
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
	assert(to_ctor->data != nullptr);\
}																									        \
                                                                                                            \
int invalid_stack_##elem_type(stack_##elem_type* stack_t)											        \
{																									        \
	return (stack_t->size > stack_t->capacity) ? 1 : 0;												        \
}																									        \
                                                                                                            \
void stack_push_##elem_type(stack_##elem_type* stack_t, elem_type* value, FILE* LOG_FILE = nullptr)							        \
{																									        \
	if (stack_t->size < stack_t->capacity)\
		stack_t->data[stack_t->size++] = *value;\
	else																							        \
	{																								        \
		assert(stack_t->size == stack_t->capacity);													        \
		stack_t->data = (elem_type*)recalloc(stack_t->data, 2*stack_t->capacity, sizeof(elem_type));  \
		stack_t->capacity *= 2;\
		stack_t->data[stack_t->size++] = *value;\
		assert(stack_t->data != nullptr);															        \
	}\
}                                                                                                           \
                                                                                                            \
int is_empty_stack_##elem_type(stack_##elem_type* stack_t)											        \
{																									        \
	assert(stack_t != nullptr);																		        \
	return (stack_t->size = 0) ? 1 : 0;																        \
                                                                                                            \
}																									        \
                                                                                                            \
elem_type* stack_pop_##elem_type(stack_##elem_type* stack_t, int* err = nullptr, FILE* LOG_FILE = nullptr)					        \
{																									        \
	if(!is_empty_stack_##elem_type(stack_t))														        \
	{																								        \
		if(stack_t->size <= stack_t->capacity / 2 - OFFSET_ON_DELETING && stack_t->capacity/ 2 > MIN_SIZE)\
        {\
			stack_t->data = (elem_type*)recalloc(stack_t->data, stack_t->capacity / 2, sizeof(elem_type));\
			stack_t->capacity /= 2;\
        }                                                                                                   \
		return &stack_t->data[stack_t->size--];														        \
                                                                                                            \
	}																								        \
	else																							        \
	{																								        \
	    *err = STACK_UNDERFLOW;	                                                                            \
	    return nullptr;                                                                                     \
    }																										\
}																											\
void dtor_stack_##elem_type(stack_##elem_type* stack_t, FILE* LOG_FILE = nullptr)							\
{																											\
    stack_t->size = stack_t->capacity = 0;\
    free(stack_t->data);\
    stack_t->data = nullptr;																				\
}																											\
stack_template(int)
int main()
{

}
