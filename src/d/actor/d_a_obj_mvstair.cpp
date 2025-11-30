/**
 * @file d_a_obj_mvstair.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_mvstair.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

static const int l_bmdName[] = {4, 4};

static const int l_dzbName[] = {7, 7};

static const f32 l_move_len[] = {
    150.0f,
    300.0f,
    450.0f,
    600.0f,
    500.0f,
    550.0f,
};

static const u32 l_heap_size[] = {0xB40, 0x2C60};

void daObjStair_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjStair_c::setBaseMtx() {
    mDoMtx_stack_c::transS(mModelPos.x, mModelPos.y, mModelPos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjStair_c::Create() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());

    mEventId = dComIfGp_getEventManager().getEventIdx(this, getEvNo());
    u8 swbit = getSwbit();
    u8 swbit2 = getSwbit2();

    if (fopAcM_isSwitch(this, swbit2)) {
        setAction(ACTION_END_e);
        modeWaitLowerInit();
    } else {
        if (getEvNo() == 0xFF) {
            setAction(ACTION_END_e);
        } else {
            setAction(ACTION_WAIT_e);
        }

        BOOL is_swbit = fopAcM_isSwitch(this, swbit);
        int dir = getDir();

        if ((dir == 0 && is_swbit) || (dir == 1 && !is_swbit)) {
            modeWaitLowerInit();
        } else {
            modeWaitUpperInit();
        }
    }

    initBaseMtx();
    mIsSwitch = fopAcM_isSwitch(this, getSwbit());
    mSoundPos = home.pos;
    mSoundPos.y = 600.0f + home.pos.y;
    return 1;
}

static char* l_arcName[2] = {
    "K_mvkai00",
    "Lv9_mvkai",
};

int daObjStair_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[mResType], l_bmdName[mResType]);
    JUT_ASSERT(250, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daObjStair_c::create1st() {
    mType = getType();
    if (getMdlType() == 7 || getMdlType() == 0) {
        mResType = 0;
    } else if (getMdlType() == 1) {
        mResType = 1;
    }

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName[mResType]);
    if (phase_state == cPhs_COMPLEATE_e) {
        phase_state = MoveBGCreate(l_arcName[mResType], l_dzbName[mResType], dBgS_MoveBGProc_TypicalRotY, l_heap_size[mResType], NULL);
        if (phase_state == cPhs_ERROR_e) {
            return phase_state;
        }

        OS_REPORT("MVSTAIR PARAM:0x%x\n", fopAcM_GetParam(this));
    }

    return phase_state;
}

void daObjStair_c::event_proc_call() {
    static int (daObjStair_c::*l_func[])() = {
        &daObjStair_c::actionWait,
        &daObjStair_c::actionOrderEvent,
        &daObjStair_c::actionEvent,
        &daObjStair_c::actionEnd,
    };

    (this->*l_func[mAction])();
}

void daObjStair_c::mode_proc_call() {
    static int (daObjStair_c::*l_func[])() = {
        &daObjStair_c::modeWaitUpper,
        &daObjStair_c::modeUpper,
        &daObjStair_c::modeWaitLower,
        &daObjStair_c::modeLower,
    };

    (this->*l_func[mMode])();
}

int daObjStair_c::Execute(Mtx** param_0) {
    event_proc_call();
    mode_proc_call();

    *param_0 = &mBgMtx;
    setBaseMtx();

    mIsSwitch = fopAcM_isSwitch(this, getSwbit());

    mParticleLeftPos[0].set(0.0f, 0.0f, -140.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&mParticleLeftPos[0], &mParticleLeftPos[0]);

    mParticleLeftPos[1].set(0.0f, 0.0f, 140.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&mParticleLeftPos[1], &mParticleLeftPos[1]);

    mParticleRightPos[0].set(-450.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&mParticleRightPos[0], &mParticleRightPos[0]);

    mParticleRightPos[1].set(450.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&mParticleRightPos[1], &mParticleRightPos[1]);
    return 1;
}

int daObjStair_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daObjStair_c::Delete() {
    if (mSmokeEmitter != NULL) {
        mSmokeEmitter->becomeInvalidEmitter();
        mSmokeEmitter = NULL;
    }

    removeParticle();
    dComIfG_resDelete(&mPhase, l_arcName[mResType]);
    return 1;
}

int daObjStair_c::actionWait() {
    if (fopAcM_isSwitch(this, getSwbit2())) {
        setAction(ACTION_END_e);
        return 1;
    }

    if (mIsSwitch != fopAcM_isSwitch(this, getSwbit())) {
        setAction(ACTION_ORDER_EVENT_e);
        fopAcM_orderOtherEventId(this, mEventId, getEvNo(), 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }

    return 1;
}

int daObjStair_c::actionOrderEvent() {
    if (eventInfo.checkCommandDemoAccrpt()) {
        setAction(ACTION_EVENT_e);
    } else {
        fopAcM_orderOtherEventId(this, mEventId, getEvNo(), 0xFFFF, 0, 1);
        eventInfo.onCondition(2);
    }

    return 1;
}

int daObjStair_c::actionEvent() {
    if (dComIfGp_evmng_endCheck(mEventId)) {
        setAction(ACTION_WAIT_e);
        dComIfGp_event_reset();
    }

    return 1;
}

int daObjStair_c::actionEnd() {
    return 1;
}

int daObjStair_c::modeWaitUpperInit() {
    current.pos.y = home.pos.y;
    mModelPos = current.pos;
    mMode = MODE_WAIT_UPPER_e;
    return 1;
}

int daObjStair_c::modeWaitUpper() {
    int is_swbit = fopAcM_isSwitch(this, getSwbit());
    int dir = getDir();

    if (!fopAcM_isSwitch(this, getSwbit2()) && ((is_swbit && dir == 0) || (!is_swbit && dir == 1))) {
        modeUpperInit();
    }

    return 1;
}

int daObjStair_c::modeUpperInit() {
    mMoveStartShakeTimer = 30;
    mMoveEndShakeTimer = 15;
    mMoveStartShakeAmount = 3.0f;
    mMoveMode = 0;
    mMoveAccel = 0.0f;
    mMoveStep = 0.0f;
    mMode = MODE_UPPER_e;
    return 1;
}

int daObjStair_c::modeUpper() {
    f32 move_speed = 50.0f;
    s16 shake_speed = 15000;

    switch (mMoveMode) {
    case 0:
        if (mMoveStartShakeTimer == 0) {
            if (mSmokeEmitter == NULL) {
                cXyz effpos(home.pos);
                effpos.y += 600.0f;
                mSmokeEmitter = dComIfGp_particle_set(0x833E, &effpos, &home.angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
            }
            setParticle();
            mMoveMode = 1;
        } else if (mMoveStartShakeTimer == 2 && getMdlType() != 1) {
            mDoAud_seStart(Z2SE_OBJ_STEP_MOVESTART, &mSoundPos, 0, 0);
        }

        mMoveStartShakeTimer--;
        break;
    case 1: {
        cLib_chaseF(&mMoveAccel, 0.45f, 0.5f + KREG_F(0));
        cLib_chaseF(&mMoveStep, move_speed, mMoveAccel);

        int is_move_end = cLib_chaseF(&current.pos.y, home.pos.y + l_move_len[mType], mMoveStep);
        mModelPos.y = current.pos.y;

        if (is_move_end) {
            mMoveMode = 2;
            if (mSmokeEmitter != NULL) {
                mSmokeEmitter->becomeInvalidEmitter();
                mSmokeEmitter = NULL;
            }

            removeParticle();

            if (getMdlType() != 1) {
                mDoAud_seStart(Z2SE_OBJ_STEP_MOVEEND, &mSoundPos, 0, 0);
            } else {
                mDoAud_seStart(Z2SE_OBJ_L6_UD_BLOCK_UP_ST, &mSoundPos, 0, 0);
            }
        } else if (getMdlType() != 1) {
            mDoAud_seStartLevel(Z2SE_OBJ_STEP_MOVE, &mSoundPos, 0, 0);
        } else {
            mDoAud_seStartLevel(Z2SE_OBJ_L6_UD_BLOCK_UP, &mSoundPos, 0, 0);
        }

        mModelPos = current.pos;
        break;
    }
    case 2:
        dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        mMoveMode = 3;
        mMoveStartShakeTimer = 15;
        break;
    case 3:
        mMoveStartShakeTimer--;
        mModelPos = current.pos;

        if (mMoveStartShakeTimer == 0) {
            modeWaitLowerInit();
        }
        break;
    }

    mModelPos.y += mMoveStartShakeAmount * cM_scos(mMoveStartShakeTimer * shake_speed);
    cLib_addCalc0(&mMoveStartShakeAmount, 0.5f + KREG_F(1), 0.5f + KREG_F(3));
    return 1;
}

int daObjStair_c::modeWaitLowerInit() {
    current.pos.y = home.pos.y + l_move_len[mType];
    mModelPos = current.pos;
    mMode = MODE_WAIT_LOWER_e;
    return 1;
}

int daObjStair_c::modeWaitLower() {
    int is_swbit = fopAcM_isSwitch(this, getSwbit());
    int dir = getDir();

    if (!fopAcM_isSwitch(this, getSwbit2()) && ((!is_swbit && dir == 0) || (is_swbit && dir == 1))) {
        modeLowerInit();
    }

    return 1;
}

int daObjStair_c::modeLowerInit() {
    mMoveAccel = 0.0f;
    mMoveStep = 0.0f;
    mMoveMode = 0;
    mMoveStartShakeTimer = 30;
    mMode = MODE_LOWER_e;
    return 1;
}

int daObjStair_c::modeLower() {
    f32 move_speed = 50.0f;
    f32 shake_amount = 3.0f;
    s16 shake_speed = 15000;

    if (mMoveMode == 0) {
        if (mMoveStartShakeTimer == 0) {
            cLib_chaseF(&mMoveAccel, 0.45f, 0.5f + KREG_F(4));
            cLib_chaseF(&mMoveStep, move_speed, mMoveAccel);

            int is_move_end = cLib_chaseF(&current.pos.y, home.pos.y, mMoveStep);
            mModelPos.y = current.pos.y;

            if (is_move_end) {
                mMoveMode = 1;
                if (mSmokeEmitter != NULL) {
                    mSmokeEmitter->becomeInvalidEmitter();
                    mSmokeEmitter = NULL;
                }

                if (getMdlType() != 1) {
                    mDoAud_seStart(Z2SE_OBJ_STEP_MOVEEND, &mSoundPos, 0, 0);
                } else {
                    mDoAud_seStart(Z2SE_OBJ_L6_UD_BLOCK_DW_ST, &mSoundPos, 0, 0);
                }
            } else if (getMdlType() != 1) {
                mDoAud_seStartLevel(Z2SE_OBJ_STEP_MOVE, &mSoundPos, 0, 0);
            } else {
                mDoAud_seStartLevel(Z2SE_OBJ_L6_UD_BLOCK_DW, &mSoundPos, 0, 0);
            }
        } else {
            mMoveStartShakeTimer--;
            if (mMoveStartShakeTimer == 0) {
                if (mSmokeEmitter == NULL) {
                    cXyz effpos(home.pos);
                    effpos.y += 600.0f;
                    mSmokeEmitter = dComIfGp_particle_set(0x833E, &effpos, &home.angle, NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                }
            } else if (mMoveStartShakeTimer == 2 && getMdlType() != 1) {
                mDoAud_seStart(Z2SE_OBJ_STEP_MOVESTART, &mSoundPos, 0, 0);
            }
        }
    } else {
        if (mMoveStartShakeTimer == 0 && mMoveMode == 1) {
            mMoveStartShakeTimer = 15;
            mMoveMode = 2;
            dComIfGp_getVibration().StartShock(4, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        }

        mMoveStartShakeTimer--;
        mModelPos = current.pos;
        mModelPos.y += shake_amount * cM_scos(mMoveStartShakeTimer * shake_speed);

        if (mMoveStartShakeTimer == 0) {
            modeWaitUpperInit();
        }
    }

    return 1;
}

void daObjStair_c::setParticle() {
    if (mParticleLeftEmitter[0].getEmitter() == NULL) {
        dComIfGp_particle_set(0x833C, &mParticleLeftPos[0], &home.angle, NULL, 0xFF, &mParticleLeftEmitter[0], fopAcM_GetRoomNo(this), NULL, NULL, NULL);
    }

    if (mParticleLeftEmitter[1].getEmitter() == NULL) {
        dComIfGp_particle_set(0x833C, &mParticleLeftPos[1], &home.angle, NULL, 0xFF, &mParticleLeftEmitter[1], fopAcM_GetRoomNo(this), NULL, NULL, NULL);
    }

    if (mParticleRightEmitter[0].getEmitter() == NULL) {
        dComIfGp_particle_set(0x833D, &mParticleRightPos[0], &home.angle, NULL, 0xFF, &mParticleRightEmitter[0], fopAcM_GetRoomNo(this), NULL, NULL, NULL);
    }

    if (mParticleRightEmitter[1].getEmitter() == NULL) {
        dComIfGp_particle_set(0x833D, &mParticleRightPos[1], &home.angle, NULL, 0xFF, &mParticleRightEmitter[1], fopAcM_GetRoomNo(this), NULL, NULL, NULL);
        // "Up/Down Stairs: Right Effect set"
        OS_REPORT("上下階段：右エフェクトセット\n");
    }
}

void daObjStair_c::removeParticle() {
    for (int i = 0; i < 2; i++) {
        mParticleLeftEmitter[i].remove();
        mParticleRightEmitter[i].remove();
    }
}

static int daObjStair_create1st(daObjStair_c* i_this) {
    fopAcM_ct(i_this, daObjStair_c);
    return i_this->create1st();
}

static int daObjStair_MoveBGDelete(daObjStair_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjStair_MoveBGExecute(daObjStair_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjStair_MoveBGDraw(daObjStair_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjStair_METHODS = {
    (process_method_func)daObjStair_create1st,
    (process_method_func)daObjStair_MoveBGDelete,
    (process_method_func)daObjStair_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjStair_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_MvStair = {
    fpcLy_CURRENT_e,        // mLayerID
    3,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_MvStair,       // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjStair_c),   // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    25,                     // mPriority
    &daObjStair_METHODS,    // sub_method
    0x00040100,             // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
