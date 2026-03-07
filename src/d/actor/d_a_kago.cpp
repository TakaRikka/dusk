/**
 * @file d_a_kago.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kago.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_msg_object.h"

#include <cmath>
#include <cstring>

#include "d/actor/d_a_balloon_2D.h"
#include "d/actor/d_a_e_ym.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_obj_balloon.h"
#include "d/actor/d_a_obj_riverrock.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_waterfall.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_overlap_mng.h"

#include "res/Object/E_yc.h"
#include "res/Object/E_kc.h"

#define TYPE_TWILIGHT 0
#define TYPE_NORMAL   1

#define SCENE_TYPE_LAKE_HYLIA  0
#define SCENE_TYPE_RIVER       1
#define SCENE_TYPE_BOARD_HOUSE 2
#define SCENE_TYPE_DEFAULT     3
namespace {
static dCcD_SrcSph cc_sph_src = {
    {
        {0x0, {{0x8, 0x0, 0x12}, {0x0, 0x0}, 0x0}}, // mObj
        {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
            {{0.0f, 0.0f, 0.0f}, 80.0f} // mSph
    } // mSphAttr
};
};  // namespace

class daKago_HIO_c : public JORReflexible {
public:
    daKago_HIO_c();
    virtual ~daKago_HIO_c() {}

    void genMessage(JORMContext* ctx);

    /* 0x04 */ s8 mChild;
    /* 0x08 */ f32 mRevoconUpDown;
    /* 0x0C */ f32 mRevoconUpDownMax;
    /* 0x10 */ f32 mRevoconLeftRight;
    /* 0x14 */ f32 mRevoconLeftRightMax;
    /* 0x18 */ f32 mBasicSize;
    /* 0x1C */ f32 mFlightSpeed;
    /* 0x20 */ f32 mFlightGroundAltitude;
    /* 0x24 */ f32 mFlightCeilingAltitude;
    /* 0x28 */ f32 mShadowDensity;
    /* 0x2C */ f32 mDescentRateIncrement;
    /* 0x30 */ f32 mAscentRateDecel;
    /* 0x34 */ f32 mDashTime;
    /* 0x38 */ f32 mDashTimeMultiplier;
    /* 0x3C */ f32 mWallHitInvulnTime;
    /* 0x40 */ f32 mDashCooldownTime;
    /* 0x44 */ f32 mZOffset;
    /* 0x48 */ f32 mZOffsetHori;
    /* 0x4C */ u8 mAngleTrackingMode;
    /* 0x50 */ f32 mYOffsetFromWaterSurface;
    /* 0x54 */ f32 mWaterSplashTime;
    /* 0x58 */ f32 mSplashGenTimeDuringDash;
};

daKago_HIO_c::daKago_HIO_c() {
    mChild = -1;
    mRevoconUpDown = 1024.0f;
    mRevoconUpDownMax = 512.0f;
    mRevoconLeftRight = 256.0f;
    mRevoconLeftRightMax = 256.0f;
    mBasicSize = 1.0f;
    mFlightSpeed = 50.0f;
    mDashTime = 70.0f;
    mDashTimeMultiplier = 1.0f;
    mWallHitInvulnTime = 90.0f;
    mDescentRateIncrement = 5.0f;
    mAscentRateDecel = 5.0f;
    mFlightGroundAltitude = 300.0f;
    mFlightCeilingAltitude = 300.0f;
    mShadowDensity = 3.0f;
    mDashCooldownTime = 85.0f;
    mZOffset = 700.0f;
    mZOffsetHori = 200.0f;
    mAngleTrackingMode = 1;
    mYOffsetFromWaterSurface = 0.0f;
    mWaterSplashTime = 5.0f;
    mSplashGenTimeDuringDash = 10.0f;
}

#if DEBUG
void daKago_HIO_c::genMessage(JORMContext* ctx) {
    // friendly Kargarok (literally, "Kargarok (good guy)")
    ctx->genLabel("カーゴロック（いい奴）", 0x80000001);
    ctx->genLabel(" ", 0x80000001);
    ctx->genLabel(" ", 0x80000001);
    ctx->genLabel(" ", 0x80000001);
    // Revocon processing (Revolution controller?)
    ctx->genLabel("レボコン処理", 0x80000001);
    ctx->genLabel(" ", 0x80000001);
    ctx->genSlider("レボコン上下", &mRevoconUpDown, 0.0f, 10000.0f);
    ctx->genSlider("レボコン上下Max", &mRevoconUpDownMax, 0.0f, 10000.0f);
    ctx->genSlider("レボコン左右", &mRevoconLeftRight, 0.0f, 10000.0f);
    ctx->genSlider("レボコン左右Max", &mRevoconLeftRightMax, 0.0f, 10000.0f);
    ctx->genLabel(" ", 0x80000001);
    ctx->genLabel(" ", 0x80000001);
    ctx->genLabel(" ", 0x80000001);
    ctx->genSlider("基本サイズ", &mBasicSize, 0.0f, 5.0f);
    ctx->genSlider("飛行速度", &mFlightSpeed, 0.0f, 100.0f);
    ctx->genSlider("飛行地面高度", &mFlightGroundAltitude, 0.0f, 20000.0f);
    ctx->genSlider("飛行天井高度", &mFlightCeilingAltitude, 0.0f, 20000.0f);
    ctx->genSlider("下降速度加算値", &mDescentRateIncrement, 0.0f, 100.0f);
    ctx->genSlider("上昇速度減速値", &mAscentRateDecel, 0.0f, 100.0f);
    ctx->genSlider("影の濃さ", &mShadowDensity, 0.0f, 10.0f);
    ctx->genSlider("壁かすり後無敵時間", &mWallHitInvulnTime, 0.0f, 100.0f);
    ctx->genSlider("ダッシュ時間", &mDashTime, 0.0f, 100.0f);
    ctx->genSlider("ダッシュ溜め時間(当社比X倍)", &mDashTimeMultiplier, 0.0f, 10.0f);
    ctx->genSlider("ダッシュ無効時間", &mDashCooldownTime, 0.0f, 1000.0f);
    ctx->genLabel("-", 0x80000001);
    ctx->genLabel("---------------------------------------", 0x80000001);
    // "Below this are experimental parameters"
    ctx->genLabel("これより下は効果線エフェクト実験パラメータ", 0x80000001);
    ctx->genLabel("-", 0x80000001);
    ctx->genSlider("Ｚオフセット", &mZOffset, 0.0f, 10000.0f);
    ctx->genSlider("Ｚオフセット(横)", &mZOffsetHori, 0.0f, 10000.0f);
    ctx->genCheckBox("角度追従モード", &mAngleTrackingMode, 0x1);
    // check OFF to track camera speed
    ctx->genLabel(".....チェックＯＦＦでカメラの速度追従", 0x80000001);
    // check ON to track Kargarok angle
    ctx->genLabel(".....チェックＯＮでカーゴの角度追従", 0x80000001);
    ctx->genLabel("-", 0x80000001);
    // "Secret ripple effect parameters"
    ctx->genLabel("こっそりさざ波エフェクトパラメータ", 0x80000001);
    ctx->genLabel("-", 0x80000001);
    ctx->genSlider("水面からのYオフセット", &mYOffsetFromWaterSurface, -1000.0f, 1000.0f);
    ctx->genSlider("着水時飛沫発生時間", &mWaterSplashTime, 0.0f, 100.0f);
    ctx->genSlider("ダッシュ時飛沫発生時間", &mSplashGenTimeDuringDash, 0.0f, 100.0f);
}
#endif

int daKago_c::getBckName(int i_resIdx) {
    if (mType == TYPE_TWILIGHT) {
        return i_resIdx;
    }

    // convert shadow kargorok res idx to normal kargorok res idx
    switch (i_resIdx) {
    case dRes_ID_E_YC_BCK_YC_CRASH_e:        return dRes_ID_E_KC_BCK_KC_CRASH_e;
    case dRes_ID_E_YC_BCK_YC_CRASH2_e:       return dRes_ID_E_KC_BCK_KC_CRASH2_e;
    case dRes_ID_E_YC_BCK_YC_FLY_e:          return dRes_ID_E_KC_BCK_KC_FLY_e;
    case dRes_ID_E_YC_BCK_YC_FLY_BRAKE_e:    return dRes_ID_E_KC_BCK_KC_FLY_BRAKE_e;
    case dRes_ID_E_YC_BCK_YC_FLY_DASH_WL_e:  return dRes_ID_E_KC_BCK_KC_FLY_DASH_WL_e;
    case dRes_ID_E_YC_BCK_YC_FLY_GLIDE_e:    return dRes_ID_E_KC_BCK_KC_FLY_GLIDE_e;
    case dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e: return dRes_ID_E_KC_BCK_KC_FLY_GLIDE_WL_e;
    case dRes_ID_E_YC_BCK_YC_FLY_LIMIT_WL_e: return dRes_ID_E_KC_BCK_KC_FLY_LIMIT_WL_e;
    case dRes_ID_E_YC_BCK_YC_FLY_WL_e:       return dRes_ID_E_KC_BCK_KC_FLY_WL_e;
    case dRes_ID_E_YC_BCK_YC_HOVERING_e:     return dRes_ID_E_KC_BCK_KC_HOVERING_e;
    default:                                 return dRes_ID_E_KC_BCK_KC_FLY_e;
    }
}

void daKago_c::setBck(int i_anm, u8 i_mode, f32 i_morf, f32 i_speed) {
    mAnm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(mArcName, getBckName(i_anm)),
                   i_mode, i_morf, i_speed, 0.0f, -1.0f);
}

bool daKago_c::checkBck(int i_anm) {
    if (mAnm_p->getAnm() == dComIfG_getObjectRes(mArcName, getBckName(i_anm))) {
        return true;
    } else {
        return false;
    }
}

int daKago_c::draw() {
    if (field_0x6de == 0) {
        return 1;
    }

    g_env_light.settingTevStruct(2, &current.pos, &tevStr);
    J3DModel* model = mAnm_p->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    fopAcM_setEffectMtx(this, model->getModelData());

    dComIfGd_setListDark();
    mAnm_p->entryDL();
    dComIfGd_setList();

    cXyz unkXyz1;
    unkXyz1.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    mShadowId =
        dComIfGd_setShadow(mShadowId, 0, model, &unkXyz1, 3500.0f, 0.0f, current.pos.y,
                           mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f,
                           dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static int daKago_Draw(daKago_c* i_this) {
    return i_this->draw();
}

int daKago_c::executeBalloonMenu() {
    dCamera_c* camera = dCam_getBody();

    switch (mBalloonMenuMode) {
    case 0:
        if (mType == TYPE_NORMAL && (mAction == ACTION_FLY_e || mAction == ACTION_ATTACK_e) &&
            !dComIfGp_event_runCheck() && mDoCPd_c::getTrigB(PAD_1))
        {
            mBalloonMenuMode = 1;
            return 1;
        }

        return 0;
    case 1:
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 1, 0xffff, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);

            #if VERSION == VERSION_SHIELD_DEBUG
            return 0;
            #else
            break;
            #endif
        }

        mMsgFlow.init(this, 0x457, 0, NULL);

        camera->Stop();
        camera->SetTrimSize(3);

        mBalloonMenuMode = 2;
        break;
    case 2:
        if (mMsgFlow.doFlow(this, NULL, 0)) {
            if (dMsgObject_getSelectCursorPos() == 0) {
                mBalloonMenuMode = 0;

                camera->Start();
                camera->SetTrimSize(0);

                dComIfGp_event_reset();
                return 1;
            }

            if (dMsgObject_getSelectCursorPos() == 1) {
                mBalloonMenuMode = 3;
                dComIfGp_setNextStage("F_SP112", 0, dComIfGp_roomControl_getStayNo(),
                                      dComIfG_play_c::getLayerNo(0), 0.0f, 10, 1, 0, 0, 1, 0);
            } else {
                mBalloonMenuMode = 3;
                dStage_changeScene(3, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            }
        }
        break;
    }

    return 1;
}

static u8 hio_set;
static daKago_HIO_c l_HIO;

f32 daKago_c::checkGroundHeight(cXyz i_pos, f32* o_step) {
    f32 gnd_height = mGroundHeight;

    if (mpPath1 != NULL) {
        cXyz nextPntPos;
        cXyz prevPntPos;
        cXyz cStack_140;

        nextPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;

        int prevPntNo = (int)mPathCurrentPointNo - (int)mPathStep;
        if (prevPntNo >= mpPath1->m_num || prevPntNo < 0) {
            prevPntNo = mPathCurrentPointNo;
        }
        prevPntPos = dPath_GetPnt(mpPath1, prevPntNo)->m_position;

        s16 pntAngle = cLib_targetAngleY(&prevPntPos, &nextPntPos);

        mDoMtx_stack_c::YrotS(-pntAngle);
        mDoMtx_stack_c::transM(-i_pos.x, -i_pos.y, -i_pos.z);
        mDoMtx_stack_c::multVec(&prevPntPos, &cStack_140);

        f32 var_f29 = -cStack_140.z;
        if (var_f29 < 0.0f) {
            var_f29 = 0.0f;
        }

        mDoMtx_stack_c::YrotS(-pntAngle);
        mDoMtx_stack_c::transM(-i_pos.x, -i_pos.y, -i_pos.z);
        mDoMtx_stack_c::multVec(&nextPntPos, &cStack_140);

        f32 var_f28 = cStack_140.z;
        if (var_f28 < 0.0f) {
            var_f28 = 0.0f;
        }

        f32 var_f27 = var_f29 / (var_f29 + var_f28);
        gnd_height = nextPntPos.y * var_f27 + prevPntPos.y * (1.0f - var_f27);

        if (o_step != NULL) {
            f32 pntDistXZ = nextPntPos.absXZ(prevPntPos);
            f32 pntDistY = std::abs(nextPntPos.y - prevPntPos.y);
            *o_step = pntDistY * l_HIO.mFlightSpeed / pntDistXZ + 5.0f;
        }
    }

    field_0x6e0 = 0;

    dBgS_GndChk gndChk;
    cXyz gndChkPos(i_pos.x, i_pos.y + 500.0f, i_pos.z);
    gndChk.SetPos(&gndChkPos);
    mGroundFlyHeight = dComIfG_Bgsp().GroundCross(&gndChk);

    mGndSpecialCode = 0;

    if (mGroundFlyHeight != -G_CM3D_F_INF) {
        mGndSpecialCode = dComIfG_Bgsp().GetSpecialCode(gndChk);

        mGroundFlyHeight += l_HIO.mFlightGroundAltitude;
        if (gnd_height < mGroundFlyHeight) {
            gnd_height = mGroundFlyHeight;
            if (current.pos.y < gnd_height) {
                mGroundHeight = current.pos.y;
            } else {
                mGroundHeight = mGroundFlyHeight;
            }

            if (o_step != NULL) {
                *o_step = 80.0f;
            }
        }
    }

    BOOL unkFlag1 = FALSE;

    dBgS_ObjGndChk_Spl gndChkSpl;
    gndChkSpl.SetPos(&gndChkPos);
    f32 gndCrossMag = dComIfG_Bgsp().GroundCross(&gndChkSpl);
    if (gndCrossMag != -G_CM3D_F_INF) {
        field_0x70c = gndCrossMag;
        gndCrossMag += l_HIO.mFlightGroundAltitude;
        if (current.pos.y < gndCrossMag + 50.0f) {
            unkFlag1 = TRUE;
        }

        if (gnd_height < gndCrossMag) {
            mGroundFlyHeight = gndCrossMag;
            gnd_height = gndCrossMag;
            field_0x6e0 = 1;

            if (current.pos.y < gndCrossMag) {
                mGroundHeight = current.pos.y;
            } else {
                mGroundHeight = mGroundFlyHeight;
            }

            if (o_step != NULL) {
                *o_step = 80.0f;
            }
        }
    }

    if (unkFlag1) {
        if (field_0x6e6 == 0) {
            field_0x6e6 = 1;
        }
    } else {
        mWaterSplashTimer = 0;
        field_0x6e6 = 0;
    }

    return gnd_height;
}

f32 daKago_c::checkRoofHeight(cXyz i_pos) {
    f32 roof_height = mRoofHeight;
    BOOL unkFlag1 = FALSE;

    field_0x6e5 = 0;

    dBgS_RoofChk roofChk;
    cXyz chkpos(i_pos.x, i_pos.y - 500.0f, i_pos.z);
    roofChk.SetPos(chkpos);

    roof_height = dComIfG_Bgsp().RoofChk(&roofChk);
    if (roof_height != G_CM3D_F_INF) {
        roof_height -= l_HIO.mFlightCeilingAltitude;
        if (current.pos.y > roof_height) {
            mRoofHeight = current.pos.y;
        } else {
            mRoofHeight = roof_height;
        }

        if (dComIfG_Bgsp().GetSpecialCode(roofChk) == 1) {
            unkFlag1 = TRUE;
        }

        if (std::abs(current.pos.y - roof_height) < 310.0f) {
            fopAc_ac_c* actor = dComIfG_Bgsp().GetActorPointer(roofChk);
            if (actor != NULL && fopAcM_GetName(actor) == PROC_Obj_RIVERROCK) {
                if (((daObjRIVERROCK_c*)actor)->mBreakSubAction == daObjRIVERROCK_c::BREAK_MOVE) {
                    field_0x6e5 = 1;
                }
            }
        }
    } else {
        mRoofHeight = G_CM3D_F_INF;
    }

    if (!unkFlag1 && current.pos.y >= mRoofHeight - 100.0f) {
        if (field_0x6e3 == 0) {
            field_0x6e3 = 1;
        }
    } else {
        field_0x6e3 = 0;
    }

    return roof_height;
}

void daKago_c::checkMoveHeight() {
    f32 step = 100.0f;
    f32 target = checkGroundHeight(current.pos, &step);
    cLib_chaseF(&mGroundHeight, target, step);
    cLib_chaseF(&mRoofHeight, checkRoofHeight(current.pos), 30.0f);
}

void daKago_c::checkSizeBg() {
    dBgS_LinChk linChk;
    cXyz line_start;
    cXyz line_end;
    cM3dGPla plane;

    J3DModel* model = mAnm_p->getModel();

    field_0x6d9 = 0;

    if (mAction == ACTION_WAIT_e) {
        return;
    }

    mDoMtx_stack_c::copy(model->getAnmMtx(YC_JNT_SHOULDERL_1_e));
    mDoMtx_stack_c::multVecZero(&line_start);
    mDoMtx_stack_c::copy(model->getAnmMtx(YC_JNT_HANDL_3_e));
    mDoMtx_stack_c::multVecZero(&line_end);
    line_end = line_start + (line_end - line_start) * 0.9f;
    linChk.Set(&line_start, &line_end, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (dComIfG_Bgsp().GetSpecialCode(linChk) == 1) {
            field_0x6d9 |= (u8)1;
            mWallHitEffPos[0] = linChk.GetCross();
        }
    }

    mDoMtx_stack_c::copy(model->getAnmMtx(YC_JNT_SHOULDERR_1_e));
    mDoMtx_stack_c::multVecZero(&line_start);
    mDoMtx_stack_c::copy(model->getAnmMtx(YC_JNT_HANDR_3_e));
    mDoMtx_stack_c::multVecZero(&line_end);
    line_end = (line_start + (line_end - line_start) * 0.9f);
    linChk.Set(&line_start, &line_end, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (dComIfG_Bgsp().GetSpecialCode(linChk) == 1) {
            field_0x6d9 |= (u8)0x2;
            mWallHitEffPos[1] = linChk.GetCross();
        }
    }

    line_start = current.pos;
    mDoMtx_stack_c::copy(model->getAnmMtx(YC_JNT_BACKBONE1_e));
    mDoMtx_stack_c::multVecZero(&line_start);
    mDoMtx_stack_c::transM(300.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&line_end);
    linChk.Set(&line_start, &line_end, NULL);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (dComIfG_Bgsp().GetSpecialCode(linChk) == 1) {
            if (!checkYaguraPos(linChk.GetCross())) {
                field_0x6d9 |= (u8)4;
                mWallHitEffPos[2] = linChk.GetCross();
            }
        }
    }
}

