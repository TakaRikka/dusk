/**
 * f_op_actor_iter.cpp
 * Actor Process Iterator
 */

#include "f_op/f_op_actor_iter.h"
#include "SSystem/SComponent/c_list_iter.h"
#include "SSystem/SComponent/c_tag_iter.h"
#include "f_op/f_op_actor_tag.h"

int fopAcIt_Executor(fopAcIt_ExecutorFunc i_execFunc, void* i_data) {
    struct {
        fopAcIt_ExecutorFunc func;
        void* data;
    } userData;
    userData.func = i_execFunc;
    userData.data = i_data;

    return cLsIt_Method(&g_fopAcTg_Queue, (cNdIt_MethodFunc)cTgIt_MethodCall, &userData);
}

void* fopAcIt_Judge(fopAcIt_JudgeFunc i_judgeFunc, void* i_data) {
    struct {
        fopAcIt_JudgeFunc func;
        void* data;
    } userData;
    userData.func = i_judgeFunc;
    userData.data = i_data;

    return cLsIt_Judge(&g_fopAcTg_Queue, (cNdIt_JudgeFunc)cTgIt_JudgeFilter, &userData);
}
