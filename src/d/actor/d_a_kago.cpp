/**
 * @file d_a_kago.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kago.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_msg_object.h"

#include <cmath.h>

#include "d/actor/d_a_balloon_2D.h"
#include "d/actor/d_a_e_ym.h"
#include "d/actor/d_a_midna.h"
#include "d/actor/d_a_obj_balloon.h"
#include "d/actor/d_a_obj_riverrock.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_tag_waterfall.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_overlap_mng.h"

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
    ctx->genLabel("カーゴロック（いい奴）", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    // Revocon processing (Revolution controller?)
    ctx->genLabel("レボコン処理", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("レボコン上下", &mRevoconUpDown, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("レボコン上下Max", &mRevoconUpDownMax, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("レボコン左右", &mRevoconLeftRight, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("レボコン左右Max", &mRevoconLeftRightMax, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel(" ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("基本サイズ", &mBasicSize, 0.0f, 5.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("飛行速度", &mFlightSpeed, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("飛行地面高度", &mFlightGroundAltitude, 0.0f, 20000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("飛行天井高度", &mFlightCeilingAltitude, 0.0f, 20000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("下降速度加算値", &mDescentRateIncrement, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("上昇速度減速値", &mAscentRateDecel, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("影の濃さ", &mShadowDensity, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("壁かすり後無敵時間", &mWallHitInvulnTime, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("ダッシュ時間", &mDashTime, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("ダッシュ溜め時間(当社比X倍)", &mDashTimeMultiplier, 0.0f, 10.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("ダッシュ無効時間", &mDashCooldownTime, 0.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel("-", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel("---------------------------------------", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    // "Below this are experimental parameters"
    ctx->genLabel("これより下は効果線エフェクト実験パラメータ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel("-", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("Ｚオフセット", &mZOffset, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("Ｚオフセット(横)", &mZOffsetHori, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genCheckBox("角度追従モード", &mAngleTrackingMode, 1, 0, NULL, 0xffff, 0xffff, 512, 24);
    // check OFF to track camera speed
    ctx->genLabel(".....チェックＯＦＦでカメラの速度追従", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    // check ON to track Kargarok angle
    ctx->genLabel(".....チェックＯＮでカーゴの角度追従", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel("-", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    // "Secret ripple effect parameters"
    ctx->genLabel("こっそりさざ波エフェクトパラメータ", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genLabel("-", 0x80000001, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("水面からのYオフセット", &mYOffsetFromWaterSurface, -1000.0f, 1000.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("着水時飛沫発生時間", &mWaterSplashTime, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
    ctx->genSlider("ダッシュ時飛沫発生時間", &mSplashGenTimeDuringDash, 0.0f, 100.0f, 0, NULL, 0xffff, 0xffff, 512, 24);
}
#endif

int daKago_c::getBckName(int param_0) {
    if (field_0x6e7 == 0) {
        return param_0;
    }

    switch (param_0) {
    case 7:
        return 4;
    case 8:
        return 5;
    case 9:
        return 6;
    case 10:
        return 7;
    case 11:
        return 8;
    case 12:
        return 9;
    case 13:
        return 10;
    case 14:
        return 11;
    case 15:
        return 12;
    case 21:
        return 13;
    default:
        return 6;
    }
}

void daKago_c::setBck(int param_0, u8 param_1, f32 param_2, f32 param_3) {
    mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes(field_0x75c, getBckName(param_0)),
                   param_1, param_2, param_3, 0.0f, -1.0f);
}

bool daKago_c::checkBck(int param_0) {
    if (mpMorf->getAnm() == dComIfG_getObjectRes(field_0x75c, getBckName(param_0))) {
        return true;
    } else {
        return false;
    }
}

int daKago_c::draw() {
    if (field_0x6de == NULL) {
        return 1;
    }

    g_env_light.settingTevStruct(2, &current.pos, &tevStr);
    J3DModel* model = mpMorf->getModel();
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    fopAcM_setEffectMtx(this, model->getModelData());
    dComIfGd_setListDark();
    mpMorf->entryDL();
    dComIfGd_setList();
    cXyz unkXyz1;
    unkXyz1.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    field_0x764 =
        dComIfGd_setShadow(field_0x764, 0, model, &unkXyz1, 3500.0f, 0.0f, current.pos.y,
                           mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f,
                           dDlst_shadowControl_c::getSimpleTex());

    return 1;
}

static int daKago_Draw(daKago_c* i_this) {
    return i_this->draw();
}

int daKago_c::executeBalloonMenu() {
    dCamera_c* camera = dCam_getBody();

    switch (field_0x6eb) {
    case 0:
        if (field_0x6e7 == 1 && (mCurrentAction == 0 || mCurrentAction == 5) &&
            dComIfGp_event_runCheck() == 0 && mDoCPd_c::getTrigB(0))
        {
            field_0x6eb = 1;
            return 1;
        }

        return 0;
    case 1:
        if (eventInfo.checkCommandDemoAccrpt() == 0) {
            fopAcM_orderPotentialEvent(this, 1, 0xffff, 0);
            eventInfo.onCondition(2);

            break;
        }

        mMsgFlow.init(this, 0x457, 0, NULL);

        camera->Stop();
        camera->SetTrimSize(3);

        field_0x6eb = 0x2;

        break;
    case 2:
        if (mMsgFlow.doFlow(this, NULL, 0)) {
            if (dMsgObject_getSelectCursorPos() == 0) {
                field_0x6eb = 0;

                camera->Start();
                camera->SetTrimSize(0);

                dComIfGp_event_reset();

                return 1;
            }

            if (dMsgObject_getSelectCursorPos() == 1) {
                field_0x6eb = 3;
                dComIfGp_setNextStage("F_SP112", 0, dComIfGp_roomControl_getStayNo(),
                                      dComIfG_play_c::getLayerNo(0), 0.0f, 10, 1, 0, 0, 1, 0);
            } else {
                field_0x6eb = 3;
                dStage_changeScene(3, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            }
        }

        break;
    }

    return 1;
}

static u8 l_HIOInit;
static daKago_HIO_c l_HIO;

f32 daKago_c::checkGroundHeight(cXyz i_pos, f32* o_step) {
    f32 retVal;

    retVal = mGroundHeight;

    if (mpPath1 != NULL) {
        cXyz pointPos1;
        cXyz pointPos2;
        cXyz cStack_140;

        pointPos1 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;

        int pointIdx2 = (int)mPathIdx - (int)mPathIdxOffset;
        if (pointIdx2 >= mpPath1->m_num || pointIdx2 < 0) {
            pointIdx2 = mPathIdx;
        }
        pointPos2 = dPath_GetPnt(mpPath1, pointIdx2)->m_position;

        s16 targetAngleY = cLib_targetAngleY(&pointPos2, &pointPos1);

        mDoMtx_stack_c::YrotS(-targetAngleY);
        mDoMtx_stack_c::transM(-i_pos.x, -i_pos.y, -i_pos.z);
        mDoMtx_stack_c::multVec(&pointPos2, &cStack_140);

        f32 var_f29 = -cStack_140.z;
        if (var_f29 < 0.0f) {
            var_f29 = 0.0f;
        }

        mDoMtx_stack_c::YrotS(-targetAngleY);
        mDoMtx_stack_c::transM(-i_pos.x, -i_pos.y, -i_pos.z);
        mDoMtx_stack_c::multVec(&pointPos1, &cStack_140);

        f32 var_f28 = cStack_140.z;
        if (var_f28 < 0.0f) {
            var_f28 = 0.0f;
        }

        f32 var_f27 = var_f29 / (var_f29 + var_f28);
        retVal = pointPos1.y * var_f27 + pointPos2.y * (1.0f - var_f27);

        if (o_step != NULL) {
            f32 pointLatDist = pointPos1.absXZ(pointPos2);
            f32 pointVertDist = std::abs(pointPos1.y - pointPos2.y);
            *o_step = pointVertDist * l_HIO.mFlightSpeed / pointLatDist + 5.0f;
        }
    }

    field_0x6e0 = 0;

    dBgS_GndChk gndChk;
    cXyz gndChkPos(i_pos.x, i_pos.y + 500.0f, i_pos.z);
    gndChk.SetPos(&gndChkPos);
    field_0x700 = dComIfG_Bgsp().GroundCross(&gndChk);

    field_0x738 = 0;

    if (field_0x700 != -G_CM3D_F_INF) {
        field_0x738 = dComIfG_Bgsp().GetSpecialCode(gndChk);
        field_0x700 += l_HIO.mFlightGroundAltitude;
        if (retVal < field_0x700) {
            retVal = field_0x700;
            if (current.pos.y < retVal) {
                mGroundHeight = current.pos.y;
            } else {
                mGroundHeight = field_0x700;
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

        if (retVal < gndCrossMag) {
            field_0x700 = gndCrossMag;
            retVal = gndCrossMag;
            field_0x6e0 = 1;

            if (current.pos.y < gndCrossMag) {
                mGroundHeight = current.pos.y;
            } else {
                mGroundHeight = field_0x700;
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
        field_0x71c = 0;
        field_0x6e6 = 0;
    }

    return retVal;
}

f32 daKago_c::checkRoofHeight(cXyz param_0) {
    f32 roofChkYVal = mRoofHeight;
    BOOL unkFlag1 = FALSE;

    field_0x6e5 = 0;

    dBgS_RoofChk roofChk;
    cXyz unkXyz1(param_0.x, param_0.y - 500.0f, param_0.z);
    roofChk.SetPos(unkXyz1);

    roofChkYVal = dComIfG_Bgsp().RoofChk(&roofChk);
    if (roofChkYVal != G_CM3D_F_INF) {
        roofChkYVal -= l_HIO.mFlightCeilingAltitude;
        if (current.pos.y > roofChkYVal) {
            mRoofHeight = current.pos.y;
        } else {
            mRoofHeight = roofChkYVal;
        }

        if (dComIfG_Bgsp().GetSpecialCode(roofChk) == 1) {
            unkFlag1 = TRUE;
        }

        if (std::abs(current.pos.y - roofChkYVal) < 310.0f) {
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

    return roofChkYVal;
}

void daKago_c::checkMoveHeight() {
    f32 step = 100.0f;
    f32 target = checkGroundHeight(current.pos, &step);
    cLib_chaseF(&mGroundHeight, target, step);
    cLib_chaseF(&mRoofHeight, checkRoofHeight(current.pos), 30.0f);
}

void daKago_c::checkSizeBg() {
    dBgS_LinChk linChk;
    cXyz unkXyz1;
    cXyz unkXyz2;
    cM3dGPla plane;

    J3DModel* model = mpMorf->getModel();

    field_0x6d9 = 0;

    if (mCurrentAction == 4) {
        return;
    }

    mDoMtx_stack_c::copy(model->getAnmMtx(10));
    mDoMtx_stack_c::multVecZero(&unkXyz1);
    mDoMtx_stack_c::copy(model->getAnmMtx(14));
    mDoMtx_stack_c::multVecZero(&unkXyz2);
    unkXyz2 = unkXyz1 + (unkXyz2 - unkXyz1) * 0.9f;
    linChk.Set(&unkXyz1, &unkXyz2, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (dComIfG_Bgsp().GetSpecialCode(linChk) == 1) {
            field_0x6d9 |= (u8)1;
            field_0x650[0] = linChk.GetCross();
        }
    }

    mDoMtx_stack_c::copy(model->getAnmMtx(15));
    mDoMtx_stack_c::multVecZero(&unkXyz1);
    mDoMtx_stack_c::copy(model->getAnmMtx(19));
    mDoMtx_stack_c::multVecZero(&unkXyz2);
    unkXyz2 = (unkXyz1 + (unkXyz2 - unkXyz1) * 0.9f);
    linChk.Set(&unkXyz1, &unkXyz2, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (dComIfG_Bgsp().GetSpecialCode(linChk) == 1) {
            field_0x6d9 |= (u8)0x2;
            field_0x650[1] = linChk.GetCross();
        }
    }

    unkXyz1 = current.pos;
    mDoMtx_stack_c::copy(model->getAnmMtx(1));
    mDoMtx_stack_c::multVecZero(&unkXyz1);
    mDoMtx_stack_c::transM(300.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&unkXyz2);
    linChk.Set(&unkXyz1, &unkXyz2, NULL);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        dComIfG_Bgsp().GetTriPla(linChk, &plane);
        if (dComIfG_Bgsp().GetSpecialCode(linChk) == 1) {
            if (!checkYaguraPos(linChk.GetCross())) {
                field_0x6d9 |= (u8)4;
                this->field_0x650[2] = linChk.GetCross();
            }
        }
    }
}

s16 daKago_c::getBeforeGroundHeight(u8 param_0) {
    return 0;
}

void daKago_c::demo_skip(int param_0) {
    dCamera_c* camera = dCam_getBody();

    switch (param_0) {
    case 0:
        field_0x748 = 2;
        field_0x74c = NULL;
        break;
    case 1:
        field_0x728 = 0;
        break;
    case 3:
        mMsgFlow.remove();
    case 2:
        setMidnaRideOn();
        setPlayerRideOn();
        field_0x718 = field_0x71a = 0;
        /* dSv_event_flag_c::M_051 - Main Event - Shadow Kargorok (?) (Large) event complete (Horse grass appears in various places) */
        dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[84]);
        field_0x748 = 7;
        field_0x74c = NULL;
        break;
    case 4:
        setSceneChange(0);
        field_0x6e4 = 1;
        break;
    case 5:
    case 6:
        if (setSceneChange(3)) {
            if (param_0 == 5) {
                field_0x748 = 6;
            } else {
                field_0x748 = 5;
                field_0x6cc = 1.0f;
            }
        }
        break;
    case 7:
        setRideOff();
    case 8: {
        field_0x68c.set(-77875.0f, -18287.0f, 42000.0f);
        field_0x698.set(-77275.0f, -18500.0f, 41090.0f);
        field_0x6a4.set(-77615.0f, -18640.0f, 41400.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&field_0x6a4, 0, 0);
        field_0x6de = 0;
        speed.y = speedF = 0.0f;
        setActionMode(4, 0);
        camera->Set(field_0x68c, field_0x698, 70.0f, 0);
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
        field_0x68c.set(3703.0f, 337.0f, 863.0f);
        field_0x698.set(3726.0f, 272.0f, 1196.0f);
        field_0x6a4.set(3782.0f, 222.0f, 690.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&field_0x6a4, 0, 0);
        field_0x6de = 0;
        speed.y = speedF = 0.0f;
        setActionMode(4, 0);
        camera->Set(field_0x68c, field_0x698, 70.0f, 0);
        camera->Reset();
        camera->Start();
        camera->SetTrimSize(0);
        dComIfGp_event_reset();
        Z2GetAudioMgr()->setDemoName(NULL);
    }
}

