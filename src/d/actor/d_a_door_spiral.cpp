//
// Door Spiral
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_door_spiral.h"
#include "d/d_door_param2.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "SSystem/SComponent/c_counter.h"

#if DEBUG
#include "d/d_debug_viewer.h"
#endif

#if VERSION == VERSION_SHIELD_DEBUG 
static cXyz l_step_offset(0.0f, 0.0f, 150.0f);
static cXyz l_dzb_offset(0.0f, 0.0f, 100.0f);

static cXyz l_spiral_path_point_d[] = {
    cXyz(360.0f, 0.0f, -600.0f) + l_step_offset,
    cXyz(250.0f, 0.0f, -600.0f) + l_step_offset,
    cXyz(150.0f, 0.0f, -600.0f) + l_step_offset,
    cXyz(82.0f, 0.0f, -560.0f) + l_step_offset,
    cXyz(10.0f, 0.0f, -500.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, -400.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, -300.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, -150.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, 270.0f) + l_step_offset,
};

static cXyz l_spiral_path_point_u[] = {
    cXyz(-360.0f, 0.0f, -600.0f) + l_step_offset,
    cXyz(-250.0f, 0.0f, -600.0f) + l_step_offset,
    cXyz(-150.0f, 0.0f, -600.0f) + l_step_offset,
    cXyz(-82.0f, 0.0f, -560.0f) + l_step_offset,
    cXyz(-10.0f, 0.0f, -500.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, -400.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, -300.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, -150.0f) + l_step_offset,
    cXyz(0.0f, 0.0f, 270.0f) + l_step_offset,
};

void* searchSpiralSub(void* i_actor, void* i_data) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_actor;
    fopAc_ac_c* other = (fopAc_ac_c*)i_data;

    if (actor != NULL && fopAcM_IsActor(actor)) {
        if (fopAcM_GetProfName(actor) == PROC_SPIRAL_DOOR && actor != other) {
            if (door_param2_c::getBRoomNo(actor) == door_param2_c::getBRoomNo(other)) {
                return actor;
            }
        }
    }

    return NULL;
}

const char* daSpiral_c::getAlwaysArcName() {
    return getBmdArcName();
}

const char* daSpiral_c::getSpiralDzbName(int i_type) {
    static const char* dzb[] = {
        "door-stepD.dzb",
        "door-stepU.dzb",
    };

    return dzb[i_type];
}

const char* daSpiral_c::getDzb() {
    return "door-dark.dzb";
}

const char* daSpiral_c::getBmdArcName() {
    return "K_step";
}

const char* daSpiral_c::getEvArcName() {
    return "DoorEvt";
}

const char* daSpiral_c::getBmd(int i_type) {
    static const char* bmd[] = {
        "K_stepd.bmd",
        "K_stepu.bmd",
    };

    return bmd[i_type];
}

const char* daSpiral_c::getBmd2(int i_type) {
    static char bmdName[32];
    if (i_type == daSpiral_TYPE_DOWN_e) {
        sprintf(bmdName, "door-stairSpiralU.bmd");
    } else {
        sprintf(bmdName, "door-stairSpiralD.bmd");
    }

    return bmdName;
}

J3DModelData* daSpiral_c::getModelData() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getStageRes(getBmd2(mType));
    if (modelData == NULL) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(getBmdArcName(), getBmd(mType));
    }

    return modelData;
}

const char* daSpiral_c::getStopBmd() {
    return "door-spiralStop.bmd";
}

const char* daSpiral_c::getStopBmd2() {
    return "door-stop.bmd";
}

J3DModelData* daSpiral_c::getStopModelData() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getStageRes(getStopBmd2());
    if (modelData == NULL) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(getBmdArcName(), getStopBmd());
    }

    return modelData;
}

const char* daSpiral_c::getStopDzb() {
    return "door-spiralStop.dzb";
}

static int CheckCreateHeap(fopAc_ac_c* actor) {
    daSpiral_c* i_this = (daSpiral_c*)actor;
    return i_this->CreateHeap();
}

#if DEBUG
bool daSpiral_c::debugCheckParam() {
    bool rt = false;
    if (!mIsCheckParam) {
        u8 fRoomNo = door_param2_c::getFRoomNo(this);
        if (fRoomNo == 0x3F) {
            // "Spiral Staircase: No room number specified!\n"
            OS_REPORT_ERROR("螺旋階段：部屋番号指定がありません！\n");
            rt = true;
        }

        u8 fOption = door_param2_c::getFrontOption(this);
        if (fOption == 2) {
            // "Spiral Staircase: Key option is not supported!\n"
            OS_REPORT_ERROR("螺旋階段：鍵オプションは対応していません！\n");
            rt = true;
        }

        mIsCheckParam = true;
    }

    return rt;
}
#endif

