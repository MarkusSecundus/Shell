#ifndef _STRINGS__GUARD___dhnskadfhlahflkjajhdlksjakljfdnslkjhdflieshfglkjdsnjdlkasjldkasjl
#define _STRINGS__GUARD___dhnskadfhlahflkjajhdlksjakljfdnslkjhdflieshfglkjdsnjdlkasjldkasjl


#include "mysh_header.h"



typedef struct{
    char *str;
    size_t len;
} string_t;

#define NULL_STRING ((string_t){.len=0, .str=NULL})

//converts char[] literal to string_t literal
#define as_string(lit) ( (string_t){.str = (lit), .len = (sizeof(lit) - 1)} )



typedef struct{
    string_t str;
    size_t buffer_len;
} string_buffer_t;

string_t str_copy(const char *to_copy);

#define string_buffer_empty ( (string_buffer_t){.buffer_len = 0, .str = (string_t){.len = 0, .str = NULL}} )

string_buffer_t make_string_buffer(int len);

void grow_buffer(string_buffer_t *buf, size_t len);
void append_char_to_buffer(string_buffer_t *buf, char c);

XLL_TYPEDEF(str_list_t, str_list_node_t,
    string_t str;
);

string_t raw_to_string(char *raw);
string_t str_cpy(string_t to_copy);
int str_equals(string_t a, string_t b);
string_t str_concat(string_t a, string_t b);

#endif