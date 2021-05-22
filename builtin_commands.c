#include "builtin_commands.h"
#include "envvars.h"
#include "shell_parser_interop.h"

#include <stdlib.h>
#include <errno.h>



builtin_command_t recognize_builtin_command(simple_command_t com)
{
    string_t command_name = com.command_name;
    if (command_name.str == NULL || !strcmp(":", command_name.str))
    {
        return nop_builtin;
    }
    else if (!strcmp("exit", command_name.str))
    {
        return exit_builtin;
    }
    else if (!strcmp("cd", command_name.str))
    {
        return cd_builtin;
    }
    return NULL;
}



int exit_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out)
{
    (void)info;
    (void)in;
    (void)out;
    exit(shell_ret_val);
    return shell_ret_val;
}

int nop_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out)
{
    (void)info;
    (void)in;
    (void)out;
    return 0;
}

int cd_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out)
{
    (void)in;
    str_list_t args = info.args_list;

    string_t path;
    int reverting_to_oldpwd = 0;
    switch (args.length)
    {
    case 0:
        path = get_home_dir();
        break;
    case 1:
        path = args.current->str;
        if (str_equals(path, as_string("-")))
        {
            path = raw_to_string(get_oldpwd());
            if (path.str == NULL)
                warnx("cd: OLDPWD not set");
            reverting_to_oldpwd = 1;
        }
        path = str_cpy(path);
        break;
    default:
        warnx("cd: too many arguments");
        return EINVAL;
    }

    string_t new_oldpwd = str_copy(get_pwd());

    if (path.str && chdir(path.str) != 0)
    {
        warn("cd: `%s`", path.str);
        free_memory(path.str);
        free_memory(new_oldpwd.str);
        return ENOENT;
    }
    if (reverting_to_oldpwd)
        dprintf(out, "%s\n", path.str);

    char *real_curr_pwd = get_current_dir();

    set_oldpwd(new_oldpwd.str);
    set_pwd(real_curr_pwd);

    free_memory(real_curr_pwd);
    free_memory(path.str);
    free_memory(new_oldpwd.str);
    return 0;
}