#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<string.h>

#define DEBUG_LEVEL 2
 void remember_log_file(FILE* log_file = nullptr);
 void forget_log_file();
 int print_to_log(const char* format ...);
 FILE* unsafe_remember_log_file(FILE* log_file_arg = nullptr);
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

void switch_errors_(int error, const char* funct, int line)
{

	print_to_log("switch_error caused by %s, on the line under the number %d\n", funct, line);
	if(DEBUG_LEVEL == 2)
	{
		switch(error)
		{
			case NOT_INITIALIZED_STACK:
            case ALREADY_CONSTRUCTED:
			    print_to_log("WARNING: NOT_INITIALIZED_STACK or ALREADY_CONSTRUCTED, "
                                   "your stack has been automatically zeroed\n");
				break;

			case STACK_UNDERFLOW:
				print_to_log("WARNING: STACK_UNDERFLOW, pop function will return you zeroed stack\n");
				break;
            case INCCORECT_INPUT_TO_CTOR:
                print_to_log("WARNING: INCCORECT_INPUT_TO_CTOR, you passed the wrong capacity to the constructor\n");
                break;
		}

	}


    switch(error)
    {
        case INCCORECT_STACK:
			    print_to_log("ERROR: INCORRECT_STACK, your stack incorrect, the program was stoped\n");
				break;
        case NULL_PTR_STACK:
                print_to_log("ERROR: NULL_PTR_STACK, you passed to the function that mentioned before the nullptr-object\n");
                break;
        case MEMORY_ALLOCATED_ERROR:
                print_to_log("ERROR: MEMORY_ALLOCATED_ERROR, something went wrong in memory allocation or reallocation\n");
                break;

    }
}

#define switch_errors(ERROR_OR_WARNING_CODE) switch_errors_(ERROR_OR_WARNING_CODE, __PRETTY_FUNCTION__, __LINE__);

void* recalloc(void* block, size_t elem_count, size_t elem_size)
{
	return realloc(block, elem_count * elem_size);
}

#define malloc()
#define realloc()



#define stack_template(elem_type)                                                                              \
struct stack_##elem_type {																					   \
	elem_type* data;																				           \
	ssize_t size;																					           \
	ssize_t capacity;																				           \
                                                                                                               \
};																						                       \
																											   \
                                                                                                               \
