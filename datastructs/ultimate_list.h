
#ifndef _ULTIMATE_LIST_H__GUARD__fdnhsskjnfhskjihfkjdsnhfgkjdesnjkjiodsnjfk
#define _ULTIMATE_LIST_H__GUARD__fdnhsskjnfhskjihfkjdsnhfgkjdesnjkjiodsnjfk


#include<stddef.h>
#include<stdarg.h>
#include "macro_magic.h"

/**
 * XORovaný spoják, který jsem si původně napsal jako pomůcku k úkolu z Operačních systémů.
 * 
 * Autor: Jakub Hroník
**/

typedef int xll_length_t;

/**
 * Header of a generic xor_linked_list node - to be embedded inside a node struct.
**/
typedef struct xor_linked_list{
    struct xor_linked_list* xor_of_next_and_last;
} xor_linked_list_t;

/**
 * Generic type for handling a xor linked list.
**/
typedef struct xor_linked_list_handl{
    xor_linked_list_t *last;
    xll_length_t length;
    xor_linked_list_t *current;
} xor_linked_list_handl_t;



/** Iterate over list members
 * 
 * @param type Type of the list we are iterating over
 *
 * @param iterator The name of the iterator to declare.
 * 
 * @param list     The list to iterate over.
 *
**/
#define xll_foreach_typed(handl_type, iterator, list) \
    for(int _XLL_foreach_flag__is_first_iteration___=1; _XLL_foreach_flag__is_first_iteration___==1 ; _XLL_foreach_flag__is_first_iteration___ = 0)\
    for(_XLL_get_list_type(handl_type)* iterator = NULL; iterator == NULL ; iterator = (void*)1)\
    for(handl_type _XLL_foreach_base___ = (list), _XLL_foreach_iterator___ = ((_XLL_foreach_flag__is_first_iteration___=(_XLL_foreach_base___.length>0)), _XLL_foreach_base___) ;\
        (iterator = _XLL_foreach_iterator___.current),\
         (_XLL_foreach_flag__is_first_iteration___ || iterator != _XLL_foreach_base___.current);\
        (_XLL_foreach_flag__is_first_iteration___=0), _XLL_foreach_iterator___ = xll_next(_XLL_foreach_iterator___)   )

/** Iterate over list members
 *
 * @param iterator The name of the iterator to declare.
 * 
 * @param list     The list to iterate over.
 *
 * Usage example:
 * 
 *   XLL_TYPEDEF(ll_h, ll_t,  //define a node and a handle to it
 *       int num;
 *   );
 *   ...
 * 
 *   ll_h a = ...;
 * 
 *   xll_foreach(it, a){                 // similar to Java's "for(var it : a) ...; "
 *      printf("List item: %d", it->num);
 *   }  //curly braces are only optional - single statement without them is also possible
 * 
**/
#define xll_foreach(iterator, list) xll_foreach_typed(__typeof__(list), iterator, list)



/**
 *  A name under which the linked list header gets embedded inside the node by convention - all macros in this header assume this name is used.
**/
#define XLL_EMBED_NAME_CONVENTION ___EMBED__xor_linked_list__INSTANCE___

/**
 * Embeds the linked list inside a struct.
 * 
 * Intended for use only by someone who is totally sure what he is doing and has a good reason to use this macro directly.
 * Normal user should just define the node type through XLL_TYPEDEF instead.
**/
#define XLL_EMBED(name_of_handle_type)\
struct{\
    xor_linked_list_t Impl;\
    union {\
        name_of_handle_type * handl_t;\
    } _typeinfo__[];\
} XLL_EMBED_NAME_CONVENTION


/**
 * Defines a handle type to a specific type that has xor_linked_list_t embedded.
 * 
 * Intended for use only by someone who is totally sure what he is doing and has a good reason to use this macro directly.
 * Normal user should just define both the node and handle types together through XLL_TYPEDEF instead.
**/
#define XLL_DEFINE_HANDL_T(handl_t, node_t)\
    struct handl_t{\
        node_t *current;\
        node_t *last;\
        xll_length_t length;\
    };\
    _Static_assert(sizeof(handl_t) == sizeof(xor_linked_list_handl_t), "size of generated handl must be same as of the generic one!")


