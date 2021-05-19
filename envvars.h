#ifndef _ENVVARS__GUARD___jneskjdfslkajfksdojhflksdjfklsdjflksdjklfsjdlkfjdsol
#define _ENVVARS__GUARD___jneskjdfslkajfksdojhflksdjfklsdjflksdjklfsjdlkfjdsol


#include "mysh_header.h"
#include "strings.h"


char* get_current_dir(void);
string_t get_home_dir(void);
char *get_pwd(void);
char *get_oldpwd(void);
int set_pwd(const char *value);
int set_oldpwd(const char *value);









#endif