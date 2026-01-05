/**
 * d_a_startAndGoal.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_startAndGoal.h"
#include "d/d_timer.h"
#include "d/d_meter2_info.h"
#include "f_op/f_op_actor_mng.h"

BOOL daStartAndGoal_Path_c::chkPassed2(cXyz i_checkPos) {
    int var_r31 = 0;
    while (var_r31 < 1) {
        if (!chkPassed(i_checkPos)) {
            break;
        }

        var_r31 = 1;
        if (setNextIdx()) {
            var_r31 = 2;
        }
    }

    return var_r31 > 0;
}

int daStartAndGoal_c::getType() {
    switch (fopAcM_GetParam(this) & 0xFF) {
    case 0:
        return 0;
    case 1:
        return 1;
    default:
        return 2;
    }
}

struct timer_info {
    /* 0x0 */ s32 mode;
    /* 0x4 */ u8 type;
    /* 0x5 */ u8 field_0x5;
};

static timer_info l_timerType[] = {
    {3, 0, 1},
    {4, 2, 0},
    {3, 1, 1},
};

int daStartAndGoal_c::Create() {
    fopAcM_ct(this, daStartAndGoal_c);

    init();
    return cPhs_COMPLEATE_e;
}

daStartAndGoal_Path_c::~daStartAndGoal_Path_c() {}

static const u8 l_DATA[1] = {0};  // unused

void daStartAndGoal_c::init() {
    field_0xb98 = 1;
    mStarted = false;
    mType = getType();

    mPath.initialize();
    mPath.setPathInfo(fopAcM_GetParam(this) >> 8, fopAcM_GetRoomNo(this), 0);
    JUT_ASSERT(0, NULL != mPath.getPathInfo());

    if (dComIfG_getTimerPtr() == NULL) {
        dTimer_createTimer(l_timerType[mType].mode, 9999000, l_timerType[mType].type, 0, 210.0f,
                           410.0f, 32.0f, 419.0f);
    }

    mAction = 0;
}

void daStartAndGoal_c::actorPosCheck() {
    cXyz pos(daPy_getPlayerActorClass()->current.pos);
    if (mPath.chkPassed2(pos)) {
        int idx = mPath.getIdx();
        int next_idx = mPath.getNextIdx();

        if (idx == next_idx) {
            mAction = 3;
            dComIfG_TimerEnd(l_timerType[mType].mode, -1);
            dMeter2Info_setMsgTimeMs(dComIfG_getTimerNowTimeMs());
        }
    }
}

int daStartAndGoal_c::readyStartTimer() {
    if (dComIfG_TimerStart(l_timerType[mType].mode, 0)) {
        mAction = 1;
        field_0xb98 = l_timerType[mType].field_0x5;
        return 1;
    }

    return 0;
}

BOOL daStartAndGoal_c::isStartCheck() {
    return dTimer_isStart();
}

BOOL daStartAndGoal_c::isReadyCheck() {
    if (mStarted) {
        return TRUE;
    }

    if (dTimer_isReadyFlag()) {
        mStarted = true;

        if (mType == 1) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_RIVER_GAME);
        }
    }

    return dTimer_isStart();
}

int daStartAndGoal_c::Execute() {
    switch (mAction) {
    case 0:
        break;
    case 1:
        if (isStartCheck()) {
            mAction = 2;

            if (mType == 0) {
                Z2GetAudioMgr()->subBgmStart(Z2BGM_RIVER_GAME_00);
            }
        } else {
            isReadyCheck();
            break;
        }
    case 2:
        actorPosCheck();
        break;
    case 3:
        break;
    }

    return 1;
}

int daStartAndGoal_c::Draw() {
    return 1;
}

int daStartAndGoal_c::Delete() {
    if (field_0xb98 == 1) {
        dComIfG_TimerDeleteRequest(l_timerType[mType].mode);
    }

    if (mStarted == true && (mType == 0 || mType == 1)) {
        Z2GetAudioMgr()->subBgmStop();
    }

    return 1;
}

static int daStartAndGoal_Execute(daStartAndGoal_c* i_this) {
    return i_this->Execute();
}

static int daStartAndGoal_Draw(daStartAndGoal_c* i_this) {
    return i_this->Draw();
}

static int daStartAndGoal_IsDelete(daStartAndGoal_c* i_this) {
    return 1;
}

static int daStartAndGoal_Delete(daStartAndGoal_c* i_this) {
    return i_this->Delete();
}

static int daStartAndGoal_create(fopAc_ac_c* i_this) {
    return static_cast<daStartAndGoal_c*>(i_this)->Create();
}

static actor_method_class l_daStartAndGoal_Method = {
    (process_method_func)daStartAndGoal_create,  (process_method_func)daStartAndGoal_Delete,
    (process_method_func)daStartAndGoal_Execute, (process_method_func)daStartAndGoal_IsDelete,
    (process_method_func)daStartAndGoal_Draw,
};

actor_process_profile_definition g_profile_START_AND_GOAL = {
    fpcLy_CURRENT_e,
    9,
    fpcPi_CURRENT_e,
    PROC_START_AND_GOAL,
    &g_fpcLf_Method.base,
    sizeof(daStartAndGoal_c),
    0,
    0,
    &g_fopAc_Method.base,
    391,
    &l_daStartAndGoal_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
