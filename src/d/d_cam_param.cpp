/**
 * d_cam_param.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_cam_param.h"

#include "JSystem/JHostIO/JORFile.h"
#include "JSystem/JHostIO/JORServer.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"

enum CameraFlags {
    CAM_PARAM_GAPX, CAM_PARAM_GAPZ,
    CAM_PARAM_HBAS, CAM_PARAM_HMIN, CAM_PARAM_HMAX,
    CAM_PARAM_CDST, CAM_PARAM_CANG,
    CAM_PARAM_RBAS, CAM_PARAM_RMIN, CAM_PARAM_RMAX, CAM_PARAM_RNAR, CAM_PARAM_RFAR,
    CAM_PARAM_VBAS, CAM_PARAM_VMIN, CAM_PARAM_VMAX, CAM_PARAM_VNAR, CAM_PARAM_VFAR,
    CAM_PARAM_FBAS, CAM_PARAM_FMIN, CAM_PARAM_FMAX, CAM_PARAM_FNAR, CAM_PARAM_FFAR,
    CAM_PARAM_UTRN, CAM_PARAM_UNAR, CAM_PARAM_UFAR,
    CAM_PARAM_HNAR, CAM_PARAM_HFAR,
    CAM_PARAM_USLW,
};

#if DEBUG
namespace camera_names {
    const char* engine_name[] = {
        "LET", "CHASE", "LOCKON", "TALK",
        "SUBJECT", "FIXEDPOS", "FIXEDFRM", "TOWER",
        "RIDE", "MANUAL", "EVENT", "HOOKSHOT",
        "COLOSSEUM", "OBSERVE", "MAGNE", "RAIL",
        "PARARAIL", "ONESIDE", "TEST1", "TEST2",
    };
    const char* mode_name[] = {
        "NORMAL", "PARALLEL", "LOCKON", "TALK",
        "SUBJECT", "ATTACK", "ATTACK2", "SHOOT",
        "OBJSHOOT", "HOOKSHOT", "PUSHPULL",
    };
    const char* param_name[] = {
        "GAPX", "GAPZ", "HBAS", "HMIN", "HMAX", "CDST", "CANG", "RBAS",
        "RMIN", "RMAX", "RNAR", "RFAR", "VBAS", "VMIN", "VMAX", "VNAR",
        "VFAR", "FBAS", "FMIN", "FMAX", "FNAR", "FFAR", "UTRN", "UNAR",
        "UFAR", "HNAR", "HFAR", "USLW",
    };
    const char* flag_name[] = {
        "BGCHECK", "BGCHKLIGHT", "BGCHECKOFF", "BUSHCHECK",
        "COCHKOFF", "FWDCHECKH", "ROLLOFF", "HIGHOFF",
        "DIRECTSTK", "USEMAPTOOL", "RZOOM", "WIDEUSE1",
        "WIDEUSE2", "TARGETUSE", "FWDCHECK",
    };
}
#endif

f32 dCamMath::rationalBezierRatio(f32 param_0, f32 param_1) {
    f64 sp68;

    if (param_0 >= 0.0f) {
        sp68 = 1.0;
    } else {
        sp68 = -1.0;
        param_0 = -param_0;
    }

    f64 sp60 = 2.0 * param_0;
    f64 sp58 = 2.0 * param_1;
    f64 sp50 = sp60 * param_1;
    f64 var_f31 = sp50 - sp60 - sp58;
    f64 sp48 = -var_f31 - 1.0;
    f64 sp40 = param_0;
    f64 sp38 = (var_f31 * var_f31) - (4.0 * sp48 * sp40);

    f64 sp30 = -var_f31 - (sp38 > 0.0 ? sqrt(sp38) : 0.0);
    f64 sp28 = sp48 * 2.0;
    if (sp28 > 1e-07 || sp28 < -1e-07) {
        f64 var_f30 = sp30 / sp28;
        f64 sp20 = var_f30 * var_f30;
        f64 sp18 = 1.0 - var_f30;
        f64 sp10 = sp20 + ((sp18 * sp18) + (param_1 * (2.0 * sp18 * var_f30)));

        if (sp10 > 1.0000000116860974e-07) {
            return sp68 * (sp20 / sp10);
        }
        return 0.0f;
    } else {
        return 0.0f;
    }
}

static f32 dummy_literal1() {
    return 1.0f;
}

f32 dCamMath::zoomFovy(f32 param_0, f32 param_1) {
    cDegree deg(param_0);

    return cAngle::r2d(cM_atan2f(deg.Sin(), param_1 * deg.Cos()));
}

cXyz dCamMath::xyzRotateX(cXyz& i_xyz, cSAngle i_angle) {
    Mtx m;
    cXyz rot_xyz;

    cMtx_XrotS(m, i_angle.Val());
    MTXMultVec(m, &i_xyz, &rot_xyz);
    return rot_xyz;
}

cXyz dCamMath::xyzRotateY(cXyz& i_xyz, cSAngle i_angle) {
    Mtx m;
    cXyz rot_xyz;

    cMtx_YrotS(m, i_angle.Val());
    MTXMultVec(m, &i_xyz, &rot_xyz);
    return rot_xyz;
}

f32 dCamMath::xyzHorizontalDistance(cXyz& i_posA, cXyz& i_posB) {
    f64 x = i_posA.x - i_posB.x;
    f64 z = i_posA.z - i_posB.z;

    return sqrt((x * x) + (z * z));
}

dCstick_c::dCstick_c() {
    mThresholdLow = 0.2f;
    mThresholdHigh = 0.95f;
    mInputSpeed = 6;
#if DEBUG
    mDisplayPosX = 170;
    mDisplayPosY = 40;
#endif
}

#if DEBUG
void dCstick_c::genMessage(JORMContext* ctx) {
    // "- Switch"
    ctx->genLabel("- スイッチ", 0);
    // " Threshold: Low"
    ctx->genSlider(" しきい値 低", &mThresholdLow, 0.0f, 1.0f);
    // "          High"
    ctx->genSlider("          高", &mThresholdHigh, 0.0f, 1.0f);
    // " Input speed"
    ctx->genSlider(" 入力スピード", &mInputSpeed, 0x0, 0x50);

    ctx->genLabel("-", 0);
    // " Debug display"
    ctx->genCheckBox(" デバッグ表示", &mIsDebugDisplay, 0x8000);
    // "   Display position X"
    ctx->genSlider("   表示位置 Ｘ", &mDisplayPosX, 0x0, 0x280);
    // "            Y"
    ctx->genSlider("            Ｙ", &mDisplayPosY, 0x0, 0x1e0);

    ctx->genLabel("-", 0);
}
#endif

bool dCstick_c::Shift(u32 param_0) {
    UNUSED(param_0);
#if DEBUG
    CheckFlag(0x8000);
#endif

    return false;
}

dCamBGChk_c::dCamBGChk_c() {
    mFloorMargin = 32.0f;

    mChkInfo[0].mDistance = 1.0f;
    mChkInfo[0].mChkAngle = 25.0f;
    mChkInfo[0].mWeightH = 0.4f;
    mChkInfo[0].mWeightL = 0.6f;

    mChkInfo[1].mDistance = 3.0f;
    mChkInfo[1].mChkAngle = 15.0f;
    mChkInfo[1].mWeightH = 0.5f;
    mChkInfo[1].mWeightL = 0.3f;

    mFwdBackMargin = 10.0f;
    mFwdCushion = 0.1f;
    field_0x2c = 0.2f;
    mGazeBackMargin = 10.0f;
    mCornerCushion = 0.75f;
    mWallCushion = 0.5f;
    mWallUpDistance = 80.0f;
    mWallBackCushion = 0.08f;
    mCornerAngleMax = 120.0f;
}

#if DEBUG
void dCamBGChk_c::genMessage(JORMContext* ctx) {
    ctx->genLabel("-", 0);
    // "- Floor check"
    ctx->genLabel("- 床チェック", 0);
    // " Margin"
    ctx->genSlider(" マージン", &mFloorMargin, 0.0f, 1000.0f);
    ctx->genLabel("-",  0);
    // "- Forward check"
    ctx->genLabel("- 前方チェック", 0);
    // " [0]Angle"
    ctx->genSlider(" [0]角度", &mChkInfo[0].mChkAngle, 1.0f, 90.0f);
    // " [0]Length (height X)"
    ctx->genSlider(" [0]長さ(身長Ｘ)", &mChkInfo[0].mDistance, 1.0f, 25.0f);
    // " [0]Weight+"
    ctx->genSlider(" [0]ウエイト＋", &mChkInfo[0].mWeightH, 0.0f, 1.0f);
    // " [0]Weight-"
    ctx->genSlider(" [0]ウエイト－", &mChkInfo[0].mWeightL, 0.0f, 1.0f);
    // " [1]Angle"
    ctx->genSlider(" [1]角度", &mChkInfo[1].mChkAngle, 1.0f, 90.0f);
    // " [1]Length (height X)"
    ctx->genSlider(" [1]長さ(身長Ｘ)", &mChkInfo[1].mDistance, 1.0f, 25.0f);
    // " [1]Weight+"
    ctx->genSlider(" [1]ウエイト＋", &mChkInfo[1].mWeightH, 0.0f, 1.0f);
    // " [1]Weight-"
    ctx->genSlider(" [1]ウエイト－", &mChkInfo[1].mWeightL, 0.0f, 1.0f);
    // " Margin"
    ctx->genSlider(" マージン", &mFwdBackMargin, 0.0f, 1000.0f);
    // " Cusion"
    ctx->genSlider(" クッション", &mFwdCushion, 0.0f, 1.0f);
    ctx->genLabel("-", 0);
    // "- Per camera"
    ctx->genLabel("- カメラあたり", 0);
    // " Margin"
    ctx->genSlider(" マージン", &mGazeBackMargin, 0.0f, 1000.0f);
    // BUG: second parameter should be &mCornerAngleMax
    // " Corner wall detection angle"
    ctx->genSlider(" 角壁判別角度", &mCornerCushion, 0.0f, 180.0f);
    // " Corner cushion"
    ctx->genSlider(" 角クッション", &mCornerCushion, 0.0f, 1.0f);
    // " Wall cushion"
    ctx->genSlider(" 壁クッション", &mWallCushion, 0.0f, 1.0f);
    // " Wall up distance"
    ctx->genSlider(" 壁上がり距離", &mWallUpDistance, 0.0f, 1000.0f);
    // " Wall back cushion"
    ctx->genSlider(" 壁戻クッション", &mWallBackCushion, 0.0f, 1.0f);
    ctx->genLabel("-", 0);
}
#endif

dCamParam_c::dCamParam_c(s32 i_styleID) {
    const char* fileName = dComIfGp_getCameraParamFileName(0);
    void* objRes = dComIfG_getObjectRes(fileName, "camstyle.dat");
    u8* cam_data = (u8*)objRes;

    mCamStyleData = (dCamStyleData::StyleData*)(cam_data + 8);
    mStyleNum = ((dCamStyleData*)cam_data)->mStyleNum;

#if DEBUG
    mpHIOChild = mDoHIO_createChild("カメラパラメタ", this);
#endif

    Change(i_styleID);

    mMapToolFovy = 0xFF;
    mMapToolArg0 = 0xFF;
    mMapToolArg1 = 0xFF;
    mMapToolArg2 = -1;
}

dCamParam_c::~dCamParam_c() {
#if DEBUG
    mDoHIO_deleteChild(mpHIOChild);
#endif
}

int dCamParam_c::Change(s32 i_styleID) {
    if (i_styleID >= 0 && i_styleID < mStyleNum) {
        mStyleID = i_styleID;
        mCurrentStyle = &mCamStyleData[mStyleID];

#if DEBUG
        mDoHIO_updateChild(mpHIOChild);
#endif

        return 1;
    }

    mStyleID = 0;
    mCurrentStyle = mCamStyleData;
    return 0;
}

int dCamParam_c::SearchStyle(u32 param_0) {
    int styleID = -1;
    int i = 0;
    while (i < mStyleNum) {
        if (param_0 == mCamStyleData[i].field_0x0) {
            styleID = i;
            break;
        }
        i++;
    }

    return styleID;
}

#if DEBUG
void dCamParam_c::genMessage(JORMContext* ctx) {
    ctx->genSlider(" STYLE ID", &mStyleID, 0, mStyleNum, 0x40000000);

    ctx->genLabel("-", 0);
    // "Origin C"
    ctx->genLabel("- 原点Ｃ", 0);
    // "Front/back (GAPZ)"
    ctx->genSlider(" 前後(GAPZ)", &mCurrentStyle->mParams[CAM_PARAM_GAPZ], -10000.0f, 10000.0f);
    // "Left/right (GAPX)"
    ctx->genSlider(" 左右(GAPX)", &mCurrentStyle->mParams[CAM_PARAM_GAPX], -10000.0f, 10000.0f);
    // "Distance (CDST)"
    ctx->genSlider(" 距離(CDST)", &mCurrentStyle->mParams[CAM_PARAM_CDST], -1.0f, 1.0f);
    // "Angle (CANG)"
    ctx->genSlider(" 角度(CANG)", &mCurrentStyle->mParams[CAM_PARAM_CANG], -1.0f, 1.0f);

    ctx->genLabel("-", 0);
    // "- Origin point high"
    ctx->genLabel("- 原点高", 0);
    // "Standard (HBAS)"
    ctx->genSlider(" 標準(HBAS)", &mCurrentStyle->mParams[CAM_PARAM_HBAS], -10000.0f, 10000.0f);
    // "Minimum (HMIN)"
    ctx->genSlider(" 最小(HMIN)", &mCurrentStyle->mParams[CAM_PARAM_HMIN], -10000.0f, 10000.0f);
    // "Maximum (HMAX)"
    ctx->genSlider(" 最大(HMAX)", &mCurrentStyle->mParams[CAM_PARAM_HMAX], -10000.0f, 10000.0f);
    // "Nearest (HNAR)"
    ctx->genSlider(" 最近(HNAR)", &mCurrentStyle->mParams[CAM_PARAM_HNAR], -10000.0f, 10000.0f);
    // "Farthest (HFAR)"
    ctx->genSlider(" 最遠(HFAR)", &mCurrentStyle->mParams[CAM_PARAM_HFAR], -10000.0f, 10000.0f);

    ctx->genLabel("-", 0);
    // "- Distance"
    ctx->genLabel("- 距離", 0);
    ctx->genSlider(" 標準(RBAS)", &mCurrentStyle->mParams[CAM_PARAM_RBAS], -10000.0f, 10000.0f);
    ctx->genSlider(" 最小(RMIN)", &mCurrentStyle->mParams[CAM_PARAM_RMIN], -10000.0f, 10000.0f);
    ctx->genSlider(" 最大(RMAX)", &mCurrentStyle->mParams[CAM_PARAM_RMAX], -10000.0f, 10000.0f);
    ctx->genSlider(" 最近(RNAR)", &mCurrentStyle->mParams[CAM_PARAM_RNAR], -10000.0f, 10000.0f);
    ctx->genSlider(" 最遠(RFAR)", &mCurrentStyle->mParams[CAM_PARAM_RFAR], -10000.0f, 10000.0f);

    ctx->genLabel("-", 0);
    ctx->genLabel("- 縦角", 0);
    // "Standard (VBAS)"
    ctx->genSlider(" 標準(VBAS)", &mCurrentStyle->mParams[CAM_PARAM_VBAS], -180.0f, 180.0f);
    // "Minimuum (VMIN)"
    ctx->genSlider(" 最小(VMIN)", &mCurrentStyle->mParams[CAM_PARAM_VMIN], -180.0f, 180.0f);
    // "Maximum (VMAX)"
    ctx->genSlider(" 最大(VMAX)", &mCurrentStyle->mParams[CAM_PARAM_VMAX], -180.0f, 180.0f);
    // "Nearest (VNAR)"
    ctx->genSlider(" 最近(VNAR)", &mCurrentStyle->mParams[CAM_PARAM_VNAR], -180.0f, 180.0f);
    // "Farthest (VFAR)"
    ctx->genSlider(" 最遠(VFAR)", &mCurrentStyle->mParams[CAM_PARAM_VFAR], -180.0f, 180.0f);

    ctx->genLabel("-", 0);
    // "- Horizontal angle"
    ctx->genLabel("- 横角", 0);
    // "Rotation speed (UTRN)"
    ctx->genSlider(" 回速(UTRN)", &mCurrentStyle->mParams[CAM_PARAM_UTRN], -1.0f, 1.0f);
    // "Lap behind (USLW)"
    ctx->genSlider(" 回遅(USLW)", &mCurrentStyle->mParams[CAM_PARAM_USLW], -1.0f, 1.0f);
    // "Nearest (UNAR)"
    ctx->genSlider(" 最近(UNAR)", &mCurrentStyle->mParams[CAM_PARAM_UNAR], -180.0f, 180.0f);
    // "Farthest (UFAR)"
    ctx->genSlider(" 最遠(UFAR)", &mCurrentStyle->mParams[CAM_PARAM_UFAR], -180.0f, 180.0f);

    ctx->genLabel("-", 0);
    // "- Field of view"
    ctx->genLabel("- 画角", 0);
    // "Standard (FBAS)"
    ctx->genSlider(" 標準(FBAS)", &mCurrentStyle->mParams[CAM_PARAM_FBAS], -30.0f, 90.0f);
    // "Minimum (FMIN)"
    ctx->genSlider(" 最小(FMIN)", &mCurrentStyle->mParams[CAM_PARAM_FMIN], -30.0f, 90.0f);
    // "Maximum (FMAX)"
    ctx->genSlider(" 最大(FMAX)", &mCurrentStyle->mParams[CAM_PARAM_FMAX], -30.0f, 90.0f);
    // "Nearest (FNAR)"
    ctx->genSlider(" 最近(FNAR)", &mCurrentStyle->mParams[CAM_PARAM_FNAR], -30.0f, 90.0f);
    // "Farthest (FFAR)"
    ctx->genSlider(" 最遠(FFAR)", &mCurrentStyle->mParams[CAM_PARAM_FFAR], -30.0f, 90.0f);
    ctx->genLabel("-", 0);

    // "- Flag"
    ctx->genLabel("- フラグ", 0);
    // "BG check (BGCHECK)"
    ctx->genCheckBox(" ＢＧチェック(BGCHECK)", &mCurrentStyle->mFlags, 0x1);
    // "BG check light (BGCHKLIGHT)"
    ctx->genCheckBox(" ＢＧチェック軽(BGCHKLIGHT)", &mCurrentStyle->mFlags, 0x2);
    // "Grass check (BUSHCHECK)"
    ctx->genCheckBox(" 草チェック(BUSHCHECK)", &mCurrentStyle->mFlags, 0x8);
    // "Front check (FWDCHECK)"
    ctx->genCheckBox(" 前方チェック(FWDCHECK)", &mCurrentStyle->mFlags, 0x4000);
    // "CO check prohibited (COCHKOFF)"
    ctx->genCheckBox(" ＣＯチェック禁止(COCHKOFF)", &mCurrentStyle->mFlags, 0x10);
    // "R button zoom (RZOOM)"
    ctx->genCheckBox(" Ｒボタンズーム(RZOOM)", &mCurrentStyle->mFlags, 0x400);
    // "Stick direct value (DIRECTSTK)"
    ctx->genCheckBox(" スティック直値(DIRECTSTK)", &mCurrentStyle->mFlags, 0x100);
    // "Use map tool (USEMAPTOOL)"
    ctx->genCheckBox(" マップツール使用(USEMAPTOOL)", &mCurrentStyle->mFlags, 0x200);
    // "No BG check (BGCHECKOFF)"
    ctx->genCheckBox(" ＢＧチェック無(BGCHECKOFF)", &mCurrentStyle->mFlags, 0x4);
    // "Forward check distance (FWDCHECKH)"
    ctx->genCheckBox(" 前方チェック遠(FWDCHECKH)", &mCurrentStyle->mFlags, 0x20);
    // "C-axis rotation prohibited (ROLLOFF)"
    ctx->genCheckBox(" Ｃ回転禁止(ROLLOFF)", &mCurrentStyle->mFlags, 0x40);
    // "C pulling prohibited (HIGHOFF)"
    ctx->genCheckBox(" Ｃ引き禁止(HIGHOFF)", &mCurrentStyle->mFlags, 0x80);
    // "General purpose 1 (WIDEUSE1)"
    ctx->genCheckBox(" 汎用１(WIDEUSE1)", &mCurrentStyle->mFlags, 0x800);
    // "General purpose 2 (WIDEUSE2)"
    ctx->genCheckBox(" 汎用２(WIDEUSE2)", &mCurrentStyle->mFlags, 0x1000);
    // "Target specification (TARGETUSE)"
    ctx->genCheckBox(" 対象指定(TARGETUSE)", &mCurrentStyle->mFlags, 0x2000);

    ctx->genLabel("-", 0);
    // "- Map tools"
    ctx->genLabel("- マップツール", 0);
    // "Field of view"
    ctx->genSlider(" 画角　", &mMapToolFovy, 0, 0xff);
    // "Argument 0"
    ctx->genSlider(" 引数０", &mMapToolArg0, 0, 0xff);
    // "Argument 1"
    ctx->genSlider(" 引数１", &mMapToolArg1, 0, 0xff);
    // "Argument 2"
    ctx->genSlider(" 引数２", &mMapToolArg2, 0, 0xffff);

    ctx->genLabel("-", 0);
    // "- Output file"
    ctx->genLabel("- ファイル出力", 0);
    // "Save"
    ctx->genButton(" 保存 ", 0x12d);

    ctx->genLabel("-", 0);
}

void dCamParam_c::listenPropertyEvent(const JORPropertyEvent* event) {
    JORReflexible::listenPropertyEvent(event);
    JORMContext* ctx = attachJORMContext(8);
    switch ((int)event->id) {
    case 0x12d:
        writeParamXML();
        break;
    }
}

int dCamParam_c::writeParamXML() {
    int i;
    char styleName[5];
    styleName[0] = (mCurrentStyle->field_0x0 >> 24) & 0xff;
    styleName[1] = (mCurrentStyle->field_0x0 >> 16) & 0xff;
    styleName[2] = (mCurrentStyle->field_0x0 >> 8) & 0xff;
    styleName[3] = (mCurrentStyle->field_0x0 >> 0) & 0xff;
    styleName[4] = '\0';

    OpenFile();
    PrintFile("\n");
    PrintFile("  <style name=\"%s\">\n", styleName);
    PrintFile("    <engine type=\"%s\"/>\n", camera_names::engine_name[mCurrentStyle->field_0x4]);
    for (i = 0; i < ARRAY_SIZE(camera_names::param_name); i++) {
        PrintFile("    <param type=\"%s\">%f</param>\n", camera_names::param_name[i], mCurrentStyle->mParams[i]);
    }
    for (i = 0; i < 16; i++) {
        if ((mCurrentStyle->mFlags & 1 << i) != 0) {
            PrintFile("    <flag type=\"%s\"/>\n", camera_names::flag_name[i]);
        }
    }
    PrintFile("  </style>\n");
    PrintFile("\n");
    CloseFile();
    return 1;
}

void dCamParam_c::OpenFile() {
    char fileSuffix[9];
    fileSuffix[0] = (mCurrentStyle->field_0x0 >> 24) & 0xff;
    fileSuffix[1] = (mCurrentStyle->field_0x0 >> 16) & 0xff;
    fileSuffix[2] = (mCurrentStyle->field_0x0 >> 8) & 0xff;
    fileSuffix[3] = (mCurrentStyle->field_0x0 >> 0) & 0xff;
    fileSuffix[4] = '.';
    fileSuffix[5] = 'x';
    fileSuffix[6] = 'c';
    fileSuffix[7] = 's';
    fileSuffix[8] = '\0';
    const char ext[] = "パラメタベースファイル(*.xcs)\0*.xcs\0その他のファイル(*.*)\0*.*\0";
    mFile.open(JORFile::EFlags_WRITE | JORFile::EFlags_UNK_0x4, ext, "xcs", NULL, fileSuffix);
}

void dCamParam_c::PrintFile(char* i_fmt, ...) {
    UNUSED(i_fmt);
    char buf[0x100];
    va_list args;
    va_start(args, i_fmt);
    vsnprintf(buf, sizeof(buf), i_fmt, args);
    va_end(args);

    mFile.writeData(buf, strlen(buf));
}

void dCamParam_c::CloseFile() {
    mFile.close();
}
#endif

static f32 dummy_literal2() {
    return 0.25f;
}

dCamSetup_c::dCamSetup_c() {
    mDrawNear = 1.0f;
    mDrawFar = 100000.0f;
    field_0xc = 1;
    mModeSwitchType = -1;
    mForceType = -1;
    mCusCus = 0.2f;
    field_0x24 = 0.05f;
    field_0x60 = 80.0f;
    field_0x5c = -60.0f;
    field_0x28 = 0.33f;
    field_0x2c = 0.02f;
    field_0x50 = 100.0f;
    field_0x30 = 0.75f;
    mBaseCushion = 0.28f;
    mJumpCushion = 1.0f;
    field_0x58 = 0.15f;
    field_0x44 = 0.005f;
    field_0x54 = 0.06f;
    mCurveWeight = 1.0f;
    field_0x34 = 25.0f;
    mSubjLinkCullDist = 70.0f;
    mParallelDist = 60.0f;
    mTrimVistaHeight = 35.0f;
    mTrimCineScopeHeight = 65.0f;
    field_0xb0 = 150;
    field_0xb4 = 60.0f;
    mManualStartCThreshold = 0.3f;
    mManualEndVal = 0.2f;
    mFalseValue = 0.1f;
    mFalseAngle = 30.0f;
    mDebugFlags = 1;

    mFlags2 = 0x118;
    mFlags2 |= (u16)0x200;
    mFlags2 |= (u16)0x400;

    mChargeLatitude = 27.0f;
    mChargeTimer = 90;
    mChargeBRatio = 0.5f;
    mLockonChangeTimer = 40;
    mLockonChangeCushion = 0.25f;
    field_0x6c = 45.0f;
    mForceLockOffDist = 3500.0f;
    mForceLockOffTimer = 90;
    mThrowTimer = 30;
    mThrowVAngle = 25.0f;
    mThrowCushion = 0.05f;
    mThrowCtrOffset = 10.0f;
    field_0x78 = 0.5f;
    field_0x7c = 0.4f;
    field_0x80 = 10.0f;
    field_0x84 = 4.0f;
    field_0xd4 = 60.0f;
    field_0xd8 = 100.0f;
    mWindShakeCtr = 15.0f;
    mWindShakeFvy = 0.3f;
    mMapToolCamShortTimer = 60;
    mMapToolCamLongTimer = 120;
#if DEBUG
    mDebugFlags |= (u16)0x3000;
    mpHIOChild = mDoHIO_createChild("カメラ", this);
#endif
#if PLATFORM_WII
    mDebugFlags |= (u16)0x7800;
#else
    mDebugFlags |= (u16)0x4800;
#endif
    field_0x15c = 0.0f;
    field_0x160 = 0.0f;
    mDebugFlags |= (u16)0x800;
#if PLATFORM_GCN
    mWaitRollTimer = 120;
#else
    mWaitRollTimer = 20;
#endif
#if PLATFORM_GCN
    mWaitRollSpeed = 0.02f;
#else
    mWaitRollSpeed = 0.05f;
#endif
    field_0x16c = 1200.0f;
    field_0xe0 = 0.06f;
    field_0xec = 25;
    field_0xf4 = 0.85f;
    field_0xf8 = 1.0f;
    field_0xf0 = 12;
    field_0xe4 = 50.0f;
    field_0xe8 = 0.0f;
    mScreensaverFirstWaitTimer = 30;
    mScreensaverWaitTimer = 600;
    mScreensaverExecTimer = 120;

    dCam_getBody()->EventRecoverNotime();
}

dCamSetup_c::~dCamSetup_c() {
#if DEBUG
    mDoHIO_deleteChild(mpHIOChild);
#endif
}

bool dCamSetup_c::CheckLatitudeRange(s16* param_0) {
    s16 temp0 = cAngle::d2s(field_0x60);
    s16 temp1 = cAngle::d2s(field_0x5c);

    if (*param_0 > temp0) {
        *param_0 = temp0;
        return false;
    }

    if (*param_0 < temp1) {
        *param_0 = temp1;
        return false;
    }

    return true;
}

f32 dCamSetup_c::PlayerHideDist() {
    return daPy_py_c::checkNowWolf() ? 120.0f : mSubjLinkCullDist;
}

#if DEBUG
void dCamSetup_c::genMessage(JORMContext* ctx) {
    int i;

    ctx->genLabel("-", 0);
    // "- Debug"
    ctx->genLabel("- デバッグ", 0);
    // "Camera information display"
    ctx->genCheckBox(" カメラ情報表示", &mDebugFlags, 0x8000);
    // "Event information display"
    ctx->genCheckBox(" イベント情報表示", &mDebugFlags, 0x40);
    // "Player 2D position"
    ctx->genCheckBox(" プレイヤ２Ｄ位置", &mDebugFlags, 0x80);
    // "Imaginary line"
    ctx->genCheckBox(" イマジナリライン", &mDebugFlags, 0x100);
    // "Location information"
    ctx->genCheckBox(" 位置情報", &mDebugFlags, 0x200);

    ctx->genLabel("-", 0);
    // "Camera type switching"
    ctx->startComboBox("カメラタイプ切替", &mForceType);
    // "Unused"
    ctx->genComboBoxItem("未使用", -1);
    for (i = 0; i < mTypeNum; i++) {
        ctx->genComboBoxItem((char*)((int)mTypeTable + i * 0x44), i);
    }
    ctx->endComboBox();

    ctx->genLabel("-", 0);
    ctx->genLabel("-", 0);
    // "- Wolf special attack"
    ctx->genLabel("- 狼特殊攻撃", 0);
    // "Return timer"
    ctx->genSlider(" もどりタイマー", &mChargeTimer, 1, 3000);

    ctx->genLabel("-", 0);
    // "- Screensaver"
    ctx->genLabel("- スクリーンセーバー", 0);
    // "Enabled"
    ctx->genCheckBox(" 有効", &mFlags2, 0x800);
    // "Initial wait time (s)"
    ctx->genSlider(" 待ち時間最初(s)", &mScreensaverFirstWaitTimer, 1, 0xe10);
    // "Wait time"
    ctx->genSlider(" 待ち時間(s)", &mScreensaverWaitTimer, 1, 0xe10);
    // "Execution time"
    ctx->genSlider(" 実行時間(s)", &mScreensaverExecTimer, 1, 0xe10);

    ctx->genLabel("-", 0);
    // "- Event"
    ctx->genLabel("- イベント", 0);
    // "Conversation cut switching"
    ctx->startComboBox("会話カット切替", &mModeSwitchType);
    // "Unused"
    ctx->genComboBoxItem("未使用", -1);
    // "00. Default"
    ctx->genComboBoxItem("00.デフォルト", 0);
    // "50. Default (no connection)"
    ctx->genComboBoxItem("50.デフォルト繋ぎ無", 50);
    // "11. Speaker to listener"
    ctx->genComboBoxItem("11.話し手から聞き手", 11);
    // "12. Listener to speaker"
    ctx->genComboBoxItem("12.聞き手から話し手", 12);
    // "13. Long shot"
    ctx->genComboBoxItem("13.ロングショット", 13);
    // "14. Listener's point of view"
    ctx->genComboBoxItem("14.聞き手の主観", 14);
    // "15. Speaker's point of view"
    ctx->genComboBoxItem("15.話し手の主観", 15);
    // "16. Close-up of listener"
    ctx->genComboBoxItem("16.聞き手のアップ", 16);
    // "17. Close-up of speaker"
    ctx->genComboBoxItem("17.話し手のアップ", 17);
    // "61. Close of of speaker B"
    ctx->genComboBoxItem("61.話し手のアップＢ", 61);
    // "18. From the side of the listener"
    ctx->genComboBoxItem("18.聞き手の横から", 18);
    // "19. From the side of the speaker"
    ctx->genComboBoxItem("19.話し手の横から", 19);
    // "20. Listener from the front"
    ctx->genComboBoxItem("20.聞き手正面", 20);
    // "21. Speaker from the front"
    ctx->genComboBoxItem("21.話し手正面", 21);
    // "62. Speaker from the front B"
    ctx->genComboBoxItem("62.話し手正面Ｂ", 62);
    // "22. Listener bust shot"
    ctx->genComboBoxItem("22.聞き手胸像", 22);
    // "23. Speaker bust shot"
    ctx->genComboBoxItem("23.話し手胸像", 23);
    // "63. Speaker bust shot B"
    ctx->genComboBoxItem("63.話し手胸像Ｂ", 63);
    // "24. Listener from the front diagonal"
    ctx->genComboBoxItem("24.聞き手斜前", 24);
    // "25. Speaker from the front diagonal"
    ctx->genComboBoxItem("25.話し手斜前", 25);
    // "26. Listener from the lower diagonal"
    ctx->genComboBoxItem("26.聞き手斜下", 26);
    // "27 Speaker from the lower diagonal"
    ctx->genComboBoxItem("27.話し手斜下", 27);
    // "28. From the side"
    ctx->genComboBoxItem("28.横から", 28);
    // "29. Large face from the front"
    ctx->genComboBoxItem("29.でか顔正面", 29);
    // "30. Speaker to listener (wide)"
    ctx->genComboBoxItem("30.話し手から聞き手広", 30);
    // "31. Listener to speaker (wide)"
    ctx->genComboBoxItem("31.聞き手から話し手広", 31);
    // "32. Long shot from the side"
    ctx->genComboBoxItem("32.横からロング", 32);
    // "33. Midna"
    ctx->genComboBoxItem("33.ミドナ", 33);
    // "34. Midna back"
    ctx->genComboBoxItem("34.ミドナ後", 34);
    // "35. Midna diagonal"
    ctx->genComboBoxItem("35.ミドナ斜", 35);
    // "36. Midna diagonal back"
    ctx->genComboBoxItem("36.ミドナ斜後", 36);
    // "37. Midna attention, long shot"
    ctx->genComboBoxItem("37.ミドナ注目ロング", 37);
    // "38. Over the shoulder shot"
    ctx->genComboBoxItem("38.肩なめ", 38);
    // "39. Listener from the front (upper body)"
    ctx->genComboBoxItem("39.聞き手正面上", 39);
    // "40. Speaker from the front (upper body)"
    ctx->genComboBoxItem("40.話し手正面上", 40);
    // "64: Speaker from the front (upper body) B"
    ctx->genComboBoxItem("64.話し手正面上Ｂ", 64);
    // "41. Listener from the waist up"
    ctx->genComboBoxItem("41.聞き手腰上", 41);
    // "42. Speaker from the waist up"
    ctx->genComboBoxItem("42.話し手腰上", 42);
    // "65. Speaker from the waist up B"
    ctx->genComboBoxItem("65.話し手腰上Ｂ", 65);
    ctx->endComboBox();

    ctx->genLabel("-", 0);
    // "Short connection"
    ctx->genSlider(" 短つなぎ", &mMapToolCamShortTimer, 0, 3000);
    // "Long connection"
    ctx->genSlider(" 長つなぎ", &mMapToolCamLongTimer, 0, 3000);

    ctx->genLabel("-", 0);
    // "- Attention"
    ctx->genLabel("- 注目", 0);
    // "Switching timer"
    ctx->genSlider(" 切替タイマー", &mLockonChangeTimer, 0, 400);
    // "Switching cushion"
    ctx->genSlider(" 切替クッション", &mLockonChangeCushion, 0.0f, 1.0f);

    ctx->genLabel("-", 0);
    // "- Subject"
    ctx->genLabel("- 主観", 0);
    // "Distance to cull the link"
    ctx->genSlider(" リンク消す距離", &mSubjLinkCullDist, 0.0f, 400.0f);

    ctx->genLabel("-", 0);
    // "- Manual"
    ctx->genLabel("- マニュアル", 0);
    // "Start C threshold"
    ctx->genSlider(" 開始Ｃしきい", &mManualStartCThreshold, 0.0f, 400.0f);

    ctx->genLabel("-", 0);
    // "Trimming"
    ctx->genLabel("- トリミング", 0);
    // "Vista"
    ctx->genSlider(" ビスタ", &mTrimVistaHeight, 0.0f, 200.0f);
    // "Cinemascope"
    ctx->genSlider(" シネスコ", &mTrimCineScopeHeight, 0.0f, 200.0f);

    ctx->genLabel("-", 0);
    // "- Fake angle system"
    ctx->genLabel("- 嘘アングルシステム", 0);
    // "Value"
    ctx->genSlider(" 値", &mFalseValue, 0.0f, 1.0f);
    // "Angle"
    ctx->genSlider(" 角度", &mFalseAngle, 0.0f, 180.0f);

    ctx->genLabel("-", 0);
    // "- Forced attention"
    ctx->genLabel("- 強制注目", 0);
    // "Limit distance"
    ctx->genSlider(" 限界距離", &mForceLockOffDist, 0.0, 100000.0);
    // "Timer"
    ctx->genSlider(" タイマー", &mForceLockOffTimer, 0, 100000);

    ctx->genLabel("-", 0);
    // "Object avoidance"
    ctx->genLabel("- オブジェクト避け", 0);
    // "Valid"
    ctx->genCheckBox(" 有効", &mFlags2, 0x400);

    ctx->genLabel("-", 0);
    // "- Windstorm shaking experiment"
    ctx->genLabel("- 暴風揺れ実験", 0);
    // "Valid"
    ctx->genCheckBox(" 有効", &mFlags2, 0x100);
    // "Focal point"
    ctx->genSlider(" 注視点", &mWindShakeCtr, 0.0, 100.0);
    // "Angle of view"
    ctx->genSlider(" 画角", &mWindShakeFvy, 0.0, 10.0);

    ctx->genLabel("-", 0);
    // "- After throwing"
    ctx->genLabel("- 投げ後", 0);
    // "Angle"
    ctx->genSlider(" 角度", &mThrowVAngle, -180.0f, 180.0f);
    // "Height (relative)"
    ctx->genSlider(" 高さ(相対)", &mThrowCtrOffset, -180.0f, 180.0f);
    // "Timer"
    ctx->genSlider(" タイマー", &mThrowTimer, 1, 3000);
    // "Cushion"
    ctx->genSlider(" クッション", &mThrowCushion, 0.0f, 1.0f);

    ctx->genLabel("-", 0);
    // "- Drawing settings"
    ctx->genLabel("- 描画設定", 0);
    // "Valid"
    ctx->genCheckBox(" 有効", &mDebugFlags, 0x400);
    ctx->genSlider(" near", &mDrawNear, 0.0f, 1000000.0f);
    ctx->genSlider(" far", &mDrawFar, 0.0f, 1000000.0f);

    ctx->genLabel("-", 0);
}
#endif