s16 daKago_c::getBeforeGroundHeight(u8 param_0) {
    return 0;
}

void daKago_c::demo_skip(int i_parameter) {
    dCamera_c* camera = dCam_getBody();

    switch (i_parameter) {
    case 0:
        mDemoMode = 2;
        field_0x74c = 0;
        break;
    case 1:
        field_0x728 = 0;
        break;
    case 3:
        mMsgFlow.remove();
    case 2:
        setMidnaRideOn();
        setPlayerRideOn();
        mHeadRotZ = mHeadRotY = 0;
        /* dSv_event_flag_c::M_051 - Main Event - Shadow Kargorok (?) (Large) event complete (Horse grass appears in various places) */
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[84]);
        mDemoMode = 7;
        field_0x74c = 0;
        break;
    case 4:
        setSceneChange(0);
        field_0x6e4 = 1;
        break;
    case 5:
    case 6:
        if (setSceneChange(3)) {
            if (i_parameter == 5) {
                mDemoMode = 6;
            } else {
                mDemoMode = 5;
                field_0x6cc = 1.0f;
            }
        }
        break;
    case 7:
        setRideOff();
    case 8: {
        mDemoCamCenter.set(-77875.0f, -18287.0f, 42000.0f);
        mDemoCamEye.set(-77275.0f, -18500.0f, 41090.0f);
        field_0x6a4.set(-77615.0f, -18640.0f, 41400.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&field_0x6a4, 0, 0);
        field_0x6de = 0;
        speed.y = speedF = 0.0f;
        setActionMode(ACTION_WAIT_e, 0);
        camera->Set(mDemoCamCenter, mDemoCamEye, 70.0f, 0);
        camera->Reset();
        camera->Start();
        camera->SetTrimSize(0);
        dComIfGp_event_reset();
        Z2GetAudioMgr()->setDemoName(NULL);
        break;
    }
    case 9:
        setRideOff();
    case 10:
        mDemoCamCenter.set(3703.0f, 337.0f, 863.0f);
        mDemoCamEye.set(3726.0f, 272.0f, 1196.0f);
        field_0x6a4.set(3782.0f, 222.0f, 690.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&field_0x6a4, 0, 0);
        field_0x6de = 0;
        speed.y = speedF = 0.0f;
        setActionMode(ACTION_WAIT_e, 0);
        camera->Set(mDemoCamCenter, mDemoCamEye, 70.0f, 0);
        camera->Reset();
        camera->Start();
        camera->SetTrimSize(0);
        dComIfGp_event_reset();
        Z2GetAudioMgr()->setDemoName(NULL);
    }
}

int daKago_c::DemoSkipCallBack(void* i_this, int i_parameter) {
    if (i_this != NULL) {
        ((daKago_c*)i_this)->demo_skip(i_parameter);
        return TRUE;
    }

    return FALSE;
}

void daKago_c::setActionMode(int i_action, int i_mode) {
    mIsFlying = false;
    mAction = i_action;
    mMode = i_mode;
}

void daKago_c::setMidnaTagPos() {
    cXyz tagPos;
    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    if (midna != NULL) {
        mDoMtx_stack_c::copy(getMidnaLocaterMtx());
        mDoMtx_stack_c::multVecZero(&tagPos);
        midna->onTagWaitPos(&tagPos);
    }
}

void daKago_c::setMidnaRideOn() {
    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    if (midna != NULL) {
        midna->offTagWaitPos();
        midna->setCargoActor(this);
    }
}

void daKago_c::setPlayerRideOn() {
    daPy_getPlayerActorClass()->setCargoCarry(this);
}

void daKago_c::setRideOff() {
    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    if (midna != NULL) {
        midna->offTagWaitPos();
        midna->offCargoActor();
    }

    daPy_getPlayerActorClass()->offCargoCarry();
}

s8 daKago_c::searchNearPassPoint() {
    cXyz pointPos;

    cXyz playerPos(daPy_getPlayerActorClass()->current.pos);
    f32 prev_nearest_distXZ, nearest_distXZ;
    nearest_distXZ = prev_nearest_distXZ = 100000.0f;
    int nearest_point_no;

    for (int i = 0; i < mpPath1->m_num; i++) {
        pointPos = dPath_GetPnt(mpPath1, i)->m_position;

        f32 player_pnt_distXZ = playerPos.absXZ(pointPos);
        if (player_pnt_distXZ < nearest_distXZ) {
            prev_nearest_distXZ = nearest_distXZ;
            nearest_distXZ = player_pnt_distXZ;
            nearest_point_no = i;
        } else if (player_pnt_distXZ < prev_nearest_distXZ) {
            prev_nearest_distXZ = player_pnt_distXZ;
        }
    }

    int next_point_no = nearest_point_no + mPathStep;
    if (next_point_no < 0) {
        next_point_no = 1;
    } else if (next_point_no >= mpPath1->m_num) {
        next_point_no = mpPath1->m_num - 1;
    }

    return next_point_no;
}

int daKago_c::setSceneChange(int i_mode) {
    int exitID = 0;

    switch (i_mode) {
    case 0:
        if (mType == TYPE_TWILIGHT) {
            exitID = 1;
        } else {
            exitID = 6;
        }
        break;
    case 1:
        exitID = 0;
        endBalloonScore();
        break;
    case 2:
        if (mType == TYPE_TWILIGHT) {
            exitID = 3;
        } else {
            exitID = 0;
        }

        endBalloonScore();
        break;
    case 3:
        exitID = 3;
        break;
    }

    return dStage_changeScene(exitID, 0.0f, 10, fopAcM_GetRoomNo(this), 0, -1);
}

void daKago_c::createBalloonScore() {
    if (mType != TYPE_TWILIGHT && mBalloon2DId == fpcM_ERROR_PROCESS_ID_e) {
        mBalloon2DId = fopAcM_create(PROC_BALLOON2D, 0, NULL, -1, NULL, NULL, -1);
        field_0x6e9 = 1;
    }
}

void daKago_c::startBalloonScore() {
    fopAc_ac_c* balloon_actor;
    if (mType != TYPE_TWILIGHT && field_0x6e9 == 1) {
        fopAcM_SearchByID(mBalloon2DId, &balloon_actor);
        if (balloon_actor != NULL) {
            ((daBalloon2D_c*)balloon_actor)->show();
            ((daBalloon2D_c*)balloon_actor)->setScoreCount(0);
            field_0x6e9 = 2;
        }
    }
}

void daKago_c::endBalloonScore() {
    fopAc_ac_c* balloon2D;
    fopAc_ac_c* balloonObj;

    if (mType != TYPE_TWILIGHT) {
        fopAcM_SearchByID(mBalloon2DId, &balloon2D);
        if (balloon2D != NULL) {
            ((daBalloon2D_c*)balloon2D)->hide();
        }

        fopAcM_SearchByName(PROC_OBJ_BALLOON, &balloonObj);
        if (balloonObj != NULL) {
            ((daObj_Balloon_c*)balloonObj)->saveBestScore();
        }
    }
}

f32 daKago_c::checkNextPath(cXyz i_pntPos) {
    cXyz cStack_14;
    mDoMtx_stack_c::YrotS(-field_0x714);
    mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
    mDoMtx_stack_c::multVec(&i_pntPos, &cStack_14);
    return cStack_14.z;
}

void daKago_c::checkHeight() {
    if (current.pos.y > mRoofHeight) {
        current.pos.y = mRoofHeight;
    } else if (current.pos.y < mGroundHeight) {
        current.pos.y = mGroundHeight;
    }
}

bool daKago_c::checkYaguraPos(cXyz param_0) {
    cXyz local_18(-22984.0f, 0.0f, 7455.0f);
    cXyz cStack_24;
    s16 rot_y = -0x770;
    mDoMtx_stack_c::YrotS(-rot_y);
    mDoMtx_stack_c::transM(-local_18.x, -local_18.y, -local_18.z);
    mDoMtx_stack_c::multVec(&param_0, &cStack_24);

    if (std::abs(cStack_24.z) < 700.0f) {
        return true;
    } else {
        return false;
    }
}

bool daKago_c::checkWallHitFall(int param_0) {
    if (field_0x6e5 != 0) {
        cXyz effpos;
        setActionMode(ACTION_STAGGER_e, 3);
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        effpos = current.pos;
        effpos.y += 200.0f;
        setWallHitEffect(effpos, 0);
        return true;
    }

    if (mObjAcch.ChkWallHit()) {
        field_0x6da = mAcchCir.GetWallAngleY();
        s16 unkInt1 = shape_angle.y - field_0x6da;
        if (!checkYaguraPos(current.pos)) {
            if (abs(unkInt1) > 0x7000) {
                field_0x6d9 |= (u8)0x4;
                mWallHitEffPos[2].set(current.pos.x + cM_ssin(shape_angle.y) * 200.0f, current.pos.y, current.pos.z + cM_scos(shape_angle.y) * 200.0f);
            }
        }

        if (unkInt1 > 0) {
            field_0x6d9 |= (u8)0x1;
            mWallHitEffPos[0].set(current.pos.x + cM_ssin(shape_angle.y + 0x4000) * 200.0f, current.pos.y,
                                  current.pos.z + cM_scos(shape_angle.y + 0x4000) * 200.0f);
        } else {
            field_0x6d9 |= (u8)0x2;
            mWallHitEffPos[1].set(current.pos.x + cM_ssin(shape_angle.y + -0x4000) * 200.0f,
                                  current.pos.y,
                                  current.pos.z + cM_scos(shape_angle.y + -0x4000) * 200.0f);
        }
    }

    if ((field_0x6d9 & 0x4) != 0) {
        setActionMode(ACTION_STAGGER_e, 0);
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        setWallHitEffect(mWallHitEffPos[2], 0);
        return true;
    }

    if (param_0 != 0 && mWallHitInvulnTimer == 0 && (field_0x6d9 & 0x3) != 0) {
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        if ((field_0x6d9 & 0x1) != 0) {
            setActionMode(ACTION_STAGGER_e, 1);
            setWallHitEffect(mWallHitEffPos[0], 1);
        } else {
            setActionMode(ACTION_STAGGER_e, 2);
            setWallHitEffect(mWallHitEffPos[1], 1);
        }
        return true;
    }

    return false;
}

