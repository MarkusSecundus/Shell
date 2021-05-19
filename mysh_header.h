#ifndef _MYSH_HEADER__GUARD___jfdokafjswdfjlkdslfjudfeasiopjfolajoa
#define _MYSH_HEADER__GUARD___jfdokafjswdfjlkdslfjudfeasiopjfolajoa


#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * Autor: Jakub Hroník
**/

#define XLL_MEMORY_ALLOCATOR alloc_memory
#define XLL_MEMORY_DEALLOCATOR free_memory
#include "datastructs/ultimate_list_on_heap.h"


typedef int file_descriptor_t;


#define STDERR_DESCRIPTOR 2

#define PATH_SEPARATOR '/'

#define DEFAULT_PWD_IF_NONE ((char[]){PATH_SEPARATOR, '\0'})

#define SYNTAX_ERROR_RET_VAL 254

#define UNKNOWN_COMMAND_RET_VAL 127

#define RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL 128


//wrapper arround malloc(1) that prints out error message and exits if not successfull
void *alloc_memory(size_t size);
//wrapper arround realloc(2) that prints out error message and exits if not successfull
void *realloc_memory(void *to_realloc, size_t new_size);
//wrapper arround free(1)
void free_memory(void *to_free);


#endif