/**
 * Defines a type to be used as a xor list node along with an appropriate handle type for it.
 * 
 * @param handl_t name for the handle type to be defined
 * 
 * @param node_t name for the node type to be defined
 * 
 * @param ... body of the node type (written in the same way a body of a struct would be) (XLL_EMBED part will by automatically added by the macro and shouldn't be explicitly specified)
 * 
 * Usage example: 
 *     XLL_TYPEDEF(employee_list_t, employee_node_t,
 *          int id;
 *          const char *first_name, *last_name;   //some fields for the node type
 *     )
 *     
 *     employee_node_t employee1, employee2; //make some instances of the node type just like with any other type made through typedef
 *     employee1.id = 432;
 * 
 *     employee_list_t employee1_list = xll_init(employee1);  //make handles for the nodes to use them as lists
 *     employee_list_t employee2_list = xll_init(employee2); 
 * 
 *     printf("id1: %d", employee1_list.current->id);  // >>> 'id1: 432'       //use employee_list_t::current to get the node being pointed to
 *     ...
**/
#define XLL_TYPEDEF(handl_t, node_t, ...) \
    struct node_t;\
    struct handl_t;\
    \
    typedef struct node_t node_t;\
    typedef struct handl_t handl_t;\
    \
    XLL_DEFINE_HANDL_T(handl_t, node_t); \
    \
    struct node_t{\
        __VA_ARGS__\
        XLL_EMBED(handl_t);\
    };\
    _Static_assert(sizeof(((node_t*)NULL)->XLL_EMBED_NAME_CONVENTION) == sizeof(xor_linked_list_t), "inconsistency - the node has different size when embedded!");



/**Gets the appropriate handle type for the specified xor list node type.*/
#define _XLL_get_handl_type(list_type) __typeof__( *( ((list_type*)0)->XLL_EMBED_NAME_CONVENTION._typeinfo__[0].handl_t)  )

/**Gets the appropriate node type pointed to by the specified xor list handle type.*/
#define _XLL_get_list_type(handl_type) __typeof__(  *(((handl_type*)0)->current)   )



/**Casts a xor_linked_list_t* to a pointer to the desired type that has xll embedded.*/
#define xll_upcast(handl, type) ((type*) GENERIC_CONTAINER_OF(handl, type, XLL_EMBED_NAME_CONVENTION.Impl))

/**Casts a pointer to a specific xor list node type with xor_linked_list_t embedded to pointer to the embedded xor_linked_list_t.*/
#define XLL_downcast_node___(node) ((node)->XLL_EMBED_NAME_CONVENTION.Impl)

/**Casts a handle for a specific xor list node down to a generic xor_linked_list_handl_t.*/
#define XLL_downcast_typed_handl___(handl)({\
    __typeof__(handl) _XLL_tmp_handl___ = (handl);\
    (xor_linked_list_handl_t){.length=_XLL_tmp_handl___.length, .last=&(_XLL_tmp_handl___.last->XLL_EMBED_NAME_CONVENTION.Impl), .current=&(_XLL_tmp_handl___.current->XLL_EMBED_NAME_CONVENTION.Impl)};\
})


#define XLL_upcast_handl_explicittyped___(handl, handl_type, list_type)({\
    xor_linked_list_handl_t _XLL_tmp_handl___ = (handl);\
    (handl_type){.length = _XLL_tmp_handl___.length, .last = xll_upcast(_XLL_tmp_handl___.last, list_type), .current = xll_upcast(_XLL_tmp_handl___.current, list_type)};\
})

#define XLL_upcast_handl___(handl, handl_type) XLL_upcast_handl_explicittyped___(handl, handl_type,  _XLL_get_list_type(handl_type) )


/**
 * Inits the list node.
 * 
 * Returns a handle for the newly initialized one-element list.
**/
xor_linked_list_handl_t xll_init_impl(xor_linked_list_t *self);


/**
 * Returns a handle for the next node in the linked list.
**/
xor_linked_list_handl_t xll_next_impl(xor_linked_list_handl_t self);

/**
 * Returns a handle for the last node in the linked list.
**/
xor_linked_list_handl_t xll_last_impl(xor_linked_list_handl_t self);


/**
 * Concatenates two arbitrarily long lists in place in O(1) time.
 * Returns a handle pointing to the beginning of the newly created list.
 * All other handles pointing to the first node of any of the two lists get invalidated and their further usage results in undefined behavior.
 * 
 * @param self list that will be first in the result
 * @param to_append list to be appended to the list 'self'
**/
xor_linked_list_handl_t xll_concat_impl(xor_linked_list_handl_t self, xor_linked_list_handl_t to_append);

