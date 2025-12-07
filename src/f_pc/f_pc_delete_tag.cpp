/**
 * f_pc_delete_tag.cpp
 * Framework - Process Delete Tag
 */

#include "SSystem/SComponent/c_list.h"
#include "f_pc/f_pc_delete_tag.h"
#include "f_pc/f_pc_debug_sv.h"

node_list_class g_fpcDtTg_Queue = {NULL, NULL, 0};

BOOL fpcDtTg_IsEmpty() {
    if (g_fpcDtTg_Queue.mSize == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}

void fpcDtTg_ToDeleteQ(delete_tag_class* i_deleteTag) {
    i_deleteTag->timer = 1;
    cTg_Addition(&g_fpcDtTg_Queue, &i_deleteTag->base);
}

void fpcDtTg_DeleteQTo(delete_tag_class* i_deleteTag) {
    cTg_SingleCut(&i_deleteTag->base);
}

s32 fpcDtTg_Do(delete_tag_class* i_deleteTag, delete_tag_func i_func) {
    if (i_deleteTag->timer <= 0) {
        fpcDtTg_DeleteQTo(i_deleteTag);

        if (i_func(i_deleteTag->base.mpTagData) == 0) {
            fpcDtTg_ToDeleteQ(i_deleteTag);
#if DEBUG
            if (i_deleteTag->unk_0x1c-- <= 0) {
                i_deleteTag->unk_0x1c = 0;
                if (g_fpcDbSv_service[4] != NULL) {
                    g_fpcDbSv_service[4](i_deleteTag->base.mpTagData);
                }
            }
#endif
            return 0;
        } else {
            return 1;
        }
    } else {
        i_deleteTag->timer--;
        return 0;
    }
}

s32 fpcDtTg_Init(delete_tag_class* i_deleteTag, void* i_data) {
    cTg_Create(&i_deleteTag->base, i_data);
    return 1;
}
