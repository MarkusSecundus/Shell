#ifndef _MYSH_UTILS_GUARD___fhndsjfhdskfdsfdsfsdfds
#define _MYSH_UTILS_GUARD___fhndsjfhdskfdsfdsfsdfds

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define xll_memory_allocator alloc_memory
#include "datastructs/ultimate_list_on_heap.h"

#define STDERR_DESCRIPTOR 2

#define PATH_SEPARATOR '/'

#define DEFAULT_PWD_IF_NONE ((char[]){PATH_SEPARATOR, '\0'})

#define SYNTAX_ERROR_RET_VAL 254

#define UNKNOWN_COMMAND_RET_VAL 127

#define RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL 128


//<foreign functions needed in the programme that for some reason are'nt mentioned in standard headers>
int yylex(void);
int yyerror(char *m);

#define errprintf(...) dprintf(STDERR_DESCRIPTOR, __VA_ARGS__)



//wrapper arround malloc(1) that prints out error message and exits if not successfull
void *alloc_memory(size_t size);
//wrapper arround realloc(2) that prints out error message and exits if not successfull
void *realloc_memory(void *to_realloc, size_t new_size);



//main function of the shell
int shell_main(int argc, char **argv);





typedef struct{
    char *str;
    size_t len;
} string_t;

//converts char[] literal to string_t literal
#define as_string(lit) ((string_t){.str = (lit), .len = (sizeof(lit) - 1)})



typedef struct{
    string_t str;
    size_t buffer_len;
} string_buffer_t;

string_t str_copy(const char *to_copy);

#define string_buffer_empty ( (string_buffer_t){.buffer_len = 0, .str = (string_t){.len = 0, .str = NULL}} )

string_buffer_t make_string_buffer(int len);

void append_char_to_buffer(string_buffer_t *buf, char c);

XLL_TYPEDEF(str_list_t, str_list_node_t,
    string_t str;
);



typedef struct{
    string_t command_name;
    str_list_t args_list;
} command_t;

//deeply deallocates provided command with all its subfields (eg. its args list)
void destroy_command(command_t com);

//creates new command with empty args list and specified name
command_t make_command(string_t command_name);

//appends new argument to the command, returns the new value for self
command_t append_arg_to_command(command_t self, string_t arg);



XLL_TYPEDEF(command_list_t, command_list_node_t,
    command_t command;
);

//deeply deallocates all nodes of the provided list with all their contents
command_list_t destroy_command_list(command_list_t list);

//allocates new node holding the provided command and returns handle to the new list
command_list_t make_command_list(command_t first);

//allocates node for the provided command and appends it to the specified list
command_list_t append_to_command_list(command_list_t list, command_t to_append);






//sets provided string as input for flex and parses its contents by bison - returns the value returned by yyparse
int scan_string(const char *to_scan);

//sets provided file as flex input and parses its contents by bison - returns the value returned by yyparse
int scan_file(FILE *file);


typedef union{
    string_t string;
    command_t command;
    command_list_t command_list;
} token_t;

#define YYSTYPE token_t //set token_t as token type for flex&bison




void set_parser_ret_val(command_list_t value); //sets the global variable for parser return value
int set_shell_ret_val(int value);              //sets the global variable for shell return value

int is_interactive_mode(void); //whether the shell runs in interactive mode
int is_filereading_mode(void); //whether the shell is running from a file

int *line_count(void); //number of the line that's currently being processed by flex

char **current_token(void); //pointer to a global variable that holds the stringic representation of the token that is currently being processed by flex (for better debug messages)








#endif