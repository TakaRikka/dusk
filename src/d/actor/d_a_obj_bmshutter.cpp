/**
* @file d_a_obj_bmshutter.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_bmshutter.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"

static const char* l_arcName = "K_saku00";

static const cull_box l_cull_box = {{-200.f, -100.f, -200.f}, {200.f, 100.f, 200.f}};
static const Vec l_check_area[4] = {
    -250.0, 0.0, -100.0,  // min
    200.0,  0.0, -100.0,  // unknown
    200.0,  0.0, 100.0,   // max
    -250.0, 0.0, 100.0,   // unknown
};

void daObjBmSh_c::initBaseMtx() {
    mModel->setBaseScale(scale);

    if (fopAcM_isSwitch(this, getSwbit())) {
        mHeight = 400.f;
    } else {
        mHeight = 0.f;
    }

    setBaseMtx();
}

void daObjBmSh_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::transM(mHeight, 0.0, 0.0);
    mDoMtx_stack_c::ZXYrotM(mRotation.x, 0, mRotation.z);

    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    PSMTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    return;
}

int daObjBmSh_c::Create() {
    initBaseMtx();

    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mModel->getModelData());

    mEvent = getEvNo();
    mEventIdx = dComIfGp_getEventManager().getEventIdx(this, mEvent);

    if (fopAcM_isSwitch(this, getSwbit())) {
        setAction(daObjBmSh_Action_DEAD_e);
        setMoveMode(daObjBmSh_MoveMode_END_e);
    } else {
        setAction(daObjBmSh_Action_WAIT_e);
        setMoveMode(daObjBmSh_MoveMode_WAIT_e);
    }

    if (mEvent == 0xff) {
        setAction(daObjBmSh_Action_DEAD_e);
    }

    return 1;
}

int daObjBmSh_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 4);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return false;
    }
    return true;
}

int daObjBmSh_c::create() {
    fopAcM_ct(this, daObjBmSh_c);

    if (!mIsInitialized) {
        mHomeX = home.angle.x;
        mHomeZ = home.angle.z;
        mIsInitialized = true;
        home.angle.z = 0;
        home.angle.x = 0;
        shape_angle.z = 0;
        shape_angle.x = 0;
        current.angle.z = 0;
        current.angle.x = 0;
    }

    int ret = dComIfG_resLoad(&mPhase, l_arcName);
    if (ret == cPhs_COMPLEATE_e) {
        ret = MoveBGCreate(l_arcName, 7, dBgS_MoveBGProc_TypicalRotY, 0x12a0, NULL);
        if (ret == cPhs_ERROR_e) {
            return ret;
        }
    }
    return ret;
}

int daObjBmSh_c::Execute(Mtx** param_0) {
    daPy_py_c* player = dComIfGp_getLinkPlayer();
    if (player->checkFrontRollCrash()) {
        cXyz min = l_check_area[0];
        cXyz max = l_check_area[2];

        if (dLib_checkActorInRectangle(player, this, &min, &max)) {
            mShakeIntensity = 120.00001f;
            mSoundPosition.set(0.0f, 150.0f, 0.0f);
            mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
            mDoMtx_stack_c::YrotM(shape_angle.y);
            mDoMtx_stack_c::multVec(&mSoundPosition, &mSoundPosition);
            mDoAud_seStart(Z2SE_OBJ_BOOMSHTR_ATTACK, &mSoundPosition, 0, 0);
        }
    }
    event_proc_call();
    mode_proc_call();
    *param_0 = &mBgMtx;
    setBaseMtx();
    mTickCounter++;
    mCurrentlyAnySw = checkAnySw();
    mCurrentlyMySw = checkMySw();
    return 1;
}

int daObjBmSh_c::checkSw() {
    u32 swbit2 = getSwbit2();
    u8 nSwitches = getSwNum();

    for (u8 iSwitch = 0; iSwitch < nSwitches; iSwitch++) {
        if (!fopAcM_isSwitch(this, swbit2 + iSwitch)) {
            return false;
        }
    }
    return true;
}

int daObjBmSh_c::checkAnySw() {
    u32 swbit2 = getSwbit2();
    u8 nSwitches = getSwNum();

    for (u8 iSwitch = 0; iSwitch < nSwitches; iSwitch++) {
        if (fopAcM_isSwitch(this, swbit2 + iSwitch)) {
            return true;
        }
    }
    return false;
}

int daObjBmSh_c::checkMySw() {
    return fopAcM_isSwitch(this, getSwbit3());
}

void daObjBmSh_c::event_proc_call() {
    typedef void (daObjBmSh_c::*actionFunc)();
    static actionFunc l_func[] = {
        &daObjBmSh_c::actionWait,
        &daObjBmSh_c::actionOrderEvent,
        &daObjBmSh_c::actionEvent,
        &daObjBmSh_c::actionDead,
    };
    (this->*l_func[mAction])();
}

void daObjBmSh_c::actionWait() {
    if (checkSw()) {
        setAction(daObjBmSh_Action_ORDER_EVENT_e);
        fopAcM_orderOtherEventId(this, mEventIdx, mEvent, 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjBmSh_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(daObjBmSh_Action_EVENT_e);
    } else {
        fopAcM_orderOtherEventId(this, mEventIdx, mEvent, 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjBmSh_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventIdx)) {
        setAction(daObjBmSh_Action_DEAD_e);
        dComIfGp_event_reset();
    }
    return;
}

void daObjBmSh_c::actionDead() {
    /* empty function */
}

