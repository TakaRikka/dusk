/**
 * @file d_a_obj_avalanche.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_avalanche.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"

void daObjAvalanche_c::initBaseMtx() {
    mModel->setBaseScale(scale);
    setBaseMtx();
}

static u32 const l_dzb[2] = {
    13, 12,
};

static Vec const l_check_pos[2] = {
    {-300.0f, 0.0f, 0.0f},
    {300.0f, 0.0f, 300.0f},
};

void daObjAvalanche_c::setBaseMtx() {
    mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::YrotM(0);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    mDoMtx_stack_c::transS(0.0f, field_0x5e4, 0.0f);
    mDoMtx_stack_c::YrotM(0);
    MTXCopy(mDoMtx_stack_c::get(), mMtx2);
}

static char* l_arcName = "V_Yuki";

static char* l_evName = "SNOW_COLLAPSE";

static char* l_staffName = "Obj_ava";

static cull_box l_cull_box = {
    {-500.0f, -500.0f, -700.0f},
    {500.0f, 1000.0f, 700.0f},
};

int daObjAvalanche_c::Create() {
    if (fopAcM_isSwitch(this, getSwbit())) {
        field_0x5e4 = 0.0f;
        if (mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
        if (mpBgW2 != NULL) {
            dComIfG_Bgsp().Regist(mpBgW2, this);
        }
        mBckAnm->setFrame(mBckAnm->getEndFrame());
        setAction(ACTION_DEAD);
    } else {
        field_0x5e4 = -400.0f;
        if (mpBgW2 != NULL) {
            dComIfG_Bgsp().Regist(mpBgW2, this);
        }
        mBckAnm->setFrame(0.0f);
        setAction(ACTION_WAIT);
    }
    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z, l_cull_box.max.x,
                          l_cull_box.max.y, l_cull_box.max.z);
    if (mpBgW2 != 0) {
        mpBgW2->Move();
    }
    mMapToolId = getEvId();
    eventInfo.setArchiveName(l_arcName);
    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, l_evName, 0xff);
    return 1;
}

int daObjAvalanche_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 9);
    JUT_ASSERT(233, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (mModel == NULL) {
        return 0;
    }
    J3DAnmTransform* anm =
        (J3DAnmTransform*)dComIfG_getObjectRes(l_arcName, 6);
    JUT_ASSERT(247, anm != NULL);
    mBckAnm = new mDoExt_bckAnm();
    if (mBckAnm == NULL || !mBckAnm->init(anm, 1, 0, 1.0, 0, -1, false)) {
        return 0;
    }
    mBckAnm->setPlaySpeed(0.0f);
    mpBgW2 = new dBgW();
    if (mpBgW2 != NULL && !mpBgW2->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, l_dzb[1]), 1, &mMtx2)) {
        mpBgW2->SetCrrFunc(dBgS_MoveBGProc_Trans);
    } else {
        mpBgW2 = NULL;
        return 0;
    }
    return 1;
}

int daObjAvalanche_c::create1st() {
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        rv = MoveBGCreate(l_arcName, 13, dBgS_MoveBGProc_Trans, 0x2920, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
    }
    return rv;
}

int daObjAvalanche_c::Execute(Mtx** param_1) {
    event_proc_call();
    *param_1 = &mBgMtx;
    setBaseMtx();
    return 1;
}

int daObjAvalanche_c::checkCollapse() {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (!daPy_py_c::checkNowWolf()) {
        return 0;
    }
    if (!player->checkWolfAttackReverse() && !player->checkFrontRollCrash()) {
        return 0;
    }
    cXyz local_1c;
    cXyz local_28;
    local_28 = l_check_pos[0];
    local_1c = l_check_pos[1];
    cXyz playerDist = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotM(-shape_angle.y);
    mDoMtx_stack_c::multVec(&playerDist, &playerDist);
    if (player->current.pos.y > current.pos.y && player->current.pos.y < current.pos.y + 300.0f &&
        playerDist.x > local_28.x && playerDist.x < local_1c.x && playerDist.z > local_28.z &&
        playerDist.z < local_1c.z)
    {
        return 1;
    } else {
        return 0;
    }
}

int daObjAvalanche_c::move() {
    int rv = 0;
    if (cLib_chaseF(&field_0x5e4, 0.0f, 40.0f)) {
        rv = 1;
    }
    setBaseMtx();
    if (mpBgW2 != NULL) {
        mpBgW2->Move();
    }
    return rv;
}

void daObjAvalanche_c::event_proc_call() {
    typedef void (daObjAvalanche_c::*actionFunc)();
    static actionFunc l_func[4] = {
        &daObjAvalanche_c::actionWait,
        &daObjAvalanche_c::actionOrderEvent,
        &daObjAvalanche_c::actionEvent,
        &daObjAvalanche_c::actionDead,
    };
    (this->*l_func[mAction])();
}

void daObjAvalanche_c::actionWait() {
    if (checkCollapse()) {
        setAction(ACTION_ORDER_EVENT);
        fopAcM_orderOtherEventId(this, mEventIdx, mMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjAvalanche_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_EVENT);
        mStaffId = dComIfGp_evmng_getMyStaffId(l_staffName, 0, 0);
        if (mpBgW != NULL && mpBgW->ChkUsed()) {
            dComIfG_Bgsp().Release(mpBgW);
        }
        demoProc();
    } else {
        fopAcM_orderOtherEventId(this, mEventIdx, mMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(2);
    }
}

void daObjAvalanche_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        setAction(ACTION_DEAD);
        dComIfGp_event_reset();
        fopAcM_onSwitch(this, getSwbit());
    } else {
        demoProc();
    }
}

void daObjAvalanche_c::actionDead() {
}

static char* action_table[2] = {
    "WAIT",
    "COLLAPSE",
};

int daObjAvalanche_c::demoProc() {
    daPy_getPlayerActorClass();
    int actIdx = dComIfGp_evmng_getMyActIdx(mStaffId, action_table, 2, 0, 0);
    if (dComIfGp_evmng_getIsAddvance(mStaffId)) {
        switch (actIdx) {
        case 0: {
            int* puVar3 = dComIfGp_evmng_getMyIntegerP(mStaffId, "Timer");
            if (puVar3 == NULL) {
                mTimer = 1;
            } else {
                mTimer = *puVar3;
            }
            break;
        }
        case 1:
            mBckAnm->setPlaySpeed(1.0f);
            eyePos = current.pos;
            fopAcM_seStart(this, Z2SE_OBJ_SNOW_SLIDE, 0);
            setEffect();
            OS_REPORT("崩れる雪：ＳＥセット\n"); // Avalanche: SE Set
            break;
        }
    }
    switch (actIdx) {
    case 0:
        if (cLib_calcTimer(&mTimer) == 0) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    case 1:
        if (mBckAnm->play() && move()) {
            dComIfGp_evmng_cutEnd(mStaffId);
        }
        break;
    default:
        dComIfGp_evmng_cutEnd(mStaffId);
        break;
    }
    return 0;
}

void daObjAvalanche_c::setEffect() {
    cXyz acStack_18(0.0f, 0.0f, 0.0f);
    csXyz cStack_20(0, 0, 0);
    dComIfGp_particle_set(0x8c51, &acStack_18, &cStack_20, NULL, 0xff, NULL, 0xffffffff, NULL, NULL,
                          NULL);
}

int daObjAvalanche_c::Draw() {
    g_env_light.settingTevStruct(32, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mBckAnm->entry(mModel->getModelData());
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daObjAvalanche_c::Delete() {
    if (mpBgW2 != NULL && mpBgW2->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW2);
    }
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjAvalanche_create1st(daObjAvalanche_c* i_this) {
    fopAcM_ct(i_this, daObjAvalanche_c);
    return i_this->create1st();
}

static int daObjAvalanche_MoveBGDelete(daObjAvalanche_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjAvalanche_MoveBGExecute(daObjAvalanche_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjAvalanche_MoveBGDraw(daObjAvalanche_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjAvalanche_METHODS = {
    (process_method_func)daObjAvalanche_create1st,  (process_method_func)daObjAvalanche_MoveBGDelete,
    (process_method_func)daObjAvalanche_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjAvalanche_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Avalanche = {
    fpcLy_CURRENT_e,          // mLayerID
    3,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_Obj_Avalanche,       // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjAvalanche_c), // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    655,                      // mPriority
    &daObjAvalanche_METHODS,  // sub_method
    0x40000,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