int daKago_c::DemoSkipCallBack(void* param_0, int param_1) {
    if (param_0 != NULL) {
        ((daKago_c*)param_0)->demo_skip(param_1);
        return 1;
    }
    return 0;
}

void daKago_c::setActionMode(int param_0, int param_1) {
    mIsFlying = false;
    mCurrentAction = param_0;
    field_0x744 = param_1;
}

void daKago_c::setMidnaTagPos() {
    cXyz unkXyz1;
    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    if (midna != NULL) {
        mDoMtx_stack_c::copy(getMidnaLocaterMtx());
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        midna->onTagWaitPos(&unkXyz1);
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
    f32 unkFloat1;
    f32 unkFloat2 = unkFloat1 = 100000.0f;
    int pointIdx1;
    for (int i = 0; i < mpPath1->m_num; i++) {
        pointPos = dPath_GetPnt(mpPath1,i)->m_position;

        f32 playerLatDist = playerPos.absXZ(pointPos);
        if (playerLatDist < unkFloat2) {
            unkFloat1 = unkFloat2;
            unkFloat2 = playerLatDist;
            pointIdx1 = i;
        } else if (playerLatDist < unkFloat1) {
            unkFloat1 = playerLatDist;
        }
    }

    int pointIdx2 = pointIdx1 + mPathIdxOffset;
    if (pointIdx2 < 0) {
        pointIdx2 = 1;
    } else if (pointIdx2 >= mpPath1->m_num) {
        pointIdx2 = mpPath1->m_num - 1;
    }

    return pointIdx2;
}

int daKago_c::setSceneChange(int param_0) {
    int unkInt1 = 0;
    switch (param_0) {
    case 0:
        if (field_0x6e7 == 0) {
            unkInt1 = 1;
        } else {
            unkInt1 = 6;
        }

        break;
    case 1:
        unkInt1 = 0;

        endBalloonScore();

        break;
    case 2:
        if (field_0x6e7 == 0) {
            unkInt1 = 3;
        } else {
            unkInt1 = 0;
        }

        endBalloonScore();

        break;
    case 3:
        unkInt1 = 3;

        break;
    }

    return dStage_changeScene(unkInt1, 0.0f, 10, fopAcM_GetRoomNo(this), 0, -1);
}

void daKago_c::createBalloonScore() {
    if (field_0x6e7 != 0 && mBalloon2DId == fpcM_ERROR_PROCESS_ID_e) {
        mBalloon2DId = fopAcM_create(PROC_BALLOON2D, 0, 0, 0xffffffff, 0, 0, 0xffffffff);
        field_0x6e9 = 1;
    }
}

void daKago_c::startBalloonScore() {
    fopAc_ac_c* balloon_actor;
    if (field_0x6e7 != 0 && field_0x6e9 == 1) {
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
    fopAc_ac_c *balloonObj;

    if (field_0x6e7 != 0) {
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

f32 daKago_c::checkNextPath(cXyz param_0) {
    cXyz cStack_14;
    mDoMtx_stack_c::YrotS(-field_0x714);
    mDoMtx_stack_c::transM(-current.pos.x, -current.pos.y, -current.pos.z);
    mDoMtx_stack_c::multVec(&param_0, &cStack_14);
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
    s16 temp = -0x770;
    mDoMtx_stack_c::YrotS(-temp);
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
        cXyz unkXyz1;
        setActionMode(1, 3);
        dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        unkXyz1 = current.pos;
        unkXyz1.y += 200.0f;
        setWallHitEffect(unkXyz1, 0);
        return true;
    }

    if (mObjAcch.ChkWallHit()) {
        field_0x6da = mAcchCir.GetWallAngleY();
        s16 unkInt1 = shape_angle.y - field_0x6da;
        if (!checkYaguraPos(current.pos)) {
            if (abs(unkInt1) > 0x7000) {
                field_0x6d9 |= (u8)0x4;
                field_0x650[2].set(current.pos.x + cM_ssin(shape_angle.y) * 200.0f, current.pos.y, current.pos.z + cM_scos(shape_angle.y) * 200.0f);
            }
        }

        if (unkInt1 > 0) {
            field_0x6d9 |= (u8)0x1;
            field_0x650[0].set(current.pos.x + cM_ssin(shape_angle.y + 0x4000) * 200.0f, current.pos.y,
                            current.pos.z + cM_scos(shape_angle.y + 0x4000) * 200.0f);
        } else {
            field_0x6d9 |= (u8)0x2;
            field_0x650[1].set(current.pos.x + cM_ssin(shape_angle.y + -0x4000) * 200.0f,
                            current.pos.y,
                            current.pos.z + cM_scos(shape_angle.y + -0x4000) * 200.0f);
        }
    }

    if ((field_0x6d9 & 0x4) != 0) {
        setActionMode(1, 0);
        dComIfGp_getVibration().StartShock(8, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        setWallHitEffect(field_0x650[2], 0);
        return true;
    }

    if (param_0 != 0 && field_0x720 == 0 && (field_0x6d9 & 0x3) != 0) {
        dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        if ((field_0x6d9 & 0x1) != 0) {
            setActionMode(1, 1);
            setWallHitEffect(field_0x650[0], 1);
        } else {
            setActionMode(1, 2);
            setWallHitEffect(field_0x650[1], 1);
        }
        return true;
    }

    return false;
}

bool daKago_c::checkAttackStart() {
    if (mDashCooldownTime != 0) {
        return false;
    }

    if (mDoCPd_c::getTrigA(0)) {
        setActionMode(5, 0);
        mIsFlying = true;
        return true;
    } else {
        return false;
    }
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

s16 daKago_c::getValueY(f32 param_0) {
    s16 valueY = field_0x6f4 * param_0;
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
    return -field_0x6f0 * param_0;
}

s16 daKago_c::getValueAbsX(f32 param_0) {
    return std::abs(-field_0x6f0 * param_0);
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
    s16 angleX2 = current.angle.y;
    cLib_addCalcAngleS(&current.angle.y, angleY, 8, 0x100, 0x10);
    shape_angle.y = current.angle.y;
    cLib_addCalcAngleS(&shape_angle.z, ((angleX2 - current.angle.y) * 0x20), 8, 0x400, 0x10);

    if (shape_angle.z > 0x3000) {
        shape_angle.z = 0x3000;
    }

    if (shape_angle.z < -0x3000) {
        shape_angle.z = -0x3000;
    }
}

void daKago_c::flySpeedCalc(s16 param_0, int param_1) {
    s16 var_r29;
    s16 var_r28;
    s16 var_r27;

    s16 sp_8;
    if (param_1 == 0) {
        sp_8 = 0x2000;
    } else if (param_1 == 1) {
        sp_8 = 0x1000;
    } else if (param_1 == 2) {
        sp_8 = 0x2aaa;
    }

    var_r28 = getValueY(sp_8);
    if (mpLockActor != NULL) {
        var_r28 = -cLib_targetAngleX(&current.pos, &mpLockActor->current.pos);
    }

    if (var_r28 != 0) {
        field_0x710 += getValueY(40.0f);

        var_r29 = 0x200;
        if (param_1 == 2) {
            var_r29 = 0x400;
        }
        if (field_0x710 > var_r29) {
            field_0x710 = var_r29;
        }
        if (field_0x710 < -var_r29) {
            field_0x710 = -var_r29;
        }

        if (field_0x6f4 > 0.0f) {
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
    if (param_1 == 1) {
        var_r27 = 0x1000;
    } else if (param_1 == 2) {
        var_r27 = 0x2aaa;
    }

    f32 unkFloat1 = 20.0f;
    var_r29 = 0x100;
    if (param_1 == 2) {
        unkFloat1 = 30.0f;
        var_r29 = 0x200;
    }

    var_r28 = param_0 + getValueX(var_r27);
    if (mpLockActor != NULL) {
        var_r28 = cLib_targetAngleY((Vec*)&current, (Vec*)&mpLockActor->current);
    }

    int always_zero = 0;
    if (!always_zero) {
        if (abs((s16)(current.angle.y - param_0)) < var_r27) {
            if (field_0x6f0) {
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
            if (field_0x6f0 < 0.0f) {
                if ((s16)(current.angle.y - param_0) < var_r27) {
                    sp_14 = 1;
                }
            } else {
                if (field_0x6f0 > 0.0f && (s16)(current.angle.y - param_0) > var_r27) {
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
        if (field_0x6e7 == 1) {
            mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
        } else {
            mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
        }
    }

    if (field_0x6e6 == 1) {
        field_0x6e6 = 2;
        if (mCurrentAction == 5) {
            field_0x71c = l_HIO.mSplashGenTimeDuringDash;
        } else {
            field_0x71c = l_HIO.mWaterSplashTime;
        }
    }
}

bool daKago_c::checkFlySceneChange() {
    if ((mPathIdxOffset > 0 && mPathIdx >= mpPath1->m_num - 1 ||
         mPathIdxOffset < 0 && mPathIdx < 1) &&
        field_0x73c == 1 && fopOvlpM_IsPeek() == 0)
    {
        setActionMode(0, 4);
        field_0x728 = 300;

        if (checkBck(9) == 0) {
            setBck(9, 2, 10.0f, 1.0f);
        }

        if (mPathIdxOffset > 0) {
            setSceneChange(1);
        } else {
            setSceneChange(2);
        }

        return true;
    } else {
        return false;
    }
}

void daKago_c::setFlyAway() {
    current.angle.y = shape_angle.y;
    setActionMode(0, 3);
    field_0x728 = 90;
    if (!checkBck(9)) {
        setBck(9, 2, 10.0f, 1.0f);
    }
    dComIfGp_getVibration().StopQuake(0x1f);
}

void daKago_c::setFlyAnime() {
    if (cM_rnd() < 0.5) {
        if (checkBck(15) == 0) {
            setBck(15, 2, 10.0f, 1.0f);
        } else {
            setBck(13, 2, 10.0f, 1.0f);
        }
    } else {
        if (!checkBck(13)) {
            setBck(13, 2, 10.0f, 1.0f);
        }
    }

    field_0x728 = 150;
}

void daKago_c::executeFly() {
    s16 unkInt1;
    s16 unkInt2;
    f32 unkFloat1;
    cXyz unkXyz1;
    cXyz unkXyz2;
    cXyz unkXyz3;

    startBalloonScore();

    if (field_0x744 < 3) {
        if (mDashCooldownTime == 0) {
            dComIfGp_setDoStatusForce(0x4b, 0);
        }
        if (field_0x6e7 == 1) {
            dComIfGp_setAStatusForce(0x2a, 0);
        } else {
            dComIfGp_setAStatusForce(0x27, 0);
        }
    }

    if (field_0x6e6 != 0) {
        setSibukiEffect();
    }

    checkHeight();
    mIsFlying = true;

    switch (field_0x744) {
    case 0:
        shape_angle.y = current.angle.y;
        mGroundHeight = checkGroundHeight(current.pos, NULL);
        mRoofHeight = checkRoofHeight(current.pos);
        setPlayerRideOn();
    case 1:
        setFlyAnime();
        field_0x710 = field_0x712 = 0;
        field_0x744 = 2;
        break;
    case 2:
        unkXyz1 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
        unkXyz2 = dPath_GetPnt(mpPath1, mPathIdx - mPathIdxOffset)->m_position;
        unkInt1 = cLib_targetAngleY(&unkXyz2, &unkXyz1);
        field_0x714 = unkInt1;
        flySpeedCalc(unkInt1, 0);
        if (checkBck(11)) {
            if (mpMorf->isStop()) {
                setFlyAnime();
            }
        } else {
            if (field_0x710 < 0) {
                if (!checkBck(15)) {
                    setBck(15, 2, 10.0f, 1.0f);
                }
                field_0x728 = 10;
            } else if (field_0x710 > 0) {
                if (!checkBck(13)) {
                    setBck(13, 2, 10.0f, 1.0f);
                }
                field_0x728 = 90;
            } else {
                if (field_0x728 == 0) {
                    setFlyAnime();
                }
            }
        }
        if (cM_ssin(current.angle.x) > 0.0f) {
            unkFloat1 = l_HIO.mFlightSpeed + l_HIO.mDescentRateIncrement * cM_ssin(current.angle.x);
        } else {
            unkFloat1 = l_HIO.mFlightSpeed + l_HIO.mAscentRateDecel * cM_ssin(current.angle.x);
        }
        if (std::abs(field_0x6f8 - unkFloat1) > 10.0f) {
            cLib_chaseF(&field_0x6f8, unkFloat1, 2.0f);
        } else {
            cLib_chaseF(&field_0x6f8, unkFloat1, 1.0f);
        }
        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
        unkXyz3.z = checkNextPath(unkXyz1);
        if (unkXyz3.z < 0.0f) {
            if (abs((s16)(unkInt1 - cLib_targetAngleY(&unkXyz2, &current.pos))) < 0x4000) {
                mPathIdx += mPathIdxOffset;
                if (checkFlySceneChange()) {
                    return;
                }
            }
        }
        break;
    case 3:
    case 4:
        if (mPathIdx >= mpPath1->m_num) {
            unkInt1 = current.angle.y;
            unkInt2 = 0;
        } else {
            unkXyz1 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
            unkXyz2 = dPath_GetPnt(mpPath1, mPathIdx - mPathIdxOffset)->m_position;
            unkInt1 = cLib_targetAngleY(&unkXyz2, &unkXyz1);
            unkInt2 = -cLib_targetAngleX(&unkXyz2, &unkXyz1);
        }
        if (field_0x744 == 3) {
            cLib_chaseUC(&field_0x6de, 0, 4);
            cLib_addCalcAngleS(&current.angle.x, 0xffffe000, 8, 0x100, 0x10);
        } else {
            cLib_addCalcAngleS(&current.angle.x, unkInt2, 8, 0x100, 0x10);
        }
        shape_angle.x = current.angle.x;
        cLib_addCalcAngleS(&current.angle.y, unkInt1, 8, 0x400, 0x10);
        shape_angle.y = current.angle.y;
        cLib_addCalcAngleS(&shape_angle.z, 0, 8, 0x400, 0x10);
        unkFloat1 = l_HIO.mFlightSpeed + 20.0f;
        if (std::abs(field_0x6f8 - unkFloat1) > 10.0f) {
            cLib_chaseF(&field_0x6f8, unkFloat1, 2.0f);
        } else {
            cLib_chaseF(&field_0x6f8, unkFloat1, 1.0f);
        }
        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
        if (field_0x728 == 0) {
            setActionMode(4, 0);
        }
        return;
    }

    if (!checkWallHitFall(1) && checkAttackStart()) {
        return;
    }
}

void daKago_c::executeStagger() {
    dCamera_c* camera = dCam_getBody();

    cXyz cStack_94 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
    cXyz cStack_a0 = dPath_GetPnt(mpPath1, mPathIdx - mPathIdxOffset)->m_position;

    field_0x714 = cLib_targetAngleY(&cStack_a0, &cStack_94);

    if (field_0x6e7 == 1) {
        dComIfGp_setAStatusForce(0x2a, 0);
    } else {
        dComIfGp_setAStatusForce(0x27, 0);
    }

    checkHeight();

    mIsFlying = true;

    switch (field_0x744) {
    case 0:
        mSph.OffAtSetBit();

        daPy_getPlayerActorClass()->setPlayerDamage(0, 1);

        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(6));
        mDoMtx_stack_c::multVecZero(&field_0x674);

        if (field_0x6e7 == 0) {
            mSound.startCreatureSound(Z2SE_EN_YC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_YC_V_HANGED, -1);
        } else {
            mSound.startCreatureSound(Z2SE_EN_KC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_KC_V_CRASH, -1);
        }

        setBck(7, 0, 10.0f, 1.0f);

        field_0x728 = 60;
        speedF = 30.0f;

        if (current.pos.y < cStack_94.y) {
            speed.y = 30.0f;
        } else {
            speed.y = -30.0f;
        }

        current.angle.y = field_0x6da - (s16)(current.angle.y - field_0x6da) + 0x8000;

        field_0x744 = 4;

        break;
    case 1:
    case 2:
        mSph.OffAtSetBit();

        daPy_getPlayerActorClass()->setPlayerDamage(0, 1);

        setBck(15, 2, 10.0f, 1.0f);

        field_0x728 = 10;
        field_0x720 = (int)l_HIO.mWallHitInvulnTime;
        speedF = 40.0f;

        if (field_0x744 == 1) {
            shape_angle.z = 0x3000;
            current.angle.y += (s16)-0x2000;
            if (abs((s16)(current.angle.y - field_0x714)) > 0x2000) {
                current.angle.y = field_0x714 + -0x2000;
            }
        } else {
            shape_angle.z = -0x3000;
            current.angle.y += (s16)0x2000;
            if (abs((s16)(current.angle.y - field_0x714)) > 0x2000) {
                current.angle.y = field_0x714 + 0x2000;
            }
        }

        mSound.startCreatureSound(Z2SE_EN_YC_HIT_SIDE, 0, -1);
        dComIfGp_getVibration().StartQuake(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        field_0x744 = 5;

        break;
    case 3:
        daPy_getPlayerActorClass()->setPlayerDamage(2, 1);

        mSph.OffAtSetBit();

        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(6));
        mDoMtx_stack_c::multVecZero(&field_0x674);

        if (field_0x6e7 == 0) {
            mSound.startCreatureSound(Z2SE_EN_YC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_YC_V_HANGED, -1);
        } else {
            mSound.startCreatureSound(Z2SE_EN_KC_CRASH, 0, -1);
            mSound.startCreatureVoice(Z2SE_EN_KC_V_CRASH, -1);
        }

        setBck(7, 0, 10.0f, 1.0f);

        field_0x728 = 0x3c;
        speedF = 30.0f;
        speed.y = -50.0f;

        field_0x744 = 6;

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
        field_0x6f8 = 40.0f;

        setActionMode(0, 1);

        dComIfGp_getVibration().StopQuake(0x1f);

        break;
    case 4:
    case 6:
        if (checkBck(7)) {
            cXyz cStack_ac;
            mDoMtx_stack_c::copy(mLegR3Mtx);
            mDoMtx_stack_c::multVecZero(&cStack_ac);
            cStack_ac.y -= 20.0f;

            cXyz cStack_b8 = current.pos;
            cStack_b8.y += 50.0f;

            dBgS_LinChk dStack_88;
            dStack_88.Set(&cStack_b8, &cStack_ac, NULL);

            if (dComIfG_Bgsp().LineCross(&dStack_88)) {
                current.pos.y += 5.0f;
            }

            setDamageEffect();

            field_0x728 = 60;

            if (mpMorf->checkFrame(22.0f)) {
                setRideOff();
                setBck(8, 2, 10.0f, 1.0f);
            }
        }

        if (field_0x744 == 4) {
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
            field_0x6f8 = -10.0f;
        }

        break;
    }
}

void daKago_c::executeWait() {
    daPy_py_c* player = (daPy_py_c*)daPy_getPlayerActorClass()->getMidnaActor();
    if (field_0x728 == 0) {
        cLib_chaseUC(&field_0x6de, 0, 4);
    }

    if (field_0x6df == 0) {
        if (player->checkWolfCargoCarrySceneChange()) {
            if (field_0x73c == 1) {
                createBalloonScore();
                if (dComIfGp_getStartStagePoint() == 0) {
                    mPathIdxOffset = 1;
                    mPathIdx = 0;
                    mPathDir = 1;
                } else {
                    mPathIdxOffset = -1;
                    mPathIdx = mpPath1->m_num + -1;
                    mPathDir = -1;
                }

                old.pos = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
                current.pos = old.pos;
                mPathIdx += mPathIdxOffset;
                field_0x6a4 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;

                current.angle.y = shape_angle.y = cLib_targetAngleY((Vec*)&current, &field_0x6a4);
                if (player != NULL) {
                    setMidnaRideOn();
                    setPlayerRideOn();

                    field_0x6dc = 0;
                    field_0x6de = 0xff;
                    field_0x6df = 1;

                    if (mPathDir == 1 && field_0x6e7 == 0 && dComIfGs_isSaveSwitch(9) == 0) {
                        setActionMode(9, 0);
                    } else {
                        setActionMode(10, 0);
                    }

                    setFlyAnime();
                    moveDemoFly();
                }
            } else if (field_0x73c == 0 || field_0x73c == 2) {
                setMidnaRideOn();
                setPlayerRideOn();

                field_0x6dc = 0;
                field_0x6de = 0xff;
                field_0x6df = 1;

                setActionMode(8, 0);
                executeLanding();
            }

        }

        return;
    }

#if DEBUG
    mPathDir = 1;

    if (mDoCPd_c::getHoldL(0) && mDoCPd_c::getHoldR(0) &&
        mDoCPd_c::getTrigB(0))
    {
        setActionMode(2, 0);

        if (field_0x73c == 2) {
            setKagoPath(1);
        } else {
            setKagoPath(5);

            if (field_0x73c == 1) {
                createBalloonScore();
            }
        }
    }
#endif
}

bool daKago_c::calcAttackMove(int param_0) {
    cXyz acStack_20;
    cXyz cStack_2c;
    cXyz cStack_38;

    acStack_20 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
    cStack_2c = dPath_GetPnt(mpPath1, mPathIdx - mPathIdxOffset)->m_position;

    s16 targetAngleY = cLib_targetAngleY(&cStack_2c, &acStack_20);
    field_0x714 = targetAngleY;

    if (mpLockActor != NULL) {
        flySpeedCalcLockOn();
    } else {
        if (param_0 == 0) {
            flySpeedCalc(targetAngleY, 1);
        } else {
            flySpeedCalc(targetAngleY, 2);
        }
    }

    speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
    speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

    cStack_38.z = checkNextPath(acStack_20);
    if (cStack_38.z < 0.0f) {
        if (abs((s16)(targetAngleY - cLib_targetAngleY(&cStack_2c, &current.pos))) < 0x4000) {
            mPathIdx += mPathIdxOffset;
            if (checkFlySceneChange()) {
                return true;
            }
        }
    }
    return false;
}

void daKago_c::executeAttack() {
    field_0x6dd = 1;

    if (field_0x6e7 == 1) {
        dComIfGp_setAStatusForce(0x2a, 0);
    } else {
        dComIfGp_setAStatusForce(0x27, 0);
    }

    checkHeight();

    mIsFlying = true;

    if (field_0x6e6 != 0) {
        setSibukiEffect();
        setDashSibukiEffect();
    }

    switch (field_0x744) {
    case 0:
        mDashCooldownTime = l_HIO.mDashCooldownTime;
        mDashTime = l_HIO.mDashTime;

        field_0x744 = 1;

        setDashBlurEffect(0);
        setBck(11, 0, 5.0f, l_HIO.mDashTimeMultiplier);

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
            cLib_chaseF(&field_0x6f8, 30.0f, l_HIO.mDashTimeMultiplier * 3.0f);
            if (mpMorf->checkFrame(9.0f)) {
                if (field_0x6e7 == 0) {
                    mSound.startCreatureSound(Z2SE_EN_YC_DASH, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_EN_KC_DASH, 0, -1);
                }
            }
            if (mpMorf->checkFrame(12.0f)) {
                field_0x744 = 2;
                field_0x728 = 0x14;

                dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartQuake(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

                mDashTime = l_HIO.mDashTime - 35.0f;

                field_0x6e6 = 0;
            }
        }
        break;
    case 2:
        setDashEffect();

        if (mpMorf->checkFrame(23.0f)) {
            mSph.OnAtSetBit();
        }

        if (calcAttackMove(1) == 0) {
            cLib_chaseF(&field_0x6f8, 100.0f, 5.0f);

            if (field_0x728 == 0) {
                field_0x744 = 3;
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
            cLib_chaseF(&field_0x6f8, 100.0f, 3.0f);
            if (checkWallHitFall(0)) {
                dComIfGp_getVibration().StopQuake(0x1f);

                mpLockActor = NULL;
            } else {
                mDashTime--;
                if (mDashTime == 0) {
                    field_0x744 = 4;

                    dComIfGp_getVibration().StopQuake(0x1f);
                }
            }
        }
        break;
    case 4:
        setDashEffect();
    case 5:
        if (calcAttackMove(0) == 0) {
            cLib_chaseF(&field_0x6f8, l_HIO.mFlightSpeed, 5.0f);
            if (field_0x6f8 <= l_HIO.mFlightSpeed) {
                mpLockActor = NULL;

                mSph.OffAtSetBit();

                setActionMode(0, 2);

                field_0x6c8 = 0;
                field_0x6c4 = 8000.0f;
                field_0x710 = field_0x712 = 0;
            }
        }
        break;
    }
}

void daKago_c::calcCircleCamera(int param_0) {
    cXyz cStack_20;

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::transM(0.0f, 0.0f, 1500.0f);
    mDoMtx_stack_c::multVecZero(&cStack_20);

    if (param_0 == 0) {
        field_0x68c = cStack_20;
    } else if (param_0 == 1) {
        cLib_addCalcPos(&field_0x68c, cStack_20, 0.1f, field_0x6f8 + 100.0f,
                        10.0f);
    } else {
        cLib_addCalcPos(&field_0x68c, cStack_20, 0.5f, field_0x6f8 + 30.0f,
                        10.0f);
    }
    mDoMtx_stack_c::transM(0.0f, 0.0f, -1000.0f);
    mDoMtx_stack_c::multVecZero(&cStack_20);

    if (param_0 == 0) {
        field_0x698 = cStack_20;
    } else if (param_0 == 1) {
        cLib_addCalcPos(&field_0x698, cStack_20, 0.1f, field_0x6f8 + 100.0f,
                        10.0f);
    } else {
        cLib_addCalcPos(&field_0x698, cStack_20, 0.5f, field_0x6f8 + 30.0f, 10.0f);
    }

    field_0x6d4 = 60.0f;
    dCam_getBody()->Set(field_0x68c, field_0x698, field_0x6d4, 0);
}

void daKago_c::executeEvent() {
    dCamera_c* camera = dCam_getBody();
    daPy_py_c* unusedPlayer = daPy_getPlayerActorClass();

    if ((field_0x73c == 0) && dComIfG_play_c::getLayerNo(0) == 13 && dComIfGs_isSaveSwitch(0x10)) {
        dComIfGs_onSaveSwitch(0x1a);
        setActionMode(4, 0);
    } else {
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(2);
        } else {
            camera->Stop();
            camera->SetTrimSize(3);
            field_0x748 = 0;
            field_0x74c = 0;
            setActionMode(3, 0);
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
    if (midna != 0) {
        midnaPos = midna->current.pos;
    }

    switch (field_0x748) {
    case 0: {
        Z2GetAudioMgr()->setDemoName(field_0x760);

        field_0x771 = 0;
        current.pos = dPath_GetPnt(mpPath2, field_0x771)->m_position;
        field_0x771++;
        if (field_0x771 >= mpPath2->m_num) {
            field_0x771 = 0;
        }

        cXyz cStack_48 = dPath_GetPnt(mpPath2, field_0x771)->m_position;
        s16 sVar5 = cLib_targetAngleY(&current.pos, &cStack_48);
        int local_80 = 2;
        if (local_80 >= (int)mpPath2->m_num) {
            local_80 = mpPath2->m_num - 1;
        }

        cXyz cStack_54 = dPath_GetPnt(mpPath2, local_80)->m_position;
        s16 temp = (s16)sVar5 - cLib_targetAngleY(&current.pos, &cStack_48);
        sVar5 = temp < 0 ? (s16)((s16)sVar5 - 0x1000) : (s16)((s16)sVar5 + 0x1000); // fakematch
        shape_angle.y = current.angle.y = sVar5;
        shape_angle.x = shape_angle.z = 0;

        speed.y = 0.0f;
        speedF = 20.0f;

        field_0x72c = 0xb4;
        field_0x728 = field_0x72c + 90;
        field_0x6e8 = 0;

        if ((field_0x73c == 0) && dComIfG_play_c::getLayerNo(0) == 13 && (playerPos.x > -90000.0f))
        {
            field_0x6e8 = 1;
        }

        calcCircleCamera(0);
        field_0x6e4 = 0;

        break;
    }
    case 1: {
        field_0x728 = 90;
        field_0x6f8 = 20.0f;
        break;
    }
    case 2: {
        if (field_0x73c != 0) {
            mPathIdx = searchNearPassPoint();
        }

        field_0x758 = 1;

        if (mPathIdxOffset < 0) {
            angleY = angleY + 0x8000;

            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&playerPos, angleY, 0);

            field_0x758 = 0xffffffff;
        }

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(angleY);
        mDoMtx_stack_c::transM(0.0f, 1000.0f, -1000.0f);
        mDoMtx_stack_c::multVecZero((Vec*)&current);

        setBck(0xc, 2, 0.0f, 1.0f);

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(angleY);
        mDoMtx_stack_c::transM(0.0f, 300.0f, -300.0f);
        mDoMtx_stack_c::multVecZero(&field_0x6a4);
        current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
        current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);

        field_0x6f8 = 20.0f;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;
        shape_angle.z = 0;

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(angleY);
        mDoMtx_stack_c::transM(-100.0f, 100.0f, -300.0f);
        mDoMtx_stack_c::multVecZero(&field_0x68c);
        mDoMtx_stack_c::transM(100.0f, -100.0f, 300.0f);
        mDoMtx_stack_c::transM(100.0f, 30.0f, 300.0f);
        mDoMtx_stack_c::multVecZero(&field_0x698);

        field_0x6d4 = 70.0f;

        dCam_getBody()->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        field_0x728 = 10;

        break;
    }
    case 3: {
        setBck(9, 2, 10.0f, 1.0f);

        setMidnaTagPos();

        speedF = speed.y = 0.0f;

        field_0x728 = 30;

        break;
    }
    case 4: {
        setBck(0xf, 2, 10.0f, 1.0f);

        if (field_0x73c == 0) {
            field_0x6a4.set(-90000.0f, -16000.0f, 40000.0f);
        } else {
            field_0x6a4 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
        }

        setMidnaRideOn();
        setPlayerRideOn();

        dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

        field_0x744 = 1;
        field_0x728 = 30;

        break;
    }
    }

    field_0x74c = 1;
    field_0x744 = 0;
}

bool daKago_c::executePerchDemo() {
    cXyz playerPos = daPy_getPlayerActorClass()->current.pos;
    s16 playerYaw = daPy_getPlayerActorClass()->shape_angle.y;
    cXyz unkXyz1;
    cXyz midnaPos;

    daMidna_c* midna = daPy_getPlayerActorClass()->getMidnaActor();
    s16 midnaYaw;
    if (midna != 0) {
        midnaPos = midna->current.pos;
        midnaYaw = midna->shape_angle.y;
    }

    field_0x750++;

    switch (field_0x748) {
    case 0: {
        calcCircleCamera(1);
        if ((field_0x750 & 0x20) != 0) {
            cLib_chaseF(&field_0x6fc, 3.0f, 0.3f);
        } else {
            cLib_chaseF(&field_0x6fc, -3.0f, 0.3f);
        }

        current.pos.y = current.pos.y + field_0x6fc;

        if (field_0x72c != 0) {
            cXyz cStack_64 = dPath_GetPnt(mpPath2, field_0x771)->m_position;
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
                if (field_0x6e7 == 1) {
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
            field_0x748 = 1;
            return true;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);

        break;
    }
    case 1: {
        calcCircleCamera(2);

        cLib_addCalcAngleS(&shape_angle.x, -cLib_targetAngleX(&current.pos, &playerPos), 4, 0x400, 0x40);
        cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY((Vec*)&current, &playerPos), 4, 0x200, 0x40);

        current.angle.y = shape_angle.y;
        current.angle.x = shape_angle.x;

        cLib_chaseF(&field_0x6f8, 60.0f, 1.0f);

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        if (current.pos.abs(playerPos) < 2000.0f) {
            field_0x748 = 2;
            return true;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 0);

        break;
    }
    case 2: {
        if (field_0x6e4 == 0) {
            if (field_0x73c == 0) {
                dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 4);
            } else if (field_0x73c == 2) {
                dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 5);
            }
        }

        if (field_0x744 == 0) {
            current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
            current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);

            speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
            speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

            if (field_0x728 != 0) {
                shape_angle.x = current.angle.x;
                if (field_0x728 == 1) {
                    if (field_0x6e7 == 1) {
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
                field_0x744 = 1;

                setBck(0x15, 2, 10.0f, 1.0f);

                field_0x728 = 60;

                mDoMtx_stack_c::transS(midnaPos);
                mDoMtx_stack_c::YrotM(playerYaw);
                mDoMtx_stack_c::transM(0.0f, 300.0f, -100.0f);
                mDoMtx_stack_c::multVecZero(&field_0x6a4);
            }
        } else {
            cLib_chaseF(&field_0x6f8, 0.0f, 1.0f);
            cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 4, 0x400, 0x100);
            cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x100, 0x80);
            cLib_addCalcAngleS(&shape_angle.x, 0, 4, 0x100, 0x80);

            shape_angle.y = current.angle.y;
            speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
            speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
            if (!field_0x6f8 && field_0x728 == 0) {
                field_0x748 = 3;
                return true;
            }
        }

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(0.0f, 400.0f, -300.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 5.0f, 5.0f);

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(100.0f, 30.0f, 300.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 20.0f, 10.0f);

        dCam_getBody()->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        break;
    }
    case 3: {
        if (field_0x6e4 == 0) {
            if (field_0x73c == 0) {
                dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 4);
            } else if (field_0x73c == 2) {
                dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 5);
            }
        }

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 20.0f, 10.0f);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(300.0f, 50.0f, 500.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 20.0f, 10.0f);

        cLib_chaseF(&field_0x6d4, 70.0f, 1.0f);

        dCam_getBody()->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        cLib_addCalcAngleS(&current.angle.x, 0, 4, 0x400, 0x100);

        shape_angle.x = current.angle.x;

        if (field_0x728 == 0) {
            field_0x748 = 4;
            return true;
        }

        break;
    }
    case 4: {
        if (field_0x6e4 == 0) {
            if (field_0x73c == 0) {
                dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 4);
            } else if (field_0x73c == 2) {
                dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 6);
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
        cLib_addCalcPos(&field_0x68c, midnaPos, 0.5f, maxStep, minStep);
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(playerYaw);
        mDoMtx_stack_c::transM(0.0f, 50.0f, -800.0f);
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, maxStep, minStep);
        cLib_chaseF(&field_0x6d4, 70.0f, 1.0f);
        dCam_getBody()->Set(field_0x68c, field_0x698, field_0x6d4, 0);

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

    switch (field_0x744) {
    case 0:
    case 1: {
        f32 unusedFloat1 = 20.0f;
        f32 unusedFloat2 = 10.0f;

        field_0x68c += speed;
        field_0x698 += speed;

        mDoMtx_stack_c::transS(midnaPos);
        mDoMtx_stack_c::YrotM(playerYaw);
        if (field_0x744 == 0) {
            mDoMtx_stack_c::transM(0.0f, 100.0f, 0.0f);
        } else {
            mDoMtx_stack_c::transM(0.0f, -200.0f, 0.0f);
        }
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 10.0f, 5.0f);

        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(playerYaw);
        if (field_0x744 == 0) {
            mDoMtx_stack_c::transM(300.0f, 50.0f, 500.0f);
        } else {
            mDoMtx_stack_c::transM(500.0f, 50.0f, 0.0f);
        }
        mDoMtx_stack_c::multVecZero(&unkXyz1);
        cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 10.0f, 5.0f);

        cLib_chaseF(&field_0x6d4, 70.0f, 1.0f);

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        mGroundHeight = checkGroundHeight(current.pos, NULL);
        mRoofHeight = checkRoofHeight(current.pos);

        cLib_chaseF(&field_0x6f8, 30.0f, 1.0f);

        cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x200, 0x80);
        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
        if (field_0x744 == 0) {
            cLib_addCalcAngleS(&shape_angle.x, 0x2000, 8, 0x200, 0x40);
        } else {
            cLib_addCalcAngleS(&shape_angle.x, 0, 8, 0x200, 0x40);
        }

        if (field_0x728 == 0x14 && field_0x744 == 0) {
            if (field_0x6e7 == 1) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 == 0) {
            if (field_0x744 == 0) {
                field_0x744 = 1;
                field_0x728 = 0x1e;

                setPlayerRideOn();

                dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            } else if (field_0x73c != 0) {
                field_0x728 = 60;
                field_0x744 = 2;
            } else {
                field_0x728 = 90;
                field_0x744 = 3;
                field_0x6cc = 1.0f;
            }
        }

        break;
    }
    case 2: {
        if (field_0x728 > 10) {
            field_0x68c += speed;
            field_0x698 += speed;

            s16 targetYaw = cLib_targetAngleY(&field_0x698, &field_0x68c);
            if (abs((s16)(midnaYaw - targetYaw)) < 0x2000) {
                cLib_addCalcPos(&field_0x68c, midnaPos, 0.5f, 5.0f, 10.0f);

                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(playerYaw);
                mDoMtx_stack_c::transM(0.0f, 50.0f, -800.0f);
                mDoMtx_stack_c::multVecZero(&unkXyz1);
                cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 20.0f, 10.0f);
            } else {
                cLib_addCalcPos(&field_0x68c, midnaPos, 0.5f, 5.0f, 10.0f);

                mDoMtx_stack_c::transS(current.pos);
                mDoMtx_stack_c::YrotM(playerYaw);
                mDoMtx_stack_c::transM(300.0f, 50.0f, -500.0f);
                mDoMtx_stack_c::multVecZero(&unkXyz1);
                cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 20.0f, 10.0f);
            }
        } else {
            cLib_addCalcPos(&field_0x68c, midnaPos, 0.5f, 50.0f, 10.0f);

            mDoMtx_stack_c::transS(current.pos);
            mDoMtx_stack_c::YrotM(playerYaw);
            mDoMtx_stack_c::transM(0.0f, 50.0f, -800.0f);
            mDoMtx_stack_c::multVecZero(&unkXyz1);
            cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 50.0f, 10.0f);
        }

        cLib_chaseF(&field_0x6d4, 70.0f, 1.0f);
        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        mGroundHeight = checkGroundHeight(current.pos, NULL);
        mRoofHeight = checkRoofHeight(current.pos);

        cLib_chaseF(&field_0x6f8, l_HIO.mFlightSpeed, 2.0f);

        if (field_0x728 < 30) {
            cLib_addCalcAngleS(&current.angle.x, 0, 8, 0x100, 0x40);
        } else {
            cLib_addCalcAngleS(&current.angle.x, 0xffffe000, 8, 0x100, 0x40);
        }

        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            if (field_0x73c == 2) {
                if (setSceneChange(3)) {
                    field_0x748 = 5;
                    field_0x6cc = 1.0f;
                }
            } else {
                field_0x6dc = 0;

                setActionMode(0, 0);

                camera->Reset(field_0x68c, field_0x698);
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

        field_0x68c += speed;
        field_0x698 += speed * field_0x6cc;

        unkXyz1.set(playerPos.x, playerPos.y + 100.0f, playerPos.z);
        cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 10.0f, 3.0f);

        field_0x6d4 = 70.0f;

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        cLib_chaseF(&field_0x6f8, 30.0f, 1.0f);

        cLib_addCalcAngleS(&current.angle.x, 0xffffe000, 8, 0x100, 0x40);
        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            setActionMode(7, 0);
            field_0x748 = 8;
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
        fopAcM_orderPotentialEvent(this,2,0xffff,3);
        eventInfo.onCondition(2);
    } else {
        camera->Stop();
        camera->SetTrimSize(3);
        field_0x748 = 0;
        field_0x74c = 0;
        setActionMode(7,0);
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
    if (midna != 0) {
        midnaPos = midna->current.pos;
    }

   s16 targetYaw;
    switch (field_0x748) {
    case 0: {
        Z2GetAudioMgr()->setDemoName(field_0x760);
        field_0x718 = -0x800;
        player->changeOriginalDemo();
        player->changeDemoMode(0x17, 0, 0, 0);
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
        field_0x68c = current.pos;
        field_0x6bc = 0x3000;
        field_0x6cc = 500.0f;
        field_0x6d0 = 400.0f;
        cStack_34.set(0.0f, field_0x6d0, field_0x6cc);
        cLib_offsetPos(&field_0x698, &current.pos, shape_angle.y + field_0x6bc,
                       &cStack_34);
        field_0x6d4 = 70.0f;
        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
        field_0x728 = 0xa0;
        setBck(15, 2, 10.0f, 1.0f);
        field_0x6e4 = 0;

        break;
    }
    case 1: {
        if (midna != NULL) {
            midna->current.pos = playerPos;
            midnaPos = midna->current.pos;
        }
        field_0x68c = current.pos;
        field_0x6bc = 0x800;
        field_0x6cc = 700.0f;
        cStack_34.set(0.0f, 0.0f, field_0x6cc);
        cLib_offsetPos(&field_0x698, &current.pos, shape_angle.y + field_0x6bc,
                       &cStack_34);
        field_0x6d4 = 70.0f;
        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
        field_0x728 = 0x50;

        break;
    }
    case 2: {
        field_0x728 = 60;
        mMsgFlow.init(this, 0x7d3, 0, NULL);

        break;
    }
    case 3: {
        field_0x718 = cLib_targetAngleX(&current.pos, &playerPos) / 4;
        targetYaw = cLib_targetAngleY(&current.pos, &playerPos) - shape_angle.y;
        field_0x71a = targetYaw / 4;
        cStack_34.set(50.0f, 150.0f, -200.0f);
        cLib_offsetPos(&field_0x698, &midnaPos, shape_angle.y, &cStack_34);
        cStack_34.set(50.0f, 0.0f, 50.0f);
        cLib_offsetPos(&field_0x68c, &midnaPos, shape_angle.y, &cStack_34);
        field_0x6d4 = 70.0f;
        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
        targetYaw = cLib_targetAngleY(&playerPos, (Vec*)&current);
        player->setPlayerPosAndAngle(&playerPos, targetYaw, 0);

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
        field_0x6f8 = 0.0f;

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
        field_0x728 = 0x96;
        field_0x6cc = 1.0f;

        break;
    }
    case 7: {
        field_0x6e8 = 0;
    }
    case 8: {
        setBck(15, 2, 10.0f, 1.0f);
        if (field_0x6e8 != 0) {
            mpMorf->setFrame(0.0f);
            field_0x6a4.set(-67000.0f, -13000.0f, 28000.0f);
            current.pos.set(-76600.0f, -15500.0f, 37340.0f);
            shape_angle.y = current.angle.y = -0x8000;
            shape_angle.x = current.angle.x = -0x1000;
            field_0x6f8 = 50.0f;
            field_0x698.set(-75232.0f, -14685.0f, 34417.0f);
            field_0x68c.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            field_0x6d4 = 70.0f;
            camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
            field_0x728 = 200;
        } else {
            field_0x6a4.set(-60000.0f, -11000.0f, 28000.0f);
            current.pos.set(-90000.0f, -16000.0f, 40000.0f);
            shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);
            shape_angle.x = current.angle.x = -0x2000;
            field_0x6f8 = 50.0f;
            cStack_34.set(-500.0f, 1000.0f, 5000.0f);
            cLib_offsetPos(&field_0x698, &current.pos, shape_angle.y, &cStack_34);
            field_0x68c.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
            field_0x6d4 = 70.0f;
            camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
            field_0x728 = 200;
        }

        break;
    }
    }

    field_0x74c = 1;
    field_0x744 = 0;
}