/**
 * Extracts the first element of the list pointed to by 'self'.
 * Returns a valid handle to the modified list with first element removed.
 * All other handles pointing to the beginning of the modified list get invalidated and their further usage results in undefined behavior.
 * 
 * The extracted node should be stored externally before calling this function and then again reinitialized.
**/
xor_linked_list_handl_t xll_extract_impl(xor_linked_list_handl_t self);

/**
 * Computes number of nodes in the list in O(n) time.
**/
size_t xll_length_impl(xor_linked_list_handl_t self);




#define xll_empty(handl_t) ((handl_t){.current=NULL, .last=NULL, .length=0})


/**
 * Inits the list node.
 * 
 * Returns appropriately typed handle for the newly initialized one-element list.
 * 
 * The node is passed to the macro by value.
 * 
 * Usage example:
 *    XLL_TYPEDEF(handl_t, node_t,
 *      ...
 *    );
 *    ...
 *    node_t n;
 *    handl_t h = xll_init(n);
**/
#define xll_init(self) (XLL_upcast_handl___( xll_init_impl(&((self).XLL_EMBED_NAME_CONVENTION.Impl)), _XLL_get_handl_type(__typeof__(self))))

/**
 * Returns handle for next element in the list.
*/
#define xll_next(self) (XLL_upcast_handl___( xll_next_impl(XLL_downcast_typed_handl___(self)) , __typeof__(self) ) )

/**
 * Returns handle for last element in the list.
**/
#define xll_last(self) (XLL_upcast_handl___( xll_last_impl(XLL_downcast_typed_handl___(self)) , __typeof__(self) ))


/**
 * Concatenates two arbitrarily long lists in place in O(1) time.
 * Returns a handle pointing to the beginning of the newly created list.
 * All other handles pointing to the first node of any of the two lists get invalidated and their further usage results in undefined behavior.
 * 
 * @param self handle for the list that will be first in the result
 * @param to_append handle for the list to be appended to the list 'self'
**/
#define xll_concat(self, to_append) (XLL_upcast_handl___(xll_concat_impl(XLL_downcast_typed_handl___(self), XLL_downcast_typed_handl___(to_append)), __typeof__(self)))

/**
 * Extracts the first element of the list pointed to by 'self'.
 * Returns a valid handle to the modified list with first element removed.
 * All other handles pointing to the beginning of the modified list get invalidated and their further usage results in undefined behavior.
 * 
 * The extracted node should be stored externally before calling this function and then again reinitialized.
 * 
 * Usage example:
 * Usage example:
 *    XLL_TYPEDEF(handl_t, node_t,
 *      ...
 *    );
 *    ...
 *    handl_t list = ...;
 *    node_t to_be_removed = list.current;
 *    list = xll_extract(list);
 *    handl_t removed_elem = xll_init(to_be_removed);
**/
#define xll_extract(self) (XLL_upcast_handl___(xll_extract_impl(XLL_downcast_typed_handl___(self)), __typeof__(self)))


/**
 * Computes number of nodes in the list in O(n) time.
**/
#define xll_length(self) (xll_length_impl(XLL_downcast_typed_handl___(self)))



#define _XLL_private___init_all__applier_function___(acc, next) (xll_concat((acc), xll_init(*next))) 

/**
 * Inits all the list nodes given by pointer to the enclosing struct type and concats them into one list.
 * 
 * usage eg. :
 *   node_t a, b, c, d, e;  //declare some list nodes
 *   handl_t h = xll_init(&a, &b, &c, &d, &e);  //now h = list [a->b->c->d->e->a->...]
**/
#define xll_init_all(first, ...) (VA_ARGS_FOLD_LEFT(_XLL_private___init_all__applier_function___, _XLL_get_handl_type(__typeof__(*(first))), __typeof__(first),  xll_init(*(first)), __VA_ARGS__))

/**
 * Concats all the lists given by their handles into one big list, each concatenation in O(1) time.
 * 
 * usage eg. :
 *    handl_t a, b, c, d;
 *    a = xll_concat_all(a,b,c,d);   //handles b, c, d are now invalid! ( <- see xll_concat)
**/
#define xll_concat_all(first, ...) (VA_ARGS_FOLD_LEFT(xll_concat, __typeof__(first), __typeof__(first), (first), __VA_ARGS__))






#endif