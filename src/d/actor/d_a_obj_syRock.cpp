/**
 * @file d_a_obj_syRock.cpp
 *
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_syRock.h"
#include "d/actor/d_a_obj_eff.h"
#include "f_op/f_op_kankyo_mng.h"
#include "d/d_s_play.h"

typedef void (daSyRock_c::*actionFunc)();

class daSyRock_HIO_c : public mDoHIO_entry_c {
public:
    daSyRock_HIO_c();
    ~daSyRock_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 mShakeAmplitude;
    /* 0x08 */ f32 mShakeXOscAngQuantum;
    /* 0x0C */ f32 mShakeZOscAngQuantum;
    /* 0x10 */ f32 mShakeDampingScale;
    /* 0x14 */ f32 mShakeMaxDecay;
    /* 0x18 */ f32 mShakeMinDecay;
    /* 0x1C */ f32 mFallAcceleration;
    /* 0x20 */ f32 mMaxFallSpeed;
    /* 0x24 */ u8 mWaitFrames;
    /* 0x25 */ u8 mVibModePower;
    /* 0x28 */ f32 mEffScale;
    /* 0x2C */ f32 mFallWaterBouyancy;
    /* 0x30 */ f32 mRange; // Maximum horizontal distance a d_a_obj_waterPillar instance may be from the current syRock instance in order for the two to be associated
};

static daSyRock_HIO_c l_HIO;

dCcD_SrcCyl daSyRock_c::mCcDCyl = {mCcDObjInfo};

daSyRock_HIO_c::daSyRock_HIO_c() {
    mShakeAmplitude = 450.0f;
    mShakeXOscAngQuantum = 90.0f;
    mShakeZOscAngQuantum = 45.0f;
    mShakeDampingScale = 1.0f / 20.0f;
    mShakeMaxDecay = 5.0f;
    mShakeMinDecay = 1.0f / 10.0f;
    mFallAcceleration = 10.0f;
    mMaxFallSpeed = 200.0f;
    mWaitFrames = 30;
    mEffScale = 4.0f;
    mVibModePower = VIBMODE_S_POWER4;
    mFallWaterBouyancy = 3.0f;
    mRange = 5.0f;
}

#if DEBUG
void daSyRock_HIO_c::genMessage(JORMContext* mctx) {
    /* ==== Falling ==== */
    mctx->genLabel("\n==== 落下 ====", 0);
    // Wait time
    mctx->genSlider("wait time", &mWaitFrames, 0, 0xFF);
    // Fall speed
    mctx->genSlider("落下速度", &mFallAcceleration, 0.0f, 100.0f);
    // Maximum fall speed
    mctx->genSlider("最大落下速度", &mMaxFallSpeed, 0.1f, 500.0f);
    // Effect scale
    mctx->genSlider("eff scale", &mEffScale, 0.1f, 20.0f);

    /* ==== Shaking ==== */
    mctx->genLabel("\n==== 揺れ ====", 0);
    // Shaking intensity
    mctx->genSlider("揺れ強さ", &mShakeAmplitude, 0.0f, 10000.0f);
    // Z Amplitude
    mctx->genSlider("振幅Ｚ", &mShakeZOscAngQuantum, 0.0f, 180.0f);
    // X Amplitude
    mctx->genSlider("振幅Ｘ", &mShakeXOscAngQuantum, 0.0f, 180.0f);
    // Shake damping
    mctx->genSlider("揺れ減衰", &mShakeDampingScale, 0.0f, 1.0f);
    // Maximum damping amount
    mctx->genSlider("最大減衰量", &mShakeMaxDecay, 0.0f, 100.0f);
    // Minimum damping amount
    mctx->genSlider("最小減衰量", &mShakeMinDecay, 0.0f, 100.0f);

    /* Vibration */
    mctx->startComboBox("振動", &mVibModePower);
    // Strength 1
    mctx->genComboBoxItem("強さ１", VIBMODE_S_POWER1);
    // Strength 2
    mctx->genComboBoxItem("強さ２", VIBMODE_S_POWER2);
    // Strength 3
    mctx->genComboBoxItem("強さ３", VIBMODE_S_POWER3);
    // Strength 4
    mctx->genComboBoxItem("強さ４", VIBMODE_S_POWER4);
    // Strength 5
    mctx->genComboBoxItem("強さ５", VIBMODE_S_POWER5);
    // Strength 6
    mctx->genComboBoxItem("強さ６", VIBMODE_S_POWER6);
    // Strength 7
    mctx->genComboBoxItem("強さ７", VIBMODE_S_POWER7);
    // Strength 8
    mctx->genComboBoxItem("強さ８", VIBMODE_S_POWER8);
    mctx->endComboBox();

    /* ==== For Testing Purposes ==== */
    mctx->genLabel("\n==== テスト用 ====", 0);
    // Range
    mctx->genSlider("renge", &mRange, 0.1f, 20.0f);
    // Sink speed
    mctx->genSlider("sink speed", &mFallWaterBouyancy, 0.1f, 20.0f);
}
#endif

