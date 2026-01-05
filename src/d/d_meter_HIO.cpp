#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_meter_HIO.h"
#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JKernel/JKRAramArchive.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JKernel/JKRDecomp.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_menu_dmap.h"
#include "d/d_menu_fmap_map.h"
#include "d/d_meter_map.h"
#include "m_Do/m_Do_ext.h"

dMeter_menuHIO_c::dMeter_menuHIO_c() {
    mGameover = 70;
}

#if DEBUG
void dMeter_menuHIO_c::genMessage(JORMContext* mctx) {
    mctx->genSlider("ゲームオーバー", &mGameover, 1, 20000, 0, NULL, -1, -1, 512, 24);
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
    mctx->genLabel("*****アイテム*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（小）", &mUnselectItemScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（大）", &mSelectItemScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****セーブ・オプション*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（小）", &mUnselectSaveOptionScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（大）", &mSelectSaveOptionScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****青いモヤモヤパーツ*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mBlueSmokePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mBlueSmokePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mBlueSmokeScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mBlueSmokeAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ミドナの仮面・鏡*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mMaskMirrorPos.x, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mMaskMirrorPos.y, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｚ", &mMaskMirrorPos.z, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アングルＸ", &mMaskMirrorAngle.x, -0x8000, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アングルＹ", &mMaskMirrorAngle.y, -0x8000, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アングルＺ", &mMaskMirrorAngle.z, -0x8000, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mMaskMirrorScale, -1.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメーション", &mMaskMirrorAnimSpeed, 0.0f, 10.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ハートの器*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHeartVesselPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHeartVesselPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mHeartVesselScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ハートのかけら*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHeartPiecePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHeartPiecePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mHeartPieceScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ボタンデバッグＯＮ", (u8*)&mButtonDebugON, 1, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("*****Ａボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonAPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonAPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonAScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｂボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonBPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonBPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonBScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ａテキスト*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonATextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonATextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonATextScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｂテキスト*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonBTextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonBTextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonBTextScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("カラーデバッグＯＮ", (u8*)&mColorDebugON, 1, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("*****カラーht_n_bas*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒Ｒ", &mVesselBack[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒Ｇ", &mVesselBack[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒Ｂ", &mVesselBack[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒Ａ", &mVesselBack[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ｒ", &mVesselFront[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ｇ", &mVesselFront[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ｂ", &mVesselFront[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ａ", &mVesselFront[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****カラーh_kaz_n*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒Ｒ", &mVesselBack[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒Ｇ", &mVesselBack[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒Ｂ", &mVesselBack[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒Ａ", &mVesselBack[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ｒ", &mVesselFront[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ｇ", &mVesselFront[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ｂ", &mVesselFront[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白Ａ", &mVesselFront[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
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
    mctx->genSlider("open frame", &mOpenFrames, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("close frame", &mCloseFrames, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mDebug, 1, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("*****矢印*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("サイズ", &mArrowSize, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****項目名カラー*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("*****選択中カラー*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("Ｒ", &mSelectColor.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("Ｇ", &mSelectColor.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("Ｂ", &mSelectColor.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****非選択中カラー*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("Ｒ", &mUnselectColor.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("Ｇ", &mUnselectColor.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("Ｂ", &mUnselectColor.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********バーのスケール********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("選択時", &mBarScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非選択時", &mBarScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ウインドウ*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mWindowPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mWindowPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ", &mWindowScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****メニュー位置*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("*****注目タイプ*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mOptionTypeBGPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mOptionTypeBGPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ふりがな*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mOptionTypeBGPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mOptionTypeBGPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****しんどう*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mOptionTypeBGPosX[2], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mOptionTypeBGPosY[2], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****サウンド*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mOptionTypeBGPosX[3], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mOptionTypeBGPosY[3], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****テレビ画面の設定*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mTVsettingPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mTVsettingPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****キャリブレーション*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mPointerCalibrationPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mPointerCalibrationPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****設定確認*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("背景アルファ", &mBackgroundAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mBackgroundPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ハイハイ矢印*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("幅オフセットX", &mArrowOffsetX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("幅オフセットX(4:3)", &mArrowOffsetX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->genSlider("open frame", &mOpenFrame[0], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("close frame", &mCloseFrame[0], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("本文オープン", &mOpenFrame[1], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("本文クローズ", &mCloseFrame[1], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("手紙の枚数", &mLetterNum, 0, 54, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mDebugON, 1, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("********選択時のページアイコンのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mSelectPageIconBack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mSelectPageIconBack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mSelectPageIconBack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mSelectPageIconBack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mSelectPageIconFront.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mSelectPageIconFront.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mSelectPageIconFront.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mSelectPageIconFront.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********非選択時のページアイコンのアルファ********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mUnselectPageIconAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********選択時のバーのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mSelectBarBack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mSelectBarBack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mSelectBarBack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mSelectBarBack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mSelectBarFront.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mSelectBarFront.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mSelectBarFront.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mSelectBarFront.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********バーのスケール********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("選択時", &mSelectBarScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非選択時", &mUnselectBarScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********選択時のテキストのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mSelectTextBack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mSelectTextBack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mSelectTextBack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mSelectTextBack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mSelectTextFront.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mSelectTextFront.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mSelectTextFront.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mSelectTextFront.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********手紙の文字のカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mLetterTextBack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mLetterTextBack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mLetterTextBack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mLetterTextBack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mLetterTextFront.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mLetterTextFront.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mLetterTextFront.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mLetterTextFront.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********手紙全体の調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mWindowPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mWindowPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mWindowScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********手紙ウインドウ全体の調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mLetterWindowPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mLetterWindowPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mLetterWindowScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********手紙ウインドウ背景の調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mWindowBGAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********手紙ウインドウラインの調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mLetterLinePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mLetterLinePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mLetterLineAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
}
#endif

dMeter_drawFishingHIO_c::dMeter_drawFishingHIO_c() {
    static f32 const fipaScale[6] = {0.98f, 1.0f, 1.0f, 1.0f, 0.95f, 1.1f};
#if PLATFORM_GCN
    static f32 const fipaPosX[6] = {1.3f, 6.6f, 0.0f, 3.96f, 4.0f, 27.5f};
#else
    static f32 const fipaPosX[6] = {1.3f, 6.6f, 0.0f, 3.96f, 11.7f, 27.5f};
#endif
    static f32 const fipaPosY[6] = {-9.2f, -27.5f, -11.7f, -9.2f, -30.1f, -17.9f};
    static f32 const finaScale[6] = {0.9f, 0.9f, 0.9f, 0.9f, 0.9f, 0.9f};

#if PLATFORM_GCN
    static f32 const finaPosX[6] = {-25.0f, -4.0f, 0.0f, 0.0f, 0.0f, -4.0f};
    static f32 const finaPosY[6] = {-16.0f, -40.6f, -24.0f, -11.7f, -37.9f, -13.0f};
#elif VERSION == VERSION_WII_USA_R0
    static f32 const finaPosX[6] = {0.0f, 0.0f, -3.96f, 0.0f, -3.96f, 0.0f};
    static f32 const finaPosY[6] = {-14.4f, -40.6f, -22.3f, -11.7f, -37.9f, -11.7f};
#elif VERSION == VERSION_WII_JPN
    static f32 const finaPosX[6] = {4.0f, 4.0f, 0.0f, 0.0f, 0.0f, 4.0f};
    static f32 const finaPosY[6] = {-16.0f, -40.6f, -24.0f, -11.7f, -37.9f, -13.0f};
#else
    static f32 const finaPosX[6] = {0.0f, 0.0f, -3.96f, 0.0f, -3.96f, 4.0f};
    static f32 const finaPosY[6] = {-14.4f, -40.6f, -22.3f, -11.7f, -37.9f, -11.7f};
#endif

    static f32 const filiScale[6] = {0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f};

#if PLATFORM_GCN
    static f32 const filiPosX[6] = {-28.0f, -8.0f, 9.2f, 9.2f, 9.2f, -8.0f};
    static f32 const filiPosY[6] = {-6.6f, -14.0f, -14.4f, 24.9f, -11.7f, 24.9f};
#else
    static f32 const filiPosX[6] = {0.0f, 0.0f, 9.2f, 9.2f, 9.2f, 1.3f};
    static f32 const filiPosY[6] = {-6.6f, -14.4f, -14.4f, 24.9f, -11.7f, 24.9f};
#endif

    static f32 const bboxScale[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    static f32 const bboxPosX[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const bboxPosY[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const rboxScale[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    static f32 const rboxPosX[6] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const rboxPosY[6] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static f32 const finfoScale[6] = {0.87f, 0.87f, 0.88f, 0.87f, 0.87f, 0.87f};

#if VERSION == VERSION_GCN_JPN
    static f32 const finfoPosX[6] = {-27.0f, 0.0f, -12.0f, 0.0f, -12.0f, -32.8f};
#elif PLATFORM_GCN
    static f32 const finfoPosX[6] = {-17.0f, 0.0f, -14.0f, 0.0f, -12.0f, -32.8f};
#elif VERSION == VERSION_WII_USA_R0 || VERSION == VERSION_WII_JPN
    static f32 const finfoPosX[6] = {-27.0f, 0.0f, 0.0f, 0.0f, 0.0f, -32.8f};
#else
    static f32 const finfoPosX[6] = {-17.0f, 0.0f, -14.0f, 0.0f, 0.0f, -32.8f};
#endif

    static f32 const finfoPosY[6] = {6.6f, 0.0f, 0.0f, 0.0f, 1.3f, 0.0f};
    static f32 const fishnScale[6] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

#if VERSION == VERSION_GCN_JPN
    static f32 const fishnPosX[6] = {12.0f, -20.0f, 1.3f, 6.6f, 14.0f, -6.6f};
#elif VERSION == VERSION_WII_USA_R0 || VERSION == VERSION_WII_JPN
    static f32 const fishnPosX[6] = {12.0f, -20.0f, 0.0f, 14.4f, 35.3f, -6.6f};
#elif PLATFORM_WII
    static f32 const fishnPosX[6] = {4.0f, -20.0f, 0.0f, 14.4f, 1.3f, 9.2f};
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
#if PLATFORM_WII
    mPosX[0] = 6.6f;
#else
    mPosX[0] = 5.0f;
#endif
    mPosY[0] = -22.2f;

    mScale[1] = 0.83f;
    mPosX[1] = -189.5f;
    mPosY[1] = -3.95f;
    mCaughtNumPosX_4x3 = -188.0f;
}

#if DEBUG
void dMeter_drawFishingHIO_c::genMessage(JORMContext* mctx) {
    static const char* fish_name[6] = {
        "********トアルナマズ********",
        "********ニオイマス********",
        "********ハイリアパイク********",
        "********グリーンギル********",
        "********ハイラルドジョウ********",
        "********ハイラルバス********",
    };
    static u8 fish_type[6] = {3, 4, 2, 5, 1, 0};
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mDebug, 1, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("open frame", &mOpenFrames, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("close frame", &mCloseFrames, 1, 20, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 6; i++) {
        mctx->genLabel(fish_name[i], 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("数", &mFishNum[fish_type[i]], 0, 999, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("サイズ", &mFishSize[fish_type[i]], 0, 255, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***魚パーツ調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishIconScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishIconPosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishIconPosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***魚の名前パーツ調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishNameScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishNamePosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishNamePosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***ラインパーツ調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishLineScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishLinePosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishLinePosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***サイズパーツ調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishSizeScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishSizePosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishSizePosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***匹数パーツ調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishCountScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishCountPosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishCountPosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***サイズ＋匹数パーツ調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishCountSizeScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishCountSizePosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishCountSizePosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("***全体調整***", 0, 0, 0, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mFishInfoScale[fish_type[i]], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｘ", &mFishInfoPosX[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置Ｙ", &mFishInfoPosY[fish_type[i]], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("\n", 0, 0, 0, -1, -1, 512, 24);
    }
    mctx->genLabel("********”最大サイズ”調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********”釣り上げた数”調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ", &mPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｘ(4:3)", &mCaughtNumPosX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置Ｙ", &mPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->startComboBox("タイプ", &mType, 0, 0, -1, -1, 256, 26);
    mctx->genComboBoxItem("デフォルト", 0);
    mctx->genComboBoxItem("コレクト画面", 1);
    mctx->genComboBoxItem("アゲハ", 2);
    mctx->endComboBox();
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mDebugON, 1, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("open frame", &mOpenFrame, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("close frame", &mCloseFrame, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****アイテム*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（小）", &mUnselectInsectScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（大）", &mSelectInsectScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****説明ウインドウ*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mDescWindowPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mDescWindowPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****選択ウインドウ(4:3)*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mGiveOptionPosX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mGiveOptionPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****選択肢ウインドウ(4:3)*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mConfirmOptionPosX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mConfirmOptionPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****選択ウインドウ(16:9)*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mGiveOptionPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mGiveOptionPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****選択肢ウインドウ(16:9)*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mConfirmOptionPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mConfirmOptionPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mDebug, 1, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("open frame", &mOpenFrames, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("close frame", &mCloseFrames, 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("点滅フレーム数", &mFlashFrameNum, 0, 300, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ステップ１*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("円のスケール", &mCircleScale, 0.0f, 5.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ステップ３*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("円のスケール", &mCircleScale2, 0.0f, 5.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->genSlider("open frame", &mOpenFrame[0], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("close frame", &mCloseFrame[0], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("本文オープン", &mOpenFrame[1], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("本文クローズ", &mCloseFrame[1], 1, 20, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mDebug, 1, 0, 0, -1, -1, 512, 24);
    mctx->genLabel("********選択時の巻物アイコンのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mSelectScrollIconBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mSelectScrollIconBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mSelectScrollIconBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mSelectScrollIconBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mSelectScrollIconWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mSelectScrollIconWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mSelectScrollIconWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mSelectScrollIconWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********非選択時の巻物アイコンのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mUnselectScrollIconBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mUnselectScrollIconBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mUnselectScrollIconBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mUnselectScrollIconBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mUnselectScrollIconWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mUnselectScrollIconWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mUnselectScrollIconWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mUnselectScrollIconWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********選択時のバーのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mSelectBarBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mSelectBarBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mSelectBarBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mSelectBarBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mSelectBarWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mSelectBarWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mSelectBarWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mSelectBarWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********バーのスケール********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("選択時", &mSelectBarScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非選択時", &mUnselectBarScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********選択時のテキストのカラー********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mSelectTextBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mSelectTextBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mSelectTextBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mSelectTextBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mSelectTextWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mSelectTextWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mSelectTextWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mSelectTextWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********奥義ウインドウ全体の調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mSkillDescPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mSkillDescPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mSkillDescScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********奥義ウインドウ背景の調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mWindowBGalpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********奥義ウインドウラインの調整********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mWindowLinePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mWindowLinePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mWindowLineAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("********奥義名********", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mSkillTitlePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mSkillTitlePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mSkillTitleScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->genCheckBox("デバッグ表示A", &mDebugDisplayButton[0], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示B", &mDebugDisplayButton[1], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示R", &mDebugDisplayButton[2], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示Z", &mDebugDisplayButton[3], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示3D", &mDebugDisplayButton[4], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示C", &mDebugDisplayButton[5], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示S", &mDebugDisplayButton[6], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示X", &mDebugDisplayButton[7], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示Y", &mDebugDisplayButton[8], 1, 0, 0, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグ表示ビンアイコン", &mDebugDisplayButton[14], 1, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("２個表示の間隔", &mDisplaySpace, 0.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****全体*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mEmpButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mEmpButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mEmpButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Aボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mButtonAPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mButtonAPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonAScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Bボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mButtonBPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mButtonBPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonBScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Rボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mRButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mRButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mRButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Zボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mButtonZPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mButtonZPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonZScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****3Dボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &m3DButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &m3DButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &m3DButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Cボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mCButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mCButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mCButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****スタートボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mSButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mSButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mSButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｘボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mButtonXPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mButtonXPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonXScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｙボタン*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mButtonYPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mButtonYPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonYScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャクコントローラ*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****リモコンコントローラ*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mWiimotePosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mWiimotePosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mWiimoteScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****リモコンコントローラ（←→）*****", 0, 0, 0, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mWiimoteLRPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mWiimoteLRPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mWiimoteLRScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ａ＋リモコンコントローラ*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mWiimoteAPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mWiimoteAPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mWiimoteAScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****３Ｄ＋Ｂ*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &m3DBButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &m3DBButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &m3DBButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャク＋リモコン*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukWiimotePosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukWiimotePosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukWiimoteScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャク＋リモコン（リール）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukWiimoteRealPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukWiimoteRealPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukWiimoteRealScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャク（リール）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukRealPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukRealPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukRealScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャク（盾アタック）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukAttackPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukAttackPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukAttackScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ａ＋Ｂ*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mAButtonBPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mAButtonBPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mAButtonBScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャクＺボタン*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukButtonZPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukButtonZPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukButtonZScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ヌンチャクＣボタン*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mNunchukCButtonPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mNunchukCButtonPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mNunchukCButtonScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ビンアイコン*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mBottleIconPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mBottleIconPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mBottleIconScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ビューチェンジ*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mViewChangePosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mViewChangePosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mViewChangeScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****テキスト1*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mText1PosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mText1PosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mText1Scale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****テキスト2*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mText2PosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mText2PosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mText2Scale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****アイテム*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mItemPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mItemPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mItemScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ミドナアイコン*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整X", &mMidnaIconPosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Y", &mMidnaIconPosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mMidnaIconScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ピカリ調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ピカリ調整（一騎撃ち用）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariFastScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mPikariFastFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mPikariFastFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mPikariFastFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mPikariFastFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mPikariFastFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mPikariFastFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mPikariFastFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mPikariFastFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mPikariFastBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mPikariFastBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mPikariFastBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mPikariFastBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mPikariFastBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mPikariFastBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mPikariFastBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mPikariFastBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mPikariFastAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ピカリ調整（盗み聞き用）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariListenScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mPikariListenFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mPikariListenFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mPikariListenFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mPikariListenFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mPikariListenFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mPikariListenFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mPikariListenFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mPikariListenFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mPikariListenBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mPikariListenBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mPikariListenBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mPikariListenBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mPikariListenBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mPikariListenBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mPikariListenBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mPikariListenBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mPikariListenAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ピカリ調整（連打）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariRepeatHitScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mPikariRepeatHitFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mPikariRepeatHitFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mPikariRepeatHitFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mPikariRepeatHitFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mPikariRepeatHitFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mPikariRepeatHitFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mPikariRepeatHitFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mPikariRepeatHitFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mPikariRepeatHitBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mPikariRepeatHitBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mPikariRepeatHitBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mPikariRepeatHitBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mPikariRepeatHitBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mPikariRepeatHitBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mPikariRepeatHitBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mPikariRepeatHitBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mPikariRepeatHitAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("連打フレーム数", &mRepeatHitFrameNum, 1, 300, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("連打スケール", &mRepeatHitScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****ホークアイ*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mHawkeyeDebugON, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***全体***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHawkeyePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHawkeyePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***テキスト***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHawkeyeTextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", mHawkeyeTextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***＋***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHawkeyePlusPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", mHawkeyeTextPosY + 1, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***−***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHawkeyeMinusPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", mHawkeyeTextPosY + 2, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****ズームイン・ズームアウト*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mHawkeyeZoomDebugON, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***全体***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mHawkeyeZoomPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHawkeyeZoomPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mHawkeyeZoomScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール(4:3)", &mHawkeyeZoomScale_4x3, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***テキスト＋アイコン***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", mHawkeyeIconPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHawkeyeTextIconPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(4:3)", &mHawkeyeTextIconPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", mHawkeyeIconScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***テキスト***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", mHawkeyeIconPosX + 1, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHawkeyeZoomTextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(4:3)", &mHawkeyeTextPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", mHawkeyeIconScale + 1, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***アイコン***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", mHawkeyeIconPosX + 2, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mHawkeyeIconPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(4:3)", &mHawkeyeIconPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", mHawkeyeIconScale + 2, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***ポインタＯＮ***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分0(R)", &mPointerONBlack[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分0(G)", &mPointerONBlack[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分0(B)", &mPointerONBlack[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分0(A)", &mPointerONBlack[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分0(R)", &mPointerONWhite[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分0(G)", &mPointerONWhite[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分0(B)", &mPointerONWhite[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分0(A)", &mPointerONWhite[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分1(R)", &mPointerONBlack[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分1(G)", &mPointerONBlack[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分1(B)", &mPointerONBlack[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分1(A)", &mPointerONBlack[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分1(R)", &mPointerONWhite[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分1(G)", &mPointerONWhite[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分1(B)", &mPointerONWhite[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分1(A)", &mPointerONWhite[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mPointerONScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメフレーム", &mPointerONAnimFrame, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***ポインタＯＦＦ***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(R)", &mPointerOFFBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(G)", &mPointerOFFBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(B)", &mPointerOFFBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分(A)", &mPointerOFFBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(R)", &mPointerOFFWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(G)", &mPointerOFFWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(B)", &mPointerOFFWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分(A)", &mPointerOFFWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mPointerOFFScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->genLabel("************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****山羊追いゲーム*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****タイマーの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mTimerSizeX[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mTimerSizeY[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mTimerPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mTimerPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mTimerAlpha[0], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ4:3", &mTimerSizeX_4x3, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ4:3", &mTimerSizeY_4x3, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ4:3", &mTimerPosX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ4:3", &mTimerPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****カウンターの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mCounterSizeX[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mCounterSizeY[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mCounterPosX[0], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mCounterPosY[0], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mCounterAlpha[0], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****牛のイメージの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mIconSizeX[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mIconSizeY[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mIconPosX[0], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(4:3)", &mIconPosX_4x3, -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mIconPosY[0], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mIconAlpha[0], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("****************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ゾーラ川下りゲーム*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("****************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****的＋カウンタの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &field_0x8[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &field_0x14[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(4:3)", &mRiverRideIconPosX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &field_0x20[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &field_0x2c[1], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****カウンターの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mCounterSizeX[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mCounterSizeY[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mCounterPosX[1], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mCounterPosY[1], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mCounterAlpha[1], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****的のイメージの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mIconSizeX[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mIconSizeY[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mIconPosX[1], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mIconPosY[1], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mIconAlpha[1], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ライダーゲーム*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****カウンターの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mCounterSizeX[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mCounterSizeY[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mCounterPosX[2], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mCounterPosY[2], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mCounterAlpha[2], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ライダーのイメージの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mIconSizeX[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mIconSizeY[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mIconPosX[2], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(4:3)", &mRiderGameIconPosX_4x3, -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mIconPosY[2], -600.0f, 600.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mIconAlpha[2], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****スタート処理*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("Ready?Go!", &mReadyGo, 0, 200, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Get In!の調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->startComboBox("出現位置", &mGetInTextLocation, 0, NULL, -1, -1, 256, 26);
    mctx->genComboBoxItem("固定位置", 0);
    mctx->genComboBoxItem("牛から", 1);
    mctx->endComboBox();
    mctx->genSlider("サイズ調整Ｘ", &mGetInTextSizeX, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mGetInTextSizeY, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mGetInTextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mGetInTextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mGetInTextAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ウエイトフレーム", &mGetInTextWaitFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファフレーム", &mGetInTextAlphaFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Get In!のピカリ調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("出現開始フレーム", &mGetInPikariAppearFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mGetInPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mGetInPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mGetInPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mGetInPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mGetInPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mGetInPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mGetInPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mGetInPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mGetInPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mGetInPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mGetInPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mGetInPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mGetInPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mGetInPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mGetInPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mGetInPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mGetInPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mGetInPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Start!!のピカリ調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("出現開始フレーム", &mStartPikariAppearFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mStartPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mStartPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mStartPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mStartPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mStartPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mStartPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mStartPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mStartPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mStartPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mStartPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mStartPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mStartPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mStartPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mStartPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mStartPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mStartPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mStartPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mStartPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("タイマー表示フレーム", &mStartPikariDisplayFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****READY? FIGHT!!の調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｘ", &mReadyFightTextSizeX, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ調整Ｙ", &mReadyFightTextSizeY, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mReadyFightTextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mReadyFightTextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mReadyFightTextAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ウエイトフレーム", &mReadyFightTextWaitFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファフレーム", &field_0x172, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****READY? FIGHT!!のピカリ調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("出現開始フレーム", &mReadyFightPikariAppearFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mReadyFightPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mReadyFightPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mReadyFightPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mReadyFightPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mReadyFightPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mReadyFightPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mReadyFightPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mReadyFightPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mReadyFightPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mReadyFightPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mReadyFightPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mReadyFightPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mReadyFightPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mReadyFightPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mReadyFightPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mReadyFightPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mReadyFightPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mReadyFightPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
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
    mctx->genSlider("拡大縮小（通常）", &mVesselScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（通常）", &mVesselPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（通常）", &mVesselPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（4:3）", &mVesselPosX_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（4;3）", &mVesselPosY_4x3, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ（通常）", &mVesselAlpha[0], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話）", &mVesselTalkScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話）", &mVesselTalkPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話）", &mVesselTalkPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ（会話）", &mVesselAlpha[1], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ（器）", &mVesselAlpha[2], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ（雫）", &mDropAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***光の器ゲット後***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mDropGetScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡縮アニメフレーム数", &mDropGetScaleAnimFrameNum, 0, 30, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***光の雫コンプリート後***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("個々ピカリ間隔", &mPikariInterval, 0, 30, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("個々～一斉ピカリ間隔", &field_0x54, -30, 30, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("パーツのスケール", &mDropScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー黒(R)", &mDropOnColorBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー黒(G)", &mDropOnColorBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー黒(B)", &mDropOnColorBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー黒(A)", &mDropOnColorBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー白(R)", &mDropOnColorWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー白(G)", &mDropOnColorWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー白(B)", &mDropOnColorWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ONカラー白(A)", &mDropOnColorWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー黒(R)", &mDropOffColorBlack.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー黒(G)", &mDropOffColorBlack.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー黒(B)", &mDropOffColorBlack.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー黒(A)", &mDropOffColorBlack.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー白(R)", &mDropOffColorWhite.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー白(G)", &mDropOffColorWhite.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー白(B)", &mDropOffColorWhite.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("OFFカラー白(A)", &mDropOffColorWhite.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***ピカリ***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（通常）", &mPikariScaleNormal, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（コンプ後）", &mPikariScaleComplete, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mDropPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mDropPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mDropPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mDropPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mDropPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mDropPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mDropPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mDropPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mDropPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mDropPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mDropPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mDropPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mDropPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mDropPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mDropPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mDropPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード（コンプ中）", &mDropPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード（コンプ後）", &mDropPikariAnimSpeed_Completed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***ピカリ（ループ再生）***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariLoopScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mPikariLoopFrontInner[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mPikariLoopFrontInner[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mPikariLoopFrontInner[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mPikariLoopFrontInner[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mPikariLoopFrontOuter[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mPikariLoopFrontOuter[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mPikariLoopFrontOuter[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mPikariLoopFrontOuter[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mPikariLoopBackInner[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mPikariLoopBackInner[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mPikariLoopBackInner[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mPikariLoopBackInner[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mPikariLoopBackOuter[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mPikariLoopBackOuter[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mPikariLoopBackOuter[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mPikariLoopBackOuter[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mPikariLoopAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("***ピカリ（ループＢＡＣＫ(停止)）***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariLoopBackScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mPikariLoopFrontInner[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mPikariLoopFrontInner[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mPikariLoopFrontInner[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mPikariLoopFrontInner[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mPikariLoopFrontOuter[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mPikariLoopFrontOuter[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mPikariLoopFrontOuter[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mPikariLoopFrontOuter[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mPikariLoopBackInner[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mPikariLoopBackInner[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mPikariLoopBackInner[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mPikariLoopBackInner[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mPikariLoopBackOuter[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mPikariLoopBackOuter[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mPikariLoopBackOuter[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mPikariLoopBackOuter[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("停止アニメフレーム", &mPikariLoopBackStopFrame, 0.0f, 60.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("アニメデバッグ", &mAnimDebug, 1, 0, NULL, -1, -1, 512, 24);
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
    static const char* base_text[2] = {
        "***アイテムあり***",
        "***アイテムなし***",
    };

    mctx->genNode("コレクト画面", &mCollectScreen, 0, 0);
    mctx->genNode("オプション画面", &mOptionScreen, 0, 0);
    mctx->genNode("強調ボタン", &mEmpButton, 0, 0);
    mctx->genNode("手紙セレクト画面", &mLetterSelectScreen, 0, 0);
    mctx->genNode("魚一覧画面", &mFishListScreen, 0, 0);
    mctx->genNode("虫一覧画面", &mInsectListScreen, 0, 0);
    mctx->genNode("奥義一覧画面", &mSkillListScreen, 0, 0);
    mctx->genNode("光の雫用", &mLightDrop, 0, 0);
    mctx->genNode("ミニゲーム用", &mMiniGame, 0, 0);
    field_0x8_debug = dMeter_map_HIO_c::mMySelfPointer;
    if (field_0x8_debug != NULL) {
      mctx->genNode("エリアマップ制御", field_0x8_debug, 0, 0);
    }
    mctx->genLabel("*****アイテムスケール割合*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mItemScaleAdjustON, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("割合", &mItemScalePercent, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* ライフゲージ・魔法メーターの調整 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　ライフの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ライフ１０以下*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mLifeTopPosX, -100.0f, 100.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mLifeTopPosY, -100.0f, 100.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****魔法が無い*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mNoMagicPosY, -100.0f, 100.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****全体の調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("全体の拡大縮小", &mParentScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("全体のアルファ", &mParentAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ライフゲージの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mLifeParentScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ハートマークのアルファ", &mLifeParentHeartAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ベースのアルファ", &mHeartBaseAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mLifeGaugePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mLifeGaugePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ハートマークの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mHeartMarkScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mHeartAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****大きいハートマークの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mBigHeartScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mBigHeartAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　魔法メーターの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****魔法メーターの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mMagicMeterScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("バーのアルファ", &mMagicMeterAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("枠のアルファ", &mMagicMeterFrameAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mMagicMeterPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mMagicMeterPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　カンテラメーターの調整　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****カンテラメーターの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mLanternMeterScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("バーのアルファ", &mLanternMeterAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("枠のアルファ", &mLanternMeterFrameAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mLanternMeterPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mLanternMeterPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　酸素メーターの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****酸素メーターの調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mOxygenMeterScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("バーのアルファ", &mOxygenMeterAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("枠のアルファ", &mOxygenMeterFrameAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mOxygenMeterPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mOxygenMeterPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　操作説明関連の調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　　全体の調整　　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mMainHUDButtonsScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mMainHUDButtonsAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mMainHUDButtonsPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mMainHUDButtonsPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　全体の調整（アイテム選択）　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mRingHUDButtonsScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mRingHUDButtonsAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mRingHUDButtonsPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mRingHUDButtonsPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　ボタン表示バックの調整　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonDisplayBackScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonDisplayBackAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ａボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonAScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonAAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonAPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonAPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話中）", &mButtonATalkScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話中）", &mButtonATalkPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話中）", &mButtonATalkPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話中A）", &mButtonATalkAScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話中A）", &mButtonATalkAPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話中A）", &mButtonATalkAPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（光の器）", &mButtonAVesselPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（光の器）", &mButtonAVesselPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ａフォントの調整　　　 　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("テキストデバッグ", (u8*)&mButtonATextDebug, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アクションＩＤ", &mButtonATextActionID, 0, 2000, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("字間", &mButtonATextSpacing, -5.0f, 5.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonATextScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonATextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonATextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話中）", &mButtonATalkScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話中）", &mButtonATalkPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話中）", &mButtonATalkPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話中A）", &mButtonATextTalkAScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話中A）", &mButtonATalkAPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話中A）", &mButtonATalkAPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｒ）", &mButtonATextColor.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｇ）", &mButtonATextColor.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｂ）", &mButtonATextColor.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ａ）", &mButtonATextColor.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ｂボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonBScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonBAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonBPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonBPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話中）", &mButtonBTalkScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話中）", &mButtonBTalkPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話中）", &mButtonBTalkPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（狼）", &mButtonBWolfPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（狼）", &mButtonBWolfPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（光の器）", &mButtonBVesselPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（光の器）", &mButtonBVesselPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ｂアイテムの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mButtonDebug[3], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonBItemPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonBItemPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("回転", &mButtonBItemRotation[0], 0.0f, 360.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonBItemScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（個数）", &mItemBNumPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（個数）", &mItemBNumPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（個数）", &mItemBNumScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(剣)", &mButtonBItemPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(剣)", &mButtonBItemPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("回転(剣)", &mButtonBItemRotation[1], 0.0f, 360.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小(剣)", &mButtonBItemScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(つり)", &mButtonBItemPosX[2], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(つり)", &mButtonBItemPosY[2], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("回転(つり)", &mButtonBItemRotation[2], 0.0f, 360.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小(つり)", &mButtonBItemScale[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　Ｂアイテムベースの調整　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 2; i++) {
          mctx->genLabel(base_text[i], 0, 0, NULL, -1, -1, 512, 24);
          mctx->genSlider("拡大縮小", &mItemBBaseScale[i], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
          mctx->genSlider("位置調整Ｘ", &mItemBBasePosX[i], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
          mctx->genSlider("位置調整Ｙ", &mItemBBasePosY[i], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
          mctx->genSlider("アルファ", &mItemBBaseAlpha[i], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    }
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ｂフォントの調整　　　 　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonBFontScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonBFontPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonBFontPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（会話中）", &mButtonBTalkScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（会話中）", &mButtonBTalkPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（会話中）", &mButtonBTalkPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｒ）", &mButtonBFontColor.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｇ）", &mButtonBFontColor.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｂ）", &mButtonBFontColor.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ａ）", &mButtonBFontColor.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　ミドナアイコンの調整　　 　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mMidnaIconScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mMidnaIconAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mMidnaIconPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mMidnaIconPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　ＸＹボタンの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイコンの半透明アルファ", &mButtonXYItemDimAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ボタンの半透明アルファ", &mButtonXYBaseDimAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ｘボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonXScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonXAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonXPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonXPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ｘアイテムの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mButtonDebug[0], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonXItemPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonXItemPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonXItemScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("回転", &mButtonItemRotation[0], 0.0f, 360.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（個数）", &mXItemNumPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（個数）", &mXItemNumPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（個数）", &mXItemNumScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　Ｘアイテムベースの調整　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 2; i++) {
        mctx->genLabel(base_text[i], 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("拡大縮小", &mButtonXItemBaseScale[i], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置調整Ｘ", &mButtonXItemBasePosX[i], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置調整Ｙ", &mButtonXItemBasePosY[i], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("アルファ", &mButtonXItemBaseAlpha[i], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    }
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ｙボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonYScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonYAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonYPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonYPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ｙアイテムの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mButtonDebug[1], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonYItemPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonYItemPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonYItemScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("回転", &mButtonItemRotation[1], 0.0f, 360.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ（個数）", &mYItemNumPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ（個数）", &mYItemNumPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小（個数）", &mYItemNumScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　Ｙアイテムベースの調整　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 2; i++) {
        mctx->genLabel(base_text[i], 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("拡大縮小", &mButtonYItemBaseScale[i], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置調整Ｘ", &mButtonYItemBasePosX[i], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("位置調整Ｙ", &mButtonYItemBasePosY[i], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("アルファ", &mButtonYItemBaseAlpha[i], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    }
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　ＸＹフォントの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonXYTextScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonXYTextPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonXYTextPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｒ）", &mButtonXYTextColor.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｇ）", &mButtonXYTextColor.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｂ）", &mButtonXYTextColor.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ａ）", &mButtonXYTextColor.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ｚボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonZScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonZAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonZPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonZPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ｚアイテムの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonZItemPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonZItemPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonZItemScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　Ｚアイテムベースの調整　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonZItemBaseScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonZItemBasePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonZItemBasePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonZItemBaseAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　Ｚフォントの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonZFontScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonZFontPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonZFontPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｒ）", &mButtonZFontColor.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｇ）", &mButtonZFontColor.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ｂ）", &mButtonZFontColor.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カラー（Ａ）", &mButtonZFontColor.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　ベース部分の調整　　　 　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonBaseAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　ルピー・カギの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　　全体の調整　　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mRupeeKeyScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mRupeeKeyPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mRupeeKeyPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mRupeeKeyAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　　ルピーの調整　　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mRupeeScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mRupeePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mRupeePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mRupeeAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　　カギの調整　　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mKeyScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mKeyPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mKeyPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mKeyAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　枠の模様の調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mRupeeFrameScale, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mRupeeFramePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mRupeeFramePosY, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mRupeeFrameAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　ルピーの数字の調整*の調整　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mRupeeCountScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mRupeeCountPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mRupeeCountPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mRupeeCountAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　カギの数字の調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mKeyNumScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mKeyNumPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mKeyNumPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mKeyNumAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　　拍車の調整　　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ＯＮ", (u8*)&mSpurDebug, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　拍車アイコンの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mSpurIconScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mSpurIconAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　非表示アイコン（黒ベタ）の調整　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mUsedSpurIconScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mUsedSpurIconAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　表示可能領域の調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mSpurBarScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mSpurBarPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mSpurBarPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拍車の数", &mMaxSpurAmount, '\x03', '\f', 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ａボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonAHorseScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mButtonAHorsePosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mButtonAHorsePosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　　ピカリの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ａボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonAPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mButtonAPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mButtonAPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mButtonAPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mButtonAPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mButtonAPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mButtonAPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mButtonAPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mButtonAPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mButtonAPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mButtonAPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mButtonAPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mButtonAPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mButtonAPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mButtonAPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mButtonAPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mButtonAPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mButtonAPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ｂボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonBPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mButtonBPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mButtonBPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mButtonBPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mButtonBPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mButtonBPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mButtonBPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mButtonBPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mButtonBPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mButtonBPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mButtonBPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mButtonBPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mButtonBPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mButtonBPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mButtonBPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mButtonBPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mButtonBPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mButtonBPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　　Ｚボタンの調整　　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonZPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mButtonZPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mButtonZPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mButtonZPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mButtonZPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mButtonZPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mButtonZPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mButtonZPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mButtonZPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mButtonZPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mButtonZPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mButtonZPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mButtonZPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mButtonZPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mButtonZPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mButtonZPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mButtonZPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mButtonZPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　ＸＹボタンの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonXYPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mButtonXYPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mButtonXYPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mButtonXYPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mButtonXYPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mButtonXYPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mButtonXYPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mButtonXYPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mButtonXYPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mButtonXYPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mButtonXYPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mButtonXYPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mButtonXYPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mButtonXYPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mButtonXYPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mButtonXYPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mButtonXYPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mButtonXYPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　拍車アイコンの調整　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mSpurIconPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mSpurIconPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mSpurIconPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mSpurIconPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mSpurIconPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mSpurIconPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mSpurIconPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mSpurIconPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mSpurIconPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mSpurIconPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mSpurIconPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mSpurIconPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mSpurIconPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mSpurIconPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mSpurIconPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mSpurIconPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mSpurIconPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mSpurIconPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　拍車アイコンRevive　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mSpurIconRevivePikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mSpurIconRevivePikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mSpurIconRevivePikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mSpurIconRevivePikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mSpurIconRevivePikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mSpurIconRevivePikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mSpurIconRevivePikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mSpurIconRevivePikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mSpurIconRevivePikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mSpurIconRevivePikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mSpurIconRevivePikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mSpurIconRevivePikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mSpurIconRevivePikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mSpurIconRevivePikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mSpurIconRevivePikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mSpurIconRevivePikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mSpurIconRevivePikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mSpurIconRevivePikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　ミドナボタンピカリ　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ボタン点滅", &mMidnaIconFlashRate, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mMidnaIconPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &mMidnaIconPikariFrontInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &mMidnaIconPikariFrontInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &mMidnaIconPikariFrontInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &mMidnaIconPikariFrontInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &mMidnaIconPikariFrontOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &mMidnaIconPikariFrontOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &mMidnaIconPikariFrontOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &mMidnaIconPikariFrontOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &mMidnaIconPikariBackInner.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &mMidnaIconPikariBackInner.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &mMidnaIconPikariBackInner.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &mMidnaIconPikariBackInner.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &mMidnaIconPikariBackOuter.r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &mMidnaIconPikariBackOuter.g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &mMidnaIconPikariBackOuter.b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &mMidnaIconPikariBackOuter.a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mMidnaIconPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　　　十字ボタン　　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mButtonCrossScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(OFF)", &mButtonCrossOFFPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(OFF)", &mButtonCrossOFFPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ(ON)", &mButtonCrossONPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ(ON)", &mButtonCrossONPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("文字拡大縮小", &mButtonCrossTextScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mButtonCrossAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ITEMアルファ", &mButtonCrossITEMAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("MAPアルファ", &mButtonCrossMAPAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("移動フレーム", &mButtonCrossMoveFrame, 1, 0x1e, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　　ハイハイ矢印の調整　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("全表示", (u8*)&mScrollArrowDisplayAll, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小Ｘ", &mScrollArrowScaleX, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小Ｙ", &mScrollArrowScaleY, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("BCKアニメスピード", &mScrollArrowBCKAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("BPKアニメスピード", &mScrollArrowBPKAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("BTKアニメスピード", &mScrollArrowBTKAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("上下用位置Ｘ", &mScrollArrowPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("上下用位置Ｙ", &mScrollArrowPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("左右用位置Ｘ", &mScrollArrowPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("左右用位置Ｙ", &mScrollArrowPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("中心位置Ｘ", &mScrollArrowCenterPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("中心位置Ｙ", &mScrollArrowCenterPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　Ｗｉｉロック矢印の調整　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("全表示", (u8*)&mWiiLockArrowDisplayAll, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小Ｘ", &mWiiLockArrowScaleX, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小Ｙ", &mWiiLockArrowScaleY, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("BCKアニメスピード", &mWiiLockArrowBCKAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("BPKアニメスピード", &mWiiLockArrowBPKAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("BTKアニメスピード", &mWiiLockArrowBTKAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("上下用位置Ｘ", &mWiiLockArrowPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("上下用位置Ｙ", &mWiiLockArrowPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("左右用位置Ｘ", &mWiiLockArrowPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("左右用位置Ｙ", &mWiiLockArrowPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　タッチエリアの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　　選択中カラーの調整　　　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分１Ｒ", &mTouchAreaSelectBlack[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分１Ｇ", &mTouchAreaSelectBlack[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分１Ｂ", &mTouchAreaSelectBlack[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分１Ａ", &mTouchAreaSelectBlack[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分１Ｒ", &mTouchAreaSelectWhite[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分１Ｇ", &mTouchAreaSelectWhite[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分１Ｂ", &mTouchAreaSelectWhite[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分１Ａ", &mTouchAreaSelectWhite[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分２Ｒ", &mTouchAreaSelectBlack[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分２Ｇ", &mTouchAreaSelectBlack[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分２Ｂ", &mTouchAreaSelectBlack[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分２Ａ", &mTouchAreaSelectBlack[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分２Ｒ", &mTouchAreaSelectWhite[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分２Ｇ", &mTouchAreaSelectWhite[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分２Ｂ", &mTouchAreaSelectWhite[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分２Ａ", &mTouchAreaSelectWhite[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("フレーム数", &mTouchAreaSelectFrameNum, 1, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　　非選択中カラーの調整　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ｒ", &mTouchAreaSelectBlack[2].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ｇ", &mTouchAreaSelectBlack[2].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ｂ", &mTouchAreaSelectBlack[2].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ａ", &mTouchAreaSelectBlack[2].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ｒ", &mTouchAreaSelectWhite[2].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ｇ", &mTouchAreaSelectWhite[2].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ｂ", &mTouchAreaSelectWhite[2].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ａ", &mTouchAreaSelectWhite[2].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　タッチエリア（ＭＡＰ）の調整　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("選択時スケール", &mTouchAreaSelectScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非選択時スケール", &mTouchAreaUnselectScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mTouchAreaScale[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アルファ", &mTouchAreaAlpha[0], 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mTouchAreaPosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mTouchAreaPosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*タッチエリア（エリアマップ）の調整*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("選択時スケール", &mTouchAreaSelectScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非選択時スケール", &mTouchAreaUnselectScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mTouchAreaScale[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mTouchAreaPosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mTouchAreaPosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　タッチエリア（ＩＴＥＭ）の調整　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("選択時スケール", &mTouchAreaSelectScale[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非選択時スケール", &mTouchAreaUnselectScale[2], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mTouchAreaPosX[2], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mTouchAreaPosY[2], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("* 　　バーのテキストの調整　　　　 *", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("MAPスケール", &mDpadMAPScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("MAPトランスＸ", &mDpadMAPPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("MAPトランスＹ", &mDpadMAPPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　フローティングメッセージの調整　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("メッセージデバッグ", (u8*)&mMessageDebug, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("メッセージＩＤ", (s32*)&mMessageID, 0, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("フローデバッグ", (u8*)&mFlowDebug, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("メッセージＩＤ", (s32*)&mFlowID, 0, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("表示時間", &mMessageDuration, 0, 1000, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mFloatingMessagePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mFloatingMessagePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*　　　カンテラアイコンメーター　　*", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("************************************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｘ", &mLanternIconMeterPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("位置調整Ｙ", &mLanternIconMeterPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("サイズ", &mLanternIconMeterSize, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
}

void dMeter_drawHIO_c::updateFMsgDebug() {
    JORMContext* mctx = attachJORMContext(8);
    mctx->startUpdateNode(this);
    mctx->updateCheckBox(2, (u8*)&mMessageDebug, 1, 0);
    mctx->updateCheckBox(2, (u8*)&mFlowDebug, 1, 0);
    mctx->endUpdateNode();
    releaseJORMContext(mctx);
}
#endif

dMeter_ringHIO_c::dMeter_ringHIO_c() {
#if WIDESCREEN_SUPPORT
    updateOnWide();
#else
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
#endif
}

#if WIDESCREEN_SUPPORT
void dMeter_ringHIO_c::updateOnWide() {
    mRingRadiusH = 180.0f;
    mRingRadiusV = 170.0f;

    mOpenFrames = 5;
    mCloseFrames = 5;

    mSelectItemScale = 1.4f;
    mUnselectItemScale = 0.9f;
    mSelectButtonScale = 1.3f;
    mUnselectButtonScale = 1.0f;
    mCursorScale = 1.0f;

    mItemNum = 8;

    mInactiveItemScale = 0.4f;
    mItemAlphaFlashDuration = 300;
    mItemAlphaMin = 255;
    mItemAlphaMax = 255;

    mOffsetLineDisplay = 0;

    mItemIconAlpha = 255;
    mItemIconAlpha_Wolf = 110;

    mItemFrame[SELECT_FRAME].r = 255;
    mItemFrame[SELECT_FRAME].g = 255;
    mItemFrame[SELECT_FRAME].b = 255;
    mItemFrame[SELECT_FRAME].a = 0;
    mItemFrame[UNSELECT_FRAME].r = 255;
    mItemFrame[UNSELECT_FRAME].g = 255;
    mItemFrame[UNSELECT_FRAME].b = 140;
    mItemFrame[UNSELECT_FRAME].a = 0;

    mItemNamePosX = 135.0f;
    mItemNamePosY = 176.0f;
    mItemNameScale = 1.2f;

    mCenterPosX = 0.0f;
    mCenterPosY = 0.0f;
    mCenterScale = 1.0f;
    mOverlayAlpha = 0.3f;

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

    mItemRingPosX = -63.0f;
    mItemRingPosY = -5.0f;
    mRingPosX = -47.0f;
    mRingPosY = -5.0f;
    mRingAlpha = 0.65;
    mRingAlpha_Wolf = 0.35f;
    mRingScaleH = 1.2f;
    mRingScaleV = 1.43f;

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

    mCursorInitSpeed = 5000;
    mCursorMax = 10000;
    mCursorAccel = 3000;
    mCursorChangeWaitFrames = 2;
    mDirectSelectWaitFrames = 4;

    field_0x10 = 120.0f;
    field_0x13e = 12;
    field_0x140 = 2048;
    field_0x142 = 128;

    field_0x44 = 0.0f;
    field_0x48 = 0.0f;
    field_0x4c = 1.0f;
    field_0x154 = 255;

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
}

void dMeter_ringHIO_c::updateOffWide() {
    mRingRadiusH = 170.0f;
    mRingRadiusV = 160.0f;

    mOpenFrames = 5;
    mCloseFrames = 5;

    mSelectItemScale = 1.4f;
    mUnselectItemScale = 0.9f;
    mSelectButtonScale = 1.4f;
    mUnselectButtonScale = 1.0f;
    mCursorScale = 1.0f;

    mItemNum = 8;

    mInactiveItemScale = 0.4f;
    mItemAlphaFlashDuration = 300;
    mItemAlphaMin = 255;
    mItemAlphaMax = 255;

    mOffsetLineDisplay = 0;

    mItemIconAlpha = 255;
    mItemIconAlpha_Wolf = 110;

    mItemFrame[SELECT_FRAME].r = 255;
    mItemFrame[SELECT_FRAME].g = 255;
    mItemFrame[SELECT_FRAME].b = 255;
    mItemFrame[SELECT_FRAME].a = 0;
    mItemFrame[UNSELECT_FRAME].r = 255;
    mItemFrame[UNSELECT_FRAME].g = 255;
    mItemFrame[UNSELECT_FRAME].b = 140;
    mItemFrame[UNSELECT_FRAME].a = 0;

    mItemNamePosX = 143.0f;
    mItemNamePosY = 177.0f;
    mItemNameScale = 1.2f;

    mCenterPosX = 0.0f;
    mCenterPosY = 0.0f;
    mCenterScale = 1.0f;
    mOverlayAlpha = 0.3f;

    mGuidePosX[8] = 0.0f;
    mGuidePosY[8] = -60.0f;
    mGuideScale[8] = 0.7;

    mGuidePosX[9] = 0.0f;
    mGuidePosY[9] = -50.0f;
    mGuideScale[9] = 1.0f;

    mGuidePosX[SET_ITEM] = 0.0f;
    mGuidePosY[SET_ITEM] = 0.0f;
    mGuideScale[SET_ITEM] = 1.0f;

    mGuidePosX[5] = 11.0f;
    mGuidePosY[5] = -1.3f;
    mGuideScale[5] = 1.4f;

    mGuidePosX[6] = 0.0f;
    mGuidePosY[6] = -11.0f;
    mGuideScale[6] = 1.0f;

    mGuidePosX[7] = 0.0f;
    mGuidePosY[7] = -11.0f;
    mGuideScale[7] = 1.0f;

    mGuidePosX[COMBO_ITEM] = -23.0f;
    mGuidePosY[COMBO_ITEM] = 0.0f;
    mGuideScale[COMBO_ITEM] = 1.0f;

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

    mItemRingPosX = -38.0f;
    mItemRingPosY = 0.0f;
    mRingPosX = -38.0f;
    mRingPosY = 1.3f;
    mRingAlpha = 0.65f;
    mRingAlpha_Wolf = 0.35f;
    mRingScaleH = 1.6f;
    mRingScaleV = 1.5f;

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

    mCursorInitSpeed = 5000;
    mCursorMax = 10000;
    mCursorAccel = 3000;
    mCursorChangeWaitFrames = 2;
    mDirectSelectWaitFrames = 4;

    field_0x10 = 120.0f;
    field_0x13e = 12;
    field_0x140 = 2048;
    field_0x142 = 128;

    field_0x44 = 0.0f;
    field_0x48 = 0.0f;
    field_0x4c = 1.0f;
    field_0x154 = 255;

    mGuidePosX[DIRECT_SELECT] = 0.0f;
    mGuidePosY[DIRECT_SELECT] = 0.0f;
    mGuideScale[DIRECT_SELECT] = 1.0f;

    mGuidePosX[2] = 0.0f;
    mGuidePosY[2] = 0.0f;
    mGuideScale[2] = 1.0f;

    mGuidePosX[ROTATE] = 0.0f;
    mGuidePosY[ROTATE] = 0.0f;
    mGuideScale[ROTATE] = 1.0f;
}
#endif

#if DEBUG
void dMeter_ringHIO_c::genMessage(JORMContext* mctx) {
    mctx->genSlider("リング横半径", &mRingRadiusH, 50.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リング縦半径", &mRingRadiusV, 50.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("表示フレーム", &mOpenFrames, 1, 0xf, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非表示フレーム", &mCloseFrames, 1, 0xf, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（選択）", &mSelectItemScale, 0.5f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムスケール（非選択）", &mUnselectItemScale, 0.5f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ボタンスケール（選択）", &mSelectButtonScale, 0.5f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ボタンスケール（非選択）", &mUnselectButtonScale, 0.5f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("カーソルスケール", &mCursorScale, 0.5f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム数", &mItemNum, '\x02', '\x15', 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アクティブでないアイテムスケール", &mInactiveItemScale, 0.1f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アクティブなアルファ点滅スピード", &mItemAlphaFlashDuration, 2, 300, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アクティブなアルファ最小値", &mItemAlphaMin, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アクティブなアルファ最大値", &mItemAlphaMax, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("オフセットライン表示", &mOffsetLineDisplay, '\x01', 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムアイコン明度", &mItemIconAlpha, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテムアイコン明度（狼）", &mItemIconAlpha_Wolf, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠のＲ（選択中）", &mItemFrame[0].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠のＧ（選択中）", &mItemFrame[0].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠のＢ（選択中）", &mItemFrame[0].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠の明度（選択中）", &mItemFrame[0].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠のＲ", &mItemFrame[1].r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠のＧ", &mItemFrame[1].g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠のＢ", &mItemFrame[1].b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白い枠の明度", &mItemFrame[1].a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム名トランスＸ", &mItemNamePosX, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム名トランスＹ", &mItemNamePosY, -500.0f, 500.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム名サイズ", &mItemNameScale, 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("センターＸ座標", &mCenterPosX, -100.0f, 100.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("センターＹ座標", &mCenterPosY, -100.0f, 100.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("センターサイズ", &mCenterScale, 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スポットアルファ", &mOverlayAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("セットｘｙＸ座標", &mGuidePosX[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("セットｘｙＹ座標", &mGuidePosY[0], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("セットｘｙサイズ", &mGuideScale[0], 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ダイレクト選択Ｘ座標", &mGuidePosX[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ダイレクト選択Ｙ座標", &mGuidePosY[1], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ダイレクト選択サイズ", &mGuideScale[1], 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("選択Ｘ座標", &mGuidePosX[3], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("選択Ｙ座標", &mGuidePosY[3], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("選択サイズ", &mGuideScale[3], 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("合成Ｘ座標", &mGuidePosX[4], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("合成Ｙ座標", &mGuidePosY[4], -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("合成サイズ", &mGuideScale[4], 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("SELECT トランスＸ", &mSelectPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("SELECT トランスＹ", &mSelectPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("SELECT スケールＸ", &mSelectScaleX, 0.0f, 5.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("SELECT スケールＹ", &mSelectScaleY, 0.0f, 5.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム説明トランスＸ", &mItemDescPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム説明トランスＹ", &mItemDescPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム説明サイズ", &mItemDescScale, 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アイテム説明アルファ", &mItemDescAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("タイトル部分トランスＸ", &mItemDescTitlePosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("タイトル部分トランスＹ", &mItemDescTitlePosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("タイトル部分サイズ", &mItemDescTitleScale, 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングのトランスＸ", &mItemRingPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングのトランスＹ", &mItemRingPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングの内側の円トランスＸ", &mRingPosX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングの内側の円トランスＹ", &mRingPosY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングの内側の円アルファ", &mRingAlpha, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングの内側の円アルファ（狼）", &mRingAlpha_Wolf, 0.0f, 1.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングの内側の円横スケール", &mRingScaleH, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リングの内側の円縦スケール", &mRingScaleV, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****ピカリ調整*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大縮小", &mPikariScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(R)", &(mPikariFrontInner).r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(G)", &(mPikariFrontInner).g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(B)", &(mPikariFrontInner).b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R0(A)", &(mPikariFrontInner).a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(R)", &(mPikariFrontOuter).r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(G)", &(mPikariFrontOuter).g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(B)", &(mPikariFrontOuter).b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moya00 R1(A)", &(mPikariFrontOuter).a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(R)", &(mPikariBackInner).r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(G)", &(mPikariBackInner).g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(B)", &(mPikariBackInner).b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R0(A)", &(mPikariBackInner).a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(R)", &(mPikariBackOuter).r, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(G)", &(mPikariBackOuter).g, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(B)", &(mPikariBackOuter).b, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("moyabs R1(A)", &(mPikariBackOuter).a, 0, 255, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("アニメスピード", &mPikariAnimSpeed, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****青沼さん用*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("初速", &mCursorInitSpeed, 0, 5000, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("Ｍａｘ", &mCursorMax, 0, 10000, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("加速度", &mCursorAccel, 0, 3000, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("回転ウエイト", &mCursorChangeWaitFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ダイレクト選択回転ウエイト", &mDirectSelectWaitFrames, 0, 100, 0, NULL, -1, -1, 512, 24);
}

BOOL subJOREventCallbackListNode::JORAct(u32 param_1, const char* param_2) {
    UNUSED(param_1);
    JORFile file;
    if (file.open(param_2, 1, "", NULL, NULL, NULL)) {
        if (!g_fmapHIO.mpArcFile) {
            g_fmapHIO.mpArcFile = mDoExt_getArchiveHeap()->alloc(0x46000, 0x20);
            JUT_ASSERT(6390, g_fmapHIO.mpArcFile != NULL);
        }
        if (!g_fmapHIO.mpDecompBuf) {
            g_fmapHIO.mpDecompBuf = mDoExt_getArchiveHeap()->alloc(0x46000, 0x20);
            JUT_ASSERT(6395, g_fmapHIO.mpDecompBuf != NULL);
        }
        memset(g_fmapHIO.mpArcFile, 0, 0x46000);
        memset(g_fmapHIO.mpDecompBuf, 0, 0x46000);
        file.readData(g_fmapHIO.mpArcFile, 0);
        file.close();
        if (g_fmapHIO.mpArcData) {
            JKRUnmountArchive(g_fmapHIO.mpArcData);
            delete g_fmapHIO.mpArcData;
            g_fmapHIO.mpArcData = NULL;
        }
        JKRDecompress((u8*)g_fmapHIO.mpArcFile, (u8*)g_fmapHIO.mpDecompBuf, 0x46000, 0);
        g_fmapHIO.mpArcData = new(mDoExt_getArchiveHeap(), 0) JKRMemArchive(g_fmapHIO.mpDecompBuf, 0x46000, JKRMEMBREAK_FLAG_UNKNOWN0);
        JUT_ASSERT(6413, g_fmapHIO.mpArcData != NULL);
        if (dComIfGp_isHeapLockFlag() == 2) {
            g_fmapHIO.field_0x2f9 = true;
        }
        OSReport("open file!\n");
    } else {
        OSReport("open error!\n");
    }
    return TRUE;
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
    mctx->genLabel("\n*****アイコン　表示調整*****\n", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　フィールド　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ポータルカーソル　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mPortalCursorScale, 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ポータルアイコン　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mPortalIconScale, 0.0f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　リンク　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mLinkScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mLinkZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　目的地　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mDestinationScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mDestinationZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　光の雫　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mLightDropScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mLightDropZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　光の雫（点滅処理）　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("フレーム数", &mLightDropFlashFrameNum, 0, 200, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("始スケール(out)", &mLightDropFlashStartScaleOut[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("終スケール(out)", &mLightDropFlashEndScaleOut[0], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("始アルファ(out)", &mLightDropFlashStartAlphaOut[0], 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("終アルファ(out)", &mLightDropFlashEndAlphaOut[0], 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("始スケール(in)", &mLightDropFlashStartScaleOut[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("終スケール(in)", &mLightDropFlashEndScaleOut[1], 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("始アルファ(in)", &mLightDropFlashStartAlphaOut[1], 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("終アルファ(in)", &mLightDropFlashEndAlphaOut[1], 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　雪男　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mYetoScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mYetoZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　雪女　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mYetaScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mYetaZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ゴールドウルフ　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mGoldWolfScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mGoldWolfZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　馬車　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mWagonScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mWagonZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ステージ出入口　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mDungeonEntranceScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mDungeonEntranceZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　入室位置　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mRoomEntranceScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mRoomEntranceZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　入室位置LV8　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mLV8EntranceScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mLV8EntranceZoomScale[0], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ダンジョン　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　リンク　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mLinkScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mLinkZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ダンジョンワープ位置　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mDungeonWarpScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mDungeonWarpZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　目的地　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mDestinationScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mDestinationZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ボス敵　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mBossScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mBossZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ガノン　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mGanonScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mGanonZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　光の玉　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mLightOrbScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mLightOrbZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　鉄玉　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mCannonBallScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mCannonBallZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　猿（ＮＰＣ）　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mMonkeyScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mMonkeyZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　天空人（おばちゃん）　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mOoccooScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mOoccooZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　天空人（壺）　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mOoccooPotScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mOoccooPotZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　コピーロッド石像　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mCopyRodStatueScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mCopyRodStatueZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　小さな鍵　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mSmallKeyScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mSmallKeyZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　宝箱　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mChestScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mChestZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　ステージ出入口　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mDungeonEntranceScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mDungeonEntranceZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　入室位置　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mRoomEntranceScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mRoomEntranceZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　入室位置　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("通常時スケール", &mLV8EntranceScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("拡大時スケール", &mLV8EntranceZoomScale[1], 0.01f, 2.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　LV5ダンジョンアイテムデバッグ　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mLV5DungeonItemDebug, 1, 0, NULL, -1, -1, 512, 24);
    mctx->startComboBox("アイテム", &mLV5DungeonItem, 0, NULL, -1, -1, 0x100, 0x1a);
    mctx->genComboBoxItem("なし", 0xff);
    mctx->genComboBoxItem("トマトピューレ", 0xf4);
    mctx->genComboBoxItem("隠し味の素", 0xf5);
    mctx->genComboBoxItem("ボス鍵", 0xf6);
    mctx->endComboBox();
    mctx->genLabel("\n*****　アイコンデバッグ表示　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("***アイコンデバッグ***", (u8*)&mIconDebug, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ポータル", (u8*)&mIconDisplay, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("リンク", (u8*)&mIconDisplay[1], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ダンジョンワープ位置", (u8*)&mIconDisplay[2], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("目的地", (u8*)&mIconDisplay[3], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ボス敵", (u8*)&mIconDisplay[4], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ガノン", (u8*)&mIconDisplay[5], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("光の雫", (u8*)&mIconDisplay[6], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("光の玉", (u8*)&mIconDisplay[7], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("鉄玉", (u8*)&mIconDisplay[8], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("雪男", (u8*)&mIconDisplay[9], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("雪女", (u8*)&mIconDisplay[10], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ゴールドウルフ", (u8*)&mIconDisplay[0xb], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("猿（ＮＰＣ）", (u8*)&mIconDisplay[0xc], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("天空人（おばちゃん）", (u8*)&mIconDisplay[0xd], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("天空人（壺）", (u8*)&mIconDisplay[0xe], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("コピーロッド石像", (u8*)&mIconDisplay[0xf], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("馬車", (u8*)&mIconDisplay[0x10], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("小さな鍵", (u8*)&mIconDisplay[0x11], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("宝箱", (u8*)&mIconDisplay[0x12], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ステージ出入口", (u8*)&mIconDisplay[0x13], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("入室位置", (u8*)&mIconDisplay[0x14], 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ダンジョン出入り口LV8", (u8*)&mIconDisplay[0x15], 1, 0, NULL, -1, -1, 512, 24);
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
    static const char* txt[8] = {
        "***リージョン１***",
        "***リージョン２***",
        "***リージョン３***",
        "***リージョン４***",
        "***リージョン５***",
        "***リージョン６***",
        "***リージョン７***",
        "***BG***",
    };
    static const char* title[3] = {
        "******　ワールド全体　******",
        "******　リージョン全体　******",
        "******　リージョン拡大　******",
    };

    mctx->genNode("アイコン", &mMapIconHIO, 0, 0);
    mctx->genLabel("*****フィールドマップ調整項目*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ワールドグリッド表示", (u8*)&mDisplayWorldGrid, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ワールドグリッド幅", &mWorldGridWidth, 0.0, 100000.0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("リージョングリッド表示", (u8*)&mDisplayRegionGrid, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リージョングリッド幅", &mRegionGridWidth, 0.0, 100000.0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ワールド原点表示", (u8*)&mDisplayWorldOrigin, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("リージョン原点表示", (u8*)&mDisplayRegionOrigin, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ステージ原点表示", (u8*)&mDisplayStageOrigin, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リージョン拡大表示範囲", &mRegionZoomRange, 1000.0, 1000000.0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("表示基準領域枠表示", (u8*)&mDisplayReferenceArea, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("スクロール範囲を表示基準", (u8*)&field_0x308, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("左上座標Ｘ", &mMapTopLeftPosX, 0.0, 608.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("左上座標Ｙ", &mMapTopLeftPosY, 0.0, 448.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("領域幅", &mMapScale, 0.0, 608.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n***　スクロール速度境界　***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("０～遅", &mScrollSpeedSlowBound, 0.0, 1.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("遅～速", &mScrollSpeedFastBound, 0.0, 1.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n***　スクロール速度（遅）　***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ワールド全体", &mScrollSpeedWorldSlow, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リージョン全体", &mScrollSpeedRegionSlow, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リージョン拡大", &mScrollSpeedRegionZoomSlow, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ダンジョンマップ", &mScrollSpeedDungeonMapSlow, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n***　スクロール速度（速）　***", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ワールド全体", &mScrollSpeedWorldFast, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リージョン全体", &mScrollSpeedRegionFast, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("リージョン拡大", &mScrollSpeedRegionZoomFast, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ダンジョンマップ", &mScrollSpeedDungeonMapFast, 0.0, 10.0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ターミナル出力", (u8*)&mTerminalOutput, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ポータルデータターミナル出力", (u8*)&mPortalDataTerminalOutput, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("リージョン到達ビットターミナル出力", (u8*)&mRegionBitTerminalOutput, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("全て到達済み", (u8*)&mAllRegionsUnlocked, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("パス全表示", (u8*)&mDisplayAllPaths, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　デバッグ用到達ビット　*****", 0, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 7; i++) {
        mctx->genCheckBox(txt[i], (u8*)&mDebugRegionBits[i], 1, 0, NULL, -1, -1, 512, 24);
    }
    mctx->genLabel("到達リージョンPRINT", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genButton("PRINT", 0x40000002, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("全世界スクロール", (u8*)&mRegionPrintGlobalScroll, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("背景画像描画ON", (u8*)&mDrawBackground, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("背景アルファ", &mBackgroundAlpha, 0.0, 1.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("背景アルファ（前面）", &mBackgroundFrontAlpha, 0.0, 1.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ステージ拡大スケール", &mStageZoomScale, 1.0, 100.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n*****　背景画像のワールド座標オフセット　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ON", (u8*)&mRegionImageDebug, 1, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 8; i++) {
        mctx->genLabel(txt[i], 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("座標Ｘ", &mRegionImagePosX[i], -300000.0, 300000.0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("座標Ｚ", &mRegionImagePosZ[i], -300000.0, 300000.0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("スケール", &mRegionImageScale[i], -50.0, 50.0, 0, NULL, -1, -1, 512, 24);
    }
    mctx->genLabel("\n*****　スクロール範囲　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("表示ON", (u8*)&mScrollRangeDisplay, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("調整ON", (u8*)&mScrollRangeDebugON, 1, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 7; i = i + 1) {
        mctx->genLabel(txt[i], 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("座標最小Ｘ", &mRegionScrollRangeMinX[i], -300000.0, 300000.0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("座標最小Ｚ", &mRegionScrollRangeMinZ[i], -300000.0, 300000.0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("座標最大Ｘ", &mRegionScrollRangeMaxX[i], -300000.0, 300000.0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("座標最大Ｚ", &mRegionScrollRangeMaxZ[i], -300000.0, 300000.0, 0, NULL, -1, -1, 512, 24);
    }
    mctx->genLabel("\n*****　範囲チェック　*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ON", (u8*)&mRangeCheck, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("描画の優先", (u8*)&mRangeCheckDrawPriority, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("間隔", &mRangeCheckInterval, 0, '\n', 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ポータルワープＯＮ", (u8*)&mPortalWarpON, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("アーカイブ読み込み", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genButton("READ FILE", 0x40000001, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****タイトル部分****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mTitlePosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mTitlePosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mTitleScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****サブタイトル部分****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mSubTitlePosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mSubTitlePosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mSubTitleScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****エリア移動****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mAreaMovementPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mAreaMovementPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mAreaMovementScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****しまう****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mClosingPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mClosingPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mClosingScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｄｏアイコン****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mDoIconPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mDoIconPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mDoIconScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("ボタンデバッグＯＮ", (u8*)&mButtonDebugON, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｚボタン****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mButtonZPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mButtonZPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonZScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ａボタン****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mButtonAPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mButtonAPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonAScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｂボタン****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mButtonBPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mButtonBPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonBScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｚテキスト****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mButtonZTextPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mButtonZTextPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonZTextScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ａテキスト****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mButtonATextPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mButtonATextPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonATextScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****Ｂテキスト****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＸ", &mButtonBTextPosX, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("トランスＹ", &mButtonBTextPosY, -300.0, 300.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mButtonBTextScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("\n\n\n*****表示・非表示*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("表示フレーム数", &mDisplayFrameNum, 1, 100, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("非表示フレーム数", &mUndisplayFrameNum, 1, 100, 0, NULL, -1, -1, 512, 24);
    mctx->startComboBox("出現マップタイプ", &mMapType, 0, NULL, -1, -1, 0x100, 0x1a);
    mctx->genComboBoxItem("デフォルト", 0);
    mctx->genComboBoxItem("全体マップ", 1);
    mctx->genComboBoxItem("ダンジョンマップ", 2);
    mctx->genComboBoxItem("ハイラル城マップ", 3);
    mctx->endComboBox();
    field_0x14 = dMfm_HIO_c::mMySelfPointer;
    if (field_0x14 != NULL) {
        mctx->genNode("フィールドマップパス関係", field_0x14, 0, 0);
    }
    field_0x18 = dMdm_HIO_c::mMySelfPointer;
    if (field_0x18 != NULL) {
        mctx->genNode("ダンジョンマップ", field_0x18, 0, 0);
    }
    field_0x20 = dMf_HIO_c::mMySelfPointer;
    if (field_0x20 != NULL) {
        mctx->genNode("フィールドマップ", field_0x20, 0, 0);
    }
    mctx->genLabel("**********************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*******カーソル*******", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genCheckBox("デバッグＯＮ", (u8*)&mCursorDebugON, 1, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ｒ", &mCursorBlack.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ｇ", &mCursorBlack.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ｂ", &mCursorBlack.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("黒成分Ａ", &mCursorBlack.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ｒ", &mCursorWhite.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ｇ", &mCursorWhite.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ｂ", &mCursorWhite.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("白成分Ａ", &mCursorWhite.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mCursorScale, 0.0, 3.0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スピード", &mCursorSpeed, 0.0, 360.0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********************", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****カラー・点滅*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("**********************", 0, 0, NULL, -1, -1, 512, 24);
    for (int i = 0; i < 3; i++) {
        mctx->genLabel(title[i], 0, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("■選択されているリージョン", 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｒ", &mMapBlink[i].mSelectedRegion.mBlack.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｇ", &mMapBlink[i].mSelectedRegion.mBlack.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｂ", &mMapBlink[i].mSelectedRegion.mBlack.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ａ", &mMapBlink[i].mSelectedRegion.mBlack.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｒ", &mMapBlink[i].mSelectedRegion.mWhite.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｇ", &mMapBlink[i].mSelectedRegion.mWhite.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｂ", &mMapBlink[i].mSelectedRegion.mWhite.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ａ", &mMapBlink[i].mSelectedRegion.mWhite.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("最大値", &mMapBlink[i].mSelectedRegion.mMax, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("最小値", &mMapBlink[i].mSelectedRegion.mMin, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("点滅スピード", &mMapBlink[i].mSelectedRegion.mBlinkSpeed, 0, 100, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("■選択されていないリージョン", 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｒ", &mMapBlink[i].mUnselectedRegion.mBlack.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｇ", &mMapBlink[i].mUnselectedRegion.mBlack.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｂ", &mMapBlink[i].mUnselectedRegion.mBlack.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ａ", &mMapBlink[i].mUnselectedRegion.mBlack.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｒ", &mMapBlink[i].mUnselectedRegion.mWhite.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｇ", &mMapBlink[i].mUnselectedRegion.mWhite.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｂ", &mMapBlink[i].mUnselectedRegion.mWhite.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ａ", &mMapBlink[i].mUnselectedRegion.mWhite.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("■未到達リージョン", 0, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｒ", &mMapBlink[i].mUnreachedRegionBlack.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｇ", &mMapBlink[i].mUnreachedRegionBlack.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ｂ", &mMapBlink[i].mUnreachedRegionBlack.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("黒成分Ａ", &mMapBlink[i].mUnreachedRegionBlack.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｒ", &mMapBlink[i].mUnreachedRegionWhite.r, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｇ", &mMapBlink[i].mUnreachedRegionWhite.g, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ｂ", &mMapBlink[i].mUnreachedRegionWhite.b, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genSlider("白成分Ａ", &mMapBlink[i].mUnreachedRegionWhite.a, 0, 0xff, 0, NULL, -1, -1, 512, 24);
        mctx->genLabel("\n", 0, 0, NULL, -1, -1, 512, 24);
    }
}

void dMeter_fmapHIO_c::listenPropertyEvent(const JORPropertyEvent* property) {
    JORReflexible::listenPropertyEvent(property);
    JORFile file;
    switch(reinterpret_cast<u32>(property->id)) {
    case 0x40000001:
        if (file.open(JORFile::EFlags_READ, "フィールドデータ(*.arc)\0*.arc\0すべてのファイル(*.*)\0*.*\0", "arc", NULL, NULL)) {
            if (!mpArcFile) {
                OSReport("arc heap free0 ===> %d\n",mDoExt_getArchiveHeap()->getTotalFreeSize());
                mpArcFile = mDoExt_getArchiveHeap()->alloc(0x46000, 0x20);
                JUT_ASSERT(7405, mpArcFile != NULL);
            }
            if (!mpDecompBuf) {
                OSReport("arc heap free1 ===> %d\n",mDoExt_getArchiveHeap()->getTotalFreeSize());
                mpDecompBuf = mDoExt_getArchiveHeap()->alloc(0x46000, 0x20);
                JUT_ASSERT(7411, mpDecompBuf != NULL);
            }
            memset(mpArcFile, 0, 0x46000);
            memset(mpDecompBuf, 0, 0x46000);
            file.readData(mpArcFile, 0);
            file.close();
            if (mpArcData) {
                JKRUnmountArchive(mpArcData);
                delete mpArcData;
                mpArcData = NULL;
            }
            JKRDecompress((u8*)mpArcFile, (u8*)mpDecompBuf, 0x46000, 0);
            mpArcData = new(mDoExt_getArchiveHeap(), 0) JKRMemArchive(mpDecompBuf, 0x46000, JKRMEMBREAK_FLAG_UNKNOWN0);
            JUT_ASSERT(7429, mpArcData != NULL);
            if (dComIfGp_isHeapLockFlag() == 2) {
                field_0x2f9 = true;
            }
            OSReport("open file!\n");
        } else {
            OSReport("open error!\n");
        }
        break;
    case 0x40000002:
        for (int i = 1; i < 8; i++) {
            OSReport("Region%d => %d\n", i, dComIfGs_isRegionBit(i));
        }
        break;
    }

}

void dMeter_fmapHIO_c::createEvtCallBackObject() {
    if (mpEvtCallBack) {
        return;
    }
    mpEvtCallBack = new subJOREventCallbackListNode();
    JUT_ASSERT(7460, mpEvtCallBack != NULL)
}

void dMeter_fmapHIO_c::deleteEvtCallBackObject() {
    if (mpEvtCallBack) {
        delete mpEvtCallBack;
        mpEvtCallBack = NULL;
    }
}

void dMeter_fmapHIO_c::update() {
    JORMContext* mctx = attachJORMContext(8);
    mctx->startUpdateNode(this);
    mctx->updateCheckBox(2, (u8*)&mAllRegionsUnlocked, 1, 0);
    mctx->updateCheckBox(2, (u8*)&mPortalWarpON, 1, 0);
    mctx->endUpdateNode();
    releaseJORMContext(mctx);
}
#endif

dMeter_cursorHIO_c::dMeter_cursorHIO_c() {
    mShopCursorScale = 1.05f;
    mShopCursorOffsetY = -30.0f;
#if PLATFORM_WII
    mMagicArmorCursorOffsetX = -10.0f;
#else
    mMagicArmorCursorOffsetX = 13.0f;
#endif
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
    mctx->genLabel("*****お店カーソル*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("オフセットＹ", &mShopCursorOffsetY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("オフセットＸ(マジックアーマー)", &mMagicArmorCursorOffsetX, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("オフセットＹ(マジックアーマー)", &mMagicArmorCursorOffsetY, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("スケール", &mShopCursorScale, 0.0f, 3.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****お店の商品（セーラの最初の店）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＸ", &mSeraShopObjZoom.x, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＹ", &mSeraShopObjZoom.y, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＺ", &mSeraShopObjZoom.z, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームアングルＸ", &mSeraShopObjZoomAngleX, -0x8000, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****お店の商品（オフセット値）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＸ", &mObjZoom.x, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＹ", &mObjZoom.y, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＺ", &mObjZoom.z, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームアングルＸ", &mShopObjZoomAngleX, -0x8000, 0x7fff, 0, NULL, -1, -1, 512, 24);
    mctx->genLabel("*****マジックアーマー（オフセット値）*****", 0, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＸ", &mMagicArmorObjZoom.x, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＹ", &mMagicArmorObjZoom.y, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームＺ", &mMagicArmorObjZoom.z, -300.0f, 300.0f, 0, NULL, -1, -1, 512, 24);
    mctx->genSlider("ズームアングルＸ", &mMagicArmorObjZoomAngleX, -0x8000, 0x7fff, 0, NULL, -1, -1, 512, 24);
}
#endif

dMeter_menuHIO_c g_menuHIO;

dMeter_drawHIO_c g_drawHIO;

dMeter_ringHIO_c g_ringHIO;

dMeter_fmapHIO_c g_fmapHIO;

dMeter_cursorHIO_c g_cursorHIO;
