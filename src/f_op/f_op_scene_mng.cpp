/**
 * f_op_scene_mng.cpp
 * Framework - Scene Process Manager
 */

#include "f_op/f_op_scene_mng.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "f_op/f_op_scene_iter.h"
#include "f_op/f_op_scene_req.h"

scene_class* fopScnM_SearchByID(fpc_ProcID id) {
    return (scene_class*)fopScnIt_Judge((fop_ScnItFunc)fpcSch_JudgeByID, &id);
}

static fpc_ProcID l_scnRqID = fpcM_ERROR_PROCESS_ID_e;

int fopScnM_ChangeReq(scene_class* i_scene, s16 i_procName, s16 param_3, u16 param_4) {
    fpc_ProcID request_id = fopScnRq_Request(2, i_scene, i_procName, NULL, param_3, param_4);
    if (request_id == fpcM_ERROR_PROCESS_ID_e) {
        return 0;
    }

    l_scnRqID = request_id;
    return 1;
}

fpc_ProcID fopScnM_DeleteReq(scene_class* i_scene) {
    return fopScnRq_Request(1, i_scene, 0x7FFF, NULL, 0x7FFF, 0) != fpcM_ERROR_PROCESS_ID_e;
}

int fopScnM_CreateReq(s16 i_procName, s16 param_2, u16 param_3, u32 i_data) {
    return fopScnRq_Request(0, 0, i_procName, (void*)i_data, param_2, param_3) != fpcM_ERROR_PROCESS_ID_e;
}

u32 fopScnM_ReRequest(s16 i_procName, u32 i_data) {
    if (l_scnRqID == fpcM_ERROR_PROCESS_ID_e) {
        return 0;
    }

    return fopScnRq_ReRequest(l_scnRqID, i_procName, (void*)i_data);
}

void fopScnM_Management() {
#if DEBUG
    if (fopScnRq_Handler()) {
        return;
    };
    JUT_ASSERT(326, 0);
#else
    fopScnRq_Handler();
#endif
}

void fopScnM_Init() {}