bool daKago_c::executeFirstDemo() {
    dCamera_c* camera = dCam_getBody();

    cXyz unkXyz1;
    cXyz unkXyz2;

    cXyz midnaPos;
    daPy_py_c* player = daPy_getPlayerActorClass();

    daMidna_c* midna = daPy_py_c::getMidnaActor();
    if (midna != 0) {
        midnaPos = midna->current.pos;
    }

    s16 playerYaw = player->shape_angle.y;
    cXyz playerPos = player->current.pos;

    int unkFlag1;
    int switchVal = field_0x748;
    switch (switchVal) {
    case 0:
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 2);

        cLib_chaseAngleS(&field_0x6bc, 0x1400, 0x40);
        cLib_chaseF(&field_0x6cc, 1250.0f, 6.7f);
        cLib_chaseF(&field_0x6d0, -400.0f, 7.1f);

        unkXyz2.set(0.0f, field_0x6d0, field_0x6cc);
        cLib_offsetPos(&field_0x698, &current.pos, shape_angle.y + field_0x6bc, &unkXyz2);

        unkXyz1.set(0.0f, -200.0f, 400.0f);
        cLib_chasePos(&field_0x680, unkXyz1, 4.0f);
        cLib_offsetPos(&field_0x68c, &current.pos, shape_angle.y, &field_0x680);

        field_0x6d4 = 70.0f;

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        if (field_0x728 < 10) {
            setMidnaTagPos();
            if (field_0x728 == 0) {
                field_0x748 = 1;
                return 1;
            }
        }

        break;
    case 1:
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 2);

        cLib_chaseAngleS(&field_0x6bc, 0x1000, 0x20);

        unkXyz2.set(0.0f, 0.0f, field_0x6cc);
        cLib_offsetPos(&field_0x698, &current.pos, shape_angle.y + field_0x6bc, &unkXyz2);

        if (field_0x728 < 0x46) {
            cLib_addCalcPos(&field_0x68c, midnaPos, 0.5f, 10.0f, 10.0f);
        } else {
            field_0x68c = current.pos;
        }

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
        setMidnaTagPos();

        if (field_0x728 == 30) {
            setBck(0x14, 0x02, 10.0f, 1.0f);
        }

        if (field_0x728 > 30) {
            cLib_chaseF(&field_0x6cc, 350.0f, 4.0f);
            cLib_addCalcAngleS(&field_0x718, 0xfffff800, 8, 0x100, 0x10);
        } else {
            cLib_chaseF(&field_0x6cc, 350.0f, 6.0f);
            cLib_addCalcAngleS(&field_0x718, 0x400, 8, 0x200, 0x10);

            if (field_0x728 == 30) {
                setMidnaRideOn();
            }

            if (field_0x728 == 0x14) {
                if (field_0x6e7 == 1) {
                    mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
                } else {
                    mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
                }
            }

            if (field_0x728 == 0) {
                field_0x748 = 2;

                return true;
            }
        }
        break;
    case 2:
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 3);

        setMidnaTagPos();
        cLib_addCalcPos(&field_0x68c, midnaPos, 0.5f, 10.0f, 10.0f);
        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        mMsgFlow.doFlow(this, NULL, 0);

        if (field_0x728 != 0) {
            if (field_0x728 == 1) {
                setBck(0xf, 0x02, 20.0f, 1.0f);
            }

            if (mMsgFlow.getNowMsgNo() == 0x1774) {
                setBck(0xf, 0x02, 20.0f, 1.0f);
                field_0x728 = 0;
            }
        }

        if (mMsgFlow.getNowMsgNo() == 0x1775) {
            field_0x748 = 3;

            return true;
        }

        break;
    case 3: {
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 3);

        setMidnaTagPos();
        cLib_addCalcAngleS(
            &field_0x718, cLib_targetAngleX(&current.pos, &playerPos) / 4,
            8, 0x100, 0x10);

        // adding braces to the switch case for this decl regresses debug match
        s16 targetYaw = cLib_targetAngleY(&current.pos, &playerPos) - shape_angle.y;
        cLib_addCalcAngleS(
            &field_0x71a, targetYaw / 4,
            8, 0x100, 0x10);

        if (mMsgFlow.doFlow(this, NULL, 0)) {
            field_0x748 = 4;

            return true;
        }

        break;
    }
    case 4:
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 2);

        cLib_addCalcAngleS(&field_0x718, 0, 8, 0x100, 0x10);
        cLib_addCalcAngleS(&field_0x71a, 0, 8, 0x100, 0x10);

        cLib_chaseF(&field_0x6cc, -300.0f, 10.0f);

        mDoMtx_stack_c::transS(playerPos);
        mDoMtx_stack_c::YrotM(field_0x6bc);
        mDoMtx_stack_c::transM(0.0f, 300.0f, field_0x6cc);
        mDoMtx_stack_c::multVecZero(&field_0x6a4);

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 4, 0x800, 0x100);
        cLib_addCalcAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x6a4), 4, 0x1000, 0x100);
        shape_angle.y = current.angle.y;
        shape_angle.x = current.angle.x;

        cLib_chaseF(&field_0x6f8, 20.0f, 1.0f);

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
        field_0x68c += speed;
        field_0x698 += speed;

        unkXyz2.set(0.0f, 200.0f, -500.0f);
        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &unkXyz2);
        cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 10.0f, 3.0f);
        unkXyz2.set(0.0f, 0.0f, 0.0f);

        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &unkXyz2);
        cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 10.0f, 3.0f);

        field_0x6d4 = 70.0f;

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        if (field_0x6a4.abs(current.pos) < 100.0f) {
            field_0x748 = 5;
            return true;
        }

        break;
    case 5:
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 2);

        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 4, 0x800, 0x100);
        cLib_addCalcAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x6a4), 4, 0x400, 0x100);
        shape_angle.y = current.angle.y;
        shape_angle.x = current.angle.x;

        cLib_chaseF(&field_0x6f8, 20.0f, 1.0f);
        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));

        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
        field_0x68c += speed;
        field_0x698 += speed;

        unkXyz2.set(400.0f, 50.0f, -300.0f);
        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &unkXyz2);
        cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 10.0f, 3.0f);

        unkXyz2.set(0.0f, 50.0f, 0.0f);
        cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &unkXyz2);
        cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 10.0f, 3.0f);

        field_0x6d4 = 70.0f;

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

        if (field_0x6a4.abs(current.pos) < 100.0f) {
            field_0x748 = 6;

            return true;
        }
        break;
    case 6:
        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 2);

        cLib_addCalcAngleS(&current.angle.x, 0xffffe000, 8, 0x100, 0x40);
        shape_angle.x = current.angle.x;
        shape_angle.y = current.angle.y;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        if (field_0x728 > 120) {
            field_0x68c += speed;
            field_0x698 += speed;

            unkXyz2.set(400.0f, 100.0f, -300.0f);
            cLib_offsetPos(&unkXyz1, &midnaPos, shape_angle.y, &unkXyz2);
            cLib_addCalcPos(&field_0x698, unkXyz1, 0.5f, 10.0f, 3.0f);

            unkXyz2.set(0.0f, 100.0f, 0.0f);
            cLib_offsetPos(&unkXyz1, &playerPos, shape_angle.y, &unkXyz2);
            cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 10.0f, 3.0f);
        } else {
            cLib_chaseF(&field_0x6cc, 0.0f, 0.1f);

            field_0x68c += speed;
            field_0x698 += speed * field_0x6cc;

            unkXyz2.set(0.0f, 100.0f, 0.0f);
            cLib_offsetPos(&unkXyz1, &playerPos, shape_angle.y, &unkXyz2);
            cLib_addCalcPos(&field_0x68c, unkXyz1, 0.5f, 10.0f, 3.0f);
        }

        field_0x6d4 = 70.0f;

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
        if (field_0x728 == 0) {
            field_0x748 = 7;

            return true;
        }

        break;
    case 7:
    case 8:
        unkFlag1 = 0;

        if (switchVal == 7 || field_0x6e8 == 0) {
            if (field_0x728 == 110) {
                unkFlag1 = 1;
            }
        } else if (switchVal == 8 && field_0x728 == 140) {
            unkFlag1 = 1;
        }

        if (unkFlag1 != 0) {
            if (field_0x6e7 == 1) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 != 0) {
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 1);
        } else {
            if (field_0x73c == 0 && setSceneChange(0)) {
                field_0x748 = 9;
            }
        }
    case 9:
        cLib_addCalcAngleS(&current.angle.y, cLib_targetAngleY(&current.pos, &field_0x6a4), 0x10, 0x100, 0x80);
        cLib_addCalcAngleS(&current.angle.x, -cLib_targetAngleX(&current.pos, &field_0x6a4), 0x10, 0x200, 0x80);
        shape_angle.y = current.angle.y;
        shape_angle.x = current.angle.x;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        unkXyz2.set(0.0f, 100.0f, 0.0f);
        cLib_offsetPos(&field_0x68c, &current.pos, shape_angle.y, &unkXyz2);

        field_0x6d4 = 70.0f;

        camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);

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

    switch (field_0x744) {
    case 0:
        current.pos.set(-74500.0f, -12775.0f, 31400.0f);

        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(2);
            return;
        }

        camera->Stop();
        camera->SetTrimSize(3);

        current.pos.set(-74500.0f, -12775.0f, 31400.0f);
        field_0x698.set(-79460.0f, -13000.0f, 34200.0f);
        field_0x68c = current.pos;

        field_0x6d4 = 70.0f;

        current.angle.x = 0;
        shape_angle.x = 0;
        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x698);
        field_0x716 = shape_angle.y + 0x2000;

        field_0x6a4.set(-77615.0f, -18500.0f, 41400.0f);
        field_0x6a4 += unkXyz2;

        field_0x744 = 1;
        field_0x712 = 0;
        field_0x728 = 150;
        field_0x6f8 = 50.0f;

        break;
    case 1:
        Z2GetAudioMgr()->setDemoName(field_0x760);
        field_0x744 = 2;
    case 2:
        if (field_0x728 == 0x8c) {
            if (field_0x6e7 == 0x01) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        field_0x68c = current.pos;

        cLib_chaseAngleS(&field_0x712, 0x80, 4);

        cLib_chaseAngleS(&shape_angle.y, field_0x716, field_0x712);
        current.angle.y = shape_angle.y;
        cLib_chaseAngleS(&shape_angle.x, 0x1000, 0x20);
        current.angle.x = shape_angle.x;

        field_0x6f8 = 50.0f;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            field_0x744 = 3;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 7);

        break;
    case 3:
        current.pos.set(-76600.0f, -15500.0f, 37340.0f);
        current.pos += unkXyz2;

        field_0x6a4.set(-77615.0f, -18500.0f, 41400.0f);
        field_0x6a4 += unkXyz2;

        field_0x698.set(-76900.0f, -18550.0f, 41660.0f);
        field_0x698 += unkXyz2;

        field_0x68c = current.pos;

        field_0x6d4 = 70.0f;

        shape_angle.x = current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);
        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);

        field_0x744 = 4;
        field_0x728 = 60;

        setBck(13, 2, 10.0f, 1.0f);
    case 4:
        field_0x68c = current.pos;
        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);
        if (field_0x728 == 0) {
            field_0x744 = 5;
            field_0x728 = 0x28;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 7);

        break;
    case 5:
        field_0x68c = current.pos;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, 0, 0x80);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 0) {
            setRideOff();

            field_0x744 = 6;
            field_0x728 = 100;

            setBck(0xf, 0x02, 10.0f, 1.0f);
        } else {
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 7);
        }

        field_0x6cc = 50.0f;
        field_0x6d0 = 20.0f;
        break;
    case 6:
        unkXyz1.set(-77875.0f, -18287.0f, 42000.0f);
        unkXyz1 += unkXyz2;
        cLib_chasePos(&field_0x68c, unkXyz1, field_0x6cc);
        if (field_0x68c.abs(unkXyz1) < 500.0f) {
            cLib_chaseF(&field_0x6cc, 0.0f, 3.0f);
        }

        unkXyz1.set(-77275.0f, -18500.0f, 41090.0f);
        unkXyz1 += unkXyz2;
        cLib_chasePos(&field_0x698, unkXyz1, field_0x6d0);
        if (field_0x698.abs(unkXyz1) < 200.0f) {
            cLib_chaseF(&field_0x6d0, 0.0f, 1.0f);
        } else {
            cLib_chaseF(&field_0x6d0, 20.0f, 1.0f);
        }

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, 0xffffe000, 0x100);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 90) {
            if (field_0x6e7 == 0x01) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 == 0) {
            setActionMode(4, 0);

            camera->Reset();
            camera->Start();
            camera->SetTrimSize(0);

            dComIfGp_event_reset();

            Z2GetAudioMgr()->setDemoName(NULL);

            return;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 8);
    }

    camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
}

