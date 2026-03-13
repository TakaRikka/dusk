/**
 * @file d_a_obj_waterPillar.cpp
 *
*/


#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_waterPillar.h"
#if DEBUG
#include "d/d_debug_viewer.h"
#endif

struct daWtPillar_HIO_c : public mDoHIO_entry_c {
    daWtPillar_HIO_c();
    ~daWtPillar_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ cXyz mDbgDrawScale;
    /* 0x10 */ csXyz mDbgDrawAngle;
    /* 0x16 */ bool mTesting;
    /* 0x17 */ u8 mDbgDrawingOn;
    /* 0x18 */ u8 mStopTime;
    /* 0x19 */ u8 mUpFirstWaitFrames;
    /* 0x1A */ u8 pad_0[6];
    /* 0x20 */ f32 mUpFirstTargetSpeed;             // Maximum speed for preliminary upward movement
    /* 0x24 */ u8 pad_1[4];
    /* 0x28 */ f32 mColScaleFactor;
    /* 0x2C */ u8 mUpWaitFrames;
    /* 0x2D */ u8 pad_2[4];
    /* 0x34 */ f32 mUpTargetSpeed;               // Maximum speed for final upward movement
    /* 0x38 */ u8 pad_3[8];
    /* 0x40 */ f32 mDownTargetSpeed;                // In units per second
    /* 0x44 */ f32 mVOscAngleQuantum;
    /* 0x48 */ f32 mVOscAmplitude;
    /* 0x4C */ f32 mVOscDampingScale;
    /* 0x50 */ f32 mVOscMaxDecay;
    /* 0x54 */ f32 mVOscMinDecay;
};

typedef void (daWtPillar_c::*actionFunc)();

int daWtPillar_Draw(daWtPillar_c* i_this);
int daWtPillar_Execute(daWtPillar_c* i_this);
int daWtPillar_Delete(daWtPillar_c* i_this);
int daWtPillar_Create(fopAc_ac_c* i_this);

daWtPillar_HIO_c::daWtPillar_HIO_c() {
    mDbgDrawScale.set(1.0f, 1.0f, 1.0f);
    mDbgDrawAngle.set(0, 0, 0);
    mTesting = false;
    mDbgDrawingOn = false;
    mStopTime = 30;
    mUpFirstTargetSpeed = 2000.0f;
    mUpFirstWaitFrames = 30;
    mUpTargetSpeed = 5000.0f;
    mUpWaitFrames = 60;
    mDownTargetSpeed = 1000.0f;
    mVOscAngleQuantum = 20.0f;
    mVOscAmplitude = 25.0f;
    mVOscDampingScale = 1.0f / 100.0f;
    mVOscMaxDecay = 1.0f / 20.0f;
    mVOscMinDecay = 1.0f / 100.0f;
}

#if DEBUG
void daWtPillar_HIO_c::genMessage(JORMContext* mctx) {
    /* === During Preparatory Movements === */
    mctx->genLabel("\n=== 予備動作時 ===", 0);
    // Waiting time
    mctx->genSlider("待ち時間", &mUpFirstWaitFrames, 0, 0xFF);

    /* === During Upward Movements === */
    mctx->genLabel("\n=== 上昇動作時 ===", 0);
    // Waiting time
    mctx->genSlider("待ち時間", &mUpWaitFrames, 0, 0xFF);

    /* ----------- For Testing ---------- */
    mctx->genCheckBox("----------- テスト用 ----------", (u8*) &mTesting, 0x1);

    /* === When Stopped === */
    mctx->genLabel("\n=== 停止時 ===", 0);
    // Stop time
    mctx->genSlider("停止時間", &mStopTime, 0, 0xFF);

    /* === During Preparatory Movements === */
    mctx->genLabel("\n=== 予備動作時 ===", 0);
    // Velocity
    mctx->genSlider("速度", &mUpFirstTargetSpeed, 0.0f, 30000.0f);

    /* === During Upward Movements === */
    mctx->genLabel("\n=== 上昇動作時 ===", 0);
    // Velocity
    mctx->genSlider("速度", &mUpTargetSpeed, 0.0f, 30000.0f);

    /* === During Descending Movements === */
    mctx->genLabel("\n=== 下降動作時 ===", 0);
    // Velocity
    mctx->genSlider("速度", &mDownTargetSpeed, 0.0f, 30000.0f);

    /* ====Shaking==== */
    mctx->genLabel("\n====揺れ====", 0);
    // Y amplitude
    mctx->genSlider("振幅Ｙ", &mVOscAngleQuantum, 0.0f, 180.0f);
    // Shake strength
    mctx->genSlider("移動強さ", &mVOscAmplitude, 0.0f, 10000.0f);
    // Shake damping
    mctx->genSlider("揺れ減衰", &mVOscDampingScale, 0.0f, 1.0f);
    // Maximum damping amount
    mctx->genSlider("最大減衰量", &mVOscMaxDecay, 0.0f, 100.0f);
    // Minimum damping amount
    mctx->genSlider("最小減衰量", &mVOscMinDecay, 0.0f, 100.0f);

    /* Model Drawing OFF */  // (Actually turns on debug cylinder drawing)
    mctx->genCheckBox("モデル描画ＯＦＦ", &mDbgDrawingOn, 0x1);
}
#endif

