/**
 * @file d_a_talk.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_talk.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_msg_mng.h"

daTalk_c::~daTalk_c() {
    dMsgObject_getMsgObjectClass()->setProcessID(-1);
}

int daTalk_c::create() {
    fopAcM_ct(this, daTalk_c);

    if (getStatus() != 1) {
        return cPhs_ERROR_e;
    }

    attention_info = dMsgObject_getMsgObjectClass()->getpTalkActor()->attention_info;
    mMessageID = -1;
    return cPhs_COMPLEATE_e;
}

int daTalk_c::execute() {
    if (dComIfGp_event_runCheck()) {
        if (!eventInfo.checkCommandTalk()) {
            fopAcM_delete(this);
        }
    } else {
        if (fopAcM_searchPlayerDistanceXZ(this) >
            dAttention_c::getDistTable(attention_info.distances[fopAc_attn_TALK_e]).mDistMaxRelease)
        {
            mMessageID = -1;
            fopAcM_delete(this);
        } else {
            eventInfo.onCondition(dEvtCnd_CANTALK_e);
        }
    }

    if (eventInfo.checkCommandTalk()) {
        if (dMsgObject_getMsgObjectClass()->getNodeIdx() == 0xFF) {
            if (mMessageID == -1) {
                mMessageID = messageSet();
            } else if (getStatus() == 14) {
                setStatus(16);
            } else if (getStatus() == 18) {
                dComIfGp_event_reset();
                setStatus(19);
                mMessageID = -1;
                fopAcM_delete(this);
            }
        } else if (mMessageID == -1) {
            mMsgFlow.init(this, dMsgObject_getMsgObjectClass()->getNodeIdx(), 0, NULL);
            mMessageID = 2;
        } else if (mMsgFlow.doFlow(dMsgObject_getMsgObjectClass()->getpTalkActor(), NULL, 0)) {
            dComIfGp_event_reset();
            mMessageID = -1;
            fopAcM_delete(this);
        }
    }

    return 1;
}

int daTalk_c::draw() {
    return 1;
}

void daTalk_c::setStatus(u16 i_status) {
    dMsgObject_getMsgObjectClass()->setStatus(i_status);
}

u16 daTalk_c::getStatus() {
    return dMsgObject_getMsgObjectClass()->getStatus();
}

u32 daTalk_c::messageSet() {
    return fopMsgM_messageSet(dMsgObject_getMsgObjectClass()->getIdx(),
                              dMsgObject_getMsgObjectClass()->getpTalkActor(), 1000);
}

static int daTalk_Create(fopAc_ac_c* i_this) {
    return static_cast<daTalk_c*>(i_this)->create();
}

static int daTalk_Delete(daTalk_c* i_this) {
    i_this->~daTalk_c();
    return 1;
}

static int daTalk_Execute(daTalk_c* i_this) {
    return i_this->execute();
}

static int daTalk_Draw(daTalk_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daTalk_Method = {
    (process_method_func)daTalk_Create,  (process_method_func)daTalk_Delete,
    (process_method_func)daTalk_Execute, (process_method_func)NULL,
    (process_method_func)daTalk_Draw,
};

actor_process_profile_definition g_profile_TALK = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_TALK,
    &g_fpcLf_Method.base,
    sizeof(daTalk_c),
    0,
    0,
    &g_fopAc_Method.base,
    766,
    &l_daTalk_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_6_e,
};
