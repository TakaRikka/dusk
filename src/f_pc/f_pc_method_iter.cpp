/**
 * f_pc_method_iter.cpp
 * Framework - Process Method Iteration
 */

#include "SSystem/SComponent/c_list_iter.h"
#include "f_pc/f_pc_method_iter.h"

int fpcMtdIt_Method(node_list_class* i_nodeList, fpcMtdIt_MethodFunc i_methods) {
    return cLsIt_Method(i_nodeList, (cNdIt_MethodFunc)i_methods, NULL);
}
