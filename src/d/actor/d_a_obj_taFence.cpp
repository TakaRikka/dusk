/**
 * @file d_a_obj_taFence.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_taFence.h"
#include "d/actor/d_a_player.h"
#include "d/d_lib.h"
#include "d/d_s_play.h"

class daTaFence_HIO_c : public mDoHIO_entry_c {
public:
    daTaFence_HIO_c();
    virtual ~daTaFence_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 fall_init_speed;
    /* 0x08 */ f32 fall_max_speed;
    /* 0x0C */ u8 vibration;
    /* 0x10 */ f32 pre_fall_shake_intensity;
    /* 0x14 */ f32 pre_fall_shakeYZ_ampZ;
    /* 0x18 */ f32 pre_fall_shakeYZ_ampY;
    /* 0x1C */ f32 pre_fall_shakeYZ_damp;
    /* 0x20 */ f32 pre_fall_shakeYZ_max_damp;
    /* 0x24 */ f32 pre_fall_shakeYZ_min_damp;
    /* 0x28 */ f32 pre_fall_shakeXZ_ampX;
    /* 0x2C */ f32 pre_fall_shakeXZ_ampZ;
    /* 0x30 */ f32 pre_fall_shakeXZ_intensity;
    /* 0x34 */ f32 pre_fall_shakeXZ_damp;
    /* 0x38 */ f32 pre_fall_shakeXZ_max_damp;
    /* 0x3C */ f32 pre_fall_shakeXZ_min_damp;
    /* 0x40 */ f32 sword_hit_shakeYZ_intensity;
    /* 0x44 */ f32 sword_hit_shakeYZ_ampZ;
    /* 0x48 */ f32 sword_hit_shakeYZ_ampY;
    /* 0x4C */ f32 sword_hit_shakeYZ_damp;
    /* 0x50 */ f32 sword_hit_shakeYZ_max_damp;
    /* 0x54 */ f32 sword_hit_shakeYZ_min_damp;
    /* 0x58 */ f32 sword_hit_shakeXZ_ampX;
    /* 0x5C */ f32 sword_hit_shakeXZ_ampZ;
    /* 0x60 */ f32 sword_hit_shakeXZ_intensity;
    /* 0x64 */ f32 sword_hit_shakeXZ_damp;
    /* 0x68 */ f32 sword_hit_shakeXZ_max_damp;
    /* 0x6C */ f32 sword_hit_shakeXZ_min_damp;
    /* 0x70 */ u8 collapse_time;
    /* 0x71 */ u8 shake_time;
};

static daTaFence_HIO_c l_HIO;

daTaFence_HIO_c::daTaFence_HIO_c() {
    fall_init_speed = 0.0f;
    fall_max_speed = 13.0f;
    vibration = 4;

    pre_fall_shake_intensity = 200.0f;
    pre_fall_shakeYZ_ampZ = 90.0f;
    pre_fall_shakeYZ_ampY = 45.0f;
    pre_fall_shakeYZ_damp = 0.1f;
    pre_fall_shakeYZ_max_damp = 10.0f;
    pre_fall_shakeYZ_min_damp = 1.0f;
    pre_fall_shakeXZ_intensity = 10.0f;
    pre_fall_shakeXZ_ampX = 90.0f;
    pre_fall_shakeXZ_ampZ = 45.0f;
    pre_fall_shakeXZ_damp = 0.1f;
    pre_fall_shakeXZ_max_damp = 10.0f;
    pre_fall_shakeXZ_min_damp = 1.0f;

    sword_hit_shakeYZ_intensity = 100.0f;
    sword_hit_shakeYZ_ampZ = 90.0f;
    sword_hit_shakeYZ_ampY = 45.0f;
    sword_hit_shakeYZ_damp = 0.3f;
    sword_hit_shakeYZ_max_damp = 1.5f;
    sword_hit_shakeYZ_min_damp = 1.0f;
    sword_hit_shakeXZ_intensity = 12.0f;
    sword_hit_shakeXZ_ampX = 90.0f;
    sword_hit_shakeXZ_ampZ = 45.0f;
    sword_hit_shakeXZ_damp = 0.3f;
    sword_hit_shakeXZ_max_damp = 1.0f;
    sword_hit_shakeXZ_min_damp = 0.5f;

    collapse_time = 10;
    shake_time = 27;
}

