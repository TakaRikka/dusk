/**
 * d_a_obj_zrTuraraRock.cpp
 * Object - Zora River Stalactite Debris
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_zrTuraraRock.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static daZrTuraRc_HIO_c l_HIO;

daZrTuraRc_HIO_c::daZrTuraRc_HIO_c() {
    field_0x04 = 450.0f;
    mGravity = 10.0f;
    mMaxFallSpeed = 200.0f;
    field_0x10 = 30;
    field_0x14 = 5.0f;
    field_0x11 = 4;
    field_0x18 = 3.0f;
    field_0x1c = 5.0f;
}

dCcD_SrcGObjInf const daZrTuraRc_c::mCcDObjInfo = {
    {0, {
        {0x20, 1, 0x1F},
        {0, 0},
        {0x79}
    }},
    {1, 0, 1, 0, 0},
    {0xA, 0, 0, 0, 2},
    {0}
};

dCcD_SrcSph daZrTuraRc_c::mCcDSph = {
    mCcDObjInfo,
    {{{0.0f, 0.0f, 0.0f}, 0.0f}}
};

void daZrTuraRc_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::scaleM(mScaleF, mScaleF, mScaleF);
    mpModel->setBaseScale(cXyz(1.0f, 1.0f, 1.0f));
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daZrTuraRc_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("M_DRockHn", 3);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

cPhs__Step daZrTuraRc_c::create() {
    fopAcM_ct(this, daZrTuraRc_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, "M_DRockHn");
    if (step == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x840)) {
            return cPhs_ERROR_e;
        }
        mAcchCir.SetWall(130.0f, 65.0f);
        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
        mAcch.SetWaterCheckOffset(10000.0f);
        mCcStatus.Init(0xff, 0xff, this);
        mCcSph.Set(mCcDSph);
        mCcSph.SetStts(&mCcStatus);
        mSound.init(&current.pos, 1);
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        u8 scale = getScale();
        if (scale == 0xff) {
            scale = 0;
        }
        mScaleF = (scale * 0.1f + 1.0f) * (1.0f - fabsf(cM_rndFX(0.9f)));
        shape_angle.x = cM_deg2s(cM_rndFX(180.0f));
        shape_angle.y = cM_deg2s(cM_rndFX(180.0f));
        shape_angle.z = cM_deg2s(cM_rndFX(180.0f));
        setBaseMtx();
        init_modeDrop();
    }
    return step;
}

int daZrTuraRc_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daZrTuraRc_c*>(i_this)->CreateHeap();
}

int daZrTuraRc_c::Execute() {
    move();
    setBaseMtx();
    mCcStatus.Move();
    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

void daZrTuraRc_c::move() {
    typedef void (daZrTuraRc_c::*daZrTuraRc_modeFunc)();
    static daZrTuraRc_modeFunc mode_proc[2] = {
        &daZrTuraRc_c::modeDrop,
        &daZrTuraRc_c::modeDropEnd,
    };
    (this->*mode_proc[mMode])();
    mCcSph.SetR(mScaleF * 130.0f);
    mCcSph.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcSph);
}

void daZrTuraRc_c::init_modeDrop() {
    fopAcM_SetGravity(this, -l_HIO.mGravity);
    fopAcM_SetMaxFallSpeed(this, -l_HIO.mMaxFallSpeed);
    fopAcM_SetSpeedF(this, 0.0f);
    mMode = 0;
}

void daZrTuraRc_c::modeDrop() {
    fopAcM_posMoveF(this, mCcStatus.GetCCMoveP());
    mSound.startLevelSound(Z2SE_OBJ_RG_ROCK_FALL, 0, -1);
    mAcch.CrrPos(dComIfG_Bgsp());
    bgCheck();
}

void daZrTuraRc_c::bgCheck() {
    // this should be ChkGroundHit but that doesn't match
    if (mAcch.m_flags >> 5 & 1) {
        init_modeDropEnd();
    }
}

void daZrTuraRc_c::init_modeDropEnd() {
    fopAcM_delete(this);
    mMode = 1;
}

void daZrTuraRc_c::modeDropEnd() {
    /* empty function */
}

int daZrTuraRc_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daZrTuraRc_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "M_DRockHn");
    mSound.deleteObject();
    return 1;
}

static int daZrTuraRc_Draw(daZrTuraRc_c* i_this) {
    return i_this->Draw();
}

static int daZrTuraRc_Execute(daZrTuraRc_c* i_this) {
    return i_this->Execute();
}

static int daZrTuraRc_Delete(daZrTuraRc_c* i_this) {
    return i_this->Delete();
}

static cPhs__Step daZrTuraRc_Create(fopAc_ac_c* i_this) {
    return static_cast<daZrTuraRc_c*>(i_this)->create();
}

static actor_method_class l_daZrTuraRc_Method = {
    (process_method_func)daZrTuraRc_Create,
    (process_method_func)daZrTuraRc_Delete,
    (process_method_func)daZrTuraRc_Execute,
    (process_method_func)NULL,
    (process_method_func)daZrTuraRc_Draw,
};

extern actor_process_profile_definition g_profile_Obj_zrTuraraRc = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_zrTuraraRc,
    &g_fpcLf_Method.base,
    sizeof(daZrTuraRc_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x28D,
    &l_daZrTuraRc_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
