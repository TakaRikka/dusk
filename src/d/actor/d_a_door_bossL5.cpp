//
// Boss Door Level 5
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_door_bossL5.h"
#include "d/actor/d_a_obj_keyhole.h"
#include "d/d_com_inf_game.h"
#include "d/d_door_param2.h"
#include "d/actor/d_a_player.h"
#include "d/d_meter2_info.h"
#include "SSystem/SComponent/c_math.h"

const char* daBdoorL5_c::getArcName() {
    return "Z_bdor00";
}

const char* daBdoorL5_c::getAlwaysArcName() {
    return "static";
}

const char* daBdoorL5_c::getBmd() {
    return "Z_bdor00.bmd";
}

const char* daBdoorL5_c::getDzb() {
    return "door-shutterBoss.dzb";
}

const char* daBdoorL5_c::getAnmArcName() {
    return "BDoorTAnm";
}

s32 daBdoorL5_c::getOpenAnm() {
    return 5;
}

s32 daBdoorL5_c::getCloseAnm() {
    return 4;
}

J3DModelData* daBdoorL5_c::getDoorModelData() {
    return static_cast<J3DModelData*>(dComIfG_getObjectRes(getArcName(), getBmd()));
}

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daBdoorL5_c*>(i_this)->CreateHeap();
}

static char const l_door_open_demo[22] = "DEFAULT_BS_SHUTTER_L5";

static char const l_door_open_demo_1st[26] = "DEFAULT_BS_SHUTTER_L5_1ST";

static char const l_staff_name[13] = "SHUTTER_DOOR";

static char* action_table[14] = {
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
    "KEYDELETE",
    "PLY_NODISP",
    "PLY_DISP",
};

int daBdoorL5_c::CreateHeap() {
    J3DModelData* modelData = getDoorModelData();
    JUT_ASSERT(313, modelData != NULL);
    field_0x580 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (field_0x580 == NULL) {
        return 0;
    }

    J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes(getAnmArcName(), getOpenAnm());
    JUT_ASSERT(354, anm != NULL);
    field_0x584 = new mDoExt_bckAnm();
    if (field_0x584 == NULL || !field_0x584->init(anm, 1, 0, 1.0f, 0, -1, false)) {
        return 0;
    }

    field_0x584->entry(field_0x580->getModelData());
    field_0x580->calc();
    calcMtx();
    field_0x588 = new dBgW();
    if (field_0x588 == NULL || field_0x588->Set((cBgD_t*)dComIfG_getObjectRes(getAlwaysArcName(), getDzb()), 1, &field_0x580->getBaseTRMtx())) {
        return 0;
    }
    return 1;
}

void daBdoorL5_c::calcMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);
    field_0x580->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daBdoorL5_c::CreateInit() {
    field_0x5b8.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                     &field_0x790, fopAcM_GetSpeed_p(this), NULL, NULL);
    int rt = dComIfG_Bgsp().Regist(field_0x588, this);
    JUT_ASSERT(217, rt == 0);
    setAction(ACTION_CLOSE_WAIT);
    attention_info.position.y += 250.0f;
    eyePos.y += 250.0f;
    attention_info.flags = fopAc_AttnFlag_DOOR_e;
    calcMtx();
    field_0x588->Move();
    door_param2_c::getSwbit(this);
    field_0x593 = 0xff;
    eventInfo.setArchiveName((char*)getArcName());
    field_0x598 = dComIfGp_getEventManager().getEventIdx(this,
                                            l_door_open_demo_1st, 0xff);
    field_0x59a =
        dComIfGp_getEventManager().getEventIdx(this, l_door_open_demo, 0xff);
    return 1;
}