bool daKago_c::checkAttackStart() {
    if (mDashCooldownTime != 0) {
        return false;
    }

    if (mDoCPd_c::getTrigA(PAD_1)) {
        setActionMode(ACTION_ATTACK_e, 0);
        mIsFlying = true;
        return true;
    } else {
        return false;
    }
}

s16 daKago_c::getValueY(f32 param_0) {
    s16 valueY = mStickY * param_0;

    f32 var_f31;
    if (valueY < 0) {
        if (current.pos.y > mRoofHeight - 200.0f) {
            var_f31 = current.pos.y - (mRoofHeight - 200.0f);
            if (var_f31 > 200.0f) {
                var_f31 = 200.0f;
            }
            var_f31 = ((200.0f - var_f31) / 200.0f);
            valueY *= var_f31;
        }
    } else if (valueY > 0 && current.pos.y < mGroundHeight + 200.0f) {
        var_f31 = mGroundHeight + 200.0f - current.pos.y;
        if (var_f31 > 200.0f) {
            var_f31 = 200.0f;
        }
        var_f31 = ((200.0f - var_f31) / 200.0f);
        valueY = valueY * var_f31;
    }

    return valueY;
}

s16 daKago_c::getValueX(f32 param_0) {
    return -mStickX * param_0;
}

s16 daKago_c::getValueAbsX(f32 param_0) {
    return std::abs(-mStickX * param_0);
}

void daKago_c::flySpeedCalcLockOn() {
    s16 angleX;
    s16 angleY;

    angleX = -cLib_targetAngleX(&current.pos, &mpLockActor->current.pos);
    angleY = cLib_targetAngleY(&current.pos, &mpLockActor->current.pos);

    if (current.pos.absXZ(mpLockActor->current.pos) < BREG_F(0) + 200.0f) {
        angleY = mpLockActor->current.angle.y;
    }

    cLib_addCalcAngleS(&current.angle.x, angleX, 8, 0x100, 0x10);
    shape_angle.x = current.angle.x;

    s16 prevAngleY = current.angle.y;
    cLib_addCalcAngleS(&current.angle.y, angleY, 8, 0x100, 0x10);
    shape_angle.y = current.angle.y;

    cLib_addCalcAngleS(&shape_angle.z, ((prevAngleY - current.angle.y) * 0x20), 8, 0x400, 0x10);
    if (shape_angle.z > 0x3000) {
        shape_angle.z = 0x3000;
    }
    if (shape_angle.z < -0x3000) {
        shape_angle.z = -0x3000;
    }
}

void daKago_c::flySpeedCalc(s16 param_0, int i_calcType) {
    s16 var_r29;
    s16 var_r28;
    s16 var_r27;

    s16 sp_8;
    if (i_calcType == 0) {
        sp_8 = 0x2000;
    } else if (i_calcType == 1) {
        sp_8 = 0x1000;
    } else if (i_calcType == 2) {
        sp_8 = 0x2aaa;
    }

    var_r28 = getValueY(sp_8);
    if (mpLockActor != NULL) {
        var_r28 = -cLib_targetAngleX(&current.pos, &mpLockActor->current.pos);
    }

    if (var_r28 != 0) {
        field_0x710 += getValueY(40.0f);

        var_r29 = 0x200;
        if (i_calcType == 2) {
            var_r29 = 0x400;
        }
        if (field_0x710 > var_r29) {
            field_0x710 = var_r29;
        }
        if (field_0x710 < -var_r29) {
            field_0x710 = -var_r29;
        }

        if (mStickY > 0.0f) {
            if (current.angle.x < var_r28) {
                current.angle.x += field_0x710;
            } else {
                cLib_addCalcAngleS(&field_0x710, 0, 8, 0x20, 0x10);
                cLib_addCalcAngleS(&current.angle.x, var_r28, 8, 0x100, 0x40);
            }
        } else {
            if (current.angle.x > var_r28) {
                current.angle.x += field_0x710;
            } else {
                cLib_addCalcAngleS(&field_0x710, 0, 8, 0x20, 0x10);
                cLib_addCalcAngleS(&current.angle.x, var_r28, 8, 0x100, 0x40);
            }
        }
    } else {
        field_0x710 = 0;

        if (current.pos.y < mGroundHeight) {
            var_r28 = getBeforeGroundHeight(0);
        }
        if (current.pos.y > mRoofHeight) {
            var_r28 = getBeforeGroundHeight(1);
        }

        if (var_r28 != 0) {
            cLib_addCalcAngleS(&current.angle.x, var_r28, 8, 0x100, 0x10);
        } else {
            cLib_addCalcAngleS(&current.angle.x, var_r28, 8, 0x100, 0x10);
        }
        current.angle.x += field_0x710;
    }

    if (current.angle.x > 0x2aaa) {
        current.angle.x = 0x2aaa;
    }
    if (current.angle.x < -0x2aaa) {
        current.angle.x = -0x2aaa;
    }
    shape_angle.x = current.angle.x;

    var_r27 = 0x2000;
    if (i_calcType == 1) {
        var_r27 = 0x1000;
    } else if (i_calcType == 2) {
        var_r27 = 0x2aaa;
    }

    f32 unkFloat1 = 20.0f;
    var_r29 = 0x100;
    if (i_calcType == 2) {
        unkFloat1 = 30.0f;
        var_r29 = 0x200;
    }

    var_r28 = param_0 + getValueX(var_r27);
    if (mpLockActor != NULL) {
        var_r28 = cLib_targetAngleY(&current.pos, &mpLockActor->current.pos);
    }

    int always_zero = 0;
    if (!always_zero) {
        if (abs((s16)(current.angle.y - param_0)) < var_r27) {
            if (mStickX) {
                field_0x712 += getValueX(unkFloat1);
                if (field_0x712 > getValueAbsX(var_r29)) {
                    field_0x712 = getValueAbsX(var_r29);
                }
                if (field_0x712 < -getValueAbsX(var_r29)) {
                    field_0x712 = -getValueAbsX(var_r29);
                }
            } else {
                cLib_addCalcAngleS(&field_0x712, 0, 8, 0x20, 0x10);
            }

            current.angle.y += field_0x712;
        } else {
            int sp_14 = 0;
            if (mStickX < 0.0f) {
                if ((s16)(current.angle.y - param_0) < var_r27) {
                    sp_14 = 1;
                }
            } else {
                if (mStickX > 0.0f && (s16)(current.angle.y - param_0) > var_r27) {
                    sp_14 = 1;
                }
            }

            if (sp_14 != 0) {
                field_0x712 += getValueX(unkFloat1);
                if (field_0x712 > getValueAbsX(var_r29)) {
                    field_0x712 = getValueAbsX(var_r29);
                }
                if (field_0x712 < -getValueAbsX(var_r29)) {
                    field_0x712 = -getValueAbsX(var_r29);
                }
                current.angle.y += field_0x712;
            } else {
                cLib_addCalcAngleS(&field_0x712, 0, 8, 0x20, 0x10);
                cLib_addCalcAngleS(&current.angle.y, var_r28, 8, 0x40, 0x10);
            }
        }
    }

    shape_angle.y = current.angle.y;

    cLib_addCalcAngleS(&shape_angle.z, -field_0x712 * 0x20, 8, 0x400, 0x10);
    if (shape_angle.z > 0x3000) {
        shape_angle.z = 0x3000;
    }
    if (shape_angle.z < -0x3000) {
        shape_angle.z = -0x3000;
    }

    if (field_0x6e3 == 1) {
        field_0x6e3 = 2;
        if (mType == TYPE_NORMAL) {
            mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
        } else {
            mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
        }
    }

    if (field_0x6e6 == 1) {
        field_0x6e6 = 2;
        if (mAction == ACTION_ATTACK_e) {
            mWaterSplashTimer = l_HIO.mSplashGenTimeDuringDash;
        } else {
            mWaterSplashTimer = l_HIO.mWaterSplashTime;
        }
    }
}

bool daKago_c::checkFlySceneChange() {
    if (((mPathStep > 0 && mPathCurrentPointNo >= mpPath1->m_num - 1) || (mPathStep < 0 && mPathCurrentPointNo < 1))
        && mSceneType == SCENE_TYPE_RIVER
        && !fopOvlpM_IsPeek())
    {
        setActionMode(ACTION_FLY_e, 4);
        field_0x728 = 300;

        if (!checkBck(dRes_ID_E_YC_BCK_YC_FLY_e)) {
            setBck(dRes_ID_E_YC_BCK_YC_FLY_e, 2, 10.0f, 1.0f);
        }

        if (mPathStep > 0) {
            setSceneChange(1);
        } else {
            setSceneChange(2);
        }

        return true;
    }

    return false;
}

void daKago_c::setFlyAway() {
    current.angle.y = shape_angle.y;
    setActionMode(ACTION_FLY_e, 3);
    field_0x728 = 90;

    if (!checkBck(dRes_ID_E_YC_BCK_YC_FLY_e)) {
        setBck(dRes_ID_E_YC_BCK_YC_FLY_e, 2, 10.0f, 1.0f);
    }

    dComIfGp_getVibration().StopQuake(0x1f);
}

void daKago_c::setFlyAnime() {
    if (cM_rnd() < 0.5) {
        if (!checkBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e)) {
            setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);
        } else {
            setBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e, 2, 10.0f, 1.0f);
        }
    } else {
        if (!checkBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e)) {
            setBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e, 2, 10.0f, 1.0f);
        }
    }

    field_0x728 = 150;
}

void daKago_c::executeFly() {
    s16 pntAngleY;
    s16 pntAngleX;
    f32 targetFlySpeed;
    cXyz nextPntPos;
    cXyz prevPntPos;
    cXyz unkXyz3;

    startBalloonScore();

    if (mMode < 3) {
        if (mDashCooldownTime == 0) {
            dComIfGp_setDoStatusForce(BUTTON_STATUS_UNK_75, 0);
        }

        if (mType == TYPE_NORMAL) {
            dComIfGp_setAStatusForce(BUTTON_STATUS_QUIT, 0);
        } else {
            dComIfGp_setAStatusForce(BUTTON_STATUS_UNK_39, 0);
        }
    }

    if (field_0x6e6 != 0) {
        setSibukiEffect();
    }

    checkHeight();
    mIsFlying = true;

    switch (mMode) {
    case 0:
        shape_angle.y = current.angle.y;
        mGroundHeight = checkGroundHeight(current.pos, NULL);
        mRoofHeight = checkRoofHeight(current.pos);
        setPlayerRideOn();
    case 1:
        setFlyAnime();
        field_0x710 = field_0x712 = 0;
        mMode = 2;
        break;
    case 2:
        nextPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
        prevPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo - mPathStep)->m_position;
        pntAngleY = cLib_targetAngleY(&prevPntPos, &nextPntPos);
        field_0x714 = pntAngleY;

        flySpeedCalc(pntAngleY, 0);

        if (checkBck(dRes_ID_E_YC_BCK_YC_FLY_DASH_WL_e)) {
            if (mAnm_p->isStop()) {
                setFlyAnime();
            }
        } else {
            if (field_0x710 < 0) {
                if (!checkBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e)) {
                    setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);
                }
                field_0x728 = 10;
            } else if (field_0x710 > 0) {
                if (!checkBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e)) {
                    setBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e, 2, 10.0f, 1.0f);
                }
                field_0x728 = 90;
            } else {
                if (field_0x728 == 0) {
                    setFlyAnime();
                }
            }
        }

        if (cM_ssin(current.angle.x) > 0.0f) {
            targetFlySpeed = l_HIO.mFlightSpeed + l_HIO.mDescentRateIncrement * cM_ssin(current.angle.x);
        } else {
            targetFlySpeed = l_HIO.mFlightSpeed + l_HIO.mAscentRateDecel * cM_ssin(current.angle.x);
        }

        if (std::abs(mFlySpeed - targetFlySpeed) > 10.0f) {
            cLib_chaseF(&mFlySpeed, targetFlySpeed, 2.0f);
        } else {
            cLib_chaseF(&mFlySpeed, targetFlySpeed, 1.0f);
        }

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        unkXyz3.z = checkNextPath(nextPntPos);
        if (unkXyz3.z < 0.0f) {
            if (abs((s16)(pntAngleY - cLib_targetAngleY(&prevPntPos, &current.pos))) < 0x4000) {
                mPathCurrentPointNo += mPathStep;
                if (checkFlySceneChange()) {
                    return;
                }
            }
        }
        break;
    case 3:
    case 4:
        if (mPathCurrentPointNo >= mpPath1->m_num) {
            pntAngleY = current.angle.y;
            pntAngleX = 0;
        } else {
            nextPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
            prevPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo - mPathStep)->m_position;
            pntAngleY = cLib_targetAngleY(&prevPntPos, &nextPntPos);
            pntAngleX = -cLib_targetAngleX(&prevPntPos, &nextPntPos);
        }

        if (mMode == 3) {
            cLib_chaseUC(&field_0x6de, 0, 4);
            cLib_addCalcAngleS(&current.angle.x, -0x2000, 8, 0x100, 0x10);
        } else {
            cLib_addCalcAngleS(&current.angle.x, pntAngleX, 8, 0x100, 0x10);
        }

        shape_angle.x = current.angle.x;
        cLib_addCalcAngleS(&current.angle.y, pntAngleY, 8, 0x400, 0x10);
        shape_angle.y = current.angle.y;
        cLib_addCalcAngleS(&shape_angle.z, 0, 8, 0x400, 0x10);
        targetFlySpeed = l_HIO.mFlightSpeed + 20.0f;

        if (std::abs(mFlySpeed - targetFlySpeed) > 10.0f) {
            cLib_chaseF(&mFlySpeed, targetFlySpeed, 2.0f);
        } else {
            cLib_chaseF(&mFlySpeed, targetFlySpeed, 1.0f);
        }

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            setActionMode(ACTION_WAIT_e, 0);
        }
        return;
    }

    if (!checkWallHitFall(1) && checkAttackStart()) {
        return;
    }
}

