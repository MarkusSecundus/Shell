

#include "mysh_utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <err.h>
#include <pwd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <readline/readline.h>
#include <readline/history.h>


#include "syntax_analyzer.tab.h"

#include "envvars.h"



//<global variables>
static command_list_t parser_ret_val;
static int shell_ret_val;

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

static int is_interactive_mode_flag = 0;
static int is_filereading_mode_flag = 0;

int is_interactive_mode(void)
{
    return is_interactive_mode_flag;
}
int is_filereading_mode(void)
{
    return is_filereading_mode_flag;
}

//</>




























//<executing commands>

XLL_TYPEDEF(pid_list_t, pid_list_node_t,
    pid_t value;
);

static pid_list_t current_children_pids;

//static pid_t current_child_pid;
static command_list_t waiting_to_be_executed;

//      <handling SIGINT interrupt>
static void sigint_handler_fnc(int t)
{
    (void)t;

    putchar('\n');
    if (is_interactive_mode())
    {
        rl_replace_line("", 0);
        if (!rl_already_prompted)
            rl_redraw_prompt_last_line();
    }
}
static void nop_handler_fnc(int t)
{
    (void)t;
}

const struct sigaction handler_when_idle = {.sa_handler = sigint_handler_fnc, .sa_flags = SA_RESTART};
const struct sigaction handler_when_child_running = {.sa_handler = nop_handler_fnc, .sa_flags = SA_RESTART};

static void set_sigint_handler(const struct sigaction *handl)
{
    if (sigaction(SIGINT, handl, NULL))
    {
        err(-1, "Unable to register SIGINT signal handler!");
    }
}

//      </>






//      <builtin commands>
static int exit_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out)
{
    (void)info;
    (void)in;
    (void)out;
    exit(shell_ret_val);
    return shell_ret_val;
}

static int nop_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out)
{
    (void)info;
    (void)in;
    (void)out;
    return 0;
}


static int cd_builtin(simple_command_t info, file_descriptor_t in, file_descriptor_t out)
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
            if(path.str == NULL)
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
//      </>







int get_child_return_value(int stat_loc)
{
    if (WIFEXITED(stat_loc))
        return WEXITSTATUS(stat_loc);
    if (WIFSIGNALED(stat_loc))
    {
        int sigterm = WTERMSIG(stat_loc);
        if (sigterm == SIGINT)
            waiting_to_be_executed = destroy_command_list(waiting_to_be_executed);
        errprintf("Killed by signal %d.\n", sigterm);
        return sigterm + RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL;
    }
    if (WIFSTOPPED(stat_loc))
    {
        int sigterm = WSTOPSIG(stat_loc);
        errprintf("Stopped by signal %d.\n", sigterm);
        return sigterm + RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL;
    }
    err(ENOTSUP, "Process terminated in unsupported way.\n");
}
/*
static int await_current_child(void)
{

    if (current_child_pid <= 0)
        return shell_ret_val;

    int stat_loc;
    if (waitpid(current_child_pid, &stat_loc, 0) != -1)
    {
        return get_child_return_value(stat_loc);
    }

    current_child_pid = 0;

    return shell_ret_val;
}*/

static int await_all_current_children(void){
    int ret = 0;

    while(current_children_pids.length > 0){
        int stat_loc;
        if(waitpid(current_children_pids.current->value, &stat_loc, 0) != -1){
            ret = get_child_return_value(stat_loc);
        }
        current_children_pids = xll_destroy(current_children_pids);
    }

    return ret;
}


char **make_command_arglist(simple_command_t com)
{
    char **ret = (char **)alloc_memory((com.args_list.length + 2) * sizeof(char *));
    char **it = ret;
    *it++ = com.command_name.str;
    xll_foreach(arg, com.args_list)
        *it++ = arg->str.str;
    *it = NULL;
    return ret;
}


