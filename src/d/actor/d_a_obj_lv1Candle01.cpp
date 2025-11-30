//
// Translation Unit: d_a_obj_lv1Candle01
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv1Candle01.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static daLv1Cdl01_HIO_c l_HIO;

dCcD_SrcGObjInf const daLv1Cdl01_c::mCcDObjInfo = {
    {
        0, {
            {0, 0, 0},
            {0xd8fbffff, 0x1f},
            {0x79}
        }
    },
    {1, 0, 0, 0, 0},
    {0xa, 0, 0, 0, 6},
    {0},
};

dCcD_SrcCyl daLv1Cdl01_c::mCcDCyl = {
    daLv1Cdl01_c::mCcDObjInfo,
    { {0.0f, 0.0f, 0.0f}, 0, 0}
};

daLv1Cdl01_HIO_c::daLv1Cdl01_HIO_c() :
    field_0x4(60),
    field_0x5(1)
    {}

void daLv1Cdl01_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daLv1Cdl01_c::CreateHeap() {
    J3DModelData* bmd = (J3DModelData*)dComIfG_getObjectRes("lv1cdl01", 4);
    mpModel = mDoExt_J3DModel__create(bmd, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return false;
    }
    return true;
}

cPhs__Step daLv1Cdl01_c::create() {
    fopAcM_ct(this, daLv1Cdl01_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, "lv1cdl01");
    if (step == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("lv1cdl01", -1, NULL, 0x820, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mLightType = fopAcM_GetParam(this) >> 8;
        if (mLightType == 0xff) {
            mLightType = 1;
        }
        mTorchPos.x = current.pos.x;
        mTorchPos.y = current.pos.y + 120.0f;
        mTorchPos.z = current.pos.z;
        lightInit();
        mIsLit = true;
        mTimer = 0;
        setLight();
        eyePos = mTorchPos;
        mStts.Init(0xff, 0xff, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mStts);
        mSound.init(&current.pos, 1);
        mTgHit = false;
    }
    return step;
}

void daLv1Cdl01_c::lightInit() {
    mLightPos = mTorchPos;
    mLightPos.y += 10.0f;
    if (mLightType != 0) {
        mLight.mPosition = mLightPos;
        mLight.mColor.r = 0xbc;
        mLight.mColor.g = 0x66;
        mLight.mColor.b = 0x42;
        mLight.mPow = 500.0f;
        mLight.mFluctuation = 1.0f;
    } else {
        mIntensity = 0.0f;
    }
}

void daLv1Cdl01_c::setLight() {
    if (mLightType != 0) {
        dKy_plight_set(&mLight);
    }
}

void daLv1Cdl01_c::cutLight() {
    if (mLightType != 0) {
        dKy_plight_cut(&mLight);
    }
}

void daLv1Cdl01_c::pointLightProc() {
    if (mLightType == 0) {
        GXColor color = { 0xbc, 0x66, 0x42, 0xff };
        if (mIsLit) {
            cLib_addCalc(&mIntensity, 1.0f, 0.5f, 0.1f, 0.0001f);
        } else {
            cLib_addCalc(&mIntensity, 0.0f, 0.5f, 0.1f, 0.0001f);
        }
        if (mIntensity >= 1e-6f) {
            dKy_BossLight_set(&mLightPos, &color, mIntensity, 0);
        }
    }
}

int daLv1Cdl01_c::Execute(Mtx** param_0) {
    *param_0 = &mpModel->getBaseTRMtx();
    if (mTimer != 0) {
        mTimer--;
        if (mTimer == 0) {
            mIsLit = true;
            setLight();
        }
    }

    if (mIsLit) {
        dComIfGp_particle_setSimple(0x83a6, &mTorchPos, 0xff,
                                    g_whiteColor, g_whiteColor, 0, 0.0f);
        dComIfGp_particle_setSimple(0x83a7, &mTorchPos, 0xff,
                                    g_whiteColor, g_whiteColor, 0, 0.0f);
        dComIfGp_particle_setSimple(0x103, &mTorchPos, 0xff,
                                    g_whiteColor, g_whiteColor, 0, 0.0f);
        mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &mTorchPos, 0,
                            dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    setBaseMtx();

    if (mCyl.ChkTgHit()) {
        dCcD_GObjInf* gobj = mCyl.GetTgHitGObj();
        if (gobj->GetAtType() == AT_TYPE_LANTERN_SWING && gobj->GetAtMtrl() == dCcD_MTRL_FIRE && !mIsLit) {
            mIsLit = true;
            setLight();
            mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &mTorchPos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
        if (gobj->GetAtType() == AT_TYPE_BOOMERANG && gobj->GetAtMtrl() == dCcD_MTRL_WIND && mIsLit == true) {
            mTimer = 30;
            mIsLit = false;
            mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &mTorchPos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            cutLight();
        }
        cCcD_Obj* obj = mCyl.GetTgHitObj();
        if (obj != NULL) {
            bool bvar1 = true;
            if (fopAcM_GetName(obj->GetAc()) == PROC_ALINK) {
                if (mCyl.GetTgHitGObj()->ChkAtType(AT_TYPE_NORMAL_SWORD) && mTgHit) {
                    bvar1 = false;
                }
            }
            if (bvar1) {
                mSound.startCollisionSE(mCyl.GetTgHitObjHitSeID(1), 5, NULL);
                mTgHit = true;
            }
        }
    } else {
        mTgHit = false;
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mCyl.SetR(25.0f);
    mCyl.SetH(130.0f);
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    pointLightProc();
    return true;
}

int daLv1Cdl01_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return true;
}

int daLv1Cdl01_c::Delete() {
    mSound.deleteObject();
    dComIfG_resDelete(&mPhase, "lv1cdl01");
    if (mLightType != 0) {
        dKy_plight_cut(&mLight);
    }
    return true;
}

static int daLv1Cdl01_Draw(daLv1Cdl01_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daLv1Cdl01_Execute(daLv1Cdl01_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daLv1Cdl01_Delete(daLv1Cdl01_c* i_this) {
    return i_this->MoveBGDelete();
}

static cPhs__Step daLv1Cdl01_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv1Cdl01_c*>(i_this)->create();
}

static actor_method_class l_daLv1Cdl01_Method = {
    (process_method_func)daLv1Cdl01_Create,
    (process_method_func)daLv1Cdl01_Delete,
    (process_method_func)daLv1Cdl01_Execute,
    (process_method_func)NULL,
    (process_method_func)daLv1Cdl01_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv1Cdl01 = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_Lv1Cdl01,
    &g_fpcLf_Method.base,
    sizeof(daLv1Cdl01_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x1EF,
    &l_daLv1Cdl01_Method,
    0x60000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
