#include "mysh_header.h"

#include <err.h>
#include <errno.h>


//<error-handled memory allocation>
void *alloc_memory(size_t size)
{
    void *ret = malloc(size);
    if (!ret)
    {
        errx(ENOMEM, "Unable to allocate %ld bytes - exiting!\n", size);
    }
    return ret;
}

void *realloc_memory(void *to_realloc, size_t new_size)
{
    void *ret = realloc(to_realloc, new_size);
    if (!ret)
    {
        errx(ENOMEM, "Unable to realloc %p to %ld bytes - exiting!\n", to_realloc, new_size);
    }
    return ret;
}

void free_memory(void *to_free){
    return free(to_free);
}
//</>