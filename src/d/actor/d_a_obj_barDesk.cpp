/**
 * d_a_obj_barDesk.cpp
 * Object - Table
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_barDesk.h"
#include "d/d_procname.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static daBarDesk_HIO_c l_HIO;

dCcD_SrcGObjInf const daBarDesk_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0xD8FAFDBF, 0x11}, {0}}},
    {1, 0, 0, 0, 0},
    {0xA, 0, 0, 0, 6},
    {0}
};

dCcD_SrcCyl daBarDesk_c::mCcDCyl = {
    mCcDObjInfo,
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f}
    }
};

daBarDesk_HIO_c::daBarDesk_HIO_c() : field_0x4(10), field_0x5(3) {
    /* empty function */
}

void daBarDesk_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daBarDesk_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("KHdesk", 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

cPhs__Step daBarDesk_c::create() {
    fopAcM_ct(this, daBarDesk_c);
    if (fopAcM_isSwitch(this, (u8)fopAcM_GetParam(this))) {
        return cPhs_ERROR_e;
    }
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, "KHdesk");
    if (step == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("KHdesk", 7, dBgS_MoveBGProc_TypicalRotY, 0xe50, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mColStatus.Init(0xff, 0xff, this);
        mColCyl.Set(mCcDCyl);
        mColCyl.SetStts(&mColStatus);
        init_modeWait();
        setBaseMtx();
    }
    return step;
}

int daBarDesk_c::Execute(Mtx** i_mtxP) {
    windowProc();
    *i_mtxP = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daBarDesk_c::windowProc() {
    typedef void (daBarDesk_c::*daBarDesk_modeFunc)();
    static const daBarDesk_modeFunc mode_proc[2] = {
        &daBarDesk_c::modeWait,
        &daBarDesk_c::modeBreak,
    };
    (this->*mode_proc[mMode])();
}

void daBarDesk_c::init_modeWait() {
    mMode = 0;
}

void daBarDesk_c::modeWait() {
    if (mColCyl.ChkTgHit()) {
        if (mpBgW != NULL) {
            dComIfG_Bgsp().Release(mpBgW);
        }
        init_modeBreak();
    }
    mColCyl.SetR(90.0f);
    mColCyl.SetH(90.0f);
    mColCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mColCyl);
    mColCyl.ClrTgHit();
}

void daBarDesk_c::init_modeBreak() {
    mDoAud_seStart(Z2SE_OBJ_TABLE_BRAKE, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setBreakEffect();
    dComIfGp_getVibration().StartShock(l_HIO.field_0x5, 15, cXyz(0.0f, 1.0f, 0.0f));
    fopAcM_onSwitch(this, (u8)fopAcM_GetParam(this));
    mMode = 1;
}

void daBarDesk_c::modeBreak() {
    fopAcM_delete(this);
}

void daBarDesk_c::setBreakEffect() {
    static const u16 particle_id[2] = {0x85F4, 0x85F5};
    for (int i = 0; i < 2; i++) {
        dComIfGp_particle_set(particle_id[i], &current.pos, NULL, &scale,
                              0xff, NULL, -1, NULL, NULL, NULL);
    }
}

int daBarDesk_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daBarDesk_c::Delete() {
    dComIfG_resDelete(&mPhase, "KHdesk");
    return 1;
}

static int daBarDesk_Draw(daBarDesk_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daBarDesk_Execute(daBarDesk_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daBarDesk_Delete(daBarDesk_c* i_this) {
    return i_this->MoveBGDelete();
}

static cPhs__Step daBarDesk_Create(fopAc_ac_c* i_this) {
    return static_cast<daBarDesk_c*>(i_this)->create();
}

static actor_method_class l_daBarDesk_Method = {
    (process_method_func)daBarDesk_Create,
    (process_method_func)daBarDesk_Delete,
    (process_method_func)daBarDesk_Execute,
    (process_method_func)NULL,
    (process_method_func)daBarDesk_Draw,
};

actor_process_profile_definition g_profile_Obj_BarDesk = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_BarDesk,
    &g_fpcLf_Method.base,
    sizeof(daBarDesk_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x279,
    &l_daBarDesk_Method,
    0x40100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
