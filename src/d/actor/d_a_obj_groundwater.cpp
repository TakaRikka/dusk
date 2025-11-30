/**
 * @file d_a_obj_groundwater.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_groundwater.h"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_graphic.h"

static daGrdWater_HIO_c l_HIO;

static daGrdWater_c::modeFunc l_mode_func[5] = {
    &daGrdWater_c::modeWait,
    &daGrdWater_c::modeLevelUpA,
    &daGrdWater_c::modeLevelDownA,
    &daGrdWater_c::modeLevelUpB,
    &daGrdWater_c::modeLevelDownB,
};

daGrdWater_HIO_c::daGrdWater_HIO_c() {
    field_0x04 = 100;
    field_0x05 = 100;
    field_0x06 = 0;
    field_0x07 = 30;
    field_0x08 = 0.0f;
    field_0x0c = 0.0f;
    field_0x10 = 0.0f;
}

void daGrdWater_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mModel1->setBaseScale(scale);
    mModel1->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mModel2->setBaseScale(scale);
    mModel2->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daGrdWater_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Water", 18);
    JUT_ASSERT(189, modelData != NULL);
    mModel2 = mDoExt_J3DModel__create(modelData, 0x80000, 0x19000284);
    if (mModel2 == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* srtKey = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Water", 26);
    int res = mBtk1.init(modelData, srtKey, 1, 2, 1.0f, 0, -1);
    JUT_ASSERT(206, res == 1);
    J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 10);
    res = mBck2a.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(213, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 9);
    res = mBck2b.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(219, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 12);
    res = mBck2c.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(225, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 11);
    res = mBck2d.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(231, res == 1);
    J3DModelData* modelData2 = (J3DModelData*)dComIfG_getObjectRes("Water", 0x11);
    JUT_ASSERT(236, modelData2 != NULL);
    mModel1 = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000284);
    if (mModel1 == NULL) {
        return 0;
    }
    srtKey = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Water", 25);
    res = mBtk2.init(&modelData2->getMaterialTable(), srtKey, 1, 2, 1.0f, 0, -1);
    JUT_ASSERT(253, res == 1);
    J3DAnmTevRegKey* regKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Water", 21);
    res = mBrk1.init(modelData2, regKey, 1, 0, 1.0f, 0, -1);
    JUT_ASSERT(262, res == 1);
    regKey = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Water", 22);
    res = mBrk2.init(modelData2, regKey, 1, 0, 1.0f, 0, -1);
    JUT_ASSERT(270, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 8);
    res = mBck1a.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(277, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 7);
    res = mBck1b.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(283, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 14);
    res = mBck1c.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(289, res == 1);
    anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("Water", 13);
    res = mBck1d.init(anmTransform, 1, 0, 1.0f, 0, -1, false);
    JUT_ASSERT(295, res == 1);
    return 1;
}

int daGrdWater_c::create() {
    fopAcM_ct(this, daGrdWater_c);
    int rv = dComIfG_resLoad(&mPhase, "Water");
    if (rv == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("Water", 29, dBgS_MoveBGProc_Typical, 0x4030, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        } else {
            fopAcM_SetMtx(this, mModel2->getBaseTRMtx());
            field_0x6f3 = getSw();
            if (field_0x6f3 != 0xff) {
                field_0x6f0 = fopAcM_isSwitch(this, field_0x6f3);
                J3DModelData* modelData1 = mModel2->getModelData();
                J3DModelData* modelData2 = mModel1->getModelData();
                if (field_0x6f0 != 0) {
                    u8 sw2 = getSw2();
                    if (fopAcM_isSwitch(this, sw2)) {
                        mBck1d.setFrame(mBck1d.getEndFrame());
                        mBck2d.setFrame(mBck2d.getEndFrame());
                        mBck1d.entry(modelData2);
                        mBck2d.entry(modelData1);
                        mBrk2.setFrame(mBrk2.getEndFrame());
                        mBrk2.entry(modelData2);
                    } else {
                        mBck1a.setFrame(mBck1a.getEndFrame());
                        mBck2a.setFrame(mBck2a.getEndFrame());
                        mBrk1.setFrame(mBrk1.getEndFrame());
                        mBck1a.entry(modelData2);
                        mBrk1.entry(modelData2);
                        mBck2a.entry(modelData1);
                    }
                } else {
                    u8 sw2 = getSw2();
                    if (fopAcM_isSwitch(this, sw2)) {
                        mBck1c.setFrame(mBck1c.getEndFrame());
                        mBck2c.setFrame(mBck2c.getEndFrame());
                        mBrk2.setFrame(mBrk2.getEndFrame());
                        mBck1c.entry(modelData2);
                        mBrk2.entry(modelData2);
                        mBck2c.entry(modelData1);
                    } else {
                        mBck1b.setFrame(mBck1b.getEndFrame());
                        mBck2b.setFrame(mBck2b.getEndFrame());
                        mBck1b.entry(modelData2);
                        mBck2b.entry(modelData1);
                        mBrk1.setFrame(mBrk1.getEndFrame());
                        mBrk1.entry(modelData2);
                    }
                }
                mModel2->calcAnmMtx();
                mModel1->calcAnmMtx();
            }
            setBaseMtx();
            init_modeWait();
        }
    }
    return rv;
}

daGrdWater_c::daGrdWater_c() {
}

int daGrdWater_c::Execute(Mtx** pMtx) {
    mBtk1.play();
    mBtk2.play();
    if (field_0x6f3 != 0xff) {
        u8 prevSw = field_0x6f0;
        u8 sw = getSw();
        field_0x6f0 = fopAcM_isSwitch(this, sw);
        if (field_0x6f0 != prevSw) {
            if (field_0x6f0 != 0) {
                u8 sw2 = getSw2();
                if (fopAcM_isSwitch(this, sw2)) {
                    init_modeLevelDownB();
                } else {
                    init_modeLevelUpA();
                }
            } else {
                u8 sw2 = getSw2();
                if (fopAcM_isSwitch(this, sw2)) {
                    init_modeLevelUpB();
                } else {
                    init_modeLevelDownA();
                }
            }
        }
    }
    (this->*(l_mode_func[mMode]))();
    if (field_0x6f3 != 0xff) {
        J3DTransformInfo& transformInfo =
            mModel2->getModelData()->getJointNodePointer(0)->getTransformInfo();
        MtxP anmMtx = mModel2->getAnmMtx(0);
        mDoMtx_stack_c::transS(0.0f, anmMtx[1][3] - transformInfo.mTranslate.y, 0.0f);
        mDoMtx_stack_c::scaleM(scale.x, scale.y, scale.z);
        MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
        *pMtx = &mBgMtx;
    } else {
        *pMtx = &mModel2->getBaseTRMtx();
    }
    setBaseMtx();
    return 1;
}

void daGrdWater_c::init_modeWait() {
    mMode = WAIT;
}

void daGrdWater_c::modeWait() {
}

void daGrdWater_c::init_modeLevelUpA() {
    mBck1a.reset();
    mBck2a.reset();
    mBrk1.reset();
    mMode = LEVEL_UP_A;
}

void daGrdWater_c::modeLevelUpA() {
    int res1 = mBck1a.play();
    int res2 = mBck2a.play();
    int res3 = mBrk1.play();
    if (res1 == 1 && res2 == 1 && res3 == 1) {
        init_modeWait();
    }
}

void daGrdWater_c::init_modeLevelDownA() {
    mBck1b.reset();
    mBck2b.reset();
    mMode = LEVEL_DOWN_A;
}

void daGrdWater_c::modeLevelDownA() {
    int res1 = mBck1b.play();
    int res2 = mBck2b.play();
    if (res1 == 1 && res2 == 1) {
        init_modeWait();
    }
}

void daGrdWater_c::init_modeLevelUpB() {
    mBck1c.reset();
    mBck2c.reset();
    mBrk2.reset();
    mMode = LEVEL_UP_B;
}

void daGrdWater_c::modeLevelUpB() {
    int res1 = mBck1c.play();
    int res2 = mBck2c.play();
    int res3 = mBrk2.play();
    if (res1 == 1 && res2 == 1 && res3 == 1) {
        init_modeWait();
    }
}

void daGrdWater_c::init_modeLevelDownB() {
    mBck1d.reset();
    mBck2d.reset();
    mMode = LEVEL_DOWN_B;
}

void daGrdWater_c::modeLevelDownB() {
    int res1 = mBck1d.play();
    int res2 = mBck2d.play();
    if (res1 == 1 && res2 == 1) {
        init_modeWait();
    }
}

int daGrdWater_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel2, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel1, &tevStr);
    dKy_bg_MAxx_proc(mModel1);
    dKy_bg_MAxx_proc(mModel2);
    J3DModelData* modelData2 = mModel2->getModelData();
    mBtk1.entry(modelData2);
    J3DModelData* modelData1 = mModel1->getModelData();
    mBtk2.entry(modelData1);
    switch (mMode) {
    case LEVEL_UP_A:
        mBck1a.entry(modelData1);
        mBrk1.entry(modelData1);
        mBck2a.entry(modelData2);
        break;
    case LEVEL_DOWN_A:
        mBck1b.entry(modelData1);
        mBck2b.entry(modelData2);
        break;
    case LEVEL_UP_B:
        mBck1c.entry(modelData1);
        mBrk2.entry(modelData1);
        mBck2c.entry(modelData2);
        break;
    case LEVEL_DOWN_B:
        mBck1d.entry(modelData1);
        mBck2d.entry(modelData2);
        break;
    }
    dComIfGd_setXluListDarkBG();
    mDoExt_modelUpdateDL(mModel1);
    dComIfGd_setList();
    J3DMaterial* material = modelData2->getMaterialNodePointer(0);
    dComIfGd_setListInvisisble();
    if (material->getTexGenBlock()->getTexMtx(0) != NULL) {
        J3DTexMtxInfo* mtxInfo = &material->getTexGenBlock()->getTexMtx(0)->getTexMtxInfo();
        if (mtxInfo != NULL) {
            Mtx afStack_50;
            C_MTXLightPerspective(afStack_50, dComIfGd_getView()->fovy, dComIfGd_getView()->aspect,
                                  1.0f, 1.0f, -0.01f, 0.0f);
            #if WIDESCREEN_SUPPORT
            mDoGph_gInf_c::setWideZoomLightProjection(afStack_50);
            #endif
            mtxInfo->setEffectMtx(afStack_50);
            modelData2->simpleCalcMaterial(0, (MtxP)j3dDefaultMtx);
        }
    }
    mDoExt_modelUpdateDL(mModel2);
    dComIfGd_setList();
    return 1;
}

int daGrdWater_c::Delete() {
    dComIfG_resDelete(&mPhase, "Water");
    return 1;
}

static int daGrdWater_Draw(daGrdWater_c* i_this) {
    return i_this->Draw();
}

static int daGrdWater_Execute(daGrdWater_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daGrdWater_Delete(daGrdWater_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daGrdWater_Create(fopAc_ac_c* i_this) {
    return static_cast<daGrdWater_c*>(i_this)->create();
}

static actor_method_class l_daGrdWater_Method = {
    (process_method_func)daGrdWater_Create,
    (process_method_func)daGrdWater_Delete,
    (process_method_func)daGrdWater_Execute,
    NULL,
    (process_method_func)daGrdWater_Draw,
};

extern actor_process_profile_definition g_profile_GRDWATER = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_GRDWATER,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daGrdWater_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  489,                    // mPriority
  &l_daGrdWater_Method,   // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