int daSpiral_c::create() {
    #if DEBUG
    if (debugCheckParam()) {
        return cPhs_ERROR_e;
    }
    #endif

    int phase_state = dComIfG_resLoad(&mAlwaysResPhase, getAlwaysArcName());
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    phase_state = dComIfG_resLoad(&mEvResPhase, getEvArcName());
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    phase_state = dComIfG_resLoad(&mBmdResPhase, getBmdArcName());
    if (phase_state != cPhs_COMPLEATE_e) {
        return phase_state;
    }

    fopAcM_SetRoomNo(this, door_param2_c::getFRoomNo(this));

    u8 back_option = door_param2_c::getBackOption(this);
    switch (back_option) {
    case 4:
        mType = daSpiral_TYPE_DOWN_e;
        break;
    case 5:
        mType = daSpiral_TYPE_UP_e;
        break;
    default:
        // "Spiral Staircase back-door option setting is incorrect!<%d>\n"
        OS_REPORT_ERROR("螺旋階段の裏オプション設定が正しくありません！<%d>\n", back_option);
        JUT_ASSERT(300, 0);
        break;
    }

    if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x7000)) {
        return cPhs_ERROR_e;
    }

    CreateInit();
    return cPhs_COMPLEATE_e;
}

// DEBUG NONMATCHING - regalloc
int daSpiral_c::CreateHeap() {
    J3DModelData* modelData = getModelData();
    JUT_ASSERT(338, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    mpSpiralDzb = new dBgW();
    if (mpSpiralDzb == NULL) {
        return 0;
    }

    cBgD_t* bgd = (cBgD_t*)dComIfG_getObjectRes(getAlwaysArcName(), getSpiralDzbName(mType));
    JUT_ASSERT(356, bgd != NULL);
    bool spiral_rt = mpSpiralDzb->Set(bgd, 1, &mSpiralBgMtx);
    if (spiral_rt == true) {
        return 0;
    }

    mDoorDarkDzb = new dBgW();
    if (mDoorDarkDzb == NULL) {
        return 0;
    }

    bgd = (cBgD_t*)dComIfG_getObjectRes(getAlwaysArcName(), getDzb());
    JUT_ASSERT(375, bgd != NULL);

    cXyz dzb_pos(l_dzb_offset);
    mDoMtx_stack_c::YrotS(home.angle.y);
    mDoMtx_stack_c::multVec(&dzb_pos, &dzb_pos);
    mDoMtx_stack_c::transS(current.pos.x + dzb_pos.x, current.pos.y + dzb_pos.y, current.pos.z + dzb_pos.z);
    mDoMtx_stack_c::YrotM(home.angle.y);
    cMtx_copy(mDoMtx_stack_c::get(), mDoorDarkBgMtx);

    bool dark_rt = mDoorDarkDzb->Set(bgd, 1, &mDoorDarkBgMtx);
    if (dark_rt == true) {
        return 0;
    }

    if (checkMakeStop() && !mStop.create(this)) {
        int _;
        return 0;
    }

    return 1;
}

int daSpiral_c::CreateInit() {
    bool rt = dComIfG_Bgsp().Regist(mpSpiralDzb, this);
    JUT_ASSERT(416, rt == 0);

    rt = dComIfG_Bgsp().Regist(mDoorDarkDzb, this);
    JUT_ASSERT(419, rt == 0);

    tevStr.room_no = current.roomNo;
    setAction(daSpiral_ACT_INIT_e);
    attention_info.position.y += 150.0f;
    eyePos.y += 150.0f;
    attention_info.flags = fopAc_AttnFlag_DOOR_e;

    calcMtx();

    mpSpiralDzb->SetRoomId((u16)door_param2_c::getFRoomNo(this));
    mDoorDarkDzb->SetRoomId((u16)door_param2_c::getFRoomNo(this));
    mpSpiralDzb->Move();
    mDoorDarkDzb->Move();

    initProc(1);
    return 1;
}

void daSpiral_c::makeEventId(int) {
    static const char* table[] = {
        "DEFAULT_SPIRAL_DOOR_F",
        "DEFAULT_SPIRAL_DOOR_F",
        "DEFAULT_STOP_OPEN",
        "DEFAULT_STOP_OPEN",
        "DEFAULT_SPIRAL_DOOR_F_STOP",
        "DEFAULT_SPIRAL_DOOR_F_STOP",
        NULL,
        NULL,
    };

    int i;
    for (i = 0; i < 8; i++) {
        mEventIds[i] = dComIfGp_getEventManager().getEventIdx(this, table[i], 0xFF);
        mMapToolIds[i] = 0xFF;
    }
}

void daSpiral_c::setEventPrm() {
    mDemoMode = 0;

    daSpiral_c* other = (daSpiral_c*)fopAcM_Search(searchSpiralSub, this);
    if (other == NULL) {
        // "Spiral Staircase: No matching spiral staircase!\n"
        OS_REPORT_ERROR("螺旋階段：対になる螺旋階段がありません！\n");
        JUT_ASSERT(488, 0);
    }

    if (other->mStop.field_0x70 == 1) {
        mDemoMode = 4;
    } else {
        mDemoMode = 0;
    }

    daPy_py_c* player;
    s32 var_r27;
    if (checkArea()) {
        player = daPy_getPlayerActorClass();
        eventInfo.setEventId(mEventIds[mDemoMode]);
        eventInfo.setMapToolId(mMapToolIds[mDemoMode]);
        eventInfo.onCondition(4);
        var_r27 = fopAcM_orderOtherEventId(this, mEventIds[mDemoMode], 0xFF, 0xFFFF, 0, 1);
        field_0x630 = player->current.pos;
    }
}

void daSpiral_c::settingStartRoom() {
    if (mStartRoomNo != mEndRoomNo && mStartRoomNo != 0x3F && mEndRoomNo != 0x3F) {
        dComIfGp_roomControl_onStatusFlag(mStartRoomNo, 8);
    }
}

void daSpiral_c::settingEndRoom() {
    fopAc_ac_c* door = dComIfGp_event_getDoorPartner();
    mStartRoomNo = (u8)door_param2_c::getFRoomNo(this);
    mEndRoomNo = (u8)door_param2_c::getFRoomNo(door);

    if (mStartRoomNo != mEndRoomNo && mStartRoomNo != 0x3F && mEndRoomNo != 0x3F) {
        dComIfGp_roomControl_offStatusFlag(mEndRoomNo, 8);
    }

    dStage_roomControl_c::setNextStayNo(mEndRoomNo);
}

void daSpiral_c::EndCommon() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp14 = player->current.pos - current.pos;

    f32 prod = sp14.inprodXZ(field_0x620);
    f32 var_f29 = prod < 0.0f ? 180.0f : -180.0f;

    cXyz pos(current.pos.x - (var_f29 * field_0x620.x), current.pos.y, current.pos.z - (var_f29 * field_0x620.z));
    s16 angle;
    if (prod > 0.0f) {
        int _;  // fakematch?
        angle = current.angle.y;
    } else {
        angle = current.angle.y + 0x8000;
    }

    dComIfGs_setRestartRoom(pos, angle, fopAcM_GetRoomNo(player));
}