void daTaFence_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::ZXYrotM(mShakeYZ.x, mShakeYZ.y, mShakeYZ.z);
    mDoMtx_stack_c::transM(mShakeXZ.x, mShakeXZ.y, mShakeXZ.z);
    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daTaFence_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("P_Mfence", 4);
    JUT_ASSERT(339, modelData != NULL);

    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

const dCcD_SrcGObjInf daTaFence_c::mCcDObjInfo = {
    {0, {{0x0, 0x0, 0x0}, {0x400032, 0x11}, {0x0}}},
    {dCcD_SE_SWORD, 0, 0, 0, 0},
    {dCcD_SE_STONE, 0, 0, 0, 0},
    {0},
};

dCcD_SrcCyl daTaFence_c::mCcDCyl = {
    daTaFence_c::mCcDObjInfo,
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            0.0f,  // mRadius
            0.0f  // mHeight
        }  // mCyl
    }
};

int daTaFence_c::create() {
    fopAcM_ct(this, daTaFence_c);

    int phase_state = dComIfG_resLoad(&mPhase, "P_Mfence");
    if (phase_state == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("P_Mfence", 7, dBgS_MoveBGProc_TypicalRotY, 0x1500, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel->getModelData());

        mCcStts.Init(0xFF, 0xFF, this);

        if (fopAcM_isSwitch(this, fopAcM_GetParam(this) & 0xFF)) {
            if (fopAcM_isSwitch(this, (fopAcM_GetParam(this) & 0xFF00) >> 8)) {
                shape_angle.x += 0x8000;
            }

            if (mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBgW);
            }

            init_modeMoveEnd();
        } else {
            shape_angle.x += 0x4000;

            for (int i = 0; i < 11; i++) {
                mCyl[i].Set(mCcDCyl);
                mCyl[i].SetStts(&mCcStts);
            }

            mShakeYZ.x = 0;
            mShakeYZ.y = 0;
            mShakeYZ.z = 0;

            mShakeYZ_intensity = 0.0f;

            mShakeXZ.x = 0.0f;
            mShakeXZ.y = 0.0f;
            mShakeXZ.z = 0.0f;

            mShakeXZ_intensity = 0.0f;
            mShakeYZ_ampY = 0.0f;
            mShakeYZ_ampZ = 0.0f;
            mShakeYZ_damp = 0.0f;
            mShakeYZ_maxDamp = 0.0f;
            mShakeYZ_minDamp = 0.0f;

            mShakeXZ_ampX = 0.0f;
            mShakeXZ_ampZ = 0.0f;
            mShakeXZ_damp = 0.0f;
            mShakeXZ_maxDamp = 0.0f;
            mShakeXZ_minDamp = 0.0f;

            mCounter = 0;
            mCollapseTimer = 0;
            mShakeTimer = 0;

            init_modeWait();
        }

        setBaseMtx();

        #if DEBUG
        l_HIO.entryHIO("体当たり金網");
        #endif
    }

    return phase_state;
}

int daTaFence_c::Execute(Mtx** param_0) {
    fenceProc();
    *param_0 = &mpModel->getBaseTRMtx();
    setBaseMtx();
    return 1;
}

static const Vec l_check_area[4] = {
    {-225.0f, 0.0f, -100.0f},
    {-225.0f, 225.0f, -100.0f},
    {225.0f, 225.0f, 100.0f},
    {225.0f, 0.0f, 100.0f},
};

