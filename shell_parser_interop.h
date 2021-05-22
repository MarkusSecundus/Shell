#ifndef _SHELL_PARSER_INTERFACE__guard___fhdskohfoksjfklsdjlkfjdslkfjsdlkjflds
#define _SHELL_PARSER_INTERFACE__guard___fhdskohfoksjfklsdjlkfjdslkfjsdlkjflds

#include "mysh_header.h"
#include "syntax_model.h"



//<foreign functions needed by flex&bison that for some reason are'nt mentioned in standard headers>
int yylex(void);
int yyerror(char *m);
int fileno(FILE *);
//</>





/**
 * Value to be set to shell_ret_val by parser on encountering a syntax error
*/
#define SYNTAX_ERROR_RET_VAL 254

/**
 * Result of the parser
*/
extern command_list_t parser_ret_val;

/**
 * Value to be used as return value of the process
*/
extern int shell_ret_val;

/**
 * Number of the line currently being processed by syntactical analysis
 * - to be used in error messages.
*/
extern int line_count;

/**
 * String to represent the last processed token in error messages  
*/
extern char *current_token;

/**
 * True or False whether the shell is currently running in the interactive mode.
 * For modifying the flag declare explicitly `extern int is_interactive_mode_flag;`
*/
int is_interactive_mode(void);
/**
 * True or False whether the shell is currently running in the filereading mode.
 * For modifying the flag declare explicitly `extern int is_filereading_mode_flag;`
*/
int is_filereading_mode(void);

//sets provided string as input for flex and parses its contents by bison - returns the value returned by yyparse
int scan_string(const char *to_scan);

//sets provided file as flex input and parses its contents by bison - returns the value returned by yyparse
int scan_file(FILE *file);

#endif