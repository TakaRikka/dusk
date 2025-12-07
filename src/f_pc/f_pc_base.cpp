/**
 * f_pc_base.cpp
 * Framework - Process Base
 */

#include "f_pc/f_pc_base.h"
#include "SSystem/SComponent/c_malloc.h"
#include "SSystem/SComponent/c_phase.h"
#include "SSystem/SStandard/s_basic.h"
#include "d/d_stage.h"
#include "f_pc/f_pc_layer.h"
#include "f_pc/f_pc_method.h"
#include "f_pc/f_pc_pause.h"
#include "f_pc/f_pc_profile.h"
#include "f_pc/f_pc_debug_sv.h"

#if VERSION == VERSION_SHIELD_DEBUG
#include "Z2AudioLib/Z2AudioMgr.h"
#endif

BOOL fpcBs_Is_JustOfType(int i_typeA, int i_typeB) {
    if (i_typeB == i_typeA) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int g_fpcBs_type;

int fpcBs_MakeOfType(int* i_type) {
    static int t_type = 0x9130000;
    if (*i_type == 0) {
        *i_type = ++t_type;
    }
    return *i_type;
}

int fpcBs_MakeOfId() {
    static int process_id = 1;
    return process_id++;
}

int fpcBs_Execute(base_process_class* i_proc) {
    int result = 1;

#if DEBUG
    if (!fpcBs_Is_JustOfType(g_fpcBs_type, i_proc->type)) {
        if (g_fpcDbSv_service[10] != NULL) {
            g_fpcDbSv_service[10](i_proc);
        }
        
        return 0;
    }
#endif

    if (result == 1) {
        layer_class* save_layer = fpcLy_CurrentLayer();

        fpcLy_SetCurrentLayer(i_proc->layer_tag.layer);
        result = fpcMtd_Execute(i_proc->methods, i_proc);

        fpcLy_SetCurrentLayer(save_layer);
    }
    
    return result;
}

void fpcBs_DeleteAppend(base_process_class* i_proc) {
    if (i_proc->append != NULL) {
        cMl::free(i_proc->append);
        i_proc->append = NULL;
    }
}

int fpcBs_IsDelete(base_process_class* i_proc) {
    int result;
    layer_class* save_layer = fpcLy_CurrentLayer();

    fpcLy_SetCurrentLayer(i_proc->layer_tag.layer);
    result = fpcMtd_IsDelete(i_proc->methods, i_proc);

    fpcLy_SetCurrentLayer(save_layer);
    return result;
}

int fpcBs_Delete(base_process_class* i_proc) {
    BOOL result = TRUE;
    if (result == TRUE) {
        result = fpcMtd_Delete(i_proc->methods, i_proc);
        if (result == 1) {
            s16 profname = i_proc->profname;
            fpcBs_DeleteAppend(i_proc);
            i_proc->type = 0;
            cMl::free(i_proc);

            #if VERSION == VERSION_SHIELD_DEBUG
            // JSUList<Z2SoundObjBase>* allList = Z2GetAudioMgr()->getAllList();
            
            // for (JSUListIterator<Z2SoundObjBase> it(allList->getFirst()); it != allList->getEnd(); it++) {
            //     static JSULink<Z2SoundObjBase>* DUMMY_FILL_IT = NULL;
            //     static Z2SoundObjBase* DUMMY_FILL_P = NULL;
            //     if (it == DUMMY_FILL_IT || it.getObject() == DUMMY_FILL_P) {
            //         const char* stageName = dStage_getName2(profname, 0);
            //         if (stageName == NULL) {
            //             JUT_PANIC_F(341, "Sound Object Not Delete !! <%d>\n", profname);
            //         } else {
            //             JUT_PANIC_F(345, "Sound Object Not Delete !! <%s>\n", stageName);
            //         }
            //     }
            // }
            #endif
        }
    }
    return result;
}

base_process_class* fpcBs_Create(s16 i_profname, fpc_ProcID i_procID, void* i_append) {
    process_profile_definition* pprofile;
    base_process_class* pprocess;
    u32 size;

    pprofile = (process_profile_definition*)fpcPf_Get(i_profname);
    size = pprofile->process_size + pprofile->unk_size;

    pprocess = (base_process_class*)cMl::memalignB(-4, size);
    if (pprocess == NULL) {
        return NULL;
    }

    sBs_ClearArea(pprocess, size);
    fpcLyTg_Init(&pprocess->layer_tag, pprofile->layer_id, pprocess);
    fpcLnTg_Init(&pprocess->line_tag_, pprocess);
    fpcDtTg_Init(&pprocess->delete_tag, pprocess);
    fpcPi_Init(&pprocess->priority, pprocess, pprofile->layer_id, pprofile->list_id,
                pprofile->list_priority);

    pprocess->state.init_state = 0;
    pprocess->unk_0xA = 0;
    pprocess->id = i_procID;
    pprocess->profname = i_profname;
    pprocess->type = fpcBs_MakeOfType(&g_fpcBs_type);
    pprocess->name = pprofile->name;
    fpcPause_Init(pprocess);
    pprocess->methods = pprofile->methods;
    pprocess->profile = pprofile;
    pprocess->append = i_append;
    pprocess->parameters = pprofile->parameters;
    return pprocess;
}

int fpcBs_SubCreate(base_process_class* i_proc) {
    switch (fpcMtd_Create(i_proc->methods, i_proc)) {
    case cPhs_NEXT_e:
    case cPhs_COMPLEATE_e:
        fpcBs_DeleteAppend(i_proc);
        i_proc->state.create_phase = cPhs_NEXT_e;
        return cPhs_NEXT_e;
    case cPhs_INIT_e:
case cPhs_LOADING_e:
        i_proc->state.init_state = 1;
        i_proc->state.create_phase = cPhs_INIT_e;
        return cPhs_INIT_e;
    case cPhs_UNK3_e:
        i_proc->state.create_phase = cPhs_UNK3_e;
        return cPhs_UNK3_e;
    case cPhs_ERROR_e:
    default:
        i_proc->state.create_phase = cPhs_ERROR_e;
        return cPhs_ERROR_e;
    }
}
