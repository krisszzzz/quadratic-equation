//
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
void general_info_stack_##elem_type (stack_##elem_type* stack_t, const char* name = nullptr,  FILE* LOG_FILE = nullptr)					\
{\
	int is_name_remembered = 0;\
    if(stack_t == nullptr)																					\
        return;\
	if(name != nullptr && !is_name_remembered)\
		static const char* elem_name = name;\
																											\
	if(LOG_FILE != nullptr && elem_name != nullptr)																					\
		fprintf(LOG_FILE, "Stack info: stack address = %p, stack name -  %s stack type - %s, stack size = %d," \
				" stack capacity = %d, stack data = %p\n", stack_t, elem_name, #elem_type,\
				stack_t->size, stack_t->capacity, stack_t->data);\
}\
\
\
void ctor_info_stack_##elem_type (stack_##elem_type* stack_t, FILE* LOG_FILE = nullptr)\
{\
    \
	if(LOG_FILE != nullptr)\
	{																												\
		fprintf(LOG_FILE, "Object with address %p was constructed\n", stack_t);										\
		general_info_stack_##elem_type (stack_t, LOG_FILE);															\
		fprintf(LOG_FILE, "Stack is constructed; number of constructor calling %d\n", num_of_call_##elem_type);		\
	}																												\
}																													\
void pop_info_stack_##elem_type (stack_##elem_type* stack_t, elem_type* value, FILE* LOG_FILE = nullptr)			\
{																													\
	if(LOG_FILE != nullptr)																							\
		general_info_stack_##elem_type (stack_t, LOG_FILE);															\
    if(LOG_FILE != nullptr)																							\
		fprintf(LOG_FILE, "Popped element addres = %p\n", value);													\
}																													\
																													\
																													\
void push_info_stack_##elem_type(stack_##elem_type* stack_t, elem_type* value, FILE* LOG_FILE = nullptr)\
{\
    general_info_stack_##elem_type(stack_t, LOG_FILE);\
    if(LOG_FILE != nullptr)\
		fprintf(LOG_FILE, "Pushed element addres = %p\n", value);\
}\
void memory_info_stack_##elem_type(stack_##elem_type* stack_t,\
int amount_of_memory, const char memory_mode, FILE* LOG_FILE = nullptr)	\
{																													\
if (LOG_FILE != nullptr)																							\
general_info_stack_##elem_type(stack_t, LOG_FILE);															\
    if(memory_mode == 'a')																							\
    {																												\
		if(LOG_FILE != nullptr)																				\
			fprintf(LOG_FILE, "memory allocated = %d\n", amount_of_memory);									\
    }																										\
    if(memory_mode == 'r')																					\
    {																										\
		if(LOG_FILE != nullptr)																				\
			fprintf(LOG_FILE, "memory reallocated = %d\n", amount_of_memory);								\
    }																										\
}																											\
																											\
																											\
void dtor_info_stack_##elem_type(stack_##elem_type* stack_t, FILE* LOG_FILE = nullptr)				\
{\
	if(LOG_FILE != nullptr)\
		fprintf(LOG_FILE, "Object with address %p destructed.", stack_t);									\
}																											\
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
		memory_info_stack_##elem_type(to_ctor, MIN_SIZE , 'a', LOG_FILE);						                        \
	}																								        \
	else {																							        \
	to_ctor->capacity = capacity;																		    \
	to_ctor->data = (elem_type*)calloc(capacity, sizeof(elem_type));                                        \
	memory_info_stack_##elem_type(to_ctor, capacity , 'a', LOG_FILE);											        \
	}																								        \
                                                                                                            \
	assert(to_ctor->data != nullptr);\
	ctor_info_stack_##elem_type (to_ctor, LOG_FILE);															        \
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
        memory_info_stack_##elem_type(stack_t, stack_t->capacity, 'r', LOG_FILE);\
		assert(stack_t->data != nullptr);															        \
	}\
	push_info_stack_##elem_type(stack_t, value, LOG_FILE);															 \
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
			memory_info_stack_##elem_type(stack_t, stack_t->capacity, 'r', LOG_FILE);                                 \
        }                                                                                                   \
         pop_info_stack_##elem_type(stack_t, &stack_t->data[stack_t->size], LOG_FILE);                                \
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
    dtor_info_stack_##elem_type(stack_t, LOG_FILE);															\
}																											\
stack_template(int)
int main()
{

}
