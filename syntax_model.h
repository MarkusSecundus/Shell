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


/**
 * Struct representing a simple command that has a name, arguments and a list of redirections.
*/
typedef struct{
    string_t command_name;
    str_list_t args_list;
    redirect_list_t redirections;
} simple_command_t;

/**
 * Deeply deallocates all fields of the provided simple_command instance
 * */
void destroy_simple_command(simple_command_t com);

/**
 * Creates an empty simple_command with no name, no arguments and no redirections -
 *  - those are to be appended later.
*/
simple_command_t make_empty_simple_command();


/**
 * A syntactic segment of a simple command. Can be either a redirection or an argument/command name
*/
typedef struct{
    string_t identifier;
    redirect_type_t redirect;
} simple_command_segment_t;


/**
 * Appends provided segment to the simple command.
*/
simple_command_t append_segment_to_simple_command(simple_command_t self, simple_command_segment_t to_append);

/**
 * Linked list of simple_commands
*/
XLL_TYPEDEF(simple_command_list_t, simple_command_list_node_t,
    simple_command_t value;
);

/**
 * Command pipeline. Consists of multiple simple_commands to be run in parallel and connected together with pipes.
*/
typedef struct{
    simple_command_list_t segments;
} command_t;

/**
 * Deeply deallocates all fields of the provided command instance
 * */
void destroy_piped_command(command_t self);

/**
 * Creates a command pipeline consisting of the one provided simple_command - more commands can be appended later
*/
command_t make_piped_command(simple_command_t first_segment);

/**
 * Appends provided simple_command at the end of the pipeline
*/
command_t append_to_piped_command(command_t self, simple_command_t to_append);


/**
 * Linked list of command pipelines.
*/
XLL_TYPEDEF(command_list_t, command_list_node_t,
    command_t command;
);

/**
 * Deeply deallocates all nodes of the provided list with all their contents
*/
command_list_t destroy_command_list(command_list_t list);

/**
 * Allocates new node holding the provided command and returns handle to the new list
*/
command_list_t make_command_list(command_t first);

/**
 * Allocates node for the provided command and appends it to the specified list
*/
command_list_t append_to_command_list(command_list_t list, command_t to_append);











#endif