void daWtPillar_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + mCurrentHeight, current.pos.z);
    mDoMtx_stack_c::transM(mVOscVOffset.x, mVOscVOffset.y, mVOscVOffset.z);

    mpModel->setBaseScale(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daWtPillar_c::createHeapCallBack(fopAc_ac_c* i_this) {
    daWtPillar_c* actor = static_cast<daWtPillar_c*>(i_this);
    return actor->CreateHeap();
}

const dCcD_SrcGObjInf daWtPillar_c::mCcDObjInfo = {
    {0x0, {{AT_TYPE_100, 0x0, 0x1D}, {AT_TYPE_0, 0x0}, 0x0}},    // mObj
    {dCcD_SE_SWORD, 0x0, 0x0, dCcD_MTRL_NONE, 0x0},      // mGObjAt
    {dCcD_SE_STONE, 0x0, 0x0, dCcD_MTRL_NONE, 0x0},      // mGObjTg
    {},  // mGObjCo
};
const dCcD_SrcGObjInf daWtPillar_c::mCcDObjCoInfo = {
    {0x0, {{AT_TYPE_0, 0x0, 0x0}, {0x00, 0x0}, 0x79}},    // mObj
    {dCcD_SE_SWORD, 0x0, 0x0, dCcD_MTRL_NONE, 0x0},      // mGObjAt
    {dCcD_SE_STONE, 0x0, 0x0, dCcD_MTRL_NONE, 0x0},      // mGObjTg
    {},  // mGObjCo
};

static daWtPillar_HIO_c l_HIO;

cull_box l_cull_box = {
    {-30.0f, -10.0f, -30.0f},
    {30.0f, 60.0f, 30.0f}
};

dCcD_SrcCps daWtPillar_c::mCcDCps = {
    daWtPillar_c::mCcDObjInfo,
    { { {0.0f, 0.0f, 0.0f}, {0.0f,1100.0f, 0.0f}, 150.0f }}
};

dCcD_SrcCyl daWtPillar_c::mCcDCyl = {daWtPillar_c::mCcDObjCoInfo};

int daWtPillar_c::CreateHeap() {
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("efWater", 8));
    JUT_ASSERT(369, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000284);

    if(!mpModel)
        return 0;

    int res = mModelRotAnm.init(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("efWater", 5)),
                                           1, 2, 1.0f, 0, -1, false);
    JUT_ASSERT(385, res == 1)

    res = mVTexScrollAnm.init(modelData,
                                               static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("efWater", 11)),
                                               1, 2, 1.0f, 0, -1);
    JUT_ASSERT(394, res == 1)

    return 1;
}

