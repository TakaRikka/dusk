/**
 * d_a_obj_heavySw.cpp
 * Object - Heavy Switch (Triggered by Player wearing Iron Boots)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_heavySw.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_procname.h"

daHeavySw_HIO_c::daHeavySw_HIO_c() {
    field_0x04 = 150.0f;
    field_0x08 = 450.0f;
    field_0x0c = 100.0f;
    field_0x10 = 50.0f;
    field_0x14 = 20.0f;
    field_0x28 = 0.0f;
    field_0x2c = 20.0f;
    field_0x18 = 0.0f;
    field_0x1c = 0.8f;
    field_0x20 = 0.0f;
    field_0x24 = 15.0f;
    field_0x30 = 30;
    field_0x31 = 8;
    field_0x32 = 30;
    field_0x33 = 5;
    field_0x34 = 4;
}

void daHeavySw_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y - field_0x5bc, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::ZXYrotM(field_0x5cc.x, field_0x5cc.y, field_0x5cc.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daHeavySw_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("Hswitch", 4);
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel != NULL) {
        return 1;
    }
    return 0;
}

int daHeavySw_c::create() {
    fopAcM_ct(this, daHeavySw_c);

    int phase = dComIfG_resLoad(&mPhaseReq, "Hswitch");
    if (phase == cPhs_COMPLEATE_e) {
        int movebg_create = MoveBGCreate("Hswitch", 7, dBgS_MoveBGProc_TypicalRotY, 0xC80, NULL);
        if (movebg_create == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        mpBgW->SetRideCallback(rideCallBack);
        mAcchCir.SetWall(0.0f, 0.0f);
        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                     fopAcM_GetSpeed_p(this), NULL, NULL);
        field_0x5bc = 0.0f;
        field_0x5b4 = 0.0f;
        field_0x5c8 = 0.0f;
        field_0x5cc.x = 0;
        field_0x5cc.y = 0;
        field_0x5cc.z = 0;
        u8 param = fopAcM_GetParam(this);
        if (fopAcM_isSwitch(this, param)) {
            field_0x5bc = 300.0f;
            init_modeMoveEnd();
        } else {
            field_0x5c5 = 0;
            init_modeWait();
        }
        setBaseMtx();
    }
    return phase;
}

void daHeavySw_c::rideCallBack(dBgW* param_0, fopAc_ac_c* param_1, fopAc_ac_c* param_2) {
    if (fopAcM_GetName(param_2) == PROC_ALINK) {
        daHeavySw_c* heavySw = static_cast<daHeavySw_c*>(param_1);
        heavySw->field_0x5d8 = 1;
        heavySw->field_0x5dc =
            static_cast<daPy_py_c*>(dComIfGp_getLinkPlayer())->checkEquipHeavyBoots();
    }
}

int daHeavySw_c::Execute(Mtx** i_mtx) {
    moveSwitch();
    mObjAcch.CrrPos(dComIfG_Bgsp());
    *i_mtx = &mpModel->getBaseTRMtx();
    setBaseMtx();
    field_0x5d8 = 0;
    return 1;
}

static daHeavySw_HIO_c l_HIO;

void daHeavySw_c::moveSwitch() {
    typedef void (daHeavySw_c::*modeProcessFunc)();
    static modeProcessFunc mode_proc[] = {
        &daHeavySw_c::modeWait, &daHeavySw_c::modeRide,    &daHeavySw_c::modeMoveInit,
        &daHeavySw_c::modeMove, &daHeavySw_c::modeMoveEnd,
    };

    (this->*mode_proc[mMode])();
    if (field_0x5d8 == 0 && field_0x5c5 == 0) {
        if (mMode != MODE_WAIT) {
            init_modeWait();
            field_0x7fc = 0;
            field_0x5b8 = field_0x5bc;
        }
        if (field_0x7fc == 0 && field_0x5bc < field_0x5b8 / 2) {
            s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
            mDoAud_seStart(Z2SE_OBJ_HEAVYSW_UP, &current.pos, 0, reverb);
            field_0x7fc = 1;
        }
        cLib_addCalc0(&field_0x5bc, 0.3f, 10.0f);
        if (field_0x5bc < 0.01f) {
            field_0x5bc = 0.0f;
        }
    }
    f32 cos = cM_scos(field_0x5c8 * 0x3000);
    field_0x5cc.x = field_0x5b0 * cos;
    f32 sin = cM_ssin(field_0x5c8 * 0x3000);
    field_0x5cc.y = field_0x5b0 * sin;
    cLib_addCalc0(&field_0x5b0, 0.1f, 10.0f);
    field_0x5c8++;
}

void daHeavySw_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daHeavySw_c::modeWait() {
    if (field_0x5d8 != 0) {
        init_modeRide();
    }
}

void daHeavySw_c::init_modeRide() {
    field_0x5c4 = l_HIO.field_0x33;
    fopAcM_SetSpeedF(this, l_HIO.field_0x18);
    field_0x5d4 = l_HIO.field_0x1c;
    mMode = MODE_RIDE;
}

void daHeavySw_c::modeRide() {
    if (field_0x5c4 != 0) {
        field_0x5c4--;
        if (field_0x5c4 == 0) {
            field_0x5b0 = l_HIO.field_0x04;
        }
    } else {
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mDoAud_seStartLevel(Z2SE_OBJ_HEAVYSW_MOVE, &current.pos, 0, reverb);
        cLib_chaseF(&speedF, field_0x5d4, field_0x5d4 / 30.0f);
        f32 calc = cLib_addCalc(&field_0x5bc, l_HIO.field_0x10, 1.0f, fopAcM_GetSpeedF(this), 0.1f);
        if (calc == 0.0f) {
            mDoAud_seStop(Z2SE_OBJ_HEAVYSW_MOVE, 0);
        }
        if (field_0x5dc != 0) {
            init_modeMoveInit();
        }
    }
}

void daHeavySw_c::init_modeMoveInit() {
    fopAcM_SetSpeedF(this, l_HIO.field_0x20);
    field_0x5d4 = l_HIO.field_0x24;
    field_0x5c4 = l_HIO.field_0x31;
    mMode = MODE_MOVE_INIT;
}

void daHeavySw_c::modeMoveInit() {
    if (field_0x5dc == 0) {
        init_modeRide();
    }
    if (field_0x5c4 != 0) {
        field_0x5c4--;
        if (field_0x5c4 == 0) {
            field_0x5b0 = l_HIO.field_0x08;
            s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
            mDoAud_seStart(Z2SE_OBJ_HEAVYSW_ON, &current.pos, 0, reverb);
        }
    } else {
        cLib_chaseF(&speedF, field_0x5d4, field_0x5d4 / 30.0f);
        f32 calc = cLib_addCalc(&field_0x5bc, l_HIO.field_0x10 + l_HIO.field_0x14, 1.0f,
                                fopAcM_GetSpeedF(this), 0.1f);
        if (calc == 0.0f) {
            init_modeMove();
        }
    }
}

void daHeavySw_c::init_modeMove() {
    field_0x5b0 = l_HIO.field_0x0c;
    field_0x5c4 = l_HIO.field_0x30;
    fopAcM_SetSpeedF(this, l_HIO.field_0x28);
    field_0x5d4 = l_HIO.field_0x2c;
    field_0x7fc = 0;
    mMode = MODE_MOVE;
}

void daHeavySw_c::modeMove() {
    if (field_0x5dc == 0) {
        init_modeRide();
    }
    if (field_0x5c4 != 0) {
        field_0x5c4--;
    } else {
        if (field_0x7fc == 0) {
            s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
            mDoAud_seStart(Z2SE_OBJ_HEAVYSW_DOWN, &current.pos, 0, reverb);
            field_0x7fc = 1;
        }
        cLib_chaseF(&speedF, field_0x5d4, field_0x5d4 / 30.0f);
        f32 calc = cLib_addCalc(&field_0x5bc, 300.0f, 1.0f, fopAcM_GetSpeedF(this), 0.1f);
        if (calc == 0.0f) {
            init_modeMoveEnd();
        }
    }
}

void daHeavySw_c::init_modeMoveEnd() {
    field_0x5c5 = 1;
    u8 param = fopAcM_GetParam(this);
    if (!fopAcM_isSwitch(this, param)) {
        s8 reverb = dComIfGp_getReverb(fopAcM_GetRoomNo(this));
        mDoAud_seStart(Z2SE_OBJ_HEAVYSW_STOP, &current.pos, 0, reverb);
        dComIfGp_getVibration().StartShock(l_HIO.field_0x34, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    }
    field_0x5c4 = l_HIO.field_0x32;
    mMode = MODE_MOVE_END;
}

void daHeavySw_c::modeMoveEnd() {
    if (field_0x5c4 != 0) {
        field_0x5c4--;
    } else {
        BOOL is_switch = fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xff);
        if (!is_switch) {
            fopAcM_onSwitch(this, fopAcM_GetParam(this) & 0xff);
        }
    }
}

int daHeavySw_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    cXyz curPos = cXyz(current.pos.x + 170.0f, current.pos.y, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, true, mpModel, &curPos, 500.0f, 0.0f, current.pos.y,
                                    mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

int daHeavySw_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "Hswitch");
    return 1;
}

static int daHeavySw_Draw(daHeavySw_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daHeavySw_Execute(daHeavySw_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daHeavySw_Delete(daHeavySw_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daHeavySw_Create(fopAc_ac_c* i_this) {
    return static_cast<daHeavySw_c*>(i_this)->create();
}

static actor_method_class l_daHeavySw_Method = {
    (process_method_func)daHeavySw_Create,  (process_method_func)daHeavySw_Delete,
    (process_method_func)daHeavySw_Execute, (process_method_func)NULL,
    (process_method_func)daHeavySw_Draw,
};

extern actor_process_profile_definition g_profile_Obj_HeavySw = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_HeavySw,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daHeavySw_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    503,                     // mPriority
    &l_daHeavySw_Method,     // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
