#include<stdlib.h>
#include<stdio.h>
#include<cassert>
#include<string.h>

#pragma warning (disable: 4996)

enum ERRORS
{
	STACK_UNDERFLOW,
	INCCORECT_STACK,

};

const int MIN_SIZE = 8;
const int OFFSET_ON_DELETING = 3;

void switch_errors(int error, FILE* LOG_FILE)
{
	assert(LOG_FILE != nullptr || "Critical error: LOG_FILE is nullptr,"
	 "impossible to switch errors" == 0);
	switch(error)
	{
		STACK_UNDERFLOW:
			fprintf(LOG_FILE, "WARNING: STACK_UNDERFLOW, please don't use pop function"
			"if stack is empty\n");
			break;
		INCCORECT_STACK:
			fprintf(LOG_FILE, "WARNING: INVALID_STACK, your stack was nulled, "
			"because he was inccorect\n");
	}
}

void* recalloc(void* block, size_t elem_count, size_t elem_size)
{
	return realloc(block, elem_count * elem_size);
}

#define malloc()
#define realloc()



#define stack_template(elem_type)																	        \
typedef struct {																					        \
	elem_type* data;																				        \
	ssize_t size;																					        \
	ssize_t capacity;																				        \
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
void ctor_stack_##elem_type(stack_##elem_type* to_ctor, size_t capacity = 8,\
                            void (*printer)(elem_type* to_print) = nullptr,\
                             int* err = nullptr, FILE* LOG_FILE = nullptr)	\
{																									        \
                                                                                                            \
	if(DEBUG_LELEV >= 1)\
	{\
		elem_type null_initialized = {0};\
		if(memcmp(to_ctor, &null_initialized) != 0)\
			\
	}\																										
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
                                                                                                            \
int invalid_stack_##elem_type(stack_##elem_type* stack_t)											        \
{																									        \
	\
}																									        \
                                                                                                            \
void stack_push_##elem_type(stack_##elem_type* stack_t, elem_type* value, FILE* LOG_FILE = nullptr)							        \
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
}                                                                                                           \
                                                                                                            \
int is_empty_stack_##elem_type(stack_##elem_type* stack_t)											        \
{																									        \
	assert(stack_t != nullptr);																		        \
	return (stack_t->size = 0) ? 1 : 0;																        \
                                                                                                            \
}																									        \
                                                                                                            \
elem_type stack_pop_##elem_type(stack_##elem_type* stack_t, int* err = nullptr, FILE* LOG_FILE = nullptr)					        \
{																									        \
	if(!is_empty_stack_##elem_type(stack_t))														        \
	{																								        \
		if(stack_t->size <= stack_t->capacity / 2 - OFFSET_ON_DELETING && stack_t->capacity/ 2 > MIN_SIZE)\
        {\
			stack_t->data = (elem_type*)recalloc(stack_t->data, stack_t->capacity / 2, sizeof(elem_type));\
			stack_t->capacity /= 2;\
        }\
		if(DEBUG_LEVEL >= 1)   {                                                                               \
			elem_type to_return =  stack->data[stack->size];\
			elem_type null_inititialized = {0};\
			stack->data[stack->size--] = null_initialized;\
			return to_return;\
			\
		}\
		else\
			return stack->data[stack->size--];\
                                                                                                            \
	}																								        \
	else																							        \
	{																								        \
	    *err = STACK_UNDERFLOW;	                                                                            \
	    elem_type null_inititialized = {0};																	\
		return null_initialized;																			\
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
