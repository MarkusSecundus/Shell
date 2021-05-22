#ifndef _ENVVARS__GUARD___jneskjdfslkajfksdojhflksdjfklsdjflksdjklfsjdlkfjdsol
#define _ENVVARS__GUARD___jneskjdfslkajfksdojhflksdjfklsdjflksdjklfsjdlkfjdsol


#include "mysh_header.h"
#include "strings.h"


/**
 * Returns path to the current directory. 
 * Ownership of the returned string is passed to caller.  
*/
char* get_current_dir(void);

/**
 * Returns path to the home directory. 
 * Ownership of the returned string is passed to caller.  
*/
string_t get_home_dir(void);

/**
 * Returns an observer pointer to the current value of PWD environment variable.
 * Should not be freed nor preserved for nontrivial time.
*/
char *get_pwd(void);
/**
 * Returns an observer pointer to the current value of OLDPWD environment variable.
 * Should not be freed nor preserved for nontrivial time.
*/
char *get_oldpwd(void);

/**
 * Copies provided value in the PWD environment variable.
*/
int set_pwd(const char *value);
/**
 * Copies provided value in the OLDPWD environment variable.
*/
int set_oldpwd(const char *value);









#endif