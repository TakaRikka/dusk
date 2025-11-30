#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/JKernel/JKRExpHeap.h"
#include "SSystem/SComponent/c_malloc.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_meter2.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_msg_mng.h"
#include "f_op/f_op_scene_mng.h"

s32 fopMsgM_setStageLayer(void* i_process) {
    scene_class* scn = fopScnM_SearchByID(dStage_roomControl_c::getProcID());

    int id = fopScnM_LayerID(scn);
    return fpcM_ChangeLayerID(i_process, id);
}

msg_class* fopMsgM_SearchByID(fpc_ProcID i_id) {
    return (msg_class*)fpcEx_SearchByID(i_id);
}

fopMsg_prm_class* fopMsgM_GetAppend(void* i_msg) {
    return (fopMsg_prm_class*)fpcM_GetAppend(i_msg);
}

void fopMsgM_Delete(void* i_this) {
    fpcM_Delete(i_this);
}

static fopMsg_prm_class* createAppend(fopAc_ac_c* i_talkActor, cXyz* i_pos, u32* i_msgIdx, u32* param_3,
                                      fpc_ProcID param_4) {
    fopMsg_prm_class* append = (fopMsg_prm_class*)cMl::memalignB(-4, sizeof(fopMsg_prm_class));
    if (append == NULL) {
        return NULL;
    }

    append->talk_actor = i_talkActor;
    dMsgObject_setTalkActor(i_talkActor);

    if (i_msgIdx != NULL) {
        append->msg_idx = *i_msgIdx;
    }

    if (param_3 != NULL) {
        append->field_0x14 = *param_3;
    }

    if (i_pos != NULL) {
        append->pos = *i_pos;
    } else {
        append->pos = cXyz(0.0f, 0.0f, 0.0f);
    }

    append->field_0x18 = param_4;

    return append;
}

static fopMsg_prm_timer* createTimerAppend(int i_mode, u32 i_limitMs, u8 i_type, u8 param_3,
                                           f32 param_4, f32 param_5, f32 param_6, f32 param_7,
                                           fpc_ProcID param_8) {
    fopMsg_prm_timer* appen = (fopMsg_prm_timer*)cMl::memalignB(-4, sizeof(fopMsg_prm_timer));
    if (appen == NULL) {
        return NULL;
    }

    appen->talk_actor = NULL;
    appen->msg_idx = 0;
    appen->field_0x14 = 0;
    appen->pos = cXyz(0.0f, 0.0f, 0.0f);
    appen->field_0x18 = param_8;
    appen->timer_mode = i_mode;
    appen->limit_ms = i_limitMs;
    appen->type = i_type;
    appen->field_0x25 = param_3;
    appen->field_0x28 = param_4;
    appen->field_0x2c = param_5;
    appen->field_0x30 = param_6;
    appen->field_0x34 = param_7;
    return appen;
}

fpc_ProcID fopMsgM_create(s16 i_procName, fopAc_ac_c* i_talkActor, cXyz* i_pos, u32* i_msgIdx, u32* param_4,
                   fopMsgCreateFunc createFunc) {
    fopMsg_prm_class* append = createAppend(i_talkActor, i_pos, i_msgIdx, param_4, fpcM_ERROR_PROCESS_ID_e);
    if (append == NULL) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    return fpcSCtRq_Request(fpcLy_CurrentLayer(), i_procName, (stdCreateFunc)createFunc, NULL, append);
}

fpc_ProcID fop_Timer_create(s16 i_procName, u8 i_mode, u32 i_limitMs, u8 i_type, u8 param_4, f32 param_5,
                     f32 param_6, f32 param_7, f32 param_8, fopMsgCreateFunc i_createFunc) {
    fopMsg_prm_timer* append = createTimerAppend(i_mode, i_limitMs, i_type, param_4, param_5,
                                                    param_6, param_7, param_8, fpcM_ERROR_PROCESS_ID_e);
    if (append == NULL) {
        return fpcM_ERROR_PROCESS_ID_e;
    }

    return fpcSCtRq_Request(fpcLy_CurrentLayer(), i_procName, (stdCreateFunc)i_createFunc, NULL,
                            append);
}