cPhs_Step daWtPillar_c::create() {
    fopAcM_ct(this, daWtPillar_c);
    const cPhs_Step phase = static_cast<cPhs_Step>(dComIfG_resLoad(&mPhase, "efWater"));

    if(phase == cPhs_COMPLEATE_e) {
        if(!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2000))
            return cPhs_ERROR_e;

        mStts.Init(0xFE, 0xFF, this);
        mCylinderCollider.Set(mCcDCyl);
        mCylinderCollider.SetStts(&mStts);
        mCapsuleCollider.Set(mCcDCps);
        mCapsuleCollider.SetStts(&mStts);

        //! @bug l_cull_box.min is used to set actor cull.box.max, and vice-versa
        //! @note Scaling is symmetric with respect to X and Z
        cull_box cullBounds;
        cullBounds.max.x = l_cull_box.min.x * scale.x;
        cullBounds.max.y = l_cull_box.min.y * scale.y;
        cullBounds.max.z = l_cull_box.min.z * scale.x;
        cullBounds.min.x = l_cull_box.max.x * scale.x;
        cullBounds.min.y = l_cull_box.max.y * scale.y;
        cullBounds.min.z = l_cull_box.max.z * scale.x;
        //! @bug cullBounds.min.y is passed as max Z for setting cull size box, making it larger than intended
        fopAcM_setCullSizeBox(this, cullBounds.max.x, cullBounds.max.y, cullBounds.max.z, cullBounds.min.x, cullBounds.min.y, cullBounds.min.y);

        mAcchCir.SetWall(150.0f, 5.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir, fopAcM_GetSpeed_p(this), 0, 0);
        mAcch.ClrWaterNone();
        mAcch.SetWaterCheckOffset(10000.0f);
        mAcch.OnLineCheckNone();

        mCarryingStalactite = false;
        mStartStalactiteShake = 0;

        mSwitchNo = getParam(0, 8);
        mType = getParam(8, 4);

        mMaxHeight = getParam(27, 5) * 100.0f;

        // "== Maximum height %f ==\n"
        OS_REPORT("== 最大の高さ %f ==\n", mMaxHeight);

        mAcch.CrrPos(dComIfG_Bgsp());

        /**
         * @bug Adjustments to the water pillar's target height are only made when the object
         * is created, leading to inconsistent behavior if the water height is changed
         * after the object is created.
         *
         * See the east room in B1 of the Lakebed Temple as an example:
         * When first entering entering the room after redirecting water towards it, a
         * cutscene plays showing the water level rising;
         * the bottom of the oscillating water pillar in this room starts above water,
         * but ends up below water after the cutscene, resulting in its target height never being modified.
         * However, exiting and re-entering the room does not result in the cutscene playing again,
         * so its target height is modified upon subsequent entries to the room
         */
        if(mAcch.ChkWaterHit() && mType == RISES_AND_FALLS) {
            mRelativeWaterHeight = mAcch.m_wtr.GetHeight() - current.pos.y;
            mMaxHeight += mRelativeWaterHeight;

            // "== Underwater (%f) (%f) type: %d ==\n"
            OS_REPORT("== 水中にある (%f) (%f) type:%d ==\n", mMaxHeight, mRelativeWaterHeight, mType);
        }

        field_0xB44 = 0;
        mIsUpOrStatic = 0;
        mIsUpFirst = 0;
        mVOscAngleStep = 0;
        mVOscVOffset.x = 0.0f;
        mVOscVOffset.y = 0.0f;
        mVOscVOffset.z = 0.0f;
        mVOscAmplitude = 0.0f;
        mVOscAngleQuantum = 0.0f;
        mVOscDampingScale = 0.0f;
        mVOscMaxDecay = 0.0f;
        mVOscMinDecay = 0.0f;
        mCurrentHeight = 0.0f;

        if(mSwitchNo != 0xFF) {
            actionSwWaitInit();
        }
        else if(mType == STATIC) {
            mCurrentHeight = mMaxHeight;
            field_0xB44 = true;
            mIsUpOrStatic = true;
            actionRockWaitInit();
        }
        else {
            actionUpFirstInit();
        }

        setBaseMtx();

        #if DEBUG
        // "Water column"
        l_HIO.entryHIO("水柱");
        #endif
    }

    return phase;
}

