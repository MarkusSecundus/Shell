

#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<errno.h>
#include<err.h>
#include<pwd.h>

#include<sys/types.h>
#include<sys/wait.h>
#include <sys/types.h>

#include<readline/readline.h>
#include<readline/history.h>

#include "mysh_utils.h"

#include "syntax_analyzer.tab.h"


//<global variables>
static command_list_t parser_ret_val;
static int shell_ret_val;

void set_parser_ret_val(command_list_t value){
    parser_ret_val = value;
}

int set_shell_ret_val(int value){
    return shell_ret_val = value;
}


int* line_count(void){
    static int value = 0;
    return &value;
}
char** current_token(void){
    static char *value = "(nil)";
    return &value;
}



static int is_interactive_mode_flag = 0;
static int is_filereading_mode_flag = 0;

int is_interactive_mode(void){
    return is_interactive_mode_flag;
}
int is_filereading_mode(void){
    return is_filereading_mode_flag;
}

//</>



//<error-handled memory allocation>
void* alloc_memory(size_t size){
    void *ret = malloc(size);
    if(!ret){
        err(ENOMEM, "Unable to allocate %ld bytes - exiting!\n", size);
        _Exit(ENOMEM);
    }
    return ret;
}

void* realloc_memory(void *to_realloc, size_t new_size){
    void *ret = realloc(to_realloc, new_size);
    if(!ret){
        err(ENOMEM, "Unable to realloc %p to %ld bytes - exiting!\n", to_realloc, new_size);
        _Exit(ENOMEM);
    }
    return ret;
}
//</>





//<strings>
string_t str_copy(const char *to_copy){
    int len = strlen(to_copy);
    string_t ret = {.len = len, .str=(char*)alloc_memory(sizeof(char)*(len+1))};
    memcpy(ret.str, to_copy, len+1);
    return ret;
}

static string_t raw_to_string(char *raw){
    return (string_t){.len=strlen(raw), .str = raw};
}


static string_t str_cpy(string_t to_copy){
    int len = to_copy.len;
    string_t ret = {.len = len, .str=(char*)alloc_memory(sizeof(char)*(len+1))};
    memcpy(ret.str, to_copy.str, len+1);
    return ret;
}

static int str_equals(string_t a, string_t b){
    return a.len == b.len && !strcmp(a.str, b.str);
}

static string_t str_concat(string_t a, string_t b){
    int len = a.len + b.len;
    char *ret = (char*)alloc_memory((len+1)*sizeof(char));
    memcpy(ret, a.str, a.len);
    memcpy(ret+a.len, b.str, b.len);
    ret[len] = '\0';
    return (string_t){.len=len, .str=ret};
}
static string_t str_chr_str_concat(string_t a,char c, string_t b){
    int len = a.len +1 + b.len;
    char *ret = (char*)alloc_memory((len+1)*sizeof(char));
    memcpy(ret, a.str, a.len);
    ret[a.len] = c;
    memcpy(ret+a.len + 1, b.str, b.len);
    ret[len] = '\0';
    return (string_t){.len=len, .str=ret};
}


string_buffer_t make_string_buffer(int len){
    if(len < 16) len = 16;
    char *s = (char*)alloc_memory(len*sizeof(char));
    *s = '\0';
    return (string_buffer_t){.str=(string_t){.str=s, .len=0}, .buffer_len=len};
}

void append_char_to_buffer(string_buffer_t *buf, char c){
    if(buf->str.len >= buf->buffer_len - 2){
        buf->buffer_len += 32;
        buf->str.str = (char*)realloc_memory(buf->str.str, buf->buffer_len);
    }
    buf->str.str[buf->str.len++] = c;
    buf->str.str[buf->str.len] = '\0';
}

//</>



//<structures for representing commands in memory>

command_t make_command(string_t command_name){
    return (command_t){.command_name = command_name, .args_list = xll_empty(str_list_t)};
}

command_t append_arg_to_command(command_t self, string_t arg){
    self.args_list = xll_add(self.args_list, {.str=arg});
    return self;
}


void destroy_command(command_t com){
    free(com.command_name.str);
    str_list_t list = com.args_list;
    while(list.length>0){
        free(list.current->str.str);
        list = xll_destroy(list);
    }
}

command_list_t make_command_list(command_t first){
    return xll_create(command_list_node_t, {.command = first});
}

