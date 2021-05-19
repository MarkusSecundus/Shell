#include "syntax_model.h"




//<structures for representing commands in memory>


void destroy_simple_command(simple_command_t com)
{
    free_memory(com.command_name.str);

    {
        str_list_t list = com.args_list;
        while (list.length > 0)
        {
            free_memory(list.current->str.str);
            list = xll_destroy(list);
        }
    }
    {
        redirect_list_t list = com.redirections;
        while (list.length > 0)
        {
            free_memory(list.current->file_name.str);
            list = xll_destroy(list);
        }
    }
}



simple_command_t make_empty_simple_command(){
    return (simple_command_t){.command_name = NULL_STRING, .args_list = xll_empty(str_list_t), .redirections = xll_empty(redirect_list_t)};
}

static simple_command_t append_redirection_to_simple_command(simple_command_t self, string_t file_name, redirect_type_t redirect_type){
    self.redirections = xll_add(self.redirections, {.file_name = file_name, .redirect_type = redirect_type});
    return self;
}

static simple_command_t append_identifier_to_simple_command(simple_command_t self, string_t to_append){
    if(self.command_name.str == NULL)
    {
        self.command_name = to_append;
    }else
    {
        self.args_list = xll_add(self.args_list, {.str = to_append});
    }
    return self;
}

simple_command_t append_segment_to_simple_command(simple_command_t self, simple_command_segment_t to_append){
    if(to_append.redirect == REDIRECT_NOREDIRECT){
        return append_identifier_to_simple_command(self, to_append.identifier);
    }else{
        return append_redirection_to_simple_command(self, to_append.identifier, to_append.redirect);
    }
}

command_t make_piped_command(simple_command_t first_segment){
    return (command_t){.segments = xll_create(simple_command_list_node_t, {.value = first_segment})};
}

command_t append_to_piped_command(command_t self, simple_command_t to_append){
    self.segments = xll_add(self.segments, {.value = to_append});
    return self;
}

void destroy_piped_command(command_t self){
    simple_command_list_t list = self.segments;
    while(list.length > 0)
    {
        destroy_simple_command(list.current->value);
        list = xll_destroy(list);
    }
}



command_list_t make_command_list(command_t first)
{
    return xll_create(command_list_node_t, {.command = first});
}

command_list_t append_to_command_list(command_list_t list, command_t to_append)
{
    return xll_add(list, {.command = to_append});
}

command_list_t destroy_command_list(command_list_t list)
{
    while (list.length > 0)
    {
        destroy_piped_command(list.current->command);
        list = xll_destroy(list);
    }
    return xll_empty(command_list_t);
}

//</>