#include "shell_parser_interop.h"



command_list_t parser_ret_val;
int shell_ret_val;

void set_parser_ret_val(command_list_t value)
{
    parser_ret_val = value;
}

int set_shell_ret_val(int value)
{
    return shell_ret_val = value;
}

int line_count = 0;
char *current_token = "(nil)";

int is_interactive_mode_flag = 0;
int is_filereading_mode_flag = 0;

int is_interactive_mode(void)
{
    return is_interactive_mode_flag;
}
int is_filereading_mode(void)
{
    return is_filereading_mode_flag;
}