void daSyRock_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(0, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::ZXYrotM(mShakeAngOffset.x, mShakeAngOffset.y, mShakeAngOffset.z);
    mDoMtx_stack_c::transM(mUnderwaterOffset.x, mUnderwaterOffset.y, mUnderwaterOffset.z);

    mpModels[mIsUnbroken]->setBaseScale(scale);

    // Set base transform matrix of current model and the background matrix both to the contents of now
    mpModels[mIsUnbroken]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daSyRock_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("syourock", 4));
    JUT_ASSERT(271, modelData != NULL);
    mpModels[0] = mDoExt_J3DModel__create(modelData, 1 << 19, 0x11000084);

    if (!mpModels[0]) {
        return 0;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("syourock", 5));
    JUT_ASSERT(282, modelData != NULL);
    mpModels[1] = mDoExt_J3DModel__create(modelData, 1 << 19, 0x11000084);

    if (!mpModels[1]) {
        return 0;
    }

    mpBrokenCollision = new dBgW;

    if (!mpBrokenCollision || mpBrokenCollision->Set(static_cast<cBgD_t*>(dComIfG_getObjectRes("syourock", 9)), 1, &mBgMtx)) {
        mpBrokenCollision = NULL;
        return 0;
    }

    return 1;
}

dCcD_SrcGObjInf const daSyRock_c::mCcDObjInfo = {
    {0x0, {{AT_TYPE_BOMB, 0x1, 0x1F}, {AT_TYPE_IRON_BALL | AT_TYPE_BOMB, 0x11}, 0x79}},    // mObj
    {dCcD_SE_SWORD, 0x0, 0x1, dCcD_MTRL_NONE, 0x0},      // mGObjAt
    {dCcD_SE_STONE, 0x0, 0x0, dCcD_MTRL_NONE, 0x2},      // mGObjTg
    {},  // mGObjCo
};

cPhs_Step daSyRock_c::create() {
    fopAcM_ct(this, daSyRock_c);

    // Check if an associated water pillar should be searched for
    if (getArg0() == TRUE) {
        mpWaterPillar = static_cast<daWtPillar_c*>(fopAcM_Search(searchWaterPillar, this));

        if (!mpWaterPillar) {
            return cPhs_INIT_e;
        }
    }

    cPhs_Step bgCreatePhaseProcess;

    const cPhs_Step requestedPhaseProcess = static_cast<cPhs_Step>(dComIfG_resLoad(&mPhase, "syourock"));
    if (requestedPhaseProcess == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("syourock", 0x8, dBgS_MoveBGProc_TypicalRotY, 0x2100, NULL) == cPhs_ERROR_e) {
            return cPhs_ERROR_e;
        }

        mAcchCir.SetWall(150.0f, 150.0f);
        mAcch.Set(fopAcM_GetPosition_p(this),
                    fopAcM_GetOldPosition_p(this),
                    this, 0x1, &mAcchCir,
                    fopAcM_GetSpeed_p(this),
                    NULL, NULL);
        mAcch.SetWaterCheckOffset(10000.0f);

        mShakeOscAngStep = 0;

        mShakeAngOffset.x = 0;
        mShakeAngOffset.y = 0;
        mShakeAngOffset.z = 0;

        mShakeAmplitude = 0.0f;
        mShakeZOscAngQuantum = 0.0f;
        mShakeXOscAngQuantum = 0.0f;

        mShakeDampingScale =  0.0f;
        mShakeMaxDecay = 0.0f;
        mShakeMinDecay = 0.0f;

        mUnderwaterOffset.x = 0.0f;
        mUnderwaterOffset.y = 0.0f;
        mUnderwaterOffset.z = 0.0f;

        mStts.Init(0xFF, 0xFF, this);
        mUnbrokenCollider.Set(mCcDCyl);
        mUnbrokenCollider.SetStts(&mStts);
        mUnbrokenCollider.OffAtSetBit();

        if (fopAcM_isSwitch(this, getSwBit1())) {
            mIsUnbroken = false;
            mUnbrokenCollider.OffCoSetBit();
            if (mpBrokenCollision && mpBrokenCollision->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBrokenCollision);
                mpBrokenCollision->Move();
            }

            // Snap already broken stalactite to its appropriate location and set its mode accordingly
            setFallStat();
        } else {
            mIsUnbroken = true;

            if (mpBgW)
                dComIfG_Bgsp().Release(mpBgW);

            if (mpBrokenCollision) {
                dComIfG_Bgsp().Regist(mpBrokenCollision, this);
                mpBrokenCollision->Move();
            }

            mAutoDropSwNo = getSwBit2();
            init_modeWait();

        }

        field_0x779 = 2;
        fopAcM_SetMtx(this, mpModels[mIsUnbroken]->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModels[mIsUnbroken]->getModelData());
        setBaseMtx();

        #if DEBUG
        l_HIO.entryHIO("鍾乳石"); // "Stalactite"
        #endif
    }

    return requestedPhaseProcess;
}

