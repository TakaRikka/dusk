/**
 * @file d_a_obj_rfHole.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_rfHole.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"

daRfHole_HIO_c::daRfHole_HIO_c() {
    field_0x04 = 130.0f;
    field_0x08 = 200.0f;
    field_0x10 = 0.0f;
    field_0x0c = 200.0f;
    field_0x14 = 10;
    field_0x15 = 4;
}

void daRfHole_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mModel->setBaseScale(scale);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daRfHole_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Otosiana", 4);
    JUT_ASSERT(167, modelData!=NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    return mModel != NULL ? 1 : 0;
}

int daRfHole_c::create() {
    fopAcM_ct(this, daRfHole_c);
    int rv = dComIfG_resLoad(&mPhase, "Otosiana");
    if (rv == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("Otosiana", 7, dBgS_MoveBGProc_TypicalRotY, 0xb40, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        } 
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mModel->getModelData());
        if (fopAcM_isSwitch(this, (u8)fopAcM_GetParam(this))) {
            init_modeEnd();
        } else {
            field_0x5ad = (fopAcM_GetParam(this) >> 8) & 0xff;
            if (field_0x5ad != 1) {
                mpBgW->SetRideCallback(rideCallBack);
                init_modeWait();
            } else {
                init_modeBreakEff();
            }
        }
        field_0x5ae = 0;
        mRoofSoundOn = 0;
        setBaseMtx();
    }
    return rv;
}

void daRfHole_c::rideCallBack(dBgW* param_1, fopAc_ac_c* param_2, fopAc_ac_c* param_3) {
    if (fopAcM_GetName(param_3) == PROC_ALINK && static_cast<daRfHole_c*>(param_2)->playerAreaCheck()) {
        static_cast<daRfHole_c*>(param_2)->field_0x5ae = 1;
    }
}

static daRfHole_HIO_c l_HIO;

int daRfHole_c::playerAreaCheck() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz cStack_30(l_HIO.field_0x0c, 0.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::multVec(&cStack_30, &cStack_30);
    cXyz cStack_3c = current.pos + cStack_30;
    cXyz cStack_48 = cStack_3c - player->current.pos;
    f32 dVar8 = cStack_48.absXZ();
    int rv = 0;
    if (dVar8 <= l_HIO.field_0x04) {
        rv = 1;
    }
    u8 soundOn = 0;
    if (dVar8 <= l_HIO.field_0x08) {
        soundOn = 1;
        if (mRoofSoundOn != 1) {
            mDoAud_seStart(Z2SE_OBJ_ROOF_ANA, &cStack_3c, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
    }
    mRoofSoundOn = soundOn;
    return rv;
}

int daRfHole_c::Execute(Mtx** i_mtx) {
    holeProc();
    *i_mtx = &mModel->getBaseTRMtx();
    setBaseMtx();
    field_0x5ae = 0;
    return 1;
}

void daRfHole_c::holeProc() {
    static ModeFunc mode_proc[] = {&daRfHole_c::modeWait, &daRfHole_c::modeBreak,
                                   &daRfHole_c::modeBreakEff, &daRfHole_c::modeEnd};
    (this->*(mode_proc[mMode]))();
}

void daRfHole_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daRfHole_c::modeWait() {
    if (field_0x5ae != 0) {
        init_modeBreak();
    }
}

void daRfHole_c::init_modeBreak() {
    setBreakEffect();
    dComIfGp_getVibration().StartShock(l_HIO.field_0x15, 15, cXyz(0.0f, 1.0f, 0.0f));
    cXyz acStack_20(l_HIO.field_0x0c, 0.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::multVec(&acStack_20, &acStack_20);
    cXyz cStack_2c = current.pos + acStack_20;
    mDoAud_seStart(Z2SE_OBJ_WOODROOF_BREAK, &cStack_2c, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    fopAcM_onSwitch(this, (u8)fopAcM_GetParam(this));
    mMode = MODE_BREAK;
}

void daRfHole_c::modeBreak() {
    fopAcM_delete(this);
}

void daRfHole_c::init_modeBreakEff() {
    setBreakEffect();
    fopAcM_onSwitch(this, (u8)fopAcM_GetParam(this));
    mMode = MODE_BREAK_EFF;
}

void daRfHole_c::modeBreakEff() {
    fopAcM_delete(this);
}

void daRfHole_c::init_modeEnd() {
    mMode = MODE_END;
}

void daRfHole_c::modeEnd() {
    fopAcM_delete(this);
}

void daRfHole_c::setBreakEffect() {
    static u16 const particle_id[5] = {
        0x83B0, 0x83B1, 0x83B2, 0x83B3, 0x83B4,
    };
    cXyz acStack_24(l_HIO.field_0x0c, 0.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::multVec(&acStack_24, &acStack_24);
    cXyz cStack_30 = current.pos + acStack_24;
    for (int i = 0; i < 5; i++) {
        dComIfGp_particle_set(particle_id[i], &cStack_30, NULL, &scale, 0xff, 0, 0xffffffff, NULL,
                              NULL, NULL);
    }
}

int daRfHole_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daRfHole_c::Delete() {
    dComIfG_resDelete(&mPhase, "Otosiana");
    return 1;
}

static int daRfHole_Draw(daRfHole_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daRfHole_Execute(daRfHole_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daRfHole_Delete(daRfHole_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daRfHole_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daRfHole_c*>(i_this)->create();
}

static actor_method_class l_daRfHole_Method = {
    (process_method_func)daRfHole_Create,
    (process_method_func)daRfHole_Delete,
    (process_method_func)daRfHole_Execute,
    NULL,
    (process_method_func)daRfHole_Draw,
};

extern actor_process_profile_definition g_profile_Obj_RfHole = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_RfHole,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daRfHole_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  514,                    // mPriority
  &l_daRfHole_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
