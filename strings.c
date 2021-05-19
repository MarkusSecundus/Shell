#include "strings.h"


//<strings>
string_t str_copy(const char *to_copy)
{
    int len = strlen(to_copy);
    string_t ret = {.len = len, .str = (char *)alloc_memory(sizeof(char) * (len + 1))};
    memcpy(ret.str, to_copy, len + 1);
    return ret;
}


string_t raw_to_string(char *raw)
{
    return (string_t){.len = strlen(raw), .str = raw};
}

string_t str_cpy(string_t to_copy)
{
    int len = to_copy.len;
    string_t ret = {.len = len, .str = (char *)alloc_memory(sizeof(char) * (len + 1))};
    memcpy(ret.str, to_copy.str, len + 1);
    return ret;
}

int str_equals(string_t a, string_t b)
{
    return a.len == b.len && !strcmp(a.str, b.str);
}

string_t str_concat(string_t a, string_t b)
{
    int len = a.len + b.len;
    char *ret = (char *)alloc_memory((len + 1) * sizeof(char));
    memcpy(ret, a.str, a.len);
    memcpy(ret + a.len, b.str, b.len);
    ret[len] = '\0';
    return (string_t){.len = len, .str = ret};
}

string_buffer_t make_string_buffer(int len)
{
    if (len < 16)
        len = 16;
    char *s = (char *)alloc_memory(len * sizeof(char));
    *s = '\0';
    return (string_buffer_t){.str = (string_t){.str = s, .len = 0}, .buffer_len = len};
}

void grow_buffer(string_buffer_t *buf, size_t len)
{
    buf->buffer_len += len;
    buf->str.str = (char *)realloc_memory(buf->str.str, buf->buffer_len);
}

void append_char_to_buffer(string_buffer_t *buf, char c)
{
    if (buf->str.len >= buf->buffer_len - 2)
    {
        grow_buffer(buf, 32);
    }
    buf->str.str[buf->str.len++] = c;
    buf->str.str[buf->str.len] = '\0';
}

//</>