void daSyRock_c::setFallStat() {
    mpWaterPillar = static_cast<daWtPillar_c*>(fopAcM_Search(searchWaterPillar, this));
    if (mpWaterPillar) {
        OS_REPORT("== 下に水柱あり ==\n"); // "== Water pillar below =="

        mpWaterPillar->onRockFlag();
        mMode = MODE_MOVE;
    } else {
        OS_REPORT("== 下に水柱無し ==\n"); // "== No water pillar below =="

        mAcch.CrrPos(dComIfG_Bgsp());

        if (mAcch.ChkWaterHit()) {
            if (mAcch.m_wtr.GetHeight() - mAcch.GetGroundH() >= 200.0f) {
                mUnderwaterOffset.x = 150.0f;
                shape_angle.z = 0x4000;
            }
            current.pos.y = mAcch.GetGroundH();
        } else {
            current.pos.y = mAcch.GetGroundH();
        }

        mAcch.CrrPos(dComIfG_Bgsp());
        mMode = MODE_DROP_END;
    }
}

int daSyRock_c::Execute(Mtx** i_mtx) {
    eventUpdate();
    move();

    *i_mtx = &mpModels[mIsUnbroken]->getBaseTRMtx();

    mInWaterPrevFrame = mAcch.ChkWaterIn() != false;
    setBaseMtx();

    return 1;
}

void daSyRock_c::move() {
    static const actionFunc mode_proc[] = {
    &daSyRock_c::modeWait, &daSyRock_c::modeDropInit,
    &daSyRock_c::modeDrop, &daSyRock_c::modeSink,
    &daSyRock_c::modeMove, &daSyRock_c::modeDropEnd
    };
    (this->*mode_proc[mMode])();


    mShakeAngOffset.x = mShakeAmplitude * cM_ssin(mShakeOscAngStep * cM_deg2s(mShakeXOscAngQuantum));
    mShakeAngOffset.z = mShakeAmplitude * cM_scos(mShakeOscAngStep * cM_deg2s(mShakeZOscAngQuantum));

    cLib_addCalc(&mShakeAmplitude, 0.0f, mShakeDampingScale, mShakeMaxDecay, mShakeMinDecay);

    mShakeOscAngStep++;

    mUnbrokenCollider.SetR(150.0f + oREG_F(0));
    mUnbrokenCollider.SetH(680.0f + oREG_F(1));

    cXyz currentPos = current.pos;
    currentPos.y -= 250.0f + oREG_F(2);

    mUnbrokenCollider.SetC(currentPos);

    dComIfG_Ccsp()->Set(&mUnbrokenCollider);
}

void daSyRock_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daSyRock_c::modeWait() {
    if (mAutoDropSwNo != 0xFF && fopAcM_isSwitch(this, mAutoDropSwNo)) {
        field_0x779 = 0;
        if (getEvetID() != 0xFF) {
            orderEvent(getEvetID(), 0xFF, 1);
        } else {
            eventStart();
        }
        return;
    }

    if (mUnbrokenCollider.ChkTgHit()) {
        dCcD_GObjInf* tgHitGObj = mUnbrokenCollider.GetTgHitGObj();
        if (tgHitGObj->GetAtType() == AT_TYPE_BOMB) {
            field_0x779 = 0;
            if (getEvetID() != 0xFF) {
                orderEvent(getEvetID(), 0xFF, 1);
            } else {
                eventStart();
            }
        }
        mUnbrokenCollider.ClrTgHit();
    }
}

