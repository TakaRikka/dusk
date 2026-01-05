/**
 * @file d_a_obj_syRock.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_syRock.h"
#include "d/actor/d_a_obj_eff.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_kankyo_mng.h"
#include "SSystem/SComponent/c_math.h"

class daSyRock_HIO_c : public mDoHIO_entry_c {
public:
    daSyRock_HIO_c();
    ~daSyRock_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ f32 mShakeAmplitude;         // "揺れ強さ" "Shake strength" | Slider
    /* 0x08 */ f32 mShakeXOscillationAngle; // "振幅Ｘ" "Amplitude X" | Slider
    /* 0x0C */ f32 mShakeZOscillationAngle; // "振幅Ｚ" "Amplitude Z" | Slider
    /* 0x10 */ f32 mShakeDamping;           // "揺れ減衰" "Shake damping" | Slider
    /* 0x14 */ f32 mShakeMaxDecay;          // "最大減衰量" "Maximum decay" | Slider
    /* 0x18 */ f32 mShakeMinDecay;          // "最小減衰量" "Minimum decay" | Slider
    /* 0x1C */ f32 mFallAcceleration;       // "落下速度" "Fall speed" | Slider
    /* 0x20 */ f32 mMaxFallSpeed;           // "最大落下速度" "Maximum falling speed" | Slider
    /* 0x24 */ u8 mWaitFrames;              // "wait time" | Slider (unused)
    /* 0x25 */ u8 mShockStrength;           // "振動" "Vibration" | Combo Box
    /* "強さ１" "Strength 1"
       "強さ２" "Strength 2"
       "強さ３" "Strength 3"
       "強さ４" "Strength 4"
       "強さ５" "Strength 5"
       "強さ６" "Strength 6"
       "強さ７" "Strength 7"
       "強さ８" "Strength 8" */
    /* 0x28 */ f32 mEffectScale;            // "eff scale" | Slider
    /* 0x2C */ f32 mFallWaterBouyancy;      // "sink speed" | Slider
    /* 0x30 */ f32 mMaxWaterPillarRange;    // "renge" | "Range" | Slider
};

typedef void (daSyRock_c::*actionFunc)();

static daSyRock_HIO_c l_HIO;

dCcD_SrcCyl daSyRock_c::mCcDCyl = {mCcDObjInfo};

daSyRock_HIO_c::daSyRock_HIO_c() {
    mShakeAmplitude = 450.0f;
    mShakeXOscillationAngle = 90.0f;
    mShakeZOscillationAngle = 45.0f;
    mShakeDamping = 1.0f / 20.0f;
    mShakeMaxDecay = 5.0f;
    mShakeMinDecay = 1.0f / 10.0f;
    mFallAcceleration = 10.0f;
    mMaxFallSpeed = 200.0f;
    mWaitFrames = 30;
    mEffectScale = 4.0f;
    mShockStrength = 4;
    mFallWaterBouyancy = 3.0f;
    mMaxWaterPillarRange = 5.0f;
}

