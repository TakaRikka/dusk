/**
 * d_a_door_boss.cpp
 * Boss Door
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_door_boss.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/d_door_param2.h"
#include "SSystem/SComponent/c_math.h"

char* daBdoor_c::getArcName() {
    return "Z_bdor00";
}

char* daBdoor_c::getBmd() {
    return "Z_bdor00.bmd";
}

char* daBdoor_c::getDzb() {
    return "door30.dzb";
}

char* daBdoor_c::getBmd2() {
    return "door_shutterBoss.bmd";
}

J3DModelData* daBdoor_c::getDoorModelData() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getStageRes(getBmd2());
    if (model_data == NULL) {
        model_data = (J3DModelData*)dComIfG_getObjectRes(getArcName(), getBmd());
    }
    return model_data;
}

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daBdoor_c*>(i_this)->CreateHeap();
}

int daBdoor_c::CreateHeap() {
    J3DModelData* door_model_data = getDoorModelData();
    mpDoorModel = mDoExt_J3DModel__create(door_model_data, 0x80000, 0x11000084);
    if (mpDoorModel == NULL) {
        return 0;
    }

    J3DModelData* lock_model_data = (J3DModelData*)dComIfG_getObjectRes(getArcName(), "VlockB.bmd");
    mpLockModel = mDoExt_J3DModel__create(lock_model_data, 0x80000, 0x11000084);
    if (mpLockModel == NULL) {
        return 0;
    }

    J3DAnmTransform* lock_bck = (J3DAnmTransform*)dComIfG_getObjectRes(getArcName(), "VlockB.bck");
    mpLockAnm = new mDoExt_bckAnm();
    if (mpLockAnm == NULL || !mpLockAnm->init(lock_bck, 1, 0, 1.0f, 0, -1, false)) {
        return 0;
    }

    J3DSkinDeform* deform = new J3DSkinDeform();
    if (deform == NULL) {
        return 0;
    }
    if (mpLockModel->setSkinDeform(deform, 1) == 4) {
        return 0;
    }

    mpBgW = new dBgW();
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(getArcName(), getDzb());
    if (dzb == NULL) {
        return 0;
    }
    calcMtx();
    return mpBgW->Set(dzb, 1, &mpDoorModel->getBaseTRMtx()) == 1 ? 0 : 1;
}

void daBdoor_c::calcMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mOffsetY, current.pos.z);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mpDoorModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + 300.0f, current.pos.z);
    mDoMtx_stack_c::YrotM(home.angle.y);
    mpLockModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daBdoor_c::CreateInit() {
    mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
    dComIfG_Bgsp().Regist(mpBgW, this);
    setAction(ACT_CLOSE_WAIT);
    attention_info.position.y += 250.0f;
    eyePos.y += 250.0f;
    attention_info.flags = fopAc_AttnFlag_DOOR_e;
    calcMtx();
    mpBgW->Move();
    u32 swbit = door_param2_c::getSwbit(this);
    if (!dComIfGs_isSwitch(swbit, fopAcM_GetRoomNo(this))) {
        mLocked = true;
    } else {
        mLocked = false;
    }
    mOffsetY = 0.0f;
    return 1;
}

cPhs__Step daBdoor_c::create() {
    fopAcM_ct(this, daBdoor_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, getArcName());
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0xa000)) {
            return cPhs_ERROR_e;
        }
        if (!CreateInit()) {
            return cPhs_ERROR_e;
        }
    }
    return step;
}

int daBdoor_c::getDemoAction() {
    static char* action_table[11] = {
        "WAIT",
        "UNLOCK",
        "OPEN",
        "SETGOAL",
        "SETGOAL2",
        "CLOSE",
        "SMOKE",
        "SMOKE_END",
        "END",
        "END_CHECK",
        "CHG_SCENE",
    };
    return dComIfGp_evmng_getMyActIdx(mStaffID, action_table, 11, 0, 0);
}

void daBdoor_c::demoProc() {
    cXyz goal;
    int done;
    int action = getDemoAction();

    if (dComIfGp_evmng_getIsAddvance(mStaffID)) {
        switch (action) {
        case 1:  // UNLOCK
            if (mLocked) {
                dComIfGs_onSwitch(door_param2_c::getSwbit(this), fopAcM_GetRoomNo(this));
                mpLockAnm->setPlaySpeed(1.0f);
            }
            break;
        case 2:  // OPEN
            mLocked = false;
            fopAcM_seStart(this, JA_SE_OBJ_STN_DOOR_MOVE_U, 0);
            dComIfG_Bgsp().Release(mpBgW);
            break;
        case 3:  // SETGOAL
            calcGoal(&goal, 0);
            dComIfGp_evmng_setGoal(&goal);
            break;
        case 4:  // SETGOAL2
            calcGoal(&goal, 1);
            dComIfGp_evmng_setGoal(&goal);
            break;
        case 5:  // CLOSE
            fopAcM_seStart(this, JA_SE_OBJ_STN_DOOR_MOVE_D, 0);
            break;
        case 6:  // SMOKE
            smokeInit();
            break;
        case 10:  // CHG_SCENE
            field_0x58c = 0;
        }
    }

    switch (action) {
    case 1:  // UNLOCK
        mpLockAnm->play();
        if (!mLocked || mpLockAnm->isStop()) {
            mLocked = false;
            dComIfGp_evmng_cutEnd(mStaffID);
        }
        break;
    case 2:  // OPEN
        cLib_chaseF(&speedF, 60.0f, 6.0f);
        done = cLib_chaseF(&mOffsetY, 600.0f, speedF);
        calcMtx();
        if (done) {
            fopAcM_seStart(this, JA_SE_OBJ_STN_DOOR_STOP_U, 0);
            dComIfGp_evmng_cutEnd(mStaffID);
        }
        break;
    case 5:  // CLOSE
        cLib_chaseF(&speedF, 60.0f, 6.0f);
        done = cLib_chaseF(&mOffsetY, 0.0f, speedF);
        calcMtx();
        if (done) {
            dComIfGp_getVibration().StartShock(4, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            fopAcM_seStart(this, JA_SE_OBJ_STN_DOOR_STOP_D, 0);
            dComIfGp_evmng_cutEnd(mStaffID);
        }
        break;
    case 10:  // CHG_SCENE
        if (field_0x58c == 0) {
            daPy_getPlayerActorClass()->onSceneChangeArea(door_param2_c::getExitNo(this), 0xff, 0);
        }
        field_0x58c--;
        break;
    case 8:  // END
        break;
    case 9:  // END_CHECK
        setAction(ACT_END);
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffID);
        break;
    }
}

void daBdoor_c::calcGoal(cXyz* i_goalP, int param_1) {
    cXyz vec = daPy_getPlayerActorClass()->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&vec, &vec);
    vec.x = 0.5f * vec.x;
    vec.y = 0.0f;
    if (param_1 == 0) {
        vec.z = -1000.0f;
    }  else {
        vec.z = -240.0f;
    }
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&vec, &vec);
    i_goalP->x = vec.x;
    i_goalP->y = 0.0f;
    i_goalP->z = vec.z;
}

void daBdoor_c::smokeInit() {
    mParticlePos = current.pos;
    mParticleAngle = shape_angle;
    dComIfGp_particle_setPolyColor(0x8156, mAcch.m_gnd, &mParticlePos, &tevStr, &mParticleAngle,
                                   NULL, 0, NULL, fopAcM_GetRoomNo(this), NULL);
}

BOOL daBdoor_c::checkArea() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz vec = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&vec, &vec);
    if (fabsf(vec.x) > 200.0f) {
        return false;
    }
    if (fabsf(vec.z) > 100.0f) {
        return false;
    }
    return (s16)fabs(current.angle.y - 0x7fff - player->current.angle.y) <= 0x4000 ? 1 : 0;
}

BOOL daBdoor_c::checkFront() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz vec = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&vec, &vec);
    return vec.z > 0.0f;
}

BOOL daBdoor_c::checkOpen() {
    if (daPy_py_c::checkNowWolf()) {
        return false;
    }
    if (!dComIfGs_isDungeonItemBossKey()) {
        return false;
    }
    if (!checkFront()) {
        return false;
    }
    if (!checkArea()) {
        return false;
    }
    return true;
}

BOOL daBdoor_c::actionWait() {
    return true;
}

static char const l_door_open_demo[21] = "DEFAULT_BS_SHUTTER_F";

static char const l_staff_name[13] = "SHUTTER_DOOR";

BOOL daBdoor_c::actionCloseWait() {
    if (eventInfo.checkCommandDoor()) {
        mStaffID = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        demoProc();
        setAction(ACT_OPEN);
    } else {
        if (checkOpen()) {
            eventInfo.setEventName((char*)l_door_open_demo);
            eventInfo.onCondition(dEvtCnd_CANDOOR_e);
        }
    }
    return 1;
}

BOOL daBdoor_c::actionOpen() {
    demoProc();
    return true;
}

BOOL daBdoor_c::actionEnd() {
    if (!mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Regist(mpBgW, this);
    }
    setAction(ACT_WAIT);
    cXyz normal(cM_ssin(home.angle.y), 0.0f, cM_scos(home.angle.y));
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz delta = player->current.pos - current.pos;
    f32 prod = delta.inprodXZ(normal);
    f32 offset = prod < 0.0f ? 180.0f : -180.0f;
    cXyz pos(current.pos.x - offset * normal.x, current.pos.y, current.pos.z - offset * normal.z);
    s32 room_no = fopAcM_GetRoomNo(player);
    s16 angle;
    if (prod > 0.0f) {
        angle = current.angle.y;
    } else {
        angle = current.angle.y + 0x8000;
    }
    dComIfGs_setRestartRoom(pos, angle, room_no);
    return true;
}

int daBdoor_c::execute() {
    typedef BOOL (daBdoor_c::*daBdoor_action)();
    static daBdoor_action l_action[4] = {
        &daBdoor_c::actionWait,
        &daBdoor_c::actionCloseWait,
        &daBdoor_c::actionOpen,
        &daBdoor_c::actionEnd,
    };
    if (fopAcM_CheckStatus(this, 0x1000)) {
        mStaffID = dComIfGp_evmng_getMyStaffId(l_staff_name, NULL, 0);
        demoProc();
    } else {
        (this->*l_action[mAction])();
    }
    mAcch.CrrPos(dComIfG_Bgsp());
    return 1;
}

BOOL daBdoor_c::checkDraw() {
    return dComIfGp_roomControl_checkRoomDisp(fopAcM_GetRoomNo(this)) ? 1 : 0;
}

int daBdoor_c::draw() {
    BOOL draw = checkDraw();
    if (!draw) {
        if (!mBgInactive) {
            dComIfG_Bgsp().Release(mpBgW);
            mBgInactive = true;
        }
        return 1;
    } else {
        if (mBgInactive) {
            dComIfG_Bgsp().Regist(mpBgW, this);
            mBgInactive = false;
        }
        if (!draw) {
            fopAcM_OffStatus(this, 0);
        }
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mpDoorModel, &tevStr);
        dComIfGd_setListBG();
        mDoExt_modelUpdateDL(mpDoorModel);
        dComIfGd_setList();
        if (mLocked) {
            g_env_light.setLightTevColorType_MAJI(mpLockModel, &tevStr);
            mpLockAnm->entry(mpLockModel->getModelData());
            mDoExt_modelUpdateDL(mpLockModel);
        }
        return 1;
    }
}

int daBdoor_c::Delete() {
    if (heap != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    dComIfG_resDelete(&mPhaseReq, getArcName());
    return 1;
}

static int daBdoor_Draw(daBdoor_c* i_this) {
    return i_this->draw();
}

static int daBdoor_Execute(daBdoor_c* i_this) {
    i_this->execute();
    return 1;
}

static int daBdoor_Delete(daBdoor_c* i_this) {
    i_this->Delete();
    return 1;
}

static cPhs__Step daBdoor_Create(fopAc_ac_c* i_this) {
    return static_cast<daBdoor_c*>(i_this)->create();
}

static actor_method_class l_daBdoor_Method = {
    (process_method_func)daBdoor_Create,
    (process_method_func)daBdoor_Delete,
    (process_method_func)daBdoor_Execute,
    (process_method_func)NULL,
    (process_method_func)daBdoor_Draw,
};

actor_process_profile_definition g_profile_BOSS_DOOR = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_BOSS_DOOR,
    &g_fpcLf_Method.base,
    sizeof(daBdoor_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x129,
    &l_daBdoor_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_6_e,
};
