/**
 * f_pc_line_tag.cpp
 * Framework - Process Line Tag
 */

#include "f_pc/f_pc_line_tag.h"
#include "f_pc/f_pc_line.h"


s32 fpcLnTg_Move(line_tag* i_lineTag, int i_newListID) {
    if (i_lineTag->list_id != i_newListID) {
        fpcLnTg_QueueTo(i_lineTag);
        return fpcLnTg_ToQueue(i_lineTag, i_newListID);
    }

    return 1;
}

void fpcLnTg_QueueTo(line_tag* i_lineTag) {
    cTg_SingleCutFromTree(&i_lineTag->base);
    i_lineTag->list_id = -1;
}

s32 fpcLnTg_ToQueue(line_tag* i_lineTag, int lineListID) {
    if (cTg_AdditionToTree(&g_fpcLn_Queue, lineListID, &i_lineTag->base)) {
        i_lineTag->list_id = lineListID;
        return 1;
    } else {
        return 0;
    }
}

void fpcLnTg_Init(line_tag* i_lineTag, void* i_data) {
    cTg_Create(&i_lineTag->base, i_data);
    i_lineTag->list_id = -1;
}