void daKago_c::executeStagger() {
    dCamera_c* camera = dCam_getBody();

    cXyz nextPntPos = (Vec)dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
    cXyz prevPntPos = (Vec)dPath_GetPnt(mpPath1, mPathCurrentPointNo - mPathStep)->m_position;

    field_0x714 = cLib_targetAngleY(&prevPntPos, &nextPntPos);

    if (mType == TYPE_NORMAL) {
        dComIfGp_setAStatusForce(BUTTON_STATUS_QUIT, 0);
    } else {
        dComIfGp_setAStatusForce(BUTTON_STATUS_UNK_39, 0);
    }

    checkHeight();

    mIsFlying = true;

    switch (mMode) {
    case 0:
        mSph.OffAtSetBit();

        daPy_getPlayerActorClass()->setPlayerDamage(0, TRUE);

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(YC_JNT_HEAD_e));
        mDoMtx_stack_c::multVecZero(&field_0x674);

        if (mType == TYPE_TWILIGHT) {
            mSound.startCreatureSound(Z2SE_EN_YC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_YC_V_HANGED, -1);
        } else {
            mSound.startCreatureSound(Z2SE_EN_KC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_KC_V_CRASH, -1);
        }

        setBck(dRes_ID_E_YC_BCK_YC_CRASH_e, 0, 10.0f, 1.0f);

        field_0x728 = 60;
        speedF = 30.0f;

        if (current.pos.y < nextPntPos.y) {
            speed.y = 30.0f;
        } else {
            speed.y = -30.0f;
        }

        current.angle.y = field_0x6da - (s16)(current.angle.y - field_0x6da) + 0x8000;

        mMode = 4;
        break;
    case 1:
    case 2:
        mSph.OffAtSetBit();

        daPy_getPlayerActorClass()->setPlayerDamage(0, TRUE);

        setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);

        field_0x728 = 10;
        mWallHitInvulnTimer = (int)l_HIO.mWallHitInvulnTime;
        speedF = 40.0f;

        if (mMode == 1) {
            shape_angle.z = 0x3000;
            ANGLE_ADD(current.angle.y, -0x2000);
            if (abs((s16)(current.angle.y - field_0x714)) > 0x2000) {
                current.angle.y = field_0x714 + -0x2000;
            }
        } else {
            shape_angle.z = -0x3000;
            ANGLE_ADD(current.angle.y, 0x2000);
            if (abs((s16)(current.angle.y - field_0x714)) > 0x2000) {
                current.angle.y = field_0x714 + 0x2000;
            }
        }

        mSound.startCreatureSound(Z2SE_EN_YC_HIT_SIDE, 0, -1);
        dComIfGp_getVibration().StartQuake(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        mMode = 5;
        break;
    case 3:
        daPy_getPlayerActorClass()->setPlayerDamage(2, TRUE);

        mSph.OffAtSetBit();

        mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(YC_JNT_HEAD_e));
        mDoMtx_stack_c::multVecZero(&field_0x674);

        if (mType == TYPE_TWILIGHT) {
            mSound.startCreatureSound(Z2SE_EN_YC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_YC_V_HANGED, -1);
        } else {
            mSound.startCreatureSound(Z2SE_EN_KC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_KC_V_CRASH, -1);
        }

        setBck(dRes_ID_E_YC_BCK_YC_CRASH_e, 0, 10.0f, 1.0f);

        field_0x728 = 60;
        speedF = 30.0f;
        speed.y = -50.0f;

        mMode = 6;
        break;
    case 5:
        cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x400, 0x40);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);
        cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x400, 0x10);
        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x400, 0x10);
        cLib_addCalcAngleS(&shape_angle.z, 0, 8, 0x400, 0x40);

        if (checkWallHitFall(0)) {
            dComIfGp_getVibration().StopQuake(0x1f);
            break;
        }

        if (field_0x728 != 0) {
            break;
        }

        shape_angle.y = current.angle.y;
        mFlySpeed = 40.0f;

        setActionMode(ACTION_FLY_e, 1);

        dComIfGp_getVibration().StopQuake(0x1f);
        break;
    case 4:
    case 6:
        if (checkBck(dRes_ID_E_YC_BCK_YC_CRASH_e)) {
            cXyz line_end;
            mDoMtx_stack_c::copy(mLegR3Mtx);
            mDoMtx_stack_c::multVecZero(&line_end);
            line_end.y -= 20.0f;

            cXyz line_start = current.pos;
            line_start.y += 50.0f;

            dBgS_LinChk linechk;
            linechk.Set(&line_start, &line_end, NULL);

            if (dComIfG_Bgsp().LineCross(&linechk)) {
                current.pos.y += 5.0f;
            }

            setDamageEffect();

            field_0x728 = 60;

            if (mAnm_p->checkFrame(22.0f)) {
                setRideOff();
                setBck(dRes_ID_E_YC_BCK_YC_CRASH2_e, 2, 10.0f, 1.0f);
            }
        }

        if (mMode == 4) {
            cLib_addCalcAngleS(&shape_angle.y, current.angle.y + 0x8000, 8, 0x100, 0x10);
        } else {
            cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 8, 0x100, 0x10);
        }

        cLib_chaseF(&speedF, 0.0f, 0.5f);
        cLib_chaseF(&speed.y, 0.0f, 1.0f);

        cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x400, 0x10);
        cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x400, 0x10);
        cLib_addCalcAngleS(&shape_angle.z, 0, 8, 0x400, 0x40);

        if (field_0x728 == 0) {
            setFlyAway();
            mFlySpeed = -10.0f;
        }
        break;
    }
}

void daKago_c::executeWait() {
    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    if (field_0x728 == 0) {
        cLib_chaseUC(&field_0x6de, 0, 4);
    }

    if (field_0x6df == 0) {
        if (daPy_py_c::checkWolfCargoCarrySceneChange()) {
            if (mSceneType == SCENE_TYPE_RIVER) {
                createBalloonScore();
                if (dComIfGp_getStartStagePoint() == 0) {
                    mPathStep = 1;
                    mPathCurrentPointNo = 0;
                    mPathDir = 1;
                } else {
                    mPathStep = -1;
                    mPathCurrentPointNo = mpPath1->m_num + -1;
                    mPathDir = -1;
                }

                current.pos = old.pos = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
                mPathCurrentPointNo += mPathStep;
                field_0x6a4 = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;

                current.angle.y = shape_angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
                if (midna != NULL) {
                    setMidnaRideOn();
                    setPlayerRideOn();

                    field_0x6dc = 0;
                    field_0x6de = 0xff;
                    field_0x6df = 1;

                    if (mPathDir == 1 && mType == TYPE_TWILIGHT && !dComIfGs_isSaveSwitch(9)) {
                        setActionMode(ACTION_DEMO_FLY_e, 0);
                    } else {
                        setActionMode(ACTION_DEMO_FLY2_e, 0);
                    }

                    setFlyAnime();
                    moveDemoFly();
                }
            } else if (mSceneType == SCENE_TYPE_LAKE_HYLIA || mSceneType == SCENE_TYPE_BOARD_HOUSE) {
                setMidnaRideOn();
                setPlayerRideOn();

                field_0x6dc = 0;
                field_0x6de = 0xff;
                field_0x6df = 1;

                setActionMode(ACTION_LANDING_e, 0);
                executeLanding();
            }
        }
        return;
    }

#if DEBUG
    mPathDir = 1;

    if (mDoCPd_c::getHoldL(PAD_1) && mDoCPd_c::getHoldR(PAD_1) &&
        mDoCPd_c::getTrigB(PAD_1))
    {
        setActionMode(ACTION_EVENT_e, 0);

        if (mSceneType == SCENE_TYPE_BOARD_HOUSE) {
            setKagoPath(1);
        } else {
            setKagoPath(5);

            if (mSceneType == SCENE_TYPE_RIVER) {
                createBalloonScore();
                return;
            }
        }
    }
#endif
}

bool daKago_c::calcAttackMove(int param_0) {
    cXyz nextPntPos;
    cXyz prevPntPos;
    cXyz cStack_38;

    nextPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
    prevPntPos = dPath_GetPnt(mpPath1, mPathCurrentPointNo - mPathStep)->m_position;

    s16 pntAngleY = cLib_targetAngleY(&prevPntPos, &nextPntPos);
    field_0x714 = pntAngleY;

    if (mpLockActor != NULL) {
        flySpeedCalcLockOn();
    } else {
        if (param_0 == 0) {
            flySpeedCalc(pntAngleY, 1);
        } else {
            flySpeedCalc(pntAngleY, 2);
        }
    }

    speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
    speed.y = -mFlySpeed * cM_ssin(current.angle.x);

    cStack_38.z = checkNextPath(nextPntPos);
    if (cStack_38.z < 0.0f) {
        if (abs((s16)(pntAngleY - cLib_targetAngleY(&prevPntPos, &current.pos))) < 0x4000) {
            mPathCurrentPointNo += mPathStep;
            if (checkFlySceneChange()) {
                return true;
            }
        }
    }

    return false;
}

void daKago_c::executeAttack() {
    mIsAttack = TRUE;

    if (mType == TYPE_NORMAL) {
        dComIfGp_setAStatusForce(BUTTON_STATUS_QUIT, 0);
    } else {
        dComIfGp_setAStatusForce(BUTTON_STATUS_UNK_39, 0);
    }

    checkHeight();

    mIsFlying = true;

    if (field_0x6e6 != 0) {
        setSibukiEffect();
        setDashSibukiEffect();
    }

    switch (mMode) {
    case 0:
        mDashCooldownTime = l_HIO.mDashCooldownTime;
        mDashTime = l_HIO.mDashTime;

        mMode = 1;

        setDashBlurEffect(0);
        setBck(dRes_ID_E_YC_BCK_YC_FLY_DASH_WL_e, 0, 5.0f, l_HIO.mDashTimeMultiplier);

        dComIfGp_getVibration().StartShock(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        mpLockActor = NULL;
        if (dComIfGp_getAttention()->LockonTruth()) {
            fopAc_ac_c* ym = dComIfGp_getAttention()->LockonTarget(0);
            if (fopAcM_GetName(ym) != PROC_E_YM) {
                break;
            }

            mpLockActor = ym;
            ((daE_YM_c*)ym)->setLockByCargo();
        }
        break;
    case 1:
        setDashEffect();

        if (calcAttackMove(0) == 0) {
            cLib_chaseF(&mFlySpeed, 30.0f, l_HIO.mDashTimeMultiplier * 3.0f);
            if (mAnm_p->checkFrame(9.0f)) {
                if (mType == TYPE_TWILIGHT) {
                    mSound.startCreatureSound(Z2SE_EN_YC_DASH, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_EN_KC_DASH, 0, -1);
                }
            }

            if (mAnm_p->checkFrame(12.0f)) {
                mMode = 2;
                field_0x728 = 20;

                dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartQuake(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

                mDashTime = l_HIO.mDashTime - 35.0f;

                field_0x6e6 = 0;
            }
        }
        break;
    case 2:
        setDashEffect();

        if (mAnm_p->checkFrame(23.0f)) {
            mSph.OnAtSetBit();
        }

        if (calcAttackMove(1) == 0) {
            cLib_chaseF(&mFlySpeed, 100.0f, 5.0f);

            if (field_0x728 == 0) {
                mMode = 3;
            }

            if (field_0x728 < 10 && checkWallHitFall(0)) {
                dComIfGp_getVibration().StopQuake(0x1f);
                mpLockActor = NULL;
            } else {
                mDashTime--;
                if (mDashTime < 1) {
                    mDashTime = 1;
                }
            }
        }
        break;
    case 3:
        setDashEffect();

        if (calcAttackMove(1) == 0) {
            cLib_chaseF(&mFlySpeed, 100.0f, 3.0f);
            if (checkWallHitFall(0)) {
                dComIfGp_getVibration().StopQuake(0x1f);

                mpLockActor = NULL;
            } else {
                mDashTime--;
                if (mDashTime == 0) {
                    mMode = 4;
                    dComIfGp_getVibration().StopQuake(0x1f);
                }
            }
        }
        break;
    case 4:
        setDashEffect();
    case 5:
        if (calcAttackMove(0) == 0) {
            cLib_chaseF(&mFlySpeed, l_HIO.mFlightSpeed, 5.0f);
            if (mFlySpeed <= l_HIO.mFlightSpeed) {
                mpLockActor = NULL;

                mSph.OffAtSetBit();

                setActionMode(ACTION_FLY_e, 2);

                field_0x6c8 = 0;
                field_0x6c4 = 8000.0f;
                field_0x710 = field_0x712 = 0;
            }
        }
        break;
    }
}

void daKago_c::calcCircleCamera(int param_0) {
    cXyz targetPos;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::transM(0.0f, 0.0f, 1500.0f);
    mDoMtx_stack_c::multVecZero(&targetPos);

    if (param_0 == 0) {
        mDemoCamCenter = targetPos;
    } else if (param_0 == 1) {
        cLib_addCalcPos(&mDemoCamCenter, targetPos, 0.1f, mFlySpeed + 100.0f,
                        10.0f);
    } else {
        cLib_addCalcPos(&mDemoCamCenter, targetPos, 0.5f, mFlySpeed + 30.0f,
                        10.0f);
    }

    mDoMtx_stack_c::transM(0.0f, 0.0f, -1000.0f);
    mDoMtx_stack_c::multVecZero(&targetPos);

    if (param_0 == 0) {
        mDemoCamEye = targetPos;
    } else if (param_0 == 1) {
        cLib_addCalcPos(&mDemoCamEye, targetPos, 0.1f, mFlySpeed + 100.0f,
                        10.0f);
    } else {
        cLib_addCalcPos(&mDemoCamEye, targetPos, 0.5f, mFlySpeed + 30.0f, 10.0f);
    }

    mDemoCamFovy = 60.0f;
    dCam_getBody()->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
}

void daKago_c::executeEvent() {
    dCamera_c* camera = dCam_getBody();
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (mSceneType == SCENE_TYPE_LAKE_HYLIA && dComIfG_play_c::getLayerNo(0) == 13 && dComIfGs_isSaveSwitch(16)) {
        dComIfGs_onSaveSwitch(26);
        setActionMode(ACTION_WAIT_e, 0);
    } else {
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        } else {
            camera->Stop();
            camera->SetTrimSize(3);
            mDemoMode = 0;
            field_0x74c = 0;
            setActionMode(ACTION_PERCH_e, 0);
            field_0x6dc = 1;
            field_0x6de = 0xff;
            dComIfGs_onSaveSwitch(8);
        }
    }
}

void daKago_c::initPerchDemo() {
    s16 angleY = daPy_getPlayerActorClass()->shape_angle.y;
    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;

    cXyz midnaPos;
    fopAc_ac_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    if (midna != NULL) {
        midnaPos = midna->current.pos;
    }

    switch (mDemoMode) {
    case 0: {
        Z2GetAudioMgr()->setDemoName(mDemoName);

        field_0x771 = 0;
        current.pos = dPath_GetPnt(mpPath2, field_0x771)->m_position;
        field_0x771++;
        if (field_0x771 >= mpPath2->m_num) {
            field_0x771 = 0;
        }

        cXyz cStack_48 = (Vec)dPath_GetPnt(mpPath2, field_0x771)->m_position;
        s16 sVar5 = cLib_targetAngleY(&current.pos, &cStack_48);
        int local_80 = 2;
        if (local_80 >= (int)mpPath2->m_num) {
            local_80 = mpPath2->m_num - 1;
        }

        cXyz cStack_54 = (Vec)dPath_GetPnt(mpPath2, local_80)->m_position;
        s16 sp08 = cLib_targetAngleY(&current.pos, &cStack_48);
        if ((s16)(sVar5 - sp08) < 0) {
            sVar5 = sVar5 - 0x1000;
        } else {
            sVar5 = sVar5 + 0x1000;
        }
        shape_angle.y = current.angle.y = sVar5;
        shape_angle.x = shape_angle.z = 0;

        speed.y = 0.0f;
        speedF = 20.0f;

        field_0x72c = 180;
        field_0x728 = field_0x72c + 90;
        field_0x6e8 = 0;

        if (mSceneType == SCENE_TYPE_LAKE_HYLIA && dComIfG_play_c::getLayerNo(0) == 13 && playerPos.x > -90000.0f) {
            field_0x6e8 = 1;
        }

        calcCircleCamera(0);
        field_0x6e4 = 0;
        break;
    }
    case 1: {
        field_0x728 = 90;
        mFlySpeed = 20.0f;
        break;
    }
    case 2: {
        if (mSceneType != SCENE_TYPE_LAKE_HYLIA) {
            mPathCurrentPointNo = searchNearPassPoint();
        }

        field_0x758 = 1;

        if (mPathStep < 0) {
            angleY = angleY + 0x8000;

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&playerPos, angleY, 0);

            field_0x758 = -1;
        }

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(angleY);
        mDoMtx_stack_c::transM(0.0f, 1000.0f, -1000.0f);
        mDoMtx_stack_c::multVecZero(&current.pos);

        setBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_e, 2, 0.0f, 1.0f);

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(angleY);
        mDoMtx_stack_c::transM(0.0f, 300.0f, -300.0f);
        mDoMtx_stack_c::multVecZero(&field_0x6a4);
        current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
        current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);

        mFlySpeed = 20.0f;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;
        shape_angle.z = 0;

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(angleY);
        mDoMtx_stack_c::transM(-100.0f, 100.0f, -300.0f);
        mDoMtx_stack_c::multVecZero(&mDemoCamCenter);
        mDoMtx_stack_c::transM(100.0f, -100.0f, 300.0f);
        mDoMtx_stack_c::transM(100.0f, 30.0f, 300.0f);
        mDoMtx_stack_c::multVecZero(&mDemoCamEye);

        mDemoCamFovy = 70.0f;

        dCam_getBody()->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        field_0x728 = 10;
        break;
    }
    case 3: {
        setBck(dRes_ID_E_YC_BCK_YC_FLY_e, 2, 10.0f, 1.0f);

        setMidnaTagPos();

        speedF = speed.y = 0.0f;

        field_0x728 = 30;
        break;
    }
    case 4: {
        setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);

        if (mSceneType == SCENE_TYPE_LAKE_HYLIA) {
            field_0x6a4.set(-90000.0f, -16000.0f, 40000.0f);
        } else {
            field_0x6a4 = dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
        }

        setMidnaRideOn();
        setPlayerRideOn();

        dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        mMode = 1;
        field_0x728 = 30;
        break;
    }
    }

    field_0x74c = 1;
    mMode = 0;
}