BOOL daSpiral_c::checkArea() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp8 = player->current.pos - current.pos;

    int angle = current.angle.y;
    if (field_0x62c == 1) {
        angle += 0x7FFF;
    }

    mDoMtx_stack_c::YrotS(-angle);
    mDoMtx_stack_c::multVec(&sp8, &sp8);

    f32 x = std::fabs(sp8.x);
    if (x > 225.0f) {
        return FALSE;
    }

    f32 y = std::fabs(sp8.y);
    if (y > 300.0f) {
        return FALSE;
    }

    f32 z = sp8.z;
    if (z < 0.0f && z > -450.0f) {
        return TRUE;
    }

    return FALSE;
}

void daSpiral_c::calcMtx() {
    cXyz step_pos(l_step_offset);
    mDoMtx_stack_c::YrotS(home.angle.y);
    mDoMtx_stack_c::multVec(&step_pos, &step_pos);
    mDoMtx_stack_c::transS(current.pos.x + step_pos.x, current.pos.y + step_pos.y, current.pos.z + step_pos.z);
    mDoMtx_stack_c::YrotM(home.angle.y);
    cMtx_copy(mDoMtx_stack_c::get(), mSpiralBgMtx);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mpSpiralDzb->Move();
    field_0x5f5 = dComIfGp_roomControl_getStayNo();
}

void daSpiral_c::initProc(int param_0) {
    field_0x620.set(cM_ssin(home.angle.y), 0.0f, cM_scos(home.angle.y));
    eventInfo.setArchiveName((char*)getEvArcName());
    makeEventId(param_0);
}

void daSpiral_c::initOpenDemo(int param_0) {
    shape_angle.y = current.angle.y;
    if (field_0x62c == 1) {
        shape_angle.y += (s16)0x7FFF;
    }

    mStaffId = dComIfGp_evmng_getMyStaffId("SHUTTER_DOOR", NULL, 0);
    if (param_0 != 0) {
        dComIfGp_event_onEventFlag(2);
    }

    dComIfGp_event_onEventFlag(0x40);
}