void daKago_c::executeLandingBoartHouse() {
    dCamera_c* camera = dCam_getBody();
    cXyz unkXyz1;

    switch (field_0x744) {
    case 0: {
        current.pos.set(5750.0f, 1600.0f, 6100.0f);

        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(2);

            return;
        }

        camera->Stop();
        camera->SetTrimSize(3);

        field_0x744 = 10;

        break;
    }
    case 10: {
        Z2GetAudioMgr()->setDemoName(field_0x760);

        current.pos.set(5750.0f, 1600.0f, 6100.0f);
        setBck(0xf, 0x02, 10.0f, 1.0f);
        field_0x698.set(2900.0f, 300.0f, 500.0f);
        field_0x68c = current.pos;
        field_0x6d4 = 70.0f;
        field_0x6a4.set(3630.0f, 300.0f, 600.0f);

        shape_angle.x = current.angle.x = -cLib_targetAngleX(&current.pos, &field_0x6a4);
        shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &field_0x6a4);

        field_0x744 = 1;
        field_0x712 = 0;
        field_0x728 = 0x50;
        field_0x6f8 = 50.0f;
    }
    case 1: {
        if (field_0x728 == 0x46) {
            if (field_0x6e7 == 0x01) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        field_0x68c = current.pos;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        if (field_0x728 == 0) {
            field_0x744 = 2;
            field_0x728 = 0x28;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 9);

        break;
    }
    case 2: {
        field_0x68c = current.pos;

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, 0, 0x80);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 0) {
            setRideOff();
            field_0x744 = 3;
            field_0x728 = 90;
        } else {
            dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 9);
        }

        field_0x6cc = 40.0f;
        field_0x6d0 = 15.0f;

        break;
    }
    case 3: {
        unkXyz1.set(3440.0f, 500.0f, 400.0f);
        cLib_chasePos(&field_0x68c, unkXyz1, field_0x6cc);
        if (field_0x68c.abs(unkXyz1) < 500.0f) {
            cLib_chaseF(&field_0x6cc, 0.0f, 3.0f);
        }

        unkXyz1.set(3584.0f, 270.0f, 1007.0f);
        cLib_chasePos(&field_0x698, unkXyz1, field_0x6d0);
        if (field_0x698.abs(unkXyz1) < 200.0f) {
            cLib_chaseF(&field_0x6d0, 0.0f, 1.0f);
        } else {
            cLib_chaseF(&field_0x6d0, 15.0f, 1.0f);
        }

        speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
        speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

        cLib_chaseAngleS(&shape_angle.x, 0xffffe000, 0x100);
        current.angle.x = shape_angle.x;

        if (field_0x728 == 0x50) {
            if (field_0x6e7 == 0x01) {
                mSound.startCreatureVoice(Z2SE_EN_KC_V_NAKU, -1);
            } else {
                mSound.startCreatureVoice(Z2SE_EN_YC_V_NAKU, -1);
            }
        }

        if (field_0x728 == 0) {
            setActionMode(4, 0);

            if (field_0x6e7 == 0x01) {
                field_0x728 = 0x78;
            }

            camera->Reset();
            camera->Start();
            camera->SetTrimSize(0);

            dComIfGp_event_reset();

            Z2GetAudioMgr()->setDemoName(NULL);

            return;
        }

        dComIfGp_getEvent().setSkipProc(this, DemoSkipCallBack, 10);

        break;
    }
    }

    camera->Set(field_0x68c, field_0x698, field_0x6d4, 0);
}