bool daKago_c::executePerchDemo() {
    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;
    s16 playerYaw = daPy_getPlayerActorClass()->shape_angle.y;
    cXyz unkXyz1;
    cXyz midnaPos;

    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    s16 midnaYaw;
    if (midna != NULL) {
        midnaPos = midna->current.pos;
        midnaYaw = midna->shape_angle.y;
    }

    field_0x750++;

    switch (mDemoMode) {
    case 0: {
        calcCircleCamera(1);
        if (field_0x750 & 0x20) {
            cLib_chaseF(&field_0x6fc, 3.0f, 0.3f);
        } else {
            cLib_chaseF(&field_0x6fc, -3.0f, 0.3f);
        }

        current.pos.y = current.pos.y + field_0x6fc;

        if (field_0x72c != 0) {
            cXyz cStack_64 = (Vec)dPath_GetPnt(mpPath2, field_0x771)->m_position;
            cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &cStack_64), 0x20, 0x100, 0x40);
            if (current.pos.abs(cStack_64) < 200.0f) {
                field_0x771++;
                if (field_0x771 >= mpPath2->m_num) {
                    field_0x771 = 0;
                }
            }
            cLib_addCalcAngleS(&shape_angle.y, current.angle.y, 0x20, 0x100, 0x40);
        } else {
            if (field_0x754 == 0) {
                if (mType == TYPE_NORMAL) {
                    mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
                } else {
                    mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
                }
                field_0x754 = 1;
            }

            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &playerPos), 8, 0x200, 0x40);
            cLib_addCalcAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &playerPos), 4, 0x200, 0x10);
            cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &playerPos), 8, 0x100, 0x10);
        }

        if (field_0x728 == 0) {
            mDemoMode = 1;
            return true;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 0);
        break;
    }
    case 1: {
        calcCircleCamera(2);

        cLib_addCalcAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &playerPos), 4, 0x400, 0x40);
        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &playerPos), 4, 0x200, 0x40);

        current.angle.y = shape_angle.y;
        current.angle.x = shape_angle.x;

        cLib_chaseF(&mFlySpeed, 60.0f, 1.0f);

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (current.pos.abs(playerPos) < 2000.0f) {
            mDemoMode = 2;
            return true;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 0);
        break;
    }
    case 2: {
        if (field_0x6e4 == 0) {
            if (mSceneType == SCENE_TYPE_LAKE_HYLIA) {
                dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 4);
            } else if (mSceneType == SCENE_TYPE_BOARD_HOUSE) {
                dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 5);
            }
        }

        if (mMode == 0) {
            current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
            current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);

            speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
            speed.y = -mFlySpeed * cM_ssin(current.angle.x);

            if (field_0x728 != 0) {
                shape_angle.x = current.angle.x;
                if (field_0x728 == 1) {
                    if (mType == TYPE_NORMAL) {
                        mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
                    } else {
                        mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
                    }
                }
            } else {
                cLib_addCalcAngleS(&shape_angle.x, 0, 4, 0x100, 0x80);
            }

            shape_angle.y = current.angle.y;
            if (field_0x6a4.abs(current.pos) < 300.0f) {
                mMode = 1;

                setBck(dRes_ID_E_YC_BCK_YC_HOVERING_e, 2, 10.0f, 1.0f);

                field_0x728 = 60;

                mDoMtx_stack_c::transS(midnaPos);
                mDoMtx_stack_c::YrotM(playerYaw);
                mDoMtx_stack_c::transM(0.0f, 300.0f, -100.0f);
                mDoMtx_stack_c::multVecZero(&field_0x6a4);
            }
        } else {
            cLib_chaseF(&mFlySpeed, 0.0f, 1.0f);
            cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 4, 0x400, 0x100);
            cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x100, 0x80);
            cLib_addCalcAngleS(&shape_angle.x, 0, 4, 0x100, 0x80);

            shape_angle.y = current.angle.y;
            speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
            speed.y = -mFlySpeed * cM_ssin(current.angle.x);
            if (!mFlySpeed && field_0x728 == 0) {
                mDemoMode = 3;
                return true;
            }
        }

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(0.0f, 400.0f, -300.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 5.0f, 5.0f);

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(100.0f, 30.0f, 300.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 20.0f, 10.0f);

        dCam_getBody()->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        break;
    }
    case 3: {
        if (field_0x6e4 == 0) {
            if (mSceneType == SCENE_TYPE_LAKE_HYLIA) {
                dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 4);
            } else if (mSceneType == SCENE_TYPE_BOARD_HOUSE) {
                dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 5);
            }
        }

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 20.0f, 10.0f);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(300.0f, 50.0f, 500.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 20.0f, 10.0f);

        cLib_chaseF(&mDemoCamFovy, 70.0f, 1.0f);

        dCam_getBody()->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x400, 0x100);

        shape_angle.x = current.angle.x;

        if (field_0x728 == 0) {
            mDemoMode = 4;
            return true;
        }
        break;
    }
    case 4: {
        if (field_0x6e4 == 0) {
            if (mSceneType == SCENE_TYPE_LAKE_HYLIA) {
                dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 4);
            } else if (mSceneType == SCENE_TYPE_BOARD_HOUSE) {
                dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 6);
            }
        }

        if (PerchDemoAwayForward()) {
            return true;
        }
        break;
    }
    case 5: {
        f32 maxStep = field_0x6cc * 50.0f;
        f32 minStep = field_0x6cc * 10.0f;
        cLib_chaseF(&field_0x6cc, 0.0f, 0.05f);
        cLib_addCalcPos(&mDemoCamCenter, midnaPos, 0.5f, maxStep, minStep);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(0.0f, 50.0f, -800.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, maxStep, minStep);
        cLib_chaseF(&mDemoCamFovy, 70.0f, 1.0f);
        dCam_getBody()->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        break;
    }
    case 6:
        break;
    }

    return false;
}

bool daKago_c::PerchDemoAwayForward() {
    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;
    s16 playerYaw = daPy_getPlayerActorClass()->shape_angle.y;

    dCamera_c* camera = dCam_getBody();

    cXyz unkXyz1;
    cXyz midnaPos;

    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    s16 midnaYaw;
    if (midna != NULL) {
        midnaPos = midna->current.pos;
        midnaYaw = midna->shape_angle.y;
    }

    switch (mMode) {
    case 0:
    case 1: {
        f32 unusedFloat1 = 20.0f;
        f32 unusedFloat2 = 10.0f;

        mDemoCamCenter += speed;
        mDemoCamEye += speed;

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        if (mMode == 0) {
            mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
        } else {
            mDoMtx_stack_c::transM(0.0f, -200.0f, 0.0f);
        }
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 10.0f, 5.0f);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(playerYaw);
        if (mMode == 0) {
            mDoMtx_stack_c::transM(300.0f, 50.0f, 500.0f);
        } else {
            mDoMtx_stack_c::transM(500.0f, 50.0f, 0.0f);
        }
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 10.0f, 5.0f);

        cLib_chaseF(&mDemoCamFovy, 70.0f, 1.0f);

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        mGroundHeight = checkGroundHeight(current.pos, NULL);
        mRoofHeight = checkRoofHeight(current.pos);

        cLib_chaseF(&mFlySpeed, 30.0f, 1.0f);

        cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x200, 0x80);
        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);
        if (mMode == 0) {
            cLib_addCalcAngleS(&shape_angle.x, 0x2000, 8, 0x200, 0x40);
        } else {
            cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x200, 0x40);
        }

        if (field_0x728 == 20 && mMode == 0) {
            if (mType == TYPE_NORMAL) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 == 0) {
            if (mMode == 0) {
                mMode = 1;
                field_0x728 = 30;

                setPlayerRideOn();

                dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            } else if (mSceneType != SCENE_TYPE_LAKE_HYLIA) {
                field_0x728 = 60;
                mMode = 2;
            } else {
                field_0x728 = 90;
                mMode = 3;
                field_0x6cc = 1.0f;
            }
        }
        break;
    }
    case 2: {
        if (field_0x728 > 10) {
            mDemoCamCenter += speed;
            mDemoCamEye += speed;

            s16 targetYaw = cLib_targetAngleY(&mDemoCamEye, &mDemoCamCenter);
            if (abs((s16)(midnaYaw - targetYaw)) < 0x2000) {
                cLib_addCalcPos(&mDemoCamCenter, midnaPos, 0.5f, 5.0f, 10.0f);

                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(playerYaw);
                mDoMtx_stack_c::transM(0.0f, 50.0f, -800.0f);
                mDoMtx_stack_c::multVecZero(&unkXyz1);
                cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 20.0f, 10.0f);
            } else {
                cLib_addCalcPos(&mDemoCamCenter, midnaPos, 0.5f, 5.0f, 10.0f);

                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(playerYaw);
                mDoMtx_stack_c::transM(300.0f, 50.0f, -500.0f);
                mDoMtx_stack_c::multVecZero(&unkXyz1);
                cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 20.0f, 10.0f);
            }
        } else {
            cLib_addCalcPos(&mDemoCamCenter, midnaPos, 0.5f, 50.0f, 10.0f);

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(playerYaw);
            mDoMtx_stack_c::transM(0.0f, 50.0f, -800.0f);
            mDoMtx_stack_c::multVecZero(&unkXyz1);
            cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 50.0f, 10.0f);
        }

        cLib_chaseF(&mDemoCamFovy, 70.0f, 1.0f);
        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        mGroundHeight = checkGroundHeight(current.pos, NULL);
        mRoofHeight = checkRoofHeight(current.pos);

        cLib_chaseF(&mFlySpeed, l_HIO.mFlightSpeed, 2.0f);

        if (field_0x728 < 30) {
            cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x100, 0x40);
        } else {
            cLib_addCalcAngleS(&current.angle.x, -0x2000, 8, 0x100, 0x40);
        }

        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            if (mSceneType == SCENE_TYPE_BOARD_HOUSE) {
                if (setSceneChange(3)) {
                    mDemoMode = 5;
                    field_0x6cc = 1.0f;
                }
            } else {
                field_0x6dc = 0;

                setActionMode(ACTION_FLY_e, 0);

                camera->Reset(mDemoCamCenter, mDemoCamEye);
                camera->Start();
                camera->SetTrimSize(0);

                dComIfGp_event_reset();

                Z2GetAudioMgr()->setDemoName(NULL);
            }
        }
        break;
    }
    case 3: {
        cLib_chaseF(&field_0x6cc, 0.0f, 0.1f);

        mDemoCamCenter += speed;
        mDemoCamEye += speed * field_0x6cc;

        unkXyz1.set(playerPos.x, playerPos.y + 100.0f, playerPos.z);
        cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 10.0f, 3.0f);

        mDemoCamFovy = 70.0f;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        cLib_chaseF(&mFlySpeed, 30.0f, 1.0f);

        cLib_addCalcAngleS(&current.angle.x, -0x2000, 8, 0x100, 0x40);
        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            setActionMode(ACTION_PERCH2_e, 0);
            mDemoMode = 8;
            return true;
        }
        break;
    }
    }

    return false;
}

void daKago_c::executePerch() {
    if (field_0x74c == 0) {
        initPerchDemo();
    }

    if (executePerchDemo()) {
        field_0x74c = 0;
    }
}

void daKago_c::executeEvent2() {
    dCamera_c* camera = dCam_getBody();
    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
        eventInfo.onCondition(2);
    } else {
        camera->Stop();
        camera->SetTrimSize(3);
        mDemoMode = 0;
        field_0x74c = 0;
        setActionMode(ACTION_PERCH2_e, 0);
        field_0x6dc = 1;
        field_0x6de = 0xff;
    }
}

