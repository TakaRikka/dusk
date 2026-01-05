#if DEBUG

#include "d/d_debug_camera.h"
#include "d/d_event_data.h"
#include "d/d_debug_viewer.h"
#include "d/d_com_inf_game.h"
#include "f_ap/f_ap_game.h"
#include "JSystem/JHostIO/JORServer.h"
#include "JSystem/JUtility/JUTDbPrint.h"
#include "JSystem/JUtility/JUTReport.h"

namespace zevwork {
    const int CutMaxNum = 32;
    const int DataMaxNum = 0x100;
    const int FDataMaxNum = 0x200;
    const int IDataMaxNum = 0x100;
    const int SDataMaxNum = 0x100;

    const int WorkSize = (sizeof(event_binary_data_header) - 8) +
                         sizeof(dEvDtEvent_DBG_c) +
                         sizeof(dEvDtStaff_DBG_c) +
                         sizeof(dEvDtStaff_DBG_c) +
                         (sizeof(dEvDtCut_c) * CutMaxNum) +
                         (sizeof(dEvDtData_DBG_c) * DataMaxNum) +
                         (sizeof(f32) * FDataMaxNum) +
                         (sizeof(int) * IDataMaxNum) +
                         (sizeof(char) * SDataMaxNum);
    u8* WorkBuffer;
};

dDbgCamera_c dDbgCamera;

dDbgCamKey_c::~dDbgCamKey_c() {}

namespace {
cXyz rel2wld(cXyz* param_0, fopAc_ac_c* actor) {
    if (actor == NULL) {
        OS_REPORT("debug camera: error: actor NULL in rel2wld() \n");
        return cXyz::Zero;
    }

    cSGlobe sp18(*param_0);
    sp18.U(cSAngle(actor->shape_angle.y) + sp18.U());

    return actor->attention_info.position + sp18.Xyz();
}

cXyz wld2rel(cXyz* param_0, fopAc_ac_c* actor) {
    if (actor == NULL) {
        OS_REPORT("debug camera: error: actor NULL in wld2rel() \n");
        return cXyz::Zero;
    }

    cSGlobe sp18(*param_0 - actor->attention_info.position);
    sp18.U(sp18.U() - cSAngle(actor->shape_angle.y));

    return sp18.Xyz();
}

inline fpc_ProcID ActorID(fopAc_ac_c* actor) {
    return fopAcM_GetID(actor);
}

inline fopAc_ac_c* Actor(fpc_ProcID id) {
    return fopAcM_SearchByID(id);
}
}

dDbgCamSetup_c::dDbgCamSetup_c() {
    mpDbgCam = NULL;

    mMoveSpeed = 20.0f;
    mMoveTurbo = 400.0f;
    mFovyTick = 2.5;
    mMoveParallelShift = 10.0f;
    mRotateSpeed = 20.0f;
    mTransAccel = 0.1f;
    mRotAccel = 0.01;
    mMoveMax = 100.0f;
    mUnkMax = 2000.0f;
    mRotateMax = 30.0f;

    mCenterCross = 400.0f;
    mCenterBall = 20.0f;
    mCenterMarker = 1;

    mActorSrchType = 0;
    mActorID = fpcM_ERROR_PROCESS_ID_e;

    mFlag = 0;
    mFlag |= (u16)0x8000;

    mPosX = 40;
    mPosY = 320;

    mGazePoint.x = 0.0f;
    mGazePoint.y = 0.0f;
    mGazePoint.z = 0.0f;

    mPerspective.x = 0.0f;
    mPerspective.y = 0.0f;
    mPerspective.z = 0.0f;

    mAngle = 0.0f;

    field_0x68.set(0.0f, 1.0f, 0.0f);
}

void dDbgCamSetup_c::Init(dDbgCamera_c* i_dbgCamera) {
    // "Debug Camera"
    m_childNo = mDoHIO_CREATE_CHILD("デバッグカメラ", this);
    mpDbgCam = i_dbgCamera;
}

void dDbgCamSetup_c::Finish() {
    mDoHIO_DELETE_CHILD(m_childNo);
}

f32 dDbgCamSetup_c::Translate(f32 param_0) {
    f32 var_f31 = mMoveSpeed * param_0;

    f32 var_f30;
    if (var_f31 > mMoveMax) {
        var_f30 = mMoveMax;
    } else {
        var_f30 = var_f31;
    }

    return var_f30;
}

s16 dDbgCamSetup_c::Rotate(f32 param_0) {
    f32 var_f31 = mRotateSpeed * param_0;

    f32 var_f30;
    if (var_f31 > mRotateMax) {
        var_f30 = mRotateMax;
    } else {
        var_f30 = var_f31;
    }

    return cAngle::d2s(var_f30);
}

f32 dDbgCamSetup_c::FlatMove(f32 param_0) {
    f32 var_f31 = mMoveParallelShift * param_0;

    f32 var_f30;
    if (var_f31 > mMoveMax) {
        var_f30 = mMoveMax;
    } else {
        var_f30 = var_f31;
    }

    return var_f30;
}

f32 dDbgCamSetup_c::Turbo(f32 param_0) {
    f32 var_f31 = mMoveTurbo * param_0;

    f32 var_f30;
    if (var_f31 > mUnkMax) {
        var_f30 = mUnkMax;
    } else {
        var_f30 = var_f31;
    }

    return var_f30;
}

static int search_actor(fopAc_ac_c* actor, void* data) {
    dDbgCamSetup_c* camsetup = (dDbgCamSetup_c*)data;

    cXyz sp18 = dDbgCamera.Center() - actor->attention_info.position;
    if (sp18.abs() < camsetup->mActorSrchRange) {
        camsetup->mActorSrchRange = sp18.abs();
        camsetup->mActorID = ActorID(actor);
    }

    return 0;
}