bool daSyRock_c::eventStart() {
    // field_0x779 is always 0 when eventStart() is called (see modeWait())
    if (!field_0x779) {
        init_modeDropInit();
    }

    return true;
}

void daSyRock_c::init_modeDropInit() {
    mDoAud_seStart(Z2SE_OBJ_STALAC_BREAK, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    mUnbrokenCollider.OffCoSetBit();
    mUnbrokenCollider.OffTgSetBit();

    mWaitFrames = 0;

    mpWaterPillar = static_cast<daWtPillar_c*>(fopAcM_Search(searchWaterPillar, this));

    mMode = MODE_DROP_INIT;
}

void daSyRock_c::modeDropInit() {
    if (mWaitFrames) {
        mWaitFrames--;
    } else {
        init_modeDrop();
    }
}

void* daSyRock_c::searchWaterPillar(void* i_proc, void* i_this) {
    daSyRock_c* const syRock = static_cast<daSyRock_c*>(i_this);
    daWtPillar_c* const wtPillar = static_cast<daWtPillar_c*>(i_proc);

    if (wtPillar && fopAcM_IsActor(wtPillar) && fopAcM_GetProfName(wtPillar) == PROC_Obj_WaterPillar) {
        const cXyz vectorFromWaterPillar = syRock->current.pos - wtPillar->current.pos;
        const f32 horizontalDistanceToWaterPillar = vectorFromWaterPillar.absXZ();

        if (horizontalDistanceToWaterPillar <= l_HIO.mRange) {
            return wtPillar;
        } else {
            return NULL;
        }
    }
    return NULL;
}

void daSyRock_c::init_modeDrop() {
    fopAcM_SetGravity(this, -l_HIO.mFallAcceleration);
    fopAcM_SetMaxFallSpeed(this, -l_HIO.mMaxFallSpeed);
    fopAcM_SetSpeedF(this, 0.0f);

    if (mpBrokenCollision) {
        dComIfG_Bgsp().Release(mpBrokenCollision);
        mpBrokenCollision->Move();
    }

    mIsUnbroken = false;
    fopAcM_SetMtx(this, mpModels[mIsUnbroken]->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModels[mIsUnbroken]->getModelData());

    const cXyz particlePos = current.pos;
    dComIfGp_particle_set(0x8616, &particlePos, NULL, NULL);
    mUnbrokenCollider.OnAtSetBit();
    mMode = MODE_DROP;
}

void daSyRock_c::modeDrop() {
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bgCheck();
}

void daSyRock_c::init_modeSink() {
    fopAcM_SetGravity(this, -l_HIO.mFallAcceleration + l_HIO.mFallWaterBouyancy);
    speed.y = cLib_minMaxLimit(speed.y, -15.0f - oREG_F(7), 13.0f + oREG_F(8));

    if (!mpWaterPillar) {
        mUnderwaterOffset.x = 150.0f;
    }

    mMode = MODE_SINK;
}

void daSyRock_c::modeSink() {
    if (!mpWaterPillar) {
        cLib_addCalcAngleS(&shape_angle.z, 0x4000, 1, 0x444, 1);
    }

    speed.y = cLib_minMaxLimit(speed.y, -15.0f - oREG_F(7), 13.0f + oREG_F(8));

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());

    bgCheck();
}