void daKago_c::initFirstDemo() {
    dCamera_c* camera = dCam_getBody();
    cXyz acStack_28;
    cXyz cStack_34;

    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 playerYaw = player->shape_angle.y;
    cXyz playerPos = player->current.pos;

    daMidna_c* midna = daPy_py_c::getMidnaActor();
    cXyz midnaPos;
    if (midna != NULL) {
        midnaPos = midna->current.pos;
    }

   s16 targetYaw;
    switch (mDemoMode) {
    case 0: {
        Z2GetAudioMgr()->setDemoName(mDemoName);
        mHeadRotZ = -0x800;
        player->changeOriginalDemo();
        player->changeDemoMode(daPy_demo_c::DEMO_UNK_23_e, 0, 0, 0);

        playerPos.set(-103292.0f, -23437.0f, 39925.0f);
        field_0x6a4.set(-60000.0f, -11000.0f, 28000.0f);
        shape_angle.y = current.angle.y = cLib_targetAngleY(&playerPos, &field_0x6a4);
        cStack_34.set(-300.0f, 400.0f, -1000.0f);
        cLib_offsetPos(&current.pos, &playerPos, shape_angle.y, &cStack_34);
        targetYaw = cLib_targetAngleY(&playerPos, &current.pos);

        player->setPlayerPosAndAngle(&playerPos, targetYaw, 0);
        if (midna != NULL) {
            midna->current.pos = playerPos;
            midnaPos = midna->current.pos;
        }

        field_0x680.set(0.0f, 0.0f, 0.0f);
        mDemoCamCenter = current.pos;
        field_0x6bc = 0x3000;
        field_0x6cc = 500.0f;
        field_0x6d0 = 400.0f;
        cStack_34.set(0.0f, field_0x6d0, field_0x6cc);
        cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x6bc, &cStack_34);
        mDemoCamFovy = 70.0f;
        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        field_0x728 = 160;
        setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);
        field_0x6e4 = 0;
        break;
    }
    case 1: {
        if (midna != NULL) {
            midna->current.pos = playerPos;
            midnaPos = midna->current.pos;
        }

        mDemoCamCenter = current.pos;
        field_0x6bc = 0x800;
        field_0x6cc = 700.0f;
        cStack_34.set(0.0f, 0.0f, field_0x6cc);
        cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x6bc, &cStack_34);
        mDemoCamFovy = 70.0f;
        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        field_0x728 = 80;
        break;
    }
    case 2: {
        field_0x728 = 60;
        mMsgFlow.init(this, 0x7d3, 0, NULL);
        break;
    }
    case 3: {
        mHeadRotZ = cLib_targetAngleX(&current.pos, &playerPos) / 4;
        targetYaw = cLib_targetAngleY(&current.pos, &playerPos) - shape_angle.y;
        mHeadRotY = targetYaw / 4;
        cStack_34.set(50.0f, 150.0f, -200.0f);
        cLib_offsetPos(&mDemoCamEye, &midnaPos, shape_angle.y, &cStack_34);
        cStack_34.set(50.0f, 0.0f, 50.0f);
        cLib_offsetPos(&mDemoCamCenter, &midnaPos, shape_angle.y, &cStack_34);
        mDemoCamFovy = 70.0f;
        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        player->setPlayerPosAndAngle(&playerPos, cLib_targetAngleY(&playerPos, &current.pos), 0);
        break;
    }
    case 4: {
        setMidnaRideOn();
        field_0x6cc = -500.0f;
        field_0x6bc = cLib_targetAngleY(&current.pos, &playerPos);
        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(field_0x6bc);
        mDoMtx_stack_c::transM(0.0f, 300.0f, field_0x6cc);
        mDoMtx_stack_c::multVecZero(&field_0x6a4);
        mFlySpeed = 0.0f;
        break;
    }
    case 5: {
        field_0x6bc = cLib_targetAngleY(&current.pos, &playerPos);
        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(field_0x6bc);
        mDoMtx_stack_c::transM(0.0f, 300.0f, 200.0f);
        mDoMtx_stack_c::multVecZero(&field_0x6a4);
        break;
    }
    case 6: {
        player->cancelOriginalDemo();
        setPlayerRideOn();
        dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        field_0x728 = 150;
        field_0x6cc = 1.0f;
        break;
    }
    case 7: {
        field_0x6e8 = 0;
    }
    case 8: {
        setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);

        if (field_0x6e8 != 0) {
            mAnm_p->setFrame(0.0f);
            field_0x6a4.set(-67000.0f, -13000.0f, 28000.0f);
            current.pos.set(-76600.0f, -15500.0f, 37340.0f);
            shape_angle.y = current.angle.y = -0x8000;
            shape_angle.x = current.angle.x = -0x1000;
            mFlySpeed = 50.0f;
            mDemoCamEye.set(-75232.0f, -14685.0f, 34417.0f);
            mDemoCamCenter.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            mDemoCamFovy = 70.0f;
            camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
            field_0x728 = 200;
        } else {
            field_0x6a4.set(-60000.0f, -11000.0f, 28000.0f);
            current.pos.set(-90000.0f, -16000.0f, 40000.0f);
            shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
            shape_angle.x = current.angle.x = -0x2000;
            mFlySpeed = 50.0f;
            cStack_34.set(-500.0f, 1000.0f, 5000.0f);
            cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y, &cStack_34);
            mDemoCamCenter.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            mDemoCamFovy = 70.0f;
            camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
            field_0x728 = 200;
        }
        break;
    }
    }

    field_0x74c = 1;
    mMode = 0;
}

bool daKago_c::executeFirstDemo() {
    dCamera_c* camera = dCam_getBody();

    cXyz unkXyz1;
    cXyz offset;

    cXyz midnaPos;
    daPy_py_c* player = daPy_getPlayerActorClass();

    daMidna_c* midna = daPy_py_c::getMidnaActor();
    if (midna != NULL) {
        midnaPos = midna->current.pos;
    }

    s16 playerYaw = player->shape_angle.y;
    cXyz playerPos = player->current.pos;

    int unkFlag1;
    int mode = mDemoMode;  // fakematch
    switch (mode) {
    case 0:
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 2);

        cLib_chaseAngleS(&field_0x6bc, 0x1400, 0x40);
        cLib_chaseF(&field_0x6cc, 1250.0f, 6.7f);
        cLib_chaseF(&field_0x6d0, -400.0f, 7.1f);

        offset.set(0.0f, field_0x6d0, field_0x6cc);
        cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x6bc, &offset);

        unkXyz1.set(0.0f, -200.0f, 400.0f);
        cLib_chasePos(&field_0x680, unkXyz1, 4.0f);
        cLib_offsetPos(&mDemoCamCenter, &current.pos, shape_angle.y, &field_0x680);

        mDemoCamFovy = 70.0f;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        if (field_0x728 < 10) {
            setMidnaTagPos();
            if (field_0x728 == 0) {
                mDemoMode = 1;
                return 1;
            }
        }

        break;
    case 1:
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 2);

        cLib_chaseAngleS(&field_0x6bc, 0x1000, 0x20);

        offset.set(0.0f, 0.0f, field_0x6cc);
        cLib_offsetPos(&mDemoCamEye, &current.pos, shape_angle.y + field_0x6bc, &offset);

        if (field_0x728 < 70) {
            cLib_addCalcPos(&mDemoCamCenter, midnaPos, 0.5f, 10.0f, 10.0f);
        } else {
            mDemoCamCenter = current.pos;
        }

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        setMidnaTagPos();

        if (field_0x728 == 30) {
            setBck(dRes_ID_E_YC_BCK_YC_HANGED_WAIT_e, 2, 10.0f, 1.0f);
        }

        if (field_0x728 > 30) {
            cLib_chaseF(&field_0x6cc, 350.0f, 4.0f);
            cLib_addCalcAngleS(&mHeadRotZ, -0x800, 8, 0x100, 0x10);
        } else {
            cLib_chaseF(&field_0x6cc, 350.0f, 6.0f);
            cLib_addCalcAngleS(&mHeadRotZ, 0x400, 8, 0x200, 0x10);

            if (field_0x728 == 30) {
                setMidnaRideOn();
            }

            if (field_0x728 == 20) {
                if (mType == TYPE_NORMAL) {
                    mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
                } else {
                    mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
                }
            }

            if (field_0x728 == 0) {
                mDemoMode = 2;

                return true;
            }
        }
        break;
    case 2:
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 3);

        setMidnaTagPos();
        cLib_addCalcPos(&mDemoCamCenter, midnaPos, 0.5f, 10.0f, 10.0f);
        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        mMsgFlow.doFlow(this, NULL, 0);

        if (field_0x728 != 0) {
            if (field_0x728 == 1) {
                setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 20.0f, 1.0f);
            }

            if (mMsgFlow.getNowMsgNo() == 0x1774) {
                setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 20.0f, 1.0f);
                field_0x728 = 0;
            }
        }

        if (mMsgFlow.getNowMsgNo() == 0x1775) {
            mDemoMode = 3;
            return true;
        }

        break;
    case 3: {
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 3);

        setMidnaTagPos();
        cLib_addCalcAngleS(
            &mHeadRotZ, cLib_targetAngleX(&current.pos, &playerPos) / 4,
            8, 0x100, 0x10);

        // adding braces to the switch case for this decl regresses debug match
        s16 targetYaw = cLib_targetAngleY(&current.pos, &playerPos) - shape_angle.y;
        cLib_addCalcAngleS(
            &mHeadRotY, targetYaw / 4,
            8, 0x100, 0x10);

        if (mMsgFlow.doFlow(this, NULL, 0)) {
            mDemoMode = 4;
            return true;
        }

        break;
    }
    case 4:
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 2);

        cLib_addCalcAngleS(&mHeadRotZ, 0, 8, 0x100, 0x10);
        cLib_addCalcAngleS(&mHeadRotY, 0, 8, 0x100, 0x10);

        cLib_chaseF(&field_0x6cc, -300.0f, 10.0f);

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(field_0x6bc);
        mDoMtx_stack_c::transM(0.0f, 300.0f, field_0x6cc);
        mDoMtx_stack_c::multVecZero(&field_0x6a4);

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 4, 0x800, 0x100);
        cLib_addCalcAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x6a4), 4, 0x1000, 0x100);
        shape_angle.y = current.angle.y;
        shape_angle.x = current.angle.x;

        cLib_chaseF(&mFlySpeed, 20.0f, 1.0f);

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);
        mDemoCamCenter += speed;
        mDemoCamEye += speed;

        offset.set(0.0f, 200.0f, -500.0f);
        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &offset);
        cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 10.0f, 3.0f);
        offset.set(0.0f, 0.0f, 0.0f);

        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &offset);
        cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 10.0f, 3.0f);

        mDemoCamFovy = 70.0f;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        if (field_0x6a4.abs(current.pos) < 100.0f) {
            mDemoMode = 5;
            return true;
        }

        break;
    case 5:
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 2);

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 4, 0x800, 0x100);
        cLib_addCalcAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x6a4), 4, 0x400, 0x100);
        shape_angle.y = current.angle.y;
        shape_angle.x = current.angle.x;

        cLib_chaseF(&mFlySpeed, 20.0f, 1.0f);
        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));

        speed.y = -mFlySpeed * cM_ssin(current.angle.x);
        mDemoCamCenter += speed;
        mDemoCamEye += speed;

        offset.set(400.0f, 50.0f, -300.0f);
        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &offset);
        cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 10.0f, 3.0f);

        offset.set(0.0f, 50.0f, 0.0f);
        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &offset);
        cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 10.0f, 3.0f);

        mDemoCamFovy = 70.0f;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);

        if (field_0x6a4.abs(current.pos) < 100.0f) {
            mDemoMode = 6;
            return true;
        }
        break;
    case 6:
        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 2);

        cLib_addCalcAngleS(&current.angle.x, -0x2000, 8, 0x100, 0x40);
        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (field_0x728 > 120) {
            mDemoCamCenter += speed;
            mDemoCamEye += speed;

            offset.set(400.0f, 100.0f, -300.0f);
            cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &offset);
            cLib_addCalcPos(&mDemoCamEye, unkXyz1, 0.5f, 10.0f, 3.0f);

            offset.set(0.0f, 100.0f, 0.0f);
            cLib_offsetPos(&unkXyz1, &playerPos, shape_angle.y, &offset);
            cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 10.0f, 3.0f);
        } else {
            cLib_chaseF(&field_0x6cc, 0.0f, 0.1f);

            mDemoCamCenter += speed;
            mDemoCamEye += speed * field_0x6cc;

            offset.set(0.0f, 100.0f, 0.0f);
            cLib_offsetPos(&unkXyz1, &playerPos, shape_angle.y, &offset);
            cLib_addCalcPos(&mDemoCamCenter, unkXyz1, 0.5f, 10.0f, 3.0f);
        }

        mDemoCamFovy = 70.0f;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        if (field_0x728 == 0) {
            mDemoMode = 7;
            return true;
        }
        break;
    case 7:
    case 8:
        unkFlag1 = 0;

        if (mode == 7 || field_0x6e8 == 0) {
            if (field_0x728 == 110) {
                unkFlag1 = 1;
            }
        } else if (mode == 8 && field_0x728 == 140) {
            unkFlag1 = 1;
        }

        if (unkFlag1 != 0) {
            if (mType == TYPE_NORMAL) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 != 0) {
            dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 1);
        } else {
            if (mSceneType == SCENE_TYPE_LAKE_HYLIA && setSceneChange(0)) {
                mDemoMode = 9;
            }
        }
    case 9:
        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 0x10, 0x100, 0x80);
        cLib_addCalcAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x6a4), 0x10, 0x200, 0x80);
        shape_angle.y = current.angle.y;
        shape_angle.x = current.angle.x;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        offset.set(0.0f, 100.0f, 0.0f);
        cLib_offsetPos(&mDemoCamCenter, &current.pos, shape_angle.y, &offset);

        mDemoCamFovy = 70.0f;

        camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
        break;
    }

    return false;
}

void daKago_c::executePerch2() {
    if (field_0x74c == 0) {
        initFirstDemo();
    }

    if (executeFirstDemo()) {
        field_0x74c = 0;
    }
}

void daKago_c::executeLandingLakeHairia() {
    dCamera_c* camera = dCam_getBody();
    cXyz unkXyz1;
    cXyz unkXyz2(-700.0f, 0.0f, -300.0f);

    switch (mMode) {
    case 0:
        current.pos.set(-74500.0f, -12775.0f, 31400.0f);

        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->Stop();
        camera->SetTrimSize(3);

        current.pos.set(-74500.0f, -12775.0f, 31400.0f);
        mDemoCamEye.set(-79460.0f, -13000.0f, 34200.0f);
        mDemoCamCenter = current.pos;

        mDemoCamFovy = 70.0f;

        shape_angle.x = current.angle.x = 0;
        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &mDemoCamEye);
        field_0x716 = shape_angle.y + 0x2000;

        field_0x6a4.set(-77615.0f, -18500.0f, 41400.0f);
        field_0x6a4 += unkXyz2;

        mMode = 1;
        field_0x712 = 0;
        field_0x728 = 150;
        mFlySpeed = 50.0f;

        break;
    case 1:
        Z2GetAudioMgr()->setDemoName(mDemoName);
        mMode = 2;
    case 2:
        if (field_0x728 == 140) {
            if (mType == TYPE_NORMAL) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        mDemoCamCenter = current.pos;

        cLib_chaseAngleS(&field_0x712, 0x80, 4);

        cLib_chaseAngleS(&shape_angle.y, field_0x716, field_0x712);
        current.angle.y = shape_angle.y;
        cLib_chaseAngleS(&shape_angle.x, 0x1000, 0x20);
        current.angle.x = shape_angle.x;

        mFlySpeed = 50.0f;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            mMode = 3;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 7);
        break;
    case 3:
        current.pos.set(-76600.0f, -15500.0f, 37340.0f);
        current.pos += unkXyz2;

        field_0x6a4.set(-77615.0f, -18500.0f, 41400.0f);
        field_0x6a4 += unkXyz2;

        mDemoCamEye.set(-76900.0f, -18550.0f, 41660.0f);
        mDemoCamEye += unkXyz2;

        mDemoCamCenter = current.pos;

        mDemoCamFovy = 70.0f;

        shape_angle.x = current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);
        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);

        mMode = 4;
        field_0x728 = 60;

        setBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e, 2, 10.0f, 1.0f);
    case 4:
        mDemoCamCenter = current.pos;
        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);
        if (field_0x728 == 0) {
            mMode = 5;
            field_0x728 = 40;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 7);
        break;
    case 5:
        mDemoCamCenter = current.pos;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, 0, 0x80);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 0) {
            setRideOff();

            mMode = 6;
            field_0x728 = 100;

            setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 2, 10.0f, 1.0f);
        } else {
            dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 7);
        }

        field_0x6cc = 50.0f;
        field_0x6d0 = 20.0f;
        break;
    case 6:
        unkXyz1.set(-77875.0f, -18287.0f, 42000.0f);
        unkXyz1 += unkXyz2;
        cLib_chasePos(&mDemoCamCenter, unkXyz1, field_0x6cc);
        if (mDemoCamCenter.abs(unkXyz1) < 500.0f) {
            cLib_chaseF(&field_0x6cc, 0.0f, 3.0f);
        }

        unkXyz1.set(-77275.0f, -18500.0f, 41090.0f);
        unkXyz1 += unkXyz2;
        cLib_chasePos(&mDemoCamEye, unkXyz1, field_0x6d0);

        if (mDemoCamEye.abs(unkXyz1) < 200.0f) {
            cLib_chaseF(&field_0x6d0, 0.0f, 1.0f);
        } else {
            cLib_chaseF(&field_0x6d0, 20.0f, 1.0f);
        }

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, -0x2000, 0x100);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 90) {
            if (mType == TYPE_NORMAL) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 == 0) {
            setActionMode(ACTION_WAIT_e, 0);

            camera->Reset();
            camera->Start();
            camera->SetTrimSize(0);

            dComIfGp_event_reset();

            Z2GetAudioMgr()->setDemoName(NULL);
            return;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 8);
    }

    camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
}

