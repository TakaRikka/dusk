/**
 * @file d_a_obj_lv2Candle.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv2Candle.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor.h"

static daLv2Candle_HIO_c l_HIO;

daLv2Candle_HIO_c::daLv2Candle_HIO_c() {
    mTimer = 30;
    mPow = 500.0f;
    mAnmIntensity[0] = 0.95f;
    mAnmIntensity[1] = 1.0f;
    mAnmIntensity[2] = 0.8f;
    mAnmIntensity[3] = 0.95f;
    mAnmIntensity[4] = 1.0f;
    mAnmIntensity[5] = 1.0f;
    mAnmTimers[0] = 2;
    mAnmTimers[1] = 2;
    mAnmTimers[2] = 1;
    mAnmTimers[3] = 2;
    mAnmTimers[4] = 2;
    mAnmTimers[5] = 3;
    mAnmMaxStep = 0.03f;
    mColor1R = 0xbc;
    mColor1G = 0x66;
    mColor1B = 0x42;
    mColor2R = 0xbc;
    mColor2G = 0x66;
    mColor2B = 0x42;
}

dCcD_SrcGObjInf const daLv2Candle_c::mCcDObjInfo = {
    {
        0, {
            {0x200, 0, 0x13},
            {0xd8fbffff, 0x1f},
            {0x79}
        }
    },
    {1, 0, 0, 0, 0},
    {0xa, 0, 0, 0, 6},
    {0},
};

dCcD_SrcCyl daLv2Candle_c::mCcDCyl = {
    daLv2Candle_c::mCcDObjInfo,
    { {0.0f, 0.0f, 0.0f}, 0, 0}
};

static char* l_resNameIdx[3] = {
    "L2candl",
    "L4candl",
    "L9Candle",
};

static u32 const l_bmdIdx[3] = {
    4,
    4,
    3,
};

void daLv2Candle_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv2Candle_c::CreateHeap() {
    J3DModelData* bmd =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mModelType], l_bmdIdx[mModelType]);
    mpModel = mDoExt_J3DModel__create(bmd, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return false;
    }
    return true;
}

cPhs__Step daLv2Candle_c::create() {
    fopAcM_ct(this, daLv2Candle_c);
    mModelType = getModelType();
    if (mModelType == 0xf) {
        mModelType = 0;
    }
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, l_resNameIdx[mModelType]);
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x1000)) {
            return cPhs_ERROR_e;
        }
        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mStts.Init(0xff, 0xff, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mStts);
        mTorchPos.x = current.pos.x;
        mTorchPos.y = current.pos.y + 135.0f;
        mTorchPos.z = current.pos.z;
        mLightAnm = getLightAnm();
        mLightType = getLightType();
        if (mLightType == 0xf) {
            mLightType = 0;
        }
        lightInit();
        mSwType = getSwType();
        if (mSwType == 0xf) {
            mSwType = 0;
        }
        eyePos = mTorchPos;
        mSw = getSw();
        if (mSw == 0xff) {
            mIsLit = true;
            mCyl.SetAtMtrl(dCcD_MTRL_FIRE);
            setLight();
        } else {
            mIsSwitch = isSwitch();
            mIsLit = false;
            if (mIsSwitch) {
                mIsLit = true;
                mCyl.SetAtMtrl(dCcD_MTRL_FIRE);
                setLight();
            }
        }
        mTimer = 0;
        mSound.init(&current.pos, 1);
        mTgHit = 0;
        field_0x734 = NULL;
        if (mModelType == 1) {
            field_0x734 = static_cast<dalv4CandleTag_c*>(fopAcM_Search(searchDemoTagAct, this));
        }
    }
    return step;
}

u8 daLv2Candle_c::isSwitch() {
    return mSwType ^ fopAcM_isSwitch(this, mSw);
}

int daLv2Candle_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daLv2Candle_c*>(i_this)->CreateHeap();
}

void* daLv2Candle_c::searchDemoTagAct(void* i_actor, void* param_1) {
    if (i_actor != NULL && fopAc_IsActor(i_actor) &&
                           fopAcM_GetProfName(i_actor) == PROC_Tag_Lv4Candle) {
        return i_actor;
    }
    return NULL;
}

void daLv2Candle_c::lightInit() {
    mLightPos = mTorchPos;
    mLightPos.y += 10.0f;
    mAnmStage = 0;
    mAnmTimer = l_HIO.mAnmTimers[mAnmStage];
    if (mLightType != 0) {
        mLight.mPosition = mLightPos;
        mLight.mColor.r = l_HIO.mColor1R;
        mLight.mColor.g = l_HIO.mColor1G;
        mLight.mColor.b = l_HIO.mColor1B;
        mLight.mPow = l_HIO.mPow;
        mLight.mFluctuation = 1.0f;
    }
    mIntensity = 0.0f;
}

void daLv2Candle_c::setLight() {
    if (mLightType != 0) {
        dKy_plight_set(&mLight);
    }
}

void daLv2Candle_c::cutLight() {
    if (mLightType != 0) {
        dKy_plight_cut(&mLight);
    }
}

void daLv2Candle_c::pointLightProc() {
    if (mLightType != 0) {
        if (mIsLit && mLightAnm != 0xf) {
            f32 delta = cLib_addCalc(&mIntensity, l_HIO.mAnmIntensity[mAnmStage],
                                    1.0f, l_HIO.mAnmMaxStep, 0.0001f);
            mLight.mPow = l_HIO.mPow * mIntensity;
            if (delta == 0.0f) {
                if (mAnmTimer == 0) {
                    mAnmStage++;
                    if (mAnmStage > 5) {
                        mAnmStage = 0;
                    }
                    mAnmTimer = l_HIO.mAnmTimers[mAnmStage];
                } else {
                    mAnmTimer--;
                }
            }
        }
    } else {
        GXColor color = {l_HIO.mColor2R, l_HIO.mColor2G, l_HIO.mColor2B, 0xff};
        if (mIsLit) {
            if (mLightAnm != 0xf) {
                f32 delta = cLib_addCalc(&mIntensity, l_HIO.mAnmIntensity[mAnmStage],
                                         1.0f, l_HIO.mAnmMaxStep, 0.0001f);
                if (delta == 0.0f) {
                    if (mAnmTimer == 0) {
                        mAnmStage++;
                        if (mAnmStage > 5) {
                            mAnmStage = 0;
                        }
                        mAnmTimer = l_HIO.mAnmTimers[mAnmStage];
                    } else {
                        mAnmTimer--;
                    }
                }
            } else {
                cLib_addCalc(&mIntensity, 1.0f, 0.5f, 0.1f, 0.0001f);
            }
        } else {
            cLib_addCalc(&mIntensity, 0.0f, 0.5f, 0.1f, 0.0001f);
        }
        if (mIntensity >= 1e-6f) {
            dKy_BossLight_set(&mLightPos, &color, mIntensity, 0);
        }
    }
}

int daLv2Candle_c::Execute() {
    if (mSw == 0xff) {
        if (mTimer != 0) {
            mTimer--;
            if (mTimer == 0) {
                mIsLit = true;
                mCyl.SetAtMtrl(dCcD_MTRL_FIRE);
                setLight();
            }
        }
    } else {
        mIsSwitch = isSwitch();
        if (mIsSwitch) {
            if (mIsLit == false) {
                mIsLit = true;
                mCyl.SetAtMtrl(dCcD_MTRL_FIRE);
                setLight();
            }
        } else {
            if (mIsLit == true) {
                mIsLit = false;
                mCyl.SetAtMtrl(dCcD_MTRL_NONE);
                cutLight();
            }
        }
    }

    if (mIsLit) {
        dComIfGp_particle_setSimple(0x100, &mTorchPos, 0xff,
                                    g_whiteColor, g_whiteColor, 0, 0.0f);
        dComIfGp_particle_setSimple(0x101, &mTorchPos, 0xff,
                                    g_whiteColor, g_whiteColor, 0, 0.0f);
        dComIfGp_particle_setSimple(0x103, &mTorchPos, 0xff,
                                    g_whiteColor, g_whiteColor, 0, 0.0f);
        mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &mTorchPos, 0,
                            dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    if (field_0x734 != NULL) {
        if (field_0x734->getNGSwStat()) {
            if (mCyl.MskTgSPrm(1)) {
                mCyl.OffTgSetBit();
            }
        } else {
            if (!mCyl.MskTgSPrm(1)) {
                mCyl.OnTgSetBit();
            }
        }
    }

    if (mCyl.ChkTgHit()) {
        dCcD_GObjInf* gobj = mCyl.GetTgHitGObj();
        if (gobj->GetAtType() == AT_TYPE_LANTERN_SWING && gobj->GetAtMtrl() == dCcD_MTRL_FIRE && !mIsLit) {
            mIsLit = true;
            mCyl.SetAtMtrl(dCcD_MTRL_FIRE);
            if (!mSwType) {
                fopAcM_onSwitch(this, mSw);
            } else {
                fopAcM_offSwitch(this, mSw);
            }
            mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &mTorchPos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            setLight();
        }
        if (gobj->GetAtType() == AT_TYPE_BOOMERANG && gobj->GetAtMtrl() == dCcD_MTRL_WIND &&
                                                      mIsLit == true && getOffType() == 1) {
            mIsLit = false;
            mCyl.SetAtMtrl(dCcD_MTRL_NONE);
            cutLight();
            mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &mTorchPos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            if (mSw == 0xff) {
                mTimer = l_HIO.mTimer;
            }
            else if (!mSwType) {
                fopAcM_offSwitch(this, mSw);
            } else {
                fopAcM_onSwitch(this, mSw);
            }
        }
        cCcD_Obj* obj = mCyl.GetTgHitObj();
        if (obj != NULL) {
            fopAc_ac_c* actor = obj->GetAc();
            bool bvar1 = true;
            if (actor != NULL) {
                if (fopAcM_GetName(actor) == PROC_E_HP && mTgHit) {
                    bvar1 = false;
                }
                if (fopAcM_GetName(actor) == PROC_E_DD) {
                    bvar1 = false;
                }
                if (fopAcM_GetName(actor) == PROC_ALINK) {
                    if (mCyl.GetTgHitGObj()->ChkAtType(AT_TYPE_NORMAL_SWORD) && mTgHit) {
                        bvar1 = false;
                    }
                }
                if (bvar1) {
                    u32 se_id = mCyl.GetTgHitObjHitSeID(1);
                    if (mModelType == 0) {
                        mSound.startCollisionSE(se_id, 2, NULL);
                    } else {
                        mSound.startCollisionSE(se_id, 9, NULL);
                    }
                    mTgHit = true;
                }
            }
        }
    } else {
        mTgHit = false;
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mCyl.SetR(25.0f);
    mCyl.SetH(160.0f);
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    pointLightProc();
    setBaseMtx();
    return true;
}

int daLv2Candle_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return true;
}

int daLv2Candle_c::Delete() {
    mSound.deleteObject();
    dComIfG_resDelete(&mPhase, l_resNameIdx[mModelType]);
    if (mLightType != 0) {
        dKy_plight_cut(&mLight);
    }
    return true;
}

static int daLv2Candle_Draw(daLv2Candle_c* i_this) {
    return i_this->Draw();
}

static int daLv2Candle_Execute(daLv2Candle_c* i_this) {
    return i_this->Execute();
}

static int daLv2Candle_Delete(daLv2Candle_c* i_this) {
    return i_this->Delete();
}

static cPhs__Step daLv2Candle_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv2Candle_c*>(i_this)->create();
}

static actor_method_class l_daLv2Candle_Method = {
    (process_method_func)daLv2Candle_Create,
    (process_method_func)daLv2Candle_Delete,
    (process_method_func)daLv2Candle_Execute,
    (process_method_func)NULL,
    (process_method_func)daLv2Candle_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv2Candle = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Lv2Candle,
    &g_fpcLf_Method.base,
    sizeof(daLv2Candle_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x20B,
    &l_daLv2Candle_Method,
    0x60000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