int daWtPillar_c::execute() {
    mModelRotAnm.play();
    mVTexScrollAnm.play();

    eventUpdate();
    actionMain();

    mAcch.CrrPos(dComIfG_Bgsp());
    effectSet();
    effectSet2();

    mDbgDrawScale.x = scale.x;
    mDbgDrawScale.z = scale.z;

    cXyz currentHeightVector(0.0f, 1.0f, 0.0f);
    currentHeightVector.y *= mCurrentHeight;


    mDoMtx_stack_c::ZXYrotS(current.angle.x, current.angle.y, current.angle.z);
    mDoMtx_stack_c::multVec(&currentHeightVector, &currentHeightVector);

    cXyz currentPos = current.pos;
    currentPos.y -= 150.0f;

    mCapsuleSource.mStart = currentPos;


    mCapsuleSource.mEnd = currentHeightVector + currentPos;
    mCapsuleSource.mRadius = scale.x * 140.0f;

    //! @bug l_HIO.mColliderUpdateScaleFactor is read, but never written to after default initialization. This therefore evaluates to mCurrentHeight / 100.0f > 0.0f
    if(mCurrentHeight / 100.0f > scale.y * l_HIO.mColScaleFactor) {
        mCapsuleCollider.cM3dGCps::Set(mCapsuleSource);
        dComIfG_Ccsp()->Set(&mCapsuleCollider);
    }

    mCylinderCollider.SetR(scale.x * 140.0f);
    mCylinderCollider.SetH(mCurrentHeight);
    mCylinderCollider.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCylinderCollider);

    mVOscVOffset.y = mVOscAmplitude * cM_ssin(mVOscAngleStep * cM_deg2s(mVOscAngleQuantum));

    cLib_addCalc(&mVOscAmplitude, 0.0f, mVOscDampingScale, mVOscMaxDecay, mVOscMinDecay);

    mVOscAngleStep++;

    setBaseMtx();

    return 1;
}

void daWtPillar_c::actionMain() {
    static actionFunc l_func[] = {
    &daWtPillar_c::actionSwWait, &daWtPillar_c::actionWait,
    &daWtPillar_c::actionUpFirst, &daWtPillar_c::actionUpFirstWait,
    &daWtPillar_c::actionUp, &daWtPillar_c::actionUpWait,
    &daWtPillar_c::actionDown, &daWtPillar_c::actionRockWait,
    &daWtPillar_c::actionRockOn, &daWtPillar_c::actionEnd
    };

    (this->*l_func[mAction])();
}

void daWtPillar_c::effectSet() {
    static u16 l_eff[7] = {
    0x86E3, // Smoke-like water foam particles, placed at pillar top
    0x86E4, // Large paint splatter-type water foam particles, placed at pillar top
    0x86E7, // Small paint splash-type water foam particles, placed at pillar top
    0x86E6, // Small paint splatter-type water foam particles, placed at pillar top
    0x86E5, // Large paint splash-type water foam particles, placed at pillar bottom
    0x86EB, // Water splash particles, placed at pillar bottom
    0x86EA  // Water splash particles, placed at pillar top
    };

    // Create foam particles at the bottom and top of the pillar
    cXyz effectOscillationOffset(0.0f, 0.0f, 0.0f);

    mDoMtx_stack_c::transS(mVOscVOffset.x, mVOscVOffset.y, mVOscVOffset.z);
    mDoMtx_stack_c::multVec(&effectOscillationOffset, &effectOscillationOffset);

    cXyz currentPosWithOscillationOffset = current.pos + effectOscillationOffset;

    mTopPos = currentPosWithOscillationOffset;
    mTopPos.y += mCurrentHeight;

    for(int i = 0; i < 3; i++)
        mBotAndTopEmmitters[i] = dComIfGp_particle_set(mBotAndTopEmmitters[i],l_eff[i], &mTopPos, NULL, NULL);

    if(!mCarryingStalactite)
        mBotAndTopEmmitters[3] = dComIfGp_particle_set(mBotAndTopEmmitters[3],l_eff[3], &mTopPos, NULL, NULL);

    if(mIsUpOrStatic)
        mBotAndTopEmmitters[4] = dComIfGp_particle_set(mBotAndTopEmmitters[4],l_eff[4], &current.pos, NULL, NULL);

    if(mIsUpFirst) {
        mBotAndTopEmmitters[5] = dComIfGp_particle_set(mBotAndTopEmmitters[5],l_eff[5], &current.pos, NULL, NULL);
    }

    if(mCarryingStalactite)
        mBotAndTopEmmitters[6] = dComIfGp_particle_set(mBotAndTopEmmitters[6],l_eff[6], &mTopPos, NULL, NULL);
}

