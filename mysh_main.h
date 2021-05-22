#ifndef _MYSH_UTILS_GUARD___fhndsjfhdskfdsfdsfsdfds
#define _MYSH_UTILS_GUARD___fhndsjfhdskfdsfdsfsdfds

#include "mysh_header.h"

#include "strings.h"
#include "syntax_model.h"
#include "shell_parser_interop.h"





/**
 * Printf for writing to error output
*/
#define errprintf(...) dprintf(STDERR_DESCRIPTOR, __VA_ARGS__)

#define STDERR_DESCRIPTOR 2

#define PATH_SEPARATOR '/'

#define DEFAULT_PWD_IF_NONE ((char[]){PATH_SEPARATOR, '\0'})

/**
 * Return value to be set when unable to call a command because it doesn't exist.
*/
#define UNKNOWN_COMMAND_RET_VAL 127

/**
 * Value to be added to return value if the command was killed by a signal.
*/
#define RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL 128

/**
 * The main function of the shell
 * */
int shell_main(int argc, char **argv);


/**
 * Type of flex&bison tokens
*/
typedef union
{
    string_t string;
    simple_command_segment_t simple_command_segment;
    simple_command_t simple_command;
    command_t piped_command;
    command_list_t command_list;
    redirect_type_t redirect_type;
} token_t;

#define YYSTYPE token_t //set token_t as token type for flex&bison

#endif