static fpc_ProcID i_msgID = fpcM_ERROR_PROCESS_ID_e;

fpc_ProcID fopMsgM_messageSet(u32 i_msgIdx, fopAc_ac_c* i_talkActor, u32 param_2) {
    if (dComIfGp_isHeapLockFlag() == 8) {
        dMeter2Info_getMeterClass()->emphasisButtonDelete();
    }

    if (dComIfGp_isHeapLockFlag() != 0 && dComIfGp_isHeapLockFlag() != 5 &&
        dComIfGp_isHeapLockFlag() != 2 && dComIfGp_isHeapLockFlag() != 3 &&
        dComIfGp_isHeapLockFlag() != 1)
    {
        return fpcM_ERROR_PROCESS_ID_e;

    }

    dComIfGp_clearMesgAnimeTagInfo();
    dComIfGp_clearMesgCameraTagInfo();

    cXyz pos;
    if (i_talkActor != NULL) {
        pos = i_talkActor->eyePos;
    } else {
        pos.set(0.0f, 0.0f, 0.0f);
    }

    if (g_MsgObject_HIO_c.mMsgDebug == true) {
        i_msgIdx = g_MsgObject_HIO_c.mMsgIndex;
    }

    dMsgObject_c* msg = (dMsgObject_c*)fopMsgM_SearchByID(i_msgID);

    if (msg != NULL && msg->mode == fopMsg_MODE_MSG_PREPARING_e) {
        msg->pos.set(pos);
        msg->msg_idx = i_msgIdx;
        msg->field_0xf0 = param_2;
        msg->talk_actor = i_talkActor;
        msg->setMessageIndex(i_msgIdx, param_2, false);
        return i_msgID;
    }

    return 0;
}

fpc_ProcID fopMsgM_messageSet(u32 i_msgIdx, u32 param_1) {
    if (dComIfGp_isHeapLockFlag() == 8) {
        dMeter2Info_getMeterClass()->emphasisButtonDelete();
    }

    if (dComIfGp_isHeapLockFlag() != 0 && dComIfGp_isHeapLockFlag() != 5 &&
        dComIfGp_isHeapLockFlag() != 2 && dComIfGp_isHeapLockFlag() != 3 &&
        dComIfGp_isHeapLockFlag() != 1)
    {
        return fpcM_ERROR_PROCESS_ID_e;

    }

    cXyz pos;
    pos.x = pos.y = pos.z = 0.0f;

    dMsgObject_c* msg = (dMsgObject_c*)fopMsgM_SearchByID(i_msgID);

    if (msg != NULL) {
        if (msg->mode == fopMsg_MODE_MSG_PREPARING_e) {
            msg->pos.set(pos);
            msg->msg_idx = i_msgIdx;
            msg->field_0xf0 = param_1;
            msg->talk_actor = NULL;
            msg->setTalkPartner(NULL);
            msg->setMessageIndex(i_msgIdx, param_1, false);
            return i_msgID;
        } else if (msg->mode == fopMsg_MODE_MSG_CONTINUE_e) {
            msg->pos.set(pos);
            msg->msg_idx = i_msgIdx;
            msg->field_0xf0 = param_1;
            msg->talk_actor = NULL;
            return i_msgID;
        }
    }

    return 0;
}