void dDbgCamSetup_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("- カメラ位置表示", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" 現在位置", &mFlag, 4, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" (補正前)", &mFlag, 8, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("- ヒットポリゴン表示", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" 壁あたり", &mFlag, 0x10, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("- 移動", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" スピード", &mMoveSpeed, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("     最大", &mMoveMax, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" ターボ", &mMoveTurbo, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("     最大", &mUnkMax, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" 平行移動", &mMoveParallelShift, 0.0f, 5000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" 加速度", &mTransAccel, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("- 回転", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" スピード", &mRotateSpeed, 0.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("     最大", &mRotateMax, 0.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" 加速度", &mRotAccel, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("- 画角", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" スピード", &mFovyTick, 0.0f, 90.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("- センターマーカ", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" 線の長さ", &mCenterCross, 0.0f, 4000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider(" 球の大きさ", &mCenterBall, 0.0f, 4000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->startRadioButton("表示", &mCenterMarker, 0, NULL, 0xFFFF, 0xFFFF, 0x100, 0xFFFF);
    mctx->genRadioButtonItem("常に表示", 3, 0, 0xFFFF, 0xFFFF, 0x80, 0x10);
    mctx->genRadioButtonItem("動くと表示", 1, 0, 0xFFFF, 0xFFFF, 0x80, 0x10);
    mctx->genRadioButtonItem("常に非表示", 0, 0, 0xFFFF, 0xFFFF, 0x80, 0x10);
    mctx->endRadioButton();

    mctx->genLabel("- 情報表示", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genCheckBox(" 有効", &mFlag, 0x8000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("   表示位置 Ｘ", &mPosX, 0, FB_WIDTH, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("            Ｙ", &mPosY, 0, 480, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("- 座標系", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->startRadioButton("基準", &mActorSrchType, 0, NULL, 0xFFFF, 0xFFFF, 0x100, 0xFFFF);
    mctx->genRadioButtonItem("絶対座標", 0, 0, 0xFFFF, 0xFFFF, 0x80, 0x10);
    mctx->genRadioButtonItem("プレイヤー相対座標", 1, 0, 0xFFFF, 0xFFFF, 0x80, 0x10);
    mctx->genRadioButtonItem("指定アクター相対座標", 2, 0, 0xFFFF, 0xFFFF, 0x80, 0x10);
    mctx->endRadioButton();
    mctx->genButton(" 更新 ", 0x66, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("- イベント", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton(" 保存 ", 0x68, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);

    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("- 座標指定", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("   注視点 Ｘ", &mGazePoint.x, -100000.0f, 100000.0f, 0, NULL, 0xFFFF, 0xFFFF,
                    0x200, 0x18);
    mctx->genSlider("   　　　 Ｙ", &mGazePoint.y, -100000.0f, 100000.0f, 0, NULL, 0xFFFF,
                    0xFFFF, 0x200, 0x18);
    mctx->genSlider("   　　　 Ｚ", &mGazePoint.z, -100000.0f, 100000.0f, 0, NULL, 0xFFFF,
                    0xFFFF, 0x200, 0x18);
    mctx->genButton(" 更新 ", 0x69, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("  視点 　 Ｘ", &mPerspective.x, -100000.0f, 100000.0f, 0, NULL, 0xFFFF,
                    0xFFFF, 0x200, 0x18);
    mctx->genSlider("   　　　 Ｙ", &mPerspective.y, -100000.0f, 100000.0f, 0, NULL,
                    0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("   　　　 Ｚ", &mPerspective.z, -100000.0f, 100000.0f, 0, NULL,
                    0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton(" 更新 ", 0x6A, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genSlider("   画角 　　", &mAngle, 0.0f, 180.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genButton(" 更新 ", 0x6B, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    mctx->genLabel("-", 0, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
}

void dDbgCamera_c::InitlChk() {
    mLchkNum = 0;
}

int dDbgCamera_c::SetlChk(dBgS_LinChk lchk) {
    if (mLchkNum <= 1) {
        mLchk[mLchkNum] = lchk;
        return ++mLchkNum;
    }

    return -1;
}

void dDbgCamSetup_c::listenPropertyEvent(const JORPropertyEvent* property) {
    int var_r27 = 0;
    JORReflexible::listenPropertyEvent(property);
    JORMContext* mctx = attachJORMContext(8);

    switch ((u32)property->id) {
    case 0x66:
        mActorSrchRange = 10000000.0f;
        mActorID = fpcM_ERROR_PROCESS_ID_e;

        switch (mActorSrchType) {
        case 1: {
            fopAc_ac_c* player = dComIfGp_getPlayer(dComIfGp_getCameraPlayer1ID(0));
            mActorID = ActorID(player);
            break;
        }
        case 2:
            fopAcIt_Executor((fopAcIt_ExecutorFunc)search_actor, this);
            break;
        }
        break;
    case 0x68:
        mpDbgCam->WriteZevData();
        break;
    case 0x69:
        if (mActorID == fpcM_ERROR_PROCESS_ID_e) {
            mpDbgCam->Center(mGazePoint);
        } else {
            mpDbgCam->Center(wld2rel(&mGazePoint, Actor(mActorID)));
        }
        break;
    case 0x6A:
        if (mActorID == fpcM_ERROR_PROCESS_ID_e) {
            mpDbgCam->Eye(mPerspective);
        } else {
            mpDbgCam->Eye(wld2rel(&mPerspective, Actor(mActorID)));
        }
        break;
    case 0x6B:
        mpDbgCam->Fovy(mAngle);
        break;
    }
}

int dDbgCamera_c::moveTool() {
    f32 substick_x = mDoCPd_c::getSubStickX3D(mPadNo);
    f32 substick_y = mDoCPd_c::getSubStickY(mPadNo);
    int sp4C = field_0xc00;

    if (mDoCPd_c::getHoldA(mPadNo)) {
        if (field_0xc0c != 0) {
            field_0xc00 = 6;
        } else {
            field_0xc00 = 0;
        }
    } else if (mDoCPd_c::getHoldB(mPadNo)) {
        if (field_0xc0c != 0) {
            field_0xc00 = 7;
        } else {
            field_0xc00 = 1;
        }
    } else if (mDoCPd_c::getSubStickValue(mPadNo) > 0.001f) {
        field_0xc00 = 3;
    } else {
        field_0xc00 = 18;
    }

    if (field_0xc00 != 18) {
        if (field_0xc00 != sp4C) {
            field_0xc10 = 0.1f;
        } else {
            field_0xc10 += field_0xc10 * mCamSetup.TransAccel();
        }
    } else {
        field_0xc10 = 0.0f;
    }

    int sp48 = field_0xc04;
    f32 stick_x = mDoCPd_c::getStickX3D(mPadNo);
    f32 stick_y = mDoCPd_c::getStickY(mPadNo);

    if (mDoCPd_c::getTrigY(mPadNo)) {
        field_0xc04 = 16;
    } else if (mDoCPd_c::getTrigX(mPadNo)) {
        field_0xc04 = 17;
    } else if (mDoCPd_c::getTrigLeft(mPadNo)) {
        if (field_0x54 == 0) {
            field_0xc04 = 4;
        } else if (field_0x54 == 1) {
            field_0xc04 = 10;
        } else if (field_0x54 == 2) {
            field_0xc04 = 14;
        }
        field_0xc1c = 0;
    } else if (mDoCPd_c::getHoldLeft(mPadNo)) {
        if (field_0xc1c++ > 20) {
            if (field_0x54 == 0) {
                field_0xc04 = 4;
            } else if (field_0x54 == 1) {
                field_0xc04 = 10;
            } else if (field_0x54 == 2) {
                field_0xc04 = 14;
            }
        } else {
            field_0xc04 = 18;
        }
    } else if (mDoCPd_c::getTrigRight(mPadNo)) {
        if (field_0x54 == 0) {
            field_0xc04 = 5;
        } else if (field_0x54 == 1) {
            field_0xc04 = 11;
        } else if (field_0x54 == 2) {
            field_0xc04 = 15;
        }
        field_0xc1c = 0;
    } else if (mDoCPd_c::getHoldRight(mPadNo)) {
        if (field_0xc1c++ > 20) {
            if (field_0x54 == 0) {
                field_0xc04 = 5;
            } else if (field_0x54 == 1) {
                field_0xc04 = 11;
            } else if (field_0x54 == 2) {
                field_0xc04 = 15;
            }
        } else {
            field_0xc04 = 18;
        }
    } else if (mDoCPd_c::getStickValue(mPadNo) > 0.001f) {
        field_0xc04 = 8;
    } else {
        field_0xc04 = 18;
    }

    if (field_0xc04 != 18) {
        if (field_0xc04 != sp48) {
            field_0xc14 = 0.1f;
        } else {
            field_0xc14 += field_0xc14 * mCamSetup.RotAccel();
        }
    } else {
        field_0xc14 = 0.0f;
    }

    switch (field_0xc04) {
    case 12:
        if (mRunKey.mAction == dDbgCam_KeyAction_FIXEDFRM) {
            mRunKey.mAction = dDbgCam_KeyAction_NOACTION;
        } else {
            mRunKey.mAction--;
        }
        break;
    case 13:
        if (mRunKey.mAction == dDbgCam_KeyAction_NOACTION) {
            mRunKey.mAction = dDbgCam_KeyAction_FIXEDFRM;
        } else {
            mRunKey.mAction++;
        }
        break;
    case 14:
        if (field_0x58 > 0) {
            field_0x58--;
            if (mActionKey[field_0x58] != '-') {
                mRunKey = mKeys[field_0x58];
                if (mKeys[field_0x58].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                    fopAc_ac_c* sp3C = Actor(mRunKey.mActorInfo.id);
                    if (sp3C != NULL) {
                        mRunKey.mCenter = rel2wld(&mKeys[field_0x58].mCenter, sp3C);
                        mRunKey.mEye = rel2wld(&mKeys[field_0x58].mEye, sp3C);
                    }
                }
            }
        }
        break;
    case 15:
        if (field_0x58 < 31) {
            field_0x58++;
            if (mActionKey[field_0x58] != '-') {
                mRunKey = mKeys[field_0x58];
                if (mKeys[field_0x58].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                    fopAc_ac_c* sp38 = Actor(mRunKey.mActorInfo.id);
                    if (sp38 != NULL) {
                        mRunKey.mCenter = rel2wld(&mKeys[field_0x58].mCenter, sp38);
                        mRunKey.mEye = rel2wld(&mKeys[field_0x58].mEye, sp38);
                    }
                }
            }
        }
        break;
    case 16:
        if (mRunKey.mAction == dDbgCam_KeyAction_NOACTION) {
            OSReport("        <data name=\"Center\" type=\"xyz\">");
            OSReport("%.3f %.3f %.3f</data>\n", mRunKey.mCenter.x, mRunKey.mCenter.y, mRunKey.mCenter.z);
            OSReport("        <data name=\"Eye\" type=\"xyz\">");
            OSReport("%.3f %.3f %.3f</data>\n", mRunKey.mEye.x, mRunKey.mEye.y, mRunKey.mEye.z);
            OSReport("        <data name=\"Fovy\" type=\"float\">");
            OSReport("%.3f</data>\n", mRunKey.mFovy);
            OSReport("        <data name=\"Bank\" type=\"float\">");
            OSReport("%.3f</data>\n", mRunKey.mBank);
        }

        if (mActionKey[field_0x58] != '-' || mRunKey.mAction == dDbgCam_KeyAction_NOACTION) {
            int var_r27 = field_0x58;
            for (; var_r27 < 31; var_r27++) {
                if (mActionKey[var_r27] == '-') {
                    break;
                }
            }

            while (var_r27 > field_0x58) {
                mActionKey[var_r27] = mActionKey[var_r27 - 1];
                mKeys[var_r27] = mKeys[var_r27 - 1];
                var_r27--;
            }
        }

        mActionKey[field_0x58] = ActionSymbol(mRunKey.mAction);
        mKeys[field_0x58] = mRunKey;

        if (mKeys[field_0x58].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
            fopAc_ac_c* actor = Actor(mRunKey.mActorInfo.id);
            if (actor != NULL) {
                mKeys[field_0x58].mCenter = wld2rel(&mRunKey.mCenter, actor);
                mKeys[field_0x58].mEye = wld2rel(&mRunKey.mEye, actor);
            }
        }

        if (mActionKey[field_0x58 + 1] == '-') {
            field_0x58++;
        }
        break;
    case 17:
        if (mActionKey[field_0x58] != '-' && (mActionKey[field_0x58 + 1] != '-' || mRunKey.mAction == dDbgCam_KeyAction_NOACTION)) {
            int var_r29 = field_0x58 + 1;
            for (; var_r29 < 33; var_r29++) {
                if (var_r29 == 0x20) {
                    mActionKey[var_r29 - 1] = '-';
                    mKeys[var_r29 - 1] = PlaneKey();
                } else {
                    mActionKey[var_r29 - 1] = mActionKey[var_r29];
                    mKeys[var_r29 - 1] = mKeys[var_r29];
                }

                if (mActionKey[var_r29] == '-') {
                    break;
                }
            }
        } else {
            mActionKey[field_0x58] = '-';
            mKeys[field_0x58] = PlaneKey();
        }

        mRunKey.mCenter = mKeys[field_0x58].mCenter;
        break;
    }

    if (mCmdMode == 3) {
        fopAc_ac_c* sp2C = dComIfGp_getPlayer(0);
        /* if (this->unk730 != 0x3E7) {
            &field_0x704 = &sp2C->unk558
            this->unk730 = 0x3E7;
        } else {
            (sp15C, (Vec* ) &sp2C->unk558);
            &mRunKey.mCenter += sp15C
            &field_0x704 = &sp2C->unk558
        } */
    } else {
        // this->unk730 = 3;
    }

    cXyz* var_r28;
    cXyz* sp30;
    f32 temp_f31;
    f32 var_f30;
    f32 var_f29;
    if (field_0xc08 != 0) {
        var_r28 = &mRunKey.mCenter;
        sp30 = &mRunKey.mEye;
        var_f30 = -1.0f;
        var_f29 = 1.0f;
    } else {
        var_r28 = &mRunKey.mEye;
        sp30 = &mRunKey.mCenter;
        var_f30 = 1.0f;
        var_f29 = -1.0f;
    }

    field_0x44.Val(*sp30 - *var_r28);

    switch (field_0xc00) {
    f32 temp_f26;
    f32 temp_f1_2;
    case 2:
        *var_r28 = *var_r28 + (field_0x44.Norm() * mCamSetup.Turbo(field_0xc10 * var_f30));

        if (field_0xc0c != 0) {
            *var_r28 = *var_r28 + (field_0x44.Norm() * mCamSetup.Turbo(field_0xc10 * var_f30));
        }
        break;
    case 0:
        *var_r28 = *var_r28 + (field_0x44.Norm() * mCamSetup.Translate(field_0xc10 * var_f30));
        break;
    case 1:
        *var_r28 = *var_r28 - (field_0x44.Norm() * mCamSetup.Translate(field_0xc10 * var_f30));
        break;
    case 3: {
        temp_f26 = fabsf(substick_x);
        temp_f1_2 = fabsf(substick_y);

        if (field_0xc0c == 0 || temp_f26 < temp_f1_2) {
            cXyz sp1B0(0.0f, 1.0f, 0.0f);
            *var_r28 = *var_r28 + (sp1B0 * mCamSetup.FlatMove(field_0xc10 * substick_y));
        }

        if (field_0xc0c == 0 || temp_f26 > temp_f1_2) {
            cSGlobe sp58(1.0f, cSAngle::_0, field_0x44.U() + cSAngle::_90);
            temp_f31 = var_f30 * (field_0xc10 * substick_x);
            *var_r28 = *var_r28 - (sp58.Xyz() * mCamSetup.FlatMove(temp_f31));
        }
        break;
    }
    case 6: {
        temp_f31 = field_0x44.R();
        field_0x44.R(temp_f31 - (0.1f * (temp_f31 * field_0xc10)));

        if (field_0x44.R() < 10.0f) {
            field_0x44.R(10.0f);
        }
        break;
    }
    case 7:
        temp_f31 = field_0x44.R();
        field_0x44.R(temp_f31 + (0.1f * (temp_f31 * field_0xc10)));
        break;
    }

    if (mRunKey.mActorInfo.id != mCamSetup.mActorID) {
        fopAc_ac_c* actor = Actor(mCamSetup.mActorID);
        if (actor != NULL) {
            mRunKey.mActorInfo.id = mCamSetup.mActorID;
            fopAcM_getNameString(actor, mRunKey.mActorInfo.name);
        } else {
            mRunKey.mActorInfo.id = mCamSetup.mActorID = fpcM_ERROR_PROCESS_ID_e;
        }
    }

    u8 sp8 = 1;
    switch (field_0xc04) {
    case 4:
        mRunKey.mFovy -= 0.1f * mCamSetup.FovyTick();
        if (mRunKey.mFovy < 1.0f) {
            mRunKey.mFovy = 1.0f;
        }
        break;
    case 5:
        mRunKey.mFovy += 0.1f * mCamSetup.FovyTick();
        if (mRunKey.mFovy > 179.0f) {
            mRunKey.mFovy = 179.0f;
        }
        break;
    case 10:            
        mRunKey.mBank += cDegree(mCamSetup.FovyTick()) * 0.1f;
        break;
    case 11:            
        mRunKey.mBank -= cDegree(mCamSetup.FovyTick()) * 0.1f;
        break;
    case 8:
        s16 temp_r26 = mCamSetup.Rotate(var_f29 * (stick_x * field_0xc14));
        field_0x44.U(field_0x44.U() + temp_r26);

        temp_r26 = mCamSetup.Rotate(var_f29 * (stick_y * field_0xc14));
        field_0x44.V(field_0x44.V() + temp_r26);
    }

    if (sp8 != 0) {
        *sp30 = *var_r28 + field_0x44.Xyz();
    }

    cSGlobe sp50(mRunKey.mEye - mRunKey.mCenter);
    mUp.set(0.0f, 1.0f, 0.0f);
    return 1;
}

int dDbgCamera_c::cmdTool() {
    if (mDoCPd_c::getTrigRight(mPadNo)) {
        if (mCmdMode >= 3) {
            mCmdMode = 0;
        } else {
            mCmdMode++;
        }
    } else if (mDoCPd_c::getTrigLeft(mPadNo)) {
        if (mCmdMode <= 0) {
            mCmdMode = 3;
        } else {
            mCmdMode--;
        }
    }

    if (!mIsPlaying) {
        switch (mCmdMode) {
        case 1:
            if (mDoCPd_c::getTrigA(mPadNo)) {
                mIsPlaying = true;
                field_0x704.field_0x0 = 0;
                field_0x704.field_0x4 = 0;
                makeZevData();
            }
            break;
        case 2:
            if (mDoCPd_c::getTrigA(mPadNo)) {
                printZevData();
            }
            break;
        case 0:
            if (mDoCPd_c::getTrigA(mPadNo)) {
                mCamSetup.mActorSrchRange = 10000000.0f;
                mCamSetup.mActorID = fpcM_ERROR_PROCESS_ID_e;
                fopAcIt_Executor((fopAcIt_ExecutorFunc)search_actor, &mCamSetup);
                
                fopAc_ac_c* actor = Actor(mCamSetup.mActorID);
                if (actor != NULL) {
                    mRunKey.mActorInfo.id = mCamSetup.mActorID;
                    fopAcM_getNameString(actor, mRunKey.mActorInfo.name);
                } else {
                    mRunKey.mActorInfo.id = fpcM_ERROR_PROCESS_ID_e;
                    mCamSetup.mActorID = fpcM_ERROR_PROCESS_ID_e;
                }
            } else if (mDoCPd_c::getTrigB(mPadNo)) {
                mRunKey.mActorInfo.id = fpcM_ERROR_PROCESS_ID_e;
                mCamSetup.mActorID = fpcM_ERROR_PROCESS_ID_e;
            }
            break;
        case 3:
            if (mDoCPd_c::getTrigY(mPadNo)) {
                mRunKey.mCenter = dComIfGp_getPlayer(0)->attention_info.position;
                // field_0x704 = mRunKey.mCenter = dComIfGp_getPlayer(0)->attention_info.position;
                // this->unk730 = 3;
            }
            /* fallthrough */
        default:
            return 1;
        }
    } else if (!dComIfGp_event_runCheck()) {
        mIsPlaying = false;
    }

    if (mIsPlaying) {
        field_0x704.field_0x4++;
    }

    return 1;
}

int dDbgCamera_c::makeZevData() {
    int keyNo;
    int spline_keyNo;
    int sp48;
    int spline_keyNum;

    int size = 0;
    int spline_timer;
    int sp38 = 0;

    field_0xb14 = 0;

    if (zevwork::WorkBuffer == NULL) {
        zevwork::WorkBuffer = new u8[zevwork::WorkSize];
        if (zevwork::WorkBuffer == NULL) {
            OSReport("debug camera: memory allocate error!! %d byte\n", zevwork::WorkSize);
            return 0;
        }
    }

    OSReport("debug camera: making demo data...\n");

    event_binary_data_header* header = (event_binary_data_header*)(zevwork::WorkBuffer + size);
    size += sizeof(event_binary_data_header) - 8;

    header->cutNum = 0;
    dEvDtEvent_DBG_c* event = (dEvDtEvent_DBG_c*)(zevwork::WorkBuffer + size);
    header->eventTop = size;
    header->eventNum = 1;
    size += sizeof(dEvDtEvent_DBG_c);
    event->Init();
    strcpy(event->mName, "_DebugCamera");
    event->mStaff[0] = 0;
    event->mStaff[1] = 1;
    event->mNStaff = 2;

    dEvDtStaff_DBG_c* camera_staff = (dEvDtStaff_DBG_c*)(zevwork::WorkBuffer + size);
    header->staffTop = size;
    header->staffNum = 2;
    size += sizeof(dEvDtStaff_DBG_c);
    camera_staff->Init();
    strcpy(camera_staff->mName, "CAMERA");
    camera_staff->mType = dEvDtStaff_c::TYPE_CAMERA;
    camera_staff->mIndex = 0;
    camera_staff->mStartCut = 1;
    camera_staff->mFlagID = sp38++;

    dEvDtStaff_DBG_c* all_staff = (dEvDtStaff_DBG_c*)(zevwork::WorkBuffer + size);
    size += sizeof(dEvDtStaff_DBG_c);
    all_staff->Init();
    strcpy(all_staff->mName, "ALL");
    all_staff->mType = dEvDtStaff_c::TYPE_ALL;
    all_staff->mIndex = 1;
    all_staff->mStartCut = 0;
    all_staff->mFlagID = sp38++;

    dEvDtCut_DBG_c* cut = (dEvDtCut_DBG_c*)(zevwork::WorkBuffer + size);
    header->cutTop = size;
    header->cutNum = 0;
    size += sizeof(dEvDtCut_c) * zevwork::CutMaxNum;
    dEvDtCut_DBG_c* prev_cut = NULL;
    cut->Init();
    strcpy(cut->mName, "DUMMY");
    cut->mIndex = header->cutNum++;
    cut->mFlagId = sp38++;
    event->mFlags[0] = cut->mFlagId;
    cut++;

    dEvDtData_DBG_c* pdata = (dEvDtData_DBG_c*)(zevwork::WorkBuffer + size);
    header->dataTop = size;
    header->dataNum = 0;
    size += sizeof(dEvDtData_DBG_c) * zevwork::DataMaxNum;

    f32* fdata = (f32*)(zevwork::WorkBuffer + size);
    header->fDataTop = size;
    header->fDataNum = 0;
    size += sizeof(f32) * zevwork::FDataMaxNum;

    int* idata = (int*)(zevwork::WorkBuffer + size);
    header->iDataTop = size;
    header->iDataNum = 0;
    size += sizeof(int) * zevwork::IDataMaxNum;

    char* sdata = (char*)(zevwork::WorkBuffer + size);
    header->sDataTop = size;
    header->sDataNum = 0;
    size += sizeof(char) * zevwork::SDataMaxNum;

    if (size > zevwork::WorkSize) {
        OSReport("debug camera: too big data!! %d > %d byte\n", size, zevwork::WorkSize);
        return 0;
    }

    int sp14 = 0;
    int sp10;

    for (keyNo = 0; keyNo < ARRAY_SIZE(mKeys); keyNo++) {
        cut->Init();
        strcpy(cut->mName, ActionName(mKeys[keyNo].mAction));
        cut->mDataTop = header->dataNum;

        switch (mKeys[keyNo].mAction) {
        case dDbgCam_KeyAction_FIXEDFRM:
        case dDbgCam_KeyAction_UNITRANS:
            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                pdata->Init();
                strcpy(pdata->mName, "RelActor");
                pdata->mIndex = header->dataNum++;
                pdata->mType = dEvDtData_c::TYPE_STRING;
                pdata->mDataIndex = sp14;
                pdata->mNumber = 1;
                pdata->mNext = header->dataNum;
                pdata++;
                strcpy(sdata, mKeys[keyNo].mActorInfo.name);
                sp48 = strlen(mKeys[keyNo].mActorInfo.name) + 1;
                header->sDataNum++;
                sdata += sp48;
                sp14 += sp48;
            }

            pdata->Init();
            strcpy(pdata->mName, "Center");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_VEC;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = 3;
            pdata->mNext = header->dataNum;
            pdata++;

            *fdata++ = mKeys[keyNo].mCenter.x;
            *fdata++ = mKeys[keyNo].mCenter.y;
            *fdata++ = mKeys[keyNo].mCenter.z;
            header->fDataNum += 3;

            pdata->Init();
            strcpy(pdata->mName, "Eye");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_VEC;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = 3;
            pdata->mNext = header->dataNum;
            pdata++;

            *fdata++ = mKeys[keyNo].mEye.x;
            *fdata++ = mKeys[keyNo].mEye.y;
            *fdata++ = mKeys[keyNo].mEye.z;
            header->fDataNum += 3;

            pdata->Init();
            strcpy(pdata->mName, "Fovy");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_FLOAT;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = 1;
            pdata->mNext = header->dataNum;
            pdata++;

            *fdata++ = mKeys[keyNo].mFovy;
            header->fDataNum += 1;

            pdata->Init();
            strcpy(pdata->mName, "Bank");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_FLOAT;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = 1;
            pdata->mNext = header->dataNum;
            pdata++;

            *fdata++ = mKeys[keyNo].mBank;
            header->fDataNum += 1;

            pdata->Init();
            strcpy(pdata->mName, "Timer");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_INT;
            pdata->mDataIndex = header->iDataNum;
            pdata->mNumber = 1;
            pdata++;

            *idata++ = mKeys[keyNo].mTimer;
            header->iDataNum += 1;

            field_0xb14 += mKeys[keyNo].mTimer;
            break;
        case dDbgCam_KeyAction_BSPLINE:
            spline_timer = 0;
            sp48 = 0;

            int sp8;
            sp8 = spline_keyNum = keyNo;
            for (; spline_keyNum < ARRAY_SIZE(mActionKey); spline_keyNum++) {
                if (mActionKey[spline_keyNum] != ActionSymbol(dDbgCam_KeyAction_BSPLINE)) {
                    break;
                }

                sp48++;
                spline_timer += mKeys[spline_keyNum].mTimer;
            }

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                pdata->Init();
                strcpy(pdata->mName, "RelActor");
                pdata->mIndex = header->dataNum++;
                pdata->mType = dEvDtData_c::TYPE_STRING;
                pdata->mDataIndex = sp14;
                pdata->mNumber = 1;
                pdata->mNext = header->dataNum;
                pdata++;

                strcpy(sdata, mKeys[keyNo].mActorInfo.name);
                sp10 = strlen(mKeys[keyNo].mActorInfo.name) + 1;
                header->sDataNum += 1;

                sdata += sp10;
                sp14 += sp10;
            }

            pdata->Init();
            strcpy(pdata->mName, "Centers");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_VEC;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = sp48 * 3;
            pdata->mNext = header->dataNum;
            pdata++;

            
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                *fdata++ = mKeys[spline_keyNo].mCenter.x;
                *fdata++ = mKeys[spline_keyNo].mCenter.y;
                *fdata++ = mKeys[spline_keyNo].mCenter.z;
            }

            header->fDataNum += sp48 * 3;

            pdata->Init();
            strcpy(pdata->mName, "Eyes");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_VEC;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = sp48 * 3;
            pdata->mNext = header->dataNum;
            pdata++;

            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                *fdata++ = mKeys[spline_keyNo].mEye.x;
                *fdata++ = mKeys[spline_keyNo].mEye.y;
                *fdata++ = mKeys[spline_keyNo].mEye.z;
            }

            header->fDataNum += sp48 * 3;

            pdata->Init();
            strcpy(pdata->mName, "Fovys");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_FLOAT;
            pdata->mDataIndex = header->fDataNum;
            pdata->mNumber = sp48;
            pdata->mNext = header->dataNum;
            pdata++;

            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                *fdata++ = mKeys[spline_keyNo].mFovy;
            }

            header->fDataNum += sp48;

            pdata->Init();
            strcpy(pdata->mName, "Timer");
            pdata->mIndex = header->dataNum++;
            pdata->mType = dEvDtData_c::TYPE_INT;
            pdata->mDataIndex = header->iDataNum;
            pdata->mNumber = 1;
            pdata++;

            *idata++ = spline_timer;
            header->iDataNum += 1;

            field_0xb14 += spline_timer;

            keyNo += sp48;
        }

        cut->mIndex = header->cutNum++;
        cut->mFlagId = sp38++;

        if (prev_cut != NULL) {
            cut->mFlags[0] = prev_cut->mFlagId;
            prev_cut->mNext = cut->mIndex;
        }

        prev_cut = cut;
        event->mFlags[0] = cut->mFlagId;
        cut++;
    }

    if (header->cutNum > zevwork::CutMaxNum) {
        OSReport("debug camera: too many cuts: %d > %d: broken event data\n", header->cutNum, zevwork::CutMaxNum);
        return 0;
    }

    if (header->dataNum > zevwork::DataMaxNum) {
        OSReport("debug camera: too many data: %d > %d: broken event data\n", header->dataNum, zevwork::DataMaxNum);
        return 0;
    }

    if (header->fDataNum > zevwork::FDataMaxNum) {
        OSReport("debug camera: too many fdata: %d > %d: broken event data\n", header->fDataNum, zevwork::FDataMaxNum);
        return 0;
    }

    if (header->fDataNum > zevwork::IDataMaxNum) {
        OSReport("debug camera: too many idata: %d > %d: broken event data\n", header->iDataNum, zevwork::IDataMaxNum);
        return 0;
    }

    if (header->sDataNum > zevwork::SDataMaxNum) {
        OSReport("debug camera: too many idata: %d > %d: broken event data\n", header->sDataNum, zevwork::SDataMaxNum);
        return 0;
    }

    if (header->cutNum != 0) {
        OSReport("              cut     %d \n", header->cutNum);
        OSReport("              data    %d \n", header->dataNum);
        OSReport("              float   %d \n", header->fDataNum);
        OSReport("              integer %d \n", header->iDataNum);
        OSReport("              string  %d \n", header->sDataNum);

        dEvM_HIO_c& hio = dComIfGp_getPEvtManager()->getEventHIO();
        hio.setDebugCameraData(zevwork::WorkBuffer);
    }

    return 1;
}

void dDbgCamera_c::printZevData() {
    OSReport("\n");
    OSReport("<?xml version=\"1.0\" encoding=\"Shift_JIS\"?>\n\n");
    OSReport("<!--\n    △ \n   △△ Game Cube Zelda - ");
    OSReport("Camera Tool Event Data \n ▼▼ \n  ▼ \n-->\n");
    OSReport("<event version=\"2.0\">\n");
    OSReport("  <sequence name=\"???\" finish=\"CAMERA\">\n");
    OSReport("    <staff name=\"CAMERA\" type=\"CAMERA\">\n");

    int keyNo;
    int spline_keyNo, spline_keyNum, spline_timer;
    for (keyNo = 0; keyNo < ARRAY_SIZE(mKeys); keyNo++) {
        switch (mKeys[keyNo].mAction) {
        case dDbgCam_KeyAction_FIXEDFRM:
            OSReport("    <cut name=\"Key%02d\" type=\"FIXEDFRM\">\n", keyNo);

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                OSReport("      <data name=\"RelActor\" type=\"STRING\">");
                OSReport("%s</data>\n", mKeys[keyNo].mActorInfo.name);
            }

            OSReport("      <data name=\"Center\" type=\"XYZ\">");
            OSReport("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mCenter.x, mKeys[keyNo].mCenter.y, mKeys[keyNo].mCenter.z);

            OSReport("      <data name=\"Eye\" type=\"XYZ\">");
            OSReport("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mEye.x, mKeys[keyNo].mEye.y, mKeys[keyNo].mEye.z);

            OSReport("      <data name=\"Fovy\" type=\"FLOAT\">");
            OSReport("%.3f</data>\n", mKeys[keyNo].mFovy);

            OSReport("      <data name=\"Bank\" type=\"FLOAT\">");
            OSReport("%.3f</data>\n", mKeys[keyNo].mBank);

            OSReport("      <data name=\"Timer\" type=\"INTEGER\">");
            OSReport("%d</data>\n", mKeys[keyNo].mTimer);

            OSReport("    </cut>\n");
            break;
        case dDbgCam_KeyAction_UNITRANS:
            OSReport("    <cut name=\"Key%02d\" type=\"UNITRANS\">\n", keyNo);

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                OSReport("      <data name=\"RelActor\" type=\"STRING\">");
                OSReport("%s</data>\n", mKeys[keyNo].mActorInfo.name);
            }

            OSReport("      <data name=\"Center\" type=\"XYZ\">");
            OSReport("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mCenter.x, mKeys[keyNo].mCenter.y, mKeys[keyNo].mCenter.z);

            OSReport("      <data name=\"Eye\" type=\"XYZ\">");
            OSReport("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mEye.x, mKeys[keyNo].mEye.y, mKeys[keyNo].mEye.z);

            OSReport("      <data name=\"Fovy\" type=\"FLOAT\">");
            OSReport("%.3f</data>\n", mKeys[keyNo].mFovy);

            OSReport("      <data name=\"Bank\" type=\"FLOAT\">");
            OSReport("%.3f</data>\n", mKeys[keyNo].mBank);

            OSReport("      <data name=\"Timer\" type=\"INTEGER\">");
            OSReport("%d</data>\n", mKeys[keyNo].mTimer);

            OSReport("    </cut>\n");
            break;
        case dDbgCam_KeyAction_BSPLINE:
            spline_keyNum = keyNo;
            for (; spline_keyNum < ARRAY_SIZE(mActionKey); spline_keyNum++) {
                if (mActionKey[spline_keyNum] != ActionSymbol(dDbgCam_KeyAction_BSPLINE)) {
                    break;
                }
            }

            OSReport("    <cut name=\"Key%02d\" type=\"BSPLINE\">\n", keyNo);

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                OSReport("      <data name=\"RelActor\" type=\"STRING\">");
                OSReport("%s</data>\n", mKeys[keyNo].mActorInfo.name);
            }

            OSReport("      <data name=\"Centers\" type=\"XYZ\">\n");
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                OSReport("        %.3f %.3f %.3f\n", mKeys[spline_keyNo].mCenter.x, mKeys[spline_keyNo].mCenter.y, mKeys[spline_keyNo].mCenter.z);
            }
            OSReport("      </data>\n");

            OSReport("      <data name=\"Eyes\" type=\"XYZ\">\n");
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                OSReport("        %.3f %.3f %.3f\n", mKeys[spline_keyNo].mEye.x, mKeys[spline_keyNo].mEye.y, mKeys[spline_keyNo].mEye.z);
            }
            OSReport("      </data>\n");

            OSReport("      <data name=\"Fovys\" type=\"FLOAT\">\n");
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                OSReport("        %.3f\n", mKeys[spline_keyNo].mFovy);
            }
            OSReport("      </data>\n");

            spline_timer = 0;
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                spline_timer += mKeys[spline_keyNo].mTimer;
            }
            OSReport("      <data name=\"Timer\" type=\"INTEGER\">%d</data>\n", spline_timer);

            OSReport("    </cut>\n");
            keyNo = spline_keyNum - 1;
            break;
        }
    }

    OSReport("    </staff>\n");
    OSReport("    <staff name=\"ALL\" type=\"EXTRA\">\n");
    OSReport("      <cut name=\"dummy\" type=\"DUMMY\"/>\n");
    OSReport("    </staff>\n");
    OSReport("  </sequence>\n");
    OSReport("</event>\n");
    OSReport("\n");
}

void dDbgCamera_c::WriteZevData() {
    OpenFile();

    PrintFile("\n");
    PrintFile("<?xml version=\"1.0\" encoding=\"Shift_JIS\"?>\n\n");
    PrintFile("<!--\n    △ \n   △△ Game Cube Zelda - ");
    PrintFile("Camera Tool Event Data \n ▼▼ \n  ▼ \n-->\n");
    PrintFile("<event version=\"2.0\">\n");
    PrintFile("  <sequence name=\"???\" finish=\"CAMERA\">\n");
    PrintFile("    <staff name=\"CAMERA\" type=\"CAMERA\">\n");

    int keyNo;
    int spline_keyNo, spline_keyNum, spline_timer;
    for (keyNo = 0; keyNo < ARRAY_SIZE(mKeys); keyNo++) {
        switch (mKeys[keyNo].mAction) {
        case dDbgCam_KeyAction_FIXEDFRM:
            PrintFile("    <cut name=\"Key%02d\" type=\"FIXEDFRM\">\n", keyNo);

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                PrintFile("      <data name=\"RelActor\" type=\"STRING\">");
                PrintFile("%s</data>\n", mKeys[keyNo].mActorInfo.name);
            }

            PrintFile("      <data name=\"Center\" type=\"XYZ\">");
            PrintFile("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mCenter.x, mKeys[keyNo].mCenter.y, mKeys[keyNo].mCenter.z);

            PrintFile("      <data name=\"Eye\" type=\"XYZ\">");
            PrintFile("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mEye.x, mKeys[keyNo].mEye.y, mKeys[keyNo].mEye.z);

            PrintFile("      <data name=\"Fovy\" type=\"FLOAT\">");
            PrintFile("%.3f</data>\n", mKeys[keyNo].mFovy);

            PrintFile("      <data name=\"Bank\" type=\"FLOAT\">");
            PrintFile("%.3f</data>\n", mKeys[keyNo].mBank);

            PrintFile("      <data name=\"Timer\" type=\"INTEGER\">");
            PrintFile("%d</data>\n", mKeys[keyNo].mTimer);

            PrintFile("    </cut>\n");
            break;
        case dDbgCam_KeyAction_UNITRANS:
            PrintFile("    <cut name=\"Key%02d\" type=\"UNITRANS\">\n", keyNo);

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                PrintFile("      <data name=\"RelActor\" type=\"STRING\">");
                PrintFile("%s</data>\n", mKeys[keyNo].mActorInfo.name);
            }

            PrintFile("      <data name=\"Center\" type=\"XYZ\">");
            PrintFile("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mCenter.x, mKeys[keyNo].mCenter.y, mKeys[keyNo].mCenter.z);

            PrintFile("      <data name=\"Eye\" type=\"XYZ\">");
            PrintFile("%.3f %.3f %.3f</data>\n", mKeys[keyNo].mEye.x, mKeys[keyNo].mEye.y, mKeys[keyNo].mEye.z);

            PrintFile("      <data name=\"Fovy\" type=\"FLOAT\">");
            PrintFile("%.3f</data>\n", mKeys[keyNo].mFovy);

            PrintFile("      <data name=\"Bank\" type=\"FLOAT\">");
            PrintFile("%.3f</data>\n", mKeys[keyNo].mBank);

            PrintFile("      <data name=\"Timer\" type=\"INTEGER\">");
            PrintFile("%d</data>\n", mKeys[keyNo].mTimer);

            PrintFile("    </cut>\n");
            break;
        case dDbgCam_KeyAction_BSPLINE:
            spline_keyNum = keyNo;
            for (; spline_keyNum < ARRAY_SIZE(mActionKey); spline_keyNum++) {
                if (mActionKey[spline_keyNum] != ActionSymbol(dDbgCam_KeyAction_BSPLINE)) {
                    break;
                }
            }

            PrintFile("    <cut name=\"Key%02d\" type=\"BSPLINE\">\n", keyNo);

            if (mKeys[keyNo].mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
                PrintFile("      <data name=\"RelActor\" type=\"STRING\">");
                PrintFile("%s</data>\n", mKeys[keyNo].mActorInfo.name);
            }

            PrintFile("      <data name=\"Centers\" type=\"XYZ\">\n");
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                PrintFile("        %.3f %.3f %.3f\n", mKeys[spline_keyNo].mCenter.x, mKeys[spline_keyNo].mCenter.y, mKeys[spline_keyNo].mCenter.z);
            }
            PrintFile("      </data>\n");

            PrintFile("      <data name=\"Eyes\" type=\"XYZ\">\n");
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                PrintFile("        %.3f %.3f %.3f\n", mKeys[spline_keyNo].mEye.x, mKeys[spline_keyNo].mEye.y, mKeys[spline_keyNo].mEye.z);
            }
            PrintFile("      </data>\n");

            PrintFile("      <data name=\"Fovys\" type=\"FLOAT\">\n");
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                PrintFile("        %.3f\n", mKeys[spline_keyNo].mFovy);
            }
            PrintFile("      </data>\n");

            spline_timer = 0;
            for (spline_keyNo = keyNo; spline_keyNo < spline_keyNum; spline_keyNo++) {
                spline_timer += mKeys[spline_keyNo].mTimer;
            }
            PrintFile("      <data name=\"Timer\" type=\"INTEGER\">%d</data>\n", spline_timer);

            PrintFile("    </cut>\n");
            keyNo = spline_keyNum - 1;
            break;
        }
    }

    PrintFile("    </staff>\n");
    PrintFile("    <staff name=\"ALL\" type=\"EXTRA\">\n");
    PrintFile("      <cut name=\"dummy\" type=\"DUMMY\"/>\n");
    PrintFile("    </staff>\n");
    PrintFile("  </sequence>\n");
    PrintFile("</event>\n");
    PrintFile("\n");

    CloseFile();
}

void dDbgCamera_c::monitor() {
    s32 pos_x = mCamSetup.mPosX;
    s32 pos_y = mCamSetup.mPosY;
    s32 y_offset = 15;

    if (!mIsPlaying) {
        cXyz center(mRunKey.mCenter);
        cXyz eye(mRunKey.mEye);
        fopAc_ac_c* actor = NULL;
        f32 u = field_0x44.U().Degree();

        if (mRunKey.mActorInfo.id != fpcM_ERROR_PROCESS_ID_e) {
            actor = Actor(mRunKey.mActorInfo.id);
            if (actor != NULL) {
                cSGlobe sp70(mRunKey.mCenter - actor->attention_info.position);
                sp70.U(sp70.U() - cSAngle(actor->shape_angle.y));
                center = sp70.Xyz();

                cSGlobe sp68(mRunKey.mEye - actor->attention_info.position);
                sp68.U(sp68.U() - cSAngle(actor->shape_angle.y));
                eye = sp68.Xyz();

                u = cSAngle(field_0x44.U() - actor->shape_angle.y).Degree();
            }
        }

        Report(pos_x, pos_y, 1, "CTR%11.3f%11.3f%11.3f", center.x, center.y, center.z);
        Report(pos_x, pos_y += y_offset, 1, "EYE%11.3f%11.3f%11.3f", eye.x, eye.y, eye.z);
        Report(pos_x, pos_y += y_offset, 1, "RVU %10.3f %10.3f %10.3f", field_0x44.R(), field_0x44.V().Degree(), u);
        Report(pos_x, pos_y += y_offset, 1, "FVY %10.3f    %s %s   %s", mRunKey.mFovy, field_0xc08 ? "[L]" : "[ ]", field_0xc0c ? "[R]" : "[ ]", field_0xc08 ? "CTR->EYE" : "EYE->CTR");
        Report(pos_x - 13, pos_y + (y_offset * field_0x54), 7, "*");
    
        if (actor != NULL) {
            Report(pos_x, pos_y += y_offset, 1, "BNK %10.3f   @%7.7s", mRunKey.mBank, &mRunKey.mActorInfo.name);
        } else {
            Report(pos_x, pos_y += y_offset, 1, "BNK %10.3f   @  WORLD", mRunKey.mBank);
        }
    
        Report(pos_x, pos_y += y_offset, 1, "KEY %s", mActionKey);

        char spC0[32];

        int var_r27;
        for (var_r27 = 0; var_r27 < ARRAY_SIZE(spC0); var_r27++) {
            if (var_r27 == field_0x58) {
                spC0[var_r27] = '_';
                var_r27++;
                break;
            }

            spC0[var_r27] = ' ';
        }

        spC0[var_r27] = 0;
        Report(pos_x, pos_y, 7, "    %s", spC0);
        Report(pos_x, pos_y += y_offset, 1, "CMD BASE PLAY TERM FIXP");

        *spC0 = 0;

        for (var_r27 = 0; var_r27 < 4; var_r27++) {
            if (var_r27 == mCmdMode) {
                strcat(spC0, "____ ");
                break;
            }

            strcat(spC0, "     ");
        }

        Report(pos_x, pos_y, 7, "    %s", spC0);
        Report(pos_x, pos_y += y_offset, 1, "ACT %c %8.8s", ActionSymbol(mRunKey.mAction), ActionName(mRunKey.mAction));
        Report(pos_x, pos_y += y_offset, 1, "TIM %3d", mRunKey.mTimer);
    } else {
        Report(pos_x, pos_y, 1, "CNT %4d/%4d", field_0x704.field_0x4 - 1, field_0xb14);
    }
}

bool dDbgCamera_c::showCenterMarker() {
    switch (mCamSetup.CenterMarker()) {
    case 1:
        if (field_0xc00 != 18 || field_0xc04 != 18 || field_0xc08 != 0 || field_0xc0c != 0) {
            field_0xc18 = 60;
        }

        if (field_0xc18 != 0) {
            field_0xc18--;
        }
 
        return field_0xc18 != 0;
    case 3:
        return true;
    case 2:
    case 0:
    default:
        return false;
    }
}

bool dDbgCamera_c::showCameraPos() {
    return mCamSetup.CheckFlag(4);
}

bool dDbgCamera_c::showIdlCameraPos() {
    return mCamSetup.CheckFlag(8);
}

void dDbgCamera_c::Init(dCamera_c* i_dbgCamera) {
    mpCamera = i_dbgCamera;
    mRunKey = PlaneKey();
    mRunKey.mAction = dDbgCam_KeyAction_FIXEDFRM;
    field_0x58 = 0;

    int i;
    for (i = 0; i < ARRAY_SIZE(mKeys); i++) {
        mKeys[i] = PlaneKey();
    }

    memset(mActionKey, '-', sizeof(mActionKey));
    field_0x80 = 0;
    field_0x54 = 0;
    mCmdMode = 0;
    mCamSetup.Init(this);
}

void dDbgCamera_c::Finish() {
    mCamSetup.Finish();
}

void dDbgCamera_c::Reset(dCamera_c* i_dbgCamera) {
    mpCamera = i_dbgCamera;
    mPadNo = PAD_3;
    field_0x50 = 0;
    mUp = i_dbgCamera->Up();

    if (mActionKey[field_0x58] == '-') {
        mRunKey = PlaneKey();
    }

    field_0xc04 = field_0xc00 = 18;
    field_0xc10 = field_0xc14 = 0.0f;
    field_0xc08 = field_0xc0c = 0;
    field_0xc18 = 0;
    mCmdMode = 0;
    mIsPlaying = false;
}

int dDbgCamera_c::Run() {
    int rt;

    if (mDoCPd_c::getHoldLockL(mPadNo)) {
        field_0xc08 = 1;
    } else {
        field_0xc08 = 0;
    }

    if (mDoCPd_c::getHoldLockR(mPadNo)) {
        field_0xc0c = 1;
    } else {
        field_0xc0c = 0;
    }

    if (mDoCPd_c::getTrigUp(mPadNo)) {
        if (field_0x54 == 0) {
            field_0x54 = 5;
        } else {
            field_0x54--;
        }
    } else if (mDoCPd_c::getTrigDown(mPadNo)) {
        if (field_0x54 >= 5) {
            field_0x54 = 0;
        } else {
            field_0x54++;
        }
    }

    if (field_0x54 == 4) {
        if (mDoCPd_c::getTrigLeft(mPadNo)) {
            if (mRunKey.mAction == dDbgCam_KeyAction_FIXEDFRM) {
                mRunKey.mAction = dDbgCam_KeyAction_NOACTION;
            } else {
                mRunKey.mAction--;
            }
        } else if (mDoCPd_c::getTrigRight(mPadNo)) {
            if (mRunKey.mAction == dDbgCam_KeyAction_NOACTION) {
                mRunKey.mAction = dDbgCam_KeyAction_FIXEDFRM;
            } else {
                mRunKey.mAction++;
            }
        }
    } else if (field_0x54 == 5) {
        if (mDoCPd_c::getTrigRight(mPadNo)) {
            mRunKey.mTimer++;
            field_0xc1c = 0;
        } else if (mDoCPd_c::getHoldRight(mPadNo)) {
            if (field_0xc1c++ > 20) {
                mRunKey.mTimer++;
            }
        } else if (mDoCPd_c::getTrigLeft(mPadNo)) {
            mRunKey.mTimer--;
            field_0xc1c = 0;
        } else if (mDoCPd_c::getHoldLeft(mPadNo)) {
            if (field_0xc1c++ > 20) {
                mRunKey.mTimer--;
            }
        }
    } else if (field_0x54 == 3) {
        rt = cmdTool();
    } else {
        rt = moveTool();
    }

    if (mCamSetup.CheckFlag(0x8000)) {
        monitor();
    }

    return rt;
}

int dDbgCamera_c::DrawShape() {
    int rt = 0;

    if (showCameraPos()) {
        cXyz sp2F0[] = {
            cXyz(5.0f, 6.5f, 10.0f),
            cXyz(-5.0f, 6.5f, 10.0f),
            cXyz(5.0f, -7.5f, 10.0f),
            cXyz(-5.0f, -7.5f, 10.0f),
            cXyz(5.0f, 6.5f, -10.0f),
            cXyz(-5.0f, 6.5f, -10.0f),
            cXyz(5.0f, -5.0f, -10.0f),
            cXyz(-5.0f, -5.0f, -10.0f),
        };

        cXyz sp290[] = {
            cXyz(3.0f, 3.0f, 10.0f),
            cXyz(-3.0f, 3.0f, 10.0f),
            cXyz(3.0f, -3.0f, 10.0f),
            cXyz(-3.0f, -3.0f, 10.0f),
            cXyz(3.0f, 3.0f, 16.0f),
            cXyz(-3.0f, 3.0f, 16.0f),
            cXyz(3.0f, -3.0f, 16.0f),
            cXyz(-3.0f, -3.0f, 16.0f),
        };

        cXyz sp1A0[] = {
            cXyz(80.0f, 0.0f, 0.0f),
            cXyz(-80.0f, 0.0f, 0.0f),
            cXyz(0.0f, 80.0f, 0.0f),
            cXyz(0.0f, -80.0f, 0.0f),
        };

        dCamera_c* camera = dCam_getBody();

        GXColor sp6C = {0xFF, 0xDC, 0x80, 0x96};
        GXColor sp68 = {0xDC, 0xFF, 0x80, 0x96};
        GXColor sp64 = {0xFF, 0xFF, 0xFF, 0x32};

        cXyz cam_center = camera->Center();
        cXyz cam_eye = camera->Eye();
        cSAngle cam_u(camera->U());
        cSAngle cam_v(camera->V());

        int i;
        for (i = 0; i < 8; i++) {
            sp2F0[i] = dCamMath::xyzRotateX(sp2F0[i], cam_v);
            sp2F0[i] = dCamMath::xyzRotateY(sp2F0[i], cam_u);
            sp2F0[i] += cam_eye;

            sp290[i] = dCamMath::xyzRotateX(sp290[i], cam_v);
            sp290[i] = dCamMath::xyzRotateY(sp290[i], cam_u);
            sp290[i] += cam_eye;
        }

        dDbVw_drawCube8pOpa(sp2F0, sp6C);
        dDbVw_drawCube8pOpa(sp290, sp6C);
        dDbVw_drawSphereXlu(cam_eye, 3.0f, sp68, 1);
        dDbVw_drawSphereXlu(cam_center, 5.0f, sp68, 1);

        for (i = 0; i < 4; i++) {
            sp1A0[i] = dCamMath::xyzRotateX(sp1A0[i], cam_v);
            sp1A0[i] = dCamMath::xyzRotateY(sp1A0[i], cam_u);
            sp1A0[i] += cam_center;
        }

        cSGlobe sp78(80.0f + camera->R(), -cam_v, cam_u);
        dDbVw_drawLineXlu(cam_eye, cam_center, sp64, 1, 0xC);
        dDbVw_drawLineXlu(sp1A0[0], sp1A0[1], sp64, 1, 0xC);
        dDbVw_drawLineXlu(sp1A0[2], sp1A0[3], sp64, 1, 0xC);
        rt = 1;
    }

    if (showIdlCameraPos()) {
        cXyz sp230[] = {
            cXyz(5.0f, 6.5f, 10.0f),
            cXyz(-5.0f, 6.5f, 10.0f),
            cXyz(5.0f, -7.5f, 10.0f),
            cXyz(-5.0f, -7.5f, 10.0f),
            cXyz(5.0f, 6.5f, -10.0f),
            cXyz(-5.0f, 6.5f, -10.0f),
            cXyz(5.0f, -5.0f, -10.0f),
            cXyz(-5.0f, -5.0f, -10.0f),
        };

        cXyz sp1D0[] = {
            cXyz(3.0f, 3.0f, 10.0f),
            cXyz(-3.0f, 3.0f, 10.0f),
            cXyz(3.0f, -3.0f, 10.0f),
            cXyz(-3.0f, -3.0f, 10.0f),
            cXyz(3.0f, 3.0f, 16.0f),
            cXyz(-3.0f, 3.0f, 16.0f),
            cXyz(3.0f, -3.0f, 16.0f),
            cXyz(-3.0f, -3.0f, 16.0f),
        };

        cXyz sp170[] = {
            cXyz(80.0f, 0.0f, 0.0f),
            cXyz(-80.0f, 0.0f, 0.0f),
            cXyz(0.0f, 80.0f, 0.0f),
            cXyz(0.0f, -80.0f, 0.0f),
        };

        dCamera_c* camera = dCam_getBody();

        GXColor sp60 = {0x80, 0xFF, 0xDC, 0x5A};
        GXColor sp5C = {0x80, 0xDC, 0xFF, 0x5A};
        GXColor sp58 = {0xFF, 0xFF, 0xFF, 0x1E};

        cXyz cam_icenter = camera->iCenter();
        cXyz cam_ieye = camera->iEye();
        cSAngle cam_iu(camera->iU());
        cSAngle cam_iv(camera->iV());

        int i;
        for (i = 0; i < 8; i++) {
            sp230[i] = dCamMath::xyzRotateX(sp230[i], cam_iv);
            sp230[i] = dCamMath::xyzRotateY(sp230[i], cam_iu);
            sp230[i] += cam_ieye;

            sp1D0[i] = dCamMath::xyzRotateX(sp1D0[i], cam_iv);
            sp1D0[i] = dCamMath::xyzRotateY(sp1D0[i], cam_iu);
            sp1D0[i] += cam_ieye;
        }

        dDbVw_drawCube8pOpa(sp230, sp60);
        dDbVw_drawCube8pOpa(sp1D0, sp60);
        dDbVw_drawSphereXlu(cam_ieye, 3.0f, sp5C, 1);
        dDbVw_drawSphereXlu(cam_icenter, 5.0f, sp5C, 1);

        for (i = 0; i < 4; i++) {
            sp170[i] = dCamMath::xyzRotateX(sp170[i], cam_iv);
            sp170[i] = dCamMath::xyzRotateY(sp170[i], cam_iu);
            sp170[i] += cam_icenter;
        }

        cSGlobe sp70(80.0f + camera->iR(), -cam_iv, cam_iu);
        cam_icenter = cam_ieye + sp70.Xyz();

        dDbVw_drawLineXlu(cam_ieye, cam_icenter, sp58, 1, 0xC);
        dDbVw_drawLineXlu(sp170[0], sp170[01], sp58, 1, 0xC);
        dDbVw_drawLineXlu(sp170[2], sp170[3], sp58, 1, 0xC);
        rt = 1;
    }

    if (showCenterMarker()) {
        f32 cam_cross = mCamSetup.CenterCloss();
        GXColor color = {0x80, 0x80, 0xFF, 0x80};
        cXyz sp134;
        cXyz sp128;

        sp134 = mRunKey.mCenter;
        sp128 = mRunKey.mCenter;
        sp134.x += cam_cross;
        sp128.x -= cam_cross;
        dDbVw_drawLineXlu(sp134, sp128, color, 1, 0xC);

        sp134.x -= cam_cross;
        sp128.x += cam_cross;
        sp134.y += cam_cross;
        sp128.y -= cam_cross;
        dDbVw_drawLineXlu(sp134, sp128, color, 1, 0xC);

        sp134.y -= cam_cross;
        sp128.y += cam_cross;
        sp134.z += cam_cross;
        sp128.z -= cam_cross;
        dDbVw_drawLineXlu(sp134, sp128, color, 1, 0xC);

        cam_cross = mCamSetup.CenterBall();

        GXColor sp50 = {0xFF, 0x80, 0x80, 0x80};
        if (field_0xc08 != 0) {
            sp50.g = 0xFF;
        }

        if (field_0xc0c != 0) {
            sp50.b = 0xFF;
        }

        dDbVw_drawSphereXlu(mRunKey.mCenter, cam_cross, sp50, 1);
        rt = 1;
    }

    if (mCamSetup.CheckFlag(0x10)) {
        switch (mLchkNum) {
        case 2:
            // dComIfG_Bgsp().DrawPoly(mLchk[1], l_color2);
            /* fallthrough */
        case 1:
            // dComIfG_Bgsp().DrawPoly(mLchk[0], l_color1);
            break;
        }
    }

    return rt;
}

JUtility::TColor get_color(JUtility::TColor color) {
    static JUtility::TColor colors[] = {
        JUtility::TColor(0xFF, 0xFF, 0xFF, 0xFF),
        JUtility::TColor(0xFF, 0xFF, 0xC0, 0xFF),
        JUtility::TColor(0xFF, 0xC0, 0xC0, 0xFF),
        JUtility::TColor(0xC0, 0xFF, 0xC0, 0xFF),
        JUtility::TColor(0xC0, 0xC0, 0xFF, 0xFF),
        JUtility::TColor(0xFF, 0xFF, 0x80, 0xFF),
        JUtility::TColor(0xFF, 0x80, 0x80, 0xFF),
        JUtility::TColor(0xC0, 0xFF, 0x80, 0xFF),
        JUtility::TColor(0x80, 0x80, 0xFF, 0xFF),
    };

    (void)(u32)color;
    int var_r30 = 1;

    if (var_r30 && (u32)color < 9) {
        return colors[(u32)color & ~0x80000000];
    }

    return color;
}

int dDbgCamera_c::Report(int x, int y, JUtility::TColor color, const char* mesg, ...) {
    char buffer[0x100];

    static JUtility::TColor ShadowDarkColor(0, 0, 0, 0x80);

    va_list list;
    va_start(list, mesg);
    vsnprintf(buffer, sizeof(buffer), mesg, list);
    va_end(list);

    JUTDbPrint::getManager()->flush();

    JUTDbPrint::getManager()->setCharColor(ShadowDarkColor);
    JUTReport(x + 2, y + 2, buffer);

    JUTDbPrint::getManager()->flush();

    JUTDbPrint::getManager()->setCharColor(get_color(color));
    JUTReport(x, y, buffer);

    JUTDbPrint::getManager()->flush();
    JUTDbPrint::getManager()->setCharColor(g_HIO.mColor);
    return 1;
}

void dDbgCamera_c::OpenFile() {
    const char ext[] = "イベントファイル(*.zev)\0*.zev\0その他のファイル(*.*)\0*.*\0";
    mFile.open("dbgcam.zev", 2, ext, NULL, NULL, NULL);
}

void dDbgCamera_c::PrintFile(char* string, ...) {
    char buffer[0x100];

    va_list args;
    va_start(args, string);
    vsnprintf(buffer, sizeof(buffer), string, args);
    va_end(args);

    mFile.writeData(buffer, strlen(buffer));
}

void dDbgCamera_c::CloseFile() {
    mFile.close();
}

#endif
