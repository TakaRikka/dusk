//
// d_a_obj_lv3Candle.cpp
// Object - Level 3 Candle
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv3Candle.h"

#include "d/d_com_inf_game.h"

static daLv3Candle_HIO_c l_HIO;

daLv3Candle_HIO_c::daLv3Candle_HIO_c() {
    field_0x04 = 0x1e;
}

dCcD_SrcGObjInf const daLv3Candle_c::mCcDObjInfo = {
    {0, {{0x200, 0, 0x13}, {0xd8fbfdff, 0x1f}, {0x79}}},
    {1, 0, 0, 0, 0},
    {0xa, 0, 0, 0, 6},
    {0},
};

dCcD_SrcSph daLv3Candle_c::mCcDSph = {daLv3Candle_c::mCcDObjInfo, {{{0.0f, 0.0f, 0.0f}, 0.0f}}};

static char* l_resNameIdx[] = {"L3candl", "L3candl2"};

void daLv3Candle_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static u32 const l_bmdIdx[] = {0x03, 0x03};

int daLv3Candle_c::CreateHeap() {
    J3DModelData* model_data =
        (J3DModelData*)dComIfG_getObjectRes(l_resNameIdx[mType], l_bmdIdx[mType]);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);

    if (mpModel == NULL) {
        return FALSE;
    }

    return TRUE;
}

cPhs__Step daLv3Candle_c::create() {
    fopAcM_ct(this, daLv3Candle_c);

    mType = getType();
    if (mType == 0xff) {
        mType = 0;
    }

    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_resNameIdx[mType]);
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x900)) {
            return cPhs_ERROR_e;
        } else {
            setBaseMtx();
            fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
            fopAcM_setCullSizeBox2(this, mpModel->getModelData());

            mStts.Init(0xff, 0xff, this);
            mSph.Set(mCcDSph);
            mSph.SetStts(&mStts);
            mTorchPos = current.pos;

            if (mType == 0) {
                cXyz v(0.0f, 20.0f, 125.0f);
                mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
                mDoMtx_stack_c::multVec(&v, &v);
                mTorchPos += v;
            } else {
                mTorchPos.y -= 200.0f;
            }

            mIsLit = getSwBit();
            if (mIsLit == 0xff) {
                mIsLit = 0;
            }

            lightInit();

            eyePos = mTorchPos;
            mSound.init(&current.pos, 1);
            mTgHit = 0;
        }
    }

    return step;
}

int daLv3Candle_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daLv3Candle_c*>(i_this)->CreateHeap();
}

void daLv3Candle_c::lightInit() {
    mLightPos = mTorchPos;
    mLightPos.y += 10.0f;

    if (mIsLit) {
        mLight.mPosition = mLightPos;
        mLight.mColor.r = 188;
        mLight.mColor.g = 102;
        mLight.mColor.b = 66;
        mLight.mPow = 500.0f;
        mLight.mFluctuation = 1.0f;
        return;
    }

    mIntensity = 0.0f;
}

void daLv3Candle_c::pointLightProc() {
    if (!mIsLit) {
        GXColor color = {188, 102, 66, 255};
        cLib_addCalc(&mIntensity, 1.0f, 0.5f, 0.1f, 0.0001f);

        if (mIntensity >= 0.000001f) {
            dKy_BossLight_set(&mLightPos, &color, mIntensity, 0);
        }
    }
}

int daLv3Candle_c::Execute() {
    dComIfGp_particle_setSimple(0x100, &mTorchPos, 0xff, g_whiteColor, g_whiteColor, 0, 0.0f);
    dComIfGp_particle_setSimple(0x101, &mTorchPos, 0xff, g_whiteColor, g_whiteColor, 0, 0.0f);
    dComIfGp_particle_setSimple(0x103, &mTorchPos, 0xff, g_whiteColor, g_whiteColor, 0, 0.0f);

    mDoAud_seStartLevel(Z2SE_OBJ_FIRE_BURNING, &mTorchPos, 0,
                        dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (mSph.ChkTgHit()) {
        cCcD_Obj* obj = mSph.GetTgHitObj();
        if (obj != NULL) {
            bool play_sound = true;
            fopAc_ac_c* actor = obj->GetAc();
            if (fopAcM_GetName(actor) == PROC_ALINK) {
                dCcD_GObjInf* gobj = mSph.GetTgHitGObj();
                if (gobj->GetAtType() & AT_TYPE_NORMAL_SWORD && mTgHit != 0) {
                    play_sound = false;
                }
            }

            if (play_sound) {
                u32 se_id = mSph.GetTgHitObjHitSeID(1);
                mSound.startCollisionSE(se_id, 9, NULL);
            }
        }
    } else {
        mTgHit = 0;
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setBaseMtx();

    cXyz pos = current.pos;
    if (mType == 0) {
        mSph.SetR(90.0f);
        pos.z += 70.0f;
    } else {
        mSph.SetR(60.0f);
        pos.y -= 200.0f;
    }

    mSph.SetC(pos);
    dComIfG_Ccsp()->Set(&mSph);

    pointLightProc();

    return TRUE;
}

int daLv3Candle_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();

    return TRUE;
}

int daLv3Candle_c::Delete() {
    mSound.deleteObject();
    dComIfG_resDelete(&mPhaseReq, l_resNameIdx[mType]);

    if (mIsLit) {
        dKy_plight_cut(&mLight);
    }
    return TRUE;
}

static int daLv3Candle_Draw(daLv3Candle_c* i_this) {
    return static_cast<daLv3Candle_c*>(i_this)->Draw();
}

static int daLv3Candle_Execute(daLv3Candle_c* i_this) {
    return static_cast<daLv3Candle_c*>(i_this)->Execute();
}

static int daLv3Candle_Delete(daLv3Candle_c* i_this) {
    return static_cast<daLv3Candle_c*>(i_this)->Delete();
}

static int daLv3Candle_Create(fopAc_ac_c* i_this) {
    return static_cast<daLv3Candle_c*>(i_this)->create();
}

static actor_method_class l_daLv3Candle_Method = {
    (process_method_func)daLv3Candle_Create,  (process_method_func)daLv3Candle_Delete,
    (process_method_func)daLv3Candle_Execute, NULL,
    (process_method_func)daLv3Candle_Draw,
};

actor_process_profile_definition g_profile_Obj_Lv3Candle = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv3Candle,      // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daLv3Candle_c),   // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    620,                     // mPriority
    &l_daLv3Candle_Method,   // sub_method
    0x60000,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
