#ifndef _MYSH_HEADER__GUARD___jfdokafjswdfjlkdslfjudfeasiopjfolajoa
#define _MYSH_HEADER__GUARD___jfdokafjswdfjlkdslfjudfeasiopjfolajoa

/**
 * Header file containing all the most basic definitions that is supposed to be included in every compilation unit of the shell.
 * 
 * 
 * Author: Jakub Hroník
*/

/**
**/
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>






#define XLL_MEMORY_ALLOCATOR alloc_memory
#define XLL_MEMORY_DEALLOCATOR free_memory
#include "datastructs/ultimate_list_on_heap.h"

typedef int file_descriptor_t;

/**
 * Wrapper arround malloc(1) that prints out error message and exits if not successfull
 * */
void *alloc_memory(size_t size);
/**
 * Wrapper arround realloc(2) that prints out error message and exits if not successfull
 * */
void *realloc_memory(void *to_realloc, size_t new_size);
/**
 * Wrapper arround free(1)
 * */
void free_memory(void *to_free);

#endif