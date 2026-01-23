/**
 * d_a_tag_bottle_item.cpp
 * 
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_bottle_item.h"
#include "d/d_item.h"
#include "d/d_procname.h"
#include "d/d_s_play.h"

int daTag_BottleItem_c::create() {
    fopAcM_ct(this, daTag_BottleItem_c);
    setTypeFromParam();

    if (!isBottleItem(mBottleItemType)) {
        return cPhs_ERROR_e;
    }

    initialize();
    return cPhs_COMPLEATE_e;
}


int daTag_BottleItem_c::Delete() {
    return 1;
}

int daTag_BottleItem_c::Execute() {
    int eventResult = chkEvent();
    if (eventResult) {
        if (mProcessFunc) {
            (this->*mProcessFunc)(0);
        }
        orderEvent();
    }

    setAttnPos();
    return 1;
}

int daTag_BottleItem_c::Draw() {
    return 1;
}

void daTag_BottleItem_c::setTypeFromParam() {
    mBottleItemType = fopAcM_GetParam(this);
    makeSoup();
}

void daTag_BottleItem_c::restart() {
    current.angle.set(0,home.angle.y,0);
    shape_angle = current.angle;
    setProcess(&daTag_BottleItem_c::wait);
}

void daTag_BottleItem_c::initialize() {
    fopAcM_setCullSizeBox(this,-30.0f,-15.0f,-30.0f,30.0f,45.0f,30.0f);
    attention_info.flags = 0;
    attention_info.distances[fopAc_attn_CARRY_e] = 6;
    fopAcM_OnCarryType(this,fopAcM_CARRY_SIDE);
    restart();
    Execute();
}

int daTag_BottleItem_c::checkProcess(ProcessFunc i_processFunc) {
    return mProcessFunc == i_processFunc;
}

int daTag_BottleItem_c::setProcess(ProcessFunc i_processFunc) {
    int retval = 0;

    if (checkProcess(i_processFunc)) {
        return retval;
    } else {
        mEventType = 2;

        if (mProcessFunc) {
            retval = (this->*mProcessFunc)(0);
        }

        mEventType = 0;
        mProcessFunc = i_processFunc;

        if (mProcessFunc) {
            retval = (this->*mProcessFunc)(0);
        }
        mEventType = 1;
    }
    return retval;
}

void daTag_BottleItem_c::setAttnPos() {
    attention_info.position = current.pos;
    eyePos = current.pos;
}

int daTag_BottleItem_c::chkEvent() {
    int uVar2 = 1;
  
    if (!dComIfGp_getEvent()->isOrderOK()) {
        uVar2 = 0;
        
        if (eventInfo.checkCommandCatch()) {
            return uVar2;
        }
    }
    return uVar2;
}

int daTag_BottleItem_c::orderEvent() {
    makeSoup();

    if (!daPy_py_c::checkNowWolf() && mBottleItemType != fpcNm_ITEM_EMPTY_BOTTLE) {
        attention_info.flags = (fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e);
    } else {
        attention_info.flags = 0;
    }

    if (attention_info.flags == (fopAc_AttnFlag_TALKCHECK_e | fopAc_AttnFlag_SPEAK_e)) {
        attention_info.distances[fopAc_attn_TALK_e] = fpcNm_ITEM_EMPTY_BOTTLE;
        attention_info.distances[fopAc_attn_SPEAK_e] = fpcNm_ITEM_EMPTY_BOTTLE;
        eventInfo.onCondition(dEvtCnd_CANTALK_e);
    }

    return 1;
}

void daTag_BottleItem_c::makeSoup() {
    if (mBottleItemType == fpcNm_ITEM_LV1_SOUP
           /* dSv_event_flag_c::F_0003 - Snowpeak Ruins - Handed over tomato puree and left room */
        && dComIfGs_isEventBit(2)) {
        mBottleItemType = fpcNm_ITEM_LV2_SOUP;
    }

    if (mBottleItemType == fpcNm_ITEM_LV2_SOUP
           /* dSv_event_flag_c::F_0004 - Snowpeak Ruins - Handed over secret ingredient and left room */
        && dComIfGs_isEventBit(1)) {
        mBottleItemType = fpcNm_ITEM_LV3_SOUP;
    }
}

int daTag_BottleItem_c::wait(void* param_0) {
    switch(mEventType) {
    case 1:
        if (eventInfo.checkCommandCatch() == 0){
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            cXyz pos = attention_info.position - player->attention_info.position;

            dComIfGp_att_CatchRequest(this, mBottleItemType, 140.0f + YREG_F(4), pos.y + 100.0f,
                                      pos.y - 100.0f, 0x2000 + YREG_S(5), 1);
            eventInfo.onCondition(dEvtCnd_40_e);
        }
    case 2:
    case 0:
    default:
        break;
    }
    return 1;
}

static int daTag_BottleItem_Create(void* i_this) {
    return static_cast<daTag_BottleItem_c*>(i_this)->create();
}

static int daTag_BottleItem_Delete(void* i_this) {
    return static_cast<daTag_BottleItem_c*>(i_this)->Delete();
}

static int daTag_BottleItem_Execute(void* i_this) {
    return static_cast<daTag_BottleItem_c*>(i_this)->Execute();
}

static int daTag_BottleItem_Draw(void* i_this) {
    return static_cast<daTag_BottleItem_c*>(i_this)->Draw();
}

static int daTag_BottleItem_IsDelete(void* i_this) {
    return 1;
}

daTag_BottleItem_c::~daTag_BottleItem_c() {}

static actor_method_class daTag_BottleItem_MethodTable = {
    (process_method_func)daTag_BottleItem_Create,
    (process_method_func)daTag_BottleItem_Delete,
    (process_method_func)daTag_BottleItem_Execute,
    (process_method_func)daTag_BottleItem_IsDelete,
    (process_method_func)daTag_BottleItem_Draw
};

actor_process_profile_definition g_profile_TAG_BTLITM = {
    fpcLy_CURRENT_e,                // mLayerID
    7,                              // mListID
    fpcPi_CURRENT_e,                // mListPri
    PROC_TAG_SSDRINK,               // mProcName
    &g_fpcLf_Method.base,          // sub_method
    sizeof(daTag_BottleItem_c),     // mSize
    0,                              // mSizeOther
    0,                              // mParameters
    &g_fopAc_Method.base,           // sub_method
    70,                             // mPriority
    &daTag_BottleItem_MethodTable,  // sub_method
    0x64100,                        // mStatus
    5,                              // mActorType
    fopAc_CULLBOX_CUSTOM_e,         // cullType
};
