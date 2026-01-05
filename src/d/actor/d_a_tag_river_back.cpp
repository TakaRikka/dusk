/**
 * d_a_tag_river_back.cpp
 * Tag - River Back
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_river_back.h"
#include "d/d_procname.h"

static char* l_arcName = "RvBack";

static char* l_evName = "RIVER_BACK";

int daTagRiverBack_c::Create() {
    mMapToolID = -1;
    eventInfo.setArchiveName(l_arcName);
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_evName, -1);
    return 1;
}

int daTagRiverBack_c::create() {
    fopAcM_ct(this, daTagRiverBack_c);
    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        int result = Create();
        if (!result) {
            return cPhs_ERROR_e;
        }
    }
    return phase;
}

int daTagRiverBack_c::execute() {
    u8 swBit = getSwBit();
    if (swBit != 0xFF && fopAcM_isSwitch(this, swBit) != 0) {
        return 1;
    }

    u8 swBit2 = getSwBit2();
    if (swBit2 != 0xFF && fopAcM_isSwitch(this, swBit2) == 0) {
        return 1;
    }

    event_proc_call();

    return 1;
}

static char* l_staffName = "rvback";

void daTagRiverBack_c::event_proc_call() {
    typedef void (daTagRiverBack_c::*actionFunc)();
    static actionFunc l_func[] = {&daTagRiverBack_c::actionWait,
                                  &daTagRiverBack_c::actionOrderEvent,
                                  &daTagRiverBack_c::actionEvent, &daTagRiverBack_c::actionDead};

    (this->*l_func[mAction])();
}

void daTagRiverBack_c::actionWait() {
    if (dComIfGp_checkPlayerStatus0(0, 0x100000) != 0) {
        mCount = 0;
        setAction(ACTION_ORDER_EVENT_e);
        fopAcM_orderOtherEventId(this, mEventID, mMapToolID, 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daTagRiverBack_c::actionOrderEvent() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_EVENT_e);
        mStaffID = dComIfGp_evmng_getMyStaffId(l_staffName, NULL, 0);
        player->mNoResetFlg0 |= 0x10000;
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mEventID, mMapToolID, 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daTagRiverBack_c::actionEvent() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    mCount += 1;
    if (dComIfGp_evmng_endCheck(mEventID)) {
        setAction(ACTION_DEAD_e);
        dComIfGp_event_reset();
    } else {
        demoProc();
        if (mCount == 30) {
            player->voiceStart(Z2SE_WL_V_FALL_TO_RESTART);
        }
    }
}

void daTagRiverBack_c::actionDead() {}

static char* action_table[] = {"WAIT", "SCENE_CHG"};

int daTagRiverBack_c::demoProc() {
    int act_id = dComIfGp_evmng_getMyActIdx(mStaffID, action_table, ARRAY_SIZEU(action_table), 0, 0);

    if (dComIfGp_evmng_getIsAddvance(mStaffID)) {
        switch (act_id) {
        case WAIT_e: {
            int* timer_p = dComIfGp_evmng_getMyIntegerP(mStaffID, "Timer");
            if (timer_p == NULL) {
                mTimer = 1;
            } else {
                mTimer = *timer_p;
            }
            break;
        }
        case SCENE_CHG_e:
            mDoAud_seStart(Z2SE_FORCE_BACK, 0, 0, 0);
        }
    }
    switch (act_id) {
    case WAIT_e:
        if ((cLib_calcTimer(&mTimer) & 0xFF) == 0) {
            dComIfGp_evmng_cutEnd(mStaffID);
        }
        break;
    case SCENE_CHG_e:
        dStage_changeScene(fopAcM_GetParamBit(this, 0, 8) & 0xFF, 0.0f, 0, fopAcM_GetRoomNo(this),
                           0, -1);
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffID);
    }

    return 0;
}

int daTagRiverBack_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daTagRiverBack_Execute(daTagRiverBack_c* i_this) {
    return i_this->execute();
}

static int daTagRiverBack_Delete(daTagRiverBack_c* i_this) {
    return i_this->_delete();
}

static int daTagRiverBack_Create(daTagRiverBack_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagRiverBack_Method = {(process_method_func)daTagRiverBack_Create,
                                                     (process_method_func)daTagRiverBack_Delete,
                                                     (process_method_func)daTagRiverBack_Execute};

actor_process_profile_definition g_profile_Tag_RiverBack = {
    fpcLy_CURRENT_e,           // mLayerID
    7,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_Tag_RiverBack,        // mProcName
    &g_fpcLf_Method.base,     // sub_method
    sizeof(daTagRiverBack_c),  // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    556,                       // mPriority
    &l_daTagRiverBack_Method,  // sub_method
    0x40000,                   // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
