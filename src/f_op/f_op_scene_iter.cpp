/**
 * f_op_scene_iter.cpp
 * Framework - Scene Process Iteration
 */

#include "f_op/f_op_scene_iter.h"
#include "SSystem/SComponent/c_list_iter.h"
#include "SSystem/SComponent/c_tag_iter.h"
#include "f_op/f_op_scene_tag.h"

// Wii: NONMATCHING, regalloc, scheduling
void* fopScnIt_Judge(fop_ScnItFunc i_judgeFunc, void* i_data) {
    struct {
        fop_ScnItFunc func;
        void* data;
    } userdata;

    userdata.func = i_judgeFunc;
    userdata.data = i_data;
    return cLsIt_Judge(&g_fopScnTg_SceneList, (cNdIt_JudgeFunc)cTgIt_JudgeFilter, &userdata);
}