void daWtPillar_c::effectSet2() {

    /* If the bottom of the pillar is at least 500.0 units underwater and the
        current height of the pillar makes the top be above water,
        create foam particles at the horizontal position of the pillar, but at
        the water surface
    */

    // Can be observed in Lakebed Temple, when water level is raised in certain rooms
    if(mAcch.m_wtr.GetHeight() - current.pos.y >= 500.0f &&
    current.pos.y + mCurrentHeight >= mAcch.m_wtr.GetHeight())
    {
        cXyz effectOscillationOffset(0.0f, 0.0f, 0.0f);

        mDoMtx_stack_c::transS(mVOscVOffset.x, mVOscVOffset.y, mVOscVOffset.z);
        mDoMtx_stack_c::multVec(&effectOscillationOffset, &effectOscillationOffset);

        cXyz currentPosWithVerticalOffset = current.pos + effectOscillationOffset;
        currentPosWithVerticalOffset.y = mAcch.m_wtr.GetHeight();

        mWaterSurfaceEmitters[0] = dComIfGp_particle_set(mWaterSurfaceEmitters[0], 0x888F, &currentPosWithVerticalOffset, NULL, NULL);
        mWaterSurfaceEmitters[1] = dComIfGp_particle_set(mWaterSurfaceEmitters[1], 0x8890, &currentPosWithVerticalOffset, NULL, NULL);
    }
}

void daWtPillar_c::actionSwWaitInit() {
    mAction = ACTION_SW_WAIT;
}

void daWtPillar_c::actionSwWait() {
    if(fopAcM_isSwitch(this, mSwitchNo)) {
        if(getEventID() != 0xFF)
            orderEvent(getEventID(), 0xFF, 1);
        else
            eventStart();
    }
}

bool daWtPillar_c::eventStart() {
    actionUpFirstInit();
    return TRUE;
}