void daKago_c::executeLandingBoartHouse() {
    dCamera_c* camera = dCam_getBody();
    cXyz unkXyz1;

    switch (mMode) {
    case 0: {
        current.pos.set(5750.0f, 1600.0f, 6100.0f);

        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(2);
            return;
        }

        camera->Stop();
        camera->SetTrimSize(3);

        mMode = 10;
        break;
    }
    case 10: {
        Z2GetAudioMgr()->setDemoName(mDemoName);

        current.pos.set(5750.0f, 1600.0f, 6100.0f);
        setBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e, 0x02, 10.0f, 1.0f);
        mDemoCamEye.set(2900.0f, 300.0f, 500.0f);
        mDemoCamCenter = current.pos;
        mDemoCamFovy = 70.0f;
        field_0x6a4.set(3630.0f, 300.0f, 600.0f);

        shape_angle.x = current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);
        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);

        mMode = 1;
        field_0x712 = 0;
        field_0x728 = 80;
        mFlySpeed = 50.0f;
    }
    case 1: {
        if (field_0x728 == 70) {
            if (mType == TYPE_NORMAL) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        mDemoCamCenter = current.pos;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            mMode = 2;
            field_0x728 = 40;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 9);
        break;
    }
    case 2: {
        mDemoCamCenter = current.pos;

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, 0, 0x80);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 0) {
            setRideOff();
            mMode = 3;
            field_0x728 = 90;
        } else {
            dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 9);
        }

        field_0x6cc = 40.0f;
        field_0x6d0 = 15.0f;
        break;
    }
    case 3: {
        unkXyz1.set(3440.0f, 500.0f, 400.0f);
        cLib_chasePos(&mDemoCamCenter, unkXyz1, field_0x6cc);
        if (mDemoCamCenter.abs(unkXyz1) < 500.0f) {
            cLib_chaseF(&field_0x6cc, 0.0f, 3.0f);
        }

        unkXyz1.set(3584.0f, 270.0f, 1007.0f);
        cLib_chasePos(&mDemoCamEye, unkXyz1, field_0x6d0);
        if (mDemoCamEye.abs(unkXyz1) < 200.0f) {
            cLib_chaseF(&field_0x6d0, 0.0f, 1.0f);
        } else {
            cLib_chaseF(&field_0x6d0, 15.0f, 1.0f);
        }

        speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
        speed.y = -mFlySpeed * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, -0x2000, 0x100);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 80) {
            if (mType == TYPE_NORMAL) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 == 0) {
            setActionMode(ACTION_WAIT_e, 0);

            if (mType == TYPE_NORMAL) {
                field_0x728 = 120;
            }

            camera->Reset();
            camera->Start();
            camera->SetTrimSize(0);

            dComIfGp_event_reset();

            Z2GetAudioMgr()->setDemoName(NULL);
            return;
        }

        dComIfGp_getEvent()->setSkipProc(this, DemoSkipCallBack, 10);
        break;
    }
    }

    camera->Set(mDemoCamCenter, mDemoCamEye, mDemoCamFovy, 0);
}

void daKago_c::executeLanding() {
    if (mSceneType == SCENE_TYPE_LAKE_HYLIA) {
        executeLandingLakeHairia();
    } else if (mSceneType == SCENE_TYPE_BOARD_HOUSE) {
        executeLandingBoartHouse();
    } else {
        setRideOff();
        setActionMode(ACTION_WAIT_e, 0);
    }
}

void daKago_c::moveDemoFly() {
    if (mMode == 0) {
        cLib_chaseF(&mFlySpeed, l_HIO.mFlightSpeed, 2.0f);
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        } else {
            setPlayerRideOn();

            mDashCooldownTime = (int)l_HIO.mDashCooldownTime;
            mDashTime = (int)l_HIO.mDashTime;
            mMode = 1;
            setDashBlurEffect(0);
            setBck(dRes_ID_E_YC_BCK_YC_FLY_DASH_WL_e, 0, 5.0f, l_HIO.mDashTimeMultiplier);
            dComIfGp_getVibration().StartShock(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

            mpLockActor = NULL;

            mGroundHeight = checkGroundHeight(current.pos, NULL);
            mRoofHeight = checkRoofHeight(current.pos);

            if (mAction == ACTION_DEMO_FLY_e) {
                dComIfGs_onSaveSwitch(9);
                mMsgFlow.init(this, 0xbbf, 0, NULL);
            }
        }
    } else {
        checkHeight();

        mIsFlying = true;

        if (mAction == ACTION_DEMO_FLY_e) {
            mMsgFlow.doFlow(this, NULL, 0);
        }

        if (mMode < 6) {
            setDashEffect();
            mIsAttack = TRUE;
            dComIfGp_setAStatusForce(BUTTON_STATUS_UNK_39, 0);
        }

        switch (mMode) {
        case 1: {
            Z2GetAudioMgr()->setDemoName(mDemoName);
            mMode = 2;
        }
        case 2: {
            cLib_chaseF(&mFlySpeed, 30.0f, l_HIO.mDashTimeMultiplier * 3.0f);
            if (mAnm_p->checkFrame(9.0f)) {
                if (mType == TYPE_TWILIGHT) {
                    mSound.startCreatureSound(Z2SE_EN_YC_DASH, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_EN_KC_DASH, 0, -1);
                }
            }

            if (mAnm_p->checkFrame(12.0f)) {
                mMode = 3;
                field_0x728 = 20;

                dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartQuake(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

                field_0x6e6 = 0;
            }
            break;
        }
        case 3:
        case 4: {
            cLib_chaseF(&mFlySpeed, 100.0f, 5.0f);
            if (field_0x728 == 0) {
                if (mMode == 3) {
                    mMode = 4;
                    field_0x728 = 15;
                } else {
                    mMode = 5;
                    dComIfGp_getVibration().StopQuake(0x1f);
                }
            }
            break;
        }
        case 5: {
            cLib_chaseF(&mFlySpeed, l_HIO.mFlightSpeed, 5.0f);
            if (mFlySpeed <= l_HIO.mFlightSpeed) {
                field_0x6c8 = 0;
                field_0x6c4 = 8000.0f;
                field_0x710 = field_0x712 = 0;

                #if VERSION == VERSION_WII_USA_R0
                field_0x72c = 30;
                #else
                if (dComIfGp_getStartStagePoint() == 0) {
                    field_0x72c = 30;
                } else {
                    field_0x72c = 60 + nREG_S(1);
                }
                #endif

                shape_angle.y = current.angle.y;

                mGroundHeight = checkGroundHeight(current.pos, NULL);
                mRoofHeight = checkRoofHeight(current.pos);

                setFlyAnime();

                field_0x710 = field_0x712 = 0;
                mMode = 6;
            }
            break;
        }
        case 6: {
            if (field_0x728 == 0) {
                setFlyAnime();
            }

            f32 fVar11 = 0.0f;
            if (cM_ssin(current.angle.x) > 0.0f) {
                fVar11 = l_HIO.mFlightSpeed + l_HIO.mDescentRateIncrement * cM_ssin(current.angle.x);
            } else {
                fVar11 = l_HIO.mFlightSpeed + l_HIO.mAscentRateDecel * cM_ssin(current.angle.x);
            }

            if (std::abs(mFlySpeed - fVar11) > 10.0f) {
                cLib_chaseF(&mFlySpeed, fVar11, 2.0f);
            } else {
                cLib_chaseF(&mFlySpeed, fVar11, 1.0f);
            }

            if (field_0x72c == 0) {
                setActionMode(ACTION_FLY_e, 0);
                dComIfGp_event_reset();
                Z2GetAudioMgr()->setDemoName(NULL);
                return;
            }
            break;
        }
        }
    }

    cXyz unkXyz1 = (Vec)dPath_GetPnt(mpPath1, mPathCurrentPointNo)->m_position;
    cXyz unkXyz2 = (Vec)dPath_GetPnt(mpPath1, mPathCurrentPointNo - mPathStep)->m_position;
    s16 targetYaw = field_0x714 = cLib_targetAngleY(&unkXyz2, &unkXyz1);

    if (dComIfGp_getStartStagePoint() == 0) {
        cLib_addCalcAngleS(&current.angle.x, cLib_targetAngleX(&unkXyz2, &unkXyz1), 8, 0x100,
                           0x10);
    } else {
        cLib_addCalcAngleS(&current.angle.x, 0x1400 + nREG_S(0), 8, 0x100, 0x10);
    }

    shape_angle.x = current.angle.x;
    cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&unkXyz2, &unkXyz1), 8, 0x40, 0x10);
    shape_angle.y = current.angle.y;

    speedF = mFlySpeed * std::abs(cM_scos(current.angle.x));
    speed.y = -mFlySpeed * cM_ssin(current.angle.x);

    f32 nextPath = checkNextPath(unkXyz1);
    if (nextPath < 0.0f) {
        if (abs((s16)(targetYaw - cLib_targetAngleY(&unkXyz2, &current.pos))) < 0x4000) {
            mPathCurrentPointNo += mPathStep;
            if (checkFlySceneChange()) {
                return;
            }
        }
    }
}

void daKago_c::setFlyEffect() {
    if (mType == TYPE_NORMAL) {
        return;
    }

    J3DModel* model = mAnm_p->getModel();
    JPABaseEmitter* emitter;

    field_0xb30 = dComIfGp_particle_set(field_0xb30, dPa_RM(ID_ZF_S_YCFLY_HANDLR2), &current.pos, &tevStr);
    emitter = dComIfGp_particle_getEmitter(field_0xb30);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(model->getAnmMtx(YC_JNT_HANDL_2_e));
    }

    field_0xb34 = dComIfGp_particle_set(field_0xb34, dPa_RM(ID_ZF_S_YCFLY_HANDLR2), &current.pos, &tevStr);
    emitter = dComIfGp_particle_getEmitter(field_0xb34);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(model->getAnmMtx(YC_JNT_HANDR_2_e));
    }
}

void daKago_c::setDashEffect() {
    J3DModel* model = mAnm_p->getModel();
    JPABaseEmitter* emitter;

    if (mType == TYPE_TWILIGHT) {
        field_0xb38 = dComIfGp_particle_set(field_0xb38, dPa_RM(ID_ZF_S_YCDASH_HANDL2), &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(field_0xb38);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(YC_JNT_HANDL_2_e));
        }

        field_0xb3c = dComIfGp_particle_set(field_0xb3c, dPa_RM(ID_ZF_S_YCDASH_HANDR2), &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(field_0xb3c);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(YC_JNT_HANDR_2_e));
        }
    }

    setDashBlurEffect(1);
}

void daKago_c::setDamageEffect() {
    J3DModel* model = mAnm_p->getModel();
    JPABaseEmitter* emitter;

    if (mType != TYPE_NORMAL) {
        field_0xb38 = dComIfGp_particle_set(field_0xb38, dPa_RM(ID_ZF_S_YCDAMAGE00_HANDLR2), &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(field_0xb38);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(YC_JNT_HANDL_2_e));
        }

        field_0xb3c = dComIfGp_particle_set(field_0xb3c, dPa_RM(ID_ZF_S_YCDAMAGE00_HANDLR2), &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(field_0xb3c);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(YC_JNT_HANDR_2_e));
        }
    }
}

void daKago_c::setDashBlurEffect(int param_0) {
    UNUSED(param_0);

    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz cameraEye = dCam_getBody()->Eye();
    cXyz cameraEyeOffset;
    cXyz local_78;
    csXyz cStack_8c;
    csXyz local_94;

    s16 sVar4 = shape_angle.y - fopCamM_GetAngleY(camera);
    f32 fVar6 = std::abs(cM_ssin(sVar4)) * l_HIO.mZOffsetHori + cM_scos(sVar4) * l_HIO.mZOffset;
    f32 fVar7 = fVar6 * cM_scos(fopCamM_GetAngleX(camera));

    local_78.y = fVar6 * cM_ssin(-fopCamM_GetAngleX(camera));
    local_78.x = fVar7 * cM_ssin(fopCamM_GetAngleY(camera));
    local_78.z = fVar7 * cM_scos(fopCamM_GetAngleY(camera));

    cameraEyeOffset = cameraEye + local_78;

    local_94.set((s16)cLib_targetAngleX(&mPrevCamEye, &cameraEye), (s16)cLib_targetAngleY(&mPrevCamEye, &cameraEye), 0);
    if (l_HIO.mAngleTrackingMode == 0) {
        field_0x6be.x = -local_94.x;
        field_0x6be.y = local_94.y;
        field_0x6be.z = 0;
    } else {
        field_0x6be.x = shape_angle.x * (0.5f + XREG_F(19));
        field_0x6be.y = shape_angle.y;
        field_0x6be.z = 0;
    }

    field_0xb40 = dComIfGp_particle_set(field_0xb40, dPa_RM(ID_ZF_S_YCSPEED), &cameraEyeOffset, &tevStr, &field_0x6be, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
}

void daKago_c::setWallHitEffect(cXyz i_pos, int i_effType) {
    // might be a 2D array?
    static u16 kago_wall_hit_id[6] = {
        dPa_RM(ID_ZF_S_YCHITA),
        dPa_RM(ID_ZF_S_YCHITA_1),
        dPa_RM(ID_ZF_S_YCHITA_2),

        dPa_RM(ID_ZF_S_YCHITB),
        dPa_RM(ID_ZF_S_YCHITB_1),
        dPa_RM(ID_ZF_S_YCHITB_2),
    };

    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(kago_wall_hit_id[i + i_effType * 3], &i_pos, &tevStr, NULL, NULL);
    }
}