void general_info_stack_##elem_type (stack_##elem_type* stack_t, void (*printer)(elem_type* to_print) = nullptr)\
{                                                                                                              \
    static void (*stat_printer)(elem_type* to_print) = nullptr;                                                \
    if(stack_t == nullptr)  {                                                                                  \
        switch_errors(NULL_PTR_STACK);                                          \
        assert(!"NULL_PTR_STACK");                                                                             \
    }                                                                                                          \
    if(printer != nullptr)                                                                                     \
        stat_printer = printer;                                                                                \
																											   \
    print_to_log("Stack info: stack address = %p, stack type - %s, stack size = %d, stack capacity = %d, stack"\
                 "data = %p\n", stack_t, #elem_type, stack_t->size, stack_t->capacity, stack_t->data);         \
                                                                                                               \
		for(int elem_of_data = 0; elem_of_data < stack_t->size; ++elem_of_data)                                \
		{                                                                                                      \
           print_to_log("&[%d] == %p\n", elem_of_data, &stack_t->data[elem_of_data]);                          \
           if(stat_printer != nullptr)                                                                         \
           {                                                                                                   \
                print_to_log("*[%d] == ", elem_of_data);                                                       \
                stat_printer(&stack_t->data[elem_of_data]);                                                    \
                print_to_log("\n");                                                                            \
           }                                                                                                   \
                                                                                                               \
		}                                                                                                      \
     																										   \
                                                                                                               \
}																											   \
int invalid_stack_##elem_type(stack_##elem_type* stack_t)					        \
{                                                                                                           \
	if(stack_t == nullptr) {                                                                                \
		switch_errors(NULL_PTR_STACK);                             \
		assert(!"NULL_PTR_STACK");                                                                          \
	}                                                                                                       \
	if(stack_t->size > stack_t->capacity || stack_t->data == nullptr                                        \
       || stack_t->size < 0 || stack_t->capacity < MIN_SIZE)                                                \
    {                                                                                                       \
		switch_errors(INCCORECT_STACK);                                      \
		assert(!"STACK INCCORECT, THE PROGRAMM WAS STOPPED ");                                              \
    }                                                                                                       \
                                                                                                            \
	return 0;                                                                                               \
}                                                                                                           \
                                                                                                            \
void ctor_stack_##elem_type(stack_##elem_type* to_ctor, size_t capacity = 8,								\
                            void (*printer)(elem_type* to_print) = nullptr)                 \
{																									        \
     if(to_ctor == nullptr)                                                                                 \
	 {                                                                                                      \
        switch_errors(NULL_PTR_STACK);                             \
		assert(!"NULL_PTR_STACK");                                                                          \
	 }                                                                                                      \
                                                                                                            \
	if(capacity < 0)                                                                                        \
		switch_errors(INCCORECT_INPUT_TO_CTOR);                    \
	if(DEBUG_LEVEL == 2)                                                                                    \
	{                                                                                                       \
		stack_##elem_type null_initialized = {0};                                                           \
		if(memcmp(to_ctor, &null_initialized, sizeof(elem_type)) != 0)                                      \
			switch_errors(NOT_INITIALIZED_STACK);					\
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
	if(to_ctor->data == nullptr) {                                                                          \
		switch_errors(MEMORY_ALLOCATED_ERROR);                     \
		assert(to_ctor->data != nullptr);                                                                   \
	}                                                                                                       \
	if(!invalid_stack_##elem_type(to_ctor) && DEBUG_LEVEL >= 1)                                   \
        general_info_stack_##elem_type(to_ctor, printer);                                         \
                                                                                                            \
}																									        \
                    																				        \
                                                                                                            \
																								        \
                                                                                                            \
void stack_push_##elem_type(stack_##elem_type* stack_t, elem_type* value)		    \
{                                                                                                           \
    if(!invalid_stack_##elem_type(stack_t))																									        \
	if (stack_t->size < stack_t->capacity)\
		stack_t->data[stack_t->size++] = *value;\
	else																							        \
	{																								        \
		stack_t->data = (elem_type*)recalloc(stack_t->data, 2*stack_t->capacity, sizeof(elem_type));  \
		stack_t->capacity *= 2;\
		stack_t->data[stack_t->size++] = *value;\
		assert(stack_t->data != nullptr);															        \
	}                                                                                                       \
	general_info_stack_##elem_type(stack_t);                                                                \
}                                                                                                           \
                                                                                                            \
int is_empty_stack_##elem_type(stack_##elem_type* stack_t)											        \
{																									        \
	assert(stack_t != nullptr);																		        \
	return (stack_t->size == 0) ? 1 : 0;																        \
}																									        \
                                                                                                            \
elem_type stack_pop_##elem_type(stack_##elem_type* stack_t)					                                \
{																									        \
	if(!is_empty_stack_##elem_type(stack_t))														        \
	{																								        \
		if((stack_t->size <= stack_t->capacity / 2 - OFFSET_ON_DELETING) && stack_t->capacity/ 2 > MIN_SIZE)\
        {                                                                                                   \
			stack_t->data = (elem_type*)recalloc(stack_t->data, stack_t->capacity / 2, sizeof(elem_type));  \
			stack_t->capacity /= 2;                                                                         \
        }                                                                                                   \
		if(DEBUG_LEVEL == 2)   {                                                                            \
			elem_type to_return =  stack_t->data[stack_t->size - 1];                                        \
			elem_type null_initialized = {0};                                                               \
			stack_t->data[stack_t->size--] = null_initialized;                                              \
			general_info_stack_##elem_type(stack_t);                                                         \
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
void dtor_stack_##elem_type(stack_##elem_type* stack_t)							                            \
{																											\
    stack_t->size = stack_t->capacity = 0;                                                                  \
    free(stack_t->data);                                                                                    \
    stack_t->data = nullptr;																				\
}


stack_template(int)
void printer(int* to_pr)
{
    print_to_log("%d", *to_pr);
}


int main()
{
    FILE* log = fopen("log_file.txt", "w");
    int arr[100] = {};
    forget_log_file();


    for(int i = 0; i < 20; ++i)
        arr[i] = i;
    stack_int integer_stack;

    ctor_stack_int(&integer_stack, 8, printer);
    for(int i = 0; i < 20; ++i)
        stack_push_int(&integer_stack, &i);
    for(int i = 0; i < 20; ++i)
        printf("%d\n", stack_pop_int(&integer_stack));
}
