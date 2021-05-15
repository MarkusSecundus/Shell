
%{
#include "mysh_utils.h"
#include<err.h>
%}


%token<string> IDENTIFIER
%token SEMIC NEWL 
%token PIPE
%token REDIRIN REDIROUT REDIROUTA

/*
%type<command> command
%type<command_list> command_list_wo_semic command_list multiline_command_list multiline_command_list__opt unit line
*/
%%

unit: multiline_command_list__opt /*{ $$ = $1; set_parser_ret_val($$); }*/
    ;

multiline_command_list__opt: %empty /*{ $$ = xll_empty(command_list_t); }*/
    | multiline_command_list /*{ $$ = $1; }*/
    | multiline_command_list command_list /*{ $$ = xll_concat($1, $2); }*/
    | command_list /*{ $$ = $1; }*/
    ;

multiline_command_list: line /*{ $$ = $1; }*/
    |  multiline_command_list line /*{ $$ = xll_concat($1, $2); }*/
    ;

line: NEWL /*{ $$ = xll_empty(command_list_t); }*/
    | command_list NEWL /*{ $$ = $1; }*/
    ;


command_list: command_list_wo_semic /*{ $$ = $1; }*/
    | command_list_wo_semic SEMIC /*{ $$ = $1; }*/
    ;

command_list_wo_semic: command /*{ $$ = make_command_list($1); }*/
    | command_list_wo_semic SEMIC command /*{ $$ = append_to_command_list($1, $3); }*/
    ;

command: simple_command
    | command PIPE simple_command
    ;

simple_command:
    command_segment
    | simple_command command_segment
    ;

command_segment:
    IDENTIFIER
    | redir IDENTIFIER
    ;


redir: REDIRIN | REDIROUT | REDIROUTA ;



%%


int main(int argc, char **argv){
    return shell_main(argc, argv);
}



int yyerror(char *m){
    set_shell_ret_val(SYNTAX_ERROR_RET_VAL);
    
    if(is_filereading_mode())
        warnx("'%s'... on line %d near token '%s'", m, line_count, current_token);
    else
        warnx("'%s'... near token '%s'", m, current_token);
    
    return 0;
}
