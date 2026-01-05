/**
 * d_a_obj_bosswarp.cpp
 * Object - Boss Warp
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bosswarp.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"
#include "d/actor/d_a_obj_life_container.h"
#include "d/actor/d_a_obj_ystone.h"

static char* l_arcName = "ef_Portal";

static char const* l_clearEvName[9] = {
    "BOSSCLEAR01",
    "BOSSCLEAR02",
    "BOSSCLEAR03",
    "BOSSCLEAR03",
    "BOSSCLEAR05",
    "BOSSCLEAR06",
    "BOSSCLEAR07",
    "BOSSCLEAR03",
    "BOSSCLEAR03",
};

static char const* l_warp_check_evName = "WARP_CHECK";

static char const* l_warp_cancel_evName = "WARP_CANCEL";

static Vec const l_hair_offset[7] = {
    {49.0f, -34.0f, -3.0f},
    {46.0f, -15.0f, 3.0f},
    {53.0f, -32.0f, 0.0f},
    {50.0f, 0.0f, 0.0f},
    {50.0f, 0.0f, 0.0f},
    {50.0f, 0.0f, 0.0f},
    {50.0f, 0.0f, 0.0f},
};

static Vec const l_hair_rotate[7] = {
    {47.0f, 80.0f, 38.0f},
    {-2.0f, 68.0f, 2.0f},
    {160.0f, -60.0f, -160.0f},
    {0.0f, 90.0f, 0.0f},
    {0.0f, 90.0f, 0.0f},
    {0.0f, 90.0f, 0.0f},
    {0.0f, 90.0f, 0.0f},
};

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjBossWarp_c*>(i_this)->CreateHeap();
}

static int getNowLevel() {
    char const* stages[9] = {
        "D_MN05A",
        "D_MN04A",
        "D_MN01A",
        "D_MN10A",
        "D_MN11A",
        "D_MN06A",
        "D_MN07A",
        "D_MN08A",
        "D_MN01A",
    };
    char const* start_stage = dComIfGp_getStartStageName();
    for (int i = 0; i < 9; i++) {
        if (!strcmp(start_stage, stages[i])) {
            return i;
        }
    }
    return -1;
}

void daObjBossWarp_c::initBaseMtx() {
    scale.y = 0.15f;
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjBossWarp_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + 2.0f, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObjBossWarp_c::Create() {
    initBaseMtx();

    int level = getNowLevel();
    if (level == -1 || level == 8) {
        mBossClearEventId = -1;
    } else {
        mBossClearEventId =
            dComIfGp_getEventManager().getEventIdx(this, l_clearEvName[level], 0xff);
    }
    mBossClearMapToolId = 0xff;

    if (!isFirst() || level == 8) {
        mBossWarpInEventId = dComIfGp_getEventManager().getEventIdx(this, "BOSS_WARPIN", 0xff);
        mBossWarpInMapToolId = 0xff;
        mAction = 3;
        set_appear();
    } else {
        mBossWarpInEventId = dComIfGp_getEventManager().getEventIdx(this, "BOSS_WARPIN_M", 0xff);
        mBossWarpInMapToolId = 0xff;
        mAction = 0;
    }

    eventInfo.mArchiveName = l_arcName;
    mWarpCheckEventId = dComIfGp_getEventManager().getEventIdx(this, l_warp_check_evName, 0xff);
    mWarpCancelEventId = dComIfGp_getEventManager().getEventIdx(this, l_warp_cancel_evName, 0xff);

    mScaleF = 1.0f;
    execute();
    return 1;
}

int daObjBossWarp_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 6);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000284);
    if (mpModel == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* brk_anm = (J3DAnmTevRegKey*)dComIfG_getObjectRes(l_arcName, 9);
    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL || !mpBrkAnm->init(model_data, brk_anm, 1, 0, 1.0f, 0, -1)) {
        return 0;
    }
    mpBrkAnm->setPlaySpeed(0.0f);

    static int const l_btk_idx[2] = {12, 13};
    static int const l_play_mode[2] = {2, 0};
    for (int i = 0; i < 2; i++) {
        J3DAnmTextureSRTKey* btk_anm =
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, l_btk_idx[i]);
        mpBtkAnm[i] = new mDoExt_btkAnm();
        if (mpBtkAnm[i] == NULL ||
            !mpBtkAnm[i]->init(model_data, btk_anm, 1, l_play_mode[i], 1.0f, 0, -1))
        {
            return 0;
        }
        mpBtkAnm[i]->setPlaySpeed(0.0f);
    }

    return 1;
}

cPhs__Step daObjBossWarp_c::create() {
    fopAcM_ct(this, daObjBossWarp_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, CheckCreateHeap, 0x18e0)) {
            return cPhs_ERROR_e;
        }
        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }
    return step;
}

void daObjBossWarp_c::appear(int param_0) {
    mParticlePos = current.pos;
    mParticlePos.y += 2.0f;
    static u16 const l_eff_id[4] = {0x8548, 0x8549, 0x854a, 0x854b};

    for (int i = 0; i < 4; i++) {
        if (mpParticle[i] == NULL) {
            if (i == 3) {
                mpParticle[i] = dComIfGp_particle_set(l_eff_id[i], &mParticlePos, NULL, &scale);
            } else {
                mpParticle[i] = dComIfGp_particle_set(l_eff_id[i], &mParticlePos, NULL, NULL);
                if (i == 0) {
                    mpParticle[0] = NULL;
                }
            }
        }
    }

    mpBrkAnm->setPlaySpeed(1.0f);
    mpBtkAnm[0]->setPlaySpeed(1.0f);
    mpBtkAnm[1]->setPlaySpeed(1.0f);
}

void daObjBossWarp_c::set_appear() {
    mpBrkAnm->setFrame(mpBrkAnm->getEndFrame());
    mpBtkAnm[1]->setFrame(mpBtkAnm[1]->getEndFrame());
    appear(0);
}

void daObjBossWarp_c::disappear(int param_0) {
    for (int i = 0; i < 4; i++) {
        if (mpParticle[i] != NULL) {
            mpParticle[i]->becomeInvalidEmitter();
            mpParticle[i]->quitImmortalEmitter();
            mpParticle[i] = NULL;
        }
    }
}

BOOL daObjBossWarp_c::checkDistance() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    f32 dist_xz = player->current.pos.absXZ(current.pos);
    f32 dist_y = player->current.pos.y - current.pos.y;
    if (dist_xz < 150.0f && dist_y < 200.0f && dist_y > -1.0f) {
        return true;
    } else {
        return false;
    }
}

int daObjBossWarp_c::execute() {
    if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) != 3) {
        u8 sw = getSwNo();
        if (sw == 0xff || fopAcM_isSwitch(this, sw)) {
            field_0x591 = true;
        } else {
            field_0x591 = false;
        }
        daPy_py_c* player = daPy_getPlayerActorClass();
        if (field_0x591) {
            appear(0);
            if (checkDistance()) {
                u8 scene = getSceneListNo();
                player->onSceneChangeArea(scene, 0xff, 0);
            }
        }
    } else {
        if (!isFirst() && !dComIfGp_evmng_checkStartDemo()) {
            field_0x595 = true;
            field_0x620 = current.pos;
        }
        event_proc_call();

        daMidna_c* midna = (daMidna_c*)daPy_py_c::getMidnaActor();
        if (field_0x595 && midna != NULL) {
            midna->onTagWaitPos(&field_0x620);
        }
        obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
        int level = getNowLevel();

        if (ystone != NULL && midna != NULL) {
            switch (field_0x599) {
            case 0:
                break;
            case 1:
                mDoMtx_stack_c::copy(midna->getMtxHairTop());
                cXyz hair_offset = l_hair_offset[level];
                mDoMtx_stack_c::transM(hair_offset);
                mDoMtx_stack_c::ZXYrotM(
                    cM_deg2s(l_hair_rotate[level].x),
                    cM_deg2s(l_hair_rotate[level].y),
                    cM_deg2s(l_hair_rotate[level].z)
                );
                mDoMtx_stack_c::scaleM(mScaleF, mScaleF, mScaleF);
                ystone->setBaseMtx(mDoMtx_stack_c::get());
                break;
            }
        }
    }

    mpBrkAnm->play();
    mpBtkAnm[0]->play();
    mpBtkAnm[1]->play();

    if (mScalingUp) {
        cLib_chaseF(&scale.y, 1.0f, 0.016f);
    }

    if (mpParticle[3] != NULL) {
        JGeometry::TVec3<f32> scale_vec;
        JGeometry::setTVec3f(&scale.x, &scale_vec.x);
        mpParticle[3]->setGlobalScale(scale_vec);
    }

    if (mpBrkAnm != NULL && mpBrkAnm->getFrame() != 0.0f) {
        mDoAud_seStartLevel(Z2SE_OBJ_MDN_ESCAPE_HOLE, &current.pos, 0, 0);
    }

    setBaseMtx();
    return 1;
}

void daObjBossWarp_c::event_proc_call() {
    typedef void (daObjBossWarp_c::*daObjBossWarp_action)();
    static daObjBossWarp_action l_func[11] = {
        &daObjBossWarp_c::actionWait,
        &daObjBossWarp_c::actionOrderEvent,
        &daObjBossWarp_c::actionEvent,
        &daObjBossWarp_c::actionWaitWarp,
        &daObjBossWarp_c::actionOrderWarpEvent,
        &daObjBossWarp_c::actionWarpEvent,
        &daObjBossWarp_c::actionDead,
        &daObjBossWarp_c::actionOrderChkEvent,
        &daObjBossWarp_c::actionChkEvent,
        &daObjBossWarp_c::actionOrderCancelEvent,
        &daObjBossWarp_c::actionCancelEvent,
    };
    (this->*l_func[mAction])();
}

void daObjBossWarp_c::actionWait() {
    setAction(ACT_ORDER_EVENT);
    fopAcM_orderOtherEventId(this, mBossClearEventId, mBossClearMapToolId, 0xffff, 3, 1);
    eventInfo.onCondition(2);
}

void daObjBossWarp_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        dComIfGp_event_setTalkPartner(fopAcM_SearchByName(PROC_OBJ_YSTONE));
        setAction(ACT_EVENT);
        mStaffId = dComIfGp_evmng_getMyStaffId("WARPHOLE", NULL, 0);
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mBossClearEventId, mBossClearMapToolId, 0xffff, 3, 1);
        eventInfo.onCondition(2);
    }
}

void daObjBossWarp_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mBossClearEventId)) {
        setAction(ACT_WAIT_WARP);
        dComIfGp_event_reset();
    } else {
        demoProc();
    }
}

void daObjBossWarp_c::actionWaitWarp() {
    if (checkDistance()) {
        setAction(ACT_ORDER_CHK_EVENT);
        fopAcM_orderOtherEventId(this, mWarpCheckEventId, 0xff, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjBossWarp_c::actionOrderWarpEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACT_WARP_EVENT);
        mStaffId = dComIfGp_evmng_getMyStaffId("WARPHOLE", NULL, 0);
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mBossWarpInEventId, mBossWarpInMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjBossWarp_c::actionWarpEvent() {
    if (!dComIfGp_evmng_endCheck(mBossWarpInEventId)) {
        demoProc();
    }
}

void daObjBossWarp_c::actionDead() {
    /* empty function */
}

