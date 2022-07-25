#ifndef _MACRO_MAGIC_h__GUARD___dshalkjhsdloajdlsajmdlaskj
#define _MACRO_MAGIC_h__GUARD___dshalkjhsdloajdlsajmdlaskj

#include<stdarg.h>


/**
 * Drobná pomnocná makra, která jsem si původně napsal jako pomůcku k úkolu z Operačních systémů.
 * 
 * Autor: Jakub Hroník
**/


/**
 * Vrací něco, co je kompilátor schopen vyhodnotit jako číselnou konstantu rovnou počtu variadických argumentů předaných tomuto makru.
 * 
 * Všechny argumenty musí být stejného typu - ten je předán jako 0. argument a do výsledného počtu se nepočítá.
**/
#define VA_ARGS_COUNT(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))



#ifdef __clang__
#define LAMBDA_DECLARE(ret_t, name, ...) ret_t (^name) __VA_ARGS__;
#else
#define LAMBDA_DECLARE(ret_t, name, ...) ret_t (*name) __VA_ARGS__;
#endif



#ifdef __clang__
#define LAMBDA(ret_t, ...) (^ret_t __VA_ARGS__)
#else
#define LAMBDA(ret_t, ...) \
    ({\
        ret_t __lambda___ __VA_ARGS__;\
        __lambda___;\
    })
#endif


/**
 * Uplatní funkci nebo makro danou 0. argumentem na všechny předané prvky. 
 * 
 * Všechny předané prvky musí být stejného typu, který je specifikovaný 1. argumentem,
 * vnitřní funkci, která přes ně iteruje, budou předány hodnotou.
**/
#define VA_ARGS_APPLY_TO_ALL(what, type, ...) (\
    LAMBDA(void, (int __MACRO_MAGIC__applier_function___remaining_count___, ...){\
        va_list __MACRO_MAGIC__applier_function___va_args___;\
        \
        va_start(__MACRO_MAGIC__applier_function___va_args___, __MACRO_MAGIC__applier_function___remaining_count___);\
        \
        while( --__MACRO_MAGIC__applier_function___remaining_count___ >= 0 ){\
            what(va_arg(__MACRO_MAGIC__applier_function___va_args___, type));\
        }\
        va_end(__MACRO_MAGIC__applier_function___va_args___);\
    })\
    (VA_ARGS_COUNT(type, __VA_ARGS__), __VA_ARGS__)\
)



/**
 * Zavine funkci danou 0. argumentem skrze všechny předané prvky.
 * 
 * Všechny předané prvky musí být stejného typu, který je specifikovaný 2. argumentem,
 * vnitřní funkci, která přes ně iteruje, budou předány hodnotou.
**/
#define VA_ARGS_FOLD_LEFT(applier, result_t, args_t, accumulator_init, ...) (\
    LAMBDA(result_t, (result_t __MACRO_MAGIC__foldl_function___accumulator___, int __MACRO_MAGIC__foldl_function___remaining_count___, ...){\
        va_list __MACRO_MAGIC__foldl_function___va_args___;\
        \
        va_start(__MACRO_MAGIC__foldl_function___va_args___, __MACRO_MAGIC__foldl_function___remaining_count___);\
        \
        while(  --__MACRO_MAGIC__foldl_function___remaining_count___ >= 0 ){\
            __MACRO_MAGIC__foldl_function___accumulator___ = applier(__MACRO_MAGIC__foldl_function___accumulator___, va_arg(__MACRO_MAGIC__foldl_function___va_args___, args_t));\
        }\
        va_end(__MACRO_MAGIC__foldl_function___va_args___);\
        return __MACRO_MAGIC__foldl_function___accumulator___;\
    })\
    ((accumulator_init), VA_ARGS_COUNT(args_t, __VA_ARGS__), __VA_ARGS__)\
)

#define VA_ARGS_FOLD_LEFT_IMPLICIT_TYPE(applier, first, second, ...) VA_ARGS_FOLD_LEFT(applier, __typeof__(first), __typeof__(second), first, second, __VA_ARGS__)




/**Prohodí hodnoty daných argumentů.*/
#define SWAP(a,b) ( (void) ({\
    const __typeof__(a) __MACRO_MAGIC__swapper__tmp___ = (a);\
    (a) = (b);\
    (b) = __MACRO_MAGIC__swapper__tmp___;\
}))






/**Vrací offset prvku s daným jménem v dané struktuře od jejího začátku.*/
#define ELEMENT_OFFSET_OF(type, member) \
    ((size_t) & ((type*)0)->member)


/**Přetypuje pointer na prvek s daným jménem v dané struktuře na pointer na onu strukturu, ve které je obsažený.*/
#define GENERIC_CONTAINER_OF(ptr, type, member) \
    ({ \
        const __typeof__(((type*)0)->member)* __mptr = (ptr); \
        (type*)((void*)__mptr - ELEMENT_OFFSET_OF(type, member)); \
    })




#define REDIRECT_VARARGS(where, what)\
    ({\
        va_list _args__;\
        va_start(_args__, what);\
        AUTO(_ret__ =, where(what, _args__));\
        va_end(_args__);\
        _ret__;\
    })

#define REDIRECT_VARARGS_VOID(where, what)\
    ({\
        va_list _args__;\
        va_start(_args__, what);\
        where(what, _args__);\
        va_end(_args__);\
    })



    
#endif
