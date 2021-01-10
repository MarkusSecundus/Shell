#ifndef _MACRO_MAGIC_h__GUARD___dshalkjhsdloajdlsajmdlaskj
#define _MACRO_MAGIC_h__GUARD___dshalkjhsdloajdlsajmdlaskj

#include<stdarg.h>



/**
 * Vrací něco, co je kompilátor schopen vyhodnotit jako číselnou konstantu rovnou počtu variadických argumentů předaných tomuto makru.
 * 
 * Všechny argumenty musí být stejného typu - ten je předán jako 0. argument a do výsledného počtu se nepočítá.
*/
#define VA_ARGS_COUNT(type, ...) (sizeof((type[]){__VA_ARGS__})/sizeof(type))



/**
 * Uplatní funkci nebo makro danou 0. argumentem na všechny předané prvky. 
 * 
 * Všechny předané prvky musí být stejného typu, který je specifikovaný 1. argumentem,
 * vnitřní funkci, která přes ně iteruje, budou předány hodnotou.
*/
#define VA_ARGS_APPLY_TO_ALL(what, type, ...) ({\
    inline void __MACRO_MAGIC__applier_function___(int __MACRO_MAGIC__applier_function___remaining_count___, ...){\
        va_list __MACRO_MAGIC__applier_function___va_args___;\
        \
        va_start(__MACRO_MAGIC__applier_function___va_args___, __MACRO_MAGIC__applier_function___remaining_count___);\
        \
        while( --__MACRO_MAGIC__applier_function___remaining_count___ >= 0 ){\
            what(va_arg(__MACRO_MAGIC__applier_function___va_args___, type));\
        }\
        va_end(__MACRO_MAGIC__applier_function___va_args___);\
    };\
    __MACRO_MAGIC__applier_function___(VA_ARGS_COUNT(type, __VA_ARGS__), __VA_ARGS__);\
})



/**
 * Zavine funkci danou 0. argumentem skrze všechny předané prvky.
 * 
 * Všechny předané prvky musí být stejného typu, který je specifikovaný 2. argumentem,
 * vnitřní funkci, která přes ně iteruje, budou předány hodnotou.
*/
#define VA_ARGS_FOLD_LEFT(applier, result_t, args_t, accumulator_init, ...) ({\
    inline result_t __MACRO_MAGIC__foldl_function___(result_t __MACRO_MAGIC__foldl_function___accumulator___, int __MACRO_MAGIC__foldl_function___remaining_count___, ...){\
        va_list __MACRO_MAGIC__foldl_function___va_args___;\
        \
        va_start(__MACRO_MAGIC__foldl_function___va_args___, __MACRO_MAGIC__foldl_function___remaining_count___);\
        \
        while(  --__MACRO_MAGIC__foldl_function___remaining_count___ >= 0 ){\
            __MACRO_MAGIC__foldl_function___accumulator___ = applier(__MACRO_MAGIC__foldl_function___accumulator___, va_arg(__MACRO_MAGIC__foldl_function___va_args___, args_t));\
        }\
        va_end(__MACRO_MAGIC__foldl_function___va_args___);\
        return __MACRO_MAGIC__foldl_function___accumulator___;\
    };\
    __MACRO_MAGIC__foldl_function___((accumulator_init), VA_ARGS_COUNT(args_t, __VA_ARGS__), __VA_ARGS__);\
})

#define VA_ARGS_FOLD_LEFT_IMPLICIT_TYPE(applier, first, second, ...) VA_ARGS_FOLD_LEFT(applier, __typeof__(first), __typeof__(second), first, second, __VA_ARGS__)




/**Prohodí hodnoty daných argumentů.*/
#define SWAP(a,b) ( (void) ({\
    const __typeof__(a) __MACRO_MAGIC__swapper__tmp___ = (a);\
    (a) = (b);\
    (b) = __MACRO_MAGIC__swapper__tmp___;\
}))



/**
 * Nadeklaruje novou proměnnou s daným jménem, inicializovanou daným výrazem, jejíž typ je odvozen z typu onoho výrazu
 * 
 * Aby to v kódu vypadalo názorněji, napsání přiřadítka za jménem proměnné ponecháváme na volajícím -> používá se stylem:
 *  AUTO(my_variable =, get_some_value(..) );
 * 
 * Nefunguje 
 * */
#define AUTO(name_and_assignment_op, ...) __typeof__(__VA_ARGS__) name_and_assignment_op (__VA_ARGS__)



/**Vrací offset prvku s daným jménem v dané struktuře od jejího začátku.*/
#define ELEMENT_OFFSET_OF(type, member) \
    ((size_t) & ((type*)0)->member)


/**Přetypuje pointer na prvek s daným jménem v dané struktuře na pointer na onu strukturu, ve které je obsažený.*/
#define GENERIC_CONTAINER_OF(ptr, type, member) \
    ({ \
        const __typeof__(((type*)0)->member)* __mptr = (ptr); \
        (type*)((void*)__mptr - ELEMENT_OFFSET_OF(type, member)); \
    })




#define STRINGIFY(s) = #s






#endif
