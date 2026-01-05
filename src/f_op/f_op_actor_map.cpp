/**
 * f_op_actor_map.cpp
 * Actor Map (Debug only)
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "f_op/f_op_actor_mng.h"


void dummy(s16 i_procName, createFunc i_createFunc, void* params) {
    fopAcM_Create(i_procName, i_createFunc, params);
}