static struct{file_descriptor_t input, output; int success;} load_file_descriptors(redirect_list_t redirects, void(*report)(int errnum, const char* format, ...)){
    __typeof__(load_file_descriptors(redirects, report)) ret;
    ret.input = -1;
    ret.output = -1;
    ret.success = 1;

    file_descriptor_t *filedes_writers[] = {&ret.input, &ret.output, &ret.output};
    int flags[] = {O_RDONLY, O_WRONLY | O_CREAT, O_WRONLY | O_CREAT | O_APPEND };

    xll_foreach(it, redirects)
    {
        if(it->redirect_type >= REDIRECT_STDIN && it->redirect_type <= REDIRECT_STDOUT_APPEND)
        {
            file_descriptor_t *filedes = filedes_writers[it->redirect_type];
            int flag = flags[it->redirect_type];
            int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

            if(*filedes >= 0) 
                close(*filedes);
            if( (*filedes = open(it->file_name.str, flag, mode)) < 0)
            {
                report(errno, "Cannot open file '%s'", it->file_name.str);
                ret.success = 0;
                break;
            }
        }
    }

    return ret;
}

static void set_file_redirections(redirect_list_t redirects){
    AUTO(redirs =, load_file_descriptors(redirects, err));

    if(redirs.input >= 0){
        dup2(redirs.input, STDIN_FILENO);
        close(redirs.input);
    }
    if(redirs.output >= 0){
        dup2(redirs.output, STDOUT_FILENO);
        close(redirs.output);
    }
}

static void turn_self_into_a_command(simple_command_t com){
        set_file_redirections(com.redirections);
        char **arglist = make_command_arglist(com);

        execvp(com.command_name.str, arglist);
        
        err(UNKNOWN_COMMAND_RET_VAL, "Unable to exec command '%s'", com.command_name.str);
}

static void warn_with_ignored_first_arg(int errnum, const char* format, ...){
    (void)errnum;
    va_list args;
    va_start(args, format);
    vwarn(format, args);
    va_end(args);
}
static int exec_provided_builtin_command(simple_command_t com, int (*impl)(simple_command_t info, file_descriptor_t in, file_descriptor_t out)){

    int ret;
    AUTO(redirs =, load_file_descriptors(com.redirections, warn_with_ignored_first_arg));
    

    if(redirs.success){
        AUTO(redr =, redirs);
        if(redr.input < 0) redr.input = STDIN_FILENO;
        if(redr.output < 0) redr.output = STDOUT_FILENO;
        ret = impl(com, redr.input, redr.output);
    }
    else{
        ret = errno;
    }

    if(redirs.input >= 0) close(redirs.input);
    if(redirs.output >= 0) close(redirs.output);
 
    return ret;
}

static int exec_general_command(simple_command_t com)
{
    set_sigint_handler(&handler_when_child_running);
    int id = fork();
    if (id == 0)
    {
        turn_self_into_a_command(com);
    }
    current_children_pids = xll_add(current_children_pids, {.value = id});
    int ret = await_all_current_children();
    set_sigint_handler(&handler_when_idle);
    return ret;
}