void daSyRock_c::init_modeMove() {
    mDoAud_seStart(Z2SE_OBJ_STALAC_LAND_WATER, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (mpBgW) {
        dComIfG_Bgsp().Regist(mpBgW, this);
    }

    mpWaterPillar->onRockFlag();

    mShakeAmplitude = l_HIO.mShakeAmplitude;
    mShakeZOscAngQuantum = l_HIO.mShakeZOscAngQuantum;
    mShakeXOscAngQuantum = l_HIO.mShakeXOscAngQuantum;
    mShakeDampingScale = l_HIO.mShakeDampingScale;
    mShakeMaxDecay = l_HIO.mShakeMaxDecay;
    mShakeMinDecay = l_HIO.mShakeMinDecay;

    mUnbrokenCollider.OffAtSetBit();

    fopAcM_onSwitch(this, getSwBit1());

    mMode = MODE_MOVE;
}

void daSyRock_c::modeMove() {
    mpWaterPillar->onRockFlag();

    current.pos = mpWaterPillar->getPos();
    current.pos.y -= 50.0f;

    if (mpWaterPillar->isRockYure()) {
        mShakeAmplitude = l_HIO.mShakeAmplitude;
        mShakeZOscAngQuantum = l_HIO.mShakeZOscAngQuantum;
        mShakeXOscAngQuantum = l_HIO.mShakeXOscAngQuantum;
        mShakeDampingScale = l_HIO.mShakeDampingScale;
        mShakeMaxDecay = l_HIO.mShakeMaxDecay;
        mShakeMinDecay = l_HIO.mShakeMinDecay;

        mpWaterPillar->clearRockYure();
    }
}

BOOL daSyRock_c::chkWaterLineIn() {
    const f32 waterHeight = mAcch.m_wtr.GetHeight();
    return waterHeight > current.pos.y + 150.0f;
}

void daSyRock_c::bgCheck() {
    const bool hitGround = mAcch.ChkGroundHit();
    const bool hitWater = mAcch.ChkWaterHit();
    const bool inWater = mAcch.ChkWaterIn();
    bool inWaterPillar = false;

    if (mMode == MODE_DROP) {
        if (mpWaterPillar) {
            const f32 pillarTop = mpWaterPillar->getPillarHeight() + mpWaterPillar->current.pos.y - 50.0f;
            if (pillarTop >= current.pos.y) {
                inWaterPillar = true;
            }
        }

        const f32 waterHeight = mAcch.m_wtr.GetHeight();
        if (hitWater) {
            const f32 verticalSpeed = speed.y;

            if (chkWaterLineIn()) {
                init_modeSink();
            }

            if (inWater && !mInWaterPrevFrame) {
                cXyz currentRockPos(current.pos);
                currentRockPos.y = waterHeight;

                fopKyM_createWpillar(&currentRockPos, l_HIO.mEffScale, 3);

                if (verticalSpeed < -15.0f) {
                    fopAcM_seStart(this, Z2SE_OBJ_FALL_WATER_M, 0);
                } else {
                    mDoAud_seStart(Z2SE_OBJ_STALAC_LAND_WATER, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
                }
            }
        }

        if (inWaterPillar) {
            init_modeMove();
        }
    }

    if (hitGround) {
        if (!hitWater && !mpWaterPillar) {
            mDoAud_seStart(Z2SE_OBJ_STALAC_LAND, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            daObjEff::Act_c::make_land_smoke(&current.pos, l_HIO.mEffScale);
        }

        if (mpWaterPillar) {
            init_modeMove();
        } else {
            init_modeDropEnd();
        }
    }
}

void daSyRock_c::init_modeDropEnd() {
    if (!mAcch.ChkWaterIn()) {
        dComIfGp_getVibration().StartShock(l_HIO.mVibModePower, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (mpBgW) {
        dComIfG_Bgsp().Regist(mpBgW, this);
    }

    mUnbrokenCollider.OffAtSetBit();

    fopAcM_onSwitch(this, getSwBit1());

    mMode = MODE_DROP_END;
}

void daSyRock_c::modeDropEnd() {
    /* empty function */
}

int daSyRock_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModels[mIsUnbroken], &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModels[mIsUnbroken]);
    dComIfGd_setList();

    return 1;
}

int daSyRock_c::Delete() {
    dComIfG_resDelete(&mPhase, "syourock");

    if (mpBrokenCollision && mpBrokenCollision->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBrokenCollision);
    }

#if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

static int daSyRock_Draw(daSyRock_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daSyRock_Execute(daSyRock_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daSyRock_Delete(daSyRock_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "daSyRock");
    return i_this->MoveBGDelete();
}

static int daSyRock_Create(fopAc_ac_c* i_this) {
    daSyRock_c* const actor = static_cast<daSyRock_c*>(i_this);
    fopAcM_RegisterCreateID(i_this, "daSyRock");
    return actor->create();
}

static actor_method_class l_daSyRock_Method = {
    (process_method_func)daSyRock_Create,
    (process_method_func)daSyRock_Delete,
    (process_method_func)daSyRock_Execute,
    0,
    (process_method_func)daSyRock_Draw,
};

actor_process_profile_definition g_profile_Obj_SyRock = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_SyRock,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daSyRock_c),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  515,                    // mPriority
  &l_daSyRock_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
