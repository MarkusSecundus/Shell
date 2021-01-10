#ifndef __ULTIMATE_LIST_ON_HEAP___GUARD____hnfdiusohfduisdhfkjdshjfjkiudshnjkfjdshfkijdshfikd
#define __ULTIMATE_LIST_ON_HEAP___GUARD____hnfdiusohfduisdhfkjdshjfjkiudshnjkfjdshfkijdshfikd

#include<stdlib.h>

#include "ultimate_list.h"


/**
 * Rozšíření ultimate_list.h o funkce řešící dynamickou alokaci a delalokaci na haldě.
 * 
 *  Autor: Jakub Hroník
**/


#ifndef xll_memory_allocator
#define xll_memory_allocator malloc
#endif

#define xll_create(type, ...) ({\
    type *_XLL_return_val___ = (type*)xll_memory_allocator(sizeof(type));\
    *_XLL_return_val___ = (type) __VA_ARGS__;\
    xll_init(*_XLL_return_val___);\
})

#define xll_add(list, ...) (xll_concat((list), xll_create(_XLL_get_list_type(__typeof__(list)), __VA_ARGS__)))
#define xll_prepend(list, ...) (xll_concat(xll_create(_XLL_get_list_type(__typeof__(list)), __VA_ARGS__), (list)))


#define xll_destroy_typed(handl_t, node_t, handl)({\
    handl_t _XLL_tmp_handl = (handl);\
    node_t *_XLL_tmp_to_be_destroyed__ = _XLL_tmp_handl.current;\
    handl_t _XLL_tmp_ret_val = _XLL_tmp_handl;\
    if(_XLL_tmp_ret_val.length>1) {\
        _XLL_tmp_ret_val = xll_extract(_XLL_tmp_ret_val);\
        free(_XLL_tmp_to_be_destroyed__);\
    }\
    else{\
        if(_XLL_tmp_handl.length == 1) \
            free(_XLL_tmp_to_be_destroyed__);\
        _XLL_tmp_ret_val = (handl_t){.current = NULL, .last = NULL, .length = 0};\
    }\
    _XLL_tmp_ret_val;\
})

#define xll_destroy(handl)\
 (xll_destroy_typed(__typeof__(handl), _XLL_get_list_type(__typeof__(handl)), handl))


#define xll_destroy_all(handl)({\
    __typeof__(handl) _XLL_tmp_handl_val_ = (handl);\
    while(_XLL_tmp_handl_val_.length > 0){\
        _XLL_tmp_handl_val_ = xll_destroy(_XLL_tmp_handl_val_);\
    }\
    _XLL_tmp_handl_val_;\
})

#endif