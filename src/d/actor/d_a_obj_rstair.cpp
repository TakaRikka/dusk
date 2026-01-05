/**
 * @file d_a_obj_rstair.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rstair.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_graphic.h"

void daObjRotStair_c::initBaseMtx() {
    mWaterModels[0]->setBaseScale(scale);
    mWaterModels[1]->setBaseScale(scale);
    mModel->setBaseScale(scale);
    setBaseMtx();
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mWaterModels[0]->setBaseTRMtx(mDoMtx_stack_c::get());
    mWaterModels[1]->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObjRotStair_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::YrotM(mAdditionalRotY);
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mWaterModels[0]->setBaseTRMtx(mDoMtx_stack_c::get());
    mWaterModels[1]->setBaseTRMtx(mDoMtx_stack_c::get());
}

static u32 const l_wtr_bmd[2] = {
    6, 7,
};

static u32 const l_wtr_btk[2] = {
    10, 11
};

static s16 const l_target_angleY[8] = {
    0x7FFF, 0x4000, 0x0000, -0x4000, 0x0000, -0x4000, 0x7FFF, 0x4000,
};

int daObjRotStair_c::Create() {
    for (int i = 0; i < 4; i = i + 1) {
        mIsSw[i] = fopAcM_isSwitch(this, i + getSwNo());
    }
    field_0x5e3 = -1;
    int angleIndex = -1;
    for (int i = 0; i < 4; i++) {
        if (fopAcM_isSwitch(this, i + getSwNo())) {
            OS_REPORT("回転階段：方向<%d>\n", i);
            angleIndex = i;
            field_0x5e3 = i;
            break;
        }
    }
    if (angleIndex >= 0) {
        shape_angle.y = l_target_angleY[angleIndex];
    }
    init_modeWait();
    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mModel->getModelData());
    mMapToolId = getEventID();
    mEventId = dComIfGp_getEventManager().getEventIdx(this, mMapToolId);
    mWaterBgw->Move();
    setAction(ACTION_WAIT);
    return 1;
}

static char* l_arcName = "K_spkai00";

static Vec l_water_check_offset = {
    0.0f, 2000.0f, -2000.0f,
};

int daObjRotStair_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(l_arcName, 5);
    JUT_ASSERT(215, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }
    for (int i = 0; i < 2; i++) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_wtr_bmd[i]);
        JUT_ASSERT(232, modelData != NULL);
        mWaterModels[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x19000284);
        if (mWaterModels[i] == NULL) {
            return 0;
        }
        J3DAnmTextureSRTKey* pbtk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(l_arcName, l_wtr_btk[i]);
        JUT_ASSERT(245, pbtk != NULL);
        mBtks[i] = new mDoExt_btkAnm();
        if (mBtks[i] == NULL || mBtks[i]->init(modelData, pbtk, 1, 2, 1.0f, 0, -1) == 0) {
            return 0;
        }
    }
    mWaterBgw = new dBgW();
    if (mWaterBgw == NULL || mWaterBgw->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName, 0xf), 1, &mBgMtx) != 0) {
        mWaterBgw = NULL;
        return 0;
    }
    return 1;
}

int daObjRotStair_c::create1st() {
    if (mInit == 0) {
        field_0x5c8 = home.angle.x;
        mSwNo = home.angle.z;
        shape_angle.x = 0;
        current.angle.x = 0;
        home.angle.x = 0;
        shape_angle.z = 0;
        current.angle.z = 0;
        home.angle.z = 0;
        mInit = 1;
    }
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    if (rv == cPhs_COMPLEATE_e) {
        rv = MoveBGCreate(l_arcName, 0xe,
                             dBgS_MoveBGProc_TypicalRotY, 0xd1c0, NULL);
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
        OS_REPORT("RSTAIR PARAM : %x \n", fopAcM_GetParam(this));
    }
    return rv;
}

int daObjRotStair_c::Execute(Mtx** ppMtx) {
    move_proc_call();
    event_proc_call();
    *ppMtx = &mBgMtx;
    setBaseMtx();
    if (mWaterModelOn) {
        mWaterBgw->Move();
    }
    for (int i = 0; i < 4; i = i + 1) {
        mIsSw[i] = fopAcM_isSwitch(this, i + getSwNo());
    }
    mBtks[0]->play();
    mBtks[1]->play();
    return 1;
}

void daObjRotStair_c::move_proc_call() {
    static daObjRotStair_c::modeFunc l_func[2] = {
        &daObjRotStair_c::modeWait,
        &daObjRotStair_c::modeRotate,
    };
    (this->*(l_func[mMode]))();
}

void daObjRotStair_c::init_modeWait() {
    s8 dVar3 = field_0x5e3;
    if (dVar3 >= 4) {
        dVar3 -= 4;
    }
    if ((dVar3 == 3 && fopAcM_isSwitch(this, getSwNo3())) || (dVar3 == 1 && fopAcM_isSwitch(this, getSwNo2())))
    {
        onWaterModel();
    } else {
        offWaterModel();
    }
    mMode = MODE_WAIT;
}

void daObjRotStair_c::modeWait() {
    s8 dVar3 = field_0x5e3;
    if (dVar3 >= 4) {
        dVar3 -= 4;
    }
    if ((dVar3 == 3 && fopAcM_isSwitch(this, getSwNo3())) || (dVar3 == 1 && fopAcM_isSwitch(this, getSwNo2())))
    {
        onWaterModel();
    } else {
        offWaterModel();
    }

    int changeSw = checkChangeSw();
    if (changeSw >= 0) {
        field_0x5e2 = true;
        field_0x5e3 = changeSw;
        mTargetAngle = l_target_angleY[field_0x5e3];
        offSwitch(field_0x5e3);
        init_modeRotate();
    }
}

void daObjRotStair_c::init_modeRotate() {
    s8 dVar3 = field_0x5e3;
    if (dVar3 >= 4) {
        dVar3 -= 4;
    }
    if ((dVar3 == 3 && fopAcM_isSwitch(this, getSwNo3())) || (dVar3 == 1 && fopAcM_isSwitch(this, getSwNo2())))
    {
        onWaterModel();
    } else {
        offWaterModel();
    }

    mAngleStep = 0;
    mStepTarget = 300;
    mMode = MODE_ROTATE;
}

void daObjRotStair_c::modeRotate() {
    if (abs(mTargetAngle - shape_angle.y) < mAngleStep) {
        dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        fopAcM_seStart(this, Z2SE_OBJ_ROLLSTAIR_STOP, 0);
        shape_angle.y = mTargetAngle;
        mAngleStep = 0;
        init_modeWait();
    } else {
        shape_angle.y += mAngleStep;
    }
    cLib_addCalcAngleS(&mAngleStep, mStepTarget, 0x14, 0x32, 10);
    fopAcM_seStartLevel(this, Z2SE_OBJ_ROLLSTAIR_ROLL, 0);
}

void daObjRotStair_c::event_proc_call() {
    static daObjRotStair_c::actionFunc l_func[4] = {
        &daObjRotStair_c::actionWait,
        &daObjRotStair_c::actionOrderEvent,
        &daObjRotStair_c::actionEvent,
        &daObjRotStair_c::actionDead,
    };

    (this->*(l_func[mAction]))();
}

void daObjRotStair_c::actionWait() {
    if (mMapToolId != 0xff && field_0x5e2) {
        field_0x5e2 = false;
        setAction(ACTION_ORDER_EVENT);
        fopAcM_orderOtherEventId(this, mEventId, mMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjRotStair_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_EVENT);
    } else {
        fopAcM_orderOtherEventId(this, mEventId, mMapToolId, 0xffff, 0, 1);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }
}

void daObjRotStair_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventId)) {
        setAction(ACTION_DEAD);
        dComIfGp_event_reset();
    }
}

void daObjRotStair_c::actionDead() {
}

int daObjRotStair_c::checkChangeSw() {
    for (int i = 0; i < 4; i++) {
        if (mIsSw[i] != fopAcM_isSwitch(this, i + getSwNo())) {
            return i;
        }
    }
    return -1;
}

void daObjRotStair_c::offSwitch(int idx) {
    for (int i = 0; i < 4; i++) {
        int swNo = getSwNo() + i;
        if (idx != i) {
            fopAcM_offSwitch(this, swNo);
        }
    }
}

void daObjRotStair_c::onWaterModel() {
    if (!mWaterModelOn) {
        dComIfG_Bgsp().Regist(mWaterBgw, this);
        mWaterModelOn = true;
    }
}

void daObjRotStair_c::offWaterModel() {
    if (mWaterModelOn) {
        if (mWaterBgw->ChkUsed()) {
            dComIfG_Bgsp().Release(mWaterBgw);
        }
        mWaterModelOn = false;
    }
}

int daObjRotStair_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    if (mWaterModelOn) {
        g_env_light.setLightTevColorType_MAJI(mWaterModels[0], &tevStr);
        g_env_light.setLightTevColorType_MAJI(mWaterModels[1], &tevStr);
        mBtks[0]->entry(mWaterModels[0]->getModelData());
        mBtks[1]->entry(mWaterModels[1]->getModelData());
        dComIfGd_setXluListDarkBG();
        mDoExt_modelUpdateDL(mWaterModels[0]);
        dComIfGd_setList();
        J3DModelData* modelData = mWaterModels[1]->getModelData();
        J3DMaterial* material = modelData->getMaterialNodePointer(0);
        dComIfGd_setListInvisisble();
        if (material->getTexGenBlock()->getTexMtx(0) != 0) {
            J3DTexMtxInfo* texMtxInfo = &material->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
            if (texMtxInfo != NULL) {
                Mtx lightMtx;
                C_MTXLightPerspective(lightMtx, dComIfGd_getView()->fovy,
                                      dComIfGd_getView()->aspect, 1.0f, 1.0f, -0.01f, 0);
                #if WIDESCREEN_SUPPORT
                mDoGph_gInf_c::setWideZoomLightProjection(lightMtx);
                #endif
                texMtxInfo->setEffectMtx(lightMtx);
                modelData->simpleCalcMaterial(0, (MtxP)j3dDefaultMtx);
            }
        }
        mDoExt_modelUpdateDL(mWaterModels[1]);
    }
    dComIfGd_setList();
    return 1;
}

int daObjRotStair_c::Delete() {
    if (mWaterBgw != NULL && mWaterBgw->ChkUsed()) {
        dComIfG_Bgsp().Release(mWaterBgw);
    }
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjRotStair_create1st(daObjRotStair_c* i_this) {
    fopAcM_ct(i_this, daObjRotStair_c);
    return i_this->create1st();
}

static int daObjRotStair_MoveBGDelete(daObjRotStair_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjRotStair_MoveBGExecute(daObjRotStair_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjRotStair_MoveBGDraw(daObjRotStair_c* i_this) {
    return i_this->Draw();
}


static actor_method_class daObjRotStair_METHODS = {
    (process_method_func)daObjRotStair_create1st,
    (process_method_func)daObjRotStair_MoveBGDelete,
    (process_method_func)daObjRotStair_MoveBGExecute,
    NULL,
    (process_method_func)daObjRotStair_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_RotStair = {
  fpcLy_CURRENT_e,         // mLayerID
  3,                       // mListID
  fpcPi_CURRENT_e,         // mListPrio
  PROC_Obj_RotStair,       // mProcName
  &g_fpcLf_Method.base,   // sub_method
  sizeof(daObjRotStair_c), // mSize
  0,                       // mSizeOther
  0,                       // mParameters
  &g_fopAc_Method.base,    // sub_method
  613,                     // mPriority
  &daObjRotStair_METHODS,  // sub_method
  0x00040100,              // mStatus
  fopAc_ACTOR_e,           // mActorType
  fopAc_CULLBOX_CUSTOM_e,  // cullType
};
