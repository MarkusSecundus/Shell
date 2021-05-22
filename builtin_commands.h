#ifndef _BUILTIN_COMMANDS__guard___fjaokfhsohjfasokhjfdslkhjfloksajflksajdl
#define _BUILTIN_COMMANDS__guard___fjaokfhsohjfasokhjfdslkhjfloksajflksajdl

#include "mysh_header.h"
#include<unistd.h>
#include<err.h>


#include "syntax_model.h"

/**
 * Interface that all builtin commands must implement.
 * 
 * @param info instance carrying aditional data for the builtin command
 * @param in descriptor to be used for text input of the command
 * @param in descriptor to be used for text output of the command
*/
typedef int (*builtin_command_t)(simple_command_t info, file_descriptor_t in, file_descriptor_t out);


/**
 * Returns a builtin command function pointer if the provided simple_command corresponds to a builtin command.
 * Returns NULL otherwise
*/
builtin_command_t recognize_builtin_command(simple_command_t com);


/**
 * Exits current process with shell_ret_val as return value
*/
int exit_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out);

/**
 * Does nothing and returns 0
*/
int nop_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out);

/**
 * Changes current directory
 * See `man cd` for more information
*/
int cd_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out);

#endif