fpc_ProcID fopMsgM_messageSetDemo(u32 i_msgidx) {
    dMsgObject_endFlowGroup();
    if (dComIfGp_isHeapLockFlag() == 8) {
        dMeter2Info_getMeterClass()->emphasisButtonDelete();
    }

    if (dComIfGp_isHeapLockFlag() != 0 && dComIfGp_isHeapLockFlag() != 5 &&
        dComIfGp_isHeapLockFlag() != 2 && dComIfGp_isHeapLockFlag() != 3 &&
        dComIfGp_isHeapLockFlag() != 1)
    {
        return fpcM_ERROR_PROCESS_ID_e;

    }

    cXyz pos;
    pos.x = pos.y = pos.z = 0.0f;

    dMsgObject_c* msg = (dMsgObject_c*)fopMsgM_SearchByID(i_msgID);

    if (msg != NULL && msg->mode == fopMsg_MODE_MSG_PREPARING_e) {
        msg->pos.set(pos);
        msg->msg_idx = i_msgidx;
        msg->field_0xf0 = 1000;
        msg->talk_actor = NULL;
        msg->setMessageIndexDemo(i_msgidx, false);
        return i_msgID;
    }

    return 0;
}

char* fopMsgM_messageGet(char* i_stringBuf, u32 i_msgId) {
    dMeter2Info_getString(i_msgId, i_stringBuf, NULL);
    return i_stringBuf;
}

void fopMsgM_setMessageID(fpc_ProcID msg_id) {
    i_msgID = msg_id;
}

fpc_ProcID fopMsgM_Create(s16 i_procName, FastCreateReqFunc i_createFunc, void* i_append) {
    return fpcM_Create(i_procName, i_createFunc, i_append);
}

u8 fopMsgM_itemNumIdx(u8 i_no) {
    static const u8 itemicon[] = {
        0x40, 0x48, 0x41, 0x45, 0x43, 0x3e, 0x42, 0xff, 0x46, 0x44, 0x47, 0x60,
        0x61, 0x62, 0x6a, 0x70, 0x71, 0x72, 0x25, 0x90, 0x4a, 0xff, 0xff, 0x4b,
    };

    return itemicon[i_no] & 0xFF;
}

void J2DPane::setAlpha(u8 alpha) {
    mAlpha = alpha;
}

f32 dummy() {
    return 0.5f;
}

f32 fopMsgM_valueIncrease(int param_0, int param_1, u8 i_type) {
    if (param_0 <= 0) {
        return 1.0f;
    }

    if (param_1 < 0) {
        param_1 = 0;
    } else if (param_1 > param_0) {
        param_1 = param_0;
    }

    f32 var_f31 = (f32)param_1 / param_0;
    f32 var_f30;

    switch (i_type) {
    case 0:
        var_f30 = var_f31 * var_f31;
        break;
    case 1:
        var_f30 = JMAFastSqrt(var_f31);
        break;
    case 2:
    default:
        var_f30 = var_f31;
        break;
    case 3:
        var_f30 = (2.0f * ((2.0f * var_f31) - 1.0f)) - 1.0f;
        break;
    case 4:
        var_f31 = cM_ssin(0.5f * ((f32)0x8000 * var_f31));
        var_f30 = var_f31 * var_f31;
        break;
    case 5:
        var_f31 = cM_ssin(0.5f * ((f32)0xFFFF * var_f31));
        var_f30 = var_f31 * var_f31;
        break;
    case 6:
        var_f30 = cM_ssin((f32)0x8000 * var_f31);
        break;
    }

    return var_f30;
}

// Here to generate J2DPicture virtual inlines
static void dummyVirtual(J2DPicture* picture, f32 param_1, f32 param_2, const char* param_3) {
    picture->append(param_3, param_1);
    picture->setBlendRatio(param_1, param_2);
}

JKRExpHeap* fopMsgM_createExpHeap(u32 i_heapSize, JKRHeap* i_heap) {
    if (i_heap == NULL) {
        i_heap = mDoExt_getGameHeap();
    }

    return JKRExpHeap::create(i_heapSize, i_heap, false);
}

void fopMsgM_destroyExpHeap(JKRExpHeap* i_heap) {
    i_heap->destroy();
}