void daKago_c::setSibukiEffect() {
    static u16 kago_wave_id[3] = {
        dPa_RM(ID_ZF_S_YCWAVE00_WAVE),
        dPa_RM(ID_ZF_S_YCWAVE01_SPLASH),
        dPa_RM(ID_ZF_S_YCWAVE02_DROP),
    };

    cXyz effpos(current.pos.x, field_0x70c + l_HIO.mYOffsetFromWaterSurface, current.pos.z);
    csXyz effrot(0, shape_angle.y, 0);
    for (int i = 0; i < 3; i++) {
        mSibukiEmitterIDs[i] =
            dComIfGp_particle_set(mSibukiEmitterIDs[i], kago_wave_id[i], &effpos, &tevStr, &effrot,
                                  NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    }

    if (mType == TYPE_TWILIGHT) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YC_SPLASH, 0, -1);
    } else {
        mSound.startCreatureSoundLevel(Z2SE_EN_KC_SPLASH, 0, -1);
    }
}

void daKago_c::setDashSibukiEffect() {
    cXyz effpos(current.pos.x, field_0x70c + l_HIO.mYOffsetFromWaterSurface, current.pos.z);
    csXyz effrot(0, shape_angle.y, 0);
    mDashSibukiEmitterID = dComIfGp_particle_set(mDashSibukiEmitterID, dPa_RM(ID_ZF_S_YCWAVE03_SPLASH), &effpos, &tevStr, &effrot, NULL,
                                                 0xff, NULL, -1, NULL, NULL, NULL);
}

void daKago_c::setWaterFallEffect() {
    static u16 kago_wave_id[2] = {
        dPa_RM(ID_ZF_S_YCWAVE01_SPLASH),
        dPa_RM(ID_ZF_S_YCWAVE02_DROP),
    };

    cXyz effpos(current.pos.x, current.pos.y, current.pos.z);
    csXyz effrot(0, shape_angle.y, 0);
    for (int i = 0; i < 2; i++) {
        mSibukiEmitterIDs[i] =
            dComIfGp_particle_set(mSibukiEmitterIDs[i], kago_wave_id[i], &effpos, &tevStr, &effrot,
                                  NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    }

    if (mType == TYPE_TWILIGHT) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YC_SPLASH, 0, -1);
    } else {
        mSound.startCreatureSoundLevel(Z2SE_EN_KC_SPLASH, 0, -1);
    }
}

static void* s_waterfall(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Tag_WaterFall) {
        if (!fpcM_IsCreating(fopAcM_GetID(i_actor))) {
            if (((daTagWaterFall_c*)i_actor)->checkHitWaterFall(((fopAc_ac_c*)i_data)->current.pos))
            {
                ((daKago_c*)i_data)->onWaterFall();
                return i_actor;
            }
        }
    }

    return NULL;
}

void daKago_c::setFlySound() {
    BOOL isPlayWingSound;

    if (field_0x6de != 0) {
        isPlayWingSound = FALSE;
        if (checkBck(dRes_ID_E_YC_BCK_YC_FLY_e) || checkBck(dRes_ID_E_YC_BCK_YC_FLY_WL_e)) {
            if (mType == TYPE_TWILIGHT) {
                if (mAnm_p->checkFrame(27.0f)) {
                    isPlayWingSound = TRUE;
                }
            } else {
                if (mAnm_p->checkFrame(29.0f)) {
                    isPlayWingSound = TRUE;
                }
            }
        } else if (checkBck(dRes_ID_E_YC_BCK_YC_HOVERING_e)) {
            if (mAnm_p->checkFrame(10.0f)) {
                isPlayWingSound = TRUE;
            }
        } else if (checkBck(dRes_ID_E_YC_BCK_YC_CATCH_START_e)) {
            if (mAnm_p->checkFrame(26.0f)) {
                isPlayWingSound = TRUE;
            }
        } else if (checkBck(dRes_ID_E_YC_BCK_YC_CATCH_MIDDLE_e)) {
            if (mAnm_p->checkFrame(10.0f)) {
                isPlayWingSound = TRUE;
            }
        } else if (checkBck(dRes_ID_E_YC_BCK_YC_CATCH_END_e)) {
            if (mAnm_p->checkFrame(10.0f) || mAnm_p->checkFrame(35.0f)) {
                isPlayWingSound = TRUE;
            }
        } else if (checkBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_e) || checkBck(dRes_ID_E_YC_BCK_YC_FLY_GLIDE_WL_e)) {
            if (mType == TYPE_TWILIGHT) {
                mSound.startCreatureSoundLevel(Z2SE_EN_YC_GLIDE, 0, -1);
            } else {
                mSound.startCreatureSoundLevel(Z2SE_EN_KC_GLIDE, 0, -1);
            }
        }

        if (isPlayWingSound) {
            if (mType == TYPE_TWILIGHT) {
                mSound.startCreatureSound(Z2SE_EN_YC_WING, 0, -1);
            } else {
                mSound.startCreatureSound(Z2SE_EN_KC_WING, 0, -1);
            }
        }
    }
}

void daKago_c::action() {
    checkMoveHeight();
    checkSizeBg();
    setFlyEffect();

    mStickX = mDoCPd_c::getStickX3D(PAD_1);
    mStickY = mDoCPd_c::getStickY(PAD_1);

    u8 prevIsWaterfall = mIsWaterfall;
    mIsWaterfall = FALSE;
    fpcM_Search(s_waterfall, this);

    if (mIsWaterfall) {
        if (!prevIsWaterfall) {
            current.angle.x = 0x1000;
            dComIfGp_getVibration().StartQuake(3, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        }

        mStickX = mStickX * 0.5f;

        if (mStickY < 0.0f) {
            mStickY = mStickY * 0.5f;
        } else {
            mStickY = mStickY * 1.5f;
        }

        setWaterFallEffect();
    } else if (prevIsWaterfall) {
        dComIfGp_getVibration().StopQuake(0x1f);
    }

    mIsAttack = FALSE;

    switch (mAction) {
    case ACTION_FLY_e:
        executeFly();
        break;
    case ACTION_STAGGER_e:
        executeStagger();
        break;
    case ACTION_EVENT_e:
        executeEvent();
        break;
    case ACTION_PERCH_e:
        executePerch();
        break;
    case ACTION_WAIT_e:
        executeWait();
        break;
    case ACTION_ATTACK_e:
        executeAttack();
        break;
    case ACTION_EVENT2_e:
        executeEvent2();
        break;
    case ACTION_PERCH2_e:
        executePerch2();
        break;
    case ACTION_LANDING_e:
        executeLanding();
        break;
    case ACTION_DEMO_FLY_e:
        moveDemoFly();
        break;
    case ACTION_DEMO_FLY2_e:
        moveDemoFly();
        break;
    }

    if (mType == TYPE_NORMAL && (mAction == ACTION_FLY_e || mAction == ACTION_ATTACK_e)) {
        mSph.OnAtSetBit();
    }

    setFlySound();

    if (daPy_getPlayerActorClass()->checkCargoCarry()) {
        if (mAction == ACTION_DEMO_FLY_e || mAction == ACTION_DEMO_FLY2_e) {
            dCam_getBody()->SetTrimTypeForce(2);
        } else if (isAttack()) {
            dCam_getBody()->SetTrimTypeForce(0);
            dCam_getBody()->ModeFix(5);
        }
    }

    fopAcM_posMoveF(this, NULL);

    if (field_0x6dc == 0) {
        mObjAcch.CrrPos(dComIfG_Bgsp());
    }

    if (mAnm_p != NULL) {
        mAnm_p->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    field_0x674 = current.pos;

    if (cLib_chaseF(&field_0x6c4, 0.0f, 60.0f) == 0) {
        ANGLE_ADD(field_0x6c8, 0x800);
    }

    field_0x6ca = field_0x6c4 * cM_ssin(field_0x6c8);
}

void daKago_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x,current.pos.y,current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.mBasicSize,l_HIO.mBasicSize,l_HIO.mBasicSize);

    J3DModel* model = mAnm_p->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mAnm_p->modelCalc();

    mDoMtx_stack_c::copy(mAnm_p->getModel()->getAnmMtx(YC_JNT_LEGR_3_e));
    mDoMtx_stack_c::ZrotM(field_0x6ca);
    cMtx_copy(mDoMtx_stack_c::get(), mLegR3Mtx);
}

void daKago_c::cc_set() {
    if (field_0x6de != 0) {
        eyePos = current.pos;

        attention_info.position = eyePos;
        attention_info.position.y += 30.0f;

        mSph.SetC(eyePos);
        mSph.SetR(150.0f);

        dComIfG_Ccsp()->Set(&mSph);
    }
}

int daKago_c::execute() {
    if (executeBalloonMenu()) {
#if VERSION != VERSION_SHIELD_DEBUG
        dComIfGp_setAStatusForce(BUTTON_STATUS_UNK_39, 0);
#endif
        return 1;
    }

    if (field_0x728 != 0) {
        field_0x728--;
    }

    if (field_0x72c != 0) {
        field_0x72c--;
    }

    if (mWallHitInvulnTimer != 0) {
        mWallHitInvulnTimer--;
    }

    if (mDashCooldownTime != 0) {
        mDashCooldownTime--;
    }

    if (mWaterSplashTimer != 0) {
        mWaterSplashTimer--;
    }

    if (unk_0x6ed != 0) {
        unk_0x6ed--;
    }

    action();
    mtx_set();
    cc_set();

    mPrevCamEye = dCam_getBody()->Eye();

    return 1;
}

static int daKago_Execute(daKago_c* i_this) {
    return i_this->execute();
}

static int daKago_IsDelete(daKago_c* i_this) {
    return 1;
}

int daKago_c::_delete() {
    dComIfG_resDelete(&mPhase, mArcName);

    if (mIsHioSet != 0) {
        hio_set = false;
        mDoHIO_DELETE_CHILD(l_HIO.mChild);
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daKago_Delete(daKago_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "Kago");
    return i_this->_delete();
}

int daKago_c::ctrlJoint(J3DJoint* i_joint, J3DModel* param_1) {
    J3DJoint* joint = i_joint;
    int jointNo = joint->getJntNo();

    mDoMtx_stack_c::copy(param_1->getAnmMtx(jointNo));
    if (jointNo == YC_JNT_NECK1_e || jointNo == YC_JNT_NECK2_e || jointNo == YC_JNT_HEAD_e) {
        mDoMtx_stack_c::YrotM(mHeadRotY);
        mDoMtx_stack_c::ZrotM(mHeadRotZ);
    }

    param_1->setAnmMtx(jointNo,mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(),J3DSys::mCurrentMtx);
    return 1;
}

int daKago_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daKago_c* kago = (daKago_c*)model->getUserArea();
        if (kago != NULL) {
            kago->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

int daKago_c::CreateHeap() {
    J3DModelData* modelData;

    if (mType == TYPE_TWILIGHT) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(mArcName, dRes_ID_E_YC_BMD_YC_e);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes(mArcName, dRes_ID_E_KC_BMD_KC_e);
    }
    JUT_ASSERT(5023, modelData != NULL);

    mAnm_p = new mDoExt_McaMorfSO(modelData, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes(mArcName, getBckName(dRes_ID_E_YC_BCK_YC_FLY_e)), 2, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mAnm_p == NULL || mAnm_p->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mAnm_p->getModel();
    model->setUserArea((uintptr_t)this);

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        if (i == YC_JNT_NECK1_e || i == YC_JNT_NECK2_e || i == YC_JNT_HEAD_e) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    daKago_c* i_this = (daKago_c*)actor;
    return i_this->CreateHeap();
}

int daKago_c::create() {
    fopAcM_ct(this, daKago_c);

    u32 param = (u8)fopAcM_GetParam(this);
    if ((param & 0xff) != 0xff) {
        mpPath1 = dPath_GetRoomPath(param & 0xff, fopAcM_GetRoomNo(this));
        if (mpPath1 == NULL) {
            OS_REPORT("KAGO ......NONONONONONO PATH !!!!\n");
            return cPhs_ERROR_e;
        }
    }

    mType = (fopAcM_GetParam(this) >> 16) & 0xff;
    if (mType == 0xFF) {
        mType = TYPE_TWILIGHT;
    }

    if (mType == TYPE_TWILIGHT) {
        mArcName = "E_YC";
        mDemoName = "KAGO_YAMI";
    } else {
        mArcName = "E_KC";
        mDemoName = "KAGO_HIKARI";
    }

    int phase_state = dComIfG_resLoad(&mPhase, mArcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("Kago PARAM %x \n", fopAcM_GetParam(this));

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x23a0)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            mIsHioSet = TRUE;
            hio_set = TRUE;
            l_HIO.mChild = mDoHIO_CREATE_CHILD("カーゴロック（いい奴）", &l_HIO);
        }

        attention_info.flags = 0;

        fopAcM_SetMtx(this, mAnm_p->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);

        mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                     &mAcchCir, fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(100.0f, 200.0f);
        mObjAcch.ClrRoofNone();
        mObjAcch.SetRoofCrrHeight(150.0f);

        field_0x560 = health = 100;

        mStts.Init(0xff, 0, this);
        mSph.Set(cc_sph_src);
        mSph.SetStts(&mStts);

        field_0x674 = current.pos;

        mSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mSound;

        maxFallSpeed = -100.0f;
        gravity = 0.0f;
        mPathStep = 1;

        setActionMode(ACTION_WAIT_e, 0);

#if DEBUG
        if (mDoCPd_c::getHoldL(PAD_1) && mDoCPd_c::getHoldR(PAD_1)) {
            dComIfGs_offSwitch(45, fopAcM_GetRoomNo(this));
            dComIfGs_offSwitch(50, fopAcM_GetRoomNo(this));

            setActionMode(ACTION_LANDING_e, 0);
            setMidnaRideOn();
            setPlayerRideOn();

            field_0x6dc = 0;
            field_0x6de = 0xff;
            field_0x6df = 1;
        }
#endif

        mSceneType = SCENE_TYPE_DEFAULT;
        if (strcmp("F_SP115", dComIfGp_getStartStageName()) == 0) {
            if (dComIfGp_getStartStageRoomNo() == 0) {
                mSceneType = SCENE_TYPE_LAKE_HYLIA;
            }
        } else if (strcmp("F_SP112", dComIfGp_getStartStageName()) == 0) {
            if (dComIfGp_getStartStageRoomNo() == 1) {
                mSceneType = SCENE_TYPE_RIVER;
            }
        } else if (strcmp("F_SP126", dComIfGp_getStartStageName()) == 0 &&
                   dComIfGp_getStartStageRoomNo() == 0)
        {
            mSceneType = SCENE_TYPE_BOARD_HOUSE;
        }

        mBalloon2DId = fpcM_ERROR_PROCESS_ID_e;

        mtx_set();

        daKago_Execute(this);
    }

    return phase_state;
}

static int daKago_Create(daKago_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daKago_Method = {
    (process_method_func)daKago_Create,
    (process_method_func)daKago_Delete,
    (process_method_func)daKago_Execute,
    (process_method_func)daKago_IsDelete,
    (process_method_func)daKago_Draw,
};

actor_process_profile_definition g_profile_KAGO = {
    fpcLy_CURRENT_e,        // mLayerID
    4,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_KAGO,              // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daKago_c),       // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    693,                    // mPriority
    &l_daKago_Method,       // sub_method
    0x00044000,             // mStatus
    fopAc_NPC_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
