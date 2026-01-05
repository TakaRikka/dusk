/**
 * d_a_tag_msg.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_msg.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

static int createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daTag_Msg_c*)i_this)->createHeap();
}

static char* l_resName = "TagMsg";

static char* l_evtNameTBL[2] = {
    NULL,
    "SPEAK",
};

daTag_Msg_c::~daTag_Msg_c() {
    dComIfG_resDelete(&mPhase, getResName());
}

int daTag_Msg_c::create() {
    fopAcM_ct(this, daTag_Msg_c);

    int phase_state = dComIfG_resLoad(&mPhase, getResName());
    if (phase_state == cPhs_COMPLEATE_e) {
        attention_info.flags = (fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e);
        getParam();

        eventInfo.setArchiveName(getResName());
        mEventIdx = -1;

        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x10)) {
            return cPhs_ERROR_e;
        }
    }

    return phase_state;
}

int daTag_Msg_c::createHeap() {
    mParam = new daTag_Msg_Param_c();
    if (mParam == NULL) {
        return 0;
    }

    return 1;
}

int daTag_Msg_c::destroy() {
    this->~daTag_Msg_c();
    return 1;
}

const daTag_Msg_HIO_Param_c daTag_Msg_Param_c::m = {0};

int daTag_Msg_c::execute() {
    bool set_event = true;

    fopAcM_SetRoomNo(this, dComIfGp_roomControl_getStayNo());
    eyePos.set(current.pos.x, current.pos.y + 150.0f, current.pos.z);
    attention_info.position = eyePos;

    if (dComIfGp_event_runCheck()) {
        set_event = false;

        if (eventInfo.checkCommandTalk()) {
            if (cut_speak(-1, field_0x5e4)) {
                if (field_0x5dc == 1 && mOffSwitch != 0xFF) {
                    dComIfGs_onSwitch(mOffSwitch, fopAcM_GetRoomNo(this));
                }

                dComIfGp_event_reset();
                set_event = true;
            }

            field_0x5e4 = 0;
        } else if (mEventIdx != -1) {
            if (dComIfGp_getEventManager().endCheck(mEventIdx)) {
                mOrderEvtNum = 0;
                mEventIdx = -1;

                if (field_0x5dc == 0 && mOffSwitch != 0xFF) {
                    dComIfGs_onSwitch(mOffSwitch, fopAcM_GetRoomNo(this));
                }

                dComIfGp_event_reset();
                set_event = true;
            } else {
                int staff_id = dComIfGp_evmng_getMyStaffId(mStaffName, NULL, 0);
                int act_idx = dComIfGp_getEventManager().getMyActIdx(staff_id, mEvtCutTBL, 2, 0, 0);
                int is_advance = dComIfGp_getEventManager().getIsAddvance(staff_id);

                BOOL var_r27 = false;
                switch (act_idx) {
                case 0:
                    var_r27 = cut_speak(staff_id, is_advance);
                    break;
                }

                if (var_r27) {
                    dComIfGp_getEventManager().cutEnd(staff_id);
                }
            }
        }
    }

    if (set_event && rangeCheck() && otherCheck()) {
        if (field_0x5dd) {
            mOrderEvtNum = 1;
            JUT_ASSERT(0, NULL != l_evtNameTBL[mOrderEvtNum]);

            mEventIdx =
                dComIfGp_getEventManager().getEventIdx(this, l_evtNameTBL[mOrderEvtNum], 0xFF);
            fopAcM_orderOtherEventId(this, mEventIdx, 0xFF, 0xFFFF, 0, 1);
        } else {
            eventInfo.onCondition(1);
            field_0x5e4 = 1;
        }
    }

    return 1;
}

int daTag_Msg_c::draw() {
    return 1;
}

BOOL daTag_Msg_c::rangeCheck() {
    cXyz player_dist = daPy_getPlayerActorClass()->current.pos - current.pos;
    return player_dist.absXZ() < scale.x && (-scale.y < player_dist.y && player_dist.y < scale.y);
}

BOOL daTag_Msg_c::otherCheck() {
    if (field_0x5dc) {
        if (mOnSaveLabel == 0xFFF ||
            dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mOnSaveLabel]))
        {
            if (mOffSaveLabel != 0xFFF &&
                dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[mOffSaveLabel]))
            {
                return false;
            }
        } else {
            return false;
        }
    } else if (mOnSwitch == 0xFF || dComIfGs_isSwitch(mOnSwitch, fopAcM_GetRoomNo(this))) {
        if (mOffSwitch != 0xFF && dComIfGs_isSwitch(mOffSwitch, fopAcM_GetRoomNo(this))) {
            return false;
        }
    } else {
        return false;
    }

    if (field_0x5dd) {
        return 1;
    }

    s16 var_r28 = fopAcM_searchActorAngleY(this, daPy_getPlayerActorClass()) + 0x7FFF;
    s16 angle_to_player = var_r28 - daPy_getPlayerActorClass()->current.angle.y;
    if (angle_to_player < 0) {
        angle_to_player = -angle_to_player;
    }

    return angle_to_player <= 0x1000;
}

char* daTag_Msg_c::getResName() {
    return l_resName;
}

void daTag_Msg_c::getParam() {
    mOnSwitch = home.angle.x & 0xFF;
    mOffSwitch = (home.angle.x >> 8) & 0xFF;
    mOnSaveLabel = fopAcM_GetParam(this) & 0xFFF;
    mOffSaveLabel = (fopAcM_GetParam(this) >> 0xC) & 0xFFF;
    field_0x5dc = (fopAcM_GetParam(this) & 0x1000000) != 0;
    field_0x5dd = (fopAcM_GetParam(this) & 0x2000000) != 0;

    if ((home.angle.z & 0xFFFF) != 0) {
        mFlowID = home.angle.z & 0xFFFF;
    } else {
        mFlowID = -1;
    }

    scale.x *= 100.0f;
    scale.y *= 100.0f;
    strcpy(mStaffName, "Tag_ms");
}

char* daTag_Msg_c::mEvtCutTBL[2] = {
    "SPEAK",
    "PAUSE",
};

BOOL daTag_Msg_c::cut_speak(int i_staffId, BOOL i_isAdvance) {
    if (i_isAdvance) {
        mMsgFlow.init(this, mFlowID, 0, NULL);
        return false;
    } else {
        return mMsgFlow.doFlow(this, NULL, 0);
    }
}

static int daTag_Msg_Create(void* i_this) {
    return ((daTag_Msg_c*)i_this)->create();
}

static int daTag_Msg_Delete(void* i_this) {
    return ((daTag_Msg_c*)i_this)->destroy();
}

static int daTag_Msg_Execute(void* i_this) {
    return ((daTag_Msg_c*)i_this)->execute();
}

static int daTag_Msg_Draw(void* i_this) {
    return ((daTag_Msg_c*)i_this)->draw();
}

static int daTag_Msg_IsDelete(void* i_this) {
    return 1;
}

void dummyString() {
    DEAD_STRING("Timer");
}

static actor_method_class daTag_Msg_MethodTable = {
    (process_method_func)daTag_Msg_Create,  (process_method_func)daTag_Msg_Delete,
    (process_method_func)daTag_Msg_Execute, (process_method_func)daTag_Msg_IsDelete,
    (process_method_func)daTag_Msg_Draw,
};

actor_process_profile_definition g_profile_TAG_MSG = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_TAG_MSG,            // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daTag_Msg_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    285,                     // mPriority
    &daTag_Msg_MethodTable,  // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
