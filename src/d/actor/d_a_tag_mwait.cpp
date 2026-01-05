/**
 * d_a_tag_mwait.cpp
 * Midna Wait Trigger
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_mwait.h"
#include "d/actor/d_a_midna.h"
#include "d/d_procname.h"

int daTagMwait_c::create() {
    fopAcM_ct(this, daTagMwait_c);

    mEnterSw = fopAcM_GetParam(this) >> 8;

    if (fopAcM_isSwitch(this, mEnterSw)) {
        return cPhs_ERROR_e;
    }

    mOnSw = fopAcM_GetParam(this);

    u32 prm = (fopAcM_GetParam(this) >> 0x10) & 0xFF;
    if (prm == 0 || prm == 0xFF) {
        field_0x570 = -1.0f;
    } else {
        field_0x570 = prm * prm * 100.0f;
    }

    mWarpToPos = shape_angle.x & 0xF;
    if (mWarpToPos >= 2) {
        mWarpToPos = 0;
    }

    field_0x56d = (shape_angle.x >> 4) & 0xF;
    if (field_0x56d >= 2) {
        field_0x56d = 0;
    }

    scale *= 100.0f;

    u32 wait_y_offset = (fopAcM_GetParam(this) >> 0x18) & 0xFF;
    if (wait_y_offset == 0 || wait_y_offset == 0xFF) {
        wait_y_offset = 0;
    }

    mWaitPosition.set(current.pos.x, current.pos.y + (wait_y_offset * 10.0f), current.pos.z);
    attention_info.position.set(mWaitPosition.x, mWaitPosition.y + 220.0f, mWaitPosition.z);
    eyePos.set(mWaitPosition.x, mWaitPosition.y + 150.0f, mWaitPosition.z);

    attention_info.distances[fopAc_attn_TALK_e] = 54;
    attention_info.flags = 0;
    return cPhs_COMPLEATE_e;
}

static int daTagMwait_Create(fopAc_ac_c* i_this) {
    return static_cast<daTagMwait_c*>(i_this)->create();
}

daTagMwait_c::~daTagMwait_c() {}

static int daTagMwait_Delete(daTagMwait_c* i_this) {
    i_this->~daTagMwait_c();
    return 1;
}

int daTagMwait_c::execute() {
    daMidna_c* midna_p = daPy_py_c::getMidnaActor();
    daPy_py_c* player_p = daPy_getLinkPlayerActorClass();

    if (midna_p == NULL) {
        return 1;
    }

    attention_info.flags = 0;

    if (eventInfo.checkCommandTalk()) {
        if (!mEnteredTrigger || !midna_p->checkReturnAnime()) {
            if (!mInitMsgFlow) {
                mMsgFlow.init(this, (u16)shape_angle.z, 0, NULL);
                mInitMsgFlow = true;
            } else if (mMsgFlow.doFlow(this, NULL, 0)) {
                dComIfGp_event_reset();

                if (mEnteredTrigger) {
                    fopAcM_delete(this);
                    return 1;
                }
            }
        }
    } else if (mEnteredTrigger) {
        fopAcM_orderSpeakEvent(this, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        attention_info.position = midna_p->attention_info.position;
        eyePos = attention_info.position;
    } else {
        mInitMsgFlow = false;

        if (daPy_py_c::checkNowWolf() && midna_p != NULL && fopAcM_isSwitch(this, mOnSw))
        {
            if (mWarpToPos == 1) {
                midna_p->onTagWaitPosWarp(&mWaitPosition);
            } else {
                midna_p->onTagWaitPos(&mWaitPosition);
            }

            if (midna_p->current.pos.abs(mWaitPosition) < 5.0f) {
                if (field_0x56d == 1) {
                    midna_p->onTagNoHairLead();
                }

                f32 player_dist = player_p->current.pos.abs2XZ(current.pos);
                if ((player_dist <= scale.x * scale.x &&
                     player_p->current.pos.y >= current.pos.y &&
                     player_p->current.pos.y <= current.pos.y + scale.y) ||
                    fopAcM_isSwitch(this, mEnterSw))
                {
                    midna_p->offTagWaitPos();
                    fopAcM_onSwitch(this, mEnterSw);
                    mEnteredTrigger = true;

                    if (shape_angle.z == 0) {
                        fopAcM_delete(this);
                    } else {
                        attention_info.position = midna_p->attention_info.position;
                        eyePos = attention_info.position;
                        fopAcM_orderSpeakEvent(this, 0, 0);
                        eventInfo.onCondition(dEvtCnd_CANTALK_e);
                    }
                } else if (field_0x570 > 0.0f && player_dist < field_0x570 &&
                           player_p->current.pos.y >= current.pos.y &&
                           player_p->current.pos.y <= current.pos.y + scale.y)
                {
                    attention_info.flags = fopAc_AttnFlag_TALK_e;
                    eventInfo.onCondition(dEvtCnd_CANTALK_e);
                }
            }
        }
    }

    return 1;
}

static int daTagMwait_Execute(daTagMwait_c* i_this) {
    return i_this->execute();
}

static int daTagMwait_Draw(daTagMwait_c* i_this) {
    return 1;
}

static actor_method_class l_daTagMwait_Method = {
    (process_method_func)daTagMwait_Create,  (process_method_func)daTagMwait_Delete,
    (process_method_func)daTagMwait_Execute, (process_method_func)NULL,
    (process_method_func)daTagMwait_Draw,
};

actor_process_profile_definition g_profile_Tag_Mwait = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Mwait,
    &g_fpcLf_Method.base,
    sizeof(daTagMwait_c),
    0,
    0,
    &g_fopAc_Method.base,
    256,
    &l_daTagMwait_Method,
    0x44000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