int daBdoorL5_c::create() {
    fopAcM_ct(this, daBdoorL5_c);
    int rv = dComIfG_resLoad(&mPhase1, getArcName());
    if (rv != cPhs_COMPLEATE_e) {
        return rv;
    }
    rv = dComIfG_resLoad(&mPhase3, getAnmArcName());
    if (rv != cPhs_COMPLEATE_e) {
        return rv;
    }
    rv = dComIfG_resLoad(&mPhase2, getAlwaysArcName());
    if (rv != cPhs_COMPLEATE_e) {
        return rv;
    }
    if (!dComIfGs_isSwitch(door_param2_c::getSwbit(this), fopAcM_GetRoomNo(this))
                            && dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) != 3) {
        createKey();
    } else {
        mKeyHoleId = -1;
    }

    if (fopAcM_entrySolidHeap(this, CheckCreateHeap, 0xd40) == 0) {
        return cPhs_ERROR_e;
    } 
    if (CreateInit() != 0) {
        return cPhs_COMPLEATE_e;
    }
    return cPhs_ERROR_e;
}

int daBdoorL5_c::getDemoAction() {
    return dComIfGp_evmng_getMyActIdx(field_0x59c, action_table, 14, 0, 0);
}

void daBdoorL5_c::demoProc() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    int demoAction = getDemoAction();
    cXyz cStack_38;
    if (dComIfGp_evmng_getIsAddvance(field_0x59c)) {
        switch (demoAction) {
        case 1:
            if (dComIfGs_isSwitch(door_param2_c::getSwbit(this), fopAcM_GetRoomNo(this)) == 0) {
                player->onPlayerNoDraw();
                player->onPlayerShadowNoDraw();
            }
            unlockInit();
            break;
        case 2:
            openInit();
            break;
        case 3:
            calcGoal(&cStack_38, 0);
            dComIfGp_evmng_setGoal(&cStack_38);
            break;
        case 4:
            calcGoal(&cStack_38, 1);
            dComIfGp_evmng_setGoal(&cStack_38);
            break;
        case 5:
            closeInit();
            break;
        case 6:
            smokeInit();
            break;
        case 10:
            field_0x596 = 0;
            break;
        case 11:
            field_0x594 = 40;
            break;
        case 12:
            player->onPlayerNoDraw();
            player->onPlayerShadowNoDraw();
            break;
        case 13:
            player->offPlayerNoDraw();
            player->offPlayerShadowNoDraw();
            break;
        }
    }
    switch (demoAction) {
    case 1: {
        obj_keyhole_class* keyhole = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);
        if ((keyhole != NULL && keyhole->checkOpenEnd()) || mKeyHoleId == -1) {
            dComIfGp_evmng_cutEnd(field_0x59c);
        }
        break;
    }
    case 2:
        if (openProc() != 0) {
            openEnd();
            dComIfGp_evmng_cutEnd(field_0x59c);
        }
        break;
    case 5:
        if (closeProc() != 0) {
            closeEnd();
            dComIfGp_evmng_cutEnd(field_0x59c);
        }
        break;
    case 8:
        break;
    case 10:
        if (field_0x596 == 0) {
            daPy_py_c* player = daPy_getPlayerActorClass();
            player->onSceneChangeArea(door_param2_c::getExitNo(this), 0xff, NULL);
        }
        field_0x596--;
        break;
    case 9:
        setAction(ACTION_END);
        dComIfGp_evmng_cutEnd(field_0x59c);
        break;
     default:
        dComIfGp_evmng_cutEnd(field_0x59c);
        break;
    }
}

int daBdoorL5_c::openInit() {
    dComIfG_Bgsp().Release(field_0x588);
    J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes(getAnmArcName(), getOpenAnm());
    JUT_ASSERT(454, anm != NULL);
    int rt = field_0x584->init(anm, 1, 0, 1.0f, 0, -1, true);
    JUT_ASSERT(456, rt == 0);
    return 1;
}