command_list_t append_to_command_list(command_list_t list, command_t to_append){
    return xll_add(list, {.command = to_append});
}


command_list_t destroy_command_list(command_list_t list){
    while(list.length > 0){
        destroy_command(list.current->command);
        list = xll_destroy(list);
    }
    return xll_empty(command_list_t);
}

//</>







//<manipulation of environment variables>

static string_t get_home_dir(void){
    char *ret = getenv("HOME");
    if(!ret)
        ret = getpwuid(getuid())->pw_dir;
    return str_copy(ret);
}

static char* get_pwd(void){
    return getenv("PWD");
}
static char* get_oldpwd(void){
    return getenv("OLDPWD");
}

static int set_pwd(const char* value){
    return setenv("PWD", value, 1);
}
static int set_oldpwd(const char* value){
    return setenv("OLDPWD", value, 1);
}

//</>



//<handling SIGINT interrupr>
static void sigint_handler_fnc(int t){
    (void)t;
    
    putchar('\n');
    if(is_interactive_mode()){
        rl_replace_line("", 0);
        if(!rl_already_prompted)
            rl_redraw_prompt_last_line();
    }
}
static void nop_handler_fnc(int t){
    (void)t;
}

const struct sigaction handler_when_idle = (struct sigaction){.sa_handler = sigint_handler_fnc, .sa_flags = SA_RESTART}; 
const struct sigaction handler_when_child_running = (struct sigaction){.sa_handler = nop_handler_fnc, .sa_flags = SA_RESTART}; 

static void set_sigint_handler(const struct sigaction *handl){
    if(sigaction(SIGINT, handl, NULL)){
        err(-1, "Unable to register SIGINT signal handler!");
        _Exit(-1);
    }
}

//</>








static pid_t current_child_pid;
static command_list_t waiting_to_be_executed;

    //<builtin commands>
static int exit_builtin(void){
    exit(shell_ret_val);
    return shell_ret_val;
}

static int nop_builtin(void){
    return 0;
}

static int cd_builtin(str_list_t args){
    string_t path;
    int reverting_to_oldpwd = 0;
    switch(args.length){
    case 0:
        path = get_home_dir();
        break;
    case 1:
        path = args.current->str;
        if(str_equals(path, as_string("-"))){
            path = raw_to_string(get_oldpwd());
            reverting_to_oldpwd = 1;
        }
        path = str_cpy(path);
        break;
    default:
        warnx("cd: too many arguments");
        return EINVAL;
    }

    string_t new_oldpwd = str_copy(get_pwd());

    if(chdir(path.str)!=0){
        warn("cd: `%s`",path.str);
        free(path.str);
        free(new_oldpwd.str);
        return ENOENT;
    }
    if(reverting_to_oldpwd)
        printf("%s\n", path.str);

    char *real_curr_pwd = get_current_dir_name();

    set_oldpwd(new_oldpwd.str);
    set_pwd(real_curr_pwd);

    free(real_curr_pwd);
    free(path.str);
    free(new_oldpwd.str);
    return 0;
}
    //</>
 

    int get_child_return_value(int stat_loc){
        if(WIFEXITED(stat_loc))
            return WEXITSTATUS(stat_loc);
        if(WIFSIGNALED(stat_loc)){
            int sigterm = WTERMSIG(stat_loc);
            if(sigterm == SIGINT) 
                waiting_to_be_executed = destroy_command_list(waiting_to_be_executed);
            errprintf("Killed by signal %d.\n", sigterm);
            return sigterm + RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL;
        }
        if(WIFSTOPPED(stat_loc)){
            int sigterm = WSTOPSIG(stat_loc);
            errprintf("Stopped by signal %d.\n", sigterm);
            return sigterm + RET_VAL_OFFSET_WHEN_KILLED_BY_SIGNAL;
        }
        err(ENOTSUP, "Process terminated in unsupported way.\n");
    }