void daKago_c::executeLanding() {
    if (this->field_0x73c == 0) {
        executeLandingLakeHairia();
    } else if (this->field_0x73c == 2) {
        executeLandingBoartHouse();
    } else {
        setRideOff();
        setActionMode(4, 0);
    }
}

void daKago_c::moveDemoFly() {
    if (field_0x744 == 0) {
        cLib_chaseF(&field_0x6f8, l_HIO.mFlightSpeed, 2.0f);
        if (!eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(this, 2, 0xffff, 3);
            eventInfo.onCondition(2);
        } else {
            setPlayerRideOn();

            mDashCooldownTime = (int)l_HIO.mDashCooldownTime;
            mDashTime = (int)l_HIO.mDashTime;
            field_0x744 = 1;
            setDashBlurEffect(0);
            setBck(11, 0, 5.0f, l_HIO.mDashTimeMultiplier);
            dComIfGp_getVibration().StartShock(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

            mpLockActor = NULL;

            mGroundHeight = checkGroundHeight(current.pos, NULL);
            mRoofHeight = checkRoofHeight(current.pos);

            if (mCurrentAction == 9) {
                dComIfGs_onSaveSwitch(9);

                mMsgFlow.init(this, 0xbbf, 0, NULL);
            }
        }
    } else {
        checkHeight();

        mIsFlying = true;

        if (mCurrentAction == 9) {
            mMsgFlow.doFlow(this, NULL, 0);
        }
        if (field_0x744 < 6) {
            setDashEffect();

            field_0x6dd = 0x01;

            dComIfGp_setAStatusForce(0x27, 0);
        }

        switch (field_0x744) {
        case 1: {
            Z2GetAudioMgr()->setDemoName(field_0x760);

            field_0x744 = 2;
        }
        case 2: {
            cLib_chaseF(&field_0x6f8, 30.0f, l_HIO.mDashTimeMultiplier * 3.0f);
            if (mpMorf->checkFrame(9.0f)) {
                if (field_0x6e7 == 0) {
                    mSound.startCreatureSound(Z2SE_EN_YC_DASH, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_EN_KC_DASH, 0, -1);
                }
            }
            if (mpMorf->checkFrame(12.0f)) {
                field_0x744 = 3;
                field_0x728 = 0x14;

                dComIfGp_getVibration().StartShock(2, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
                dComIfGp_getVibration().StartQuake(1, 0x1f, cXyz(0.0f, 1.0f, 0.0f));

                field_0x6e6 = 0;
            }

            break;
        }
        case 3:
        case 4: {
            cLib_chaseF(&field_0x6f8, 100.0f, 5.0f);
            if (field_0x728 == 0) {
                if (field_0x744 == 3) {
                    field_0x744 = 4;
                    field_0x728 = 0xf;
                } else {
                    field_0x744 = 5;
                    dComIfGp_getVibration().StopQuake(0x1f);
                }
            }

            break;
        }
        case 5: {
            cLib_chaseF(&field_0x6f8, l_HIO.mFlightSpeed, 5.0f);
            if (field_0x6f8 <= l_HIO.mFlightSpeed) {
                field_0x6c8 = 0;
                field_0x6c4 = 8000.0f;
                field_0x710 = field_0x712 = 0;

                if (dComIfGp_getStartStagePoint() == 0) {
                    field_0x72c = 30;
                } else {
                    field_0x72c = 60 + nREG_S(1);
                }

                shape_angle.y = current.angle.y;

                mGroundHeight = checkGroundHeight(current.pos, NULL);
                mRoofHeight = checkRoofHeight(current.pos);

                setFlyAnime();

                field_0x710 = field_0x712 = 0;
                field_0x744 = 6;
            }

            break;
        }
        case 6: {
            if (field_0x728 == 0) {
                setFlyAnime();
            }

            f32 fVar11 = 0.0f;
            if (cM_ssin(current.angle.x) > 0.0f) {
                fVar11 =
                    l_HIO.mFlightSpeed + l_HIO.mDescentRateIncrement * cM_ssin(current.angle.x);
            } else {
                fVar11 = l_HIO.mFlightSpeed + l_HIO.mAscentRateDecel * cM_ssin(current.angle.x);
            }

            if (std::abs(field_0x6f8 - fVar11) > 10.0f) {
                cLib_chaseF(&field_0x6f8, fVar11, 2.0f);
            } else {
                cLib_chaseF(&field_0x6f8, fVar11, 1.0f);
            }

            if (field_0x72c == 0) {
                setActionMode(0, 0);
                dComIfGp_event_reset();
                Z2GetAudioMgr()->setDemoName(NULL);
                return;
            }

            break;
        }
        }
    }

    cXyz unkXyz1 = dPath_GetPnt(mpPath1, mPathIdx)->m_position;
    cXyz unkXyz2 = dPath_GetPnt(mpPath1, mPathIdx - mPathIdxOffset)->m_position;
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

    speedF = field_0x6f8 * std::abs(cM_scos(current.angle.x));
    speed.y = -field_0x6f8 * cM_ssin(current.angle.x);

    f32 nextPath = checkNextPath(unkXyz1);
    if (nextPath < 0.0f) {
        if (abs((s16)(targetYaw - cLib_targetAngleY(&unkXyz2, &current.pos))) < 0x4000) {
            mPathIdx += this->mPathIdxOffset;
            if (checkFlySceneChange()) {
                return;
            }
        }
    }
}

void daKago_c::setFlyEffect() {
    if (field_0x6e7 == 1) {
        return;
    }

    J3DModel* model = mpMorf->getModel();
    JPABaseEmitter* emitter;

    field_0xb30 = dComIfGp_particle_set(field_0xb30, 0x8607, &current.pos, &tevStr);
    emitter = dComIfGp_particle_getEmitter(field_0xb30);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(model->getAnmMtx(13));
    }

    field_0xb34 = dComIfGp_particle_set(field_0xb34, 0x8607, &current.pos, &tevStr);
    emitter = dComIfGp_particle_getEmitter(field_0xb34);
    if (emitter != NULL) {
        emitter->setGlobalSRTMatrix(model->getAnmMtx(18));
    }
}

void daKago_c::setDashEffect() {
    J3DModel* model = mpMorf->getModel();
    JPABaseEmitter* emitter;

    if (field_0x6e7 == 0) {
        field_0xb38 = dComIfGp_particle_set(field_0xb38, 0x8609, &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(this->field_0xb38);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(13));
        }

        field_0xb3c = dComIfGp_particle_set(field_0xb3c, 0x860a, &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(field_0xb3c);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(18));
        }
    }

    setDashBlurEffect(1);
}