void daObjBmSh_c::mode_proc_call() {
    typedef void (daObjBmSh_c::*mode_func)();
    static mode_func l_func[] = {
        &daObjBmSh_c::modeWait,
        &daObjBmSh_c::modeMove,
        &daObjBmSh_c::modeEnd,
    };

    (this->*l_func[mMoveMode])();

    mRotation.x = mShakeIntensity * cM_scos((mTickCounter * 0x3000) & 0xfff8U);
    mRotation.z = mShakeIntensity * cM_ssin((mTickCounter * 0x3000) & 0xfff8U);
    cLib_addCalc0(&mShakeIntensity, 1.0f / 20.0f, 10.0f);
}

void daObjBmSh_c::modeWait() {
    checkAnySw();
    int checkedSw = checkSw();

    if (checkMySw() != 0 && checkedSw == 0 && mCurrentlyMySw == 0) {
        mShakeIntensity = 400.0f;
        mSoundPosition.set(0.0f, 150.0f, 0.0f);
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::multVec(&mSoundPosition, &mSoundPosition);
        mDoAud_seStart(Z2SE_OBJ_BOOMSHTR_GATA, &mSoundPosition, 0, 0);
    }
    if (checkedSw != 0) {
        fopAcM_onSwitch(this, getSwbit());
        setMoveMode(daObjBmSh_MoveMode_MOVE_e);
        mSoundPosition.set(-220.0, 150.0, 0.0);

        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::multVec(&mSoundPosition, &mSoundPosition);

        u32 evNo = getEvNo();
        if (!getShtrNum()) {
            mDoAud_seStart(Z2SE_OBJ_BOOMSHTR_S_OPEN, &mSoundPosition, 0, 0);
        } else if ((evNo & 0xff) != 0xff) {
            mDoAud_seStart(Z2SE_OBJ_BOOMSHTR_OPEN, &mSoundPosition, 0, 0);
        }
    }
}

void daObjBmSh_c::modeMove() {
    cLib_addCalcAngleS2(&mRotation.x, 0, 5, 256);
    cLib_addCalcAngleS2(&mRotation.z, 0, 5, 256);
    if (cLib_addCalc(&mHeight, 400.0f, 0.05f, 10.0f, 2.0f) == 0.0f) {
        setMoveMode(daObjBmSh_MoveMode_END_e);
    }
    return;
}

void daObjBmSh_c::modeEnd() {
    /* empty function */
}

int daObjBmSh_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel->mModelData, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();

    return 1;
}

int daObjBmSh_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

int daObjBmSh_Draw(daObjBmSh_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjBmSh_c*>(i_this)->Draw();
}

int daObjBmSh_Execute(daObjBmSh_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGExecute();
}

int daObjBmSh_Delete(daObjBmSh_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<dBgS_MoveBgActor*>(i_this)->MoveBGDelete();
}

int daObjBmSh_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjBmSh_c*>(i_this)->create();
}

static actor_method_class l_daObjBmSh_Method = {
    (process_method_func)daObjBmSh_Create,
    (process_method_func)daObjBmSh_Delete,
    (process_method_func)daObjBmSh_Execute,
    NULL,
    (process_method_func)daObjBmSh_Draw,
};

actor_process_profile_definition g_profile_Obj_BoomShutter = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_BoomShutter,    // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjBmSh_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    449,                     // mPriority
    &l_daObjBmSh_Method,     // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
