/**
 * f_pc_line_iter.cpp
 * Framework - Process Line Iterator
 */

#include "f_pc/f_pc_line_iter.h"
#include "SSystem/SComponent/c_tag_iter.h"
#include "SSystem/SComponent/c_tree_iter.h"
#include "f_pc/f_pc_base.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_line.h"


static s32 fpcLnIt_MethodCall(create_tag_class* i_createTag, method_filter* i_filter) {
    layer_class* layer = static_cast<base_process_class*>(i_createTag->mpTagData)->layer_tag.layer;
    layer_class* save_layer = fpcLy_CurrentLayer();
    s32 ret;

    fpcLy_SetCurrentLayer(layer);
    ret = cTgIt_MethodCall(i_createTag, i_filter);
    fpcLy_SetCurrentLayer(save_layer);

    return ret;
}

void fpcLnIt_Queue(fpcLnIt_QueueFunc i_queueFunc) {
    method_filter filter;
    filter.mpMethodFunc = (cNdIt_MethodFunc)i_queueFunc;
    filter.mpUserData = NULL;
    cTrIt_Method(&g_fpcLn_Queue, (cNdIt_MethodFunc)fpcLnIt_MethodCall, &filter);
}
