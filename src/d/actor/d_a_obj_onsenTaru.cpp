/**
 * @file d_a_obj_onsenTaru.cpp
 * Object - Hotspring Water Barrel
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_onsenTaru.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "f_op/f_op_kankyo_mng.h"

daOnsTaru_HIO_c::daOnsTaru_HIO_c() {
    mCoolTime = 120;
    mVibration = 3;
    mSmokeScale = 1.0f;
    mCollisionRadius = 125.0f;
}

static daOnsTaru_HIO_c l_HIO;

const dCcD_SrcGObjInf daOnsTaru_c::mCcDObjInfo = {
    {0, {{8, 0, 0x1F}, {0xD8FADDAF, 0x1F}, {0x79}}},
    {dCcD_SE_NONE, 0, 1, 0, 0},
    {dCcD_SE_NONE, 0, 0, 0, 2},
    {0},
};

dCcD_SrcCyl daOnsTaru_c::mCcDCyl = {
    daOnsTaru_c::mCcDObjInfo,
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }  // mCyl
};

void daOnsTaru_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daOnsTaru_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("maroTaru", 3);
    JUT_ASSERT(231, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

int daOnsTaru_c::create() {
    fopAcM_ct(this, daOnsTaru_c);

    if (fopAcM_isSwitch(this, getSw())) {
        return cPhs_ERROR_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, "maroTaru");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0xF00)) {
            return cPhs_ERROR_e;
        }

        setBaseMtx();
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mAcchCir.SetWall(110.0f, 50.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);

        fopAcM_SetGravity(this, -7.0f);
        fopAcM_SetMaxFallSpeed(this, -100.0f);
        fopAcM_SetModel(this, mpModel);

        mCcStts.Init(200, 0xFF, this);
        mCcCyl.Set(mCcDCyl);
        mCcCyl.SetStts(&mCcStts);
        mCcCyl.OffAtSetBit();

        fopAcM_OnCarryType(this, fopAcM_CARRY_TYPE_1);
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
        attention_info.distances[fopAc_attn_CARRY_e] = 42;
        mIsCarryNow = fopAcM_checkCarryNow(this);

        mode_init_wait();
        mCollisionRadius = 50.0f;
        mTimer = 0;
        mStartTimer = false;
        mTempStat = 1;
        field_0x93e = 1;
    }

    return phase_state;
}

int daOnsTaru_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return ((daOnsTaru_c*)i_this)->CreateHeap();
}

BOOL daOnsTaru_c::chkSinkAll() {
    return mAcch.m_wtr.GetHeight() > current.pos.y + 100.0f;
}

BOOL daOnsTaru_c::chkWaterLineIn() {
    return mAcch.m_wtr.GetHeight() > current.pos.y + 40.0f;
}

void daOnsTaru_c::bgCheck() {
    bool roof_hit = mAcch.ChkRoofHit();
    bool wall_hit = mAcch.ChkWallHit();
    bool gnd_land = mAcch.ChkGroundLanding() != 0;
    bool gnd_hit = mAcch.ChkGroundHit() != 0;
    bool wtr_hit = mAcch.ChkWaterHit() != 0;
    bool wtr_in = mAcch.ChkWaterIn() != 0;

    if (mMode == MODE_DROP_e) {
        if (wall_hit || gnd_land || roof_hit) {
            mode_init_break();
        }
    } else if (gnd_hit) {
        if (!wtr_hit && field_0x93e == 0) {
            fopAcM_seStart(this, JA_SE_OBJ_PUT_WBOX_S, 0);
            mCcCyl.OnCoSetBit();
            field_0x93e = 1;
        }
    } else {
        field_0x93e = 0;
    }

    if (mMode == MODE_WAIT_e || mMode == MODE_DROP_e || mMode == MODE_CARRY_e) {
        f32 water_height = mAcch.m_wtr.GetHeight();
        if (wtr_hit) {
            f32 var_f30 = speed.y;

            if (chkWaterLineIn() && mMode != MODE_CARRY_e) {
                mode_init_sink();
            }

            if (wtr_in && !mInWater) {
                cXyz sp5C(current.pos);
                sp5C.y = water_height;

                if (var_f30 < -15.0f) {
                    fopKyM_createWpillar(&sp5C, 2.0f, 0);
                    fopAcM_seStart(this, Z2SE_OBJ_FALL_WATER_M, 0);
                }
            }
        }
    }

    cXyz pos(current.pos);
    pos.y = mAcch.m_wtr.GetHeight();

    if (pos.y > current.pos.y &&
        ((mMode == MODE_SINK_e && !chkSinkAll()) || (mMode == MODE_WAIT_e && gnd_hit) ||
         (mMode == MODE_CARRY_e && wtr_in && !chkSinkAll())))
    {
        fopAcM_effHamonSet(&mHamonEmtrID, &pos, 1.0f, 0.1f);
    }
}

int daOnsTaru_c::Execute() {
    field_0x594 = speed;
    mode_proc_call();

    mPrevPos = current.pos;
    mAcch.CrrPos(dComIfG_Bgsp());
    mGrabOffset = current.pos - mPrevPos;

    if (mMode == MODE_CARRY_e) {
        current.pos = mPrevPos;
    }

    bgCheck();
    attention_info.position.x = current.pos.x;
    attention_info.position.y = current.pos.y + 50.0f;
    attention_info.position.z = current.pos.z;
    eyePos = attention_info.position;

    mInWater = mAcch.ChkWaterIn();
    setBaseMtx();

    if (mCcCyl.ChkAtHit()) {
        if (mCcCyl.GetAtHitGObj()->GetTgType() & 0xD8FBFDFF) {
            mode_init_break();
        }
    } else if (mCcCyl.ChkTgHit()) {
        if (mCcCyl.GetTgHitGObj()->GetAtType() & 0xD8FBFDFF) {
            mode_init_break();
        }
    }

    mCcCyl.SetR(mCollisionRadius + oREG_F(2));
    mCcCyl.SetH(110.0f + oREG_F(3));
    mCcCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCyl);
    mCcStts.Move();

    if (mStartTimer) {
        if (mTimer < (s16)(l_HIO.mCoolTime * 30)) {
            if (!g_dComIfG_gameInfo.play.mEvent.runCheck()) {
                mTimer++;
            }

            cXyz sp2C(current.pos);
            sp2C.y += 100.0f;
        } else {
            mTempStat = 0;
            mStartTimer = false;
        }
    }

    return 1;
}

void daOnsTaru_c::mode_proc_call() {
    typedef void (daOnsTaru_c::*mode_func)();
    static mode_func l_mode_func[] = {
        &daOnsTaru_c::mode_proc_wait,  &daOnsTaru_c::mode_proc_carry, &daOnsTaru_c::mode_proc_drop,
        &daOnsTaru_c::mode_proc_break, &daOnsTaru_c::mode_proc_sink,
    };

    if (fopAcM_checkCarryNow(this) && !mIsCarryNow) {
        mode_init_carry();
    }

    (this->*l_mode_func[mMode])();

    mIsCarryNow = fopAcM_checkCarryNow(this);
}

void daOnsTaru_c::mode_init_wait() {
    cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    speedF = 0.0f;
    gravity = -7.0f;
    mMode = MODE_WAIT_e;
}

void daOnsTaru_c::mode_proc_wait() {
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
}

void daOnsTaru_c::mode_init_carry() {
    mCcCyl.OffCoSetBit();
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    speedF = 0.0f;
    mMode = MODE_CARRY_e;
}

void daOnsTaru_c::mode_proc_carry() {
    daPy_getPlayerActorClass()->setGrabCollisionOffset(mGrabOffset.x, mGrabOffset.z, NULL);

    if (!fopAcM_checkCarryNow(this)) {
        if (chkWaterLineIn()) {
            mode_init_sink();
        } else if (fopAcM_GetSpeedF(this) != 0.0f) {
            mode_init_drop();
        } else {
            mode_init_wait();
        }
    }
}

void daOnsTaru_c::mode_init_drop() {
    mCcCyl.OnAtSetBit();
    mCollisionRadius = l_HIO.mCollisionRadius;
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    gravity = -7.0f;
    mMode = MODE_DROP_e;
}

void daOnsTaru_c::mode_proc_drop() {
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
}

void daOnsTaru_c::mode_init_break() {
    mDoAud_seStart(Z2SE_OBJ_ONSEN_TARU_BRK, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dComIfGp_getVibration().StartShock(l_HIO.mVibration, 15, cXyz(0.0f, 1.0f, 0.0f));
    breakEffSet();

    fopAcM_onSwitch(this, getSw2());
    fopAcM_delete(this);
    mMode = MODE_BREAK_e;
}

void daOnsTaru_c::mode_proc_break() {}

void daOnsTaru_c::mode_init_sink() {
    cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    mMode = MODE_SINK_e;
}

void daOnsTaru_c::mode_proc_sink() {
    cLib_addCalc(&speedF, 0.0f, 0.05f, 0.1f, 0.01f);
    speed.y = cLib_minMaxLimit<f32>(speed.y, -15.0f, 13.0f);

    if (mAcch.ChkGroundHit()) {
        speedF *= 0.9f;
    }

    if (chkWaterLineIn()) {
        gravity = -7.0f;
    } else {
        gravity = -6.0f;
    }

    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());

    if (mAcch.ChkGroundHit()) {
        cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    } else {
        cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_CARRY_e);
    }
}

void daOnsTaru_c::breakEffSet() {
    dComIfGp_particle_set(0x8A9A, &current.pos, NULL, NULL);
    dComIfGp_particle_set(0x8A9B, &current.pos, NULL, NULL);
    dComIfGp_particle_set(0x8A9C, &current.pos, NULL, NULL);
    dComIfGp_particle_set(0x8A9D, &current.pos, NULL, NULL);

    if (mTempStat != 0) {
        dComIfGp_particle_set(0x8A9E, &current.pos, NULL, NULL);
    }

    cXyz pos(current.pos);
    cXyz scale(0.75f, 0.75f, 0.75f);

    J3DModelData* kibako_bmd = (J3DModelData*)dComIfG_getObjectRes("Always", "BreakWoodBox.bmd");
    JUT_ASSERT(784, kibako_bmd != NULL);

    JPABaseEmitter* emitter =
        dComIfGp_particle_set(0x8A99, &pos, NULL, NULL, 0xFF, &dPa_modelEcallBack::getEcallback(),
                              fopAcM_GetRoomNo(this), NULL, NULL, &scale);
    if (emitter != NULL) {
        dPa_modelEcallBack::setModel(emitter, kibako_bmd, tevStr, 3, NULL, 0, 0);
    }
}

int daOnsTaru_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daOnsTaru_c::Delete() {
    dComIfG_resDelete(&mPhase, "maroTaru");
    return 1;
}

static int daOnsTaru_Draw(daOnsTaru_c* i_this) {
    return i_this->Draw();
}

static int daOnsTaru_Execute(daOnsTaru_c* i_this) {
    return i_this->Execute();
}

static int daOnsTaru_Delete(daOnsTaru_c* i_this) {
    return i_this->Delete();
}

static int daOnsTaru_Create(fopAc_ac_c* i_this) {
    return ((daOnsTaru_c*)i_this)->create();
}

static actor_method_class l_daOnsTaru_Method = {
    (process_method_func)daOnsTaru_Create,  (process_method_func)daOnsTaru_Delete,
    (process_method_func)daOnsTaru_Execute, (process_method_func)NULL,
    (process_method_func)daOnsTaru_Draw,
};

extern actor_process_profile_definition g_profile_Obj_OnsenTaru = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_OnsenTaru,      // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daOnsTaru_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    510,                     // mPriority
    &l_daOnsTaru_Method,     // sub_method
    0x00040020,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