void daTaFence_c::fenceProc() {
    static void (daTaFence_c::*mode_proc[])() = {
        &daTaFence_c::modeWait,
        &daTaFence_c::modeMoveInit,
        &daTaFence_c::modeMove,
        &daTaFence_c::modeMoveEnd,
    };

    (this->*mode_proc[mMode])();

    mShakeYZ.z = mShakeYZ_intensity * cM_scos(mCounter * (s16)cM_deg2s(mShakeYZ_ampY));
    mShakeYZ.y = mShakeYZ_intensity * cM_ssin(mCounter * (s16)cM_deg2s(mShakeYZ_ampZ));
    cLib_addCalc(&mShakeYZ_intensity, 0.0f, mShakeYZ_damp, mShakeYZ_maxDamp, mShakeYZ_minDamp);

    mShakeXZ.x = mShakeXZ_intensity * cM_scos(mCounter * (s16)cM_deg2s(mShakeXZ_ampX));
    mShakeXZ.y = mShakeXZ_intensity * cM_ssin(mCounter * (s16)cM_deg2s(mShakeXZ_ampZ));
    cLib_addCalc(&mShakeXZ_intensity, 0.0f, mShakeXZ_damp, mShakeXZ_maxDamp, mShakeXZ_minDamp);

    mCounter++;
}

void daTaFence_c::init_modeWait() {
    mMode = 0;
}

void daTaFence_c::modeWait() {
    daPy_py_c* player = dComIfGp_getLinkPlayer();
    if (player->checkFrontRollCrash()) {
        cXyz sp2C;
        cXyz sp20;
        sp2C = l_check_area[0];
        sp20 = l_check_area[2];

        BOOL pl_in_area = dLib_checkActorInRectangle(player, this, &sp2C, &sp20);
        if (pl_in_area) {
            init_modeMoveInit();
        }
    }

    for (int i = 0; i < 11; i++) {
        if (mCyl[i].ChkTgHit()) {
            dCcD_GObjInf* tg_hitgobj = mCyl[i].GetTgHitGObj();

            if (tg_hitgobj->GetAtType() & (AT_TYPE_IRON_BALL | AT_TYPE_BOMB | AT_TYPE_SHIELD_ATTACK)) {
                init_modeMoveInit();
            } else if (tg_hitgobj->ChkAtType(AT_TYPE_NORMAL_SWORD) && mShakeTimer == 0) {
                mShakeYZ_intensity = l_HIO.sword_hit_shakeYZ_intensity;
                mShakeYZ_ampY = l_HIO.sword_hit_shakeYZ_ampY;
                mShakeYZ_ampZ = l_HIO.sword_hit_shakeYZ_ampZ;
                mShakeYZ_damp = l_HIO.sword_hit_shakeYZ_damp;
                mShakeYZ_maxDamp = l_HIO.sword_hit_shakeYZ_max_damp;
                mShakeYZ_minDamp = l_HIO.sword_hit_shakeYZ_min_damp;

                mShakeXZ_intensity = l_HIO.sword_hit_shakeXZ_intensity;
                mShakeXZ_ampX = l_HIO.sword_hit_shakeXZ_ampX;
                mShakeXZ_ampZ = l_HIO.sword_hit_shakeXZ_ampZ;
                mShakeXZ_damp = l_HIO.sword_hit_shakeXZ_damp;
                mShakeXZ_maxDamp = l_HIO.sword_hit_shakeXZ_max_damp;
                mShakeXZ_minDamp = l_HIO.sword_hit_shakeXZ_min_damp;

                mShakeTimer = l_HIO.shake_time;
                break;
            }
        }

        mCyl[i].ClrTgHit();
    }

    for (int i = 0; i < 11; i++) {
        mCyl[i].SetR(25.0f + oREG_F(0));
        mCyl[i].SetH(250.0f + oREG_F(1));

        cXyz pos(current.pos);
        pos.x = (pos.x + (i * (41.0f + oREG_F(2)))) - 205.0f;
        mCyl[i].SetC(pos);
        dComIfG_Ccsp()->Set(&mCyl[i]);
    }

    if (mShakeTimer != 0) {
        mShakeTimer--;
    }
}