int daSpiral_c::getDemoAction() {
    static char* action_table[] = {
        "WAIT",
        "START",
        "SETGOAL",
        "SETSTART",
        "SETPOS",
        "L_SET",
        "SETROOM",
        "SETROOM2",
        "RUN",
        "STOP_OPEN",
        "STOP_CLOSE",
        "END_CHECK",
    };

    return dComIfGp_evmng_getMyActIdx(mStaffId, action_table, ARRAY_SIZE(action_table), 0, 0);
}

int daSpiral_c::demoProc() {
    fopAc_ac_c* door = dComIfGp_event_getDoorPartner();
    if (door == this) {
        mStaffId = dComIfGp_evmng_getMyStaffId("PARTNER_DOOR", NULL, 0);
    } else {
        mStaffId = dComIfGp_evmng_getMyStaffId("SHUTTER_DOOR", NULL, 0);
    }

    int action_idx = getDemoAction();
    int rt = 0;
    daPy_py_c* player = daPy_getPlayerActorClass();

    cXyz goal_point;
    cXyz* point_list;
    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch (action_idx) {
        case daSpiral_DEMOACT_START_e:
            setPartner();
            break;
        case daSpiral_DEMOACT_SETGOAL_e:
            setGoal();
            break;
        case daSpiral_DEMOACT_RUN_e: {
            mPlRunPointNo = 1;

            if (mType == daSpiral_TYPE_DOWN_e) {
                point_list = l_spiral_path_point_d;
            } else {
                point_list = l_spiral_path_point_u;
            }

            goal_point = point_list[mPlRunPointNo];
            mDoMtx_stack_c::YrotS(home.angle.y);
            mDoMtx_stack_c::multVec(&goal_point, &goal_point);
            goal_point += current.pos;
            dComIfGp_evmng_setGoal(&goal_point);
            break;
        }
        case daSpiral_DEMOACT_SETROOM_e:
            settingEndRoom();
            break;
        case daSpiral_DEMOACT_SETROOM2_e:
            settingStartRoom();
            break;
        case daSpiral_DEMOACT_SETPOS_e:
            setNextSpiral();
            // "Spiral Staircase: force player move!<%d>"
            OS_REPORT("\x1B[33m螺旋階段：プレイヤー強制移動！<%d>\n\x1B[m", g_Counter.mCounter0);
            mTimer = 10;
            break;
        case daSpiral_DEMOACT_STOP_OPEN_e:
            mStop.openInit(this);
            break;
        case daSpiral_DEMOACT_STOP_CLOSE_e:
            setStop();
            if (mStop.field_0x70 != 0) {
                mStop.closeInit(this);
                mStop.regist(this);
            }
            break;
        case daSpiral_DEMOACT_L_SET_e:
            mTimer = 2;
            break;
        case daSpiral_DEMOACT_END_CHECK_e:
            clrPartner();
            EndCommon();
            break;
        }
    }

    switch (action_idx) {
    case daSpiral_DEMOACT_END_CHECK_e:
        shape_angle.y = current.angle.y;
        setAction(daSpiral_ACT_WAIT_e);
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    case daSpiral_DEMOACT_STOP_OPEN_e: 
        if (mStop.openProc(this)) {
            mStop.release(this);
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        mStop.calcMtx(this);
        break;
    case daSpiral_DEMOACT_STOP_CLOSE_e:
        if (mStop.closeProc(this)) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        mStop.calcMtx(this);
        break;
    case daSpiral_DEMOACT_RUN_e: 
        if (RunPlayerSpiral()) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case daSpiral_DEMOACT_L_SET_e: 
        if (cLib_calcTimer(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }

    return rt;
}

int daSpiral_c::actionWait() {
    fopAc_ac_c* door = dComIfGp_event_getDoorPartner();
    if (door == this) {
        shape_angle.y = current.angle.y;
        shape_angle.y += (s16)0x7FFF;
        mStaffId = dComIfGp_evmng_getMyStaffId("PARTNER_DOOR", NULL, 0);
        setAction(daSpiral_ACT_DEMO_e);
        eventInfo.setEventId(mEventIds[mDemoMode]);
        eventInfo.setMapToolId(mMapToolIds[mDemoMode]);
        eventInfo.onCondition(4);
        return 1;
    }

    if (mStop.field_0x70 != 0) {
        if (eventInfo.checkCommandDemoAccrpt()) {
            mStaffId = dComIfGp_evmng_getMyStaffId("SHUTTER_DOOR", NULL, 0);
            shape_angle.y = current.angle.y;
            if (field_0x62c == 1) {
                shape_angle.y += (s16)0x7FFF;
            }
            setAction(daSpiral_ACT_DEMO_e);
            demoProc();
        } else if (checkStopOpen()) {
            setStopDemo();
            fopAcM_orderOtherEventId(this, mEventIds[mDemoMode], mMapToolIds[mDemoMode], 0xFFFF, 0, 1);
        }
    } else if (eventInfo.checkCommandDemoAccrpt()) {
        initOpenDemo(0);
        setAction(daSpiral_ACT_DEMO_e);
        demoProc();
    } else if (checkStopClose()) {
        mStop.field_0x70 = 1;
        mStop.regist(this);
        mStop.closeInit(this);
        mStop.calcMtx(this);
        setAction(daSpiral_ACT_STOP_CLOSE_e);
        return 1;
    } else {
        makeEventId(1);
        setEventPrm();
    }

    return 1;
}

int daSpiral_c::actionDemo() {
    fopAc_ac_c* door = dComIfGp_event_getDoorPartner();
    if (door == this) {
        demoProc();
    } else if (dComIfGp_evmng_endCheck(mEventIds[mDemoMode])) {
        setAction(daSpiral_ACT_WAIT_e);
        dComIfGp_event_reset();
        shape_angle.y = current.angle.y;
    } else {
        demoProc();
    }

    return 1;
}

int daSpiral_c::actionStopClose() {
    if (mStop.closeProc(this)) {
        setAction(daSpiral_ACT_WAIT_e);
    }

    mStop.calcMtx(this);
    return 1;
}

int daSpiral_c::actionInit() {
    actionWait();
    setAction(daSpiral_ACT_WAIT_e);
    setStop();
    mStop.calcMtx(this);
    return 1;
}

void daSpiral_c::setPartner() {
    fopAc_ac_c* actor = fopAcM_Search(searchSpiralSub, this);
    JUT_ASSERT(1065, actor);

    dComIfGp_event_setDoorPartner(actor);
    fopAcM_OnStatus(actor, 0x8000);
}

void daSpiral_c::clrPartner() {
    dComIfGp_event_setDoorPartner(NULL);
}

void daSpiral_c::setGoal() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz goal_pos(current.pos);
    f32 var_f31 = 450.0f;
    s16 angle = shape_angle.y + 0x7FFF;

    if (this == dComIfGp_event_getDoorPartner()) {}

    goal_pos.x += var_f31 * cM_ssin(angle);
    goal_pos.y = current.pos.y;
    goal_pos.z += var_f31 * cM_scos(angle);
    dComIfGp_evmng_setGoal(&goal_pos);
}

void daSpiral_c::setNextSpiral() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* door = dComIfGp_event_getDoorPartner();
    JUT_ASSERT(1121, door != NULL);

    cXyz pl_pos;
    s16 pl_angle = (s16)door->current.angle.y;
    if (mType == daSpiral_TYPE_DOWN_e) {
        pl_pos = l_spiral_path_point_u[0];
        pl_angle += (s16)0x4000;
    } else {
        pl_pos = l_spiral_path_point_d[0];
        pl_angle -= (s16)0x4000;
    }

    mDoMtx_stack_c::transS(door->current.pos.x, door->current.pos.y, door->current.pos.z);
    mDoMtx_stack_c::YrotM(door->current.angle.y);
    mDoMtx_stack_c::multVec(&pl_pos, &pl_pos);

    if (mType == daSpiral_TYPE_DOWN_e) {
        pl_pos.y += 200.0f;
    }

    player->setPlayerPosAndAngle(&pl_pos, pl_angle, 0);
}