void daObjBossWarp_c::actionOrderChkEvent() {
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACT_CHK_EVENT);
        mStaffId = dComIfGp_evmng_getMyStaffId("WARPHOLE", NULL, 0);
        attention_info.position = midna->attention_info.position;
        attention_info.position.y += 100.0f;
        eyePos = attention_info.position;
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mWarpCheckEventId, 0xff, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjBossWarp_c::actionChkEvent() {
    if (dComIfGp_evmng_endCheck(mWarpCheckEventId)) {
        dComIfGp_event_reset();
        if (mWarpChoice == 0) {
            setAction(ACT_ORDER_WARP_EVENT);
            fopAcM_orderOtherEventId(this, mBossWarpInEventId, mBossWarpInMapToolId, 0xffff, 0, 1);
            eventInfo.onCondition(2);
        } else if (mWarpChoice == 1) {
            setAction(ACT_ORDER_CANCEL_EVENT);
            fopAcM_orderOtherEventId(this, mWarpCancelEventId, 0xff, 0xffff, 0, 1);
            eventInfo.onCondition(2);
        }
    } else {
        demoProc();
    }
}

void daObjBossWarp_c::actionOrderCancelEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACT_CANCEL_EVENT);
        mStaffId = dComIfGp_evmng_getMyStaffId("WARPHOLE", NULL, 0);
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mWarpCancelEventId, 0xff, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjBossWarp_c::actionCancelEvent() {
    if (dComIfGp_evmng_endCheck(mWarpCancelEventId)) {
        dComIfGp_event_reset();
        setAction(ACT_WAIT_WARP);
    } else {
        demoProc();
    }
}