static int await_current_child(void){   

    if(current_child_pid <= 0)
        return shell_ret_val;

    int stat_loc;
    if(waitpid(current_child_pid, &stat_loc, 0)!=-1){
        return get_child_return_value(stat_loc);
    }

    current_child_pid = 0;

    return shell_ret_val;
}





    char **make_command_arglist(command_t com){
        char **ret = (char**)alloc_memory((com.args_list.length+2)*sizeof(char*));
        char **it = ret;
        *it++ = com.command_name.str;
        xll_foreach(arg, com.args_list)
            *it++ = arg->str.str;
        *it = NULL;
        return ret;
    }
    
    char* make_command_path(string_t partial){
        if(partial.str[0]==PATH_SEPARATOR || strchr(partial.str, PATH_SEPARATOR) == NULL)
            return partial.str;
        return str_chr_str_concat(raw_to_string(get_pwd()), PATH_SEPARATOR, partial).str;
    }
static int exec_general_command(command_t com){

    set_sigint_handler(&handler_when_child_running);
    int id = fork();
    if(id == 0){
        char **arglist = make_command_arglist(com);
        char *command_name = make_command_path(com.command_name);
        execvp(command_name, arglist);
        err(UNKNOWN_COMMAND_RET_VAL, "Unable to exec command '%s'", command_name);
        _Exit(UNKNOWN_COMMAND_RET_VAL);
    }
    current_child_pid = id;
    int ret = await_current_child();
    set_sigint_handler(&handler_when_idle);
    return ret;
}



static int exec_command(command_t com){
    if(!strcmp("exit", com.command_name.str)){
        return exit_builtin();
    }
    else if(!strcmp(":", com.command_name.str)){
        return nop_builtin();
    }else if(!strcmp("cd", com.command_name.str)){
        return cd_builtin(com.args_list);
    }
    return exec_general_command(com);
}

static int exec_command_list(command_list_t list){
    waiting_to_be_executed = list;

    while(waiting_to_be_executed.length > 0){
        command_t first = waiting_to_be_executed.current->command;
        waiting_to_be_executed = xll_destroy(waiting_to_be_executed);
        set_shell_ret_val(exec_command(first));
        destroy_command(first);
    }
    return shell_ret_val;
}




static char *get_prompt(void){
    static const string_t BASIC_PROMPT = as_string("$ ");

    static string_t value = {.len=0, .str=NULL};
    static char* saved_pwd = NULL;
    
    if(value.str == NULL || saved_pwd != get_pwd()){
        if(value.str!=NULL)
            free(value.str);
        saved_pwd = get_pwd();
        value = str_concat(raw_to_string(saved_pwd), BASIC_PROMPT);
    }

    return value.str;
}

















static int interactive_mode(void){
    is_interactive_mode_flag = 1;
    char *line = NULL;
    while( ( line = readline(get_prompt())) ){
        if(strcmp(line, ""))
            add_history(line);

        if(scan_string(line) == 0)
            exec_command_list(parser_ret_val);

        free(line);
    }
    is_interactive_mode_flag = 0;
    return 0;
}


    static int string_mode(char *str){
        if(scan_string(str) == 0)
            exec_command_list(parser_ret_val);
        return shell_ret_val;
    }

    static int file_mode(char *path){
        is_filereading_mode_flag = 1;

        FILE *f = fopen(path, "r");
        if(f == NULL){
            warn("Can't open '%s'", path);
            return set_shell_ret_val(ENOENT);
        }

        *line_count() = 1;
        if(scan_file(f) == 0)
            exec_command_list(parser_ret_val);

        fclose(f);
        is_filereading_mode_flag = 0;
        return shell_ret_val;
    }

static int noninteractive_mode(int argc, char **argv){

    if(**++argv == '-' && strchr(*argv, 'c') != NULL){
        if(argc <= 2){
            warnx("-c requires an argument");
            return set_shell_ret_val(EINVAL);
        }
        return string_mode(argv[1]);
    }
    return file_mode(*argv);
}



static int init_envvars(void){
    char *wd = get_current_dir_name();
    if(wd==NULL) wd = str_copy(DEFAULT_PWD_IF_NONE).str;
    setenv("PWD", wd, 0);
    setenv("OLDPWD", DEFAULT_PWD_IF_NONE, 0);
    free(wd);
    str_list_node_t tmp = {.str = raw_to_string(get_pwd())};
    cd_builtin(xll_init(tmp));
    return set_shell_ret_val(0);
}

static int init(void){
    set_sigint_handler(&handler_when_idle);
    return init_envvars();
}


int shell_main(int argc, char **argv){

    if(init())
        return shell_ret_val;

    if(argc <= 1)
        interactive_mode();
    else
        noninteractive_mode(argc, argv);

    return shell_ret_val;
}