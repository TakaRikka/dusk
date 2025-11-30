/**
 * d_a_tag_lantern.cpp
 * Tag - Lantern
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_lantern.h"
#include "d/d_procname.h"

daTag_Lantern_c::~daTag_Lantern_c() {}

int daTag_Lantern_c::create() {
    fopAcM_ct(this, daTag_Lantern_c);
    initialize();
    return cPhs_COMPLEATE_e;
}

int daTag_Lantern_c::Delete() {
    this->~daTag_Lantern_c();
    return 1;
}

int daTag_Lantern_c::Execute() {
    dComIfG_play_c& play = g_dComIfG_gameInfo.getPlay();  // fake match

    if (dComIfGp_event_runCheck()) {
        if (eventInfo.checkCommandTalk()) {
            if (!field_0x604) {
                mMsgFlow.init(this, mFlowIndex, 0, NULL);
                field_0x604++;
            } else if (field_0x604 == 1 && mMsgFlow.doFlow(this, NULL, 0)) {
                play.getEvent().reset();
                field_0x604 = 0;
                field_0x5f8 = 0;
            }
        }
    } else if (field_0x5f8) {
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
        fopAcM_orderSpeakEvent(this, 0, 0);
    }

    mGndChk.SetPos(&current.pos);
    mGroundCross = dComIfG_Bgsp().GroundCross(&mGndChk);

    s32 room_no;
    if (mGroundCross != -G_CM3D_F_INF) {
        room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    } else {
        room_no = dComIfGp_roomControl_getStayNo();
    }

    tevStr.room_no = room_no;
    fopAcM_SetRoomNo(this, tevStr.room_no);
    attention_info.flags = 0;
    attention_info.position = current.pos;
    eyePos = attention_info.position;
    return 1;
}

int daTag_Lantern_c::Draw() {
    return 1;
}

void daTag_Lantern_c::initialize() {
    fopAcM_setCullSizeBox(this, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    if (home.angle.z != 0xFFFF) {
        mFlowIndex = home.angle.z;
    } else {
        mFlowIndex = -1;
    }

    home.angle.z = 0;
    field_0x5fc = fopAcM_GetParam(this) & 0xFFF;

    scale.x *= 100.0f;
    scale.y *= 100.0f;
    scale.z *= 100.0f;

    Execute();
}

static int daTag_Lantern_Create(void* i_this) {
    return static_cast<daTag_Lantern_c*>(i_this)->create();
}

static int daTag_Lantern_Delete(void* i_this) {
    return static_cast<daTag_Lantern_c*>(i_this)->Delete();
}

static int daTag_Lantern_Execute(void* i_this) {
    return static_cast<daTag_Lantern_c*>(i_this)->Execute();
}

static int daTag_Lantern_Draw(void* i_this) {
    return static_cast<daTag_Lantern_c*>(i_this)->Draw();
}

static int daTag_Lantern_IsDelete(void* i_this) {
    return 1;
}

static daTag_Lantern_Param_c l_HIO;

static actor_method_class daTag_Lantern_MethodTable = {
    (process_method_func)daTag_Lantern_Create,  (process_method_func)daTag_Lantern_Delete,
    (process_method_func)daTag_Lantern_Execute, (process_method_func)daTag_Lantern_IsDelete,
    (process_method_func)daTag_Lantern_Draw,
};

extern actor_process_profile_definition g_profile_TAG_LANTERN = {
    fpcLy_CURRENT_e,             // mLayerID
    7,                           // mListID
    fpcPi_CURRENT_e,             // mListPrio
    PROC_TAG_LANTERN,            // mProcName
    &g_fpcLf_Method.base,       // sub_method
    sizeof(daTag_Lantern_c),     // mSize
    0,                           // mSizeOther
    0,                           // mParameters
    &g_fopAc_Method.base,        // sub_method
    286,                         // mPriority
    &daTag_Lantern_MethodTable,  // sub_method
    0x40000,                     // mStatus
    fopAc_ACTOR_e,               // mActorType
    fopAc_CULLBOX_CUSTOM_e,      // cullType
};
