
#include "ultimate_list.h"



static xor_linked_list_t* xor_(xor_linked_list_t *a, xor_linked_list_t *b){
    return (xor_linked_list_t*)(void*) (((size_t)(a)) ^ ((size_t)(b)));
}


xor_linked_list_handl_t xll_init_impl(xor_linked_list_t *self){
    self->xor_of_next_and_last = (xor_linked_list_t*) 0; //jedna vec sama se sebou da jako xor vzdy nulu
    return (xor_linked_list_handl_t){.current = self, .last = self, .length = 1};
}


xor_linked_list_handl_t xll_next_impl(xor_linked_list_handl_t self){
    xor_linked_list_t *next =  xor_(self.last, self.current->xor_of_next_and_last);
    return (xor_linked_list_handl_t){.last = self.current, .current = next, .length = self.length};
}

xor_linked_list_handl_t xll_last_impl(xor_linked_list_handl_t self){
    xor_linked_list_t *before_last =  xor_(self.last->xor_of_next_and_last, self.current);
    return (xor_linked_list_handl_t){.last = before_last, .current = self.last, .length = self.length};
}




xor_linked_list_handl_t xll_concat_impl(xor_linked_list_handl_t self, xor_linked_list_handl_t to_append){
    if(self.length==0)
        return to_append;
    if(to_append.length==0)
        return self;
    if(self.current == self.last){
        if(to_append.current == to_append.last){
            to_append.current->xor_of_next_and_last 
                = self.current->xor_of_next_and_last = (xor_linked_list_t*)0;
        }
        else{
            self.current->xor_of_next_and_last = xor_(to_append.last, to_append.current);

            xor_linked_list_t* to_append__next = xll_next_impl(to_append).current;
            xor_linked_list_t* to_append__last__last = xll_last_impl(to_append).last;

            to_append.current->xor_of_next_and_last = xor_(self.last, to_append__next);
            to_append.last->xor_of_next_and_last = xor_(to_append__last__last, self.current);
        }
    }
    else if(to_append.current == to_append.last){
        to_append.current->xor_of_next_and_last = xor_(self.last, self.current);

        xor_linked_list_t* self__next = xll_next_impl(self).current;
        xor_linked_list_t* self__last__last = xll_last_impl(self).last;

        self.current->xor_of_next_and_last = xor_(self__next, to_append.last);
        self.last->xor_of_next_and_last = xor_(self__last__last, to_append.current);
    }
    else{
        xor_linked_list_t* self__last__last = xll_last_impl(self).last;
        xor_linked_list_t* self__next = xll_next_impl(self).current;
        xor_linked_list_t* to_append__last__last = xll_last_impl(to_append).last;
        xor_linked_list_t* to_append__next = xll_next_impl(to_append).current;

        self.last->xor_of_next_and_last = xor_(self__last__last, to_append.current);      //self.last.next = to_append
        to_append.last->xor_of_next_and_last = xor_(to_append__last__last, self.current); //to_append.last.next = self
        self.current->xor_of_next_and_last = xor_(self__next, to_append.last);            //self.last = to_append.last
        to_append.current->xor_of_next_and_last = xor_(to_append__next, self.last);       //to_append.last = self.last
    }
    return (xor_linked_list_handl_t){.last = to_append.last, .current = self.current, .length = to_append.length + self.length};
}




xor_linked_list_handl_t xll_extract_impl(xor_linked_list_handl_t self){
    if(self.length == 0)
        return self;
    if(self.current->xor_of_next_and_last == (xor_linked_list_t*)0 ){   //if (self.next == self.last) -> list of length 1 or 2
        return xll_init_impl(self.last);
    }else{
        xor_linked_list_t* last = self.last;
        xor_linked_list_t* last_last = xll_last_impl(self).last;
        xor_linked_list_handl_t next_next_ptr = xll_next_impl(xll_next_impl(self));
        xor_linked_list_t* next = next_next_ptr.last;
        xor_linked_list_t* next_next = next_next_ptr.current;

        next->xor_of_next_and_last = xor_(last, next_next);
        last->xor_of_next_and_last = xor_(last_last, next);

        return (xor_linked_list_handl_t){.last=last, .current = next, .length = self.length - 1};
    }
}


size_t xll_length_impl(xor_linked_list_handl_t self){
    if(self.length==0)
        return 0;
    xor_linked_list_handl_t it = self;
    size_t ret = 0;
    do{
        ++ret;
        it = xll_next_impl(it);
    } while(it.current != self.current);
    return ret;
}