static int(*recognize_builtin_command(string_t command_name))(simple_command_t, file_descriptor_t, file_descriptor_t){
    if(command_name.str == NULL || !strcmp(":", command_name.str))
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

static int exec_simple_command(simple_command_t com)
{
    AUTO(builtin =, recognize_builtin_command(com.command_name));
    return builtin
        ? exec_provided_builtin_command(com, builtin)
        : exec_general_command(com);
}

static int as_simple_command(simple_command_t com)
{
    AUTO(builtin =, recognize_builtin_command(com.command_name));
    return builtin
        ? exec_provided_builtin_command(com, builtin)
        : (turn_self_into_a_command(com), 0);
}


static int exec_command_pipeline(command_t com){
    if(com.segments.length == 1)
        return exec_simple_command(com.segments.current->value);

    int pd[2], last_pipe = -1;

    set_sigint_handler(&handler_when_child_running);

    xll_foreach(it, com.segments){
        if( it != com.segments.last && pipe(pd) < 0 ){
            warn("Failed to create pipe!");
            break;
        }

        int id = fork();
        if(id == 0){
            if(last_pipe >= 0){
                dup2(last_pipe, STDIN_FILENO);
                close(last_pipe);
            }
            if(it != com.segments.last){
                dup2(pd[1], STDOUT_FILENO);
                close(pd[1]);
                close(pd[0]);
            }
            exit(as_simple_command(it->value));
        }else{
            if(it != com.segments.last) close(pd[1]);
            if(last_pipe>= 0) close(last_pipe);
            last_pipe = pd[0];
            current_children_pids = xll_add(current_children_pids, {.value = id});
        }
    }


    int ret = await_all_current_children();
    set_sigint_handler(&handler_when_idle);
    return ret;
}




static int exec_command_list(command_list_t list)
{
    waiting_to_be_executed = list;

    while (waiting_to_be_executed.length > 0)
    {
        command_t first = waiting_to_be_executed.current->command;
        waiting_to_be_executed = xll_destroy(waiting_to_be_executed);
        set_shell_ret_val(exec_command_pipeline(first)); 
        destroy_piped_command(first);
    }
    return shell_ret_val;
}








static char *get_prompt(void)
{
    static const string_t BASIC_PROMPT = as_string("$ ");

    static string_t value = NULL_STRING;
    static char *saved_pwd = NULL;

    if (value.str == NULL || saved_pwd != get_pwd())
    {
        if (value.str != NULL)
            free_memory(value.str);
        saved_pwd = get_pwd();
        value = str_concat(raw_to_string(saved_pwd), BASIC_PROMPT);
    }

    return value.str;
}








static int interactive_mode(void)
{
    is_interactive_mode_flag = 1;
    char *line = NULL, *last_line = NULL;
    while ((line = readline(get_prompt())))
    {
        if (strcmp(line, "") && (!last_line || strcmp(line, last_line)))
            add_history(line);

        if (scan_string(line) == 0)
            exec_command_list(parser_ret_val);

        free_memory(last_line);
        last_line = line;
    }
    free_memory(last_line);
    putchar('\n');
    is_interactive_mode_flag = 0;
    return 0;
}

static int string_mode(char *str)
{
    if (scan_string(str) == 0)
        exec_command_list(parser_ret_val);
    return shell_ret_val;
}

static int file_mode(char *path)
{
    is_filereading_mode_flag = 1;

    FILE *f = fopen(path, "r");
    if (f == NULL)
    {
        warn("Can't open '%s'", path);
        return set_shell_ret_val(ENOENT);
    }

    line_count = 1;
    if (scan_file(f) == 0)
        exec_command_list(parser_ret_val);

    fclose(f);
    is_filereading_mode_flag = 0;
    return shell_ret_val;
}

static int noninteractive_mode(int argc, char **argv)
{

    if (**++argv == '-' && strchr(*argv, 'c') != NULL)
    {
        if (argc <= 2)
        {
            warnx("-c requires an argument");
            return set_shell_ret_val(EINVAL);
        }
        return string_mode(argv[1]);
    }
    return file_mode(*argv);
}

static int init_envvars(void)
{
    char *wd = get_current_dir();
    if (wd == NULL)
        wd = str_copy(DEFAULT_PWD_IF_NONE).str;
    setenv("PWD", wd, 0);
    setenv("OLDPWD", DEFAULT_PWD_IF_NONE, 0);
    free_memory(wd);
    chdir(get_pwd());
    return set_shell_ret_val(0);
}


//</>








static int init(void)
{
    set_sigint_handler(&handler_when_idle);
    return init_envvars();
}

int shell_main(int argc, char **argv)
{
    if (init())
        return shell_ret_val;

    if (argc <= 1)
        interactive_mode();
    else
        noninteractive_mode(argc, argv);

    return shell_ret_val;
}