void daSyRock_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(0, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::ZXYrotM(mShakeXZAngleOffset.x, mShakeXZAngleOffset.y, mShakeXZAngleOffset.z);
    mDoMtx_stack_c::transM(mUnderwaterRotatedStalactiteOffset.x, mUnderwaterRotatedStalactiteOffset.y, mUnderwaterRotatedStalactiteOffset.z);

    mpModels[mIsUnbroken]->setBaseScale(scale);

    // Set base transform matrix of current model and the background matrix both to the contents of now
    mpModels[mIsUnbroken]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daSyRock_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("syourock", 4));
    JUT_ASSERT(271, modelData != NULL);
    mpModels[0] = mDoExt_J3DModel__create(modelData, 1 << 19, 0x11000084);
    
    if(!mpModels[0])
        return 0;

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("syourock", 5));
    JUT_ASSERT(282, modelData != NULL);
    mpModels[1] = mDoExt_J3DModel__create(modelData, 1 << 19, 0x11000084);

    if(!mpModels[1])
        return 0;
    
    mpBrokenCollision = new dBgW;

    if(!mpBrokenCollision || mpBrokenCollision->Set(static_cast<cBgD_t*>(dComIfG_getObjectRes("syourock", 9)), 1, &mBgMtx)) {
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

cPhs__Step daSyRock_c::create() {
    fopAcM_ct(this, daSyRock_c);

    if(getArg0() == TRUE) {
        mpWaterPillar = static_cast<daWtPillar_c*>(fopAcIt_Judge(searchWaterPillar, this));

        if(!mpWaterPillar)
            return cPhs_INIT_e;
    }

    const cPhs__Step requestedPhaseProcess = static_cast<cPhs__Step>(dComIfG_resLoad(&mPhase, "syourock"));
    if(requestedPhaseProcess == cPhs_COMPLEATE_e) {
        const cPhs__Step bgCreatePhaseProcess = static_cast<cPhs__Step>(MoveBGCreate("syourock", 0x8, dBgS_MoveBGProc_TypicalRotY, 0x2100, NULL));
        if(bgCreatePhaseProcess == cPhs_ERROR_e)
            return cPhs_ERROR_e;

        mAcchCir.SetWall(150.0f, 150.0f);
        mAcch.Set(&current.pos, &old.pos, this, 0x1, &mAcchCir, &speed, NULL, NULL);
        mAcch.SetWaterCheckOffset(10000.0f);
        mShakeOscillationAngleStep = 0;
        mShakeXZAngleOffset.setall(0);

        mShakeXOscillationAngle = mShakeZOscillationAngle = mShakeAmplitude = 0.0f;
        mShakeMinDecay = mShakeMaxDecay = mShakeDamping =  0.0f;
        mUnderwaterRotatedStalactiteOffset.setall(0.0f);

        mStts.Init(0xFF, 0xFF, this);
        mUnbrokenCylinderCollider.Set(mCcDCyl);
        mUnbrokenCylinderCollider.SetStts(&mStts);
        mUnbrokenCylinderCollider.OffAtSetBit();

        const u32 wasBrokenSwitchNo = getSwBit1();
        if(dComIfGs_isSwitch(wasBrokenSwitchNo, fopAcM_GetHomeRoomNo(this))) {
            mIsUnbroken = false;
            mUnbrokenCylinderCollider.OffCoSetBit();
            if(mpBrokenCollision && mpBrokenCollision->ChkUsed()) {
                dComIfG_Bgsp().Release(mpBrokenCollision);
                mpBrokenCollision->Move();
            }

            // Snap already broken stalactite to its appropriate location and set its mode accordingly
            setFallStat();
        }
        else {
            mIsUnbroken = true;

            if(mpBgW)
                dComIfG_Bgsp().Release(mpBgW);
                
            if(mpBrokenCollision) {
                dComIfG_Bgsp().Regist(mpBrokenCollision, this);
                mpBrokenCollision->Move();
            }

            mDropAutomaticallySwitchNo = getSwBit2();
            init_modeWait();

        }

        field_0x779 = 2;
        fopAcM_SetMtx(this, mpModels[mIsUnbroken]->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModels[mIsUnbroken]->getModelData());
        setBaseMtx();
    }

    return requestedPhaseProcess;
}

void daSyRock_c::setFallStat() {
    mpWaterPillar = static_cast<daWtPillar_c*>(fopAcIt_Judge(searchWaterPillar, this));
    if(mpWaterPillar) {
        mpWaterPillar->onRockFlag();
        mMode = MODE_MOVE;
    }
    else {
        mAcch.CrrPos(dComIfG_Bgsp());

        if(mAcch.ChkWaterHit()) {
            if(mAcch.m_wtr.GetHeight() - mAcch.GetGroundH() >= 200.0f) {
                mUnderwaterRotatedStalactiteOffset.x = 150.0f;
                shape_angle.z = 0x4000;
            }
            current.pos.y = mAcch.GetGroundH();
        }
        else {
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

    mInWaterPreviousFrame = mAcch.ChkWaterIn();
    setBaseMtx();

    return 1;
}

void daSyRock_c::move() {
    static actionFunc mode_proc[] = {
    &daSyRock_c::modeWait, &daSyRock_c::modeDropInit,
    &daSyRock_c::modeDrop, &daSyRock_c::modeSink,
    &daSyRock_c::modeMove, &daSyRock_c::modeDropEnd
    };

    (this->*mode_proc[mMode])(); 

    mShakeXZAngleOffset.x = mShakeAmplitude * cM_ssin(mShakeOscillationAngleStep * cM_deg2s(mShakeXOscillationAngle));
    mShakeXZAngleOffset.z = mShakeAmplitude * cM_scos(mShakeOscillationAngleStep * cM_deg2s(mShakeZOscillationAngle));

    cLib_addCalc(&mShakeAmplitude, 0.0f, mShakeDamping, mShakeMaxDecay, mShakeMinDecay);

    mShakeOscillationAngleStep++;

    mUnbrokenCylinderCollider.SetR(150.0f);
    mUnbrokenCylinderCollider.SetH(680.0f);

    cXyz currentPos = current.pos;
    currentPos.y -= 250.0f;

    mUnbrokenCylinderCollider.SetC(currentPos);

    dComIfG_Ccsp()->Set(&mUnbrokenCylinderCollider);
}

void daSyRock_c::init_modeWait() {
    mMode = MODE_WAIT;
}

void daSyRock_c::modeWait() {
    if(mDropAutomaticallySwitchNo != 0xFF && dComIfGs_isSwitch(mDropAutomaticallySwitchNo, fopAcM_GetHomeRoomNo(this))) {
        field_0x779 = 0;
        if(getEvetID() != 0xFF){
            orderEvent(getEvetID(), 0xFF, 1);
        } else {
            eventStart();
        }
    }
    else if(mUnbrokenCylinderCollider.ChkTgHit()) {
        if(mUnbrokenCylinderCollider.GetTgHitGObj()->GetAtType() == AT_TYPE_BOMB) {
            field_0x779 = 0;
            if(getEvetID() != 0xFF)
                orderEvent(getEvetID(), 0xFF, 1);
            else {
                eventStart();
            }
        }
        mUnbrokenCylinderCollider.ClrTgHit();
    }
}

bool daSyRock_c::eventStart() {
    // field_0x779 is always 0 when eventStart() is called (see modeWait())
    if(!field_0x779)
        init_modeDropInit();

    return true;
}

void daSyRock_c::init_modeDropInit() {
    fopAcM_seStartCurrent(this, Z2SE_OBJ_STALAC_BREAK, 0);

    mUnbrokenCylinderCollider.OffCoSetBit();
    mUnbrokenCylinderCollider.OffTgSetBit();

    mWaitFrames = 0;

    mpWaterPillar = static_cast<daWtPillar_c*>(fopAcIt_Judge(searchWaterPillar, this));

    mMode = MODE_DROP_INIT;
}

void daSyRock_c::modeDropInit() {
    if(mWaitFrames)
        mWaitFrames--;
    else
        init_modeDrop();
}

void* daSyRock_c::searchWaterPillar(void* i_proc, void* i_this) {
    daSyRock_c* const syRock = static_cast<daSyRock_c*>(i_this);
    daWtPillar_c* const wtPillar = static_cast<daWtPillar_c*>(i_proc);

    if(wtPillar && fopAcM_IsActor(wtPillar) && fopAcM_GetProfName(wtPillar) == PROC_Obj_WaterPillar) {
        const cXyz vectorToWaterPillar = fopAcM_GetPosition(syRock) - fopAcM_GetPosition(wtPillar);
        const f32 horizontalDistanceToWaterPillar = sqrtf(vectorToWaterPillar.getMagXZ());
         
        if(horizontalDistanceToWaterPillar <= l_HIO.mMaxWaterPillarRange)
            return wtPillar;
        else
            return NULL;
    }
    return NULL;
}

void daSyRock_c::init_modeDrop() {
    gravity = -l_HIO.mFallAcceleration;
    maxFallSpeed = -l_HIO.mMaxFallSpeed;
    speedF = 0.0f;

    if(mpBrokenCollision) {
        dComIfG_Bgsp().Release(mpBrokenCollision);
        mpBrokenCollision->Move();
    }

    mIsUnbroken = false;
    fopAcM_SetMtx(this, mpModels[mIsUnbroken]->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModels[mIsUnbroken]->getModelData());

    const cXyz particlePos = current.pos;
    dComIfGp_particle_set(0x8616, &particlePos, NULL, NULL);
    mUnbrokenCylinderCollider.OnAtSetBit();
    mMode = MODE_DROP;
}

void daSyRock_c::modeDrop() {
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    bgCheck();
}

void daSyRock_c::init_modeSink() {
    gravity = -l_HIO.mFallAcceleration + l_HIO.mFallWaterBouyancy;
    speed.y = cLib_minMaxLimit(speed.y, -15.0f, 13.0f);

    if(!mpWaterPillar)
        mUnderwaterRotatedStalactiteOffset.x = 150.0f;

    mMode = MODE_SINK;
}

void daSyRock_c::modeSink() {
    if(!mpWaterPillar)
        cLib_addCalcAngleS(&shape_angle.z, 0x4000, 1, 0x444, 1);

    speed.y = cLib_minMaxLimit(speed.y, -15.0f, 13.0f);

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());

    bgCheck();
}

void daSyRock_c::init_modeMove() {
    fopAcM_seStartCurrent(this, Z2SE_OBJ_STALAC_LAND_WATER, 0);

    if(mpBgW)
        dComIfG_Bgsp().Regist(mpBgW, this);

    mpWaterPillar->onRockFlag();

    mShakeAmplitude = l_HIO.mShakeAmplitude;
    mShakeZOscillationAngle = l_HIO.mShakeZOscillationAngle;
    mShakeXOscillationAngle = l_HIO.mShakeXOscillationAngle;
    mShakeDamping = l_HIO.mShakeDamping;
    mShakeMaxDecay = l_HIO.mShakeMaxDecay;
    mShakeMinDecay = l_HIO.mShakeMinDecay;

    mUnbrokenCylinderCollider.OffAtSetBit();

    const u32 swBit = getSwBit1();
    dComIfGs_onSwitch(swBit, fopAcM_GetHomeRoomNo(this));

    mMode = MODE_MOVE;
}

void daSyRock_c::modeMove() {
    mpWaterPillar->onRockFlag();

    current.pos = mpWaterPillar->getPos();
    current.pos.y -= 50.0f;
    
    if(mpWaterPillar->isRockYure()) {
        mShakeAmplitude = l_HIO.mShakeAmplitude;
        mShakeZOscillationAngle = l_HIO.mShakeZOscillationAngle;
        mShakeXOscillationAngle = l_HIO.mShakeXOscillationAngle;
        mShakeDamping = l_HIO.mShakeDamping;
        mShakeMaxDecay = l_HIO.mShakeMaxDecay;
        mShakeMinDecay = l_HIO.mShakeMinDecay;

        mpWaterPillar->clearRockYure();
    }
}

BOOL daSyRock_c::chkWaterLineIn() {
    return mAcch.m_wtr.GetHeight() > current.pos.y + 150.0f;
}

void daSyRock_c::bgCheck() {
    // Note stack ordering issues arise if fopAcM_seStartCurrent() is used instead of mDoAud_seStart()
    const u32 hitGround = mAcch.ChkGroundHit() ? TRUE : FALSE;
    const u32 hitWater = mAcch.ChkWaterHit() ? TRUE : FALSE;
    const u32 inWater = mAcch.ChkWaterIn() ? TRUE : FALSE;
    bool inWaterPillar = false;

    if(mMode == MODE_DROP) {
        if(mpWaterPillar) {
            const f32 waterPillarHeight = mpWaterPillar->getPillarHeight();
            const f32 waterPillarVerticalPos = fopAcM_GetPosition(mpWaterPillar).y;
            if(waterPillarVerticalPos + waterPillarHeight - 50.0f >= current.pos.y)
                inWaterPillar = true;
        }

        const f32 waterHeight = mAcch.m_wtr.GetHeight();
        if(hitWater) {
            const f32 verticalSpeed = speed.y;
            
            if(chkWaterLineIn())
                init_modeSink();

            if(inWater && !mInWaterPreviousFrame) {
                cXyz currentRockPos = current.pos;
                currentRockPos.y = waterHeight;
                fopKyM_createWpillar(&currentRockPos, l_HIO.mEffectScale, 3);

                if(verticalSpeed < -15.0f)
                    fopAcM_seStart(this, Z2SE_OBJ_FALL_WATER_M, 0);
                else
                    mDoAud_seStart(Z2SE_OBJ_STALAC_LAND_WATER, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            }
        }

        if(inWaterPillar)
            init_modeMove();
    }

    if(hitGround) {
        if(!hitWater && !mpWaterPillar) {
            mDoAud_seStart(Z2SE_OBJ_STALAC_LAND, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            daObjEff::Act_c::make_land_smoke(&current.pos, l_HIO.mEffectScale);
        }

        if(mpWaterPillar)
            init_modeMove();
        else
            init_modeDropEnd();
    }
}

void daSyRock_c::init_modeDropEnd() {
    if(!mAcch.ChkWaterIn()) {
        dComIfGp_getVibration().StartShock(l_HIO.mShockStrength, 0xF, cXyz(0.0f, 1.0f, 0.0f));
    }

    if(mpBgW)
        dComIfG_Bgsp().Regist(mpBgW, this);

    mUnbrokenCylinderCollider.OffAtSetBit();

    const u32 switchBitIndex = getSwBit1(); 
    dComIfGs_onSwitch(switchBitIndex, fopAcM_GetHomeRoomNo(this));

    mMode = MODE_DROP_END;
}

void daSyRock_c::modeDropEnd() {
    /* empty function */
}

int daSyRock_c::Draw() {
    g_env_light.settingTevStruct(0x10, fopAcM_GetPosition_p(this), &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModels[mIsUnbroken], &tevStr);
    
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModels[mIsUnbroken]);
    dComIfGd_setList();

    return 1;
}

int daSyRock_c::Delete() {
    dComIfG_resDelete(&mPhase, "syourock");

    if(mpBrokenCollision && mpBrokenCollision->ChkUsed())
        dComIfG_Bgsp().Release(mpBrokenCollision);

    return 1;
}

static int daSyRock_Draw(daSyRock_c* i_this) {
    return i_this->Draw();
    return 1;
}

static int daSyRock_Execute(daSyRock_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daSyRock_Delete(daSyRock_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daSyRock_Create(fopAc_ac_c* i_this) {
    return static_cast<daSyRock_c*>(i_this)->create();
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
