/**
 * d_a_scene_exit2.cpp
 * Grotto Scene Exit trigger
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_scene_exit2.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

void daScExit_c::initBaseMtx() {
    setBaseMtx();
}

void daScExit_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
}

static char* l_arcName = "SceneExit";

static char* l_evName = "SCENE_EXIT";

int daScExit_c::Create() {
    mRadius = scale.x * 100.0f;
    initBaseMtx();

    mToolID = 0xFF;
    eventInfo.setArchiveName(l_arcName);
    mEventID = dComIfGp_getEventManager().getEventIdx(this, l_evName, 0xFF);
    return 1;
}

int daScExit_c::create() {
    fopAcM_ct(this, daScExit_c);

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase;
}

int daScExit_c::execute() {
    event_proc_call();
    return 1;
}

void daScExit_c::event_proc_call() {
    typedef void (daScExit_c::*actionFunc)();
    static actionFunc l_func[] = {&daScExit_c::actionWait, &daScExit_c::actionOrderEvent,
                                  &daScExit_c::actionEvent, &daScExit_c::actionDead};

    (this->*l_func[mAction])();
}

void daScExit_c::actionWait() {
    if (checkArea()) {
        setAction(ACTION_ORDER_EVENT_e);
        fopAcM_orderOtherEventId(this, mEventID, mToolID, 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daScExit_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_EVENT_e);
        demoProc();
        dComIfGp_setPlayerStatus1(0, 0x4000000);
    } else {
        fopAcM_orderOtherEventId(this, mEventID, mToolID, 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daScExit_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventID)) {
        setAction(ACTION_DEAD_e);
        dComIfGp_event_reset();
    } else {
        demoProc();
        dComIfGp_setPlayerStatus1(0, 0x4000000);
    }
}

void daScExit_c::actionDead() {}

int daScExit_c::demoProc() {
    static char* action_table[3] = {"WAIT", "START", "SCENE_CHG"};

    u8 scene_id = getSceneID();
    int act_id = dComIfGp_evmng_getMyActIdx(mStaffID, action_table, 3, 0, 0);

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
        case START_e:
            fopAcM_seStart(this, Z2SE_SY_WARP_FADE, 0);
            attention_info.position.y = current.pos.y + 800.0f;
            eyePos.y = attention_info.position.y;
            break;
        case SCENE_CHG_e:
            break;
        }
    }

    switch (act_id) {
    case WAIT_e:
        if (cLib_calcTimer(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffID);
        }
        break;
    case START_e:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    case SCENE_CHG_e:
        dStage_changeScene(scene_id, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    }

    return 0;
}

BOOL daScExit_c::checkArea() {
    return fopAcM_searchPlayerDistanceXZ(this) < mRadius;
}

int daScExit_c::draw() {
    return 1;
}

int daScExit_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daScExit_Draw(daScExit_c* i_this) {
    return i_this->draw();
}

static int daScExit_Execute(daScExit_c* i_this) {
    return i_this->execute();
}

static int daScExit_Delete(daScExit_c* i_this) {
    return i_this->_delete();
}

static int daScExit_Create(daScExit_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daScExit_Method = {
    (process_method_func)daScExit_Create,  (process_method_func)daScExit_Delete,
    (process_method_func)daScExit_Execute, (process_method_func)NULL,
    (process_method_func)daScExit_Draw,
};

actor_process_profile_definition g_profile_SCENE_EXIT2 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_SCENE_EXIT2,
    &g_fpcLf_Method.base,
    sizeof(daScExit_c),
    0,
    0,
    &g_fopAc_Method.base,
    583,
    &l_daScExit_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
