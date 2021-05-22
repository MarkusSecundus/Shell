#ifndef _STRINGS__GUARD___dhnskadfhlahflkjajhdlksjakljfdnslkjhdflieshfglkjdsnjdlkasjldkasjl
#define _STRINGS__GUARD___dhnskadfhlahflkjajhdlksjakljfdnslkjhdflieshfglkjdsnjdlkasjldkasjl

#include "mysh_header.h"



/**
 * Wrapper arround C-string that remembers its length
*/
typedef struct
{
    char *str;
    size_t len;
} string_t;

#define NULL_STRING ((string_t){.len = 0, .str = NULL})


/**
 * Converts char[] literal to string_t literal
*/
#define as_string(lit) ((string_t){.str = (lit), .len = (sizeof(lit) - 1)})

/**
 * Dynamically grown buffer for creating a long string by parts.
*/
typedef struct
{
    string_t str;
    size_t buffer_len;
} string_buffer_t;

/**
 * Copies provided C-string into a new instance of string_t
*/
string_t str_copy(const char *to_copy);

/**
 * Literal representing a buffer of zero size that can be grown or appended to. 
*/
#define EMPTY_STRING_BUFFER ((string_buffer_t){.buffer_len = 0, .str = (string_t){.len = 0, .str = NULL}})

/**
 * Creates a buffer of specified initial length
*/
string_buffer_t make_string_buffer(int len);

/**
 * Grows the ammount of memory available to the buffer by specified length
*/
void grow_string_buffer(string_buffer_t *buf, size_t additional_length);

/**
 * Appends provided character to the buffer.
 * Grows the buffer automatically if necessary.
*/
void append_char_to_buffer(string_buffer_t *buf, char c);

/**
 * Linked list of strings.
*/
XLL_TYPEDEF(str_list_t, str_list_node_t,
    string_t str;
);

/**
 * Wraps the provided C-string reference into a string_t. 
*/
string_t raw_to_string(char *raw);

/**
 * Creates a copy of the provided string.
*/
string_t str_cpy(string_t to_copy);
/**
 * Determines whether the provided strings are the same.
*/
int str_equals(string_t a, string_t b);

/**
 * Creates a new string that is a concatenation of the two arguments.  
*/
string_t str_concat(string_t a, string_t b);

#endif