int daBdoorL5_c::openProc() {
    int rv = field_0x584->play();
    if (field_0x584->getFrame() == 18.0f) {
        mDoAud_seStart(Z2SE_OBJ_LV3_SHTR_OP, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
    return rv;
}

int daBdoorL5_c::openEnd() {
    return 1;
}

int daBdoorL5_c::closeInit() {
    J3DAnmTransform* anm = (J3DAnmTransform*)dComIfG_getObjectRes(getAnmArcName(), getCloseAnm());
    JUT_ASSERT(511, anm != NULL);
    int rt = field_0x584->init(anm, 1, 0, 1.0f, 0, -1, true);
    JUT_ASSERT(513, rt == 0);
    mDoAud_seStart(Z2SE_OBJ_LV3_SHTR_CL, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

int daBdoorL5_c::closeProc() {
    return field_0x584->play();
}

int daBdoorL5_c::closeEnd() {
    dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));
    return 1;
}

int daBdoorL5_c::unlockInit() {
    if (mKeyHoleId == -1) {
        return 1;
    }
    obj_keyhole_class* keyHole = (obj_keyhole_class*)fopAcM_SearchByID(mKeyHoleId);
    if (keyHole != NULL) {
        keyHole->setOpen();
        mDoAud_seStart(Z2SE_OBJ_BOSS_LOCK_OPEN_LV5, &keyHole->actor.current.pos, 0, 0);
    }
    dComIfGs_onSwitch(door_param2_c::getSwbit(this), fopAcM_GetRoomNo(this));
    return 1;
}

void daBdoorL5_c::calcGoal(cXyz* param_1, int param_2) {
    cXyz local_24 = dComIfGp_getPlayer(0)->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&local_24, &local_24);
    local_24.x = local_24.x * 0.5f;
    local_24.y = 0.0f;
    if (param_2 == 0) {
        local_24.z = -1000.0f;
    } else {
        local_24.z = -240.0f;
    }
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&local_24, &local_24);
    param_1->x = local_24.x;
    param_1->y = 0.0f;
    param_1->z = local_24.z;
}

void daBdoorL5_c::smokeInit() {
    field_0x5a4 = current.pos;
    field_0x5b0 = shape_angle;
    dComIfGp_particle_setPolyColor(0x8156, field_0x5b8.m_gnd, &field_0x5a4, &tevStr, &field_0x5b0,
                                   0, 0, 0, fopAcM_GetRoomNo(this), 0);
}

int daBdoorL5_c::createKey() {
    if (field_0x590 == 0) {
        cXyz prevPos(current.pos);
        current.pos = home.pos;
        cXyz pos(0.0f, 0.0f, 50.0f);
        mKeyHoleId = fopAcM_createChildFromOffset(PROC_OBJ_KEYHOLE, fopAcM_GetID(this), 0xffffff05,
                                                             &pos, fopAcM_GetRoomNo(this), &shape_angle,
                                                             &scale, -1, 0);
        current.pos = prevPos;
        if (mKeyHoleId == -1) {
            return 0;
        }
        field_0x590 = 1;
    }
    return 1;
}

int daBdoorL5_c::checkArea() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz local_48 = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&local_48, &local_48);
    if (fabsf(local_48.x) > 200.0f) {
        return 0;
    }
    if (fabsf(local_48.z) > 100.0f) {
        return 0;
    }
    if ((s16)fabs(current.angle.y - 0x7fff - player->current.angle.y) <= 0x4000) {
        return 1;
    } else {
        return 0;
    }
}

int daBdoorL5_c::checkFront() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz distance = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&distance, &distance);
    return distance.z > 0.0f;
}

int daBdoorL5_c::checkOpen() {
    if (daPy_py_c::checkNowWolf()) {
        return 0;
    }

    if (!dComIfGs_isDungeonItemBossKey()) {
        return 0;
    } 
    if (!checkFront()) {
        return 0;
    }
    if (checkArea()) {
        return 1;
    } else {
        return 0;
    }
}

int daBdoorL5_c::actionWait() {
    return 1;
}

int daBdoorL5_c::actionCloseWait() {
    if (eventInfo.checkCommandDoor()) {
        field_0x59c = dComIfGp_evmng_getMyStaffId(l_staff_name, 0, 0);
        demoProc();
        setAction(ACTION_OPEN);
    } else {
        if (checkOpen()) {
            if (dComIfGs_isSwitch(door_param2_c::getSwbit(this), fopAcM_GetRoomNo(this))) {
                eventInfo.setEventId(field_0x59a);
                eventInfo.setMapToolId(field_0x593);
            } else {
                eventInfo.setEventId(field_0x598);
                eventInfo.setMapToolId(field_0x593);
            }
            eventInfo.onCondition(4);
        }
    }
    return 1;
}