void daKago_c::setDamageEffect() {
    J3DModel* model = mpMorf->getModel();
    JPABaseEmitter* emitter;

    if (field_0x6e7 != 1) {
        field_0xb38 = dComIfGp_particle_set(field_0xb38, 0x8603, &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(this->field_0xb38);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(13));
        }

        field_0xb3c = dComIfGp_particle_set(field_0xb3c, 0x8603, &current.pos, &tevStr);
        emitter = dComIfGp_particle_getEmitter(field_0xb3c);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(model->getAnmMtx(18));
        }
    }
}

void daKago_c::setDashBlurEffect(int param_0) {
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

    local_94.set((s16)cLib_targetAngleX(&field_0x6b0, &cameraEye), (s16)cLib_targetAngleY(&field_0x6b0, &cameraEye), 0);
    if (l_HIO.mAngleTrackingMode == 0) {
        field_0x6be.x = -local_94.x;
        field_0x6be.y = local_94.y;
        field_0x6be.z = 0;
    } else {
        field_0x6be.x = shape_angle.x * (0.5f + XREG_F(0x13));
        field_0x6be.y = shape_angle.y;
        field_0x6be.z = 0;
    }
    field_0xb40 = dComIfGp_particle_set(field_0xb40, 0x860f, &cameraEyeOffset, &tevStr, &field_0x6be, 0, 0xff, 0, 0xffffffff, 0, 0, 0);
}

