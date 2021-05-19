#ifndef _MYSH_UTILS_GUARD___fhndsjfhdskfdsfdsfsdfds
#define _MYSH_UTILS_GUARD___fhndsjfhdskfdsfdsfsdfds

#include "mysh_header.h"


typedef int file_descriptor_t;


//<foreign functions needed in the programme that for some reason are'nt mentioned in standard headers>
int yylex(void);
int yyerror(char *m);
int fileno(FILE *);

#define errprintf(...) dprintf(STDERR_DESCRIPTOR, __VA_ARGS__)




//main function of the shell
int shell_main(int argc, char **argv);



#include "strings.h"

#include "syntax_model.h"



//sets provided string as input for flex and parses its contents by bison - returns the value returned by yyparse
int scan_string(const char *to_scan);

//sets provided file as flex input and parses its contents by bison - returns the value returned by yyparse
int scan_file(FILE *file);

typedef union{
    string_t string;
    simple_command_segment_t simple_command_segment;
    simple_command_t simple_command;
    command_t piped_command;
    command_list_t command_list;
    redirect_type_t redirect_type;
} token_t;

#define YYSTYPE token_t //set token_t as token type for flex&bison




void set_parser_ret_val(command_list_t value); //sets the global variable for parser return value
int set_shell_ret_val(int value);              //sets the global variable for shell return value

int is_interactive_mode(void); //whether the shell runs in interactive mode
int is_filereading_mode(void); //whether the shell is running from a file

extern int line_count; //number of the line that's currently being processed by flex

extern char *current_token; //pointer to a global variable that holds the stringic representation of the token that is currently being processed by flex (for better debug messages)








#endif