int daBdoorL5_c::actionOpen() {
    field_0x59c = dComIfGp_evmng_getMyStaffId(l_staff_name, 0, 0);
    demoProc();
    dMeter2Info_onGameStatus(2);
    if (cLib_calcTimer(&field_0x594) == 1) {
        deleteKey();
    }
    return 1;
}

int daBdoorL5_c::actionEnd() {
    if (!field_0x588->ChkUsed()) {
        bool rt = dComIfG_Bgsp().Regist(field_0x588, this);
        JUT_ASSERT(860, rt != 0);
    }
    setAction(0);
    cXyz local_70(cM_ssin(home.angle.y), 0.0f, cM_scos(home.angle.y));
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz cStack_7c = player->current.pos - current.pos;
    f32 fVar1;
    f32 dVar12 = cStack_7c.inprodXZ(local_70);
    fVar1 = dVar12 < 0.0f ? 180.0f : -180.0f;
    cXyz cStack_88(current.pos.x - fVar1 * local_70.x, current.pos.y,
                   current.pos.z - fVar1 * local_70.z);
    s32 roomNo = fopAcM_GetRoomNo(player);
    s16 angle;
    if (dVar12 > 0.0f) {
        angle = current.angle.y;
    } else {
        angle = current.angle.y + 0x8000;
    }
    dComIfGs_setRestartRoom(cStack_88, angle, roomNo);
    return 1;
}

int daBdoorL5_c::execute() {
    static actionFunc l_action[4] = {&daBdoorL5_c::actionWait, &daBdoorL5_c::actionCloseWait, &daBdoorL5_c::actionOpen, &daBdoorL5_c::actionEnd};
    field_0x59c = -1;
    if (fopAcM_CheckStatus(this, 0x1000)) {
        field_0x59c = dComIfGp_evmng_getMyStaffId(l_staff_name, 0, 0);
        dMeter2Info_onGameStatus(2);
        demoProc();
    } else {
        (this->*(((actionFunc*)l_action)[mAction]))();
    }
    return 1;
}

int daBdoorL5_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(field_0x580, &tevStr);
    dComIfGd_setListBG();
    g_env_light.setLightTevColorType_MAJI(field_0x580, &tevStr);
    field_0x584->entry(field_0x580->getModelData());
    mDoExt_modelUpdateDL(field_0x580);
    mDoExt_bckAnmRemove(field_0x580->getModelData());
    dComIfGd_setList();
    return 1;
}

int daBdoorL5_c::Delete() {
    if (heap != NULL && field_0x588->ChkUsed()) {
        dComIfG_Bgsp().Release(field_0x588);
    }
    dComIfG_resDelete(&mPhase1, getArcName());
    dComIfG_resDelete(&mPhase3, getAnmArcName());
    dComIfG_resDelete(&mPhase2, getAlwaysArcName());
    return 1;
}

static int daBdoorL5_Draw(daBdoorL5_c* i_this) {
    return i_this->draw();
}

static int daBdoorL5_Execute(daBdoorL5_c* i_this) {
    i_this->execute();
    return 1;
}

static int daBdoorL5_Delete(daBdoorL5_c* i_this) {
    i_this->Delete();
    return 1;
}

static int daBdoorL5_Create(fopAc_ac_c* i_this) {
    return static_cast<daBdoorL5_c*>(i_this)->create();
}


static actor_method_class l_daBdoorL5_Method = {
    (process_method_func)daBdoorL5_Create,
    (process_method_func)daBdoorL5_Delete,
    (process_method_func)daBdoorL5_Execute,
    (process_method_func)NULL,
    (process_method_func)daBdoorL5_Draw,
};

actor_process_profile_definition g_profile_L5BOSS_DOOR = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_L5BOSS_DOOR,
    &g_fpcLf_Method.base,
    sizeof(daBdoorL5_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x12C,
    &l_daBdoorL5_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_6_e,
};