void daKago_c::setWallHitEffect(cXyz param_0, int param_1) {
    // might be a 2D array?
    static u16 kago_wall_hit_id[6] = {
        0x8658,
        0x86F9,
        0x86FA,
        0x8659,
        0x86FB,
        0x86FC,
    };

    for (int i = 0; i < 3; i++) {
        dComIfGp_particle_set(kago_wall_hit_id[i + param_1 * 3], &param_0, &tevStr, 0, 0);
    }
}

void daKago_c::setSibukiEffect() {
    static u16 kago_wave_id[3] = {
        0x865A,
        0x865B,
        0x865C,
    };

    cXyz acStack_28(current.pos.x, field_0x70c + l_HIO.mYOffsetFromWaterSurface, current.pos.z);
    csXyz cStack_30(0, shape_angle.y, 0);
    for (int i = 0; i < 3; i++) {
        field_0xb44[i] =
            dComIfGp_particle_set(field_0xb44[i], kago_wave_id[i], &acStack_28, &tevStr, &cStack_30,
                                  0, 0xff, 0, 0xffffffff, 0, 0, 0);
    }

    if (field_0x6e7 == 0) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YC_SPLASH, 0, -1);
    } else {
        mSound.startCreatureSoundLevel(Z2SE_EN_KC_SPLASH, 0, -1);
    }
}