int daSpiral_c::RunPlayerSpiral() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz* point_list;
    if (mType == daSpiral_TYPE_DOWN_e) {
        point_list = l_spiral_path_point_d;
    } else {
        point_list = l_spiral_path_point_u;
    }

    cXyz* pgoal = dComIfGp_evmng_getGoal();
    if (pgoal->absXZ(player->current.pos) < 80.0f + KREG_F(4)) {
        mPlRunPointNo++;
        if (mPlRunPointNo == 9) {
            return 1;
        }

        cXyz pos(point_list[mPlRunPointNo]);
        mDoMtx_stack_c::YrotS(home.angle.y);
        mDoMtx_stack_c::multVec(&pos, &pos);
        pos += current.pos;
        dComIfGp_evmng_setGoal(&pos);
    }

    return 0;
}

u8 daSpiral_c::frontCheck() {
    int roomNo = dComIfGp_roomControl_getStayNo();
    int fRoomNo = door_param2_c::getFRoomNo(this);
    if (roomNo == fRoomNo) {
        return 0;
    }

    return 1;
}

int daSpiral_c::checkExecute() {
    field_0x62c = frontCheck();

    if (eventInfo.checkCommandDemoAccrpt() || eventInfo.checkCommandDoor()) {
        return 2;
    }

    fopAc_ac_c* door = dComIfGp_event_getDoorPartner();
    if (door == this) {
        return 2;
    }

    if (mRoomNo != dComIfGp_roomControl_getStayNo()) {
        return 0;
    }

    if (field_0x62c != 2 && adjoinPlayer()) {
        return 2;
    }

    return 0;
}

