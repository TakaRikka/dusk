/**
 * @file d_a_obj_magLift.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_magLift.h"
#include "d/d_path.h"

static daMagLift_HIO_c l_HIO;

daMagLift_HIO_c::daMagLift_HIO_c() {
    field_0x4 = 0x3c;
    field_0x5 = 1;
}

void daMagLift_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(current.angle.x, current.angle.y, current.angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daMagLift_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("L_maglift", 4);

    if (modelData == NULL) {
        // FIXME: For shield decomp matching, needs a JUT assert.
    }

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

const f32 daMagLift_c::mSpeed[16] = {
    5.0f,       6.6666665f, 8.333333f,  10.0f,      11.666667f, 13.333333f, 15.0f,      16.666666f,
    18.333334f, 20.0f,      21.666666f, 23.333334f, 25.0f,      26.666666f, 28.333334f, 3.3333333f,
};

int daMagLift_c::create() {
    fopAcM_ct(this, daMagLift_c);
    int phase = dComIfG_resLoad(&mPhaseReq,"L_maglift");
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("L_maglift", 7, dBgS_MoveBGProc_TypicalRotY, 0x4000, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }
        field_0x5ae = fopAcM_GetParam(this);
        dPath* path = dPath_GetRoomPath(field_0x5ae, fopAcM_GetRoomNo(this));
        if (path == NULL) {
            return cPhs_INIT_e;
        }
        current.pos = path->m_points->m_position;
        field_0x5af = 1;
        mPoint = 0;
        mMoveSpeed = mSpeed[getMoveSpeed()];
        fopAcM_SetSpeedF(this, 0.0f);
        u8 mInitMove = fopAcM_GetParam(this) >> 0xc & 0xFF;
        if (mInitMove == 0xff) {
            init_modeWaitInit();
        } else {
            is_switch = dComIfGs_isSwitch(mInitMove, fopAcM_GetHomeRoomNo(this));
            if (is_switch) {
                init_modeWaitInit();
            } else {
                init_modeMoveWait();
            }
        }
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());
        setBaseMtx();
    }
    return phase;
}

int daMagLift_c::Execute(Mtx** i_mtx) {
    moveLift();
    *i_mtx = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

void daMagLift_c::moveLift() {
    if (field_0x5ae != 0xff) {
        typedef void (daMagLift_c::*modeFunc)();
        static modeFunc mode_proc[8] = {&daMagLift_c::modeAcc, &daMagLift_c::modeMove,
                                       &daMagLift_c::modeBrk, &daMagLift_c::modeWait,
                                       &daMagLift_c::modeWaitInit, &daMagLift_c::modeDead,
                                       &daMagLift_c::modeMoveWait};

        (this->*mode_proc[mMode])();
    }
}

void daMagLift_c::modeAcc() {
    if (TRUE == cLib_chaseF(&speedF, mMoveSpeed, mMoveSpeed / 30.0f) || current.pos == field_0x5b4) {
        init_modeMove();
    }
    cLib_addCalcPos(&current.pos, field_0x5b4, 1.0f, fopAcM_GetSpeedF(this), 0.1f);
}

void daMagLift_c::init_modeMove() {
    mMode = MODE_MOVE_e;
}

void daMagLift_c::modeMove() {
    cXyz sp54(field_0x5c0);
    cXyz sp60(field_0x5b4);

    f32 magn = (sp54 - sp60).abs() * 0.3f;
    f32 min = 30.0f;
    f32 max = 300.0f;

    if (magn < min) {
        magn = min;
    }

    if (magn > max) {
        magn = max;
    }

    f32 o_magn = (current.pos - field_0x5b4).abs();
    if (magn > o_magn) {
        init_modeBrk();
    }
    cLib_addCalcPos(&current.pos, field_0x5b4, 1.0f, fopAcM_GetSpeedF(this), 0.1f);
}

void daMagLift_c::init_modeBrk() {
    mMode = MODE_BRK_e;
}

void daMagLift_c::modeBrk() {
    f32 val = cLib_addCalcPos(&current.pos, field_0x5b4, 0.1f, fopAcM_GetSpeedF(this), 0.5f);
    if (val == 0.0f) {
        fopAcM_SetSpeedF(this, 0.0f);
        init_modeWaitInit();
    }
}

void daMagLift_c::init_modeWaitInit() {
    mMode = MODE_WAIT_INIT_e;
}

void daMagLift_c::modeWaitInit() {
    setNextPoint();
    init_modeWait();
}

void daMagLift_c::init_modeWait() {
    mMode = MODE_WAIT_e;
}

void daMagLift_c::modeWait() {
    mMode = MODE_ACC_e;
}

void daMagLift_c::init_modeMoveWait() {
    mMode = MODE_MOVE_WAIT_e;
}

void daMagLift_c::modeMoveWait() {
    if (fopAcM_isSwitch(this, (fopAcM_GetParam(this) & 0xff000) >> 0xc) & 0xff) {
        init_modeWaitInit();
    }
}

void daMagLift_c::modeDead() {
    /* empty function */
}

void daMagLift_c::setNextPoint() {
    s16 sVar1 = mPoint + field_0x5af;
    dPath* path = dPath_GetRoomPath(field_0x5ae, fopAcM_GetRoomNo(this));
    if (dPath_ChkClose(path) != 0) {
        int val = path->m_num - 1;
        if (sVar1 > val) {
            sVar1 = 0;
        } else if (sVar1 < 0) {
            sVar1 = (s8) val;
        }
    } else {
        if (sVar1 > path->m_num - 1) {
            field_0x5af = 0xff;
            sVar1 = (s8) (path->m_num - 2);
        } else if (sVar1 < 0) {
            field_0x5af = 0x1;
            sVar1 = 1;
        } else {
            dPnt* pnt = &path->m_points[mPoint];
            if (pnt->mArg0 == 0) {
                sVar1 = mPoint;
            }
        }
    }
    field_0x5c0 = field_0x5b4;
    dPnt* pnt = &path->m_points[sVar1];
    field_0x5b4 = pnt->m_position;
    mPoint = sVar1;
}

int daMagLift_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daMagLift_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "L_maglift");
    return 1;
}

static int daMagLift_Draw(daMagLift_c* i_this) {
    return i_this->Draw();
}

static int daMagLift_Execute(daMagLift_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daMagLift_Delete(daMagLift_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daMagLift_Create(fopAc_ac_c* i_this) {
    return static_cast<daMagLift_c*>(i_this)->create();
}

static actor_method_class l_daMagLift_Method = {
    (process_method_func) daMagLift_Create,
    (process_method_func) daMagLift_Delete,
    (process_method_func) daMagLift_Execute,
    (process_method_func) NULL,
    (process_method_func) daMagLift_Draw,
};

actor_process_profile_definition g_profile_Obj_MagLift = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_MagLift,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daMagLift_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  492,                    // mPriority
  &l_daMagLift_Method,    // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
