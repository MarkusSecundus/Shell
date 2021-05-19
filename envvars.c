#include "envvars.h"

#include<pwd.h>
#include<unistd.h>


//<manipulation of environment variables>



char* get_current_dir(void)
{
    char *ret = getcwd(NULL, 0);
    if(ret)
        return ret;
    
    string_buffer_t buf = make_string_buffer(32);
    while(!(ret = getcwd(buf.str.str, buf.buffer_len)))
        grow_buffer(&buf, 32);
    return ret;
}


string_t get_home_dir(void)
{
    char *ret = getenv("HOME");
    if (!ret)
        ret = getpwuid(getuid())->pw_dir;
    return str_copy(ret);
}

char *get_pwd(void)
{
    return getenv("PWD");
}
char *get_oldpwd(void)
{
    return getenv("OLDPWD");
}

int set_pwd(const char *value)
{
    return setenv("PWD", value, 1);
}
int set_oldpwd(const char *value)
{
    return setenv("OLDPWD", value, 1);
}

//</>

