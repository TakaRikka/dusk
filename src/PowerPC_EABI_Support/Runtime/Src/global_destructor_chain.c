#include "NMWException.h"
#include "MWCPlusLib.h"

DestructorChain* __global_destructor_chain;

void* __register_global_object(void* object, void* destructor, void* regmem) {
    ((DestructorChain*)regmem)->next = __global_destructor_chain;
    ((DestructorChain*)regmem)->destructor = destructor;
    ((DestructorChain*)regmem)->object = object;
    __global_destructor_chain = (DestructorChain*)regmem;
    return object;
}

void __destroy_global_chain(void) {
    DestructorChain* iter;
    while ((iter = __global_destructor_chain) != 0) {
        __global_destructor_chain = iter->next;
        DTORCALL_COMPLETE(iter->destructor, iter->object);
    }
}

/* clang-format off */
#pragma section ".dtors$10"
__declspec(section ".dtors$10")
static void* const __destroy_global_chain_reference = __destroy_global_chain;
/* clang-format on */
