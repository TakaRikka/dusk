/**
 * d_a_alink_HIO.inc
 * Player HostIO interface handling
 */

#include "JSystem/JHostIO/JORFile.h"
#include "d/actor/d_a_alink.h"
#include "d/d_com_inf_game.h"

#if DEBUG

static void daAlinkHIO_setAnmGenMessage(daAlinkHIO_anm_c* i_anmP, JORMContext* ctx, char* param_3,
                                        f32 param_4, int param_5) {
    size_t sVar1;
    char acStack_94[100];

    sVar1 = strlen(param_3);
    sprintf(acStack_94, param_3);

    // A Speed
    sprintf(acStack_94 + sVar1, "Ａ速度");
    ctx->genSlider(acStack_94, &i_anmP->mSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // Starting F
    sprintf(acStack_94 + sVar1, "開始Ｆ");
    ctx->genSlider(acStack_94, &i_anmP->mStartFrame, 0.0f, param_4, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);

    // Ending F
    sprintf(acStack_94 + sVar1, "終了Ｆ");
    ctx->genSlider(acStack_94, &i_anmP->mEndFrame, 0, param_4, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);

    if (param_5 != 0) {
        sprintf(acStack_94 + sVar1, "ＣＦ");
        ctx->genSlider(acStack_94, &i_anmP->mCancelFrame, -1.0f, param_4 + 1.0f, 0, NULL, 0xffff,
                       0xffff, 0x200, 0x18);
    }

    // Interpolation
    sprintf(acStack_94 + sVar1, "補間");
    ctx->genSlider(acStack_94, &i_anmP->mInterpolation, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

static void daAlinkHIO_setReverseAnmGenMessage(daAlinkHIO_anm_c* i_anmP, JORMContext* ctx,
                                               char* param_3, f32 param_4, int param_5) {
    size_t sVar1;
    char acStack_94[100];

    sVar1 = strlen(param_3);
    sprintf(acStack_94, param_3);

    // A Speed
    sprintf(acStack_94 + sVar1, "Ａ速度");
    ctx->genSlider(acStack_94, &i_anmP->mSpeed, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);

    // Starting F
    sprintf(acStack_94 + sVar1, "開始Ｆ");
    ctx->genSlider(acStack_94, &i_anmP->mEndFrame, 0, param_4, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);

    // Ending F
    sprintf(acStack_94 + sVar1, "終了Ｆ");
    ctx->genSlider(acStack_94, &i_anmP->mStartFrame, 0.0f, param_4, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);

    if (param_5 != 0) {
        sprintf(acStack_94 + sVar1, "ＣＦ");
        ctx->genSlider(acStack_94, &i_anmP->mCancelFrame, -1.0f, param_4 + 1.0f, 0, NULL, 0xffff,
                       0xffff, 0x200, 0x18);
    }

    // Interpolation
    sprintf(acStack_94 + sVar1, "補間");
    ctx->genSlider(acStack_94, &i_anmP->mInterpolation, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

// NONMATCHING - regalloc + extra mr instruction
void daAlinkHIO_data_c::setStructData(char* param_1) {
    u8* pfVar2;
    u8* pfVar4;

    sprintf(param_1 + strlen(param_1), m_name_string);
    pfVar2 = (u8*)mp_data;

    for (int i = 0; i < m_anm_num; i++) {
        sprintf(param_1 + strlen(param_1), "\t{\n");
        pfVar4 = pfVar2;

        for (int i = 0; i < 2; i++) {
            sprintf(param_1 + strlen(param_1), "\t\t%d,\n", *(s16*)pfVar4);
            pfVar4 += 2;
        }

        pfVar4 = pfVar2;

        for (int i = 0; i < 4; i++) {
            sprintf(param_1 + strlen(param_1), "\t\t%ff,\n", *(f32*)pfVar4);
            pfVar4 += 4;
        }

        pfVar2 += 0x14;
        sprintf(param_1 + strlen(param_1), "\t},\n");
    }

    pfVar4 = pfVar2;

    for (int i = 0; i < m_u8_num; i++) {
        sprintf(param_1 + strlen(param_1), "\t%d,\n", *(u8*)pfVar2);
        pfVar2++;
    }

    for (int i = 0; i < m_s16_num; i++) {
        sprintf(param_1 + strlen(param_1), "\t%d,\n", *(s16*)pfVar2);
        pfVar2 += 2;
    }

    for (int i = 0; i < m_f32_num; i++) {
        sprintf(param_1 + strlen(param_1), "\t%ff,\n", *(f32*)pfVar2);
        pfVar2 += 4;
    }

    sprintf(param_1 + strlen(param_1), "};\n\n");
}

daAlinkHIO_data_c* l_HIO_before;
daAlinkHIO_data_c* l_HIO_top;

void daAlinkHIO_data_c::checkDataSize() {
    u32 dataSize = (m_u8_num + m_anm_num * 0x14) + (m_s16_num * 2) + (m_f32_num * 4);
    JUT_ASSERT(188, dataSize == m_len);

    if (l_HIO_before != NULL) {
        l_HIO_before->mpNext = this;
    } else {
        l_HIO_top = this;
    }

    mpNext = NULL;
    l_HIO_before = this;
}

void daAlinkHIO_data_c::baseCopy() {
    cLib_memCpy(mp_data, mp_src_data, m_len);
}

// NONMATCHING
void daAlinkHIO_data_c::update() {
    baseCopy();
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(this);
    u8* local_20 = (u8*)mp_data;

    for (int i = 0; i < m_anm_num; i++) {
        ctx->updateSlider(2, (s16*)local_20, 0, 0, 0);

        for (int j = 0; j < 4; local_20 += 4, j++) {
            ctx->updateSlider(2, (f32*)local_20, 0.0f, 0.0f, 0);
        }
    }

    for (int i = 0; i < f14; i++) {
        ctx->updateCheckBox(2, local_20, 1, 0);
    }

    for (int i = 0; i < f18; i++) {
        ctx->updateComboBox(2, local_20, 0);
        // local_20++;
    }

    for (int i = 0; i < (m_u8_num - f14 + f18); i++) {
        // local_20++;
    }

    int iVar1 = f1c;
    int iVar3 = m_s16_num;

    for (int i = 0; i < iVar3 - iVar1; i++) {
        ctx->updateSlider(2, (s16*)local_20, 0, 0, 0);
        // local_20 += 2;
    }

    for (int i = 0; i < f1c; i++) {
        // local_20 += 2;
    }

    for (int i = 0; i < m_f32_num; i++) {
        ctx->updateSlider(2, (f32*)local_20, 0.0f, 0.0f, 0);
        // local_20 += 4;
    }

    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

void daAlinkHIO_data_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);

    switch ((u32)event->id)
    case 0x4000003:
        update();
}

void daAlinkHIO_basic_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Basic Interpolation
    ctx->genSlider("基本補間", &m.mBasicInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rotation A Speed
    ctx->genSlider("旋回Ａ速度", &m.mRotationASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max horizontal head turn
    ctx->genSlider("首横向き最大", &m.mHeadMaxTurnHorizontal, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max vertical head turn up
    ctx->genSlider("首上向き最大", &m.mHeadMaxTurnUp, -0x7fff, 0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max vertical head turn down
    ctx->genSlider("首下向き最大", &m.mHeadMaxTurnDown, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Standing guard interpolation
    ctx->genSlider("立ち防御補間", &m.mStandingGuardInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // One hit death
    ctx->genCheckBox("一撃死", (u8*)(&m.mOneHitKill), 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Adult water effect height
    ctx->genSlider("大人水面エフェクト高さ", &m.mWaterSurfaceEffectHeight, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Wolf water effect height
    ctx->genSlider("狼水面エフェクト高さ", &m.mWolfWaterSurfaceEffectHeight, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Max wind influence distance
    ctx->genSlider("風影響最大距離", &m.mMaxWindInfluenceDist, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // No wind influence distance
    ctx->genSlider("風影響無し距離", &m.mNoWindInfluenceDist, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Max wind speed
    ctx->genSlider("風最大速度", &m.mMaxWindSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Lava instant death depth
    ctx->genSlider("溶岩即死深さ", &m.mLavaDeathDepth, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Link -> Wolf Cancel Frame
    ctx->genSlider("リンク→狼ＣＦ", &m.mLinkToWolfCancelFrame, 0.0f, 35.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wolf -> Link Cancel Frame
    ctx->genSlider("狼→リンクＣＦ", &m.mWolfToLinkCancelFrame, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Room speed rate
    ctx->genSlider("部屋速度率", &m.mIndoorSpeedFactor, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Hot spring heal time
    ctx->genSlider("温泉回復時間", &m.mHotspringRecoverTime, 0, 0xff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sand sinking speed
    ctx->genSlider("砂沈み速度", &m.mSandSinkSpeed, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sand floating speed
    ctx->genSlider("砂浮上速度", &m.mSandSurfaceSpeed, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Wii perspective camera speed (horizontal)
    ctx->genSlider("Wii主観カメラ速度横", &m.mWiiCamSpeedH, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wii perspective camera speed (vertical)
    ctx->genSlider("Wii主観カメラ速度縦", &m.mWiiCamSpeedV, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Transformation prohibit distance (near)
    ctx->genSlider("変身禁止距離（近）", &m.mTransformBlockNearDis, 0.0f, 10000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Transformation prohibit distance (far)
    ctx->genSlider("変身禁止距離（遠）", &m.mTransformBlockFarDis, 0.0, 10000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Transformation prohibit angle (far)
    ctx->genSlider("変身禁止角度（遠）", &m.mTransformBlockFarAngle, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_basic_c::daAlinkHIO_basic_c() {
    const static char plabel[] = "const daAlinkHIO_basic_c1 daAlinkHIO_basic_c0::m = {\n";
    m_len = 88;
    mp_src_data = (void*)&daAlinkHIO_basic_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 2;
    m_s16_num = 9;
    m_f32_num = 17;
    f14 = 1;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

daAlinkHIO_data_c::~daAlinkHIO_data_c() {}
daAlinkHIO_data_c::daAlinkHIO_data_c() {}

void daAlinkHIO_move_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速度", &m.mAcceleration, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Wait A speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walking A speed
    ctx->genSlider("歩きＡ速度", &m.mWalkAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Running A speed
    ctx->genSlider("走りＡ速度", &m.mRunAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walking change rate
    ctx->genSlider("歩き変更率", &m.mWalkChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Running change rate
    ctx->genSlider("走り変更率", &m.mRunChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walking minimum synthesis rate
    ctx->genSlider("歩き最低合成率", &m.mMinWalkRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Tired walking minimum synthesis rate
    ctx->genSlider("疲れ歩き最低合成率", &m.mMinTiredWalkRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Rotation angle max
    ctx->genSlider("旋回角度最大", &m.mMaxTurnAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rotation angle min
    ctx->genSlider("旋回角度最小", &m.mMinTurnAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rotation angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0, 20.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slide
    daAlinkHIO_setAnmGenMessage(&m.mSlideAnm, ctx, "滑り", 11.0f, 0);
    // Slide start
    ctx->genSlider("滑り開始", &m.mSlideThresholdSpeed, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Slide initial speed rate
    ctx->genSlider("滑り初速率", &m.mSlideSpeed, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slide deceleration
    ctx->genSlider("滑り減速", &m.mSlideDeceleration, 0.0, 20.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Foot propotional placement when walking
    ctx->genSlider("歩き時足位置比率", &m.mFootPositionRatio, 0.0, 1.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Light stance interpolation
    ctx->genSlider("軽く構え補間", &m.mWaitBInterpolation, 0.0, 10.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_move_c::daAlinkHIO_move_c() {
    static const char plabel[] = "const daAlinkHIO_move_c1 daAlinkHIO_move_c0::m = {\n";
    m_len = 88;
    mp_src_data = (void*)&daAlinkHIO_move_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 15;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_atnMove_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速度", &m.mAcceleration, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle A speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walking A speed
    ctx->genSlider("歩きＡ速度", &m.mWalkAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Running A speed
    ctx->genSlider("走りＡ速度", &m.mRunAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walking update rate
    ctx->genSlider("歩き変更率", &m.mWalkChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Running update rate
    ctx->genSlider("走り変更率", &m.mRunChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walking minimum synthesis rate
    ctx->genSlider("歩き最低合成率", &m.mMinWalkFrame, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Forward walk A speed
    ctx->genSlider("前歩きＡ速度", &m.mWalkForwardAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Forward run A speed
    ctx->genSlider("前走りＡ速度", &m.mRunForwardAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max rotation angle
    ctx->genSlider("旋回角度最大", &m.mMaxTurnAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Min rotation angle
    ctx->genSlider("旋回角度最小", &m.mMinTurnAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rotation angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0, 20.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed backwards
    ctx->genSlider("後方最高速度", &m.mMaxBackwardsSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Acceleration backwards
    ctx->genSlider("後方加速度", &m.mBackAcceleration, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back walk A speed
    ctx->genSlider("後歩Ａ速度", &m.mBackWalkAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back run A speed
    ctx->genSlider("後走Ａ速度", &m.mBackRunAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back walk rate of change
    ctx->genSlider("後歩変更率", &m.mBackWalkChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back run rate of change
    ctx->genSlider("後走変更率", &m.mBackRunChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back walk minimum synthesis rate
    ctx->genSlider("後歩最低合成率", &m.mMinBackWalkFrame, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back walk deceleration
    ctx->genSlider("後方減速", &m.mBackDeceleration, 0.0, 20.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_atnMove_c::daAlinkHIO_atnMove_c() {
    static const char plabel[] = "const daAlinkHIO_atnMove_c1 daAlinkHIO_atnMove_c0::m = {\n";
    m_len = 84;
    mp_src_data = (void*)&daAlinkHIO_atnMove_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 19;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_noActAtnMove_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速度", &m.mAcceleration, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Wait A speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk A speed
    ctx->genSlider("歩きＡ速度", &m.mWalkAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run A speed
    ctx->genSlider("走りＡ速度", &m.mRunAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walk rate of change
    ctx->genSlider("歩き変更率", &m.mWalkChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run rate of change
    ctx->genSlider("走り変更率", &m.mRunChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walk minimum synthesis rate
    ctx->genSlider("歩き最低合成率", &m.mMinWalkFrame, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max rotation angle
    ctx->genSlider("旋回角度最大", &m.mMaxTurnAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Min rotation angle
    ctx->genSlider("旋回角度最小", &m.mMinTurnAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rotation angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0, 20.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max backwards speed
    ctx->genSlider("後方最高速度", &m.mMaxBackwardsSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Backwards acceleration
    ctx->genSlider("後方加速度", &m.mBackAcceleration, 0.0, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back walk A speed
    ctx->genSlider("後歩Ａ速度", &m.mBackWalkAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back run A speed
    ctx->genSlider("後走Ａ速度", &m.mBackRunAnmSpeed, 0.01, 10.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Backwards walk rate of change
    ctx->genSlider("後歩変更率", &m.mBackWalkChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Backwards run rate of change
    ctx->genSlider("後走変更率", &m.mBackRunChangeRate, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Minimum back walk synthesis rate
    ctx->genSlider("後歩最低合成率", &m.mMinBackWalkFrame, 0.0, 1.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Backwards deceleration
    ctx->genSlider("後方減速", &m.mBackDeceleration, 0.0, 20.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Minimum rotation A speed
    ctx->genSlider("旋回Ａ速度最小", &m.mWalkForwardAnmSpeed, 0.0, 5.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Maximum backwards A speed
    ctx->genSlider("後方Ａ速度最大", &m.mRunForwardAnmSpeed, 0.0, 5.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_noActAtnMove_c::daAlinkHIO_noActAtnMove_c() {
    static const char plabel[] =
        "const daAlinkHIO_noActAtnMove_c1 daAlinkHIO_noActAtnMove_c0::m = {\n";
    m_len = 84;
    mp_src_data = (void*)&daAlinkHIO_noActAtnMove_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 19;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_frontRoll_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Forward roll
    daAlinkHIO_setAnmGenMessage(&m.mRollAnm, ctx, "前転", 29.0, 1);
    // Initial speed
    ctx->genSlider("初速", &m.mInitSpeed, 0.0, 50.0, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Speed rate of change
    ctx->genSlider("速度率", &m.mSpeedRate, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Minimum speed
    ctx->genSlider("最低速度", &m.mMinSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Rotation rate
    ctx->genSlider("旋回率", &m.mTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max rotation angle
    ctx->genSlider("旋回最大角度", &m.mMaxTurnAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Min rotation angle
    ctx->genSlider("旋回最小角度", &m.mTurnMinAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Boots attack start F
    ctx->genSlider("ブーツ攻撃開始Ｆ", &m.mBootsAttackInitF, 0.0f, 33.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boots attack end F
    ctx->genSlider("ブーツ攻撃終了Ｆ", &m.mBootsAttackEndF, 0.0f, 33.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boots attack radius
    ctx->genSlider("ブーツ攻撃半径", &m.mBootsAttackRadius, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Required impact speed
    ctx->genSlider("衝突必要速度", &m.mCrashSpeedThreshold, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Required impact angle
    ctx->genSlider("衝突必要角度", &m.mCrashAngleThreshold, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Impact allowed start F
    ctx->genSlider("衝突許可開始Ｆ", &m.mCrashInitF, 0.0f, 29.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Impact allowed end F
    ctx->genSlider("衝突許可終了Ｆ", &m.mCrashEndF, 0.0f, 29.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Impact
    daAlinkHIO_setAnmGenMessage(&m.mCrashAnm, ctx, "衝突", 29.0f, 1);
    // Impact horizontal speed
    ctx->genSlider("衝突水平速度", &m.mCrashSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Impact vertical speed
    ctx->genSlider("衝突垂直速度", &m.mCrashSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Impact success
    daAlinkHIO_setAnmGenMessage(&m.mCrashHitAnm, ctx, "衝突成功", 33.0f, 1);
}

daAlinkHIO_frontRoll_c::daAlinkHIO_frontRoll_c() {
    static const char plabel[] = "const daAlinkHIO_frontRoll_c1 daAlinkHIO_frontRoll_c0::m = {\n";
    m_len = 112;
    mp_src_data = (void*)&daAlinkHIO_frontRoll_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 11;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_backJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Backflip
    daAlinkHIO_setAnmGenMessage(&m.mBackflipAnm, ctx, "バク転", 9.0f, 0);
    // Backflip horizontal speed
    ctx->genSlider("バク転水平速度", &m.mBackflipSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Backflip vertical speed
    ctx->genSlider("バク転垂直速度", &m.mBackflipSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 17.0f, 1);
    // Fall height
    ctx->genSlider("落下高さ", &m.mFallHeight, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall interpolation
    ctx->genSlider("落下補間", &m.mFallInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_backJump_c::daAlinkHIO_backJump_c() {
    static const char plabel[] = "const daAlinkHIO_backJump_c1 daAlinkHIO_backJump_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_backJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_sideStep_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Side step
    daAlinkHIO_setAnmGenMessage(&m.mSideJumpAnm, ctx, "横飛", 5.0f, 0);
    // Side step horizontal speed
    ctx->genSlider("横飛水平速度", &m.mSideJumpSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Side step vertical speed
    ctx->genSlider("横飛垂直速度", &m.mSideJumpSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Side landing
    daAlinkHIO_setAnmGenMessage(&m.mSideLandAnm, ctx, "横着地", 9.0f, 1);
    // Back step
    daAlinkHIO_setAnmGenMessage(&m.mBackJumpAnm, ctx, "後飛", 6.0f, 0);
    // Back step horizontal speed
    ctx->genSlider("後飛水平速度", &m.mBackJumpSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back step vertical speed
    ctx->genSlider("後飛垂直速度", &m.mBackJumpSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back landing
    daAlinkHIO_setAnmGenMessage(&m.mBackLandAnm, ctx, "後着地", 9.0f, 1);
    // Fall height
    ctx->genSlider("落下高さ", &m.mFallHeight, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall interpolation
    ctx->genSlider("落下補間", &m.mFallInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_sideStep_c::daAlinkHIO_sideStep_c() {
    static const char plabel[] = "const daAlinkHIO_sideStep_c1 daAlinkHIO_sideStep_c0::m = {\n";
    m_len = 104;
    mp_src_data = (void*)&daAlinkHIO_sideStep_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 6;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_slide_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slide angle
    ctx->genSlider("滑り角度", &m.mSlideAngle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slope climb angle
    ctx->genSlider("登坂角度", &m.mClimbAngle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slope climb A minimum speed
    ctx->genSlider("登坂Ａ速度最小", &m.mClimbAnmMinSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Slope climb A max speed
    ctx->genSlider("登坂Ａ速度最大", &m.mMaxClimbAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Slope climb max speed
    ctx->genSlider("登坂最高速度", &m.mMaxClimbSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Forward slide
    daAlinkHIO_setAnmGenMessage(&m.mForwardSlideAnm, ctx, "前滑り", 14.0f, 0);
    // Back slide
    daAlinkHIO_setAnmGenMessage(&m.mBackwardSlideAnm, ctx, "後滑り", 6.0f, 0);
    // Max speed
    ctx->genSlider("最大速度", &m.mMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速度", &m.mAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Forward landing
    daAlinkHIO_setAnmGenMessage(&m.mForwardLandAnm, ctx, "前着地", 15.0f, 1);
    // Backward landing
    daAlinkHIO_setAnmGenMessage(&m.mBackwardLandAnm, ctx, "後着地", 20.0f, 1);
    // Lava floor slide angle
    ctx->genSlider("溶岩床滑り角度", &m.mLV2MinibossFloorSlideAngle, 0.0f, 90.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Lava floor weak slide angle
    ctx->genSlider("溶岩床弱滑り角度", &m.mLV2MinibossFloorWeakSlideAngle, 0.0f, 90.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Lava floor weak slide speed
    ctx->genSlider("溶岩床弱滑り速度", &m.mLV2MinibossFloorWeakSlideSpeed, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_slide_c::daAlinkHIO_slide_c() {
    static const char plabel[] = "const daAlinkHIO_slide_c1 daAlinkHIO_slide_c0::m = {\n";
    m_len = 120;
    mp_src_data = (void*)&daAlinkHIO_slide_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 10;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutNormal_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", field_0x34, 1);
    // Speed
    ctx->genSlider("速度", &m.mSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_cutNormal_c::daAlinkHIO_cutNormal_c(int param_0, float param_1) {
    field_0x34 = param_1;

    static void* baseP[5] = {(void*)&daAlinkHIO_cutNmV_c0::m, (void*)&daAlinkHIO_cutNmL_c0::m,
                             (void*)&daAlinkHIO_cutNmR_c0::m, (void*)&daAlinkHIO_cutNmSL_c0::m,
                             (void*)&daAlinkHIO_cutNmSR_c0::m};

    static const char* plabel[] = {"const daAlinkHIO_cutNormal_c1 daAlinkHIO_cutNmSL_c0::m = {\n",
                                   "const daAlinkHIO_cutNormal_c1 daAlinkHIO_cutNmV_c0::m = {\n",
                                   "const daAlinkHIO_cutNormal_c1 daAlinkHIO_cutNmL_c0::m = {\n",
                                   "const daAlinkHIO_cutNormal_c1 daAlinkHIO_cutNmSR_c0::m = {\n",
                                   "const daAlinkHIO_cutNormal_c1 daAlinkHIO_cutNmR_c0::m = {\n"};

    m_len = 32;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutFinish_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", field_0x34, 1);
    // Speed
    ctx->genSlider("速度", &m.mSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Stop time
    ctx->genSlider("停止時間", &m.mStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Mid-combo start F
    ctx->genSlider("コンボ中開始Ｆ", &m.mComboStartFrame, 0.0f, field_0x34 + 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Mid-combo CF
    ctx->genSlider("コンボ中ＣＦ", &m.mComboCheckFrame, 0.0f, field_0x34 + 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Mid-combo stop time
    ctx->genSlider("コンボ中停止時間", &m.mComboStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_cutFinish_c::daAlinkHIO_cutFinish_c(int param_0, float param_1) {
    field_0x34 = param_1;

    static void* baseP[6] = {(void*)&daAlinkHIO_cutFnL_c0::m,  (void*)&daAlinkHIO_cutFnV_c0::m,
                             (void*)&daAlinkHIO_cutFnS_c0::m,  (void*)&daAlinkHIO_cutFnSl_c0::m,
                             (void*)&daAlinkHIO_cutFnSm_c0::m, (void*)&daAlinkHIO_cutFnR_c0::m};

    static const char* plabel[] = {
        "const daAlinkHIO_cutFinish_c1 daAlinkHIO_cutFnL_c0::m = {\n",
        "const daAlinkHIO_cutFinish_c1 daAlinkHIO_cutFnV_c0::m = {\n",
        "const daAlinkHIO_cutFinish_c1 daAlinkHIO_cutFnS_c0::m = {\n",
        "const daAlinkHIO_cutFinish_c1 daAlinkHIO_cutFnSl_c0::m = {\n",
        "const daAlinkHIO_cutFinish_c1 daAlinkHIO_cutFnSm_c0::m = {\n",
        "const daAlinkHIO_cutFinish_c1 daAlinkHIO_cutFnR_c0::m = {\n",
    };

    m_len = 44;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 5;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutFnJU_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", 19.0f, 0);
    // Horizontal speed
    ctx->genSlider("水平速度", &m.mSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Vertical speed
    ctx->genSlider("垂直速度", &m.mSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, 19.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, 19.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Stop time
    ctx->genSlider("停止時間", &m.mStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Mid-combo start F
    ctx->genSlider("コンボ中開始Ｆ", &m.mComboStartFrame, 0.0f, 19.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 17.0f, 1);
    // Mid-combo CF
    ctx->genSlider("コンボ中ＣＦ", &m.mComboCheckFrame, 0.0f, 18.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Mid-combo stop time
    ctx->genSlider("コンボ中停止時間", &m.mComboStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Fall height
    ctx->genSlider("落下高さ", &m.mFallHeight, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall interpolation
    ctx->genSlider("落下補間", &m.mFallInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack offset
    ctx->genSlider("攻撃オフセット", &m.mAttackOffset, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_cutFnJU_c::daAlinkHIO_cutFnJU_c() {
    static const char plabel[] = "const daAlinkHIO_cutFnJU_c1 daAlinkHIO_cutFnJU_c0::m = {\n";
    m_len = 84;
    mp_src_data = (void*)&daAlinkHIO_cutFnJU_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 10;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutDash_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", field_0x34, 1);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_cutDash_c::daAlinkHIO_cutDash_c(int param_0, float param_1) {
    field_0x34 = param_1;

    static void* baseP[3] = {
        (void*)&daAlinkHIO_cutDaL_c0::m,
        (void*)&daAlinkHIO_cutDaR_c0::m,
        (void*)&daAlinkHIO_cutDaCharge_c0::m,
    };

    static const char* plabel[] = {
        "const daAlinkHIO_cutDash_c1 daAlinkHIO_cutDaL_c0::m = {\n",
        "const daAlinkHIO_cutDash_c1 daAlinkHIO_cutDaR_c0::m = {\n",
        "const daAlinkHIO_cutDash_c1 daAlinkHIO_cutDaCharge_c0::m = {\n",
    };

    m_len = 28;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 2;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", 17.0f, 0);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mStartAttackFrame, 0.0f, 17.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mEndAttackFrame, 0.0f, 14.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Horizontal cut speed
    ctx->genSlider("斬り水平速度", &m.mBaseJumpSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Vertical cut speed
    ctx->genSlider("斬り垂直速度", &m.mBaseJumpSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Cut short horizontal speed
    ctx->genSlider("斬り短い水平速度", &m.mAirJumpSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Cut short vertical speed
    ctx->genSlider("斬り短い垂直速度", &m.mAirJumpSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 29.0f, 1);
    // Spin attack CF
    ctx->genSlider("回転斬りＣＦ", &m.mJumpSpinDelay, 0.0f, 29.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_cutJump_c::daAlinkHIO_cutJump_c() {
    static const char plabel[] = "const daAlinkHIO_cutJump_c1 daAlinkHIO_cutJump_c0::m = {\n";
    m_len = 68;
    mp_src_data = (void*)&daAlinkHIO_cutJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 7;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutTurn_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Right spin
    daAlinkHIO_setAnmGenMessage(&m.mRightTurnAnm, ctx, "右回り", 47.0f, 1);
    // Right mid-combo start F
    ctx->genSlider("右コンボ中開始Ｆ", &m.mRightComboStartFrame, 0.0f, 47.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Right spin input time start F
    ctx->genSlider("右回転入力時開始Ｆ", &m.mRightTurnInputStartFrame, 0.0f, 47.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Right attack start F
    ctx->genSlider("右攻撃開始Ｆ", &m.mRightAttackStartFrame, 0.0f, 47.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Right attack end F
    ctx->genSlider("右攻撃終了Ｆ", &m.mRightAttackEndFrame, 0.0f, 47.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left spin
    daAlinkHIO_setAnmGenMessage(&m.mLeftTurnAnm, ctx, "左回り", 42.0f, 1);
    // Left mid-combo start F
    ctx->genSlider("左コンボ中開始Ｆ", &m.mLeftComboStartFrame, 0.0f, 42.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Left spin input time start F
    ctx->genSlider("左回転入力時開始Ｆ", &m.mLeftTurnInputStartFrame, 0.0f, 42.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Left attack start F
    ctx->genSlider("左攻撃開始Ｆ", &m.mLeftAttackStartFrame, 0.0f, 42.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left attack end F
    ctx->genSlider("左攻撃終了Ｆ", &m.mLeftAttackEndFrame, 0.0f, 42.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Stop time
    ctx->genSlider("停止時間", &m.mStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Charge movement
    daAlinkHIO_setAnmGenMessage(&m.mChargeAnm, ctx, "ため移行", 9.0f, 0);
    // Forward/back movement A speed
    ctx->genSlider("前後移動Ａ速度", &m.mMoveFBAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Forward/back movement interpolation
    ctx->genSlider("前後移動補間", &m.mMoveFBInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left/right movement A speed
    ctx->genSlider("左右移動Ａ速度", &m.mMoveLRAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left/right movement interpolation
    ctx->genSlider("左右移動補間", &m.mMoveLRInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Charge movement acceleration
    ctx->genSlider("ため移動加速", &m.mChargeMoveAccel, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Charge movement deceleration
    ctx->genSlider("ため移動減速", &m.mChargeMoveDecel, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Speed
    ctx->genSlider("速度", &m.mSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack radius speed increase
    ctx->genSlider("攻撃半径増加速度", &m.mAttackRadiusAccel, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Light sword attack radius
    ctx->genSlider("光攻撃半径", &m.mLightAttackRadius, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Light sword attack radius speed increase
    ctx->genSlider("光攻撃半径増加速度", &m.mLightAttackRadiusAccel, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Big attack radius
    ctx->genSlider("大攻撃半径", &m.mLargeAttackRadius, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Big attack radius speed increase
    ctx->genSlider("大攻撃半径増加速度", &m.mLargeAttackAccel, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_cutTurn_c::daAlinkHIO_cutTurn_c() {
    static const char plabel[] = "const daAlinkHIO_cutTurn_c1 daAlinkHIO_cutTurn_c0::m = {\n";
    m_len = 152;
    mp_src_data = (void*)&daAlinkHIO_cutTurn_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 22;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_hoCut_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", field_0x34, 1);
    // Post-cut interpolation
    ctx->genSlider("斬り後補間", &m.mAfterCutMorf, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_hoCut_c::daAlinkHIO_hoCut_c(int param_0, float param_1) {
    field_0x34 = param_1;

    static void* baseP[4] = {
        (void*)&daAlinkHIO_hoCutLA_c0::m,
        (void*)&daAlinkHIO_hoCutLB_c0::m,
        (void*)&daAlinkHIO_hoCutRA_c0::m,
        (void*)&daAlinkHIO_hoCutRB_c0::m,
    };

    static const char* plabel[] = {
        "const daAlinkHIO_hoCut_c1 daAlinkHIO_hoCutLA_c0::m = {\n",
        "const daAlinkHIO_hoCut_c1 daAlinkHIO_hoCutLB_c0::m = {\n",
        "const daAlinkHIO_hoCut_c1 daAlinkHIO_hoCutRA_c0::m = {\n",
        "const daAlinkHIO_hoCut_c1 daAlinkHIO_hoCutRB_c0::m = {\n",
    };

    m_len = 32;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_hoCutCharge_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Charge
    daAlinkHIO_setAnmGenMessage(&m.mChargeAnm, ctx, "ため", 6.0f, 0);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", 21.0f, 0);
    // Cut attack start F
    ctx->genSlider("斬り攻撃開始Ｆ", &m.mCutAttackStartFrame, 0.0f, 21.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Cut attack end F
    ctx->genSlider("斬り攻撃終了Ｆ", &m.mCutAttackEndFrame, 0.0f, 21.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Cut stop time
    ctx->genSlider("斬り停止時間", &m.mCutStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Spin
    daAlinkHIO_setAnmGenMessage(&m.mSpinAnm, ctx, "回転", 49.0f, 1);
    // Spin attack start F
    ctx->genSlider("回転攻撃開始Ｆ", &m.mSpinAttackStartFrame, 0.0f, 49.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Spin attack end F
    ctx->genSlider("回転攻撃終了Ｆ", &m.mSpinAttackEndFrame, 0.0f, 49.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Spin attack radius
    ctx->genSlider("回転攻撃半径", &m.mSpinAttackRadius, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Spin stop time
    ctx->genSlider("回転停止時間", &m.mSpinStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_hoCutCharge_c::daAlinkHIO_hoCutCharge_c() {
    static const char plabel[] =
        "const daAlinkHIO_hoCutCharge_c1 daAlinkHIO_hoCutCharge_c0::m = {\n";
    m_len = 84;
    mp_src_data = (void*)&daAlinkHIO_hoCutCharge_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 5;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutDown_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Vertical speed
    ctx->genSlider("垂直速度", &m.mRecoverSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump
    daAlinkHIO_setAnmGenMessage(&m.mJumpAnm, ctx, "飛出", 8.0f, 0);
    // Brace
    daAlinkHIO_setAnmGenMessage(&m.mFallAnm, ctx, "構え", 15.0f, 0);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 16.0f, 0);
    // Success stop time
    ctx->genSlider("成功停止時間", &m.mSuccessStopTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Failure stop time
    ctx->genSlider("失敗停止時間", &m.mFailStopTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Return horizontal speed
    ctx->genSlider("戻り水平速度", &m.mRecoverSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Return vertical speed
    ctx->genSlider("戻り垂直速度", &m.mSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Return
    daAlinkHIO_setAnmGenMessage(&m.mRecoverAnm, ctx, "戻り", 9.0f, 0);
    // Failure
    daAlinkHIO_setAnmGenMessage(&m.mMissAnm, ctx, "失敗", 73.0f, 1);
}

daAlinkHIO_cutDown_c::daAlinkHIO_cutDown_c() {
    static const char plabel[] = "const daAlinkHIO_cutDown_c1 daAlinkHIO_cutDown_c0::m = {\n";
    m_len = 116;
    mp_src_data = (void*)&daAlinkHIO_cutDown_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 5;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutHead_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Jump
    daAlinkHIO_setAnmGenMessage(&m.mJumpAnm, ctx, "飛出", 8.0f, 0);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max height
    ctx->genSlider("最大高さ", &m.mMaxHeight, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max distance
    ctx->genSlider("最大距離", &m.mMaxDistance, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", 21.0f, 0);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, 21.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, 21.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sword length ratio
    ctx->genSlider("剣長さ比率", &m.mSwordLength, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Sword radius
    ctx->genSlider("剣半径", &m.mSwordRadius, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Additional vertical speed
    ctx->genSlider("追加垂直速度", &m.mAddSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Additional horizontal speed
    ctx->genSlider("追加水平速度", &m.mAddSpeedH, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 21.0f, 1);
}

daAlinkHIO_cutHead_c::daAlinkHIO_cutHead_c() {
    static const char plabel[] = "const daAlinkHIO_cutHead_c1 daAlinkHIO_cutHead_c0::m = {\n";
    m_len = 96;
    mp_src_data = (void*)&daAlinkHIO_cutHead_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 9;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cutLargeJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Charge
    daAlinkHIO_setAnmGenMessage(&m.mChargeAnm, ctx, "ため", 29.0f, 0);
    // Jump forward
    daAlinkHIO_setAnmGenMessage(&m.mChargeMoveAnm, ctx, "飛前", 6.0f, 0);
    // Jump forward attack start F
    ctx->genSlider("飛前攻撃開始Ｆ", &m.mJumpAttackStartFrame, 0.0f, 6.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump forward attack end F
    ctx->genSlider("飛前攻撃終了Ｆ", &m.mJumpAttackEndFrame, 0.0f, 6.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Cut
    daAlinkHIO_setAnmGenMessage(&m.mCutAnm, ctx, "斬り", 16.0f, 0);
    // Cut attack start F
    ctx->genSlider("斬り攻撃開始Ｆ", &m.mCutInitFrame, 0.0f, 16.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Cut horizontal speed
    ctx->genSlider("斬り水平速度", &m.mCutSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Cut vertical speed
    ctx->genSlider("斬り垂直速度", &m.mCutSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 35.0f, 1);
    // Spin attack CF
    ctx->genSlider("回転斬りＣＦ", &m.mSpinSlashCheckFrame, 0.0f, 35.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Landing attack start F
    ctx->genSlider("着地攻撃開始Ｆ", &m.mLandAttackInitFrame, 0.0f, 35.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Landing attack end F
    ctx->genSlider("着地攻撃終了Ｆ", &m.mLandAttackEndFrame, 0.0f, 35.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Landing attack radius
    ctx->genSlider("着地攻撃半径", &m.mLandAttackRadius, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_cutLargeJump_c::daAlinkHIO_cutLargeJump_c() {
    static const char plabel[] =
        "const daAlinkHIO_cutLargeJump_c1 daAlinkHIO_cutLargeJump_c0::m = {\n";
    m_len = 116;
    mp_src_data = (void*)&daAlinkHIO_cutLargeJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 9;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_cut_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Vertical
    ctx->genNode("縦", &mCutVertical, 0, 0);
    // Left
    ctx->genNode("左", &mCutLeft, 0, 0);
    // Right
    ctx->genNode("右", &mCutRight, 0, 0);
    // Left stab
    ctx->genNode("左突", &mCutLeftStab, 0, 0);
    // Right stab
    ctx->genNode("右突", &mCutRightStab, 0, 0);
    // E left
    ctx->genNode("Ｅ左", &mCutFinishLeft, 0, 0);
    // E vertical
    ctx->genNode("Ｅ縦", &mCutFinishVertical, 0, 0);
    // E stab
    ctx->genNode("Ｅ突", &mCutFinishStab, 0, 0);
    // E sweep
    ctx->genNode("Ｅ払", &mCutFinishSweep, 0, 0);
    // E slash
    ctx->genNode("Ｅ袈裟", &mCutFinishSlash, 0, 0);
    // E right
    ctx->genNode("Ｅ右", &mCutFinishRight, 0, 0);
    // E J upper cut
    ctx->genNode("ＥＪ斬上", &mCutFinishJumpUppercut, 0, 0);
    // Jump
    ctx->genNode("ジャンプ", &mCutJump, 0, 0);
    // Spin
    ctx->genNode("回転", &mCutTurn, 0, 0);
    // Horse left A
    ctx->genNode("馬上左Ａ", &mHorseCutLeftA, 0, 0);
    // Horse left B
    ctx->genNode("馬上左Ｂ", &mHorseCutLeftB, 0, 0);
    // Horse right A
    ctx->genNode("馬上右Ａ", &mHorseCutRightA, 0, 0);
    // Horse right B
    ctx->genNode("馬上右Ｂ", &mHorseCutRightB, 0, 0);
    // Horse charge
    ctx->genNode("馬上ため", &mHorseCutCharge, 0, 0);
    // Run left
    ctx->genNode("走り左", &mCutDashLeft, 0, 0);
    // Run right
    ctx->genNode("走り右", &mCutDashRight, 0, 0);
    // Run charge
    ctx->genNode("走りため", &mCutDashCharge, 0, 0);
    // Ending Blow
    ctx->genNode("ダウン下突き", &mCutDown, 0, 0);
    // Helm Splitter
    ctx->genNode("兜割", &mCutHead, 0, 0);
    // Jump Strike
    ctx->genNode("大ジャンプ", &mCutLargeJump, 0, 0);
    // Equip
    daAlinkHIO_setReverseAnmGenMessage(&m.mEquipAnm, ctx, "装備", 22.0f, 0);
    // Cancel
    daAlinkHIO_setAnmGenMessage(&m.mUnequipAnm, ctx, "解除", 22.0f, 0);
    // Early cancel A speed
    ctx->genSlider("早解除Ａ速度", &m.mFastUnequipAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Combo continue time
    ctx->genSlider("コンボ持続時間", &m.mComboDuration, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump back
    daAlinkHIO_setAnmGenMessage(&m.mRecoilAnm, ctx, "跳返", 45.0f, 1);
    // Jump back speed
    ctx->genSlider("跳返速度", &m.mRecoilSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Jump back deceleration
    ctx->genSlider("跳返減速", &m.mRecoilDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Blur alpha
    ctx->genSlider("ブラーアルファ", &m.mBlurAlpha, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Dash blur alpha
    ctx->genSlider("ダッシュブラーアルファ", &m.mDashBlurAlpha, 0, 0xff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Special sword cancel A speed
    ctx->genSlider("特殊剣解除Ａ速度", &m.mFlourishAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Special sword cancel finish A speed
    ctx->genSlider("特殊剣解除終了Ａ速度", &m.mFlourishEndAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Special sword cancel CF
    ctx->genSlider("特殊剣解除ＣＦ", &m.mFlourishControlStartFrame, 0.0f, 59.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Special sword cancel accept time
    ctx->genSlider("特殊剣解除受付時間", &m.mFlourishTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sword length ratio
    ctx->genSlider("剣長さ比率", &m.mSwordLength, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sword radius
    ctx->genSlider("剣半径", &m.mSwordRadius, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Mounted sword length ratio
    ctx->genSlider("馬上剣長さ比率", &m.mSwordLengthHorseback, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Mounted sword radius
    ctx->genSlider("馬上剣半径", &m.mSwordRadiusHorseback, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Running sword length ratio
    ctx->genSlider("走り剣長さ比率", &m.mRunCutLength, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Running sword radius
    ctx->genSlider("走り剣半径", &m.mRunCutRadius, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Normal sword swing decision time
    ctx->genSlider("通常剣振り判定時間", &m.mNormalSwingDuration, 0, 0x1e, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Force hit combo
    ctx->genCheckBox("強制ヒットコンボ", (u8*)&m.mForceHitCombo, 1, 0, NULL, 0xffff, 0xffff, 0x200,
                     0x18);
    // Mounted duel sword length ratio
    ctx->genSlider("一騎打ち馬上剣長さ比率", &m.mSwordLengthHorsebackFight, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Mounted duel sword radius
    ctx->genSlider("一騎打ち馬上剣半径", &m.mSwordRadiusHorsebackFight, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Mortal draw time
    ctx->genSlider("居合時間", &m.mUnkTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max body face up
    ctx->genSlider("体上向き最大", &m.mUnkBodyUpwards, -0x7fff, 0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max body face down
    ctx->genSlider("体下向き最大", &m.mUnkBodyDownwards, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // OI stab
    daAlinkHIO_setAnmGenMessage(&m.mStabAnm, ctx, "ＯＩ刺し", 47.0f, 1);
    // Spin attack wait
    ctx->genSlider("回転斬り待ち", &m.mSpinSlashWait, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_cut_c::daAlinkHIO_cut_c()
    : mCutVertical(0, 27.0f), mCutLeft(1, 27.0f), mCutRight(2, 27.0f), mCutLeftStab(3, 31.0f),
      mCutRightStab(4, 27.0f), mCutFinishLeft(0, 29.0f), mCutFinishVertical(1, 33.0f),
      mCutFinishStab(2, 31.0f), mCutFinishSweep(3, 33.0f), mCutFinishSlash(4, 31.0f),
      mCutFinishRight(5, 34.0f), mCutFinishJumpUppercut(), mCutJump(), mCutTurn(),
      mHorseCutLeftA(0, 21.0f), mHorseCutLeftB(1, 24.0f), mHorseCutRightA(2, 24.0f),
      mHorseCutRightB(3, 11.0f), mHorseCutCharge(), mCutDashLeft(0, 12.0f), mCutDashRight(1, 14.0f),
      mCutDashCharge(2, 14.0f), mCutDown(), mCutHead(), mCutLargeJump() {
    static const char plabel[] = "const daAlinkHIO_cut_c1 daAlinkHIO_cut_c0::m = {\n";
    m_len = 156;
    mp_src_data = (void*)&daAlinkHIO_cut_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 2;
    m_s16_num = 9;
    m_f32_num = 14;
    f14 = 1;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

daAlinkHIO_cutLargeJump_c::~daAlinkHIO_cutLargeJump_c() {}
daAlinkHIO_cutHead_c::~daAlinkHIO_cutHead_c() {}
daAlinkHIO_cutDown_c::~daAlinkHIO_cutDown_c() {}
daAlinkHIO_cutDash_c::~daAlinkHIO_cutDash_c() {}
daAlinkHIO_hoCutCharge_c::~daAlinkHIO_hoCutCharge_c() {}
daAlinkHIO_hoCut_c::~daAlinkHIO_hoCut_c() {}
daAlinkHIO_cutTurn_c::~daAlinkHIO_cutTurn_c() {}
daAlinkHIO_cutJump_c::~daAlinkHIO_cutJump_c() {}
daAlinkHIO_cutFnJU_c::~daAlinkHIO_cutFnJU_c() {}
daAlinkHIO_cutFinish_c::~daAlinkHIO_cutFinish_c() {}
daAlinkHIO_cutNormal_c::~daAlinkHIO_cutNormal_c() {}

void daAlinkHIO_guardAttack_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack
    daAlinkHIO_setAnmGenMessage(&m.mAttackAnm, ctx, "攻撃", field_0x34, 1);
    // Slash check frame
    ctx->genSlider("剣振りＣＦ", &m.mSlashCheckFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Speed
    ctx->genSlider("速度", &m.mSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack start F
    ctx->genSlider("攻撃開始Ｆ", &m.mAttackStartFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack end F
    ctx->genSlider("攻撃終了Ｆ", &m.mAttackEndFrame, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_guardAttack_c::daAlinkHIO_guardAttack_c(int param_0, float param_1) {
    field_0x34 = param_1;

    static void* baseP[2] = {
        (void*)&daAlinkHIO_gAtPush_c0::m,
        (void*)&daAlinkHIO_gAtKick_c0::m,
    };

    static const char* plabel[] = {
        "const daAlinkHIO_guardAttack_c1 daAlinkHIO_gAtPush_c0::m = {\n",
        "const daAlinkHIO_guardAttack_c1 daAlinkHIO_gAtKick_c0::m = {\n",
    };

    m_len = 36;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_turnMove_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    //
    daAlinkHIO_setAnmGenMessage((daAlinkHIO_anm_c*)(this + 0x34), ctx, "回込", 27.0, 1);
    //
    ctx->genSlider("半回転回込Ａ速度", (float*)(this + 0x50), 0.0, 10.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    //
    ctx->genSlider("斬上げＣＦ", (float*)(this + 0x54), 0.0, 28.0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    //
    ctx->genSlider("回転速度率", (short*)(this + 0x48), 0, 100, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    //
    ctx->genSlider("回転速度最大", (short*)(this + 0x4a), 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    //
    ctx->genSlider("回転速度最小", (short*)(this + 0x4c), 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    //
    ctx->genSlider("半回転速度最大", (short*)(this + 0x4e), 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    //
    ctx->genSlider("通常側転Ａ速度", (float*)(this + 0x58), 0.0, 10.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    //
    ctx->genSlider("通常側転速度", (float*)(this + 0x5c), 0.0, 100.0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_turnMove_c::daAlinkHIO_turnMove_c() {
    static const char plabel[] = "const daAlinkHIO_turnMove_c1 daAlinkHIO_turnMove_c0::m = {\n";
    m_len = 44;
    mp_src_data = (void*)&daAlinkHIO_turnMove_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_guard_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Shield
    ctx->genNode("盾", &mAtPush, 0, 0);
    // Sidestep
    ctx->genNode("回り込み", &mTurnMove, 0, 0);
    // Crouch guard anm speed
    ctx->genSlider("しゃがみ防御Ａ速度", &m.mCrouchGuardAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Crouch guard interpolation
    ctx->genSlider("しゃがみ防御補間", &m.mCrouchGuardInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Guard hit
    daAlinkHIO_setAnmGenMessage(&m.mGuardHitAnm, ctx, "防御ヒット", 11.0f, 0);
    // Crouch guard hit
    daAlinkHIO_setAnmGenMessage(&m.mCrouchGuardHitAnm, ctx, "しゃがみ防御ヒット", 11.0f, 0);
    // Guard speed normal
    ctx->genSlider("防御通常速度", &m.mGuardSpeedNormal, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Guard speed large
    ctx->genSlider("防御大速度", &m.mGuardSpeedLarge, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Guard speed huge
    ctx->genSlider("防御強大速度", &m.mGuardSpeedHuge, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Magne guard speed
    ctx->genSlider("大マグネ速度", &m.mMagneGuardSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Magne heavy guard speed
    ctx->genSlider("大マグネ重い速度", &m.mMagneHvyGuardSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Guard front-back body angle max
    ctx->genSlider("防御前後体角度最大", &m.mGuardFBAngleMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Guard left-right body angle max
    ctx->genSlider("防御左右体角度最大", &m.mGuardLRAngleMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Guard body interpolation
    ctx->genSlider("防御体角度補間", &m.mGuardBodyInterpolation, 0, 0x1e, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack position offset
    ctx->genSlider("攻撃位置オフセット", &m.mAttackPosOffset, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Small guard front-back body angle max
    ctx->genSlider("小防御前後体角度最大", &m.mSmallGuardFBAngleMax, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Small guard left-right body angle max
    ctx->genSlider("小防御左右体角度最大", &m.mSmallGuardLRAngleMax, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Recoil
    daAlinkHIO_setAnmGenMessage(&m.mRecoilAnm, ctx, "跳返", 19.0f, 1);
    // Guard break
    daAlinkHIO_setAnmGenMessage(&m.mGuardBreakAnm, ctx, "崩され", 35.0f, 1);
}

daAlinkHIO_guard_c::daAlinkHIO_guard_c()
    : mAtPush(0, 17.0f), mAtKick(1, 21.0f), mTurnMove() {
    static const char plabel[] = "const daAlinkHIO_guard_c1 daAlinkHIO_guard_c0::m = {\n";
    m_len = 128;
    mp_src_data = (void*)&daAlinkHIO_guard_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 6;
    m_f32_num = 9;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

daAlinkHIO_turnMove_c::~daAlinkHIO_turnMove_c() {}
daAlinkHIO_guardAttack_c::~daAlinkHIO_guardAttack_c() {}

void daAlinkHIO_crouch_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Crouch animation speed
    ctx->genSlider("しゃがみＡ速度", &m.mCrouchAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Crouch interpolation
    ctx->genSlider("しゃがみ補間", &m.mCrouchInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Stand interpolation
    ctx->genSlider("立ち上がり補間", &m.mStandInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Crawl start
    daAlinkHIO_setAnmGenMessage(&m.mCrawlStartAnm, ctx, "ほふく開始", 9.0f, 1);
    // Crawl minimum animation speed
    ctx->genSlider("ほふく最小Ａ速度", &m.mCrawlAnmSpeedMin, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Crawl maximum animation speed
    ctx->genSlider("ほふく最大Ａ速度", &m.mCrawlAnmSpeedMax, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Crawl movement interpolation
    ctx->genSlider("ほふく移動補間", &m.mCrawlInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Crawl movement rate
    ctx->genSlider("ほふく移動率", &m.mCrawlMoveRate, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Crawl turn rate
    ctx->genSlider("ほふく旋回率", &m.mCrawlTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Crawl turn maximum
    ctx->genSlider("ほふく旋回最大", &m.mCrawlTurnMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Crawl turn minimum
    ctx->genSlider("ほふく旋回最小", &m.mCrawlTurnMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Crawl end
    daAlinkHIO_setReverseAnmGenMessage(&m.mCrawlEndAnm, ctx, "ほふく終了", 9.0f, 1);
}

daAlinkHIO_crouch_c::daAlinkHIO_crouch_c() {
    static const char plabel[] = "const daAlinkHIO_crouch_c1 daAlinkHIO_crouch_c0::m = {\n";
    m_len = 76;
    mp_src_data = (void*)&daAlinkHIO_crouch_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 7;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_autoJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max fall speed
    ctx->genSlider("最大落下速度", &m.mMaxFallSpeed, -300.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump
    daAlinkHIO_setAnmGenMessage(&m.mJumpAnm, ctx, "ジャンプ", 6.0f, 0);
    // Jump speed limit
    ctx->genSlider("ジャンプ限度速度", &m.mJumpSpeedLimit, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Min jump speed
    ctx->genSlider("ジャンプ最低速度", &m.mMinJumpSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max jump speed
    ctx->genSlider("ジャンプ最高速度", &m.mMaxJumpSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump speed rate
    ctx->genSlider("ジャンプ速度率", &m.mJumpSpeedRate, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump angle
    ctx->genSlider("ジャンプ角度", &m.mJumpAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Airborne interpolation
    ctx->genSlider("滞空補間", &m.mAirborneInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump fall interpolation
    ctx->genSlider("ジャンプ落下補間", &m.mJumpFallInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Always max speed jump
    ctx->genCheckBox("必ず最高速度ジャンプ", (u8*)&m.mAlwaysMaxSpeedJump, 1, 0, NULL, 0xffff,
                     0xffff, 0x200, 0x18);
    // Land
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 7.0f, 1);
    // Fall interpolation
    ctx->genSlider("通常落下補間", &m.mFallInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Spin jump interpolation
    ctx->genSlider("回転ジャンプ補間", &m.mSpinJumpInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Spin jump rotate speed
    ctx->genSlider("回転ジャンプ回転速度", &m.mSpinJumpRotateSpeed, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Spin jump fall interpolation
    ctx->genSlider("回転ジャンプ落下補間", &m.mSpinJumpFallInterpolation, 0.0f, 20.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Spin jump land stop time
    ctx->genSlider("回転ジャンプ着地停止時間", &m.mSpinJumpLandStopTime, 0, 100, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Spin jump add speed
    ctx->genSlider("回転ジャンプ追加速度", &m.mSpinJumpAddSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Spin jump accel
    ctx->genSlider("回転ジャンプ加速度", &m.mSpinJumpAccel, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Hang height limit
    ctx->genSlider("ぶら下がり限界高さ", &m.mHangHeightLimit, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Grab height limit
    ctx->genSlider("掴まり限界高さ", &m.mGrabHeightLimit, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Ooccoo jump max speed
    ctx->genSlider("天空人滑空ジャンプ最高速度", &m.mOoccooJumpMaxSpeed, 0.0f, 50.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Dive
    daAlinkHIO_setAnmGenMessage(&m.mDiveAnm, ctx, "飛込", 4.0f, 0);
    // Dive connect
    daAlinkHIO_setAnmGenMessage(&m.mDiveConnectAnm, ctx, "飛込繋", 10.0f, 0);
    // Dive horizontal speed
    ctx->genSlider("飛込水平速度", &m.mDiveSpeedH, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dive vertical speed
    ctx->genSlider("飛込垂直速度", &m.mDiveSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dive gravity
    ctx->genSlider("飛込重力", &m.mDiveGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Cucco jump max speed
    ctx->genSlider("コッコジャンプ最高速度", &m.mCuccoJumpMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Cucco jump angle
    ctx->genSlider("コッコジャンプ角度", &m.mCuccoJumpAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Cucco max fall speed
    ctx->genSlider("コッコ最大落下速度", &m.mCuccoFallMaxSpeed, -300.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Cucco start speed
    ctx->genSlider("コッコ初速", &m.mCuccoStartSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_autoJump_c::daAlinkHIO_autoJump_c() {
    static const char plabel[] = "const daAlinkHIO_autoJump_c1 daAlinkHIO_autoJump_c0::m = {\n";
    m_len = 180;
    mp_src_data = (void*)&daAlinkHIO_autoJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 2;
    m_s16_num = 5;
    m_f32_num = 22;
    f14 = 1;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_smallJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Small jump
    daAlinkHIO_setAnmGenMessage(&m.mSmallJumpAnm, ctx, "小ジャンプ", 8.0f, 0);
    // Horizontal speed
    ctx->genSlider("水平速度", &m.mSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Target height offset
    ctx->genSlider("目標高さオフセット", &m.mTargetHeightOffset, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Fall interpolation
    ctx->genSlider("落下補間", &m.mFallInterpolation, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Step climb
    daAlinkHIO_setAnmGenMessage(&m.mStepClimbAnm, ctx, "段差登", 5.0f, 0);
    // Step land
    daAlinkHIO_setAnmGenMessage(&m.mStepLandAnm, ctx, "段差着地", 29.0f, 1);
}

daAlinkHIO_smallJump_c::daAlinkHIO_smallJump_c() {
    static const char plabel[] = "const daAlinkHIO_smallJump_c1 daAlinkHIO_smallJump_c0::m = {\n";
    m_len = 72;
    mp_src_data = (void*)&daAlinkHIO_smallJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wallCatch_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Grab A
    daAlinkHIO_setAnmGenMessage(&m.mGrabAAnm, ctx, "掴みＡ", 19.0f, 1);
    // Grab B
    daAlinkHIO_setAnmGenMessage(&m.mGrabBAnm, ctx, "掴みＢ", 7.0f, 0);
    // Climb
    daAlinkHIO_setAnmGenMessage(&m.mClimbAnm, ctx, "よじ登り", 29.0f, 1);
    // Climb midway start F
    ctx->genSlider("よじ登り途中開始Ｆ", &m.mClimbStartFrame, 0.0f, 29.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump
    daAlinkHIO_setAnmGenMessage(&m.mJumpAnm, ctx, "ジャンプ", 13.0f, 0);
}

daAlinkHIO_wallCatch_c::daAlinkHIO_wallCatch_c() {
    static const char plabel[] = "const daAlinkHIO_wallCatch_c1 daAlinkHIO_wallCatch_c0::m = {\n";
    m_len = 84;
    mp_src_data = (void*)&daAlinkHIO_wallCatch_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 1;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wallFall_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // One hand grab
    daAlinkHIO_setAnmGenMessage(&m.mOneHandGrabAnm, ctx, "片手掴み", 22.0f, 0);
    // Two hand grab
    daAlinkHIO_setAnmGenMessage(&m.mTwoHandGrabAnm, ctx, "両手掴み", 12.0f, 0);
}
daAlinkHIO_wallFall_c::daAlinkHIO_wallFall_c() {
    static const char plabel[] = "const daAlinkHIO_wallFall_c1 daAlinkHIO_wallFall_c0::m = {\n";
    m_len = 40;
    mp_src_data = (void*)&daAlinkHIO_wallFall_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 0;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wallMove_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Minimum animation speed
    ctx->genSlider("最小Ａ速度", &m.mMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Maximum animation speed
    ctx->genSlider("最大Ａ速度", &m.mMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Interpolation
    ctx->genSlider("補間", &m.mInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Minimum speed
    ctx->genSlider("最小移動速度", &m.mMinSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Maximum speed
    ctx->genSlider("最大移動速度", &m.mMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wallMove_c::daAlinkHIO_wallMove_c() {
    static const char plabel[] = "const daAlinkHIO_wallMove_c1 daAlinkHIO_wallMove_c0::m = {\n";
    m_len = 20;
    mp_src_data = (void*)&daAlinkHIO_wallMove_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 5;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wallHang_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Small Jump
    ctx->genNode("小ジャンプ", &mSmallJump, 0, 0);
    // Wall Catch
    ctx->genNode("壁掴み", &mWallCatch, 0, 0);
    // Wall Fall
    ctx->genNode("落下掴み", &mWallFall, 0, 0);
    // Wall Move
    ctx->genNode("掴み移動", &mWallMove, 0, 0);
    // Auto walk height
    ctx->genSlider("自動歩き高さ", &m.auto_walk_height, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Small jump height
    ctx->genSlider("小ジャンプ高さ", &m.small_jump_height, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Climb height
    ctx->genSlider("よじ登り高さ", &m.climb_height, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump climb height
    ctx->genSlider("ジャンプ登り高さ", &m.jump_climb_height, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump hang height
    ctx->genSlider("ジャンプぶら下がり高さ", &m.jump_hang_height, 0.0f, 300.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Hang foot position height
    ctx->genSlider("ぶら下がり足位置高さ", &m.hang_foot_pos_height, 0.0f, 300.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Small jump key input time
    ctx->genSlider("小ジャンプキー入力時間", &m.small_jump_input_time, 0, 0x1e, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Grab key input time
    ctx->genSlider("掴みキー入力時間", &m.grab_input_time, 0, 0x1e, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wallHang_c::daAlinkHIO_wallHang_c()
    : mSmallJump(), mWallCatch(), mWallFall(), mWallMove() {
    static const char plabel[] = " const daAlinkHIO_wallHang_c1 daAlinkHIO_wallHang_c0::m = {\n";
    m_len = 28;
    mp_src_data = (void*)&daAlinkHIO_wallHang_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 6;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

daAlinkHIO_wallMove_c::~daAlinkHIO_wallMove_c() {}
daAlinkHIO_wallFall_c::~daAlinkHIO_wallFall_c() {}
daAlinkHIO_wallCatch_c::~daAlinkHIO_wallCatch_c() {}
daAlinkHIO_smallJump_c::~daAlinkHIO_smallJump_c() {}

void daAlinkHIO_pushpull_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Standby A speed
    ctx->genSlider("待機Ａ速度", &m.mStandbyASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Standby interpolation
    ctx->genSlider("待機補間", &m.mStandbyInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Push A speed
    ctx->genSlider("押しＡ速度", &m.mPushASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Push A speed (heavy)
    ctx->genSlider("押しＡ速度（重）", &m.mPushASpeedHeavy, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Push interpolation
    ctx->genSlider("押し補間", &m.mPushInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Pull interpolation
    ctx->genSlider("引き補間", &m.mPullInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sumo push A speed
    ctx->genSlider("相撲押しＡ速度", &m.mSumoPushASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sumo push additional A speed
    ctx->genSlider("相撲押し追加Ａ速度", &m.mSumoPushAddASpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Sumo pushed A speed
    ctx->genSlider("相撲押されＡ速度", &m.mSumoPushedASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sumo pushed additional A speed
    ctx->genSlider("相撲押され追加Ａ速度", &m.mSumoPushedAddASpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Sumo max gauge
    ctx->genSlider("相撲最大ゲージ", &m.mSumoMaxGauge, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sumo increase gauge
    ctx->genSlider("相撲増加ゲージ", &m.mSumoIncreaseGauge, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sumo penalty time
    ctx->genSlider("相撲ペナルティ時間", &m.mSumoPenaltyTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sumo power 3/2 switch
    ctx->genSlider("相撲パワー３・２境界", &m.mSumoPower3To2Switch, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Sumo power 2/1 switch
    ctx->genSlider("相撲パワー２・１境界", &m.mSumoPower2To1Switch, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Sumo rotation allow power
    ctx->genSlider("相撲旋回許可パワー", &m.mSumoRotationAllowPower, 0, 3, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sumo rotation angle
    ctx->genSlider("相撲旋回角度", &m.mSumoRotationAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sumo wait rotation allow
    ctx->genCheckBox("相撲待機旋回許可", &m.mEnableSumoWaitRotation, '\x01', 0, NULL, 0xffff,
                     0xffff, 0x200, 0x18);
}

daAlinkHIO_pushpull_c::daAlinkHIO_pushpull_c() {
    static const char plabel[] = "const daAlinkHIO_pushpull_c1 daAlinkHIO_pushpull_c0::m = {\n";
    m_len = 60;
    mp_src_data = (void*)&daAlinkHIO_pushpull_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 2;
    m_s16_num = 7;
    m_f32_num = 11;
    f14 = 1;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_damNormal_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Front
    daAlinkHIO_setAnmGenMessage(&m.mFrontAnm, ctx, "前方", 22.0f, 1);
    // Rear
    daAlinkHIO_setAnmGenMessage(&m.mRearAnm, ctx, "後方", 19.0f, 1);
    // Left
    daAlinkHIO_setAnmGenMessage(&m.mLeftAnm, ctx, "左方", 19.0f, 1);
    // Right
    daAlinkHIO_setAnmGenMessage(&m.mRightAnm, ctx, "右方", 19.0f, 1);
    // Front-back body angle maximum
    ctx->genSlider("前後体角度最大", &m.mFrontBackBodyMaxAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left-right body angle maximum
    ctx->genSlider("左右体角度最大", &m.mLeftRightBodyMaxAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Initial speed
    ctx->genSlider("初速", &m.mInitialSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack speed rate
    ctx->genSlider("攻撃速度率", &m.mAttackSpeedRate, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Ice damage A speed
    ctx->genSlider("氷ダメージＡ速度", &m.mIceDamageASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_damNormal_c::daAlinkHIO_damNormal_c() {
    static const char plabel[] = "const daAlinkHIO_damNormal_c1 daAlinkHIO_damNormal_c0::m = {\n";
    m_len = 100;
    mp_src_data = (void*)&daAlinkHIO_damNormal_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_damLaHu_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage interpolation
    ctx->genSlider("ダメージ補間", &m.mDamageBlend, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Horizontal speed
    ctx->genSlider("水平速度", &m.mHorizontalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Vertical speed
    ctx->genSlider("垂直速度", &m.mVerticalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Bounce speed
    ctx->genSlider("跳ね返り速度", &m.mBounceSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Body rotation speed
    ctx->genSlider("体回転速度", &m.mBodyRotateRate, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Front get up
    daAlinkHIO_setAnmGenMessage(&m.mFrontGetUpAnm, ctx, "前起", 49.0f, 1);
    // Back get up
    daAlinkHIO_setAnmGenMessage(&m.mBackGetUpAnm, ctx, "後起", 64.0f, 1);
    // Left get up
    daAlinkHIO_setAnmGenMessage(&m.mLeftGetUpAnm, ctx, "左起", 39.0f, 1);
    // Right get up
    daAlinkHIO_setAnmGenMessage(&m.mRightGetUpAnm, ctx, "右起", 39.0f, 1);
    // Front wall hit
    daAlinkHIO_setAnmGenMessage(&m.mFrontWallHitAnm, ctx, "前壁", 49.0f, 0);
    // Back wall hit
    daAlinkHIO_setAnmGenMessage(&m.mBackWallHitAnm, ctx, "後壁", 39.0f, 0);
    // Left wall hit
    daAlinkHIO_setAnmGenMessage(&m.mLeftWallHitAnm, ctx, "左壁", 39.0f, 0);
    // Right wall hit
    daAlinkHIO_setAnmGenMessage(&m.mRightWallHitAnm, ctx, "右壁", 39.0f, 0);
}

daAlinkHIO_damLaHu_c::daAlinkHIO_damLaHu_c(int param_0) {
    static void* baseP[2] = {
        (void*)&daAlinkHIO_damLarge_c0::m,
        (void*)&daAlinkHIO_damHuge_c0::m,
    };

    static const char* plabel[] = {
        "const daAlinkHIO_guardAttack_c1 daAlinkHIO_damLarge_c0::m = {\n",
        "const daAlinkHIO_guardAttack_c1 daAlinkHIO_damHuge_c0::m = {\n",
    };

    m_len = 188;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 8;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 6;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_damHorse_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // No direction
    daAlinkHIO_setAnmGenMessage(&m.mNoDirectionAnm, ctx, "方向なし", 9.0f, 0);
    // With direction
    daAlinkHIO_setAnmGenMessage(&m.mWithDirectionAnm, ctx, "方向あり", 11.0f, 0);
    // Front-back body angle maximum
    ctx->genSlider("前後体角度最大", &m.mFrontBackBodyMaxAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left-right body angle maximum
    ctx->genSlider("左右体角度最大", &m.mLeftRightBodyMaxAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_damHorse_c::daAlinkHIO_damHorse_c() {
    static const char plabel[] = "const daAlinkHIO_damHorse_c1 daAlinkHIO_damHorse_c0::m = {\n";
    m_len = 44;
    mp_src_data = (void*)&daAlinkHIO_damHorse_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 0;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_damFall_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Min safe landing height
    ctx->genSlider("受身可能最小高さ", &m.mMinRollHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max safe landing height
    ctx->genSlider("受身可能最大高さ", &m.mMaxRollHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Small damage height
    ctx->genSlider("小ダメージ高さ", &m.mSmallDmgHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Large damage height
    ctx->genSlider("大ダメージ高さ", &m.mBigDmgHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Fall animation transition height
    ctx->genSlider("空中アニメ移行高さ", &m.mFallAnmTransitionHeight, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Fall animation interpolation
    ctx->genSlider("空中アニメ補間", &m.mFallAnmMorf, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 11.0f, 0);
    // Small damage landing start F
    ctx->genSlider("小ダメージ着地開始Ｆ", &m.mSmallDmgLandStartFrame, 0.0f, 11.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Small stop time
    ctx->genSlider("小停止時間", &m.mSmallStopTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Large stop time
    ctx->genSlider("大停止時間", &m.mBigStopTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Aerial get up
    daAlinkHIO_setAnmGenMessage(&m.mStandAnm, ctx, "起上り", 17.0f, 1);
}

daAlinkHIO_damFall_c::daAlinkHIO_damFall_c() {
    static const char plabel[] = "const daAlinkHIO_damFall_c1 daAlinkHIO_damFall_c0::m = {\n";
    m_len = 72;
    mp_src_data = (void*)&daAlinkHIO_damFall_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 7;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_damCaught_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Grab duration
    ctx->genSlider("捕まり時間", &m.mGrabDuration, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Input fade time
    ctx->genSlider("入力減り時間", &m.mInputFadeTime, 0, 0x1e, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Standby add A speed
    ctx->genSlider("待機追加Ａ速度", &m.mStandbyAddASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Standby interpolation
    ctx->genSlider("待機補間", &m.mStandbyInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Escape
    daAlinkHIO_setAnmGenMessage(&m.mEscapeAnm, ctx, "脱出", 59.0f, 1);
}

daAlinkHIO_damCaught_c::daAlinkHIO_damCaught_c() {
    static const char plabel[] = "const daAlinkHIO_damCaught_c1 daAlinkHIO_damCaught_c0::m = {\n";
    m_len = 36;
    mp_src_data = (void*)&daAlinkHIO_damCaught_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_damSwim_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Front
    daAlinkHIO_setAnmGenMessage(&m.mFrontAnm, ctx, "前", 22.0f, 1);
    // Rear
    daAlinkHIO_setAnmGenMessage(&m.mRearAnm, ctx, "後", 19.0f, 1);
    // Left
    daAlinkHIO_setAnmGenMessage(&m.mLeftAnm, ctx, "左", 19.0f, 1);
    // Right
    daAlinkHIO_setAnmGenMessage(&m.mRightAnm, ctx, "右", 19.0f, 1);
    // Front-back body angle maximum
    ctx->genSlider("前後体角度最大", &m.mFrontBackBodyMaxAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left-right body angle maximum
    ctx->genSlider("左右体角度最大", &m.mLeftRightBodyMaxAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Initial speed
    ctx->genSlider("初速", &m.mInitialSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack speed rate
    ctx->genSlider("攻撃速度率", &m.mAttackSpeedRate, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max speed
    ctx->genSlider("最高速", &m.mMaxSpeed, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Sink
    daAlinkHIO_setAnmGenMessage(&m.mSinkAnm, ctx, "下沈", 10.0f, 0);
    // Water surface
    daAlinkHIO_setAnmGenMessage(&m.mSurfaceAnm, ctx, "水面", 11.0f, 1);
}

daAlinkHIO_damSwim_c::daAlinkHIO_damSwim_c() {
    static const char plabel[] = "const daAlinkHIO_damSwim_c1 daAlinkHIO_damSwim_c0::m = {\n";
    m_len = 140;
    mp_src_data = (void*)&daAlinkHIO_damSwim_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 6;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_damage_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Normal
    ctx->genNode("通常", &mDamNormal, 0, 0);
    // Large
    ctx->genNode("大", &mDamLarge, 0, 0);
    // Heavy large
    ctx->genNode("強大", &mDamHuge, 0, 0);
    // On horse
    ctx->genNode("馬上", &mDamHorse, 0, 0);
    // Fall
    ctx->genNode("落下", &mDamFall, 0, 0);
    // Captured
    ctx->genNode("捕縛", &mDamCaught, 0, 0);
    // Swim
    ctx->genNode("泳ぎ", &mDamSwim, 0, 0);
    // Invincible state
    ctx->genCheckBox("無敵状態", &m.mInvincible, '\x01', 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Invincible time
    ctx->genSlider("無敵時間", &m.mInvincibleTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Wolf floor invincible time
    ctx->genSlider("狼ポリゴン時無敵時間", &m.mWolfFloorInvincibleTime, 0, 0x96, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Floor get up
    daAlinkHIO_setAnmGenMessage(&m.mFloorDmgAnm, ctx, "床起き", 14.0f, 1);
    // Dash
    daAlinkHIO_setAnmGenMessage(&m.mDashDmgAnm, ctx, "ダッシュ", 9.0f, 0);
    // Recover stand anm speed
    ctx->genSlider("復帰起き上がりＡ速度", &m.mRecoverStandAnmSpeed, 1.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Inverted fall interpolation
    ctx->genSlider("逆さま落下補間", &m.mInvertedFallInterpolation, 0.0f, 50.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Freeze wait time
    ctx->genSlider("凍り待ち時間", &m.mFreezeTime, 1, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Freeze initial R
    ctx->genSlider("凍り初期Ｒ", &m.mFreezeInitR, -0x40, 0x40, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Freeze initial G
    ctx->genSlider("凍り初期Ｇ", &m.mFreezeInitG, -0x40, 0x40, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Freeze initial B
    ctx->genSlider("凍り初期Ｂ", &m.mFreezeInitB, -0x40, 0x40, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Freeze middle R
    ctx->genSlider("凍り中Ｒ", &m.mFreezeR, -0xff, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Freeze middle G
    ctx->genSlider("凍り中Ｇ", &m.mFreezeG, -0xff, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Freeze middle B
    ctx->genSlider("凍り中Ｂ", &m.mFreezeB, -0xff, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage R0
    ctx->genSlider("ダメージＲ０", &m.mDamageR0, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage G0
    ctx->genSlider("ダメージＧ０", &m.mDamageG0, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage B0
    ctx->genSlider("ダメージＢ０", &m.mDamageB0, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage R1
    ctx->genSlider("ダメージＲ１", &m.mDamageR1, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage G1
    ctx->genSlider("ダメージＧ１", &m.mDamageG1, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage B1
    ctx->genSlider("ダメージＢ１", &m.mDamageB1, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage R2
    ctx->genSlider("ダメージＲ２", &m.mDamageR2, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage G2
    ctx->genSlider("ダメージＧ２", &m.mDamageG2, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage B2
    ctx->genSlider("ダメージＢ２", &m.mDamageB2, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_damage_c::daAlinkHIO_damage_c()
    : mDamNormal(), mDamLarge(0), mDamHuge(1), mDamHorse(), mDamFall(), mDamCaught(), mDamSwim() {
    static const char plabel[] = "const daAlinkHIO_damage_c1 daAlinkHIO_damage_c0::m = {\n";
    m_len = 88;
    mp_src_data = (void*)&daAlinkHIO_damage_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 2;
    m_s16_num = 19;
    m_f32_num = 2;
    f14 = 1;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

daAlinkHIO_damSwim_c::~daAlinkHIO_damSwim_c() {}
daAlinkHIO_damCaught_c::~daAlinkHIO_damCaught_c() {}
daAlinkHIO_damFall_c::~daAlinkHIO_damFall_c() {}
daAlinkHIO_damHorse_c::~daAlinkHIO_damHorse_c() {}
daAlinkHIO_damLaHu_c::~daAlinkHIO_damLaHu_c() {}
daAlinkHIO_damNormal_c::~daAlinkHIO_damNormal_c() {}

void daAlinkHIO_horse_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Sword up Animation speed
    ctx->genSlider("剣上Ａ速度", &m.mSwordUpAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sword up interpolation
    ctx->genSlider("剣上補間", &m.mSwordUpInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sword up time
    ctx->genSlider("剣上時間", &m.mSwordUpTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Search range angle
    ctx->genSlider("サーチ範囲角度", &m.mSearchRangeAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Walk out prohibition time
    ctx->genSlider("歩出禁止時間", &m.mWalkOutProhibitionTime, 0, 500, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Walk out
    daAlinkHIO_setAnmGenMessage(&m.mWalkOutAnm, ctx, "歩出", 18.0f, 0);
    // Run out
    daAlinkHIO_setAnmGenMessage(&m.mRunOutAnm, ctx, "走出", 22.0f, 0);
    // Horse walk start F
    ctx->genSlider("馬歩出開始Ｆ", &m.mHorseWalkStartFrame, 0.0f, 18.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Whip(ctx,
    daAlinkHIO_setAnmGenMessage(&m.mWhipAnm, ctx, "鞭", 43.0f, 0);
    // Whip run out
    daAlinkHIO_setAnmGenMessage(&m.mWhipRunOutAnm, ctx, "鞭走出", 22.0f, 0);
    // Tired wait interpolation
    ctx->genSlider("疲れ待機補間", &m.mTiredWaitInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Whip wait time
    ctx->genSlider("鞭打ち待ち時間", &m.mWhipWaitTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Subjective downward max angle
    ctx->genSlider("主観下向最大角度", &m.mSubjectiveDownwardMaxAngle, 0, 0x4000, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Subjective upward max angle
    ctx->genSlider("主観上向最大角度", &m.mSubjectiveUpwardMaxAngle, -0x4000, 0, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_horse_c::daAlinkHIO_horse_c() {
    static const char plabel[] = "const daAlinkHIO_horse_c1 daAlinkHIO_horse_c0::m = {\n";
    m_len = 108;
    mp_src_data = (void*)&daAlinkHIO_horse_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 6;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_canoe_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Stick rowing animation speed
    ctx->genSlider("スティック漕ぎＡ速度", &m.mStickRowAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Button rowing animation speed
    ctx->genSlider("ボタン漕ぎＡ速度", &m.mBtnRowAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Stick rowing start animation speed
    ctx->genSlider("スティック漕ぎ始めＡ速度", &m.mStickRowStartAnmSpeed, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Button rowing start animation speed
    ctx->genSlider("ボタン漕ぎ始めＡ速度", &m.mBtnRowStartAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Canoe speed rate
    ctx->genSlider("カヌー速度率", &m.mSpeedRate, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Canoe maximum speed
    ctx->genSlider("カヌー最高速度", &m.mMaxSpeed, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Canoe backward speed rate
    ctx->genSlider("カヌー後退速度率", &m.mBackSpeedRate, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Canoe backward maximum speed
    ctx->genSlider("カヌー後退最高速度", &m.mBackMaxSpeed, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Canoe deceleration value
    ctx->genSlider("カヌー減速値", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Canoe maximum turning angle
    ctx->genSlider("カヌー最高旋回角度", &m.mMaxTurnAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Left-right grip change
    daAlinkHIO_setAnmGenMessage(&m.mLeftRightChangeAnm, ctx, "左右持替", 23.0f, 0);
    // Downriver canoe maximum turning angle
    ctx->genSlider("川下りカヌー最高旋回角度", &m.mMaxTurnAngle_RiverRide, 0, 0x7fff, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Downriver canoe maximum speed
    ctx->genSlider("川下りカヌー最高速度", &m.mMaxSpeed_RiverRide, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Downriver canoe speed rate
    ctx->genSlider("川下りカヌー速度率", &m.mSpeedRate_RiverRide, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Downriver canoe deceleration value
    ctx->genSlider("川下りカヌー減速値", &m.mDeceleration_RiverRide, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_canoe_c::daAlinkHIO_canoe_c() {
    static const char plabel[] = "const daAlinkHIO_canoe_c1 daAlinkHIO_canoe_c0::m = {\n";
    m_len = 72;
    mp_src_data = (void*)&daAlinkHIO_canoe_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 12;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_bow_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Shoot
    daAlinkHIO_setAnmGenMessage(&m.mShootAnm, ctx, "撃つ", 9.0f, 1);
    // Load
    daAlinkHIO_setAnmGenMessage(&m.mLoadAnm, ctx, "装填", 6.0f, 0);
    // Start interpolation
    ctx->genSlider("開始補間", &m.mStartInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Arrow speed
    ctx->genSlider("矢速度", &m.mArrowSpeed, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Arrow distance
    ctx->genSlider("矢飛距離", &m.mArrowDistance, 0.0f, 100000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Charge time
    ctx->genSlider("ため時間", &m.mChargeArrowTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Charge arrow speed
    ctx->genSlider("ため矢速度", &m.mChargeArrowSpeed, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Charge arrow distance
    ctx->genSlider("ため矢飛距離", &m.mChargeArrowDistance, 0.0f, 100000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Scope arrow speed
    ctx->genSlider("スコープ矢速度", &m.mScopeArrowSpeed, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Scope arrow distance
    ctx->genSlider("スコープ矢飛距離", &m.mScopeArrowDistance, 0.0f, 100000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Arrow additional attack radius
    ctx->genSlider("矢追加攻撃半径", &m.mArrowAttackRadius, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Arrow additional attack radius start
    ctx->genSlider("矢追加攻撃半径開始", &m.mArrowIncAttackMaxStart, 0.0f, 100000.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Arrow additional attack radius max
    ctx->genSlider("矢追加攻撃半径最大", &m.mArrowIncAttackMax, 0.0f, 100000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Bomb arrow speed
    ctx->genSlider("爆弾矢速度", &m.mBombArrowSpeed, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bomb arrow distance
    ctx->genSlider("爆弾矢飛距離", &m.mBombArrowDistance, 0.0f, 100000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Charge bomb arrow speed
    ctx->genSlider("ため爆弾矢速度", &m.mChargeBombArrowSpeed, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Charge bomb arrow distance
    ctx->genSlider("ため爆弾矢飛距離", &m.mChargeBombArrowDistance, 0.0f, 100000.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Arrow wait time
    ctx->genSlider("矢待ち時間", &m.mBombArrowHoldTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bomb arrow fly explode time
    ctx->genSlider("爆弾矢飛行爆発時間", &m.mBombArrowFlyExplodeTime, 0, 1000, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Slingshot speed
    ctx->genSlider("パチンコ速度", &m.mSlingshotSpeed, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Slingshot distance
    ctx->genSlider("パチンコ飛距離", &m.mSlingshotDistance, 0.0f, 100000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_bow_c::daAlinkHIO_bow_c() {
    static const char plabel[] = "const daAlinkHIO_bow_c1 daAlinkHIO_bow_c0::m = {\n";
    m_len = 112;
    mp_src_data = (void*)&daAlinkHIO_bow_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 16;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_boom_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Throw
    daAlinkHIO_setAnmGenMessage(&m.mThrowAnm, ctx, "投げ", 21.0f, 1);
    // Catch
    daAlinkHIO_setAnmGenMessage(&m.mCatchAnm, ctx, "キャッチ", 25.0f, 1);
    // Idle animation speed
    ctx->genSlider("待機Ａ速度", &m.mIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Start interpolation
    ctx->genSlider("開始補間", &m.mStartInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Fly speed
    ctx->genSlider("飛行速度", &m.mFlySpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Charge fly speed
    ctx->genSlider("ため飛行速度", &m.mChargeFlySpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Entangling fly speed
    ctx->genSlider("巻込飛行速度", &m.mCatchSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Fly distance max
    ctx->genSlider("飛行距離", &m.mFlyDistMax, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Horseback fly distance max
    ctx->genSlider("馬上飛行距離", &m.mHorsebackFlyDistMax, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Charge time
    ctx->genSlider("ため時間", &m.mChargeTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Lock distance max
    ctx->genSlider("ロック距離", &m.mLockDistMax, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Wall piercing time
    ctx->genSlider("壁突き抜け時間", &m.mBgThroughTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_boom_c::daAlinkHIO_boom_c() {
    static const char plabel[] = "const daAlinkHIO_boom_c1 daAlinkHIO_boom_c0::m = {\n";
    m_len = 76;
    mp_src_data = (void*)&daAlinkHIO_boom_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 8;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_bomb_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Explode time
    ctx->genSlider("爆発時間", &m.mExplodeTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -5.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max fall speed
    ctx->genSlider("最大落下速度", &m.mMaxFallSpeed, -500.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Throw horizontal speed
    ctx->genSlider("投げ水平速度", &m.mThrowSpeedH, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Throw vertical speed
    ctx->genSlider("投げ垂直速度", &m.mThrowSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bound rate
    ctx->genSlider("バウンド率", &m.mBoundRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Vertical stop speed
    ctx->genSlider("垂直停止速度", &m.mStopSpeedY, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bounce time max speed
    ctx->genSlider("跳返時最大速度", &m.mMaxSpeedY, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Effect scale
    ctx->genSlider("爆発エフェクトスケール", &m.mEffScale, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater explode switch
    ctx->genSlider("水中爆発切替え", &m.mExplodeWaterEffectLimit, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Explode attack radius
    ctx->genSlider("爆発攻撃半径", &m.mAttackRadius, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bomb insect red blink
    ctx->genSlider("爆弾虫赤点滅", &m.mEnemyBombColorR, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bomb insect effect track rate
    ctx->genSlider("爆弾虫エフェクト追従率", &m.mPokeBombTrackRate, 0.0f, 5.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Water gravity
    ctx->genSlider("水中重力", &m.mWaterGravity, -5.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Water max fall speed
    ctx->genSlider("水中最大落下速度", &m.mWaterMaxFallSpeed, -500.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Water throw horizontal speed
    ctx->genSlider("水中投げ水平速度", &m.mWaterThrowSpeedH, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Water throw vertical speed
    ctx->genSlider("水中投げ垂直速度", &m.mWaterThrowSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wolf throw horizontal speed
    ctx->genSlider("狼投げ水平速度", &m.mWolfThrowSpeedH, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wolf throw vertical speed
    ctx->genSlider("狼投げ垂直速度", &m.mWolfThrowSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Poke insect limit angle
    ctx->genSlider("ポケムシ制限角度", &m.mBombInsectLimitAngle, 0, 0x5a, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_bomb_c::daAlinkHIO_bomb_c() {
    static const char plabel[] = "const daAlinkHIO_bomb_c1 daAlinkHIO_bomb_c0::m = {\n";
    m_len = 76;
    mp_src_data = (void*)&daAlinkHIO_bomb_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 17;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_light_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // X angle
    ctx->genSlider("Ｘ角度", &m.mXAngle, -0xb4, 0xb4, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Power
    ctx->genSlider("パワー", &m.mPower, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Width
    ctx->genSlider("幅", &m.mWidth, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Y offset
    ctx->genSlider("Ｙずらし", &m.mYOffset, -10000.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Z offset
    ctx->genSlider("Ｚずらし", &m.mZOffset, -10000.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Color R
    ctx->genSlider("色Ｒ", &m.mColorR, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color G
    ctx->genSlider("色Ｇ", &m.mColorG, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color B
    ctx->genSlider("色Ｂ", &m.mColorB, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Distance attenuation
    ctx->startComboBox("距離減衰", &m.mDistanceAttenuationType, 0, NULL, 0xffff, 0xffff, 0x100,
                       0x1a);
    // No attenuation [GX_DA_OFF]
    ctx->genComboBoxItem("減衰なし [GX_DA_OFF]", 0);
    // Gentle [GX_DA_GENTLE]
    ctx->genComboBoxItem("おだやか [GX_DA_GENTLE]", 1);
    // Medium [GX_DA_MEDIUM]
    ctx->genComboBoxItem("中間     [GX_DA_MEDIUM]", 2);
    // Steep [GX_DA_STEEP]
    ctx->genComboBoxItem("険しい   [GX_DA_STEEP]", 3);
    ctx->endComboBox();
    // Angle attenuation
    ctx->startComboBox("角度減衰", &m.mAngleAttenuationType, 0, NULL, 0xffff, 0xffff, 0x100, 0x1a);
    // Point [GX_SP_OFF]
    ctx->genComboBoxItem("ポイント [GX_SP_OFF]", 0);
    // Right angle [GX_SP_FLAT]
    ctx->genComboBoxItem("直角     [GX_SP_FLAT]", 1);
    // COS curve [GX_SP_COS]
    ctx->genComboBoxItem("COS曲線  [GX_SP_COS]", 2);
    // COS2 curve [GX_SP_COS2]
    ctx->genComboBoxItem("COS2曲線 [GX_SP_COS2]", 3);
    // Sharp [GX_SP_SHARP]
    ctx->genComboBoxItem("シャープ [GX_SP_SHARP]", 4);
    // Ring shape [GX_SP_RING1]
    ctx->genComboBoxItem("リング状 [GX_SP_RING1]", 5);
    // Ring shape 2 [GX_SP_RING2]
    ctx->genComboBoxItem("リング状2[GX_SP_RING2]", 6);
    ctx->endComboBox();
}

daAlinkHIO_light_c::daAlinkHIO_light_c(int param_0) {
    static void* baseP[] = {
        (void*)&daAlinkHIO_huLight_c0::m,
        (void*)&daAlinkHIO_wlLight_c0::m,
        (void*)&daAlinkHIO_zwLight_c0::m,

    };

    static const char* plabel[] = {
        "const daAlinkHIO_light_c1 daAlinkHIO_huLight_c0::m = {\n",
        "const daAlinkHIO_light_c1 daAlinkHIO_wlLight_c0::m = {\n",
        "const daAlinkHIO_light_c1 daAlinkHIO_zwLight_c0::m = {\n",
    };

    m_len = 28;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 4;
    m_s16_num = 4;
    m_f32_num = 4;
    f14 = 0;
    f18 = 2;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_kandelaar_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Shake
    daAlinkHIO_setAnmGenMessage(&m.mShakeAnm, ctx, "振る", 30.0f, 1);
    // Color Reg 1 R
    ctx->genSlider("カラレジ１Ｒ", &m.mColorReg1R, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color Reg 1 G
    ctx->genSlider("カラレジ１Ｇ", &m.mColorReg1G, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color Reg 1 B
    ctx->genSlider("カラレジ１Ｂ", &m.mColorReg1B, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color Reg 2 R
    ctx->genSlider("カラレジ２Ｒ", &m.mColorReg2R, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color Reg 2 G
    ctx->genSlider("カラレジ２Ｇ", &m.mColorReg2G, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Color Reg 2 B
    ctx->genSlider("カラレジ２Ｂ", &m.mColorReg2B, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Flame Track Rate
    ctx->genSlider("炎追従率", &m.mFlameTrackRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Pour Begin
    daAlinkHIO_setAnmGenMessage(&m.mBeginUnkAnm, ctx, "注始", 11.0f, 0);
    // Pour End
    daAlinkHIO_setAnmGenMessage(&m.mEndUnkAnm, ctx, "注終", 17.0f, 0);
    // Normal Oil Loss
    ctx->genSlider("通常油減り", &m.mNormalOilLoss, 0, 30000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Shake Oil Loss
    ctx->genSlider("振り油減り", &m.mShakeOilLoss, 0, 30000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_kandelaar_c::daAlinkHIO_kandelaar_c() {
    static const char plabel[] = "const daAlinkHIO_kandelaar_c1 daAlinkHIO_kandelaar_c0::m = {\n";
    m_len = 80;
    mp_src_data = (void*)&daAlinkHIO_kandelaar_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 8;
    m_f32_num = 1;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_magneBoots_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Equip
    daAlinkHIO_setAnmGenMessage(&m.mEquipAnm, ctx, "装備", 29.0f, 1);
    // Key input rate
    ctx->genSlider("キー入力率", &m.mInputFactor, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Foot position ratio
    ctx->genSlider("足位置比率", &m.mFeetPositionRatio, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk A speed max
    ctx->genSlider("歩きＡ速度最大", &m.mWalkAnmSpeedMax, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Walk A speed min
    ctx->genSlider("歩きＡ速度最小", &m.mWalkAnmSpeedMin, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater key input rate
    ctx->genSlider("水中キー入力率", &m.mWaterInputFactor, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater A speed rate
    ctx->genSlider("水中Ａ速度率", &m.mWaterStartWalkAnmRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater A speed rate (slow)
    ctx->genSlider("水中Ａ速度率（遅）", &m.mWaterWalkAnmRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater A speed rate (sword)
    ctx->genSlider("水中Ａ速度率（剣）", &m.mWaterVelRateSword, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max fly speed
    ctx->genSlider("最大飛び速度", &m.mMaxMagneFlySpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Fly acceleration
    ctx->genSlider("飛び加速度", &m.mMagneFlyAccelRate, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater vertical speed rate
    ctx->genSlider("水中垂直速度率", &m.mWaterVelocityY, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Underwater horizontal speed rate
    ctx->genSlider("水中平面速度率", &m.mWaterVelocityX, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Underwater key input rate (Zora)
    ctx->genSlider("水中キー入力率（ゾーラ）", &m.mZoraWaterInputFactor, 0.0f, 1.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Underwater A speed rate (Zora)
    ctx->genSlider("水中Ａ速度率（ゾーラ）", &m.mZoraWaterAnmSpeed, 0.0f, 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_magneBoots_c::daAlinkHIO_magneBoots_c() {
    static const char plabel[] = "const daAlinkHIO_magneBoots_c1 daAlinkHIO_magneBoots_c0::m = {\n";
    m_len = 76;
    mp_src_data = (void*)&daAlinkHIO_magneBoots_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 14;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_fmChain_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Grip
    daAlinkHIO_setAnmGenMessage(&m.mGripAnm, ctx, "掴み", 20.0f, 1);
    // Unfinished
    daAlinkHIO_setReverseAnmGenMessage(&m.mUnfinishedAnm, ctx, "放し", 20.0f, 1);
}

daAlinkHIO_fmChain_c::daAlinkHIO_fmChain_c() {
    static const char plabel[] = "const daAlinkHIO_fmChain_c1 daAlinkHIO_fmChain_c0::m = {\n";
    m_len = 40;
    mp_src_data = (void*)&daAlinkHIO_fmChain_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 0;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_hookshot_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle A speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Start interpolation
    ctx->genSlider("開始補間", &m.mStartInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Maximum length
    ctx->genSlider("最大長さ", &m.mMaxLength, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Launch speed
    ctx->genSlider("発射速度", &m.mShootSpeed, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Return speed
    ctx->genSlider("戻り速度", &m.mReturnSpeed, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Pierce return speed
    ctx->genSlider("刺戻り速度", &m.mStickReturnSpeed, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Shoot
    daAlinkHIO_setAnmGenMessage(&m.mShootAnm, ctx, "撃つ", 12.0f, 0);
    // Throw wait
    daAlinkHIO_setAnmGenMessage(&m.mRoofHangAnm, ctx, "天井停止", 39.0f, 1);
    // Ceiling ascent speed
    ctx->genSlider("天井上昇速度", &m.mRoofHangRiseSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Ceiling descent speed
    ctx->genSlider("天井降下速度", &m.mRoofHangDecendSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wall wait
    daAlinkHIO_setAnmGenMessage(&m.mWallHangAnm, ctx, "壁停止", 28.0f, 1);
    // Forced pierce
    ctx->genCheckBox("強制刺さり", &m.mForceStick, '\x01', 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Boss battle maximum length
    ctx->genSlider("ボス戦最大長さ", &m.mBossMaxLength, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boss battle launch speed
    ctx->genSlider("ボス戦発射速度", &m.mBossShootSpeed, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boss battle return speed
    ctx->genSlider("ボス戦戻り速度", &m.mBossReturnSpeed, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boss battle pierce return speed
    ctx->genSlider("ボス戦刺戻り速度", &m.mBossStickReturnSpeed, 0.0f, 500.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_hookshot_c::daAlinkHIO_hookshot_c() {
    static const char plabel[] = "const daAlinkHIO_hookshot_c1 daAlinkHIO_hookshot_c0::m = {\n";
    m_len = 112;
    mp_src_data = (void*)&daAlinkHIO_hookshot_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 4;
    m_s16_num = 0;
    m_f32_num = 12;
    f14 = 1;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_spinner_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max fall speed
    ctx->genSlider("最大落下速度", &m.mMaxFallSpeed, -100.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump rate
    ctx->genSlider("ジャンプ比率", &m.mJumpRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Board wait time
    ctx->genSlider("乗込待ち時間", &m.mBoardWaitTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Ride move time
    ctx->genSlider("乗り移動時間", &m.mRideMoveTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Ride speed
    ctx->genSlider("乗り速度", &m.mRideSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Ride decel max
    ctx->genSlider("乗り減速最大", &m.mDecelSpeedMax, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Ride decel min
    ctx->genSlider("乗り減速最小", &m.mDecelSpeedMin, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Ride decel rate
    ctx->genSlider("乗り減速率", &m.mDecelRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Ride rot max
    ctx->genSlider("乗り旋回最大", &m.mRideRotAngleMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Ride rot min
    ctx->genSlider("乗り旋回最小", &m.mRideRotAngleMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Kick
    daAlinkHIO_setAnmGenMessage(&m.mKickAnm, ctx, "蹴り", 21.0f, 1);
    // Boss ride move time
    ctx->genSlider("ボス乗り移動時間", &m.mBossRideMoveTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boss ride speed
    ctx->genSlider("ボス乗り速度", &m.mBossRideSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_spinner_c::daAlinkHIO_spinner_c() {
    static const char plabel[] = "const daAlinkHIO_spinner_c1 daAlinkHIO_spinner_c0::m = {\n";
    m_len = 64;
    mp_src_data = (void*)&daAlinkHIO_spinner_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 6;
    m_f32_num = 8;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_ironBall_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle A speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Foot position ratio
    ctx->genSlider("足位置比率", &m.mFeetPosRatio, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk A speed max
    ctx->genSlider("歩きＡ速度最大", &m.mWalkAnmSpeedMax, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Walk A speed min
    ctx->genSlider("歩きＡ速度最小", &m.mWalkAnmSpeedMin, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Prepare A speed
    ctx->genSlider("準備Ａ速度", &m.mPrepareAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Prepare interpolation
    ctx->genSlider("準備補間", &m.mPrepareInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Swing A speed
    ctx->genSlider("回しＡ速度", &m.mTurnAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Swing interpolation
    ctx->genSlider("回し補間", &m.mTurnInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Pre-throw A speed
    ctx->genSlider("投げ前Ａ速度", &m.mPreThrowAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Pre-throw interpolation
    ctx->genSlider("投げ前補間", &m.mPreThrowAnmInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Throw A speed
    ctx->genSlider("投げＡ速度", &m.mThrowAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Throw interpolation
    ctx->genSlider("投げ補間", &m.mThrowInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Pull A speed
    ctx->genSlider("引きＡ速度", &m.mPullAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Pull interpolation
    ctx->genSlider("引き補間", &m.mPullInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Catch
    daAlinkHIO_setAnmGenMessage(&m.mCatchAnm, ctx, "キャッチ", 19.0f, 1);
    // Ball gravity
    ctx->genSlider("球重力", &m.mBallGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Ball max fall speed
    ctx->genSlider("球最大落下速度", &m.mBallMaxFallSpeed, -300.0f, 0.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Throw ball gravity
    ctx->genSlider("投げ球重力", &m.mThrowBallGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Throw ball speed Z
    ctx->genSlider("投げ球速度Ｚ", &m.mThrowSpeedZ, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Throw ball speed Y
    ctx->genSlider("投げ球速度Ｙ", &m.mThrowSpeedY, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Return ball gravity
    ctx->genSlider("戻り球重力", &m.mReturnBallGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Chain speed rate
    ctx->genSlider("鎖速度率", &m.mChainSpeedRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Chain gravity
    ctx->genSlider("鎖重力", &m.mChainGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Throw chain gravity
    ctx->genSlider("投げ鎖重力", &m.mThrowChainGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Throw chain accel Z
    ctx->genSlider("投げ鎖付加速度Ｚ", &m.mThrowChainAccelZ, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Throw chain accel Y
    ctx->genSlider("投げ鎖付加速度Ｙ", &m.mThrowChainAccelY, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Throw chain add num
    ctx->genSlider("投げ鎖追加数", &m.mThrowChainNum, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Return chain accel Y
    ctx->genSlider("戻り鎖付加速度Ｙ", &m.mReturnChainAccelY, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Return chain add rate
    ctx->genSlider("戻り鎖付加率", &m.mReturnChainRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Return chain reduce num
    ctx->genSlider("戻り鎖減少数", &m.mReturnChainRemoveNum, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_ironBall_c::daAlinkHIO_ironBall_c() {
    static const char plabel[] = "const daAlinkHIO_ironBall_c1 daAlinkHIO_ironBall_c0::m = {\n";
    m_len = 140;
    mp_src_data = (void*)&daAlinkHIO_ironBall_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 29;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_copyRod_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Swing
    daAlinkHIO_setAnmGenMessage(&m.mSwingAnm, ctx, "振り", 19.0f, 1);
    // Swing end
    daAlinkHIO_setAnmGenMessage(&m.mBigSwingAnm, ctx, "振り大", 39.0f, 1);
    // Ball speed
    ctx->genSlider("ボール速度", &m.mBallSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Ball return speed
    ctx->genSlider("ボール戻速度", &m.mBallReturnSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Ball fly distance
    ctx->genSlider("ボール飛距離", &m.mBallMaxDistance, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Boss battle ball fly distance
    ctx->genSlider("ボス戦ボール飛距離", &m.mBossBallMaxDistance, 0.0f, 10000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_copyRod_c::daAlinkHIO_copyRod_c() {
    static const char plabel[] = "const daAlinkHIO_copyRod_c1 daAlinkHIO_copyRod_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_copyRod_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_pickUp_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Grab
    daAlinkHIO_setAnmGenMessage(&m.mGrabAnm, ctx, "掴み", 18.0f, 1);
    // Place
    daAlinkHIO_setReverseAnmGenMessage(&m.mPlaceAnm, ctx, "置く", 18.0f, 1);
}

daAlinkHIO_pickUp_c::daAlinkHIO_pickUp_c() {
    static const char plabel[] = "const daAlinkHIO_pickUp_c1 daAlinkHIO_pickUp_c0::m = {\n";
    m_len = 48;
    mp_src_data = (void*)&daAlinkHIO_pickUp_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 2;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_board_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最大速度", &m.mMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Paddling acceleration
    ctx->genSlider("漕ぎ加速", &m.mPushAccel, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Paddling max speed
    ctx->genSlider("漕ぎ最大速度", &m.mMaxPushSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Start min speed
    ctx->genSlider("開始最低速度", &m.mStartMinSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Start add speed
    ctx->genSlider("開始追加速度", &m.mStartAddSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Min jump Y speed
    ctx->genSlider("最低ジャンプＹ速度", &m.mMinJumpSpeedY, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max jump Y speed
    ctx->genSlider("最高ジャンプＹ速度", &m.mMaxJumpSpeedY, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Stand A speed
    ctx->genSlider("立ちＡ速度", &m.mStandAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Stand interpolation
    ctx->genSlider("立ち補間", &m.mStandInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sit A speed
    ctx->genSlider("座りＡ速度", &m.mSitAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sit interpolation
    ctx->genSlider("座り補間", &m.mSitInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Paddling max A speed
    ctx->genSlider("漕ぎ最高Ａ速度", &m.mPushAnmMaxSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Paddling min A speed
    ctx->genSlider("漕ぎ最小Ａ速度", &m.mPushAnmMinSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Paddling interpolation
    ctx->genSlider("漕ぎ補間", &m.mPushInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Fast paddling switch speed
    ctx->genSlider("早漕ぎ切替速度", &m.mFastPushSwitchSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Fast paddling interpolation
    ctx->genSlider("早漕ぎ補間", &m.mFastPushInterpolation, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Fly out / Jump
    daAlinkHIO_setAnmGenMessage(&m.mJumpAnm, ctx, "飛出", 9.0f, 0);
    // Airborne
    daAlinkHIO_setAnmGenMessage(&m.mAirborneAnm, ctx, "滞空", 14.0f, 0);
    // Land
    daAlinkHIO_setAnmGenMessage(&m.mLandAnm, ctx, "着地", 24.0f, 0);
    // Rotation slash
    daAlinkHIO_setAnmGenMessage(&m.mSpinAnm, ctx, "回転斬り", 24.0f, 0);
    // Rotation slash min jump Y speed
    ctx->genSlider("回転斬最低ジャンプＹ速度", &m.mSpinMinJumpSpeedY, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Rotation slash max jump Y speed
    ctx->genSlider("回転斬最高ジャンプＹ速度", &m.mSpinMaxJumpSpeedY, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Dedicated effect max speed
    ctx->genSlider("専用エフェクト最大速度", &m.mEffectMaxSpeed, 0.1f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_board_c::daAlinkHIO_board_c() {
    static const char plabel[] = "const daAlinkHIO_board_c1 daAlinkHIO_board_c0::m = {\n";
    m_len = 156;
    mp_src_data = (void*)&daAlinkHIO_board_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 19;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_bottle_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, (JOREventListener*)0x0, 0xffff, 0xffff, 0x200, 0x18);
    // Start drink
    daAlinkHIO_setAnmGenMessage(&m.mStartDrinkAnm, ctx, "飲む始め", 56.0f, 0);
    // End drink
    daAlinkHIO_setAnmGenMessage(&m.mEndDrinkAnm, ctx, "飲み終", 39.0f, 0);
    // Milk end drink
    daAlinkHIO_setAnmGenMessage(&m.mEndDrinkAnm, ctx, "ミルク飲み終", 86.0f, 0);
    // Open
    daAlinkHIO_setAnmGenMessage(&m.mOpenBottleAnm, ctx, "開ける", 49.0f, 0);
    // Scoop
    daAlinkHIO_setAnmGenMessage(&m.mScoopAnm, ctx, "すくう", 54.0f, 0);
    // Swing down
    daAlinkHIO_setAnmGenMessage(&m.mSwingDownAnm, ctx, "下振り", 24.0f, 1);
    // Swing side
    daAlinkHIO_setAnmGenMessage(&m.mSwingSideAnm, ctx, "横振り", 19.0f, 1);
    // Medicine
    daAlinkHIO_setAnmGenMessage(&m.mDrinkNastyAnm, ctx, "くすり", 29.0f, 0);
    // Get
    daAlinkHIO_setAnmGenMessage(&m.mGetAnm, ctx, "ゲット", 47.0f, 0);
}

daAlinkHIO_bottle_c::daAlinkHIO_bottle_c() {
    static const char plabel[] = "const daAlinkHIO_bottle_c1 daAlinkHIO_bottle_c0::m = {\n";
    m_len = 180;
    mp_src_data = (void*)&daAlinkHIO_bottle_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 9;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 0;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_item_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Bow
    ctx->genNode("弓", &mBow, 0, 0);
    // Boomerang
    ctx->genNode("ブーメラン", &mBoomerang, 0, 0);
    // Bomb
    ctx->genNode("爆弾", &mBomb, 0, 0);
    // Lantern PL
    ctx->genNode("カンテラＰＬ", &mLanternPL, 0, 0);
    // Lantern
    ctx->genNode("カンテラ", &mLantern, 0, 0);
    // Fire Mage Chain
    ctx->genNode("火魔人鎖", &mFmChain, 0, 0);
    // Iron Boots
    ctx->genNode("アイアンブーツ", &mIronBoots, 0, 0);
    // Picked up item
    ctx->genNode("拾い物", &mPickUp, 0, 0);
    // Board
    ctx->genNode("ボード", &mBoard, 0, 0);
    // Bottle
    ctx->genNode("ビン", &mBottle, 0, 0);
    // Hookshot
    ctx->genNode("フックショット", &mHookshot, 0, 0);
    // Spinner
    ctx->genNode("スピナー", &mSpinner, 0, 0);
    // Iron Ball
    ctx->genNode("鉄球", &mIronBall, 0, 0);
    // Copy Rod
    ctx->genNode("コピーロッド", &mCopyRod, 0, 0);
    // Zora Armor PL
    ctx->genNode("ゾーラ服ＰＬ", &mZoraArmorPL, 0, 0);
    // Bag hold equip
    daAlinkHIO_setAnmGenMessage(&m.mOneHandEquipAnm, ctx, "片手装備", 12.0f, 1);
    // Bag hold quick release A speed
    ctx->genSlider("片手早解除Ａ速度", &m.mOneHandReleaseAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Two-hand hold equip
    daAlinkHIO_setAnmGenMessage(&m.mTwoHandEquipAnm, ctx, "両手装備", 14.0f, 1);
    // Two-hand hold quick release A speed
    ctx->genSlider("両手早解除Ａ速度", &m.mTwoHandReleaseAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Item first-person long press time
    ctx->genSlider("アイテム主観長押時間", &m.mItemFPTransitionTimer, 0, 0x78, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // First-person downward max min move interval
    ctx->genSlider("主観下方最大", &m.mItemFPMaxUnk, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // First-person camera downward max min move interval
    ctx->genSlider("主観上方最大", &m.mItemFPUpMaxUnk, -0x4000, 0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // First-person upward max min move interval
    ctx->genSlider("Wii主観カメラ下方最大", &m.mWiiItemFPDownMaxUnk, 0, 0x4000, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // First-person camera upward max min move interval
    ctx->genSlider("Wii主観カメラ上方最大", &m.mWiiItemFPUpMaxUnk, -0x4000, 0, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_item_c::daAlinkHIO_item_c()
    : mBow(), mBoomerang(), mBomb(), mLanternPL(0), mLantern(), mFmChain(), mIronBoots(), mPickUp(),
      mBoard(), mBottle(), mHookshot(), mSpinner(), mIronBall(), mCopyRod(), mZoraArmorPL(2) {
    static const char plabel[] = "const daAlinkHIO_item_c1 daAlinkHIO_item_c0::m = {\n";
    m_len = 60;
    mp_src_data = (void*)&daAlinkHIO_item_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 6;
    m_f32_num = 2;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

daAlinkHIO_copyRod_c::~daAlinkHIO_copyRod_c() {}
daAlinkHIO_ironBall_c::~daAlinkHIO_ironBall_c() {}
daAlinkHIO_spinner_c::~daAlinkHIO_spinner_c() {}
daAlinkHIO_hookshot_c::~daAlinkHIO_hookshot_c() {}
daAlinkHIO_bottle_c::~daAlinkHIO_bottle_c() {}
daAlinkHIO_board_c::~daAlinkHIO_board_c() {}
daAlinkHIO_pickUp_c::~daAlinkHIO_pickUp_c() {}
daAlinkHIO_magneBoots_c::~daAlinkHIO_magneBoots_c() {}
daAlinkHIO_fmChain_c::~daAlinkHIO_fmChain_c() {}
daAlinkHIO_kandelaar_c::~daAlinkHIO_kandelaar_c() {}
daAlinkHIO_light_c::~daAlinkHIO_light_c() {}
daAlinkHIO_bomb_c::~daAlinkHIO_bomb_c() {}
daAlinkHIO_boom_c::~daAlinkHIO_boom_c() {}
daAlinkHIO_bow_c::~daAlinkHIO_bow_c() {}

void daAlinkHIO_ladder_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    // Climb up start A speed
    ctx->genSlider("登開始Ａ速度", &m.mClimbUpStartASpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Climb up start interpolation
    ctx->genSlider("登開始補間", &m.mClimbUpStartInterp, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Climb up end A speed
    ctx->genSlider("登終了Ａ速度", &m.mClimbUpEndASpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Climb up end CF (transition frame?)
    ctx->genSlider("登終了ＣＦ", &m.mClimbUpEndCF, 0.0f, 45.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    // Climb up end interpolation
    ctx->genSlider("登終了補間", &m.mClimbUpEndInterp, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    // Climb down start A speed
    ctx->genSlider("降開始Ａ速度", &m.mClimbDownStartASpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Climb down start interpolation
    ctx->genSlider("降開始補間", &m.mClimbDownStartInterp, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Climb down end A speed
    ctx->genSlider("降終了Ａ速度", &m.mClimbDownEndASpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Climb down end CF (transition frame?)
    ctx->genSlider("降終了ＣＦ", &m.mClimbDownEndCF, 0.0f, 20.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    // Climb down end interpolation
    ctx->genSlider("降終了補間", &m.mClimbDownEndInterp, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF,
                   0x200, 0x18);
    // Movement minimum A speed
    ctx->genSlider("移動最低Ａ速度", &m.mMoveMinASpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    // Movement maximum speed
    ctx->genSlider("移動最高Ａ速度", &m.mMoveMaxSpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                   0x18);
    // Movement interpolation
    ctx->genSlider("移動補間", &m.mMoveInterp, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    // Wall attach animation
    daAlinkHIO_setAnmGenMessage(&m.mWallAttachAnm, ctx, "壁飛付", 13.0f, 1);
    // Wall approach offset
    ctx->genSlider("壁飛付オフセット", &m.mWallApproachOffset, 0.0f, 300.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    // Wall attach miss animation
    daAlinkHIO_setAnmGenMessage(&m.mWallAttachMissAnm, ctx, "壁飛付ミス", 47.0f, 1);
    // Wall vertical movement minimum anm speed
    ctx->genSlider("壁縦移動最低Ａ速度", &m.mWallVerticalMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    // Wall vertical movement maximum anm speed
    ctx->genSlider("壁縦移動最高Ａ速度", &m.mWallVerticalMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xFFFF,
                   0xFFFF, 0x200, 0x18);
    // Wall horizontal movement minimum anm speed
    ctx->genSlider("壁横移動最低Ａ速度", &m.mWallHorizontalMinAnmSpeed, 0.0f, 10.0f, 0, NULL,
                   0xFFFF, 0xFFFF, 0x200, 0x18);
    // Wall horizontal movement maximum anm speed
    ctx->genSlider("壁横移動最高Ａ速度", &m.mWallHorizontalMaxAnmSpeed, 0.0f, 10.0f, 0, NULL,
                   0xFFFF, 0xFFFF, 0x200, 0x18);
}

daAlinkHIO_ladder_c::daAlinkHIO_ladder_c() {
    static const char plabel[] = "const daAlinkHIO_ladder_c1 daAlinkHIO_ladder_c0::m = {\n";
    m_len = 112;
    mp_src_data = (void*)&daAlinkHIO_ladder_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 18;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_roofHang_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Roof hang
    daAlinkHIO_setAnmGenMessage(&m.mRoofHangAnm, ctx, "張付", 34.0f, 1);
    // Standby A speed
    ctx->genSlider("待機Ａ速度", &m.mStandbyASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Standby interpolation
    ctx->genSlider("待機補間", &m.mStandbyInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Vertical movement minimum A speed
    ctx->genSlider("縦移動最低Ａ速度", &m.mVertMoveMinASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Vertical movement maximum A speed
    ctx->genSlider("縦移動最高Ａ速度", &m.mVertMoveMaxASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Vertical movement interpolation
    ctx->genSlider("縦移動補間", &m.mVertMoveInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Vertical movement minimum speed
    ctx->genSlider("縦移動最低速度", &m.mVertMoveMinSpeed, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Vertical movement maximum speed
    ctx->genSlider("縦移動最高速度", &m.mVertMoveMaxSpeed, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Horizontal movement minimum A speed
    ctx->genSlider("横移動最低Ａ速度", &m.mHorizMoveMinASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Horizontal movement maximum A speed
    ctx->genSlider("横移動最高Ａ速度", &m.mHorizMoveMaxASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Horizontal movement interpolation
    ctx->genSlider("横移動補間", &m.mHorizMoveInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Turn angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Maximum turn angle
    ctx->genSlider("旋回角度最大", &m.mMaxTurnAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Minimum turn angle
    ctx->genSlider("旋回角度最小", &m.mMinTurnAngle, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Invert A speed
    ctx->genSlider("反転Ａ速度", &m.mInvertASpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Invert interpolation
    ctx->genSlider("反転補間", &m.mInvertInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_roofHang_c::daAlinkHIO_roofHang_c() {
    static const char plabel[] = "const daAlinkHIO_roofHang_c1 daAlinkHIO_roofHang_c0::m = {\n";
    m_len = 76;
    mp_src_data = (void*)&daAlinkHIO_roofHang_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 12;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_grab_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Force weight specification
    ctx->genSlider("強制重さ指定", &m.mForceWeightSpec, 0, 3, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Preparation
    daAlinkHIO_setAnmGenMessage(&m.mPrepareAnm, ctx, "準備", 4.0f, 0);
    // Lift up
    daAlinkHIO_setAnmGenMessage(&m.mLiftAnm, ctx, "持上", 9.0f, 1);
    // Throw
    daAlinkHIO_setAnmGenMessage(&m.mThrowAnm, ctx, "投げる", 39.0f, 1);
    // Carry
    daAlinkHIO_setAnmGenMessage(&m.mCarryAnm, ctx, "運び", 12.0f, 1);
    // Lift back
    daAlinkHIO_setReverseAnmGenMessage(&m.mLiftBackAnm, ctx, "持ち上げ後", 9.0f, 0);
    // Stand reverse
    daAlinkHIO_setReverseAnmGenMessage(&m.mStandReverseAnm, ctx, "立つ", 4.0f, 1);
    // Recoil
    daAlinkHIO_setAnmGenMessage(&m.mRecoilAnm, ctx, "反動", 19.0f, 1);
    // Failure
    daAlinkHIO_setAnmGenMessage(&m.mFailAnm, ctx, "失敗", 19.0f, 1);
    // Failure stop time
    ctx->genSlider("失敗停止時間", &m.mFailStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Carry lift
    daAlinkHIO_setAnmGenMessage(&m.mCarryLiftAnm, ctx, "抱え上げ", 39.0f, 0);
    // Carry place reverse
    daAlinkHIO_setReverseAnmGenMessage(&m.mCarryPlaceReverseAnm, ctx, "抱え置き", 39.0f, 1);
    // Iron ball throw
    daAlinkHIO_setAnmGenMessage(&m.mIronBallThrowAnm, ctx, "鉄玉投げ", 49.0f, 1);
}

daAlinkHIO_grab_c::daAlinkHIO_grab_c() {
    static const char plabel[] = "const daAlinkHIO_grab_c1 daAlinkHIO_grab_c0::m = {\n";
    m_len = 228;
    mp_src_data = (void*)&daAlinkHIO_grab_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 11;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 1;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_swim_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Start height
    ctx->genSlider("開始高さ", &m.mStartHeight, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Float up height
    ctx->genSlider("浮上高さ", &m.mFloatUpHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Float up
    daAlinkHIO_setAnmGenMessage(&m.mFloatUpAnm, ctx, "浮上", 25.0f, 1);
    // Turn rate
    ctx->genSlider("旋回率", &m.mTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max turn
    ctx->genSlider("旋回最大", &m.mMaxTurn, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Min turn
    ctx->genSlider("旋回最小", &m.mMinTurn, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Underwater turn rate
    ctx->genSlider("水中旋回率", &m.mUnderwaterTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Underwater max turn
    ctx->genSlider("水中旋回最大", &m.mUnderwaterMaxTurn, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Underwater min turn
    ctx->genSlider("水中旋回最小", &m.mUnderwaterMinTurn, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Forward min speed
    ctx->genSlider("前最小速度", &m.mForwardMinSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Forward max speed
    ctx->genSlider("前最高速度", &m.mForwardMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Strafe max speed
    ctx->genSlider("横最高速度", &m.mStrafeMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Backward max speed
    ctx->genSlider("後最高速度", &m.mBackwardMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Underwater max speed
    ctx->genSlider("水中最高速度", &m.mUnderwaterMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater button additional speed
    ctx->genSlider("水中ボタン追加速度", &m.mUnderwaterButtonAdditionalSpeed, 0.0f, 50.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Underwater fall max speed
    ctx->genSlider("水中落下時最高速度", &m.mUnderwaterFallMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速", &m.mAcceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max fall speed
    ctx->genSlider("最大落下速度", &m.mMaxFallSpeed, -100.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Standing max fall speed
    ctx->genSlider("立ち最大落下速度", &m.mStandingMaxFallSpeed, -100.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Boots gravity
    ctx->genSlider("ブーツ重力", &m.mBootsGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Boots max fall speed
    ctx->genSlider("ブーツ最大落下速度", &m.mBootsMaxFallSpeed, -100.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Max float up speed
    ctx->genSlider("最大浮上速度", &m.mMaxFloatUpSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Buoyancy
    ctx->genSlider("浮力", &m.mBuoyancy, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Wait anm speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Wait interpolation
    ctx->genSlider("待機補間", &m.mWaitInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Wait up down shake amount
    ctx->genSlider("待機上下揺れ量", &m.mWaitUpDownShakeAmount, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Forward min anm speed
    ctx->genSlider("前最小Ａ速度", &m.mForwardMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Forward max anm speed
    ctx->genSlider("前最大Ａ速度", &m.mForwardMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Strafe min anm speed
    ctx->genSlider("横最小Ａ速度", &m.mStrafeMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Strafe max anm speed
    ctx->genSlider("横最大Ａ速度", &m.mStrafeMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Backward min anm speed
    ctx->genSlider("後最小Ａ速度", &m.mBackwardMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Backward max anm speed
    ctx->genSlider("後最大Ａ速度", &m.mBackwardMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Underwater min anm speed
    ctx->genSlider("水中最小Ａ速度", &m.mUnderwaterMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Underwater max anm speed
    ctx->genSlider("水中最大Ａ速度", &m.mUnderwaterMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Underwater additional A speed
    ctx->genSlider("水中追加Ａ速度", &m.mUnderwaterAdditionalAnmSpeed, 0.0f, 5.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Movement interpolation
    ctx->genSlider("移動補間", &m.mMoveInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Climb height
    ctx->genSlider("よじ登り高さ", &m.mClimbHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Climb water surface under
    ctx->genSlider("よじ登り水面下", &m.mClimbWaterSurfaceUnder, -100.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Normal buoyancy water depth
    ctx->genSlider("通常浮力水深", &m.mNormalBuoyancyWaterDepth, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Zora clothes buoyancy
    ctx->genSlider("ゾーラ服浮力", &m.mZoraClothesBuoyancy, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Dive
    daAlinkHIO_setAnmGenMessage(&m.mDiveAnm, ctx, "潜り", 23.0f, 0);
    // Underwater dive
    daAlinkHIO_setAnmGenMessage(&m.mUnderwaterDiveAnm, ctx, "水中潜り", 23.0f, 0);
    // Float up swim speed rate
    ctx->genSlider("浮上泳ぎ速度率", &m.mFloatUpSwimSpeedRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Dash max speed
    ctx->genSlider("Ｄ最高速度", &m.mDashMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dash deceleration
    ctx->genSlider("Ｄ減速", &m.mDashDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dash min A speed
    ctx->genSlider("Ｄ最小Ａ速度", &m.mDashMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dash max A speed
    ctx->genSlider("Ｄ最大Ａ速度", &m.mDashMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dash
    daAlinkHIO_setAnmGenMessage(&m.mDashAnm, ctx, "ダッシュ", 37.0f, 0);
}

daAlinkHIO_swim_c::daAlinkHIO_swim_c() {
    static const char plabel[] = "const daAlinkHIO_swim_c1 daAlinkHIO_swim_c0::m = {\n";
    m_len = 252;
    mp_src_data = (void*)&daAlinkHIO_swim_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 8;
    m_f32_num = 39;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlMove_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle A speed
    ctx->genSlider("待機Ａ速度", &m.mIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk A speed
    ctx->genSlider("歩きＡ速度", &m.mWalkAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Brisk walk A speed
    ctx->genSlider("速歩Ａ速度", &m.mBriskWalkAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run A speed
    ctx->genSlider("走りＡ速度", &m.mRunAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Quick run A speed
    ctx->genSlider("速走Ａ速度", &m.mQuickRunAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle to walk rate
    ctx->genSlider("待機→歩き率", &m.mIdleToWalkRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk to brisk walk rate
    ctx->genSlider("歩き→速歩率", &m.mWalkToBriskWalkRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Standby run to run rate
    ctx->genSlider("待走→走り率", &m.mStandbyRunToRunRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Run to quick run rate
    ctx->genSlider("走り→速走率", &m.mRunToQuickRunRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run A minimum blend rate
    ctx->genSlider("走りＡ最低合成率", &m.mRunAnmMinBlendRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Normal interpolation
    ctx->genSlider("通常補間", &m.mNormalInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk-run interpolation
    ctx->genSlider("歩き走り補間", &m.mWalkRunInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Idle interpolation
    ctx->genSlider("待機補間", &m.mIdleInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Turn maximum
    ctx->genSlider("旋回最大", &m.mTurnMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Turn minimum
    ctx->genSlider("旋回最小", &m.mTurnMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Turn angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速", &m.mAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slip
    daAlinkHIO_setAnmGenMessage(&m.mSlipAnm, ctx, "滑り", 14.0f, 0);
    // Slip start rate
    ctx->genSlider("滑り開始率", &m.mSlipStartRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Slip initial speed rate
    ctx->genSlider("滑り初速率", &m.mSlipInitSpeedRate, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Slip deceleration
    ctx->genSlider("滑り減速", &m.mSlipDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Reverse turn
    daAlinkHIO_setAnmGenMessage(&m.mReverseAnm, ctx, "反転", 14.0f, 1);
    // Idle reverse start frame
    ctx->genSlider("待機反転開始Ｆ", &m.mIdleReverseStartFrame, 0.0f, 14.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Reverse speed
    ctx->genSlider("反転速度", &m.mReverseSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Reverse fall height
    ctx->genSlider("反転落下高さ", &m.mReverseFallHeight, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Reverse fall interpolation
    ctx->genSlider("反転落下補間", &m.mReverseFallInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Gaze max speed
    ctx->genSlider("注目最高速度", &m.mGazeMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Gaze idle A speed
    ctx->genSlider("注目待機Ａ速度", &m.mGazeIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Dash
    daAlinkHIO_setAnmGenMessage(&m.mADashAnm, ctx, "Ａダッシュ", 19.0f, 1);
    // A Dash duration
    ctx->genSlider("Ａダッシュ時間", &m.mADashDuration, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // A Dash cooldown time
    ctx->genSlider("Ａダッシュ禁止時間", &m.mADashCooldownTime, 0, 300, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Dash max speed
    ctx->genSlider("Ａダッシュ最高速度", &m.mADashMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Dash initial speed
    ctx->genSlider("Ａダッシュ初速", &m.mADashInitSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Dash quick run A speed
    ctx->genSlider("Ａダッシュ速走Ａ速度", &m.mADashQuickRunAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash acceleration
    ctx->genSlider("Ａダッシュ加速", &m.mADashAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Dash duration (slow)
    ctx->genSlider("Ａダッシュ時間（遅）", &m.mADashDurationSlow, 0, 300, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Dash cooldown time (slow)
    ctx->genSlider("Ａダッシュ禁止時間（遅）", &m.mADashCooldownTimeSlow, 0, 300, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash max speed (slow)
    ctx->genSlider("Ａダッシュ最高速度（遅）", &m.mADashMaxSpeedSlow, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash initial speed (slow)
    ctx->genSlider("Ａダッシュ初速（遅）", &m.mADashInitSpeedSlow, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash quick run A speed (slow)
    ctx->genSlider("Ａダッシュ速走Ａ速度（遅）", &m.mADashQuickRunASpeedSlow, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // A Dash acceleration (slow)
    ctx->genSlider("Ａダッシュ加速（遅）", &m.mADashAccelerationSlow, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash duration (slow 2)
    ctx->genSlider("Ａダッシュ時間（遅２）", &m.mADashDurationSlow2, 0, 300, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash cooldown time (slow 2)
    ctx->genSlider("Ａダッシュ禁止時間（遅２）", &m.mADashCooldownTimeSlow2, 0, 300, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // A Dash max speed (slow 2)
    ctx->genSlider("Ａダッシュ最高速度（遅２）", &m.mADashMaxSpeedSlow2, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // A Dash initial speed (slow 2)
    ctx->genSlider("Ａダッシュ初速（遅２）", &m.mADashInitSpeedSlow2, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // A Dash quick run A speed (slow 2)
    ctx->genSlider("Ａダッシュ速走Ａ速度（遅２）", &m.mADashQuickRunASpeedSlow2, 0.0f, 10.0f, 0,
                   NULL, 0xffff, 0xffff, 0x200, 0x18);
    // A Dash acceleration (slow 2)
    ctx->genSlider("Ａダッシュ加速（遅２）", &m.mADashAccelerationSlow2, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Dash rebound
    daAlinkHIO_setAnmGenMessage(&m.mDashReboundAnm, ctx, "ダッシュ跳返", 41.0f, 1);
    // Dash rebound horizontal speed
    ctx->genSlider("ダッシュ跳返水平速度", &m.mDashReboundHorizontalSpeed, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Dash rebound vertical speed
    ctx->genSlider("ダッシュ跳返垂直速度", &m.mDashReboundVerticalSpeed, 0.0f, 100.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlMove_c::daAlinkHIO_wlMove_c() {
    static const char plabel[] = "const daAlinkHIO_wlMove_c1 daAlinkHIO_wlMove_c0::m = {\n";
    m_len = 256;
    mp_src_data = (void*)&daAlinkHIO_wlMove_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 10;
    m_f32_num = 39;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlMoveNoP_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle animation speed
    ctx->genSlider("待機Ａ速度", &m.mIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk animation speed
    ctx->genSlider("歩きＡ速度", &m.mWalkAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jog animation speed
    ctx->genSlider("速歩Ａ速度", &m.mJogAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run animation speed
    ctx->genSlider("走りＡ速度", &m.mRunAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle to walk rate
    ctx->genSlider("待機→歩き率", &m.mIdleToWalkRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk to jog rate
    ctx->genSlider("歩き→速歩率", &m.mWalkToJogRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jog to run rate
    ctx->genSlider("待走→走り率", &m.mJogToRunRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run animation minimum blend rate
    ctx->genSlider("走りＡ最低合成率", &m.mRunAnmMinBlendRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Walk run interpolation
    ctx->genSlider("歩き走り補間", &m.mWalkRunInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max turn
    ctx->genSlider("旋回最大", &m.mTurnMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Min turn
    ctx->genSlider("旋回最小", &m.mTurnMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Turn angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速", &m.mAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Slide start rate
    ctx->genSlider("滑り開始率", &m.mSlideStartRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Slide initial speed
    ctx->genSlider("滑り初速率", &m.mSlideInitSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Slide deceleration
    ctx->genSlider("滑り減速", &m.mSlideDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Reverse speed
    ctx->genSlider("反転速度", &m.mReverseSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlMoveNoP_c::daAlinkHIO_wlMoveNoP_c() {
    static const char plabel[] = "const daAlinkHIO_wlMoveNoP_c1 daAlinkHIO_wlMoveNoP_c0::m = {\n";
    m_len = 72;
    mp_src_data = (void*)&daAlinkHIO_wlMoveNoP_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 16;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlAtnMove_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速度", &m.mAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Walk animation speed
    ctx->genSlider("歩きＡ速度", &m.mWalkAnmSpeed, 0.01f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run animation speed
    ctx->genSlider("走りＡ速度", &m.mRunAnmSpeed, 0.01f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Walk change rate
    ctx->genSlider("歩き変更率", &m.mWalkChangeRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Run change rate
    ctx->genSlider("走り変更率", &m.mRunChangeRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Turn angle max
    ctx->genSlider("旋回角度最大", &m.mTurnAngleMax, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Turn angle min
    ctx->genSlider("旋回角度最小", &m.mTurnAngleMin, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Turn angle rate
    ctx->genSlider("旋回角度率", &m.mTurnAngleRate, 0, 0x14, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Rear max speed
    ctx->genSlider("後方最高速度", &m.mRearMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rear acceleration
    ctx->genSlider("後方加速度", &m.mRearAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rear walk animation speed
    ctx->genSlider("後歩Ａ速度", &m.mRearWalkAnmSpeed, 0.01f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rear walk change rate
    ctx->genSlider("後歩変更率", &m.mRearWalkChangeRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Rear deceleration
    ctx->genSlider("後方減速", &m.mRearDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlAtnMove_c::daAlinkHIO_wlAtnMove_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtnMove_c1 daAlinkHIO_wlAtnMove_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_wlAtnMove_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 12;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlHowl_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Howl animation
    daAlinkHIO_setAnmGenMessage(&m.mHowlAnm, ctx, "遠吠え", 49.0f, 1);
    // Sitting howl animation
    daAlinkHIO_setAnmGenMessage(&m.mSittingHowlAnm, ctx, "座遠吠え", 71.0f, 1);
    // Threat animation
    daAlinkHIO_setAnmGenMessage(&m.mThreatAnm, ctx, "威嚇", 14.0f, 1);
    // Threat stance animation
    daAlinkHIO_setAnmGenMessage(&m.mThreatStanceAnm, ctx, "威嚇ため", 17.0f, 0);
    // Threat move interpolation
    ctx->genSlider("威嚇移動補間", &m.mThreatMoveInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Threat move min anm speed
    ctx->genSlider("威嚇移動最小Ａ速度", &m.mThreatMoveMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Threat move max anm speed
    ctx->genSlider("威嚇移動最大Ａ速度", &m.mThreatMoveMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Threat move rate
    ctx->genSlider("威嚇移動率", &m.mThreatMoveRate, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlHowl_c::daAlinkHIO_wlHowl_c() {
    static const char plabel[] = "const daAlinkHIO_wlHowl_c1 daAlinkHIO_wlHowl_c0::m = {\n";
    m_len = 96;
    mp_src_data = (void*)&daAlinkHIO_wlHowl_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlSideStep_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Side jump animation
    daAlinkHIO_setAnmGenMessage(&m.mSideJumpAnm, ctx, "横飛", 4.0f, 0);
    // Side jump horizontal speed
    ctx->genSlider("横飛水平速度", &m.mSideJumpHorizSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Side jump vertical speed
    ctx->genSlider("横飛垂直速度", &m.mSideJumpVertSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Side landing animation
    daAlinkHIO_setAnmGenMessage(&m.mSideLandAnm, ctx, "横着地", 23.0f, 1);
    // Back jump animation
    daAlinkHIO_setAnmGenMessage(&m.mBackJumpAnm, ctx, "後飛", 4.0f, 0);
    // Back jump horizontal speed
    ctx->genSlider("後飛水平速度", &m.mBackJumpHorizSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Back jump vertical speed
    ctx->genSlider("後飛垂直速度", &m.mBackJumpVertSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Back landing animation
    daAlinkHIO_setAnmGenMessage(&m.mBackLandAnm, ctx, "後着地", 23.0f, 1);
    // Fall height
    ctx->genSlider("落下高さ", &m.mFallHeight, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall interpolation
    ctx->genSlider("落下補間", &m.mFallInterp, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlSideStep_c::daAlinkHIO_wlSideStep_c() {
    static const char plabel[] = "const daAlinkHIO_wlSideStep_c1 daAlinkHIO_wlSideStep_c0::m = {\n";
    m_len = 104;
    mp_src_data = (void*)&daAlinkHIO_wlSideStep_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 6;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlBackJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Back jump animation
    daAlinkHIO_setAnmGenMessage(&m.mBackJumpAnm, ctx, "ジャンプ", 11.0f, 0);
    // Back jump horizontal speed
    ctx->genSlider("水平速度", &m.mBackJumpHorizSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back jump vertical speed
    ctx->genSlider("垂直速度", &m.mBackJumpVertSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Back landing animation
    daAlinkHIO_setAnmGenMessage(&m.mBackLandAnm, ctx, "着地", 28.0f, 1);
    // Fall height
    ctx->genSlider("落下高さ", &m.mFallHeight, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall interpolation
    ctx->genSlider("落下補間", &m.mFallInterp, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlBackJump_c::daAlinkHIO_wlBackJump_c() {
    static const char plabel[] = "const daAlinkHIO_wlBackJump_c1 daAlinkHIO_wlBackJump_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_wlBackJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAutoJump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max fall speed
    ctx->genSlider("最大落下速度", &m.mMaxFallSpeed, -300.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump animation
    daAlinkHIO_setAnmGenMessage(&m.mJumpAnm, ctx, "ジャンプ", 3.0f, 0);
    // Climb animation
    daAlinkHIO_setAnmGenMessage(&m.mClimbAnm, ctx, "空中", 6.0f, 0);
    // Jump limit speed
    ctx->genSlider("ジャンプ限度速度", &m.mJumpLimitSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump min speed
    ctx->genSlider("ジャンプ最低速度", &m.mJumpMinSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump max speed
    ctx->genSlider("ジャンプ最高速度", &m.mJumpMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Jump speed rate
    ctx->genSlider("ジャンプ速度率", &m.mJumpSpeedRate, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump angle
    ctx->genSlider("ジャンプ角度", &m.mJumpAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Jump fall interpolation
    ctx->genSlider("ジャンプ落下補間", &m.mJumpFallInterp, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Always max speed jump
    ctx->genCheckBox("必ず最高速度ジャンプ", &m.mAlwaysMaxSpeedJump, '\x01', 0, NULL, 0xffff,
                     0xffff, 0x200, 0x18);
    // Landing animation
    daAlinkHIO_setAnmGenMessage(&m.mLandingAnm, ctx, "着地", 24.0f, 1);
    // Normal fall interpolation
    ctx->genSlider("通常落下補間", &m.mNormalFallInterp, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Weak jump anm speed
    ctx->genSlider("ジャンプA速度(弱)", &m.mWeakJumpAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Weak jump limit speed
    ctx->genSlider("ジャンプ限度速度(弱)", &m.mWeakJumpLimitSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Weak jump min speed
    ctx->genSlider("ジャンプ最低速度(弱)", &m.mWeakJumpMinSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Weak jump max speed
    ctx->genSlider("ジャンプ最高速度(弱)", &m.mWeakJumpMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Weak jump speed rate
    ctx->genSlider("ジャンプ速度率(弱)", &m.mWeakJumpSpeedRate, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Weak jump angle
    ctx->genSlider("ジャンプ角度(弱)", &m.mWeakJumpAngle, 0, 0x4000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Hang limit height
    ctx->genSlider("ぶら下がり限界高さ", &m.mHangLimitHeight, 0.0f, 1000.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_wlAutoJump_c::daAlinkHIO_wlAutoJump_c() {
    static const char plabel[] = "const daAlinkHIO_wlAutoJump_c1 daAlinkHIO_wlAutoJump_c0::m = {\n";
    m_len = 124;
    mp_src_data = (void*)&daAlinkHIO_wlAutoJump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 4;
    m_s16_num = 2;
    m_f32_num = 14;
    f14 = 1;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlPush_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle Anm Speed
    ctx->genSlider("待機Ａ速度", &m.mIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle Interpolation
    ctx->genSlider("待機補間", &m.mIdleInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Push Anm Speed
    ctx->genSlider("押しＡ速度", &m.mPushAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Heavy Push Anm Speed
    ctx->genSlider("押しＡ速度（重）", &m.mHeavyPushAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Push Interpolation
    ctx->genSlider("押し補間", &m.mPushInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlPush_c::daAlinkHIO_wlPush_c() {
    static const char plabel[] = "const daAlinkHIO_wlPush_c1 daAlinkHIO_wlPush_c0::m = {\n";
    m_len = 20;
    mp_src_data = (void*)&daAlinkHIO_wlPush_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 5;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlLie_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Sitting animation
    daAlinkHIO_setAnmGenMessage(&m.mSittingAnm, ctx, "座り", 14.0f, 1);
    // Sitting idle anm speed
    ctx->genSlider("座り待機Ａ速度", &m.mSittingIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Sitting idle interp
    ctx->genSlider("座り待機補間", &m.mSittingIdleInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Stand up interp
    ctx->genSlider("立ち上がり補間", &m.mStandUpInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Prone start animation
    daAlinkHIO_setAnmGenMessage(&m.mProneStartAnm, ctx, "伏せ開始", 11.0f, 1);
    // Prone min anm speed
    ctx->genSlider("伏せ最小Ａ速度", &m.mProneMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Prone max anm speed
    ctx->genSlider("伏せ最大Ａ速度", &m.mProneMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Prone move interp
    ctx->genSlider("伏せ移動補間", &m.mProneMoveInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Prone move rate
    ctx->genSlider("伏せ移動率", &m.mProneMoveRate, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Prone turn rate
    ctx->genSlider("伏せ旋回率", &m.mProneTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Prone turn max
    ctx->genSlider("伏せ旋回最大", &m.mProneTurnMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Prone turn min
    ctx->genSlider("伏せ旋回最小", &m.mProneTurnMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlLie_c::daAlinkHIO_wlLie_c() {
    static const char plabel[] = "const daAlinkHIO_wlLie_c1 daAlinkHIO_wlLie_c0::m = {\n";
    m_len = 76;
    mp_src_data = (void*)&daAlinkHIO_wlLie_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 4;
    m_f32_num = 7;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlWallHang_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Auto walk height
    ctx->genSlider("自動歩き高さ", &m.mAutoWalkHeight, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Small jump height
    ctx->genSlider("小ジャンプ高さ", &m.mSmallJumpHeight, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wall grab height
    ctx->genSlider("壁掴み高さ", &m.mWallGrabHeight, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Climb height
    ctx->genSlider("よじ登り高さ", &m.mClimbHeight, 0.0f, 300.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Hanging foot height
    ctx->genSlider("ぶら下がり足位置高さ", &m.mHangingFootHeight, 0.0f, 300.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Jump horizontal speed
    ctx->genSlider("ジャンプ水平速度", &m.mJumpHorizontalSpeed, 0.0f, 50.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Jump target height offset
    ctx->genSlider("ジャンプ目標高さオフセット", &m.mJumpTargetHeightOffset, 0.0f, 200.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Jump to fall interpolation
    ctx->genSlider("ジャンプ→落下補間", &m.mJumpToFallInterp, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Pre-grab jump animation
    daAlinkHIO_setAnmGenMessage(&m.mPreGrabJumpAnm, ctx, "掴前ジャンプ", 13.0f, 0);
    // Wall grab animation
    daAlinkHIO_setAnmGenMessage(&m.mWallGrabAnm, ctx, "壁掴み", 5.0f, 0);
    // Climb animation
    daAlinkHIO_setAnmGenMessage(&m.mClimbAnm, ctx, "よじ登り", 39.0f, 1);
    // Fall grab animation
    daAlinkHIO_setAnmGenMessage(&m.mFallGrabAnm, ctx, "落下掴み", 39.0f, 0);
    // Hanging fall interpolation
    ctx->genSlider("ぶら下がり落下補間", &m.mHangingFallInterp, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Ledge climb animation
    daAlinkHIO_setAnmGenMessage(&m.mLedgeClimbAnm, ctx, "段差登", 6.0f, 0);
    // Ledge land animation
    daAlinkHIO_setAnmGenMessage(&m.mLedgeLandAnm, ctx, "段差着地", 16.0f, 1);
}

daAlinkHIO_wlWallHang_c::daAlinkHIO_wlWallHang_c() {
    static const char plabel[] = "const daAlinkHIO_wlWallHang_c1 daAlinkHIO_wlWallHang_c0::m = {\n";
    m_len = 156;
    mp_src_data = (void*)&daAlinkHIO_wlWallHang_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 6;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 9;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlDamNormal_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Forward animation
    daAlinkHIO_setAnmGenMessage(&m.mForwardAnm, ctx, "前方", 9.0f, 1);
    // Backward animation
    daAlinkHIO_setAnmGenMessage(&m.mBackwardAnm, ctx, "後方", 9.0f, 1);
    // Left animation
    daAlinkHIO_setAnmGenMessage(&m.mLeftAnm, ctx, "左方", 9.0f, 1);
    // Right animation
    daAlinkHIO_setAnmGenMessage(&m.mRightAnm, ctx, "右方", 9.0f, 1);
    // Initial speed
    ctx->genSlider("初速", &m.mInitSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack speed rate
    ctx->genSlider("攻撃速度率", &m.mAttackSpeedRate, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Ice damage A speed
    ctx->genSlider("氷ダメージＡ速度", &m.mIceDamageAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_wlDamNormal_c::daAlinkHIO_wlDamNormal_c() {
    static const char plabel[] =
        "const daAlinkHIO_wlDamNormal_c1 daAlinkHIO_wlDamNormal_c0::m = {\n";
    m_len = 96;
    mp_src_data = (void*)&daAlinkHIO_wlDamNormal_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 4;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlDamLaHu_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Damage interpolation
    ctx->genSlider("ダメージ補間", &m.mDamageInterp, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Horizontal speed
    ctx->genSlider("水平速度", &m.mHorizontalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Vertical speed
    ctx->genSlider("垂直速度", &m.mVerticalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Bounce speed
    ctx->genSlider("跳ね返り速度", &m.mBounceSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Body rotation speed
    ctx->genSlider("体回転速度", &m.mBodyRotationSpeed, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Front rise animation
    daAlinkHIO_setAnmGenMessage(&m.mFrontRiseAnm, ctx, "前起", 49.0f, 1);
    // Back rise animation
    daAlinkHIO_setAnmGenMessage(&m.mBackRiseAnm, ctx, "後起", 39.0f, 1);
    // Left rise animation
    daAlinkHIO_setAnmGenMessage(&m.mLeftRiseAnm, ctx, "左起", 34.0f, 1);
    // Right rise animation
    daAlinkHIO_setAnmGenMessage(&m.mRightRiseAnm, ctx, "右起", 34.0f, 1);
}

daAlinkHIO_wlDamLaHu_c::daAlinkHIO_wlDamLaHu_c(int param_0) {
    static void* baseP[] = {
        (void*)&daAlinkHIO_wlDamLarge_c0::m,
        (void*)&daAlinkHIO_wlDamHuge_c0::m,
    };

    static const char* plabel[] = {
        "const daAlinkHIO_wlDamLaHu_c1 daAlinkHIO_wlDamLarge_c0::m = {\n",
        "const daAlinkHIO_wlDamLaHu_c1 daAlinkHIO_wlDamHuge_c0::m = {\n",
    };

    m_len = 108;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 6;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlDamCaught_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Caught time
    ctx->genSlider("捕まり時間", &m.mCaughtTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Input decay time
    ctx->genSlider("入力減り時間", &m.mInputDecayTime, 0, 0x1e, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Wait additional anm speed
    ctx->genSlider("待機追加Ａ速度", &m.mWaitAddAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Wait interpolation
    ctx->genSlider("待機補間", &m.mWaitInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlDamCaught_c::daAlinkHIO_wlDamCaught_c() {
    static const char plabel[] =
        "const daAlinkHIO_wlDamCaught_c1 daAlinkHIO_wlDamCaught_c0::m = {\n";
    m_len = 16;
    mp_src_data = (void*)&daAlinkHIO_wlDamCaught_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlDamFall_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Small damage height
    ctx->genSlider("小ダメージ高さ", &m.mSmallDamageHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Large damage height
    ctx->genSlider("大ダメージ高さ", &m.mLargeDamageHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Air animation transition height
    ctx->genSlider("空中アニメ移行高さ", &m.mAirAnmTransitionHeight, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Air animation interpolation
    ctx->genSlider("空中アニメ補間", &m.mAirAnmInterp, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Landing animation
    daAlinkHIO_setAnmGenMessage(&m.mLandingAnm, ctx, "着地", 11.0f, 0);
    // Small damage landing start frame
    ctx->genSlider("小ダメージ着地開始Ｆ", &m.mSmallDamageLandingStartFrame, 0.0f, 11.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Small stop time
    ctx->genSlider("小停止時間", &m.mSmallStopTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Large stop time
    ctx->genSlider("大停止時間", &m.mLargeStopTime, 0, 0x96, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Right rise animation
    daAlinkHIO_setAnmGenMessage(&m.mRightRiseAnm, ctx, "起上り", 17.0f, 1);
}

daAlinkHIO_wlDamFall_c::daAlinkHIO_wlDamFall_c() {
    static const char plabel[] = "const daAlinkHIO_wlDamFall_c1 daAlinkHIO_wlDamFall_c0::m = {\n";
    m_len = 64;
    mp_src_data = (void*)&daAlinkHIO_wlDamFall_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 5;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlDamage_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Normal
    ctx->genNode("通常", &mNormal, 0, 0);
    // Large
    ctx->genNode("大", &mLarge, 0, 0);
    // Huge
    ctx->genNode("強大", &mHuge, 0, 0);
    // Fall
    ctx->genNode("落下", &mFall, 0, 0);
    // Capture
    ctx->genNode("捕縛", &mCapture, 0, 0);
    // Running animation
    daAlinkHIO_setAnmGenMessage(&m.mRunningAnm, ctx, "走り", 9.0f, 0);
    // Polygon animation
    daAlinkHIO_setAnmGenMessage(&m.mPolygonAnm, ctx, "ポリゴン", 9.0f, 1);
    // Return wake-up anm speed
    ctx->genSlider("復帰起き上がりＡ速度", &m.mReturnWakeupAnmSpeed, 1.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
}

daAlinkHIO_wlDamage_c::daAlinkHIO_wlDamage_c()
    : mNormal(), mLarge(0), mHuge(1), mFall(), mCapture() {
    static const char plabel[] = "const daAlinkHIO_wlDamage_c1 daAlinkHIO_wlDamage_c0::m = {\n";
    m_len = 44;
    mp_src_data = (void*)&daAlinkHIO_wlDamage_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 1;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

daAlinkHIO_wlDamCaught_c::~daAlinkHIO_wlDamCaught_c() {}
daAlinkHIO_wlDamFall_c::~daAlinkHIO_wlDamFall_c() {}
daAlinkHIO_wlDamLaHu_c::~daAlinkHIO_wlDamLaHu_c() {}
daAlinkHIO_wlDamNormal_c::~daAlinkHIO_wlDamNormal_c() {}

void daAlinkHIO_wlSlide_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Sliding angle
    ctx->genSlider("滑り角度", &m.mSlidingAngle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Climbing angle
    ctx->genSlider("登坂角度", &m.mClimbingAngle, 0.0f, 90.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Climbing anm speed
    ctx->genSlider("登坂Ａ速度", &m.mClimbingAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Climbing max speed
    ctx->genSlider("登坂最高速度", &m.mClimbingMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Front slide animation
    daAlinkHIO_setAnmGenMessage(&m.mFrontSlideAnm, ctx, "前滑り", 24.0f, 0);
    // Back slide animation
    daAlinkHIO_setAnmGenMessage(&m.mBackSlideAnm, ctx, "後滑り", 7.0f, 0);
    // Max speed
    ctx->genSlider("最大速度", &m.mMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Acceleration
    ctx->genSlider("加速度", &m.mAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Landing animation
    daAlinkHIO_setAnmGenMessage(&m.mLandingAnm, ctx, "着地", 14.0f, 1);
    // Climbing anm speed (weak)
    ctx->genSlider("登坂Ａ速度(弱)", &m.mClimbingAnmSpeedWeak, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Climbing max speed (weak)
    ctx->genSlider("登坂最高速度(弱)", &m.mClimbingMaxSpeedWeak, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Slip down acceleration
    ctx->genSlider("ずり落ち加速度", &m.mSlipDownAcceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Slip down max speed
    ctx->genSlider("ずり落ち最高速度", &m.mSlipDownMaxSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Slip down rotation max
    ctx->genSlider("ずり落ち回転最大", (short*)&m.mSlipDownRotationMax, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Slip down rotation min
    ctx->genSlider("ずり落ち回転最小", (short*)&m.mSlipDownRotationMin, 0, 0x7fff, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Slip down recovery animation
    daAlinkHIO_setAnmGenMessage(&m.mSlipDownRecoveryAnm, ctx, "ずり落ち復帰", 29.0f, 1);
}

daAlinkHIO_wlSlide_c::daAlinkHIO_wlSlide_c() {
    static const char plabel[] = "const daAlinkHIO_wlSlide_c1 daAlinkHIO_wlSlide_c0::m = {\n";
    m_len = 124;
    mp_src_data = (void*)&daAlinkHIO_wlSlide_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 10;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlRope_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Movement Min Anm Speed
    ctx->genSlider("移動最小Ａ速度", &m.mMovementMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Movement Max Anm Speed
    ctx->genSlider("移動最大Ａ速度", &m.mMovementMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Back Movement Max Anm Speed
    ctx->genSlider("後移動最大Ａ速度", &m.mBackMovementMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Movement Interpolation
    ctx->genSlider("移動補間", &m.mMovementInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Movement Rate
    ctx->genSlider("移動率", &m.mMovementRate, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Twist Climb Anm Speed
    ctx->genSlider("よじ登りＡ速度", &m.mTwistClimbAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Fall Interpolation
    ctx->genSlider("落下補間", &m.mFallInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Twist Climb Interpolation
    ctx->genSlider("よじ登り補間", &m.mTwistClimbInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Flip
    daAlinkHIO_setAnmGenMessage(&m.mFlipAnm, ctx, "反転", 16.0f, 0);
    // Movement Sway Transition Time
    ctx->genSlider("移動ふらつき移行時間", &m.mMovementSwayTransitionTime, 0, 0x7fff, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlRope_c::daAlinkHIO_wlRope_c() {
    static const char plabel[] = "const daAlinkHIO_wlRope_c1 daAlinkHIO_wlRope_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_wlRope_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 8;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtWait_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack animation
    daAlinkHIO_setAnmGenMessage(&m.mAttackAnm, ctx, "攻撃", field_0x34, 1);
    // Speed
    ctx->genSlider("速度", &m.mSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Speed addition F
    ctx->genSlider("速度追加Ｆ", &m.mSpeedAddForward, 0.0f, field_0x34, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Attack judgment start F
    ctx->genSlider("攻撃判定開始Ｆ", &m.mAttackJudgmentStartForward, 0.0f, field_0x34, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Attack judgment end F
    ctx->genSlider("攻撃判定終了Ｆ", &m.mAttackJudgmentEndForward, 0.0f, field_0x34, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Stop time
    ctx->genSlider("停止時間", (short*)&m.mStopTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Combo mid start F
    ctx->genSlider("コンボ中開始Ｆ", &m.mComboMidStartF, 0.0f, field_0x34 + 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Combo mid C F
    ctx->genSlider("コンボ中ＣＦ", &m.mComboMidCF, 0.0f, field_0x34 + 1.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Combo mid stop time
    ctx->genSlider("コンボ中停止時間", (short*)&m.mComboMidStopTime, 0, 300, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Attack radius offset
    ctx->genSlider("攻撃半径オフセット", &m.mAttackRadiusOffset, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Attack radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack height
    ctx->genSlider("攻撃高さ", &m.mAttackHeight, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlAtWait_c::daAlinkHIO_wlAtWait_c(int param_0, float param_1) {
    field_0x34 = param_1;

    static void* baseP[] = {
        (void*)&daAlinkHIO_wlAtWaLr_c0::m,
        (void*)&daAlinkHIO_wlAtWaTl_c0::m,
        (void*)&daAlinkHIO_wlAtWaSc_c0::m,
    };

    static const char* plabel[] = {
        "const daAlinkHIO_cutNormal_c1 daAlinkHIO_wlAtWaLr_c0::m = {\n",
        "const daAlinkHIO_cutNormal_c1 daAlinkHIO_wlAtWaTl_c0::m = {\n",
        "const daAlinkHIO_cutNormal_c1 daAlinkHIO_wlAtWaSc_c0::m = {\n",
    };

    m_len = 60;
    mp_src_data = baseP[param_0];
    mp_data = (void*)&m;
    m_name_string = plabel[param_0];
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 9;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtRoll_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack animation
    daAlinkHIO_setAnmGenMessage(&m.mAttackAnm, ctx, "攻撃", 40.0f, 1);
    // Spider thread escape start F
    ctx->genSlider("クモ糸脱出時開始Ｆ", &m.mSpiderThreadEscapeStartF, 0.0f, 40.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Attack radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack speed
    ctx->genSlider("攻撃速度", &m.mAttackSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlAtRoll_c::daAlinkHIO_wlAtRoll_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtRoll_c1 daAlinkHIO_wlAtRoll_c0::m = {\n";
    m_len = 32;
    mp_src_data = (void*)&daAlinkHIO_wlAtRoll_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtNjump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Aerial Attack
    daAlinkHIO_setAnmGenMessage(&m.mAerialAttackAnm, ctx, "攻撃", 6.0f, 0);
    // Attack Initial Speed
    ctx->genSlider("攻撃初速", &m.mAttackInitSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Horizontal Speed
    ctx->genSlider("最高水平速度", &m.mMaxHorizontalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Min Horizontal Speed
    ctx->genSlider("最低水平速度", &m.mMinHorizontalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max Vertical Speed
    ctx->genSlider("最高垂直速度", &m.mMaxVerticalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Min Vertical Speed
    ctx->genSlider("最低垂直速度", &m.mMinVerticalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Aerial Anm Speed
    ctx->genSlider("空中Ａ速度", &m.mAerialAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Aerial Interpolation
    ctx->genSlider("空中補間", &m.mAerialInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack Radius Offset
    ctx->genSlider("攻撃半径オフセット", &m.mAttackRadiusOffset, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Attack Radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack Height
    ctx->genSlider("攻撃高さ", &m.mAttackHeight, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlAtNjump_c::daAlinkHIO_wlAtNjump_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtNjump_c1 daAlinkHIO_wlAtNjump_c0::m = {\n";
    m_len = 60;
    mp_src_data = (void*)&daAlinkHIO_wlAtNjump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 10;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtCjump_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Aerial Attack
    daAlinkHIO_setAnmGenMessage(&m.mAerialAttackAnm, ctx, "攻撃", 3.0f, 0);
    // Attack Initial Speed
    ctx->genSlider("攻撃初速", &m.mAttackInitSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Horizontal Speed
    ctx->genSlider("最高水平速度", &m.mMaxHorizontalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Min Horizontal Speed
    ctx->genSlider("最低水平速度", &m.mMinHorizontalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max Vertical Speed
    ctx->genSlider("最高垂直速度", &m.mMaxVerticalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Min Vertical Speed
    ctx->genSlider("最低垂直速度", &m.mMinVerticalSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Aerial Anm Speed
    ctx->genSlider("空中Ａ速度", &m.mAerialAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Aerial Interpolation
    ctx->genSlider("空中補間", &m.mAerialInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack Radius Offset
    ctx->genSlider("攻撃半径オフセット", &m.mAttackRadiusOffset, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Attack Radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack Height
    ctx->genSlider("攻撃高さ", &m.mAttackHeight, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Flip Kick
    daAlinkHIO_setAnmGenMessage(&m.mFlipKickAnm, ctx, "反転蹴り", 7.0f, 0);
    // Flip Kick Post Start F
    ctx->genSlider("反転蹴り後開始Ｆ", &m.mFlipKickPostStartF, 0.0f, 11.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Flip Kick Post Anm Speed
    ctx->genSlider("反転蹴り後Ａ速度", &m.mFlipKickPostAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Hit Decel Rate
    ctx->genSlider("ヒット時減速率", &m.mHitDecelRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Hit Min Speed
    ctx->genSlider("ヒット時最低速度", &m.mHitMinSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Hit Decel
    ctx->genSlider("ヒット時減速", &m.mHitDecel, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlAtCjump_c::daAlinkHIO_wlAtCjump_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtCjump_c1 daAlinkHIO_wlAtCjump_c0::m = {\n";
    m_len = 100;
    mp_src_data = (void*)&daAlinkHIO_wlAtCjump_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 15;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtLand_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Normal Landing
    daAlinkHIO_setAnmGenMessage(&m.mNormalLandingAnm, ctx, "通常着地", 19.0f, 1);
    // Front Slide
    daAlinkHIO_setAnmGenMessage(&m.mFrontSlideAnm, ctx, "前滑", 14.0f, 1);
    // Back Slide
    daAlinkHIO_setAnmGenMessage(&m.mBackSlideAnm, ctx, "後滑", 19.0f, 1);
    // Slide Deceleration
    ctx->genSlider("滑り減速", &m.mSlideDeceleration, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}
daAlinkHIO_wlAtLand_c::daAlinkHIO_wlAtLand_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtLand_c1 daAlinkHIO_wlAtLand_c0::m = {\n";
    m_len = 64;
    mp_src_data = (void*)&daAlinkHIO_wlAtLand_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 1;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtDown_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Low Stance
    daAlinkHIO_setAnmGenMessage(&m.mLowStanceAnm, ctx, "開始", 3.0f, 0);
    // Push Down
    daAlinkHIO_setAnmGenMessage(&m.mPushDownAnm, ctx, "滞空", 16.0f, 0);
    // Side Roll Movement Width
    ctx->genSlider("垂直速度", &m.mSideRollMovementWidth, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Front Roll
    daAlinkHIO_setAnmGenMessage(&m.mFrontRollAnm, ctx, "着地", 21.0f, 0);
    // Crouch
    daAlinkHIO_setAnmGenMessage(&m.mCrouchAnm, ctx, "引き", 49.0f, 0);
    // Prone Get Up
    daAlinkHIO_setAnmGenMessage(&m.mProneGetUpAnm, ctx, "引き抜き", 8.0f, 0);
    // Back Roll
    daAlinkHIO_setAnmGenMessage(&m.mBackRollAnm, ctx, "後転", 11.0f, 0);
    // Landing Side Roll Movement Width
    ctx->genSlider("後転垂直速度", &m.mLandingSideRollMovementWidth, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Landing Front-Back Movement Width
    ctx->genSlider("後転水平速度", &m.mLandingFrontBackMovementWidth, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Roll Over
    daAlinkHIO_setAnmGenMessage(&m.mRollOverAnm, ctx, "失敗", 84.0f, 1);
    // Fall Duration
    ctx->genSlider("停止時間", (s16*)&m.mFallDuration, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlAtDown_c::daAlinkHIO_wlAtDown_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtDown_c1 daAlinkHIO_wlAtDown_c0::m = {\n";
    m_len = 156;
    mp_src_data = (void*)&daAlinkHIO_wlAtDown_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 7;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlAtLock_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Start Radius
    ctx->genSlider("開始半径", &m.mStartRadius, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Radius
    ctx->genSlider("最大半径", &m.mMaxRadius, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Radius Acceleration
    ctx->genSlider("半径加速", &m.mRadiusAcceleration, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack Speed
    ctx->genSlider("攻撃速度", &m.mAttackSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Attack Radius Offset
    ctx->genSlider("攻撃半径オフセット", &m.mAttackRadiusOffset, 0.0f, 200.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Attack Radius
    ctx->genSlider("攻撃半径", &m.mAttackRadius, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Attack Height
    ctx->genSlider("攻撃高さ", &m.mAttackHeight, 0.0f, 250.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Auto Activation Time
    ctx->genSlider("自動発動時間", &m.mAutoActivationTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Front Landing Anm
    daAlinkHIO_setAnmGenMessage(&m.mFrontLandingAnm, ctx, "前着地", 14.0f, 0);
    // Rear Landing Anm
    daAlinkHIO_setAnmGenMessage(&m.mRearLandingAnm, ctx, "後着地", 19.0f, 0);
    // Gravity
    ctx->genSlider("重力", &m.mGravity, -15.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max Jump Height
    ctx->genSlider("最大ジャンプ高さ", &m.mMaxJumpHeight, 0.0f, 5000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max Jump Distance
    ctx->genSlider("最大ジャンプ距離", &m.mMaxJumpDistance, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_wlAtLock_c::daAlinkHIO_wlAtLock_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtLock_c1 daAlinkHIO_wlAtLock_c0::m = {\n";
    m_len = 84;
    mp_src_data = (void*)&daAlinkHIO_wlAtLock_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 10;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlAtBite_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Enemy Bite Throw Anm Speed
    ctx->genSlider("敵咥投げＡ速度", &m.mEnemyBiteThrowAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Enemy Bite Throw Interpolation
    ctx->genSlider("敵咥投げ補間", &m.mEnemyBiteThrowInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Enemy Bite Throw CF
    ctx->genSlider("敵咥投げＣＦ", &m.mEnemyBiteThrowCF, 0.0f, 45.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Bite Attach Time
    ctx->genSlider("噛付き時間", &m.mBiteAttachTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Bite Preparation
    daAlinkHIO_setAnmGenMessage(&m.mBitePreparationAnm, ctx, "噛準備", 5.0f, 0);
    // Bite Preparation Mid Start F
    ctx->genSlider("噛準備途中開始Ｆ", &m.mBitePreparationMidStartF, 0.0f, 5.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Bite Mid Anm Speed Min
    ctx->genSlider("噛中Ａ速度最小", &m.mBiteMidAnmSpeedMin, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Bite Mid Anm Speed Max
    ctx->genSlider("噛中Ａ速度最大", &m.mBiteMidAnmSpeedMax, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Bite Mid Anm Speed Acceleration
    ctx->genSlider("噛中Ａ速度加速", &m.mBiteMidAnmSpeedAcceleration, 0.0f, 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Bite Mid Anm Speed Deceleration
    ctx->genSlider("噛中Ａ速度減速", &m.mBiteMidAnmSpeedDeceleration, 0.0f, 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Hanging Bite Preparation
    daAlinkHIO_setAnmGenMessage(&m.mHangingBitePreparationAnm, ctx, "ぶら下がり噛準備", 31.0f, 0);
    // Hanging Bite Mid Anm Speed Min
    ctx->genSlider("ぶら下がり噛中Ａ速度最小", &m.mHangingBiteMidAnmSpeedMin, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Hanging Bite Mid Anm Speed Max
    ctx->genSlider("ぶら下がり噛中Ａ速度最大", &m.mHangingBiteMidAnmSpeedMax, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Hanging Bite Mid Anm Speed Acceleration
    ctx->genSlider("ぶら下がり噛中Ａ速度加速", &m.mHangingBiteMidAnmSpeedAcceleration, 0.0f, 1.0f,
                   0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Hanging Bite Mid Anm Speed Deceleration
    ctx->genSlider("ぶら下がり噛中Ａ速度減速", &m.mHangingBiteMidAnmSpeedDeceleration, 0.0f, 1.0f,
                   0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlAtBite_c::daAlinkHIO_wlAtBite_c() {
    static const char plabel[] = "const daAlinkHIO_wlAtBite_c1 daAlinkHIO_wlAtBite_c0::m = {\n";
    m_len = 92;
    mp_src_data = (void*)&daAlinkHIO_wlAtBite_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 12;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlAttack_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Horizontal
    ctx->genNode("左右", &mHorizontalAttack, 0, 0);
    // Scratch
    ctx->genNode("引掻き", &mScratchAttack, 0, 0);
    // Tail
    ctx->genNode("尻尾", &mTailAttack, 0, 0);
    // Jump
    ctx->genNode("ジャンプ", &mWlAtNjump, 0, 0);
    // Corkscrew (Kick)
    ctx->genNode("きりもみ（蹴り）", &mWlAtCjump, 0, 0);
    // Landing
    ctx->genNode("着地", &mWlAtLand, 0, 0);
    // Rotation
    ctx->genNode("回転", &mAtRoll, 0, 0);
    // Down
    ctx->genNode("ダウン", &mWlAtDown, 0, 0);
    // Lock
    ctx->genNode("ロック", &mWlAtLock, 0, 0);
    // Bite
    ctx->genNode("噛付き", &mWlAtBite, 0, 0);
    // Attack Preparation Interpolation
    ctx->genSlider("攻撃準備補間", &m.mReadyInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Charge Time
    ctx->genSlider("ため時間", &m.mChargeTime, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall Height
    ctx->genSlider("落下高さ", &m.mFallHeight, 0.0f, 500.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Fall Interpolation
    ctx->genSlider("落下補間", &m.mFallInterpolation, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Combo Duration Time
    ctx->genSlider("コンボ持続時間", &m.mComboDuration, 0, 300, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Stationary Attack Distance
    ctx->genSlider("その場攻撃距離", &m.mAttackRange, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump Back Landing
    daAlinkHIO_setAnmGenMessage(&m.mJumpBackLandAnm, ctx, "跳返着地", 59.0f, 1);
    // Jump Back Horizontal Speed
    ctx->genSlider("跳返水平速度", &m.mJumpBackSpeedH, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump Back Vertical Speed
    ctx->genSlider("跳返垂直速度", &m.mJumpBackSpeedV, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // A Attack Vertical Speed
    ctx->genSlider("Ａ攻撃垂直速度", &m.mJumpAttackSpeedV, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // A Attack Horizontal Speed
    ctx->genSlider("Ａ攻撃水平速度", &m.mJumpAttackSpeedH, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_wlAttack_c::daAlinkHIO_wlAttack_c()
    : mHorizontalAttack(0, 41.0f), mScratchAttack(1, 18.0f), mTailAttack(2, 42.0f), mAtRoll(),
      mWlAtNjump(), mWlAtCjump(), mWlAtLand(), mWlAtDown(), mWlAtLock(), mWlAtBite() {
    static const char plabel[] = "const daAlinkHIO_wlAttack_c1 daAlinkHIO_wlAttack_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_wlAttack_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 1;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 8;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

daAlinkHIO_wlAtBite_c::~daAlinkHIO_wlAtBite_c() {}
daAlinkHIO_wlAtLock_c::~daAlinkHIO_wlAtLock_c() {}
daAlinkHIO_wlAtDown_c::~daAlinkHIO_wlAtDown_c() {}
daAlinkHIO_wlAtLand_c::~daAlinkHIO_wlAtLand_c() {}
daAlinkHIO_wlAtCjump_c::~daAlinkHIO_wlAtCjump_c() {}
daAlinkHIO_wlAtNjump_c::~daAlinkHIO_wlAtNjump_c() {}
daAlinkHIO_wlAtRoll_c::~daAlinkHIO_wlAtRoll_c() {}
daAlinkHIO_wlAtWait_c::~daAlinkHIO_wlAtWait_c() {}

void daAlinkHIO_wlPoint_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle Anm Speed
    ctx->genSlider("待機Ａ速度", &m.mIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle Interpolation
    ctx->genSlider("待機補間", &m.mIdleInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Forward Jump
    daAlinkHIO_setAnmGenMessage(&m.mForwardJumpAnm, ctx, "前ジャンプ", 6.0f, 0);
    // Backward Jump
    daAlinkHIO_setAnmGenMessage(&m.mBackwardJumpAnm, ctx, "後ジャンプ", 7.0f, 0);
    // Aerial Interpolation
    ctx->genSlider("空中補間", &m.mAerialInterpolation, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Jump Speed
    ctx->genSlider("ジャンプ速度", &m.mJumpSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Landing
    daAlinkHIO_setAnmGenMessage(&m.mLandingAnm, ctx, "着地", 11.0f, 1);
    // Failure Jump Speed
    ctx->genSlider("失敗時ジャンプ速度", &m.mFailureJumpSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Tag Jump Speed
    ctx->genSlider("タグジャンプ速度", &m.mTagJumpSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Tag Landing
    daAlinkHIO_setAnmGenMessage(&m.mTagLandingAnm, ctx, "タグ着地", 14.0f, 1);
    // Lock Landing Ice Initial Speed
    ctx->genSlider("ロック着地氷初期速度", &m.mLockLandingIceInitSpeed, 0.0f, 50.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Lock Landing Ice Deceleration Rate
    ctx->genSlider("ロック着地氷減速率", &m.mLockLandingIceDecelRate, 0.0f, 1.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Lock Landing Ice Max Deceleration
    ctx->genSlider("ロック着地氷減速最大", &m.mLockLandingIceMaxDecel, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Lock Landing Ice Min Deceleration
    ctx->genSlider("ロック着地氷減速最小", &m.mLockLandingIceMinDecel, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Lock Jump Stop Time
    ctx->genSlider("ロックジャンプ停止時間", &m.mLockJumpStopTime, 0, 100, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
}

daAlinkHIO_wlPoint_c::daAlinkHIO_wlPoint_c() {
    static const char plabel[] = "const daAlinkHIO_wlPoint_c1 daAlinkHIO_wlPoint_c0::m = {\n";
    m_len = 124;
    mp_src_data = (void*)&daAlinkHIO_wlPoint_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 4;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 10;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wlChain_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Lowering start
    daAlinkHIO_setAnmGenMessage(&m.mLoweringStartAnm, ctx, "下がり始め", 49.0f, 0);
    // Wait Anm Speed
    ctx->genSlider("待機Ａ速度", &m.mWaitAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Wait Interp
    ctx->genSlider("待機補間", &m.mWaitInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Swing Anm Speed
    ctx->genSlider("振りＡ速度", &m.mSwingAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Swing Interp
    ctx->genSlider("振り補間", &m.mSwingInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Tension Wait Time
    ctx->genSlider("引張り待ち時間", &m.mTensionWaitTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Fall Speed
    ctx->genSlider("落下速度", &m.mFallSpeed, 0.0f, 30.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Release Wait Time
    ctx->genSlider("放し待ち時間", &m.mReleaseWaitTime, 0, 1000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Dig
    daAlinkHIO_setAnmGenMessage(&m.mDigAnm, ctx, "掘る", 81.0f, 0);
    // Dig fall
    daAlinkHIO_setAnmGenMessage(&m.mDigFallAnm, ctx, "掘落下", 47.0f, 0);
    // Sniff Anm Speed
    ctx->genSlider("嗅ぐＡ速度", &m.mSniffAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Sniff Interp
    ctx->genSlider("嗅ぐ補間", &m.mSniffInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

daAlinkHIO_wlChain_c::daAlinkHIO_wlChain_c() {
    static const char plabel[] = "const daAlinkHIO_wlChain_c1 daAlinkHIO_wlChain_c0::m = {\n";
    m_len = 92;
    mp_src_data = (void*)&daAlinkHIO_wlChain_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 7;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlSwim_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Start Height
    ctx->genSlider("開始高さ", &m.mStartHeight, 0.0f, 200.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Surfacing Height
    ctx->genSlider("浮上高さ", &m.mSurfacingHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Surfacing
    daAlinkHIO_setAnmGenMessage(&m.mSurfacingAnm, ctx, "浮上", 25.0f, 1);
    // Turn Rate
    ctx->genSlider("旋回率", &m.mTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Turn Max
    ctx->genSlider("旋回最大", &m.mTurnMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Turn Min
    ctx->genSlider("旋回最小", &m.mTurnMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle Turn Rate
    ctx->genSlider("待機旋回率", &m.mIdleTurnRate, 0, 100, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle Turn Max
    ctx->genSlider("待機旋回最大", &m.mIdleTurnMax, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle Turn Min
    ctx->genSlider("待機旋回最小", &m.mIdleTurnMin, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Speed
    ctx->genSlider("最高速度", &m.mMaxSpeed, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Deceleration
    ctx->genSlider("減速", &m.mDeceleration, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Max Fall Speed
    ctx->genSlider("最大落下速度", &m.mMaxFallSpeed, -100.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Surfacing Speed
    ctx->genSlider("最大浮上速度", &m.mMaxSurfacingSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Buoyancy
    ctx->genSlider("浮力", &m.mBuoyancy, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle Anm Speed
    ctx->genSlider("待機Ａ速度", &m.mIdleAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle Interp
    ctx->genSlider("待機補間", &m.mIdleInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Idle Up-Down Sway Amount
    ctx->genSlider("待機上下揺れ量", &m.mIdleUpDownSwayAmount, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Move Min Anm Speed
    ctx->genSlider("移動最小Ａ速度", &m.mMoveMinAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Move Max Anm Speed
    ctx->genSlider("移動最大Ａ速度", &m.mMoveMaxAnmSpeed, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Move Interp
    ctx->genSlider("移動補間", &m.mMoveInterp, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Climb Height
    ctx->genSlider("よじ登り高さ", &m.mClimbHeight, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Climb Below Water Surface
    ctx->genSlider("よじ登り水面下", &m.mClimbBelowWaterSurface, -100.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Max Speed (Weak)
    ctx->genSlider("最高速度（弱）", &m.mMaxSpeedWeak, 0.0f, 50.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Deceleration (Weak)
    ctx->genSlider("減速（弱）", &m.mDecelerationWeak, 0.0f, 20.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Idle Anm Speed (Weak)
    ctx->genSlider("待機Ａ速度（弱）", &m.mIdleAnmSpeedWeak, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Move Min Anm Speed (Weak)
    ctx->genSlider("移動最小Ａ速度（弱）", &m.mMoveMinAnmSpeedWeak, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Move Max Anm Speed (Weak)
    ctx->genSlider("移動最大Ａ速度（弱）", &m.mMoveMaxAnmSpeedWeak, 0.0f, 10.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Water Splash Idle
    daAlinkHIO_setAnmGenMessage(&m.mWaterSplashIdleAnm, ctx, "水弾き待機", 45.0f, 1);
    // Heavy Idle Up-Down Sway Amount
    ctx->genSlider("重時待機上下揺れ量", &m.mHeavyIdleUpDownSwayAmount, 0.0f, 10.0f, 0, NULL,
                   0xffff, 0xffff, 0x200, 0x18);
    // Heavy Buoyancy
    ctx->genSlider("重時浮力", &m.mHeavyBuoyancy, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Heavy Max Fall Speed
    ctx->genSlider("重時最大落下速度", &m.mHeavyMaxFallSpeed, -100.0f, 0.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Heavy Max Surfacing Speed
    ctx->genSlider("重時最大浮上速度", &m.mHeavyMaxSurfacingSpeed, 0.0f, 100.0f, 0, NULL, 0xffff,
                   0xffff, 0x200, 0x18);
    // Dash
    daAlinkHIO_setAnmGenMessage(&m.mDashAnm, ctx, "ダッシュ", 31.0f, 1);
}

daAlinkHIO_wlSwim_c::daAlinkHIO_wlSwim_c() {
    static const char plabel[] = "const daAlinkHIO_wlSwim_c1 daAlinkHIO_wlSwim_c0::m = {\n";
    m_len = 168;
    mp_src_data = (void*)&daAlinkHIO_wlSwim_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 3;
    m_u8_num = 0;
    m_s16_num = 6;
    m_f32_num = 24;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlGrab_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Pot Grab
    daAlinkHIO_setAnmGenMessage(&m.mPotGrabAnm, ctx, "壷咥え", 20.0f, 1);
    // Pot Release
    daAlinkHIO_setReverseAnmGenMessage(&m.mPotReleaseAnm, ctx, "壷置く", 20.0f, 1);
    // Bone Grab
    daAlinkHIO_setAnmGenMessage(&m.mBoneGrabAnm, ctx, "骨咥え", 14.0f, 1);
    // Bone Release
    daAlinkHIO_setReverseAnmGenMessage(&m.mBoneReleaseAnm, ctx, "骨置く", 14.0f, 1);
    // Throw
    daAlinkHIO_setAnmGenMessage(&m.mThrowAnm, ctx, "投げ", 19.0f, 1);
}

daAlinkHIO_wlGrab_c::daAlinkHIO_wlGrab_c() {
    static const char plabel[] = "const daAlinkHIO_wlGrab_c1 daAlinkHIO_wlGrab_c0::m = {\n";
    m_len = 100;
    mp_src_data = (void*)&daAlinkHIO_wlGrab_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 5;
    m_u8_num = 0;
    m_s16_num = 0;
    m_f32_num = 0;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

void daAlinkHIO_wlBall_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Full Body
    daAlinkHIO_setAnmGenMessage(&m.mFullBodyAnm, ctx, "全身", 64.0f, 1);
    // Neck
    daAlinkHIO_setAnmGenMessage(&m.mNeckAnm, ctx, "首", 54.0f, 0);
    // Life Orb Gravity
    ctx->genSlider("命珠重力", &m.mLifeOrbGravity, -10.0f, 0.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Life Orb Horizontal Speed
    ctx->genSlider("命珠水平速度", &m.mLifeOrbHorizSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Life Orb Vertical Speed
    ctx->genSlider("命珠垂直速度", &m.mLifeOrbVertSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // MP Consumption Time
    ctx->genSlider("ＭＰ消費時間", &m.mMpConsumptionTime, 0, 0xff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}

daAlinkHIO_wlBall_c::daAlinkHIO_wlBall_c() {
    static const char plabel[] = "const daAlinkHIO_wlBall_c1 daAlinkHIO_wlBall_c0::m = {\n";
    m_len = 56;
    mp_src_data = (void*)&daAlinkHIO_wlBall_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 2;
    m_u8_num = 0;
    m_s16_num = 2;
    m_f32_num = 3;
    f14 = 0;
    f18 = 0;
    f1c = 1;
    checkDataSize();
}

void daAlinkHIO_wolf_c::genMessage(JORMContext* ctx) {
    // Reset
    ctx->genButton("リセット", 0x4000003, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Move
    ctx->genNode("移動", &mWlMove, 0, 0);
    // Move (Weak)
    ctx->genNode("移動(弱)", &mWlMoveNoP, 0, 0);
    // Attention Move
    ctx->genNode("注目移動", &mWlAtnMove, 0, 0);
    // Side Jump
    ctx->genNode("横っ飛び", &mWlSideStep, 0, 0);
    // Back Jump
    ctx->genNode("バックジャンプ", &mWlBackJump, 0, 0);
    // Howl / Intimidate
    ctx->genNode("遠吠・威嚇", &mWlHowl, 0, 0);
    // Auto Jump
    ctx->genNode("オートジャンプ", &mWlAutoJump, 0, 0);
    // Push
    ctx->genNode("押し", &mWlPush, 0, 0);
    // Lie Down
    ctx->genNode("伏せ", &mWlLie, 0, 0);
    // Light
    ctx->genNode("ライト", &mLight, 0, 0);
    // Wall Grab
    ctx->genNode("壁掴み", &mWlWallHang, 0, 0);
    // Damage
    ctx->genNode("ダメージ", &mWlDamage, 0, 0);
    // Slope Slide
    ctx->genNode("坂滑り", &mWlSlide, 0, 0);
    // Rope Walk
    ctx->genNode("綱渡り", &mWlRope, 0, 0);
    // Attack
    ctx->genNode("攻撃", &mWlAttack, 0, 0);
    // Small Platform Jump
    ctx->genNode("小足場飛", &mWlPoint, 0, 0);
    // Chain, Dig
    ctx->genNode("鎖、掘る", &mWlChain, 0, 0);
    // Swim
    ctx->genNode("泳ぎ", &mWlSwim, 0, 0);
    // Grab
    ctx->genNode("咥え", &mWlGrab, 0, 0);
    // Life Orb
    ctx->genNode("命珠", &mWlBall, 0, 0);
    // Max Neck Turn H
    ctx->genSlider("首横向き最大", &m.mMaxNeckTurnH, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Neck Turn Up
    ctx->genSlider("首上向き最大", &m.mMaxNeckTurnUp, -0x7fff, 0, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Neck Turn Down
    ctx->genSlider("首下向き最大", &m.mMaxNeckTurnDown, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Max Tired Neck Turn H
    ctx->genSlider("疲れ首横向き最大", &m.mMaxTiredNeckTurnH, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max Tired Neck Turn Up
    ctx->genSlider("疲れ首上向き最大", &m.mMaxTiredNeckTurnUp, -0x7fff, 0, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Max Tired Neck Turn Down
    ctx->genSlider("疲れ首下向き最大", &m.mMaxTiredNeckTurnDown, 0, 0x7fff, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    // Senses Linger Time
    ctx->genSlider("五感余韻時間", &m.mSensesLingerTime, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Underwater Input Rate
    ctx->genSlider("沼キー率", &m.mUnderwaterInputRate, 0.0f, 1.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    // Light Drop R
    ctx->genSlider("光雫Ｒ", &m.mLightDropR, -0x40, 0x40, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Light Drop G
    ctx->genSlider("光雫Ｇ", &m.mLightDropG, -0x40, 0x40, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Light Drop B
    ctx->genSlider("光雫Ｂ", &m.mLightDropB, -0x40, 0x40, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
}

// NONMATCHING
daAlinkHIO_wolf_c::daAlinkHIO_wolf_c()
    : mWlMove(), mWlMoveNoP(), mWlAtnMove(), mWlSideStep(), mWlBackJump(), mWlHowl(), mWlAutoJump(),
      mWlPush(), mWlLie(), mLight(1), mWlWallHang(), mWlDamage(), mWlSlide(), mWlRope(),
      mWlAttack(), mWlPoint(), mWlChain(), mWlSwim(), mWlGrab(), mWlBall() {
    static const char plabel[] = "const daAlinkHIO_wolf_c1 daAlinkHIO_wolf_c0::m = {\n";
    m_len = 24;
    mp_src_data = (void*)&daAlinkHIO_wolf_c0::m;
    mp_data = (void*)&m;
    m_name_string = plabel;
    baseCopy();
    m_anm_num = 0;
    m_u8_num = 0;
    m_s16_num = 10;
    m_f32_num = 1;
    f14 = 0;
    f18 = 0;
    f1c = 0;
    checkDataSize();
}

daAlinkHIO_wlBall_c::~daAlinkHIO_wlBall_c() {}
daAlinkHIO_wlGrab_c::~daAlinkHIO_wlGrab_c() {}
daAlinkHIO_wlSwim_c::~daAlinkHIO_wlSwim_c() {}
daAlinkHIO_wlChain_c::~daAlinkHIO_wlChain_c() {}
daAlinkHIO_wlPoint_c::~daAlinkHIO_wlPoint_c() {}
daAlinkHIO_wlAttack_c::~daAlinkHIO_wlAttack_c() {}
daAlinkHIO_wlRope_c::~daAlinkHIO_wlRope_c() {}
daAlinkHIO_wlSlide_c::~daAlinkHIO_wlSlide_c() {}
daAlinkHIO_wlDamage_c::~daAlinkHIO_wlDamage_c() {}
daAlinkHIO_wlWallHang_c::~daAlinkHIO_wlWallHang_c() {}
daAlinkHIO_wlLie_c::~daAlinkHIO_wlLie_c() {}
daAlinkHIO_wlPush_c::~daAlinkHIO_wlPush_c() {}
daAlinkHIO_wlAutoJump_c::~daAlinkHIO_wlAutoJump_c() {}
daAlinkHIO_wlHowl_c::~daAlinkHIO_wlHowl_c() {}
daAlinkHIO_wlBackJump_c::~daAlinkHIO_wlBackJump_c() {}
daAlinkHIO_wlSideStep_c::~daAlinkHIO_wlSideStep_c() {}
daAlinkHIO_wlAtnMove_c::~daAlinkHIO_wlAtnMove_c() {}
daAlinkHIO_wlMoveNoP_c::~daAlinkHIO_wlMoveNoP_c() {}
daAlinkHIO_wlMove_c::~daAlinkHIO_wlMove_c() {}

void daAlinkHIO_c::jumpStateUpdate(const cXyz* param_0, const cXyz* param_1, f32 param_2) {
    char buf[68];

    f32 xDiff = param_0->x - param_1->x;
    f32 zDiff = param_0->z - param_1->z;
    JORMContext* ctx = attachJORMContext(8);
    ctx->startUpdateNode(this);
    sprintf(buf, "水平距離 %-8.3f", JMAFastSqrt(xDiff * xDiff + zDiff * zDiff));
    ctx->updateLabel(2, 0x4000006, buf);
    sprintf(buf, "最頂点   %-8.3f", param_2);
    ctx->updateLabel(2, 0x4000007, buf);
    ctx->endUpdateNode();
    releaseJORMContext(ctx);
}

void daAlinkHIO_c::genMessage(JORMContext* ctx) {
    // ------ Info Display ------
    ctx->genLabel("------情報表示------", 0x4000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Horizontal Distance
    ctx->genLabel("水平距離", 0x4000006, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Vertical Distance
    ctx->genLabel("最頂点", 0x4000007, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Reset
    ctx->genButton("位置", 0x4000002, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Save to File
    ctx->genButton("ファイルに保存", 0x4000004, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Load File
    ctx->genButton("ファイル読込", 0x4000005, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    // Basic
    ctx->genNode("基本", &mBasic, 0, 0);
    // Move
    ctx->genNode("移動", &mMove, 0, 0);
    // Atn Move
    ctx->genNode("注目移動", &mAtnMove, 0, 0);
    // No Act Atn Move
    ctx->genNode("空注目移動", &mNoActAtnMove, 0, 0);
    // Front Roll
    ctx->genNode("前転", &mFrontRoll, 0, 0);
    // Back Jump
    ctx->genNode("バク転", &mBackJump, 0, 0);
    // Side Step
    ctx->genNode("横っ飛び", &mSideStep, 0, 0);
    // Slide
    ctx->genNode("坂滑り", &mSlide, 0, 0);
    // Cut
    ctx->genNode("剣振り", &mCut, 0, 0);
    // Guard
    ctx->genNode("ガード関連", &mGuard, 0, 0);
    // Crouch
    ctx->genNode("しゃがみ関連", &mCrouch, 0, 0);
    // Auto Jump
    ctx->genNode("オートジャンプ", &mAutoJump, 0, 0);
    // Wall Hang
    ctx->genNode("壁掴み関連", &mWallHang, 0, 0);
    // Pushpull
    ctx->genNode("押し引き", &mPushpull, 0, 0);
    // Damage
    ctx->genNode("ダメージ", &mDamage, 0, 0);
    // Horse
    ctx->genNode("馬", &mHorse, 0, 0);
    // Canoe
    ctx->genNode("カヌー", &mCanoe, 0, 0);
    // Item
    ctx->genNode("アイテム", &mItem, 0, 0);
    // Ladder, Wall Climb
    ctx->genNode("はしご、壁登", &mLadder, 0, 0);
    // Ceiling Hang
    ctx->genNode("天井ぶら下がり", &mRoofHang, 0, 0);
    // Grab
    ctx->genNode("持上げ", &mGrab, 0, 0);
    // Swim
    ctx->genNode("泳ぎ", &mSwim, 0, 0);
    // Wolf
    ctx->genNode("狼", &mWolf, 0, 0);
}

void daAlinkHIO_c::readFileData(char* param_0) {
    int src;
    daAlinkHIO_data_c* top;
    src = 0;

    for (top = l_HIO_top; top != NULL; top = top->mpNext) {
        cLib_memCpy(top->mp_src_data, param_0 + src, top->m_len);
        top->update();
        src += top->m_len;
    }
}

size_t daAlinkHIO_c::makeFileOutData(char* param_0, char* param_1) {
    size_t sVar1;
    int iVar2;
    daAlinkHIO_data_c* top;
    iVar2 = 0;

    for (top = l_HIO_top; top != NULL; top = top->mpNext) {
        cLib_memCpy(param_0 + iVar2, top->mp_data, top->m_len);
        iVar2 += top->m_len;
    }

    param_0[iVar2] = 10;
    iVar2++;
    top = l_HIO_top;
    *param_1 = 0;

    for (; top != NULL; top = top->mpNext) {
        top->setStructData(param_1);
    }

    cLib_memCpy(param_0 + iVar2, param_1, strlen(param_1));
    return iVar2 + strlen(param_1);
}

void daAlinkHIO_c::listenPropertyEvent(const JORPropertyEvent* event) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    JORReflexible::listenPropertyEvent(event);
    JORFile aJStack_168;

    switch ((u32)event->id) {
    case 0x4000002: {
        OS_REPORT("X = %f, Y = %f, Z = %f,\n", player->current.pos.x, player->current.pos.y,
                  player->current.pos.z);
        OS_REPORT("SHAPE_ANGLE = %x, %d\n", player->shape_angle.y, player->shape_angle.y);
        OS_REPORT("CURRENT_ANGLE = %x, %d\n", player->current.angle.y, player->current.angle.y);

        // "%s, Room No. %d, Start Position %d, Layer %d"
        OS_REPORT("%s, 部屋Ｎｏ %d, 開始位置 %d, レイヤー %d\n", dComIfGp_getStartStageName(),
                  fopAcM_GetRoomNo(player), dComIfGs_getStartPoint(),
                  dComIfG_play_c::getLayerNo(0));
        break;
    }
    case 0x4000004: {
        if (aJStack_168.open(6, "すべてのファイル(*.*)\0*.*\0", 0, 0, 0) != 0) {
            char* buffer1 = new (0x20) char[0x10000];
            char* buffer2 = new (0x20) char[0x10000];

            if (buffer1 != NULL && buffer2 != NULL) {
                size_t uVar5 = makeFileOutData(buffer1, buffer2);
                aJStack_168.writeData(buffer1, uVar5);

                // Write area %x\n
                OS_REPORT("書き込み領域%x\n", uVar5);

                if (uVar5 > 0x20000) {
                    // Memory corruption has occurred!!!!!
                    OS_REPORT("メモリ破壊が起こりました！！！！！\n");
                }

                delete[] buffer1;
                delete[] buffer2;

            } else {
                if (buffer1 != NULL) {
                    delete[] buffer1;
                }

                if (buffer2 != NULL) {
                    delete[] buffer2;
                }

                // Could not secure the write area
                OS_REPORT("書き込み用領域確保できませんでした\n");
            }

            aJStack_168.close();
        } else {
            // Cannot open file
            OS_REPORT("ファイルが開けません\n");
        }
        break;
    }
    case 0x4000005: {
        if (aJStack_168.open(1, "すべてのファイル(*.*)\0*.*\0", (char*)0x0, (char*)0x0, (char*)0x0) != 0) {
            char* buffer1 = new (0x20) char[0x10000];

            if (buffer1 != NULL) {
                aJStack_168.readData(buffer1, 0);
                readFileData(buffer1);
                delete[] buffer1;
            } else {
                // Could not secure the read area
                OS_REPORT("読み込み用領域確保できませんでした\n");
            }

            aJStack_168.close();
        } else {
            // The file cannot be opened
            OS_REPORT("ファイルが開けません\n");
        }
    }
    }
}

#endif
