#ifndef _SYNTAX_MODEL__GUARD___hfsjiuhfojdshjlfjkdshkljfhkjsfjioshfiusdnkjjhoiuahfjkdshfkjdshbfgkjslju
#define _SYNTAX_MODEL__GUARD___hfsjiuhfojdshjlfjkdshkljfhkjsfjioshfiusdnkjjhoiuahfjkdshfkjdshbfgkjslju



#include "mysh_header.h"
#include "strings.h"


typedef enum{
    REDIRECT_NOREDIRECT = -1, REDIRECT_STDIN = 0, REDIRECT_STDOUT, REDIRECT_STDOUT_APPEND
}  redirect_type_t;


XLL_TYPEDEF(redirect_list_t, redirect_list_node_t,
    string_t file_name;
    redirect_type_t redirect_type;
);



typedef struct{
    string_t command_name;
    str_list_t args_list;
    redirect_list_t redirections;
} simple_command_t;

//deeply deallocates provided command with all its subfields (eg. its args list)
void destroy_simple_command(simple_command_t com);

/*
//creates new command with empty args list and specified name
simple_command_t make_simple_command(string_t command_name);

//appends new argument to the command, returns the new value for self
simple_command_t append_arg_to_simple_command(simple_command_t self, string_t arg);*/

typedef struct{
    string_t identifier;
    redirect_type_t redirect;
} simple_command_segment_t;

simple_command_t make_empty_simple_command();

simple_command_t append_segment_to_simple_command(simple_command_t self, simple_command_segment_t to_append);

XLL_TYPEDEF(simple_command_list_t, simple_command_list_node_t,
    simple_command_t value;
);


typedef struct{
    simple_command_list_t segments;
} command_t;


command_t make_piped_command(simple_command_t first_segment);

command_t append_to_piped_command(command_t self, simple_command_t to_append);

void destroy_piped_command(command_t self);

XLL_TYPEDEF(command_list_t, command_list_node_t,
    command_t command;
);

//deeply deallocates all nodes of the provided list with all their contents
command_list_t destroy_command_list(command_list_t list);

//allocates new node holding the provided command and returns handle to the new list
command_list_t make_command_list(command_t first);

//allocates node for the provided command and appends it to the specified list
command_list_t append_to_command_list(command_list_t list, command_t to_append);











#endif