int daObjBossWarp_c::demoProc() {
    static char* action_table[15] = {
        "WAIT",
        "APPEAR",
        "DISAPPEAR",
        "SCENE_CHG",
        "STONE_FALL",
        "STONE_MIDNA",
        "WALK_TARGET1",
        "APPEAR_END",
        "STONE_DELETE",
        "STONE_PUTAWAY",
        "WCHECK",
        "SETPOS",
        "SCALING",
        "STONE_SCALE",
        "HEART_MOVE",
    };

    static int const l_dangeon_clr_bitNo[9] = {
        0x37, /* dSv_event_flag_c::M_022 - Forest Temple - Forest Temple clear (Midna creates warp hole) */
        0x40, /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
        0x4E, /* dSv_event_flag_c::M_045 - Lakebed Temple - Lakebed Temple clear */
        0x109, /* dSv_event_flag_c::F_0265 - Arbiter's Grounds - Arbiter's Grounds clear */
        0x10A, /* dSv_event_flag_c::F_0266 - Snowpeak Ruins - Snowpeak Ruins clear */
        0x10B, /* dSv_event_flag_c::F_0267 - Temple of Time - Temple of Time clear */
        0x10C,
        -1,
        -1
    };

    daPy_py_c* player = daPy_getPlayerActorClass();
    obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
    cXyz unused(100000.0f, 0.0f, 0.0f);
    daMidna_c* midna = (daMidna_c*)daPy_py_c::getMidnaActor();
    daObjLife_c* life_container = (daObjLife_c*)fopAcM_SearchByName(PROC_Obj_LifeContainer);
    cXyz life_pos(900.0f, 0.0f, 0.0f);
    int act_idx = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 15, 0, 0);

    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch (act_idx) {
        case 0: { // WAIT
            int* timer = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
            if (timer == NULL) {
                mTimer = 1;
            } else {
                mTimer = *timer;
            }
            break;
        }
        case 1: { // APPEAR
            int level = getNowLevel();
            if (level >= 0 && level < 10 && l_dangeon_clr_bitNo[level] > 0) {
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[l_dangeon_clr_bitNo[level]]);
            }
            appear(0);
            break;
        }
        case 2:  // DISAPPEAR
            disappear(0);
            break;
        case 3:  // SCENE_CHG
            int scene;
            if (isFirst()) {
                scene = getSceneListNo();
            } else {
                scene = getSceneListNo();
            }
            player->onSceneChangeArea(scene, 0xff, 0);
            break;
        case 4:  // STONE_FALL
            if (ystone != NULL) {
                mYstonePos = ystone->current.pos;
                if (getNowLevel() < 3) {
                    mYstonePos.y += 50.0f;
                } else if (getNowLevel() == 5) {
                    mYstonePos.y = player->current.pos.y + 270.0f + 120.0f;
                    ystone->setCurrentPos(mYstonePos);
                } else {
                    mYstonePos.y += 120.0f;
                }
            }
            if (getNowLevel() < 3) {
                mYstoneTargetPos.set(0.0f, 145.0f, 74.0f);
            } else {
                mYstoneTargetPos.set(0.0f, 175.0f, 54.0f);
            }
            mDoMtx_stack_c::transS(player->current.pos);
            mDoMtx_stack_c::YrotM(player->shape_angle.y);
            mDoMtx_stack_c::multVec(&mYstoneTargetPos, &mYstoneTargetPos);
            mYstonePos.x = mYstoneTargetPos.x;
            mYstonePos.z = mYstoneTargetPos.z;
            switch (getNowLevel()) {
            case 0:
                dComIfGs_onCollectCrystal(0);
                break;
            case 1:
                dComIfGs_onCollectCrystal(1);
                break;
            case 2:
                dComIfGs_onCollectCrystal(2);
                break;
            case 4:
                dComIfGs_onCollectMirror(1);
                break;
            case 5:
                dComIfGs_onCollectMirror(2);
                break;
            case 6:
                dComIfGs_onCollectMirror(3);
                break;
            }
            mCounter = 0;
            break;
        case 5:  // STONE_MIDNA
            mCounter = 0;
            break;
        case 9:  // STONE_PUTAWAY
            mCounter = 0;
            break;
        case 6:  // WALK_TARGET1
            dComIfGp_evmng_setGoal(&current.pos);
            break;
        case 7:  // APPEAR_END
            break;
        case 8:  // STONE_DELETE
            break;
        case 10:  // WCHECK
            mMsgFlow.init(this, 0xbc0, 0, NULL);
            break;
        case 11:  // SETPOS
            setGoal();
            break;
        case 12:  // SCALING
            mScalingUp = true;
            break;
        case 13:  // STONE_SCALE
            break;
        case 14:  // HEART_MOVE
            if (life_container != NULL) {
                life_container->setPos(life_pos);
            }
            break;
        }
    }

    switch (act_idx) {
    case 0:  // WAIT
        if (cLib_calcTimer(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 1:  // APPEAR
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    case 2:  // DISAPPEAR
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    case 3:  // SCENE_CHG
        break;
    case 4:  // STONE_FALL
        if (ystone != NULL) {
            f32 dist = cLib_addCalc(&mYstonePos.y, mYstoneTargetPos.y, 0.05f, 2.0f, 0.1f);
            if (dist < 40.0f) {
                mCounter++;
                if (ystone->getMirrorMode() == 2) {
                    ystone->setMirrorMode(3);
                }
            }
            if (dist == 0.0f && mCounter > 50) {
                dComIfGp_evmng_cutEnd(mStaffId);
            }
            ystone->setCurrentPos(mYstonePos);
        }
        break;
    case 5:  // STONE_MIDNA
        mCounter++;
        if (mCounter == 12) {
            field_0x599 = 1;
        }
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    case 9:  // STONE_PUTAWAY
        mCounter++;
        if (cLib_chaseF(&mScaleF, 0.0f, 0.033f)) {
            fopAcM_delete(fopAcM_GetID(ystone));
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 7:  // APPEAR_END
        field_0x595 = true;
        field_0x620 = midna->current.pos;
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    case 10:  // WCHECK
        if (mMsgFlow.doFlow(this, NULL, 0)) {
            dComIfGp_evmng_cutEnd(mStaffId);
        } else if (mMsgFlow.isSelectMessage()) {
            mWarpChoice = mMsgFlow.getSelectNum();
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }

    return 1;
}

void daObjBossWarp_c::setGoal() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz delta = player->current.pos - current.pos;
    cXyz goal = current.pos + delta.normalizeZP() * 250.0f;
    dComIfGp_evmng_setGoal(&goal);
}

int daObjBossWarp_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mpBrkAnm->entry(mpModel->getModelData());
    mpBtkAnm[0]->entry(mpModel->getModelData());
    mpBtkAnm[1]->entry(mpModel->getModelData());
    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daObjBossWarp_c::_delete() {
    disappear(0);
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static int daObjBossWarp_Draw(daObjBossWarp_c* i_this) {
    return i_this->draw();
}

static int daObjBossWarp_Execute(daObjBossWarp_c* i_this) {
    return i_this->execute();
}

static int daObjBossWarp_Delete(daObjBossWarp_c* i_this) {
    return i_this->_delete();
}

static cPhs__Step daObjBossWarp_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjBossWarp_c*>(i_this)->create();
}

static actor_method_class l_daObjBossWarp_Method = {
    (process_method_func)daObjBossWarp_Create,
    (process_method_func)daObjBossWarp_Delete,
    (process_method_func)daObjBossWarp_Execute,
    (process_method_func)NULL,
    (process_method_func)daObjBossWarp_Draw,
};

actor_process_profile_definition g_profile_Obj_BossWarp = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_BossWarp,
    &g_fpcLf_Method.base,
    sizeof(daObjBossWarp_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x2DA,
    &l_daObjBossWarp_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