int daSpiral_c::execute() {
    int var_r29 = checkExecute();
    switch (var_r29) {
    case 0:
        setAction(daSpiral_ACT_INIT_e);
        break;
    case 2:
        switch (mAction) {
        case daSpiral_ACT_INIT_e:
            actionInit();
            break;
        case daSpiral_ACT_WAIT_e:
            actionWait();
            break;
        case daSpiral_ACT_DEMO_e:
            actionDemo();
            break;
        case daSpiral_ACT_STOP_CLOSE_e:
            actionStopClose();
            break;
        }
        break;
    default:
    case 1:
        JUT_ASSERT(1364, 0);
        break;
    }

    mRoomNo = dComIfGp_roomControl_getStayNo();
    return 1;
}

BOOL daSpiral_c::adjoinPlayer() {
    int fRoomNo = door_param2_c::getFRoomNo(this);
    if (dComIfGp_roomControl_checkRoomDisp(fRoomNo)) {
        return TRUE;
    }

    return FALSE;
}

BOOL daSpiral_c::drawCheck_local() {
    if (!adjoinPlayer()) {
        return FALSE;
    }

    return TRUE;
}

BOOL daSpiral_c::drawCheck(int) {
    BOOL rt = drawCheck_local();
    if (rt) {
        fopAcM_SetStatusMap(this, 9);
    } else {
        fopAcM_OffStatus(this, 0);
    }

    return rt;
}

#if DEBUG
// DEBUG NONMATCHING - stack
void daSpiral_c::debugDraw() {
    GXColor spC = {0xFF, 0xFF, 0xFF, 0xFF};
    GXColor sp8 = spC;
    GXColor color = sp8;

    cXyz sp68[4];
    cXyz sp38[4];

    f32 temp_f31 = 225.0f;
    f32 temp_f30 = 0.0f;

    sp68[0] = sp68[1] = sp68[2] = sp68[3] = current.pos;

    sp38[0].x = sp38[1].x = temp_f31;
    sp38[2].x = sp38[3].x = -temp_f31;

    sp38[0].y = sp38[1].y = sp38[2].y = sp38[3].y = 5.0f;

    sp38[0].z = sp38[3].z = -450.0f;
    sp38[1].z = sp38[2].z = temp_f30;

    s16 var_r25 = current.angle.y;
    if (field_0x62c == 1) {
        var_r25 += (s16)0x7FFF;
    }

    mDoMtx_stack_c::YrotS(var_r25);

    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::multVec(&sp38[i], &sp38[i]);
    }

    for (int i = 0; i < 4; i++) {
        sp68[i] = sp68[i] + sp38[i];
    }

    if (KREG_S(9) == 1000) {
        dDbVw_drawQuadOpa(sp68, spC, TRUE);
        dDbVw_drawSphereXlu(current.pos, 30.0f, color, TRUE);
    }

    if (KREG_S(9) == 2000) {
        cXyz* var_r27 = l_spiral_path_point_d;
        if (mType == daSpiral_TYPE_UP_e) {
            var_r27 = l_spiral_path_point_u;
        }

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(home.angle.y);

        cXyz sp2C;
        cXyz sp20;
        for (int i = 0; i < 8; i++) {
            sp2C = var_r27[0];
            sp20 = var_r27[1];
            mDoMtx_stack_c::multVec(&sp2C, &sp2C);
            mDoMtx_stack_c::multVec(&sp20, &sp20);
            dDbVw_drawLineXlu(sp2C, sp20, color, TRUE, 12);
            var_r27++;
        }
    }
}
#endif

