#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_meter_HIO.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "m_Do/m_Do_ext.h"

dMeter_menuHIO_c::dMeter_menuHIO_c() {
    mGameover = 70;
}

#if DEBUG
void dMeter_menuHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawCollectHIO_c::dMeter_drawCollectHIO_c() {
    mUnselectItemScale = 1.0f;
    mSelectItemScale = 1.5f;
    mUnselectSaveOptionScale = 1.0f;
    mSelectSaveOptionScale = 1.1f;
    mBlueSmokePosX = -12.0f;
    mBlueSmokePosY = 1.3f;
    mBlueSmokeScale = 1.3f;
    mBlueSmokeAlpha = 0.8f;
    mMaskMirrorPos.set(0.0f, 0.0f, 0.0f);
    mMaskMirrorAngle.set(0, 0, 0);
    field_0x9a = false;
    mMaskMirrorScale = 0.0f;
    mMaskMirrorAnimSpeed = 1.0f;
    mHeartVesselPosX = -5.0f;
    mHeartVesselPosY = 4.0f;
    mHeartVesselScale = 0.9f;
    mHeartPiecePosX = 0.0f;
    mHeartPiecePosY = 0.0f;
    mHeartPieceScale = 1.0f;
    mButtonDebugON = false;
    mButtonAPosX = -22.3f;
    mButtonAPosY = 24.9f;
    mButtonAScale = 1.3f;
    mButtonBPosX = -22.3f;
    mButtonBPosY = 25.0f;
    mButtonBScale = 1.3f;
    mButtonATextPosX = -24.9f;
    mButtonATextPosY = 24.9f;
    mButtonATextScale = 1.1f;
    mButtonBTextPosX = -27.5f;
    mButtonBTextPosY = 26.0f;
    mButtonBTextScale = 1.1f;
    mColorDebugON = false;
    mVesselBack[VESSEL_HEART].set(0, 0, 0, 0);
    mVesselFront[VESSEL_HEART].set(250, 250, 210, 220);
    mVesselBack[VESSEL_DECOR].set(200, 190, 100, 0);
    mVesselFront[VESSEL_DECOR].set(200, 190, 100, 175);
}

#if DEBUG
void dMeter_drawCollectHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawOptionHIO_c::dMeter_drawOptionHIO_c() {
    mBarScale[0] = 1.02f;
    mBarScale[1] = 0.98f;
    mArrowSize = 0.3f;

    mSelectNameColor.set(255, 255, 255, 255);
    mSelectColor.set(255, 200, 0, 255);
    mUnselectColor.set(180, 180, 150, 255);

    mWindowPosX = 0.0f;
    mWindowPosY = 0.0f;
    mWindowScale = 1.0f;

    for (int i = 0; i < 4; i++) {
        mOptionTypeBGPosX[i] = 0.0f;
        mOptionTypeBGPosY[i] = 0.0f;
    }

    mOpenFrames = 10;
    mCloseFrames = 10;

    mDebug = false;
    mBackgroundAlpha = 150;
    mBackgroundPosY = 45.0f;

    mArrowOffsetX = 38.0f;
    mArrowOffsetX_4x3 = 4.0f;
}

#if DEBUG
void dMeter_drawOptionHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawLetterHIO_c::dMeter_drawLetterHIO_c() {
    for (int i = 0; i < 2; i++) {
        mOpenFrame[i] = 10;
        mCloseFrame[i] = 10;
    }

    mLetterNum = 0;
    mDebugON = false;

    mUnselectPageIconAlpha = 255;

    mSelectBarScale = 1.02f;
    mUnselectBarScale = 0.98f;

    mWindowPosX = 0.0f;
    mWindowPosY = -20.0f;
    mWindowScale = 1.0f;

    mLetterWindowPosX = 0.0f;
    mLetterWindowPosY = 0.0f;
    mLetterWindowScale = 1.0f;
    mWindowBGAlpha = 150;

    mLetterLinePosX = 0.0f;
    mLetterLinePosY = 0.0f;
    mLetterLineAlpha = 130;

    mSelectPageIconBack.set(0, 0, 0, 0);
    mSelectPageIconFront.set(255, 255, 255, 255);
    mSelectBarBack.set(105, 95, 55, 255);
    mSelectBarFront.set(200, 180, 135, 255);
    mSelectTextBack.set(31, 24, 12, 0);
    mSelectTextFront.set(255, 255, 200, 255);
    mLetterTextBack.set(0, 0, 0, 0);
    mLetterTextFront.set(65, 55, 30, 255);
}