void daKago_c::setDashSibukiEffect() {
    cXyz acStack_20(current.pos.x, field_0x70c + l_HIO.mYOffsetFromWaterSurface, current.pos.z);
    csXyz cStack_28(0, shape_angle.y, 0);
    field_0xb50 = dComIfGp_particle_set(field_0xb50, 0x86fe, &acStack_20, &tevStr, &cStack_28, 0,
                                        0xff, 0, 0xffffffff, 0, 0, 0);
}

void daKago_c::setWaterFallEffect() {
    static u16 kago_wave_id[2] = {
        0x865B,
        0x865C,
    };

    cXyz acStack_28(current.pos.x, current.pos.y, current.pos.z);
    csXyz cStack_30(0, shape_angle.y, 0);
    for (int i = 0; i < 2; i++) {
        field_0xb44[i] =
            dComIfGp_particle_set(field_0xb44[i], kago_wave_id[i], &acStack_28, &tevStr, &cStack_30,
                                  0, 0xff, 0, 0xffffffff, 0, 0, 0);
    }
    if (field_0x6e7 == 0) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YC_SPLASH, 0, -1);
    } else {
        mSound.startCreatureSoundLevel(Z2SE_EN_KC_SPLASH, 0, -1);
    }
}

static void* s_waterfall(void* param_0, void* param_1) {
    if (fopAcM_IsActor(param_0) && fopAcM_GetName(param_0) == PROC_Tag_WaterFall) {
        if (!fpcM_IsCreating(fopAcM_GetID(param_0))) {
            if (((daTagWaterFall_c*)param_0)
                    ->checkHitWaterFall(((fopAc_ac_c*)param_1)->current.pos))
            {
                ((daKago_c*)param_1)->onWaterFall();
                return param_0;
            }
        }
    }

    return NULL;
}

void daKago_c::setFlySound() {
    if (field_0x6de != 0) {
        BOOL unkFlag1 = FALSE;
        if (checkBck(9) || checkBck(15)) {
            if (field_0x6e7 == 0) {
                if (mpMorf->checkFrame(27.0f)) {
                    unkFlag1 = TRUE;
                }
            } else {
                if (mpMorf->checkFrame(29.0f)) {
                    unkFlag1 = TRUE;
                }
            }
        } else if (checkBck(21)) {
            if (mpMorf->checkFrame(10.0f)) {
                unkFlag1 = TRUE;
            }
        } else if (checkBck(6)) {
            if (mpMorf->checkFrame(26.0f)) {
                unkFlag1 = TRUE;
            }
        } else if (checkBck(5)) {
            if (mpMorf->checkFrame(10.0f)) {
                unkFlag1 = TRUE;
            }
        } else if (checkBck(4)) {
            if (mpMorf->checkFrame(10.0f) || mpMorf->checkFrame(35.0f)) {
                unkFlag1 = TRUE;
            }
        } else if (checkBck(12) || checkBck(13)) {
            if (field_0x6e7 == 0) {
                mSound.startCreatureSoundLevel(Z2SE_EN_YC_GLIDE, 0, -1);
            } else {
                mSound.startCreatureSoundLevel(Z2SE_EN_KC_GLIDE, 0, -1);
            }
        }

        if (unkFlag1) {
            if (field_0x6e7 == 0) {
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

    field_0x6f0 = mDoCPd_c::getStickX3D(0);
    field_0x6f4 = mDoCPd_c::getStickY(0);

    u32 unkInt1 = field_0x6ea;
    field_0x6ea = 0;
    fpcM_Search(s_waterfall, this);

    if (field_0x6ea != 0) {
        if (unkInt1 == 0) {
            current.angle.x = 0x1000;
            dComIfGp_getVibration().StartQuake(3, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
        }

        field_0x6f0 = field_0x6f0 * 0.5f;

        if (field_0x6f4 < 0.0f) {
            field_0x6f4 = field_0x6f4 * 0.5f;
        } else {
            field_0x6f4 = field_0x6f4 * 1.5f;
        }

        setWaterFallEffect();
    } else if (unkInt1 != 0) {
        dComIfGp_getVibration().StopQuake(0x1f);
    }

    field_0x6dd = 0;

    switch (mCurrentAction) {
    case 0:
        executeFly();
        break;
    case 1:
        executeStagger();
        break;
    case 2:
        executeEvent();
        break;
    case 3:
        executePerch();
        break;
    case 4:
        executeWait();
        break;
    case 5:
        executeAttack();
        break;
    case 6:
        executeEvent2();
        break;
    case 7:
        executePerch2();
        break;
    case 8:
        executeLanding();
        break;
    case 9:
        moveDemoFly();
        break;
    case 10:
        moveDemoFly();
        break;
    }

    if (field_0x6e7 == 1 && (mCurrentAction == 0 || mCurrentAction == 5)) {
        mSph.OnAtSetBit();
    }

    setFlySound();

    if (daPy_getPlayerActorClass()->checkCargoCarry()) {
        if (mCurrentAction == 9 || mCurrentAction == 10) {
            dCam_getBody()->SetTrimTypeForce(2);
        } else if (isAttack()) {
            dCam_getBody()->SetTrimTypeForce(0);
            dCam_getBody()->ModeFix(5);
        }
    }

    fopAcM_posMoveF(this, 0);
    if (field_0x6dc == 0) {
        mObjAcch.CrrPos(dComIfG_Bgsp());
    }

    if (mpMorf != NULL) {
        mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }

    field_0x674 = current.pos;

    if (cLib_chaseF(&field_0x6c4, 0.0f, 60.0f) == 0) {
        field_0x6c8 = field_0x6c8 + 0x800;
    }

    field_0x6ca = field_0x6c4 * cM_ssin(field_0x6c8);
}

void daKago_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x,current.pos.y,current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.mBasicSize,l_HIO.mBasicSize,l_HIO.mBasicSize);

    J3DModel* model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();

    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0x1c));
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
        dComIfGp_setAStatusForce(0x27, 0);
#endif
        return 1;
    }

    if (field_0x728 != 0) {
        field_0x728--;
    }

    if (field_0x72c != 0) {
        field_0x72c--;
    }

    if (field_0x720 != 0) {
        field_0x720--;
    }
    if (mDashCooldownTime != 0) {
        mDashCooldownTime--;
    }
    if (field_0x71c != 0) {
        field_0x71c--;
    }
    if (field_0x6ed != 0) {
        field_0x6ed--;
    }

    action();
    mtx_set();
    cc_set();

    field_0x6b0 = dCam_getBody()->Eye();

    return 1;
}

static int daKago_Execute(daKago_c* i_this) {
    return i_this->execute();
}

static int daKago_IsDelete(daKago_c* i_this) {
    return 1;
}

int daKago_c::_delete() {
    dComIfG_resDelete(&mPhase, field_0x75c);

    if (field_0xb54 != 0) {
        l_HIOInit = false;
        mDoHIO_DELETE_CHILD(l_HIO.mChild);
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daKago_Delete(daKago_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);
    return i_this->_delete();
}

int daKago_c::ctrlJoint(J3DJoint* param_0, J3DModel* param_1) {
    J3DJoint* joint = param_0;
    int jointNo = joint->getJntNo();

    mDoMtx_stack_c::copy(param_1->getAnmMtx(jointNo));
    if (jointNo == 3 || jointNo == 5 || jointNo == 6) {
        mDoMtx_stack_c::YrotM(field_0x71a);
        mDoMtx_stack_c::ZrotM(field_0x718);
    }

    param_1->setAnmMtx(jointNo,mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(),J3DSys::mCurrentMtx);

    return 1;
}

int daKago_c::JointCallBack(J3DJoint* param_0, int param_1) {
    if (param_1 == NULL) {
        J3DModel* model = j3dSys.getModel();
        daKago_c* kago = (daKago_c*)model->getUserArea();
        if (kago != NULL) {
            kago->ctrlJoint(param_0, model);
        }
    }
    return 1;
}

int daKago_c::CreateHeap() {
    J3DModelData* modelData;

    if (field_0x6e7 == 0) {
        modelData = (J3DModelData*)dComIfG_getObjectRes(field_0x75c, 0x18);
    } else {
        modelData = (J3DModelData*)dComIfG_getObjectRes(field_0x75c, 0x10);
    }
    JUT_ASSERT(0x139f, modelData != NULL);

    mpMorf = new mDoExt_McaMorfSO(
        modelData, NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes(field_0x75c, getBckName(9)), 2, 1.0f, 0, -1, &this->mSound, 0x80000, 0x11000084);

    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = mpMorf->getModel();
    model->setUserArea((uintptr_t)this);

    for (u16 i = 1; i < model->getModelData()->getJointNum(); i++) {
        if (i == 3 || i == 5 || i == 6) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
        }
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* param_0) {
    daKago_c* kago = (daKago_c*)param_0;
    return kago->CreateHeap();
}

int daKago_c::create() {
    fopAcM_ct(this, daKago_c);

    u32 param = (u8)fopAcM_GetParam(this);
    if ((param & 0xff) != 0xff) {
        mpPath1 = dPath_GetRoomPath(param & 0xff, fopAcM_GetRoomNo(this));
        if (mpPath1 == NULL) {
            OS_REPORT("KAGO ......NONONONONONO PATH !!!!\n");
            return 5;
        }
    }

    field_0x6e7 = (fopAcM_GetParam(this) >> 16) & 0xff;
    if (field_0x6e7 == 0xff) {
        field_0x6e7 = 0;
    }
    if (field_0x6e7 == 0) {
        field_0x75c = "E_YC";
        field_0x760 = "KAGO_YAMI";
    } else {
        field_0x75c = "E_KC";
        field_0x760 = "KAGO_HIKARI";
    }

    int phase_state = dComIfG_resLoad(&mPhase, field_0x75c);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("Kago PARAM %x \n", fopAcM_GetParam(this));

        if (fopAcM_entrySolidHeap(this, useHeapInit, 0x23a0) == 0) {
            return cPhs_ERROR_e;
        }

        if (l_HIOInit == 0) {
            field_0xb54 = 1;
            l_HIOInit = 1;
            l_HIO.mChild = mDoHIO_CREATE_CHILD("", &l_HIO);
        }

        attention_info.flags = 0;

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
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
        mPathIdxOffset = 0x01;

        setActionMode(4, 0);

#if DEBUG
        if (mDoCPd_c::getHoldL(0) && mDoCPd_c::getHoldR(0)) {
            dComIfGs_offSwitch(0x2d, fopAcM_GetRoomNo(this));
            dComIfGs_offSwitch(0x32, fopAcM_GetRoomNo(this));

            setActionMode(8, 0);
            setMidnaRideOn();
            setPlayerRideOn();

            field_0x6dc = 0;
            field_0x6de = 0xff;
            field_0x6df = 1;
        }
#endif

        field_0x73c = 3;
        if (strcmp("F_SP115", dComIfGp_getStartStageName()) == 0) {
            if (dComIfGp_getStartStageRoomNo() == 0) {
                field_0x73c = 0;
            }
        } else if (strcmp("F_SP112", dComIfGp_getStartStageName()) == 0) {
            if (dComIfGp_getStartStageRoomNo() == 1) {
                field_0x73c = 1;
            }
        } else if (strcmp("F_SP126", dComIfGp_getStartStageName()) == 0 &&
                   dComIfGp_getStartStageRoomNo() == 0)
        {
            field_0x73c = 2;
        }

        mBalloon2DId = fpcM_ERROR_PROCESS_ID_e;

        mtx_set();

        daKago_Execute(this);
    }

    return phase_state;
}

static int daKago_Create(daKago_c* param_0) {
    return param_0->create();
}

static actor_method_class l_daKago_Method = {
    (process_method_func)daKago_Create,
    (process_method_func)daKago_Delete,
    (process_method_func)daKago_Execute,
    (process_method_func)daKago_IsDelete,
    (process_method_func)daKago_Draw,
};

extern actor_process_profile_definition g_profile_KAGO = {
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