int daSpiral_c::draw() {
    BOOL var_r27 = drawCheck(0);
    if (!var_r27) {
        if (field_0x5f6 == 0) {
            field_0x5f6 = 1;
            dComIfG_Bgsp().Release(mpSpiralDzb);
            dComIfG_Bgsp().Release(mDoorDarkDzb);
            mStop.release(this);
        }
        return 1;
    }

    if (field_0x5f6 != 0) {
        field_0x5f6 = 0;

        bool rt = dComIfG_Bgsp().Regist(mpSpiralDzb, this);
        JUT_ASSERT(1544, !rt);

        rt = dComIfG_Bgsp().Regist(mDoorDarkDzb, this);
        JUT_ASSERT(1546, !rt);

        if (mStop.field_0x70 != 0) {
            mStop.regist(this);
        }
    }

    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    if (mStop.field_0x70 != 0) {
        mStop.draw(this);
    }

    #if DEBUG
    debugDraw();
    #endif
    return 1;
}

int daSpiral_c::Delete() {
    if (heap != NULL) {
        if (mpSpiralDzb->ChkUsed()) {
            dComIfG_Bgsp().Release(mpSpiralDzb);
        }

        if (mDoorDarkDzb->ChkUsed()) {
            dComIfG_Bgsp().Release(mDoorDarkDzb);
        }
    }

    mStop.remove(this);
    dComIfG_resDelete(&mAlwaysResPhase, getAlwaysArcName());
    dComIfG_resDelete(&mEvResPhase, getEvArcName());
    dComIfG_resDelete(&mBmdResPhase, getBmdArcName());
    return 1;
}

BOOL daSpiral_c::checkMakeStop() {
    u8 option = door_param2_c::getFrontOption(this);
    if (option == 1 || option == 3) {
        return 1;
    }

    return 0;
}

int daSpiral_c::checkStopF() {
    int fRoomNo = door_param2_c::getFRoomNo(this);
    u8 option = door_param2_c::getFrontOption(this);
    u8 swbit = door_param2_c::getSwbit(this);
    if (swbit == 0xFF) {
        return 0;
    }

    if (option != 1 && option != 3) {
        return 0;
    }

    if (!dComIfGp_roomControl_checkStatusFlag(fRoomNo, 1)) {
        return -1;
    }

    return dComIfGs_isSwitch(swbit, fRoomNo) == FALSE;
}

void daSpiral_c::setStopDemo() {
    u8 option = door_param2_c::getFrontOption(this);
    BOOL isMsgDoor = door_param2_c::isMsgDoor(this);
    u8 eventNo = door_param2_c::getEventNo(this);

    if (field_0x62c == 0) {
        if ((option == 3 || option == 1) && isMsgDoor == 0 && eventNo != 0xFF) {
            mDemoMode = 6;
            mMapToolIds[mDemoMode] = door_param2_c::getEventNo(this);
            mEventIds[mDemoMode] = dComIfGp_getEventManager().getEventIdx(this, mMapToolIds[mDemoMode]);
        } else {
            mDemoMode = 2;
        }
    }
}

BOOL daSpiral_c::checkStopClose() {
    u8 swbit = 0xFF;
    u8 fRoomNo = door_param2_c::getFRoomNo(this);
    if (mStop.mpModel == NULL) {
        return 0;
    }

    int option = door_param2_c::getFrontOption(this);
    if (field_0x62c == 0) {
        if (option == 3) {
            return 0;
        }

        swbit = door_param2_c::getSwbit(this);
        fRoomNo = door_param2_c::getFRoomNo(this);

    }

    if (swbit == 0xFF) {
        return 0;
    }

    if (!dComIfGs_isSwitch(swbit, fRoomNo)) {
        return 1;
    }

    return 0;
}

void daSpiral_c::setStop() {
    if (checkMakeStop() && mStop.mpModel != NULL) {
        mStop.field_0x72 = field_0x62c;
        mStop.field_0x70 = checkStopF();
        mStop.mHeight = 0.0f;
    }
}

BOOL daSpiral_c::checkStopOpen() {
    u8 option = door_param2_c::getFrontOption(this);
    u8 swbit = door_param2_c::getSwbit(this);
    u8 fRoomNo = door_param2_c::getFRoomNo(this);

    if (field_0x62c == 0 && option == 3) {
        if (!dComIfGp_event_runCheck() || field_0x5f8 == 0) {
            if (dComIfGp_roomControl_checkRoomDisp(fRoomNo) && !fopAcM_myRoomSearchEnemy(fRoomNo)) {
                if (field_0x5f8 > 0) {
                    field_0x5f8--;
                    return 0;
                }

                if (swbit != 0xFF) {
                    dComIfGs_onSwitch(swbit, fRoomNo);
                }

                return 1;
            }
            field_0x5f8 = 65;
        }
    } else if (swbit != 0xFF && dComIfGs_isSwitch(swbit, fRoomNo)) {
        return 1;
    }

    return 0;
}