void daTaFence_c::init_modeMoveInit() {
    if (mCollapseTimer == 0) {
        mCollapseTimer = l_HIO.collapse_time;
    }

    mShakeYZ_intensity = l_HIO.pre_fall_shake_intensity;
    mShakeYZ_ampY = l_HIO.pre_fall_shakeYZ_ampY;
    mShakeYZ_ampZ = l_HIO.pre_fall_shakeYZ_ampZ;
    mShakeYZ_damp = l_HIO.pre_fall_shakeYZ_damp;
    mShakeYZ_maxDamp = l_HIO.pre_fall_shakeYZ_max_damp;
    mShakeYZ_minDamp = l_HIO.pre_fall_shakeYZ_min_damp;

    mShakeXZ_intensity = l_HIO.pre_fall_shakeXZ_intensity;
    mShakeXZ_ampX = l_HIO.pre_fall_shakeXZ_ampX;
    mShakeXZ_ampZ = l_HIO.pre_fall_shakeXZ_ampZ;
    mShakeXZ_damp = l_HIO.pre_fall_shakeXZ_damp;
    mShakeXZ_maxDamp = l_HIO.pre_fall_shakeXZ_max_damp;
    mShakeXZ_minDamp = l_HIO.pre_fall_shakeXZ_min_damp;

    mDoAud_seStart(Z2SE_OBJ_ATK_FENCE_HIT, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mMode = 1;
}

void daTaFence_c::modeMoveInit() {
    if (mCollapseTimer == 0) {
        init_modeMove();
    } else {
        mCollapseTimer--;
    }
}

void daTaFence_c::init_modeMove() {
    for (int i = 0; i < 11; i++) {
        mCyl[i].OffCoSetBit();
    }

    if (mpBgW->ChkUsed()) {
        int result = dComIfG_Bgsp().Release(mpBgW);
    }

    mFallSpeed = l_HIO.fall_max_speed;

    fopAcM_SetSpeedF(this, l_HIO.fall_init_speed);

    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz sp8 = player->current.pos - current.pos;
    mDoMtx_stack_c::YrotS(-current.angle.y);
    mDoMtx_stack_c::multVec(&sp8, &sp8);

    if (sp8.z < 0.0f) {
        field_0x1384 = -0x8000;
    } else {
        field_0x1384 = 0;
    }

    mMode = 2;
}

void daTaFence_c::modeMove() {
    cLib_chaseF(&speedF, mFallSpeed, mFallSpeed / 30.0f);
    if (cLib_addCalcAngleS(&shape_angle.x, field_0x1384, 1, (182.04445f * fopAcM_GetSpeedF(this)), 1) == 0) {
        init_modeMoveEnd();
    }
}

void daTaFence_c::init_modeMoveEnd() {
    mDoAud_seStart(Z2SE_OBJ_ATK_FENCE_FALLDOWN, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dComIfGp_getVibration().StartShock(l_HIO.vibration, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    fopAcM_onSwitch(this, fopAcM_GetParam(this) & 0xFF);

    if (field_0x1384 == 0) {
        fopAcM_offSwitch(this, (fopAcM_GetParam(this) & 0xFF00) >> 8);
    } else {
        fopAcM_onSwitch(this, (fopAcM_GetParam(this) & 0xFF00) >> 8);
    }

    mMode = 3;
}

void daTaFence_c::modeMoveEnd() {}

int daTaFence_c::Draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daTaFence_c::Delete() {
    dComIfG_resDelete(&mPhase, "P_Mfence");
    #if DEBUG
    l_HIO.removeHIO();
    #endif
    return 1;
}

static int daTaFence_Draw(daTaFence_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daTaFence_Execute(daTaFence_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daTaFence_Delete(daTaFence_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daTaFence_Create(fopAc_ac_c* i_this) {
    return ((daTaFence_c*)i_this)->create();
}

static actor_method_class l_daTaFence_Method = {
    (process_method_func)daTaFence_Create,
    (process_method_func)daTaFence_Delete,
    (process_method_func)daTaFence_Execute,
    NULL,
    (process_method_func)daTaFence_Draw,
};

actor_process_profile_definition g_profile_Obj_TaFence = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_TaFence,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daTaFence_c),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  505,                    // mPriority
  &l_daTaFence_Method,    // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
