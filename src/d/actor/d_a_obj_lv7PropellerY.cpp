/**
 * @file d_a_obj_lv7PropellerY.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv7PropellerY.h"

class daPropY_HIO_c : public mDoHIO_entry_c {
public:
    daPropY_HIO_c();
    virtual ~daPropY_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 rot_speed;
    /* 0x08 */ f32 rot_accel;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 max_rot_speed;
    /* 0x14 */ f32 rot_time_sec;
    /* 0x18 */ u8 wait_time;
    /* 0x1C */ f32 y_offset;
};

static char* l_type[2] = {
    "stickwl00",
    "stickwl01",
};

static int l_bmdIdx[] = {4, 4};

static int l_dzbIdx[] = {7, 7};

daPropY_HIO_c::daPropY_HIO_c() {
    rot_speed = 0.0f;
    rot_accel = 0.1f;
    field_0xc = 0.003f;
    max_rot_speed = 1.0f;
    rot_time_sec = 20.0f;
    wait_time = 30;
    y_offset = 0.0f;
}

static daPropY_HIO_c l_HIO;

void daPropY_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + l_HIO.y_offset, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daPropY_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_type[mType], l_bmdIdx[mType]);
    JUT_ASSERT(184, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daPropY_c::create() {
    fopAcM_ct(this, daPropY_c);

    mType = getType();

    int phase_state = dComIfG_resLoad(&mPhase, l_type[mType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate(l_type[mType], l_dzbIdx[mType], dBgS_MoveBGProc_TypicalRotY, 0x2A40, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        setBaseMtx();

        mSwbit = getSw();
        mTurnType = getTurnType();
        mTurnVec = getTurnVec();
        if (mTurnVec > 1) {
            mTurnVec = 0;
        }

        if (mTurnVec == 0) {
            field_0x5b2 = -1;
            field_0x5b6 = -0x8000;
        } else {
            field_0x5b2 = 1;
            field_0x5b6 = 0;
        }

        if (mSwbit == 0xFF) {
            if (mTurnType == 0) {
                init_modeMove();
            } else {
                init_modeWait();
            }
        } else {
            mIsSwitch = fopAcM_isSwitch(this, mSwbit);
            if (mIsSwitch) {
                if (mTurnType == 0) {
                    init_modeMove();
                } else {
                    init_modeWait();
                }
            } else {
                init_modeMoveWait();
            }
        }

        #if DEBUG
        l_HIO.entryHIO("プロペラ横");
        #endif
    }

    return phase_state;
}

int daPropY_c::Execute(Mtx** param_0) {
    procMain();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daPropY_c::procMain() {
    static void (daPropY_c::*mode_proc[])() = {
        &daPropY_c::modeMoveWait,
        &daPropY_c::modeMove,
        &daPropY_c::modeWait,
        &daPropY_c::modeMove2,
        &daPropY_c::modeStop,
    };

    if (mSwbit != 0xFF) {
        u8 prev_switch = mIsSwitch;
        mIsSwitch = fopAcM_isSwitch(this, mSwbit);
        if (mIsSwitch != prev_switch) {
            if (mIsSwitch) {
                if (mTurnType == 0) {
                    init_modeMove();
                } else {
                    init_modeMove2();
                }
            } else {
                init_modeStop();
            }
        }
    }

    (this->*mode_proc[mMode])();
}

void daPropY_c::init_modeMoveWait() {
    mMode = 0;
}

void daPropY_c::modeMoveWait() {}

void daPropY_c::init_modeMove() {
    fopAcM_SetSpeedF(this, 0.0f);
    field_0x5c4 = 0.0f;
    mMode = 1;
}

void daPropY_c::modeMove() {
    cLib_chaseF(&speedF, 360.0f / (30.0f * l_HIO.rot_time_sec), l_HIO.field_0xc);
    field_0x5b4 = cM_deg2s(speedF);
    field_0x5c4 = 127.0f * (speedF / (360.0f / (30.0f * l_HIO.rot_time_sec)));
    mDoAud_seStartLevel(Z2SE_OBJ_AMI_ROLL, &current.pos, field_0x5c4, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (l_HIO.rot_time_sec != 0.0f) {
        shape_angle.y += (field_0x5b4 * field_0x5b2);
    }
}

void daPropY_c::init_modeWait() {
    mTimer = l_HIO.wait_time;
    mMode = 2;
}

void daPropY_c::modeWait() {
    if (mTimer == 0) {
        init_modeMove2();
    } else {
        mTimer--;
    }
}

void daPropY_c::init_modeMove2() {
    fopAcM_SetSpeedF(this, l_HIO.rot_speed);
    field_0x5c4 = 0.0f;
    mMode = 3;
}

void daPropY_c::modeMove2() {
    cLib_chaseF(&speedF, l_HIO.max_rot_speed, l_HIO.rot_accel);
    s16 temp_r30 = cLib_addCalcAngleS(&shape_angle.y, field_0x5b6, 1, (182.04445f * fopAcM_GetSpeedF(this)), 1);
    field_0x5c4 = 127.0f * (speedF / l_HIO.max_rot_speed);
    mDoAud_seStartLevel(Z2SE_OBJ_AMI_ROLL, &current.pos, field_0x5c4, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (temp_r30 == 0) {
        if (mTurnVec == 0) {
            field_0x5b6 -= 0x8000;
        } else {
            field_0x5b6 += 0x7FFF;
        }
        init_modeWait();
    }
}

void daPropY_c::init_modeStop() {
    if (mTurnType == 0) {
        fopAcM_SetSpeedF(this, cM_deg2s(360.0f / (30.0f * l_HIO.rot_time_sec)));
    }

    if (mTurnVec == 0) {
        if (shape_angle.y <= 0) {
            field_0x5b6 = home.angle.y - 0x8000;
        } else {
            field_0x5b6 = home.angle.y;
        }
    } else if (shape_angle.y >= 0) {
        field_0x5b6 = home.angle.y + 0x7FFF;
    } else {
        field_0x5b6 = home.angle.y;
    }

    field_0x5bc = std::fabs((f32)shape_angle.y - (f32)field_0x5b6);
    field_0x5c0 = field_0x5c4;
    mMode = 4;
}

void daPropY_c::modeStop() {
    s16 var_r30;
    if (mTurnType == 0) {
        var_r30 = cLib_addCalcAngleS(&shape_angle.y, field_0x5b6, 16, fopAcM_GetSpeedF(this), cM_deg2s(0.1f));
    } else {
        var_r30 = cLib_addCalcAngleS(&shape_angle.y, field_0x5b6, 1, (182.04445f * fopAcM_GetSpeedF(this)), 1);
    }

    // probably not a real version difference - GCN has fsub for some reason instead of fsubs
#if PLATFORM_GCN
    field_0x5c4 = field_0x5c0 * ((f32)std::fabs((f64)var_r30) / field_0x5bc);
#else
    field_0x5c4 = field_0x5c0 * (std::fabs((f32)var_r30) / field_0x5bc);
#endif
    mDoAud_seStartLevel(Z2SE_OBJ_AMI_ROLL, &current.pos, field_0x5c4, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (var_r30 == 0) {
        init_modeMoveWait();
    }
}

int daPropY_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daPropY_c::Delete() {
    dComIfG_resDelete(&mPhase, l_type[mType]);
    #if DEBUG
    l_HIO.removeHIO();
    #endif
    return 1;
}

static int daPropY_Draw(daPropY_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daPropY_Execute(daPropY_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daPropY_Delete(daPropY_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daPropY_Create(fopAc_ac_c* i_this) {
    return ((daPropY_c*)i_this)->create();
}

static actor_method_class l_daPropY_Method = {
    (process_method_func)daPropY_Create,
    (process_method_func)daPropY_Delete,
    (process_method_func)daPropY_Execute,
    NULL,
    (process_method_func)daPropY_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv7PropY = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Lv7PropY,      // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daPropY_c),      // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  641,                    // mPriority
  &l_daPropY_Method,      // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