void daWtPillar_c::actionWaitInit() {
    mWaitFrames = getParam(12, 5) * 0.5f * 30.0f;

    #if DEBUG
    if(l_HIO.mTesting)
        mWaitFrames = l_HIO.mStopTime;
    #endif

    field_0xB44 = false;
    mIsUpOrStatic = false;

    mDoAud_seStart(Z2SE_OBJ_WTR_CLMN_WAIT, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    mAction = ACTION_WAIT;
}

void daWtPillar_c::actionWait() {
    if(mWaitFrames)
        mWaitFrames--;
    else
        actionUpFirstInit();
}

void daWtPillar_c::actionUpFirstInit() {
    mTargetSpeed = (getParam(17, 5) * 100.0f * 5.0f) / 30.0f;

    #if DEBUG
    if(l_HIO.mTesting)
        mTargetSpeed = l_HIO.mUpFirstTargetSpeed / 30.0f;
    #endif

    if(mCarryingStalactite)
        mUpFirstTargetHeight = 50.0f;
    else
        mUpFirstTargetHeight = 300.0f;

    speedF = 0.0f;

    field_0xB44 = true;
    mIsUpFirst = true;
    mStartStalactiteShake = true;

    mDoAud_seStart(Z2SE_OBJ_WTR_CLMN_READY, &current.pos, 0,
                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    mAction = ACTION_UP_FIRST;
}

void daWtPillar_c::actionUpFirst() {
    const BOOL maxSpeedReached = cLib_chaseF(&speedF, mTargetSpeed, mTargetSpeed / 30.0f);

    const f32 differenceBetweenCurrentAndFirstTargetHeight = cLib_addCalc(&mCurrentHeight, mUpFirstTargetHeight, 1.0f, speedF, 1.0f / 10.0f);

    if(differenceBetweenCurrentAndFirstTargetHeight == 0.0f)
        actionUpFirstWaitInit();
}

void daWtPillar_c::actionUpFirstWaitInit() {
    mWaitFrames = l_HIO.mUpFirstWaitFrames;
    mVOscAmplitude = l_HIO.mVOscAmplitude;
    mVOscAngleQuantum = l_HIO.mVOscAngleQuantum;
    mVOscDampingScale = l_HIO.mVOscDampingScale;
    mVOscMaxDecay = l_HIO.mVOscMaxDecay;
    mVOscMinDecay = l_HIO.mVOscMinDecay;
    mAction = ACTION_UP_FIRST_WAIT;
}

void daWtPillar_c::actionUpFirstWait() {
    if(mWaitFrames)
        mWaitFrames--;
    else
        actionUpInit();
}

void daWtPillar_c::actionUpInit() {
    mTargetSpeed = getParam(22, 5) * 100.0f * 5.0f / 30.0f;

    #if DEBUG
    if(l_HIO.mTesting)
        mTargetSpeed = l_HIO.mUpTargetSpeed / 30.0f;
    #endif

    if(mCarryingStalactite)
        mTargetHeightStalactiteOffset = -250.0f;
    else
        mTargetHeightStalactiteOffset = 0.0f;

    mIsUpFirst = false;

    mIsUpOrStatic = true;
    mStartStalactiteShake = true;

    speedF = 0.0f;

    mAction = ACTION_UP;
}

void daWtPillar_c::actionUp() {
    const BOOL maxSpeedReached = cLib_chaseF(&speedF, mTargetSpeed, mTargetSpeed / 30.0f);

    const f32 differenceBetweenCurrentAndMaxHeightWithStalactiteWeight = cLib_addCalc(&mCurrentHeight, mMaxHeight + mTargetHeightStalactiteOffset, 1.0f, speedF, 1.0f / 10.0f);

    mDoAud_seStartLevel(Z2SE_OBJ_WTR_CLMN_UP, &mTopPos, mMaxHeight + mTargetHeightStalactiteOffset, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if(differenceBetweenCurrentAndMaxHeightWithStalactiteWeight == 0.0f) {
        if(mType == STATIC)
            actionRockWaitInit();
        else
            actionUpWaitInit();
    }

}

void daWtPillar_c::actionUpWaitInit() {
    mWaitFrames = l_HIO.mUpWaitFrames;
    mVOscAmplitude = l_HIO.mVOscAmplitude;
    mVOscAngleQuantum = l_HIO.mVOscAngleQuantum;
    mVOscDampingScale = l_HIO.mVOscDampingScale;
    mVOscMaxDecay = l_HIO.mVOscMaxDecay;
    mVOscMinDecay = l_HIO.mVOscMinDecay;
    mAction = ACTION_UP_WAIT;
}

void daWtPillar_c::actionUpWait() {
    mDoAud_seStartLevel(Z2SE_OBJ_WTR_CLMN_UP, &mTopPos, mMaxHeight + mTargetHeightStalactiteOffset, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if(mWaitFrames)
        mWaitFrames--;
    else
        actionDownInit();
}

void daWtPillar_c::actionDownInit() {
    speedF = 0.0f;
    mStartStalactiteShake = true;
    mAction = ACTION_DOWN;
}

void daWtPillar_c::actionDown() {
    const BOOL maxDownwardSpeedReached = cLib_chaseF(&speedF, l_HIO.mDownTargetSpeed / 30.0f, (l_HIO.mDownTargetSpeed / 30.0f) / 30.0f);

    const f32 diff = cLib_addCalc(&mCurrentHeight, 0.0f, 1.0f, speedF, 0.1f);

    mDoAud_seStartLevel(Z2SE_OBJ_WTR_CLMN_DOWN, &mTopPos, mCurrentHeight, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if(diff == 0.0f)
        actionWaitInit();
}

void daWtPillar_c::actionRockWaitInit() {
    mAction = ACTION_ROCK_WAIT;
}

void daWtPillar_c::actionRockWait() {
    mDoAud_seStartLevel(Z2SE_OBJ_WTR_CLMN_UP, &mTopPos, mMaxHeight + mTargetHeightStalactiteOffset, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if(mCarryingStalactite)
        actionRockOnInit();
}

void daWtPillar_c::actionRockOnInit() {
    mAction = ACTION_ROCK_ON;
}

void daWtPillar_c::actionRockOn() {
    const f32 differenceBetweenCurrentAndMaxHeightWithStalactiteWeight = cLib_addCalc(&mCurrentHeight, mMaxHeight - 250.0f, 3.0f / 5.0f, 10.0f, 0.1f);

    mDoAud_seStartLevel(Z2SE_OBJ_WTR_CLMN_UP, &mTopPos, mMaxHeight + mTargetHeightStalactiteOffset, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if(differenceBetweenCurrentAndMaxHeightWithStalactiteWeight == 0.0f)
        actionEndInit();
}

void daWtPillar_c::actionEndInit() {
    mAction = ACTION_END;
}

void daWtPillar_c::actionEnd() {
    mDoAud_seStartLevel(Z2SE_OBJ_WTR_CLMN_UP, &mTopPos, mMaxHeight + mTargetHeightStalactiteOffset, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

f32 daWtPillar_c::getPillarHeight() {
    return mCurrentHeight;
}

#if DEBUG
void drawCylinder(cXyz* pos, cXyz* scale, csXyz* angle) {
    UNUSED(angle);
    Mtx finalMtx;

    // fakematch?
    GXColor cylinderColor;
    GXColor baseColor = {0x00, 0x00, 0xFF, 0xE6};
    GXColor cylinderColorRedundant;
    *(u32*) &cylinderColorRedundant = *(u32*) &baseColor;
    *(u32*) &cylinderColor = *(u32*) &cylinderColorRedundant;

    mDoMtx_stack_c::transS(pos->x, pos->y, pos->z);
    //! @note Scaling is symmetric with respect to X and Z
    mDoMtx_stack_c::scaleM(150.0f * scale->x, 50.0f * scale->y, 150.0f * scale->x);
    mDoMtx_stack_c::transM(0.0f, 1.0f, 0.0f);
    mDoMtx_stack_c::XrotM(0x4000);
    MTXCopy(mDoMtx_stack_c::get(), finalMtx);

    dDbVw_drawCylinderMXlu(finalMtx, cylinderColor, 1);
}
#endif

int daWtPillar_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    J3DModelData* const modelData = mpModel->getModelData();
    mModelRotAnm.entry(modelData);
    mVTexScrollAnm.entry(modelData);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();


    #if DEBUG
    u8 draw = false;
    if(l_HIO.mTesting) {
        current.angle = l_HIO.mDbgDrawAngle;
        scale = l_HIO.mDbgDrawScale;
    }

    //! @bug mDbgDrawScale is always <scale.x, 0, scale.z>
    if((draw = l_HIO.mDbgDrawingOn)) {
        drawCylinder(&current.pos, &mDbgDrawScale, &current.angle);
    }
    #endif

    return 1;
}

int daWtPillar_c::_delete() {
    dComIfG_resDelete(&mPhase, "efWater");

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

int daWtPillar_Draw(daWtPillar_c* i_this) {
    return i_this->draw();
}

int daWtPillar_Execute(daWtPillar_c* i_this) {
    return i_this->execute();
}

int daWtPillar_Delete(daWtPillar_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "daWtPillar");
    return i_this->_delete();
}

int daWtPillar_Create(fopAc_ac_c* i_this) {
    daWtPillar_c* const actor = static_cast<daWtPillar_c*>(i_this);
    fopAcM_RegisterCreateID(i_this, "daWtPillar");
    return actor->create();
}

actor_method_class l_daWtPillar_Method = {
    (process_method_func)daWtPillar_Create,
    (process_method_func)daWtPillar_Delete,
    (process_method_func)daWtPillar_Execute,
    0,
    (process_method_func)daWtPillar_Draw,
};

actor_process_profile_definition g_profile_Obj_WaterPillar = {
    /* Layer ID     */ fpcLy_CURRENT_e,
    /* List ID      */ 3,
    /* List Prio    */ fpcPi_CURRENT_e,
    /* Proc Name    */ fpcNm_Obj_WaterPillar_e,
    /* Proc SubMtd  */ &g_fpcLf_Method.base,
    /* Size         */ sizeof(daWtPillar_c),
    /* Size Other   */ 0,
    /* Parameters   */ 0,
    /* Leaf SubMtd  */ &g_fopAc_Method.base,
    /* Draw Prio    */ fpcDwPi_Obj_WaterPillar_e,
    /* Actor SubMtd */ &l_daWtPillar_Method,
    /* Status       */ fopAcStts_UNK_0x40000_e,
    /* Group        */ fopAc_ACTOR_e,
    /* Cull Type    */ fopAc_CULLBOX_CUSTOM_e,
};