#if DEBUG
void dMeter_drawLetterHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawFishingHIO_c::dMeter_drawFishingHIO_c() {
    static f32 const fipaScale[6] = {0.98f, 1.0f, 1.0f, 1.0f, 0.95f, 1.1f};
    static f32 const fipaPosX[6] = {1.3f, 6.6f, 0.0f, 3.96f, 4.0f, 27.5f};
    static f32 const fipaPosY[6] = {-9.2f, -27.5f, -11.7f, -9.2f, -30.1f, -17.9f};
    static f32 const finaScale[6] = {0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f};

#if PLATFORM_SHIELD
    static f32 const finaPosX[6] = {0.0f, -4.0f, 0.0f, 0.0f, 0.0f, -4.0f};
#else
    static f32 const finaPosX[6] = {-25.0f, -4.0f, 0.0f, 0.0f, 0.0f, -4.0f};
#endif

#if PLATFORM_SHIELD
    static f32 const finaPosY[6] = {-14.4f, -40.6, -24.0f, -11.7f, -37.9f, -13.0f};
#else
    static f32 const finaPosY[6] = {-16.0f, -40.6, -24.0f, -11.7f, -37.9f, -13.0f};
#endif

    static f32 const filiScale[6] = {0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f};

#if PLATFORM_SHIELD
    static f32 const filiPosX[6] = {0.0f, -8.0f, 9.2f, 9.2f, 9.2f, -8.0f};
#else
    static f32 const filiPosX[6] = {-28.0f, -8.0f, 9.2f, 9.2f, 9.2f, -8.0f};
#endif

    static f32 const filiPosY[6] = {-6.6f, -14.0f, -14.4f, 24.9f, -11.7f, 24.9f};
    static f32 const bboxScale[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    static f32 const bboxPosX[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const bboxPosY[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const rboxScale[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    static f32 const rboxPosX[6] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const rboxPosY[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const finfoScale[6] = {0.87f, 0.87f, 0.88f, 0.87f, 0.87f, 0.87f};

#if VERSION == VERSION_GCN_JPN
    static f32 const finfoPosX[6] = {-27.0f, 0.0f, -12.0f, 0.0f, -12.0f, -32.8f};
#else
    static f32 const finfoPosX[6] = {-17.0f, 0.0f, -14.0f, 0.0f, -12.0f, -32.8f};
#endif

    static f32 const finfoPosY[6] = {6.6f, 0.0f, 0.0f, 0.0f, 1.3f, 0.0f};
    static f32 const fishnScale[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

#if VERSION == VERSION_GCN_JPN
    static f32 const fishnPosX[6] = {12.0f, -20.0f, 1.3f, 6.6f, 14.0f, -6.6f};
#else
    static f32 const fishnPosX[6] = {4.0f, -20.0f, 1.3f, 6.6f, 1.3f, 9.2f};
#endif

    static f32 const fishnPosY[6] = {-72.1f, 129.6f, 0.0f, 6.6f, 32.8f, 11.7f};

    mOpenFrames = 10;
    mCloseFrames = 10;
    mDebug = false;

    for (int i = 0; i < 6; i++) {
        mFishNum[i] = 0;
        mFishSize[i] = 0;
        mFishIconScale[i] = fipaScale[i];
        mFishIconPosX[i] = fipaPosX[i];
        mFishIconPosY[i] = fipaPosY[i];
        mFishNameScale[i] = finaScale[i];
        mFishNamePosX[i] = finaPosX[i];
        mFishNamePosY[i] = finaPosY[i];
        mFishLineScale[i] = filiScale[i];
        mFishLinePosX[i] = filiPosX[i];
        mFishLinePosY[i] = filiPosY[i];
        mFishSizeScale[i] = bboxScale[i];
        mFishSizePosX[i] = rboxPosX[i];
        mFishSizePosY[i] = rboxPosY[i];
        mFishCountScale[i] = rboxScale[i];
        mFishCountPosX[i] = rboxPosX[i];
        mFishCountPosY[i] = rboxPosY[i];
        mFishCountSizeScale[i] = finfoScale[i];
        mFishCountSizePosX[i] = finfoPosX[i];
        mFishCountSizePosY[i] = finfoPosY[i];
        mFishInfoScale[i] = fishnScale[i];
        mFishInfoPosX[i] = fishnPosX[i];
        mFishInfoPosY[i] = fishnPosY[i];
    }
    mScale[0] = 0.83f;
    mPosX[0] = 5.0f;
    mPosY[0] = -22.2f;

    mScale[1] = 0.83f;
    mPosX[1] = -189.5f;
    mPosY[1] = -3.95f;
    mCaughtNumPosX_4x3 = -188.0f;
}

#if DEBUG
void dMeter_drawFishingHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawInsectHIO_c::dMeter_drawInsectHIO_c() {
    mOpenFrame = 10;
    mCloseFrame = 10;

    mType = DEFAULT;
    mDebugON = false;

    mUnselectInsectScale = 1.0f;
    mSelectInsectScale = 1.5f;

    mDescWindowPosX = 0.0f;
    mDescWindowPosY = 77.0f;

    mGiveOptionPosX_4x3 = 0.0f;
    mGiveOptionPosY_4x3 = 43.0f;

    mConfirmOptionPosX_4x3 = 0.0f;
    mConfirmOptionPosY_4x3 = 119.0f;

    mGiveOptionPosX = 20.0f;
    mGiveOptionPosY = 43.0f;

    mConfirmOptionPosX = -34.0f;
    mConfirmOptionPosY = 119.0f;
}

#if DEBUG
void dMeter_drawInsectHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawCalibrationHIO_c::dMeter_drawCalibrationHIO_c() {
    mOpenFrames = 10;
    mCloseFrames = 10;
    mDebug = false;
    mFlashFrameNum = 30;
    mCircleScale = 1.0f;
    mCircleScale2 = 0.75f;
}

#if DEBUG
void dMeter_drawCalibrationHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawSkillHIO_c::dMeter_drawSkillHIO_c() {
    for (int i = 0; i < 2; i++) {
        mOpenFrame[i] = 10;
        mCloseFrame[i] = 10;
    }

    mDebug = false;
    field_0x58 = 0xFF;

    mSelectBarScale = 1.02f;
    mUnselectBarScale = 0.98f;

    mSkillDescPosX = 0.0f;
    mSkillDescPosY = 80.0f;
    mSkillDescScale = 0.95f;
    mWindowBGalpha = 150;

    mWindowLinePosX = 0.0f;
    mWindowLinePosY = 0.0f;
    mWindowLineAlpha = 130;

    mSelectScrollIconBlack.set(0, 0, 0, 0);
    mSelectScrollIconWhite.set(255, 255, 255, 255);
    mUnselectScrollIconBlack.set(0, 0, 0, 0);
    mUnselectScrollIconWhite.set(180, 180, 180, 100);
    mSelectBarBlack.set(105, 95, 55, 255);
    mSelectBarWhite.set(200, 180, 135, 255);
    mSelectTextBlack.set(31, 24, 12, 0);
    mSelectTextWhite.set(255, 255, 200, 255);

    mSkillTitlePosX = 0.0f;
    mSkillTitlePosY = 0.0f;
    mSkillTitleScale = 1.1f;
}

#if DEBUG
void dMeter_drawSkillHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawEmpButtonHIO_c::dMeter_drawEmpButtonHIO_c() {
    mPikariScale = 2.5f;

    mPikariFrontInner.r = 255;
    mPikariFrontInner.g = 255;
    mPikariFrontInner.b = 255;
    mPikariFrontInner.a = 185;

    mPikariFrontOuter.r = 0;
    mPikariFrontOuter.g = 155;
    mPikariFrontOuter.b = 0;
    mPikariFrontOuter.a = 0;

    mPikariBackInner.r = 255;
    mPikariBackInner.g = 255;
    mPikariBackInner.b = 255;
    mPikariBackInner.a = 87;

    mPikariBackOuter.r = 0;
    mPikariBackOuter.g = 115;
    mPikariBackOuter.b = 0;
    mPikariBackOuter.a = 0;

    mPikariAnimSpeed = 0.772705f;
    mPikariFastScale = 3.0f;

    mPikariFastFrontInner.r = 255;
    mPikariFastFrontInner.g = 255;
    mPikariFastFrontInner.b = 255;
    mPikariFastFrontInner.a = 255;

    mPikariFastFrontOuter.r = 220;
    mPikariFastFrontOuter.g = 220;
    mPikariFastFrontOuter.b = 215;
    mPikariFastFrontOuter.a = 0;

    mPikariFastBackInner.r = 255;
    mPikariFastBackInner.g = 255;
    mPikariFastBackInner.b = 190;
    mPikariFastBackInner.a = 255;

    mPikariFastBackOuter.r = 200;
    mPikariFastBackOuter.g = 200;
    mPikariFastBackOuter.b = 210;
    mPikariFastBackOuter.a = 0;

    mPikariFastAnimSpeed = 2.0f;

    mPikariListenScale = 1.0f;
    mPikariListenFrontInner.set(255, 255, 255, 185);
    mPikariListenFrontOuter.set(0, 155, 0, 0);
    mPikariListenBackInner.set(255, 255, 255, 87);
    mPikariListenBackOuter.set(0, 115, 0, 0);
    mPikariListenAnimSpeed = 0.3f;

    mPikariRepeatHitScale = 2.5f;
    mPikariRepeatHitFrontInner.set(255, 255, 255, 185);
    mPikariRepeatHitFrontOuter.set(0, 155, 0, 0);
    mPikariRepeatHitBackInner.set(255, 255, 255, 87);
    mPikariRepeatHitBackOuter.set(0, 115, 0, 0);
    mPikariRepeatHitAnimSpeed = 1.1f;
    mRepeatHitScale = 1.15f;
    mRepeatHitFrameNum = 4;

    for (int i = 0; i < 17; i++) {
        mDebugDisplayButton[0] = 0;
    }

    #if PLATFORM_SHIELD
    mDisplaySpace = 70.0f;
    #else
    mDisplaySpace = 35.0f;
    #endif

    mEmpButtonPosX = 0.0f;
    mEmpButtonPosY = 175.0f;
    mEmpButtonScale = 1.0f;

    mButtonAPosX = 0.0f;
    mButtonAPosY = 0.0f;
    mButtonAScale = 1.0f;

    mButtonBPosX = 0.0f;
    mButtonBPosY = 0.0f;
    mButtonBScale = 1.0f;

    mRButtonPosX = 0.0f;
    mRButtonPosY = 0.0f;
    mRButtonScale = 1.0f;

    mButtonZPosX = 0.0f;
    mButtonZPosY = 0.0f;
    mButtonZScale = 1.0f;

    m3DButtonPosX = 0.0f;
    m3DButtonPosY = 0.0f;
    m3DButtonScale = 1.0f;

    mCButtonPosX = 0.0f;
    mCButtonPosY = 0.0f;
    mCButtonScale = 1.0f;

    mSButtonPosX = 0.0f;
    mSButtonPosY = -392.0f;
    mSButtonScale = 0.9f;

    mButtonXPosX = 0.0f;
    mButtonXPosY = 0.0f;
    mButtonXScale = 1.0f;

    mButtonYPosX = 0.0f;
    mButtonYPosY = 0.0f;
    mButtonYScale = 1.0f;

    mNunchukPosX = 0.0f;
    mNunchukPosY = 0.0f;
    mNunchukScale = 1.0f;

    mWiimotePosX = 0.0f;
    mWiimotePosY = 0.0f;
    mWiimoteScale = 1.0f;

    mWiimoteLRPosX = 0.0f;
    mWiimoteLRPosY = 0.0f;
    mWiimoteLRScale = 1.0f;

    mWiimoteAPosX = 0.0f;
    mWiimoteAPosY = 0.0f;
    mWiimoteAScale = 1.0f;

    m3DBButtonPosX = 0.0f;
    m3DBButtonPosY = 0.0f;
    m3DBButtonScale = 1.0f;

    mNunchukWiimotePosX = 0.0f;
    mNunchukWiimotePosY = 0.0f;
    mNunchukWiimoteScale = 1.0f;

    mNunchukWiimoteRealPosX = 0.0f;
    mNunchukWiimoteRealPosY = 0.0f;
    mNunchukWiimoteRealScale = 1.0f;

    mNunchukRealPosX = 0.0f;
    mNunchukRealPosY = 0.0f;
    mNunchukRealScale = 1.0f;

    mNunchukAttackPosX = 0.0f;
    mNunchukAttackPosY = 0.0f;
    mNunchukAttackScale = 1.0f;

    mAButtonBPosX = 0.0f;
    mAButtonBPosY = 0.0f;
    mAButtonBScale = 1.0f;

    mNunchukButtonZPosX = 0.0f;
    mNunchukButtonZPosY = 0.0f;
    mNunchukButtonZScale = 1.0f;

    mNunchukCButtonPosX = 0.0f;
    mNunchukCButtonPosY = 0.0f;
    mNunchukCButtonScale = 1.0f;

    mBottleIconPosX = 0.0f;
    mBottleIconPosY = 0.0f;
    mBottleIconScale = 1.0f;

    mViewChangePosX = -26.0f;
    mViewChangePosY = 67.0f;
    mViewChangeScale = 1.0f;

    mText1PosX = mText2PosX = 2.0f;
    mText1PosY = mText2PosY = 3.0f;
    mText1Scale = mText2Scale = 1.0f;

    mItemPosX = 12.0f;
    mItemPosY = 0.0f;
    mItemScale = 1.0f;

    mMidnaIconPosX = 0.0f;
    mMidnaIconPosY = 0.0f;
    mMidnaIconScale = 1.0f;

    mHawkeyeDebugON = false;
    mHawkeyePosX = 0.0f;
    mHawkeyePosY = 0.0f;
    for (int i = 0; i < 3; i++) {
        mHawkeyeTextPosY[i] = 0.0f;
    }
    mHawkeyeTextPosX = 0.0f;
    mHawkeyePlusPosX = 17.0f;
    mHawkeyeMinusPosX = -17.0f;

    mHawkeyeZoomDebugON = false;
    mHawkeyeZoomPosX = 0.0f;
    mHawkeyeZoomPosY = 25.0f;
    mHawkeyeZoomScale = 0.8f;
    mHawkeyeZoomScale_4x3 = 0.9f;
    for (int i = 0; i < 3; i++) {
        mHawkeyeIconPosX[i] = 0.0f;
        mHawkeyeIconScale[i] = 1.0f;
    }
    mHawkeyeTextIconPosY = -6.0f;
    mHawkeyeZoomTextPosY = -8.0f;
    mHawkeyeIconPosY = 0.0f;
    mHawkeyeTextIconPosY_4x3 = 12.0f;
    mHawkeyeTextPosY_4x3 = -8.0f;
    mHawkeyeIconPosY_4x3 = 0.0f;

    mPointerONBlack[0].set(200, 200, 200, 0);
    mPointerONWhite[0].set(255, 255, 0, 255);
    mPointerONBlack[1].set(255, 145, 100, 0);
    mPointerONWhite[1].set(255, 145, 100, 255);
    mPointerOFFBlack.set(0, 0, 0, 0);
    mPointerOFFWhite.set(0, 0, 0, 0);

    mPointerONScale = 1.1f;
    mPointerOFFScale = 0.9f;
    mPointerONAnimFrame = 30;
}

#if DEBUG
void dMeter_drawEmpButtonHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawMiniGameHIO_c::dMeter_drawMiniGameHIO_c() {
    field_0x8[0] = 1.0f;
    field_0x14[0] = 0.0f;
    field_0x20[0] = 0.0f;
    field_0x2c[0] = 1.0f;

    mTimerSizeX[0] = 1.2f;
    mTimerSizeY[0] = 0.9f;
    mTimerPosX[0] = 207.0f;
    mTimerPosY[0] = -43.0f;
    mTimerAlpha[0] = 0.5f;
    mTimerSizeX_4x3 = 1.2f;
    mTimerSizeY_4x3 = 0.9f;
    mTimerPosX_4x3 = 207.0f;
    mTimerPosY_4x3 = -43.0f;

    mCounterSizeX[0] = 1.1f;
    mCounterSizeY[0] = 0.9f;
    mCounterPosX[0] = 17.0f;
    mCounterPosY[0] = 317.0f;
    mCounterAlpha[0] = 0.7f;

    mIconSizeX[0] = 0.75f;
    mIconSizeY[0] = 0.75f;
    mIconPosX[0] = -275.0f;
    mIconPosX_4x3 = -275.0f;
    mIconPosY[0] = 76.0f;
    mIconAlpha[0] = 0.9f;

    field_0x8[1] = 1.0f;
    field_0x14[1] = -22.0f;
    mRiverRideIconPosX_4x3 = -30.0f;
    field_0x20[1] = 0.0f;
    field_0x2c[1] = 1.0f;

    mTimerSizeX[1] = 1.4f;
    mTimerSizeY[1] = 1.0f;
    mTimerPosX[1] = 198.0f;
    mTimerPosY[1] = 160.0f;
    mTimerAlpha[1] = 0.5f;

    mCounterSizeX[1] = 1.1f;
    mCounterSizeY[1] = 0.95f;
    mCounterPosX[1] = 28.0f;
    mCounterPosY[1] = -18.0f;
    mCounterAlpha[1] = 0.7f;

    mIconSizeX[1] = 0.9f;
    mIconSizeY[1] = 0.9f;
    mIconPosX[1] = 23.0f;
    mIconPosY[1] = -16.0f;
    mIconAlpha[1] = 0.9f;

    field_0x8[2] = 1.0f;
    field_0x14[2] = 0.0f;
    field_0x20[2] = 0.0f;
    field_0x2c[2] = 1.0f;
    mTimerSizeX[2] = 1.4f;
    mTimerSizeY[2] = 1.0f;
    mTimerPosX[2] = 198.0f;
    mTimerPosY[2] = -30.0f;
    mTimerAlpha[2] = 0.5f;

    mCounterSizeX[2] = 1.1f;
    mCounterSizeY[2] = 0.95f;
    mCounterPosX[2] = 267.0f;
    mCounterPosY[2] = 288.0f;
    mCounterAlpha[2] = 0.7f;

    mIconSizeX[2] = 0.75f;
    mIconSizeY[2] = 0.75f;
    mIconPosX[2] = -29.0f;
    mRiderGameIconPosX_4x3 = -26.0f;
    mIconPosY[2] = 50.0f;
    mIconAlpha[2] = 0.9f;

    mReadyGo = 60;
    field_0x15a = 10;

    mGetInTextSizeX = 1.0f;
    mGetInTextSizeY = 1.0f;
    mGetInTextPosX = 0.0f;
    mGetInTextPosY = 0.0f;
    mGetInTextAnimSpeed = 1.0f;
    mGetInTextWaitFrames = 10;
    mGetInTextAlphaFrames = 10;
    mGetInPikariAppearFrames = 30;
    mGetInPikariScale = 1.8f;

    mGetInPikariFrontInner.r = 255;
    mGetInPikariFrontInner.g = 255;
    mGetInPikariFrontInner.b = 167;
    mGetInPikariFrontInner.a = 80;

    mGetInPikariFrontOuter.r = 220;
    mGetInPikariFrontOuter.g = 110;
    mGetInPikariFrontOuter.b = 30;
    mGetInPikariFrontOuter.a = 0;

    mGetInPikariBackInner.r = 255;
    mGetInPikariBackInner.g = 255;
    mGetInPikariBackInner.b = 190;
    mGetInPikariBackInner.a = 90;

    mGetInPikariBackOuter.r = 170;
    mGetInPikariBackOuter.g = 110;
    mGetInPikariBackOuter.b = 0;
    mGetInPikariBackOuter.a = 0;

    mGetInPikariAnimSpeed = 0.6f;
    mGetInTextLocation = 0;

    mStartPikariAppearFrames = 30;
    mStartPikariScale = 1.8f;

    mStartPikariFrontInner.r = 255;
    mStartPikariFrontInner.g = 255;
    mStartPikariFrontInner.b = 167;
    mStartPikariFrontInner.a = 80;

    mStartPikariFrontOuter.r = 220;
    mStartPikariFrontOuter.g = 110;
    mStartPikariFrontOuter.b = 30;
    mStartPikariFrontOuter.a = 0;

    mStartPikariBackInner.r = 255;
    mStartPikariBackInner.g = 255;
    mStartPikariBackInner.b = 190;
    mStartPikariBackInner.a = 90;

    mStartPikariBackOuter.r = 170;
    mStartPikariBackOuter.g = 110;
    mStartPikariBackOuter.b = 0;
    mStartPikariBackOuter.a = 0;

    mStartPikariAnimSpeed = 0.6f;
    mStartPikariDisplayFrames = 30;

    mReadyFightTextSizeX = 1.0f;
    mReadyFightTextSizeY = 1.0f;
    mReadyFightTextPosX = 0.0f;
    mReadyFightTextPosY = 0.0f;
    mReadyFightTextAnimSpeed = 1.0f;
    mReadyFightTextWaitFrames = 10;
    field_0x172 = 10;
    mReadyFightPikariAppearFrames = 30;
    mReadyFightPikariScale = 1.8f;

    mReadyFightPikariFrontInner.r = 255;
    mReadyFightPikariFrontInner.g = 255;
    mReadyFightPikariFrontInner.b = 167;
    mReadyFightPikariFrontInner.a = 80;

    mReadyFightPikariFrontOuter.r = 220;
    mReadyFightPikariFrontOuter.g = 110;
    mReadyFightPikariFrontOuter.b = 30;
    mReadyFightPikariFrontOuter.a = 0;

    mReadyFightPikariBackInner.r = 255;
    mReadyFightPikariBackInner.g = 255;
    mReadyFightPikariBackInner.b = 190;
    mReadyFightPikariBackInner.a = 90;

    mReadyFightPikariBackOuter.r = 170;
    mReadyFightPikariBackOuter.g = 110;
    mReadyFightPikariBackOuter.b = 0;
    mReadyFightPikariBackOuter.a = 0;

    mReadyFightPikariAnimSpeed = 0.6f;
}

#if DEBUG
void dMeter_drawMiniGameHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawLightDropHIO_c::dMeter_drawLightDropHIO_c() {
    mVesselScale = 0.76f;
    mVesselPosX = -18.0f;
    mVesselPosY = 11.0f;
    mVesselPosX_4x3 = -20.0f;
    mVesselPosY_4x3 = 6.6f;
    mVesselAlpha[0] = 1.0f;

    mVesselTalkScale = 0.76f;
    mVesselTalkPosX = 20.0f;
    mVesselTalkPosY = -35.0f;
    mVesselAlpha[1] = 0.5f;

    mDropGetScale = 2.5f;
    mDropGetScaleAnimFrameNum = 20;
    mDropScale = 1.0f;
    mPikariInterval = 1;
    field_0x54 = 0xFFF1;
    mVesselAlpha[2] = 0.5f;
    mDropAlpha = 1.0f;

    mDropOnColorBlack.set(30, 255, 255, 0);
    mDropOnColorWhite.set(255, 255, 255, 255);
    mDropOffColorBlack.set(0, 0, 0, 0);
    mDropOffColorWhite.set(255, 255, 255, 255);

    mPikariLoopScale = 0.6f;
    mPikariLoopFrontInner[0].set(255, 255, 255, 255);
    mPikariLoopFrontOuter[0].set(0, 0, 0, 0);
    mPikariLoopBackInner[0].set(255, 255, 255, 255);
    mPikariLoopBackOuter[0].set(126, 238, 218, 0);
    mPikariLoopAnimSpeed = 0.2f;

    mPikariLoopBackScale = 0.7f;
    mPikariLoopFrontInner[1].set(255, 255, 255, 255);
    mPikariLoopFrontOuter[1].set(0, 0, 0, 0);
    mPikariLoopBackInner[1].set(255, 255, 255, 255);
    mPikariLoopBackOuter[1].set(126, 238, 218, 0);
    mPikariLoopBackStopFrame = 19.0f;

    mDropPikariFrontInner.r = 255;
    mDropPikariFrontInner.g = 255;
    mDropPikariFrontInner.b = 255;
    mDropPikariFrontInner.a = 255;

    mDropPikariFrontOuter.r = 0;
    mDropPikariFrontOuter.g = 0;
    mDropPikariFrontOuter.b = 0;
    mDropPikariFrontOuter.a = 0;

    mDropPikariBackInner.r = 255;
    mDropPikariBackInner.g = 255;
    mDropPikariBackInner.b = 255;
    mDropPikariBackInner.a = 255;

    mDropPikariBackOuter.r = 126;
    mDropPikariBackOuter.g = 238;
    mDropPikariBackOuter.b = 218;
    mDropPikariBackOuter.a = 0;

    mDropPikariAnimSpeed = 0.1f;
    mDropPikariAnimSpeed_Completed = 0.6f;
    mPikariScaleNormal = 1.3f;
    mPikariScaleComplete = 0.3f;
    mAnimDebug = 0;
}

#if DEBUG
void dMeter_drawLightDropHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_drawHIO_c::dMeter_drawHIO_c() {
    #if DEBUG
    field_0x8_debug = 0;
    #endif
    mLifeTopPosX = 40.0f;
    mLifeTopPosY = -5.6f;

    mNoMagicPosY = -15.0f;

    mParentScale = 1.0f;
    mParentAlpha = 1.0f;

    mLifeParentScale = 1.0f;
    mLifeParentHeartAlpha = 1.0f;
    mHeartBaseAlpha = 0.4f;
    mLifeGaugePosX = 5.0f;
    mLifeGaugePosY = 18.0f;

    mHeartMarkScale = 1.0f;
    mHeartAlpha = 0.7f;
    mBigHeartScale = 1.0f;
    mBigHeartAlpha = 1.0f;

    mMagicMeterScale = 0.7f;
    mMagicMeterAlpha = 1.0f;
    mMagicMeterFrameAlpha = 0.55f;
    mMagicMeterPosX = -42.0f;
    mMagicMeterPosY = 0.0f;

    mLanternMeterScale = 0.7f;
    mLanternMeterAlpha = 1.0f;
    mLanternMeterFrameAlpha = 0.55f;
    mLanternMeterPosX = 8.0f;
    mLanternMeterPosY = 20.0f;

    mOxygenMeterScale = 0.7f;
    mOxygenMeterAlpha = 1.0f;
    mOxygenMeterFrameAlpha = 0.55f;
    mOxygenMeterPosX = 8.0f;
    mOxygenMeterPosY = 20.0f;

    mMainHUDButtonsScale = 0.92f;
    mMainHUDButtonsAlpha = 1.0f;
    mMainHUDButtonsPosX = -15.0f;
    mMainHUDButtonsPosY = 8.0f;

    mRingHUDButtonsScale = 0.92f;
    mRingHUDButtonsAlpha = 1.0f;
    mRingHUDButtonsPosX = -15.0f;
    mRingHUDButtonsPosY = 8.0f;

    mButtonDisplayBackScale = 1.0f;
    mButtonDisplayBackAlpha = 1.0f;

    mButtonAScale = 1.1f;
    mButtonAAlpha = 1.0f;
    mButtonAPosX = 0.0f;
    mButtonAPosY = 0.0f;
    mButtonATalkScale[0] = 1.0f;
    mButtonATalkPosX[0] = 0.0f;
    mButtonATalkPosY[0] = -63.0f;
    mButtonATalkAScale = 1.0f;
    mButtonATalkAPosX[0] = 0.0f;
    mButtonATalkAPosY[0] = -49.0f;

    mButtonATalkScale[1] = 1.0f;
    mButtonATalkPosX[1] = -20.0f;
    mButtonATalkPosY[1] = -62.0f;
    mButtonATextTalkAScale = 1.0f;
    mButtonATalkAPosX[1] = -20.0f;
    mButtonATalkAPosY[1] = -48.0f;

    mButtonAVesselPosX = 0.0f;
    mButtonAVesselPosY = 0.0f;

    mButtonBScale = 1.1f;
    mButtonBAlpha = 1.0f;
    mButtonBPosX = -2.2f;
    mButtonBPosY = -1.3f;
    mButtonBTalkScale[0] = 1.0f;
    mButtonBTalkPosX[0] = 0.0f;
    mButtonBTalkPosY[0] = -62.0f;

    mButtonBTalkScale[1] = 1.0f;
    mButtonBTalkPosX[1] = -9.2f;
    mButtonBTalkPosY[1] = -65.0f;

    mButtonBWolfPosX = 0.0f;
    mButtonBWolfPosY = 0.0f;

    mButtonBVesselPosX = 0.0f;
    mButtonBVesselPosY = 0.0f;

    mMidnaIconScale = 1.1f;
    mMidnaIconAlpha = 1.0f;
    mMidnaIconPosX = 0.0f;
    mMidnaIconPosY = 0.0f;

    field_0x134 = 1.1f;
    field_0x138 = 1.0f;
    field_0x13c = 0.0f;
    field_0x140 = 0.0f;

    mButtonXScale = 1.0f;
    mButtonXAlpha = 0.9f;
    mButtonXPosX = 0.0f;
    mButtonXPosY = 0.0f;

    mButtonYScale = 1.0f;
    mButtonYAlpha = 0.9f;
    mButtonYPosX = 0.0f;
    mButtonYPosY = 0.0f;

    field_0x164 = 1.5f;
    field_0x168 = 1.0f;
    field_0x16c = 0.0f;
    field_0x170 = 0.0f;

    field_0x304 = 1.0f;
    field_0x308 = 0.0f;
    field_0x30c = 0.0f;

    field_0x314 = 1.0f;
    field_0x318 = 0.0f;
    field_0x31c = 0.0f;

    field_0x320 = 1.0f;
    field_0x324 = 0.0f;
    field_0x328 = 0.0f;

    mButtonZScale = 1.0f;
    mButtonZAlpha = 0.9f;
    mButtonZPosX = 0.0f;
    mButtonZPosY = 0.0f;

    mButtonBItemPosX[0] = 3.0f;
    mButtonBItemPosY[0] = 18.0f;
    mButtonBItemRotation[0] = 0.0f;
    mButtonBItemScale[0] = 1.4f;

    mButtonBItemPosX[1] = 17.0f;
    mButtonBItemPosY[1] = 25.0f;
    mButtonBItemRotation[1] = 76.0f;
    mButtonBItemScale[1] = 2.3f;

    mButtonBItemPosX[2] = 4.0f;
    mButtonBItemPosY[2] = 22.0f;
    mButtonBItemRotation[2] = 20.0f;
    mButtonBItemScale[2] = 2.1f;

    mButtonXItemPosX = 4.0f;
    mButtonXItemPosY = -14.0f;
    mButtonXItemScale = 1.5f;

    mButtonYItemPosX = 0.0f;
    mButtonYItemPosY = 0.0f;
    mButtonYItemScale = 1.5f;

    field_0x1cc = 0.0f;
    field_0x1d0 = 0.0f;
    field_0x1d4 = 1.0f;

    mXItemNumPosX = -13.0f;
    mXItemNumPosY = -12.0f;
    mXItemNumScale = 0.95f;

    mYItemNumPosX = -1.0f;
    mYItemNumPosY = -18.0f;
    mYItemNumScale = 0.95f;

    field_0x1f8 = 0.0f;
    field_0x208 = 0.0f;
    field_0x218 = 1.0f;

    mItemBNumPosX = 0.0f;
    mItemBNumPosY = 0.0f;
    mItemBNumScale = 1.0f;

    for (int i = 0; i < 3; i++) {
        mButtonItemRotation[i] = 0.0f;
    }

    mButtonZItemPosX = 0.0f;
    mButtonZItemPosY = 0.0f;
    mButtonZItemScale = 1.0f;

    for (int i = 0; i < 2; i++) {
        mItemBBaseScale[i] = 1.0f;
        mItemBBasePosX[i] = 0.0f;
        mItemBBasePosY[i] = 0.0f;
    }

    mItemBBaseAlpha[0] = 0.5f;
    mItemBBaseAlpha[1] = 0.5f;

    mButtonXItemBaseScale[0] = 1.5f;
    mButtonXItemBasePosX[0] = 14.0f;
    mButtonXItemBasePosY[0] = -12.0f;
    mButtonXItemBaseAlpha[0] = 0.5f;

    mButtonXItemBaseScale[1] = 1.5f;
    mButtonXItemBasePosX[1] = 0.0f;
    mButtonXItemBasePosY[1] = 0.0f;
    mButtonXItemBaseAlpha[1] = 1.0f;

    mButtonYItemBaseScale[0] = 1.5f;
    mButtonYItemBasePosX[0] = -15.0f;
    mButtonYItemBasePosY[0] = -12.0f;
    mButtonYItemBaseAlpha[0] = 0.5f;

    mButtonYItemBaseScale[1] = 1.5f;
    mButtonYItemBasePosX[1] = 0.0f;
    mButtonYItemBasePosY[1] = 0.0f;
    mButtonYItemBaseAlpha[1] = 0.5f;

    field_0x280 = 1.5f;
    field_0x288 = 0.0f;
    field_0x290 = 6.0f;
    field_0x298[0] = 0.5f;

    field_0x284 = 1.0f;
    field_0x28c = 0.0f;
    field_0x294 = 4.0f;
    field_0x298[1] = 1.0f;

    mButtonZItemBaseScale = 1.0f;
    mButtonZItemBasePosX = 0.0f;
    mButtonZItemBasePosY = 0.0f;
    mButtonZItemBaseAlpha = 0.5f;

    mButtonBaseAlpha = 0.55f;

    mButtonATextActionID = 1001;
    field_0x2cc = 1001;
    mButtonATextDebug = false;

#if VERSION == VERSION_GCN_JPN
    mButtonATextSpacing = -2.0f;
#else
    mButtonATextSpacing = 1.0f;
#endif
    mButtonATextScale = 1.0f;
    mButtonATextPosX = 0.0f;
    mButtonATextPosY = 0.0f;
    mButtonATextColor.r = 255;
    mButtonATextColor.g = 255;
    mButtonATextColor.b = 255;
    mButtonATextColor.a = 255;

    mButtonBFontScale = 1.0f;
    mButtonBFontPosX = -8.0f;
    mButtonBFontPosY = 4.0f;
    mButtonBFontColor.r = 255;
    mButtonBFontColor.g = 255;
    mButtonBFontColor.b = 255;
    mButtonBFontColor.a = 255;

    field_0x2e4 = 1.0f;
    field_0x2e8 = 0.0f;
    field_0x2ec = 0.0f;
    field_0x2f0.r = 255;
    field_0x2f0.g = 255;
    field_0x2f0.b = 255;
    field_0x2f0.a = 255;

    mButtonXYTextScale = 1.0f;
    mButtonXYTextPosX = 0.0f;
    mButtonXYTextPosY = 0.0f;
    mButtonXYTextColor.r = 255;
    mButtonXYTextColor.g = 255;
    mButtonXYTextColor.b = 255;
    mButtonXYTextColor.a = 255;

    mButtonZFontScale = 1.0f;
    mButtonZFontPosX = 0.0f;
    mButtonZFontPosY = -4.7f;
    mButtonZFontColor.r = 255;
    mButtonZFontColor.g = 255;
    mButtonZFontColor.b = 255;
    mButtonZFontColor.a = 255;

    mRupeeKeyScale = 1.0f;
    mRupeeKeyPosX = 9.2f;
    mRupeeKeyPosY = -4.0f;
    mRupeeKeyAlpha = 0.8f;

    mRupeeScale = 0.8f;
    mRupeePosX = 0.0f;
    mRupeePosY = 0.0f;
    mRupeeAlpha = 1.0f;

    mKeyScale = 0.7f;
    mKeyPosX = -14.0f;
    mKeyPosY = 1.3f;
    mKeyAlpha = 1.0f;

    mRupeeFrameScale = 0.0f;
    mRupeeFramePosX = 0.0f;
    mRupeeFramePosY = 1.0f;
    mRupeeFrameAlpha = 1.0f;

    mRupeeCountScale = 1.0f;
    mRupeeCountPosX = 0.0f;
    mRupeeCountPosY = 0.0f;
    mRupeeCountAlpha = 1.0f;

    mKeyNumScale = 1.0f;
    mKeyNumPosX = 0.0f;
    mKeyNumPosY = 0.0f;
    mKeyNumAlpha = 1.0f;

    mSpurDebug = false;
    mSpurIconScale = 1.0f;
    mSpurIconAlpha = 1.0f;
    mUsedSpurIconScale = 1.0f;
    mUsedSpurIconAlpha = 1.0f;
    mSpurBarScale = 0.95f;
    mSpurBarPosX = 6.6f;
    mSpurBarPosY = -1.3f;

    mButtonAHorsePosX = 2.2f;
    mButtonAHorsePosY = 0.0f;
    mButtonAHorseScale = 0.85f;

    mButtonAPikariScale = 1.5f;
    mButtonBPikariScale = 0.7f;
    field_0x3cc = 1.0f;
    mButtonZPikariScale = 1.2f;
    mButtonXYPikariScale = 1.2f;
    mMidnaIconPikariScale = 1.0f;
    mSpurIconPikariScale = 2.5f;
    mSpurIconRevivePikariScale = 1.3f;
    field_0x3f0 = 0.0f;
    field_0x3f4 = 0.0f;
    field_0x3f8 = 1.0f;

    mButtonCrossScale = 0.9f;
    mButtonCrossOFFPosX = 12.0f;
    mButtonCrossOFFPosY = 0.0f;
    mButtonCrossONPosX = 12.0f;
    mButtonCrossONPosY = 0.0f;
    mButtonCrossTextScale = 0.9f;
    mButtonCrossAlpha = 1.0f;
    mButtonCrossITEMAlpha = 1.0f;
    mButtonCrossMAPAlpha = 1.0f;
    mButtonCrossMoveFrame = 5;
    field_0x422 = 10;
    field_0x424 = 20;

    mButtonXYItemDimAlpha = 128;
    mButtonXYBaseDimAlpha = 128;
    field_0x42c = 128;

    for (int i = 0; i < 4; i++) {
        mButtonDebug[i] = false;
    }

    mMaxSpurAmount = 6;

    mSpurIconPikariFrontInner.r = 255;
    mSpurIconPikariFrontInner.g = 255;
    mSpurIconPikariFrontInner.b = 255;
    mSpurIconPikariFrontInner.a = 255;

    mSpurIconPikariFrontOuter.r = 140;
    mSpurIconPikariFrontOuter.g = 232;
    mSpurIconPikariFrontOuter.b = 228;
    mSpurIconPikariFrontOuter.a = 0;

    mSpurIconPikariBackInner.r = 255;
    mSpurIconPikariBackInner.g = 255;
    mSpurIconPikariBackInner.b = 190;
    mSpurIconPikariBackInner.a = 255;

    mSpurIconPikariBackOuter.r = 55;
    mSpurIconPikariBackOuter.g = 215;
    mSpurIconPikariBackOuter.b = 90;
    mSpurIconPikariBackOuter.a = 0;

    mSpurIconPikariAnimSpeed = 1.5f;

    mSpurIconRevivePikariFrontInner.r = 255;
    mSpurIconRevivePikariFrontInner.g = 255;
    mSpurIconRevivePikariFrontInner.b = 255;
    mSpurIconRevivePikariFrontInner.a = 255;

    mSpurIconRevivePikariFrontOuter.r = 220;
    mSpurIconRevivePikariFrontOuter.g = 110;
    mSpurIconRevivePikariFrontOuter.b = 30;
    mSpurIconRevivePikariFrontOuter.a = 0;

    mSpurIconRevivePikariBackInner.r = 255;
    mSpurIconRevivePikariBackInner.g = 255;
    mSpurIconRevivePikariBackInner.b = 190;
    mSpurIconRevivePikariBackInner.a = 255;

    mSpurIconRevivePikariBackOuter.r = 170;
    mSpurIconRevivePikariBackOuter.g = 110;
    mSpurIconRevivePikariBackOuter.b = 0;
    mSpurIconRevivePikariBackOuter.a = 0;

    mSpurIconRevivePikariAnimSpeed = 0.5f;

    mButtonZPikariFrontInner.r = 255;
    mButtonZPikariFrontInner.g = 255;
    mButtonZPikariFrontInner.b = 255;
    mButtonZPikariFrontInner.a = 255;

    mButtonZPikariFrontOuter.r = 0;
    mButtonZPikariFrontOuter.g = 0;
    mButtonZPikariFrontOuter.b = 0;
    mButtonZPikariFrontOuter.a = 0;

    mButtonZPikariBackInner.r = 255;
    mButtonZPikariBackInner.g = 255;
    mButtonZPikariBackInner.b = 255;
    mButtonZPikariBackInner.a = 255;

    mButtonZPikariBackOuter.r = 160;
    mButtonZPikariBackOuter.g = 160;
    mButtonZPikariBackOuter.b = 110;
    mButtonZPikariBackOuter.a = 0;

    mButtonZPikariAnimSpeed = 0.4f;

    mButtonXYPikariFrontInner.r = 255;
    mButtonXYPikariFrontInner.g = 255;
    mButtonXYPikariFrontInner.b = 255;
    mButtonXYPikariFrontInner.a = 255;

    mButtonXYPikariFrontOuter.r = 0;
    mButtonXYPikariFrontOuter.g = 0;
    mButtonXYPikariFrontOuter.b = 0;
    mButtonXYPikariFrontOuter.a = 0;

    mButtonXYPikariBackInner.r = 255;
    mButtonXYPikariBackInner.g = 255;
    mButtonXYPikariBackInner.b = 255;
    mButtonXYPikariBackInner.a = 255;

    mButtonXYPikariBackOuter.r = 160;
    mButtonXYPikariBackOuter.g = 160;
    mButtonXYPikariBackOuter.b = 110;
    mButtonXYPikariBackOuter.a = 0;

    mButtonXYPikariAnimSpeed = 0.4f;

    mButtonAPikariFrontInner.r = 255;
    mButtonAPikariFrontInner.g = 255;
    mButtonAPikariFrontInner.b = 255;
    mButtonAPikariFrontInner.a = 160;

    mButtonAPikariFrontOuter.r = 0;
    mButtonAPikariFrontOuter.g = 0;
    mButtonAPikariFrontOuter.b = 0;
    mButtonAPikariFrontOuter.a = 0;

    mButtonAPikariBackInner.r = 255;
    mButtonAPikariBackInner.g = 255;
    mButtonAPikariBackInner.b = 255;
    mButtonAPikariBackInner.a = 128;

    mButtonAPikariBackOuter.r = 0;
    mButtonAPikariBackOuter.g = 255;
    mButtonAPikariBackOuter.b = 0;
    mButtonAPikariBackOuter.a = 0;

    mButtonAPikariAnimSpeed = 0.4f;

    mButtonBPikariFrontInner.r = 255;
    mButtonBPikariFrontInner.g = 180;
    mButtonBPikariFrontInner.b = 180;
    mButtonBPikariFrontInner.a = 160;

    mButtonBPikariFrontOuter.r = 0;
    mButtonBPikariFrontOuter.g = 0;
    mButtonBPikariFrontOuter.b = 0;
    mButtonBPikariFrontOuter.a = 0;

    mButtonBPikariBackInner.r = 255;
    mButtonBPikariBackInner.g = 255;
    mButtonBPikariBackInner.b = 255;
    mButtonBPikariBackInner.a = 128;

    mButtonBPikariBackOuter.r = 255;
    mButtonBPikariBackOuter.g = 0;
    mButtonBPikariBackOuter.b = 0;
    mButtonBPikariBackOuter.a = 0;

    mButtonBPikariAnimSpeed = 0.4f;

    field_0x4a8.r = 255;
    field_0x4a8.g = 180;
    field_0x4a8.b = 180;
    field_0x4a8.a = 160;

    field_0x4ac.r = 0;
    field_0x4ac.g = 0;
    field_0x4ac.b = 0;
    field_0x4ac.a = 0;

    field_0x4b0.r = 255;
    field_0x4b0.g = 255;
    field_0x4b0.b = 255;
    field_0x4b0.a = 128;

    field_0x4b4.r = 255;
    field_0x4b4.g = 0;
    field_0x4b4.b = 0;
    field_0x4b4.a = 0;

    field_0x4b8 = 0.4f;

    field_0x4bc.r = 255;
    field_0x4bc.g = 180;
    field_0x4bc.b = 180;
    field_0x4bc.a = 160;

    field_0x4c0.r = 0;
    field_0x4c0.g = 0;
    field_0x4c0.b = 0;
    field_0x4c0.a = 0;

    field_0x4c4.r = 255;
    field_0x4c4.g = 255;
    field_0x4c4.b = 255;
    field_0x4c4.a = 128;

    field_0x4c8.r = 255;
    field_0x4c8.g = 255;
    field_0x4c8.b = 255;
    field_0x4c8.a = 0;

    field_0x4cc = 0.4f;
    field_0x3d0 = 0.7f;

    field_0x4d0.r = 255;
    field_0x4d0.g = 180;
    field_0x4d0.b = 180;
    field_0x4d0.a = 160;

    field_0x4d4.r = 0;
    field_0x4d4.g = 0;
    field_0x4d4.b = 0;
    field_0x4d4.a = 0;

    field_0x4d8.r = 255;
    field_0x4d8.g = 255;
    field_0x4d8.b = 255;
    field_0x4d8.a = 128;

    field_0x4dc.r = 255;
    field_0x4dc.g = 255;
    field_0x4dc.b = 255;
    field_0x4dc.a = 0;

    field_0x4e0 = 0.4f;
    field_0x3d4 = 0.7f;

    mMidnaIconPikariFrontInner.r = 200;
    mMidnaIconPikariFrontInner.g = 235;
    mMidnaIconPikariFrontInner.b = 255;
    mMidnaIconPikariFrontInner.a = 255;

    mMidnaIconPikariFrontOuter.r = 0;
    mMidnaIconPikariFrontOuter.g = 0;
    mMidnaIconPikariFrontOuter.b = 0;
    mMidnaIconPikariFrontOuter.a = 0;

    mMidnaIconPikariBackInner.r = 255;
    mMidnaIconPikariBackInner.g = 255;
    mMidnaIconPikariBackInner.b = 255;
    mMidnaIconPikariBackInner.a = 255;

    mMidnaIconPikariBackOuter.r = 140;
    mMidnaIconPikariBackOuter.g = 200;
    mMidnaIconPikariBackOuter.b = 125;
    mMidnaIconPikariBackOuter.a = 0;

    mMidnaIconPikariAnimSpeed = 0.5f;
    mMidnaIconFlashRate = 50;

    mScrollArrowScaleX = 1.0f;
    mScrollArrowScaleY = 1.0f;
    mScrollArrowBCKAnimSpeed = 1.0f;
    mScrollArrowBPKAnimSpeed = 1.0f;
    mScrollArrowBTKAnimSpeed = 1.0f;
    mScrollArrowPosX[0] = 0.0f;
    mScrollArrowPosY[0] = 200.0f;
    mScrollArrowPosX[1] = 200.0f;
    mScrollArrowPosY[1] = 0.0f;
    mScrollArrowCenterPosX = 0.0f;
    mScrollArrowCenterPosY = 0.0f;
    mScrollArrowDisplayAll = false;

    mWiiLockArrowScaleX = 0.5f;
    mWiiLockArrowScaleY = 0.5f;
    mWiiLockArrowBCKAnimSpeed = 1.0f;
    mWiiLockArrowBPKAnimSpeed = 1.0f;
    mWiiLockArrowBTKAnimSpeed = 1.0f;
    mWiiLockArrowPosX[0] = 0.0f;
    mWiiLockArrowPosY[0] = 150.0f;
    mWiiLockArrowPosX[1] = 208.0f;
    mWiiLockArrowPosY[1] = 0.0f;
    mWiiLockArrowDisplayAll = false;

    field_0x54c = 1.0f;
    mItemScaleAdjustON = false;
    mItemScalePercent = 100;

    mFloatingMessagePosX = 0.0f;
    mFloatingMessagePosY = -100.0f;
    mMessageID = 395;
    mFlowID = 3003;
    mMessageDuration = 150;
    mMessageDebug = false;
    mFlowDebug = false;

    mTouchAreaSelectBlack[0].set(0, 0, 0, 0);
    mTouchAreaSelectWhite[0].set(255, 145, 100, 255);
    mTouchAreaSelectBlack[1].set(0, 0, 0, 0);
    mTouchAreaSelectWhite[1].set(255, 255, 0, 255);
    mTouchAreaSelectBlack[2].set(0, 0, 0, 0);
    mTouchAreaSelectWhite[2].set(230, 205, 180, 255);
    mTouchAreaSelectFrameNum = 10;

    for (int i = 0; i < 5; i++) {
        mTouchAreaUnselectScale[i] = 1.0f;
        mTouchAreaSelectScale[i] = 1.0f;
        mTouchAreaScale[i] = 1.0f;
        mTouchAreaAlpha[i] = 1.0f;
        mTouchAreaPosX[i] = 0.0f;
        mTouchAreaPosY[i] = 0.0f;
    }

    mDpadMAPScale = 1.0f;
    mDpadMAPPosX = 0.0f;
    mDpadMAPPosY = 0.0f;
    field_0x60a = false;

    mLanternIconMeterPosX = 0.0f;
    mLanternIconMeterPosY = 0.0f;
    mLanternIconMeterSize = 1.0f;
}

#if WIDESCREEN_SUPPORT
void dMeter_drawHIO_c::updateOnWide() {}

void dMeter_drawHIO_c::updateOffWide() {}
#endif

#if DEBUG
void dMeter_drawHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

void dMeter_drawHIO_c::updateFMsgDebug() {

}
#endif

dMeter_ringHIO_c::dMeter_ringHIO_c() {
    mRingRadiusH = 175.0f;
    mRingRadiusV = 160.0f;

    mOpenFrames = 5;
    mCloseFrames = 5;

    mSelectItemScale = 1.4f;
    mUnselectItemScale = 0.9f;
    mSelectButtonScale = 1.4f;
    mUnselectButtonScale = 1.0f;
    mCursorScale = 1.2f;

    mItemNum = 8;

    mInactiveItemScale = 0.4f;
    mItemAlphaFlashDuration = 300;
    mItemAlphaMin = 255;
    mItemAlphaMax = 255;

    mOffsetLineDisplay = 0;

    mItemIconAlpha = 255;
    mItemIconAlpha_Wolf = 110;

    mItemFrame[SELECT_FRAME].set(255, 255, 255, 0);
    mItemFrame[UNSELECT_FRAME].set(255, 255, 140, 0);

    mItemNamePosX = 125.0f;
    mItemNamePosY = 173.0f;
    mItemNameScale = 1.2f;

    mCenterPosX = 0.0f;
    mCenterPosY = 0.0f;
    mCenterScale = 1.0f;
    mOverlayAlpha = 0.35f;

    mGuidePosX[SET_ITEM] = -59.0f;
    mGuidePosY[SET_ITEM] = -64.0f;
    mGuideScale[SET_ITEM] = 0.8f;

    mGuidePosX[DIRECT_SELECT] = 90.0f;
    mGuidePosY[DIRECT_SELECT] = -124.0f;
    mGuideScale[DIRECT_SELECT] = 0.8f;

    mGuidePosX[2] = 0.0f;
    mGuidePosY[2] = 0.0f;
    mGuideScale[2] = 1.0f;

    mGuidePosX[ROTATE] = 118.0f;
    mGuidePosY[ROTATE] = -127.0f;
    mGuideScale[ROTATE] = 0.8f;

    mGuidePosX[COMBO_ITEM] = -1.3f;
    mGuidePosY[COMBO_ITEM] = -14.0f;
    mGuideScale[COMBO_ITEM] = 0.85f;

    for (int i = 5; i < 10; i++) {
        mGuidePosX[i] = 0.0f;
        mGuidePosY[i] = 18.0f;
        mGuideScale[i] = 0.85f;
    }

    mSelectPosX = 2.2f;
    mSelectPosY = 3.0f;
    mSelectScaleX = 0.8f;
    mSelectScaleY = 0.85f;

    mItemDescPosX = 0.0f;
    mItemDescPosY = 46.0f;
    mItemDescScale = 0.95f;
    mItemDescAlpha = 1.0f;

    mItemDescTitlePosX = 9.0f;
    mItemDescTitlePosY = 0.0f;
    mItemDescTitleScale = 1.1f;

    mItemRingPosX = -60.0f;
    mItemRingPosY = -5.0f;
    mRingPosX = -58.0f;
    mRingPosY = -8.0f;
    mRingAlpha = 0.75f;
    mRingAlpha_Wolf = 0.35f;
    mRingScaleH = 1.5f;
    mRingScaleV = 1.5f;

    mPikariScale = 2.5f;
    mPikariFrontInner.set(255, 255, 255, 185);
    mPikariFrontOuter.set(0, 155, 0, 0);
    mPikariBackInner.set(255, 255, 255, 87);
    mPikariBackOuter.set(0, 115, 0, 0);
    mPikariAnimSpeed = 0.772705f;

    mCursorInitSpeed = 5000;
    mCursorMax = 10000;
    mCursorAccel = 0;
    mCursorChangeWaitFrames = 2;

    field_0x10 = 120.0f;
    field_0x13e = 12;
    field_0x140 = 2048;
    field_0x142 = 128;

    field_0x44 = 0.0f;
    field_0x48 = 0.0f;
    field_0x4c = 1.0f;
    field_0x154 = 255;

    mDirectSelectWaitFrames = 4;
}

#if WIDESCREEN_SUPPORT
void dMeter_ringHIO_c::updateOnWide() {}

void dMeter_ringHIO_c::updateOffWide() {}
#endif

#if DEBUG
void dMeter_ringHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_mapIconHIO_c::dMeter_mapIconHIO_c() {
    mPortalCursorScale = 0.5f;
    mPortalIconScale = 0.65f;
    mDestinationScale[0] = 0.5f;
    mDestinationZoomScale[0] = 0.8f;
    mLinkScale[0] = 0.5f;
    mLinkZoomScale[0] = 0.6f;
    mYetoScale[0] = 0.3f;
    mYetoZoomScale[0] = 1.2f;
    mYetaScale[0] = 0.3f;
    mYetaZoomScale[0] = 1.2f;
    mGoldWolfScale[0] = 0.2f;
    mGoldWolfZoomScale[0] = 1.28f;
    mMonkeyScale[0] = 0.2f;
    mMonkeyZoomScale[0] = 1.6f;
    mOoccooScale[0] = 0.2f;
    mOoccooZoomScale[0] = 1.6f;
    mOoccooPotScale[0] = 0.2f;
    mOoccooPotZoomScale[0] = 1.6f;
    mCopyRodStatueScale[0] = 0.2f;
    mCopyRodStatueZoomScale[0] = 1.6f;
    mWagonScale[0] = 0.2f;
    mWagonZoomScale[0] = 1.0f;
    mChestScale[0] = 0.2f;
    mChestZoomScale[0] = 1.6f;
    mSmallKeyScale[0] = 0.2f;
    mSmallKeyZoomScale[0] = 1.4f;
    mDungeonEntranceScale[0] = 0.4f;
    mDungeonEntranceZoomScale[0] = 0.6f;
    mBossScale[0] = 0.2f;
    mBossZoomScale[0] = 1.6f;
    mGanonScale[0] = 0.2f;
    mGanonZoomScale[0] = 1.6f;
    mRoomEntranceScale[0] = 0.5f;
    mRoomEntranceZoomScale[0] = 0.6f;
    mLV8EntranceScale[0] = 0.5f;
    mLV8EntranceZoomScale[0] = 0.6f;
    mDungeonWarpScale[0] = 0.2f;
    mDungeonWarpZoomScale[0] = 1.6f;
    mLightDropScale[0] = 0.25f;
    mLightDropZoomScale[0] = 0.55f;
    mLightOrbScale[0] = 0.2f;
    mLightOrbZoomScale[0] = 1.6f;
    mCannonBallScale[0] = 0.2f;
    mCannonBallZoomScale[0] = 1.6f;
    mDestinationScale[1] = 0.5f;
    mDestinationZoomScale[1] = 0.8f;
    mLinkScale[1] = 0.4f;
    mLinkZoomScale[1] = 0.5f;
    mYetoScale[1] = 0.2f;
    mYetoZoomScale[1] = 0.8f;
    mYetaScale[1] = 0.2f;
    mYetaZoomScale[1] = 0.8f;
    mGoldWolfScale[1] = 0.2f;
    mGoldWolfZoomScale[1] = 0.8f;
    mMonkeyScale[1] = 0.2f;
    mMonkeyZoomScale[1] = 0.8f;
    mOoccooScale[1] = 0.2f;
    mOoccooZoomScale[1] = 0.8f;
    mOoccooPotScale[1] = 0.2f;
    mOoccooPotZoomScale[1] = 0.8f;
    mCopyRodStatueScale[1] = 0.2f;
    mCopyRodStatueZoomScale[1] = 0.8f;
    mWagonScale[1] = 0.2f;
    mWagonZoomScale[1] = 0.5f;
    mChestScale[1] = 0.2f;
    mChestZoomScale[1] = 0.8f;
    mSmallKeyScale[1] = 0.2f;
    mSmallKeyZoomScale[1] = 0.7f;
    mDungeonEntranceScale[1] = 0.3f;
    mDungeonEntranceZoomScale[1] = 0.6f;
    mBossScale[1] = 0.2f;
    mBossZoomScale[1] = 0.8f;
    mGanonScale[1] = 0.2f;
    mGanonZoomScale[1] = 0.8f;
    mRoomEntranceScale[1] = 0.4f;
    mRoomEntranceZoomScale[1] = 0.5f;
    mLV8EntranceScale[1] = 0.4f;
    mLV8EntranceZoomScale[1] = 0.5f;
    mDungeonWarpScale[1] = 0.2f;
    mDungeonWarpZoomScale[1] = 0.8f;
    mLightDropScale[1] = 0.25f;
    mLightDropZoomScale[1] = 0.3f;
    mLightOrbScale[1] = 0.2f;
    mLightOrbZoomScale[1] = 0.8f;
    mCannonBallScale[1] = 0.2f;
    mCannonBallZoomScale[1] = 0.8f;

    mLV5DungeonItemDebug = false;
    mLV5DungeonItem = fpcNm_ITEM_NONE;
    mIconDebug = false;

    for (int i = 0; i < ARRAY_SIZE(mIconDisplay); i++) {
        mIconDisplay[i] = true;
    }

    mLightDropFlashStartScaleOut[0] = 1.2f;
    mLightDropFlashEndScaleOut[0] = 1.5f;
    mLightDropFlashStartAlphaOut[0] = 255;
    mLightDropFlashEndAlphaOut[0] = 255;
    mLightDropFlashStartScaleOut[1] = 1.0f;
    mLightDropFlashEndScaleOut[1] = 1.3f;
    mLightDropFlashStartAlphaOut[1] = 255;
    mLightDropFlashEndAlphaOut[1] = 255;
    mLightDropFlashFrameNum = 10;
}

#if DEBUG
void dMeter_mapIconHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_fmapHIO_c::dMeter_fmapHIO_c() {
    mpArcFile = NULL;
    mpDecompBuf = NULL;
    mpArcData = NULL;

    #if DEBUG
    mpEvtCallBack = NULL;
    #endif

    mDisplayFrameNum = 8;
    mUndisplayFrameNum = 6;
    mMapType = TYPE_DEFAULT;

    mRegionZoomRange = 50000.0f;
    mStageZoomScale = 20.0f;
    mRegionGridWidth = 10000.0f;
    mWorldGridWidth = 50000.0f;
    mRangeCheck = false;
    mRangeCheckDrawPriority = false;
    mPortalWarpON = false;
    mDisplayWorldOrigin = false;
    mDisplayRegionOrigin = false;
    mDisplayStageOrigin = false;
    mDisplayWorldGrid = false;
    mDisplayRegionGrid = false;

    mScrollSpeedWorldSlow = 0.5f;
    mScrollSpeedRegionSlow = 0.5f;
    mScrollSpeedRegionZoomSlow = 1.0f;
    mScrollSpeedDungeonMapSlow = 0.5f;

    mScrollSpeedWorldFast = 2.5f;
    mScrollSpeedRegionFast = 2.5f;
    mScrollSpeedRegionZoomFast = 5.0f;
    mScrollSpeedDungeonMapFast = 2.5f;

    mScrollSpeedSlowBound = 0.2f;
    mScrollSpeedFastBound = 1.0f;

    for (int i = 0; i < 8; i++) {
        mRegionImagePosX[i] = 0.0f;
        mRegionImagePosZ[i] = 0.0f;
        mRegionImageScale[i] = 0.0f;
        
        mRegionScrollRangeMinX[i] = -300000.0f;
        mRegionScrollRangeMinZ[i] = -300000.0f;

        mRegionScrollRangeMaxX[i] = 300000.0f;
        mRegionScrollRangeMaxZ[i] = 300000.0f;
    }

    mScrollRangeDebugON = false;
    mScrollRangeDisplay = false;
    field_0x308 = true;
    mRegionImageDebug = false;

    mMapTopLeftPosX = 142.0f;
    mMapTopLeftPosY = 61.0f;
    mMapScale = 329.0f;
    mDisplayReferenceArea = false;

    mBackgroundAlpha = 1.0f;
    mBackgroundFrontAlpha = 0.7f;

    field_0x2f9 = false;
    mTerminalOutput = false;
    mPortalDataTerminalOutput = false;
    mRegionBitTerminalOutput = false;
    mAllRegionsUnlocked = false;
    mDrawBackground = true;

    mCursorScale = 1.0f;
    mCursorSpeed = 10.0f;
    mCursorWhite.set(255, 255, 255, 255);
    mCursorBlack.set(0, 182, 255, 0);
    mCursorDebugON = false;

    mDisplayAllPaths = false;
    mRegionPrintGlobalScroll = false;
    field_0x303 = 0;
    mRangeCheckInterval = 0;
    field_0x305 = 1;

    mTitlePosX = 6.6f;
    mTitlePosY = 7.0f;
    mTitleScale = 0.86499f;
    mSubTitlePosX = -22.0f;
    mSubTitlePosY = 4.5f;
    mSubTitleScale = 0.9f;

    mAreaMovementPosX = 6.0f;
    mAreaMovementPosY = -20.0f;
    mAreaMovementScale = 1.0f;

    mClosingPosX = 9.2f;
    mClosingPosY = -20.0f;
    mClosingScale = 0.9f;

    mDoIconPosX = 6.0f;
    mDoIconPosY = -6.0f;
    mDoIconScale = 0.9f;
    mButtonDebugON = false;

    mButtonZPosX = 25.0f;
    mButtonZPosY = 25.0f;
    mButtonZScale = 0.92f;

    mButtonAPosX = -0.5f;
    mButtonAPosY = 1.0f;
    mButtonAScale = 0.92f;

    mButtonBPosX = -1.0f;
    mButtonBPosY = -1.0f;
    mButtonBScale = 1.05f;

    mButtonZTextPosX = 20.0f;
    mButtonZTextPosY = 12.0f;
    mButtonZTextScale = 0.95f;

    mButtonATextPosX = -7.5f;
    mButtonATextPosY = 8.0f;
    mButtonATextScale = 0.95f;

    mButtonBTextPosX = -7.0f;
    mButtonBTextPosY = 5.0f;
    mButtonBTextScale = 0.95f;

    mMapBlink[WORLD_VIEW].mSelectedRegion.mBlack.set(29, 24, 0, 0);
    mMapBlink[WORLD_VIEW].mSelectedRegion.mWhite.set(255, 255, 255, 255);
    mMapBlink[WORLD_VIEW].mSelectedRegion.mMax = 255;
    mMapBlink[WORLD_VIEW].mSelectedRegion.mMin = 200;
    mMapBlink[WORLD_VIEW].mSelectedRegion.mBlinkSpeed = 60;

    mMapBlink[WORLD_VIEW].mUnselectedRegion.mBlack.set(0, 0, 0, 0);
    mMapBlink[WORLD_VIEW].mUnselectedRegion.mWhite.set(170, 170, 170, 255);
    mMapBlink[WORLD_VIEW].mUnselectedRegion.mMax = 255;
    mMapBlink[WORLD_VIEW].mUnselectedRegion.mMin = 255;
    mMapBlink[WORLD_VIEW].mUnselectedRegion.mBlinkSpeed = 0;

    mMapBlink[WORLD_VIEW].mUnreachedRegionBlack.set(0, 20, 80, 0);
    mMapBlink[WORLD_VIEW].mUnreachedRegionWhite.set(30, 95, 190, 0);
    mMapBlink[WORLD_VIEW].field_0x20.set(0, 0, 0, 0);
    mMapBlink[WORLD_VIEW].field_0x24.set(0, 0, 0, 255);

    mMapBlink[REGION_VIEW].mSelectedRegion.mBlack.set(0, 0, 0, 0);
    mMapBlink[REGION_VIEW].mSelectedRegion.mWhite.set(255, 255, 255, 255);
    mMapBlink[REGION_VIEW].mSelectedRegion.mMax = 84;
    mMapBlink[REGION_VIEW].mSelectedRegion.mMin = 84;
    mMapBlink[REGION_VIEW].mSelectedRegion.mBlinkSpeed = 60;

    mMapBlink[REGION_VIEW].mUnselectedRegion.mBlack.set(0, 0, 0, 0);
    mMapBlink[REGION_VIEW].mUnselectedRegion.mWhite.set(48, 48, 48, 255);
    mMapBlink[REGION_VIEW].mUnselectedRegion.mMax = 255;
    mMapBlink[REGION_VIEW].mUnselectedRegion.mMin = 255;
    mMapBlink[REGION_VIEW].mUnselectedRegion.mBlinkSpeed = 0;

    mMapBlink[REGION_VIEW].mUnreachedRegionBlack.set(0, 20, 80, 0);
    mMapBlink[REGION_VIEW].mUnreachedRegionWhite.set(30, 95, 190, 0);
    mMapBlink[REGION_VIEW].field_0x20.set(0, 0, 0, 0);
    mMapBlink[REGION_VIEW].field_0x24.set(0, 0, 0, 255);

    mMapBlink[ZOOM_VIEW].mSelectedRegion.mBlack.set(0, 0, 0, 0);
    mMapBlink[ZOOM_VIEW].mSelectedRegion.mWhite.set(255, 255, 255, 255);
    mMapBlink[ZOOM_VIEW].mSelectedRegion.mMax = 40;
    mMapBlink[ZOOM_VIEW].mSelectedRegion.mMin = 40;
    mMapBlink[ZOOM_VIEW].mSelectedRegion.mBlinkSpeed = 60;

    mMapBlink[ZOOM_VIEW].mUnselectedRegion.mBlack.set(0, 0, 0, 0);
    mMapBlink[ZOOM_VIEW].mUnselectedRegion.mWhite.set(22, 22, 22, 255);
    mMapBlink[ZOOM_VIEW].mUnselectedRegion.mMax = 255;
    mMapBlink[ZOOM_VIEW].mUnselectedRegion.mMin = 255;
    mMapBlink[ZOOM_VIEW].mUnselectedRegion.mBlinkSpeed = 0;

    mMapBlink[ZOOM_VIEW].mUnreachedRegionBlack.set(0, 20, 80, 0);
    mMapBlink[ZOOM_VIEW].mUnreachedRegionWhite.set(30, 95, 190, 0);
    mMapBlink[ZOOM_VIEW].field_0x20.set(0, 0, 0, 0);
    mMapBlink[ZOOM_VIEW].field_0x24.set(0, 0, 0, 255);

    for (int i = 0; i < ARRAY_SIZE(mDebugRegionBits); i++) {
        mDebugRegionBits[i] = 0;
    }
}

dMeter_fmapHIO_c::~dMeter_fmapHIO_c() {
    if (mpArcFile != NULL) {
        mDoExt_getGameHeap()->free(mpArcFile);
        mpArcFile = NULL;
    }

    if (mpDecompBuf != NULL) {
        mDoExt_getGameHeap()->free(mpDecompBuf);
        mpDecompBuf = NULL;
    }

    if (mpArcData != NULL) {
        JKRUnmountArchive(mpArcData);
        delete mpArcData;
        mpArcData = NULL;
    }

    #if DEBUG
    if (mpEvtCallBack != NULL) {
        delete mpEvtCallBack;
        mpEvtCallBack = NULL;
    }
    #endif
}

#if DEBUG
void dMeter_fmapHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}

void dMeter_fmapHIO_c::listenPropertyEvent(const JORPropertyEvent*) {

}

void dMeter_fmapHIO_c::createEvtCallBackObject() {
    
}

void dMeter_fmapHIO_c::deleteEvtCallBackObject() {
    
}

void dMeter_fmapHIO_c::update() {
    
}
#endif

dMeter_cursorHIO_c::dMeter_cursorHIO_c() {
    mShopCursorScale = 1.05f;
    mShopCursorOffsetY = -30.0f;
    mMagicArmorCursorOffsetX = 13.0f;
    mMagicArmorCursorOffsetY = -102.0f;
    mSeraShopObjZoom.set(100.0f, 145.0f, -155.0f);
    mSeraShopObjZoomAngleX = 3000;
    mObjZoom.set(0.0f, 0.0f, 0.0f);
    mShopObjZoomAngleX = 5000;
    mMagicArmorObjZoom.set(0.0f, 20.0f, 28.0f);
    mMagicArmorObjZoomAngleX = -500;
}

#if DEBUG
void dMeter_cursorHIO_c::genMessage(JORMContext* mctx) {
    // DEBUG NONMATCHING
}
#endif

dMeter_menuHIO_c g_menuHIO;

dMeter_drawHIO_c g_drawHIO;

dMeter_ringHIO_c g_ringHIO;

dMeter_fmapHIO_c g_fmapHIO;

dMeter_cursorHIO_c g_cursorHIO;
