/**
 * f_pc_manager.cpp
 * Framework - Process Manager
 */

#include "f_pc/f_pc_manager.h"
#include "SSystem/SComponent/c_API_graphic.h"
#include "d/d_error_msg.h"
#include "d/d_lib.h"
#include "f_pc/f_pc_creator.h"
#include "f_pc/f_pc_deletor.h"
#include "f_pc/f_pc_draw.h"
#include "f_pc/f_pc_fstcreate_req.h"
#include "f_pc/f_pc_line.h"
#include "f_pc/f_pc_pause.h"
#include "f_pc/f_pc_priority.h"
#include "f_pc/f_pc_profile.h"
#include "m_Do/m_Do_controller_pad.h"

void fpcM_Draw(void* i_proc) {
    fpcDw_Execute((base_process_class*)i_proc);
}

s32 fpcM_DrawIterater(fpcM_DrawIteraterFunc i_drawIterFunc) {
    return fpcLyIt_OnlyHere(fpcLy_RootLayer(), (fpcLyIt_OnlyHereFunc)i_drawIterFunc, NULL);
}

s32 fpcM_Execute(void* i_proc) {
    return fpcEx_Execute((base_process_class*)i_proc);
}

s32 fpcM_Delete(void* i_proc) {
    return fpcDt_Delete((base_process_class*)i_proc);
}

BOOL fpcM_IsCreating(fpc_ProcID i_id) {
    return fpcCt_IsCreatingByID(i_id);
}

void fpcM_Management(fpcM_ManagementFunc i_preExecuteFn, fpcM_ManagementFunc i_postExecuteFn) {
    MtxInit();
    dComIfGd_peekZdata();

    if (!dShutdownErrorMsg_c::execute()) {
        static bool l_dvdError = false;

        if (!dDvdErrorMsg_c::execute()) {
            if (l_dvdError) {
                dLib_time_c::startTime();
                Z2GetSoundMgr()->pauseAllGameSound(false);
                l_dvdError = false;
            }

            cAPIGph_Painter();

            if (!dPa_control_c::isStatus(1)) {
                fpcDt_Handler();
            } else {
                dPa_control_c::offStatus(1);
            }

            if (!fpcPi_Handler()) {
                JUT_ASSERT(353, FALSE);
            }

            if (!fpcCt_Handler()) {
                JUT_ASSERT(357, FALSE);
            }

            if (i_preExecuteFn != NULL) {
                i_preExecuteFn();
            }

            fpcEx_Handler((fpcLnIt_QueueFunc)fpcM_Execute);
            fpcDw_Handler((fpcDw_HandlerFuncFunc)fpcM_DrawIterater, (fpcDw_HandlerFunc)fpcM_Draw);

            if (i_postExecuteFn != NULL) {
                i_postExecuteFn();
            }

            dComIfGp_drawSimpleModel();
        } else if (!l_dvdError) {
            dLib_time_c::stopTime();
            Z2GetSoundMgr()->pauseAllGameSound(true);
            mDoCPd_c::stopMotorWaveHard(0);
            l_dvdError = true;
        }
    }
}

void fpcM_Init() {
    static layer_class rootlayer;
    static node_list_class queue[10];

    fpcLy_Create(&rootlayer, NULL, queue, 10);
    fpcLn_Create();
}

base_process_class* fpcM_FastCreate(s16 i_procname, FastCreateReqFunc i_createReqFunc,
                                    void* i_createData, void* i_append) {
    return fpcFCtRq_Request(fpcLy_CurrentLayer(), i_procname, (fstCreateFunc)i_createReqFunc,
                            i_createData, i_append);
}

s32 fpcM_IsPause(void* i_proc, u8 i_flag) {
    return fpcPause_IsEnable((base_process_class*)i_proc, i_flag & 0xFF);
}

void fpcM_PauseEnable(void* i_proc, u8 i_flag) {
    fpcPause_Enable((process_node_class*)i_proc, i_flag & 0xFF);
}

void fpcM_PauseDisable(void* i_proc, u8 i_flag) {
    fpcPause_Disable((process_node_class*)i_proc, i_flag & 0xFF);
}

void* fpcM_JudgeInLayer(fpc_ProcID i_layerID, fpcCtIt_JudgeFunc i_judgeFunc, void* i_data) {
    layer_class* layer = fpcLy_Layer(i_layerID);
    if (layer != NULL) {
        void* ret = fpcCtIt_JudgeInLayer(i_layerID, i_judgeFunc, i_data);
        if (ret == NULL) {
            return fpcLyIt_Judge(layer, i_judgeFunc, i_data);
        }
        return ret;
    }

    return NULL;
}