int dSpiral_stop_c::create(daSpiral_c* i_spiral) {
    J3DModelData*  modelData = i_spiral->getStopModelData();
    JUT_ASSERT(1830, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel== NULL) {
        return 0;
    }

    mpBgw = new dBgW();
    if (mpBgw == NULL) {
        return 0;
    }

    cBgD_t* bgd = (cBgD_t*)dComIfG_getObjectRes(i_spiral->getAlwaysArcName(), i_spiral->getStopDzb());
    JUT_ASSERT(1849, bgd != NULL);
    bool rt = mpBgw->Set(bgd, 1, &mBgMtx);
    if (rt == true) {
        int _; // fakematch?
        return 0;
    }

    return 1;
}

int dSpiral_stop_c::remove(daSpiral_c* i_spiral) {
    release(i_spiral);
    field_0x70 = 0;
    return 1;
}

void dSpiral_stop_c::calcMtx(daSpiral_c* i_spiral) {
    f32 var_f31 = -40.0f;
    cXyz pos(i_spiral->current.pos);
    cXyz offset;
    offset.set(0.0f, 0.0f, 100.0f);
    if (field_0x72 == 1) {
        offset.set(0.0f, 0.0f, -100.0f);
    }

    cLib_offsetPos(&pos, &i_spiral->home.pos, i_spiral->home.angle.y, &offset);

    s16 angle = i_spiral->current.angle.y;
    if (field_0x72 == 1) {
        angle += (s16)0x7FFF;
    }

    mDoMtx_stack_c::transS(pos.x, pos.y, pos.z);
    mDoMtx_stack_c::YrotM(angle);
    mDoMtx_stack_c::transM(0.0f, mHeight, 0.0f);
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);

    if (mpBgw != NULL) {
        mpBgw->Move();
    }

    if (mpModel != NULL) {
        mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

void dSpiral_stop_c::openInit(daSpiral_c*) {
    mHeight = 0.0f;
    mHeightStep = 0.0f;
}

int dSpiral_stop_c::openProc(daSpiral_c*) {
    cLib_chaseF(&mHeightStep, 30.0f, 4.0f);
    if (cLib_chaseF(&mHeight, 300.0f, mHeightStep)) {
        field_0x70 = 0;
        return 1;
    }

    return 0;
}

void dSpiral_stop_c::closeInit(daSpiral_c*) {
    mHeight = 300.0f;
    mHeightStep = 0.0f;
}

int dSpiral_stop_c::closeProc(daSpiral_c*) {
    cLib_chaseF(&mHeightStep, 60.0f, 6.0f);
    if (cLib_chaseF(&mHeight, 0.0f, mHeightStep)) {
        return 1;
    }

    return 0;
}

void dSpiral_stop_c::draw(daSpiral_c* i_spiral) {
    if (mpModel != NULL) {
        g_env_light.settingTevStruct(16, &i_spiral->current.pos, &i_spiral->tevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &i_spiral->tevStr);
        mDoExt_modelUpdateDL(mpModel);
    }
}

BOOL dSpiral_stop_c::regist(daSpiral_c* i_spiral) {
    if (mpBgw != NULL) {
        bool rt = dComIfG_Bgsp().Regist(mpBgw, i_spiral);
        if (!rt) {
            return 1;
        }
    }

    return 0;
}

BOOL dSpiral_stop_c::release(daSpiral_c*) {
    if (mpBgw != NULL && mpBgw->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgw);
    }

    return 1;
}

int daSpiral_Draw(daSpiral_c* i_this) {
    return i_this->draw();
}

int daSpiral_Execute(daSpiral_c* i_this) {
    i_this->execute();
    return 1;
}

int daSpiral_Delete(daSpiral_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Door10");
    i_this->Delete();
    i_this->~daSpiral_c();
    return 1;
}

int daSpiral_Create(fopAc_ac_c* i_this) {
    fopAcM_RegisterCreateID(daSpiral_c, i_this, "Door10");
    fopAcM_ct(a_this, daSpiral_c);
    return a_this->create();
}

static actor_method_class l_daSpiral_Method = {
    (process_method_func)daSpiral_Create,  (process_method_func)daSpiral_Delete,
    (process_method_func)daSpiral_Execute, (process_method_func)NULL,
    (process_method_func)daSpiral_Draw,
};
#endif

#if VERSION == VERSION_SHIELD_DEBUG
#define DASPIRAL_METHODS &l_daSpiral_Method
#else
#define DASPIRAL_METHODS NULL
#endif

actor_process_profile_definition g_profile_SPIRAL_DOOR = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_SPIRAL_DOOR,       // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daSpiral_c),                  // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    301,                    // mPriority
    DASPIRAL_METHODS,     // sub_method
    0x44000,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_6_e,      // cullType
};
