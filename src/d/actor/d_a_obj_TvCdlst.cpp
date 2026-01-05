/**
 * @file d_a_obj_TvCdlst.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_TvCdlst.h"
#include "d/d_com_inf_game.h"

static daTvCdlst_HIO_c l_HIO;

dCcD_SrcCyl daTvCdlst_c::mCcDCyl = {
    daTvCdlst_c::mCcDObjInfo, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            0.0f, // mRadius
            0.0f // mHeight
        } // mCyl
    } // mCylAttr
};

daTvCdlst_HIO_c::daTvCdlst_HIO_c() {
    field_0x4 = 0x3c;
    field_0x5 = 1;
}

void daTvCdlst_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daTvCdlst_c::CreateHeap() {
    J3DModelData* pModelData = (J3DModelData*) dComIfG_getObjectRes("HShokudai", 3);
    JUT_ASSERT(222, pModelData != NULL);
    mpModel = mDoExt_J3DModel__create(pModelData, 0x80000,0x11000084);
    if (mpModel == NULL)
        return 0;
    return 1;
}

const dCcD_SrcGObjInf daTvCdlst_c::mCcDObjInfo = {
    {0, // mFlags
        {
            {0x0, 0x0, 0x0},  // mObjAt
            {0xD8FBFFFF, 0x1F}, // mObjTg
            {0x79}              // mObjCo
        } // mSrcObjHitInf
    }, // mObj
    {1, 0, 0, 0, 0}, // mGObjAt
    {0xA, 0, 0, 0, 0}, // mGObjTg
    {0} // mGObjCo
};

int daTvCdlst_c::create() {
    fopAcM_ct(this, daTvCdlst_c);
    int phase = dComIfG_resLoad(&mPhaseReq,"HShokudai");
    if (phase == cPhs_COMPLEATE_e) {
        if (fopAcM_entrySolidHeap(this,createHeapCallBack,0x820) == 0) {
            return cPhs_ERROR_e;
        }
        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        field_0x6ec.x = current.pos.x;
        field_0x6ec.y = current.pos.y + 155.0f;
        field_0x6ec.z = current.pos.z;
        mLightType = getLightType();
        if (mLightType == 0xff) {
            mLightType = 0;
        }
        lightInit();
        eyePos = field_0x6ec;
        mOnSw = getOnSw();
        mOffSw = getOffSw();
        mIsOn = fopAcM_isSwitch(this, mOnSw);
        mIsOff = fopAcM_isSwitch(this, mOffSw);
        mDoIgnite = 0;
        field_0x6f8 = 0;
        if (mIsOn && mIsOff == 0) {
            field_0x6f8 = 1;
            mDoIgnite = 0;
            setLight();
        }
        mStts.Init(0xff, 0xff, this);
        mCyl.Set(mCcDCyl);
        mCyl.SetStts(&mStts);
        mSound.init(&current.pos, 1);
        // FIXME: Need mDoHIO_entry_c::entryHIO
    }
    return phase;
}

int daTvCdlst_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daTvCdlst_c*>(i_this)->CreateHeap();
}

void daTvCdlst_c::lightInit() {
    field_0x700 = field_0x6ec;
    field_0x700.y += 10.0f;
    if (mLightType) {
        mLight.mPosition = field_0x700;
        mLight.mColor.r = 0xbc;
        mLight.mColor.g = 0x66;
        mLight.mColor.b = 0x42;
        mLight.mPow = 500.0f;
        mLight.mFluctuation = 1.0f;
        return;
    }
    field_0x710 = 0.0f;
}

void daTvCdlst_c::setLight() {
    if (mLightType) {
        dKy_plight_set(&mLight);
    }
}

void daTvCdlst_c::cutLight() {
    if (mLightType) {
        dKy_plight_cut(&mLight);
    }
}

void daTvCdlst_c::pointLightProc() {
    if (mLightType == 0) {
        GXColor color = {0xbc, 0x66, 0x42, 0xff};
        if (field_0x6f8 != 0) {
            cLib_addCalc(&field_0x710, 1.0f, 0.5f, 0.1f, 0.0001f);
        } else {
            cLib_addCalc(&field_0x710, 0.0f, 0.5f, 0.1f, 0.0001f);
        }
        if (field_0x710 >= 1e-06f) {
            dKy_BossLight_set(&field_0x700, &color, field_0x710, 0);
        }
    }
}

int daTvCdlst_c::Execute() {
    mIsOn = fopAcM_isSwitch(this, mOnSw);
    mIsOff = fopAcM_isSwitch(this, mOffSw);
    if (mIsOn && mIsOff == 0 && field_0x6f8 == 0) {
        field_0x6f8 = 1;
        mDoIgnite = 1;
        setLight();
    }
    if ((mIsOn == 0 && mIsOff == 0 || mIsOff) && field_0x6f8 == 1) {
        s8 roomNo = fopAcM_GetRoomNo(this);
        mDoAud_seStart(Z2SE_OBJ_FIRE_OFF, &field_0x6ec, 0, dComIfGp_getReverb(roomNo));
        field_0x6f8 = 0;
        cutLight();
    }
    if (field_0x6f8) {
        dComIfGp_particle_setSimple(0x100, &field_0x6ec, 0xff, g_whiteColor, g_whiteColor, 0, 0.0f);
        dComIfGp_particle_setSimple(0x101, &field_0x6ec, 0xff, g_whiteColor, g_whiteColor, 0, 0.0f);
        dComIfGp_particle_setSimple(0x103, &field_0x6ec, 0xff, g_whiteColor, g_whiteColor, 0, 0.0f);
        if (mDoIgnite) {
            s8 roomNo = fopAcM_GetRoomNo(this);
            mDoAud_seStart(Z2SE_OBJ_FIRE_IGNITION, &field_0x6ec, 0, dComIfGp_getReverb(roomNo));
            mDoIgnite = 0;
        } else {
            s8 roomNo = fopAcM_GetRoomNo(this);
            mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &field_0x6ec, 0, dComIfGp_getReverb(roomNo));
        }
    }
    setBaseMtx();
    if (mCyl.ChkTgHit() && mCyl.GetTgHitObj()) {
        mSound.startCollisionSE(mCyl.GetTgHitObjHitSeID(1), 5, NULL);
    }
    s8 roomNo = fopAcM_GetRoomNo(this);
    mSound.framework(0, dComIfGp_getReverb(roomNo));
    mCyl.SetR(25.0f);
    mCyl.SetH(140.0f);
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
    pointLightProc();
    return 1;
}

int daTvCdlst_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daTvCdlst_c::Delete() {
    mSound.deleteObject();
    dComIfG_resDelete(&mPhaseReq, "HShokudai");
    if (mLightType) {
        dKy_plight_cut(&mLight);
    }
    return 1;
}

static int daTvCdlst_Draw(daTvCdlst_c* i_this) {
    return i_this->Draw();
}

static int daTvCdlst_Execute(daTvCdlst_c* i_this) {
    return i_this->Execute();
}

static int daTvCdlst_Delete(daTvCdlst_c* i_this) {
    return i_this->Delete();
}

static int daTvCdlst_Create(fopAc_ac_c* i_this) {
    return static_cast<daTvCdlst_c*>(i_this)->create();
}

static actor_method_class l_daTvCdlst_Method = {
    (process_method_func)daTvCdlst_Create,
    (process_method_func)daTvCdlst_Delete,
    (process_method_func)daTvCdlst_Execute,
    0,
    (process_method_func)daTvCdlst_Draw,
};

actor_process_profile_definition g_profile_Obj_TvCdlst = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_TvCdlst,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daTvCdlst_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  496,                    // mPriority
  &l_daTvCdlst_Method,    // sub_method
  0x00044000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
