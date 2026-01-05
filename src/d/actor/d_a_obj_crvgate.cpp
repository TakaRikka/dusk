/**
 * @file d_a_obj_crvgate.cpp
 *
 */

#include "d/dolzel_rel.h"  // IWYU pragma: keep

#include "d/actor/d_a_obj_crvgate.h"
#include "d/d_s_play.h"

const static dCcD_SrcSph ccSphSrc = {
    {
        {0x0, {{0x0, 0x0, 0x13}, {0x100000, 0x1F}, 0x75}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1},                 // mGObjTg
        {0x0},                                              // mGObjCo
    },                                                      // mObjInf
    {
        {{0.0f, 100.0f, 0.0f}, 60.0f},  // mSph
    }  // mSphAttr
};

static char* l_arcName = "CrvGate";

namespace {
static u16 s_CoCount = 0;
}

void daObjCRVGATE_c::initCcCylinder() {
    mStts.Init(0xff, 0xff, this);

    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::transM(i * 95.0f + 100.0f, 0, 0);
        mDoMtx_stack_c::multVecZero(&mXyzSph[i]);
    }

    mSph[0].Set(ccSphSrc);
    mSph[0].SetStts(&mStts);
    mSph[0].SetC(mXyzSph[0]);

    mSph[1].Set(ccSphSrc);
    mSph[1].SetStts(&mStts);
    mSph[1].SetC(mXyzSph[1]);

    mSph[2].Set(ccSphSrc);
    mSph[2].SetStts(&mStts);
    mSph[2].SetC(mXyzSph[2]);
}

void daObjCRVGATE_c::setCcCylinder() {
    for (int i = 0; i < 3; i++) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_stack_c::YrotM(mDoorAngle);
        mDoMtx_stack_c::transM(i * (nREG_F(8) + 95.0f) + 80.0f + nREG_F(9), nREG_F(10) + 130.0f, 0);
        mDoMtx_stack_c::multVecZero(&mXyzSph[i]);
    }

    mSph[0].SetC(mXyzSph[0]);
    dComIfG_Ccsp()->Set(&mSph[0]);

    mSph[1].SetC(mXyzSph[1]);
    dComIfG_Ccsp()->Set(&mSph[1]);

    mSph[2].SetC(mXyzSph[2]);
    dComIfG_Ccsp()->Set(&mSph[2]);
}

static int daObjCRVGATE_Create(fopAc_ac_c* i_this) {
    daObjCRVGATE_c* a_this = (daObjCRVGATE_c*)i_this;
    fpc_ProcID _ = fopAcM_GetID(i_this);
    return a_this->create();
}

static int daObjCRVGATE_Delete(daObjCRVGATE_c* i_this) {
    fpc_ProcID _ = fopAcM_GetID(i_this);
    i_this->MoveBGDelete();
    return 1;
}

int daObjCRVGATE_c::checkOpen() {
    if (dComIfGs_getKeyNum() == 0) {
        return 0;
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
    s16 height_diff = current.angle.y - player->current.angle.y;

    if (abs(height_diff) < 0x5000) {
        return 0;
    }

    if (mPos.absXZ(player->current.pos) < 200.0f) {
        return 1;
    } else {
        return 0;
    }
}

void daObjCRVGATE_c::actionStartEvent() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* camera_body = dCam_getBody();

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xffff, 0);
        eventInfo.onCondition(0x0);
    } else {
        camera->mCamera.Stop();

        field_0x5bc = 0x0;
        mFovY = camera_body->Fovy();

        mCamCenter = camera_body->Center();
        mCamEye = camera_body->Eye();

        camera->mCamera.SetTrimSize(3);
        mEventID = 2;
    }
}

void daObjCRVGATE_c::actionWaitEvent() {
    if (eventInfo.checkCommandDoor()) {
        daPy_py_c* player = daPy_getPlayerActorClass();
        player->changeOriginalDemo();
        player->changeDemoMode(1, 0, 0, 0);

        mEventID = 1;
        dComIfGp_setItemKeyNumCount(-1);
        mEventFlag = true;
        Demo_Set();

        Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_OPEN, &mPos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);

        fopAcM_OffStatus(this, 0x100);
    } else {
        if (checkOpen() != 0) {
            eventInfo.onCondition(4);
        }
    }
}

void daObjCRVGATE_c::Demo_Set() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* player_pos = &fopAcM_GetPosition(player);

    s16 target_angle = cLib_targetAngleY(&current.pos, player_pos);

    f32 _ = current.pos.absXZ(*player_pos);  // ?

    mCamEye.set(current.pos.x + cM_scos(target_angle + 0x1000) * 300.0f, current.pos.y + 500.0f,
                current.pos.z - cM_ssin(target_angle + 0x1000) * 300.0f);
    mCamCenter.set(current.pos.x, current.pos.y + 200.0f, current.pos.z);
}

void daObjCRVGATE_c::SetOpen() {
    mStatus = 0x0;
}

void daObjCRVGATE_c::actionDemoEvent() {
    if (!mFlagDemoEventPlay) {
        mPosAccel.y += -5.0f;
        mPos.y += mPosAccel.y;
        if (mPos.y <= mMinHeight + 55.0f) {
            mFlagDemoEventPlay = true;
            mPos.y = mMinHeight + 55.0f;
            daObjEff::Act_c::make_land_smoke(&mPos, 0.5f);
            mMoveAngle.z = 0x500;
        }
    } else {
        cLib_chaseAngleS(&mDoorOpenAngle.x, 0x4000, mMoveAngle.z);

        mMoveAngle.z += (s16)0x300;
        if (mDoorOpenAngle.x == 0x4000) {
            mEventID = 3;
            camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

            camera->mCamera.Reset(mCamCenter, mCamEye);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);

            dComIfGp_event_reset();

            SetOpen();
            mpDoorPair->SetOpen();
        }
    }
}

void daObjCRVGATE_c::event_proc_call() {
    switch (mEventID) {
    case 0:
        actionWaitEvent();
        break;
    case 1:
        actionStartEvent();
        break;
    case 2:
        actionDemoEvent();
        break;
    case 3:
        mEventFlag = false;
        break;
    }
}

int daObjCRVGATE_c::CheckVec() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* player_pos = &fopAcM_GetPosition(player);
    cXyz pos = *player_pos;

    Mtx inv_mtx;
    mDoMtx_inverse(mpModelGate->getBaseTRMtx(), inv_mtx);

    mDoMtx_stack_c::copy(inv_mtx);
    mDoMtx_stack_c::multVec(&pos, &pos);

    if (pos.z > 0) {
        return 1;
    } else {
        return 0;
    }
}

void daObjCRVGATE_c::KeyVib() {
    mMoveAngle.x -= (s16)0x21;
    mMoveAngle.z += mMoveAngle.x;
    cLib_chaseAngleS(&mMoveAngle.y, 0, 0x150);

    if (mMoveAngle.y != 0x0) {
        if (mKeyParam == 0x00) {
            mpDoorPair->mDoorOpenAngle.x = mMoveAngle.y * cM_ssin(mMoveAngle.z);
            mDoorOpenAngle.x = mMoveAngle.y * cM_ssin(mMoveAngle.z);

            if (mDoorOpenAngle.x > 0) {
                mpDoorPair->mDoorOpenAngle.x = -mDoorOpenAngle.x;
                mDoorOpenAngle.x = -mDoorOpenAngle.x;
            }
        } else {
            mDoorOpenAngle.x = mMoveAngle.y * cM_ssin(mMoveAngle.z);
            if (mDoorOpenAngle.x > 0) {
                mDoorOpenAngle.x = -mDoorOpenAngle.x;
            }
        }
    } else {
        mFlagDoorStuck = false;
    }
}

void daObjCRVGATE_c::DoorVib() {
    mDoorVib.y -= (s16)(KREG_S(1) + 0x100);
    mDoorVib.z += mDoorVib.y;
    cLib_chaseAngleS(&mDoorVib.x, 0, 0x40);

    if (mDoorVib.x != 0) {
        if (mKeyParam == 0x01) {
            mDoorAngle = mDoorVib.x * cM_ssin(mDoorVib.z);
            mpDoorPair->mDoorAngle = -mDoorAngle;
        } else {
            mDoorAngle = -mDoorVib.x * cM_ssin(mDoorVib.z);
            mpDoorPair->mDoorAngle = -mDoorAngle;
        }
    }
}

void daObjCRVGATE_c::VibStop() {
    mDoorVib.y = 0;
    mDoorVib.x = 0;
}

void daObjCRVGATE_c::CloseVibration() {
    if (mStatus != 0x4) {
        KeyVib();
    }
    DoorVib();
}

static f32 dummyFloat1() {
    return 350.0f;
}

static f32 dummyFloat2() {
    return 120.0f;
}

void daObjCRVGATE_c::CloseAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* player_pos = &fopAcM_GetPosition(player);
    csXyz og_DoorOpenAngle(mDoorOpenAngle.x, mDoorOpenAngle.y, mDoorOpenAngle.z);
    int local_44 = 0;

    if (mXyzSph[1].absXZ(*player_pos) < 300.0f && player->checkFrontRollCrash()) {
        if (mKeyParam == 0x01) {
            mMoveAngle.y = -0x3000;
            mMoveAngle.x = 0x1500;
        } else {
            mpDoorPair->mMoveAngle.y = -0x4000;
            mpDoorPair->mMoveAngle.x = 0x1500;
        }

        if (!mFlagGateClosed) {
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_ATTACK, &mPos, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
        } else {
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
        }

        mDoorVib.y = 0x3000;
        mDoorVib.x = 0x500;
        mpDoorPair->VibStop();
        mFlagDoorStuck = true;
    }

    for (int i = 0; i < 3; i++) {
        if (local_44 == 0) {
            cLib_addCalcAngleS(&mDoorOpenAngle.x, 0, 2, 0x2000, 0x400);
        }

        if (mSph[i].ChkTgHit()) {
            if (!mFlagGateClosed) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_ATTACK, &mPos, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            } else if (mKeyParam == 0xff) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
            }

            if (mKeyParam == 0x01) {
                mMoveAngle.y = -0x3000;
                mMoveAngle.x = 0x1500;
            } else {
                mpDoorPair->mMoveAngle.y = -0x4000;
                mpDoorPair->mMoveAngle.x = 0x1500;
            }

            mDoorVib.y = 0x3000;
            mDoorVib.x = 0x500;
            mFlagDoorStuck = true;
            mpDoorPair->VibStop();
            break;
        }
        if (mSph[i].ChkCoHit()) {
            fopAc_ac_c* hitObj = dCc_GetAc(mSph[i].GetCoHitObj()->GetAc());
            fopAc_ac_c* boar_ac;

            if (fopAcM_GetName(hitObj) == PROC_E_WB && player->checkBoarRun() != 0 &&
                fopAcM_SearchByName(PROC_E_WB, &boar_ac) != 0)
            {
                cXyz* boar_speed = &fopAcM_GetSpeed(boar_ac);
                daPy_py_c* player = daPy_getPlayerActorClass();
                cXyz* player_pos = &fopAcM_GetPosition(player);

                if (dComIfGp_event_runCheck()) {
                    cXyz* boar_speed = &fopAcM_GetSpeed(boar_ac);
                    daPy_py_c* player = daPy_getPlayerActorClass();
                    cXyz* player_pos = &fopAcM_GetPosition(player);

                    if (player_pos->absXZ(mXyzSph[i]) < KREG_F(0) + 490.0f &&
                        boar_speed->absXZ() > 0.0f)
                    {
                        // Start escape event, destroying the door

                        dComIfGp_getVibration().StartShock(5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                        fopAcM_seStartCurrent(this, 0x802C5, 0);
                        mpDoorPair->mStatus = 0x2;
                        mStatus = 0x2;
                        mFlagDoorStuck = true;
                        mpDoorPair->mFlagDoorStuck = true;
                        mDestructionCount = 0;

                        if (mKeyParam == 0x01) {
                            mDoorSwingTargetAngle = -0x7000;
                        } else {
                            mpDoorPair->mDoorSwingTargetAngle = -0x7000;
                        }

                        mPosAccel.set(10.0f, KREG_F(0) + 70.0f, -30.0f);
                    }
                }
            }
        }
    }

    CloseVibration();

    if (mStatus != 0x04) {
        mPos.set(current.pos.x + cM_scos(shape_angle.y + mDoorAngle) * 350.0f,
                 120.0f + current.pos.y,
                 current.pos.z - cM_ssin(shape_angle.y + mDoorAngle) * 350.0f);
    } else {
        mDoorOpenAngle = og_DoorOpenAngle;
    }
}

void daObjCRVGATE_c::OpenAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz* player_pos = &fopAcM_GetPosition(player);
    s16 doorAngle1 = mDoorAngle;
    s16 doorAngle2 = mDoorAngle;
    int local_3c = 0;

    if (CheckVec() != 0) {
        if (mXyzSph[1].absXZ(*player_pos) < 300.0f && player->checkFrontRollCrash()) {
            mDoorSwingTargetAngle = mDoorAngle + 0x1000;
            mFlagDoorMove = true;
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
        }

        for (int i = 0; i < 3; i++) {
            if (mSph[i].ChkTgHit() != 0) {
                mDoorSwingTargetAngle = mDoorAngle + 0x1000;
                mFlagDoorMove = true;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);

            } else {
                if (mSph[i].ChkCoHit() != 0) {
                    local_3c++;
                    fopAc_ac_c* hitObj = dCc_GetAc(mSph[i].GetCoHitObj()->GetAc());
                    if (fopAcM_GetName(hitObj) == PROC_E_WB) {
                        if (mKeyParam == 0xff) {
                            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_BREAK, &current.pos, 0, 0,
                                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
                        }
                        daPy_py_c* player = daPy_getPlayerActorClass();
                        mDoorSwingTargetAngle = mDoorAngle + fopAcM_GetSpeedF(player) * 288.0f;
                        mFlagDoorMove = true;
                    } else {
                        doorAngle1 = mDoorAngle + fopAcM_GetSpeedF(player) * (nREG_S(0) + 112);
                        if (doorAngle1 > 0x4000) {
                            doorAngle1 = 0x4000;
                        }

                        if (doorAngle1 != doorAngle2 && !field_0x5a5) {
                            field_0x5a5 = true;
                            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0,
                                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
                        } else if (doorAngle1 == doorAngle2) {
                            field_0x5a5 = false;
                        }
                    }

                    break;
                }
            }
        }

        if (doorAngle1 == doorAngle2) {
            field_0x5a5 = false;
        }

        if (mFlagDoorMove == true) {
            if (mDoorAngle > 0x4000) {
                mDoorSwingTargetAngle = 0x4000 - (mDoorSwingTargetAngle - 0x4000);
            }

            cLib_addCalcAngleS(&mDoorAngle, mDoorSwingTargetAngle, 6, 0x2000, 0x50);
            if ((u16)(mDoorAngle - mDoorSwingTargetAngle) < 0x100) {
                mFlagDoorMove = false;
            }
        } else {
            cLib_addCalcAngleS(&mDoorAngle, doorAngle1, 6, 0x2000, 0x50);
        }
    } else {
        if (mXyzSph[1].absXZ(*player_pos) < 300.0f && player->checkFrontRollCrash()) {
            mDoorSwingTargetAngle = -0x1000;  //??
            mDoorSwingTargetAngle = (mDoorAngle - 0x1000);
            mFlagDoorMove = true;
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
        }

        for (int i = 0; i < 3; i++) {
            if (mSph[i].ChkTgHit() != 0) {
                mDoorSwingTargetAngle = mDoorAngle - 0x1000;
                mFlagDoorMove = true;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);

            } else {
                if (mSph[i].ChkCoHit() != 0) {
                    local_3c++;
                    fopAc_ac_c* hitObj = dCc_GetAc(mSph[i].GetCoHitObj()->GetAc());
                    if (fopAcM_GetName(hitObj) == PROC_E_WB) {
                        daPy_py_c* player = daPy_getPlayerActorClass();

                        mDoorSwingTargetAngle = mDoorAngle - fopAcM_GetSpeedF(player) * 288.0f;
                        mFlagDoorMove = true;

                        if (mKeyParam == 0xff) {
                            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_BREAK, &current.pos, 0, 0,
                                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
                        }
                    } else {
                        doorAngle1 = mDoorAngle - fopAcM_GetSpeedF(player) * (nREG_S(0) + 112);

                        if (mKeyParam == 0x01) {
                            if (doorAngle1 < -0x2300) {
                                doorAngle1 = -0x2300;
                            }
                        } else if (doorAngle1 < -0x4000) {
                            doorAngle1 = -0x4000;
                        }

                        if (doorAngle1 != doorAngle2 && !field_0x5a5) {
                            field_0x5a5 = true;
                            Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0,
                                                     1.0f, 1.0f, -1.0f, -1.0f, 0);
                        } else {
                            if (doorAngle1 == doorAngle2) {
                                field_0x5a5 = false;
                            }
                        }
                    }
                }
            }
        }

        if (doorAngle1 == doorAngle2) {
            field_0x5a5 = false;
        }

        if (mFlagDoorMove == 1) {
            if (mKeyParam == 0x01) {
                if (mDoorAngle < -0x2300) {
                    mDoorSwingTargetAngle = -0x2300 - (mDoorSwingTargetAngle + 0x2300);
                }
            } else if (mDoorAngle < -0x4000) {
                mDoorSwingTargetAngle = -0x4000 - (mDoorSwingTargetAngle + 0x4000);
            }

            cLib_addCalcAngleS(&mDoorAngle, mDoorSwingTargetAngle, 6, 0x2000, 0x50);
            if ((u16)(mDoorAngle - mDoorSwingTargetAngle) < 0x100) {
                mFlagDoorMove = false;
            }
        } else {
            cLib_addCalcAngleS(&mDoorAngle, doorAngle1, 6, 0x2000, 0x50);
        }
    }
}

void daObjCRVGATE_c::HakaiMotion() {
    if (mDoorAngle < -0x6000) {
        mDoorSwingTargetAngle = -0x6000 - (mDoorSwingTargetAngle + 0x6000);
    }

    cLib_addCalcAngleS(&mDoorAngle, mDoorSwingTargetAngle, 6, 0x2000, 0x50);
    cLib_addCalcAngleS(&mpDoorPair->mDoorAngle, (int)-mDoorSwingTargetAngle, 6, 0x2000, 0x50);

    if (mPos.y <= mMinHeight) {
        mDestructionCount++;
        mPos.y = mMinHeight;
        mDoorOpenAngle.x = 0x4000;
        if (mDestructionCount > 3) {
            SetOpen();
            mpDoorPair->SetOpen();
        } else {
            mPosAccel.y = mPosAccel.y * -0.4f;
        }
    }
}

void daObjCRVGATE_c::DoorAction() {
    switch (mStatus) {
    case 0:
        OpenAction();
        break;
    case 1:
        CloseAction();
        break;
    case 2:
        if (mKeyParam == 0x01) {
            HakaiMotion();
        }
        break;
    case 4:
        B_CloseAction();
        break;
    default:
        break;
    }
}

static f32 const dummyFloat3() {
    return 50.0f;
}

static f32 const dummyFloat4() {
    return -10.0f;
}

void daObjCRVGATE_c::B_CloseAction() {
    if (mDoorStep > 0x10) {
        cLib_chaseAngleS(&mDoorAngle, mDoorTargetAngle, mDoorStep);
        if (mDoorAngle == mDoorTargetAngle) {
            if (!mFlagGateClosed) {
                mFlagGateClosed = true;
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CLOSE, &current.pos, 0, 0, 1.0f, 1.0f,
                                         -1.0f, -1.0f, 0);
            }
            mDoorStep *= (JREG_F(2) + 0.7f);
            mDoorTargetAngle = -0.7 * mDoorTargetAngle;
        }
    } else {
        CloseAction();
    }
}

void daObjCRVGATE_c::SetB_Close() {
    Z2GetAudioMgr()->seStart(Z2SE_OBJ_CRVN_GATE_CREAK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                             0);

    if (mKeyParam == 0x01) {
        mpDoorPair->SetB_Close();
    }

    mStatus = 0x4;
    mDoorStep = JREG_S(0) + 3096.0f;

    if (abs(mDoorStep) < 0x500) {
        mDoorStep = 0x500;
    }

    if (mDoorAngle > 0x0) {
        mDoorTargetAngle = -(JREG_S(1) + 2608.0f);
    } else {
        mDoorTargetAngle = JREG_S(1) + 2608.0f;
    }
}

void daObjCRVGATE_c::setBaseMtx() {
    cXyz local(0.0f, 0.0f, 35.0f);
    mDoMtx_stack_c::transS(mPos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::transM(local);

    if (mFlagDemoEventPlay == true) {
        mDoMtx_stack_c::transM(0.0f, nREG_F(5) + -55.0f, 0.0f);
    }

    mDoMtx_stack_c::ZXYrotM(mDoorOpenAngle);

    if (mFlagDemoEventPlay == true) {
        mDoMtx_stack_c::transM(0.0f, nREG_F(5) + 55.0f, 0.0f);
    }

    mpModelKey->setBaseTRMtx(mDoMtx_stack_c::get());
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::YrotM(mDoorAngle);
    mpModelGate->setBaseTRMtx(mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), mBgMtx);
}

static int daObjCRVGATE_Draw(daObjCRVGATE_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daObjCRVGATE_Execute(daObjCRVGATE_c* i_this) {
    return i_this->MoveBGExecute();
}

int daObjCRVGATE_c::CreateHeap() {
    J3DModelData* model_data_gate =
        (J3DModelData*)dComIfG_getObjectRes(l_arcName, "CaravanGate.bmd");

    JUT_ASSERT(762, model_data_gate != NULL)

    mpModelGate = mDoExt_J3DModel__create(model_data_gate, 0x80000, 0x11000084);

    if (mpModelGate == NULL) {
        return 0;
    }

    J3DModelData* model_data_key;
    if (mKeyParam == 0x00) {
        model_data_key = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "CaravanKey.bmd");
        mpModelKey = mDoExt_J3DModel__create(model_data_key, 0x80000, 0x11000084);
    }

    if (mpModelGate == NULL) {  // ?
        return 0;
    }

    return 1;
}

void daObjCRVGATE_c::SetDoor() {
    cXyz child_pos(current.pos.x + cM_scos(shape_angle.y) * 700.0f, current.pos.y,
                   current.pos.z - cM_ssin(shape_angle.y) * 700.0f);

    mPos.set(current.pos.x + cM_scos(shape_angle.y) * 350.0f, 120.0f + current.pos.y,
             current.pos.z - cM_ssin(shape_angle.y) * 350.0f);

    csXyz child_angle(shape_angle.x, shape_angle.y + 0x8000, shape_angle.z);

    if (mKeyParam == 0xff) {
        attention_info.position = mPos;
        attention_info.flags = fopAc_AttnFlag_DOOR_e;
        attention_info.distances[fopAc_attn_DOOR_e] = 0x0F;

        mKeyParam = 0x01;

        // Create the second (identical) part of the door (swinging gate).
        mDoorPairProcID = fopAcM_createChild(PROC_Obj_CRVGATE, fopAcM_GetID(this), 1, &child_pos,
                                             fopAcM_GetRoomNo(this), &child_angle, NULL, -1, 0);
        mDoorY = shape_angle.y;
    } else {
        mKeyParam = 0x00;
        mDoorY = shape_angle.y;
        fopAcM_SearchByID(parentActorID, (fopAc_ac_c**)&mpDoorPair);
    }
}

int daObjCRVGATE_c::create() {
    fopAcM_ct(this, daObjCRVGATE_c);
    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, l_arcName);

    if (phase == cPhs_COMPLEATE_e) {
        gravity = nREG_F(0) + -9.0f;

        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "CaravanGate.dzb");
        if (dzb_id == -1) {
            // "dzb data not found!<%s>"
            OS_REPORT("dzbデータが見つかりませんでした!<%s>\n\n", l_arcName);
            JUT_ASSERT(783, dzb_id != -1)
        }
        phase =
            (cPhs__Step)MoveBGCreate(l_arcName, dzb_id, dBgS_MoveBGProc_TypicalRotY, 0x1480, NULL);

        if (phase == cPhs_ERROR_e) {
            return phase;
        }

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);

        initCcCylinder();
        fopAcM_SetMtx(this, mpModelGate->getBaseTRMtx());
        mKeyParam = (u8)fopAcM_GetParam(this);
        SetDoor();

        mFlagDemoEventPlay = false;
        mEventID = 0;
        mStatus = 0x1;
        mPosAccel.set(0.0f, 0.0f, 0.0f);
        mDoorAngle = 0;
        mPosTmp = mPos;

        for (int i = 0; i < 3; i++) {
            mSph[i].OnTgShield();
        }

        fopAcM_setCullSizeBox(this, -100.0f, 0.0, -200.0f, 500.0, 500.0, 200.0);

        cXyz acStack_40(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        if (fopAcM_gc_c::gndCheck(&acStack_40)) {
            mMinHeight = fopAcM_gc_c::getGroundY();
        }

        mFlagGateClosed = false;
        field_0x5a5 = false;
        daObjCRVGATE_Execute(this);
    }

    return phase;
}

static int daObjCRVGATE_IsDelete(daObjCRVGATE_c* i_this) {
    return 1;
}

int daObjCRVGATE_c::Create() {
    fopAcM_setCullSizeBox(this, -1000.0f, -500.0f, -1000.0f, 1000.0f, 500.0f, 1000.0f);
    return cPhs_COMPLEATE_e;
}

int daObjCRVGATE_c::Execute(Mtx** param_0) {
    s_CoCount = 0;

    if (mKeyParam == 0x01) {
        if (fopAcM_SearchByID(mDoorPairProcID) != NULL && !mDoorPairLoaded) {
            fopAcM_SearchByID(mDoorPairProcID, (fopAc_ac_c**)&mpDoorPair);
            mDoorPairLoaded = true;
            mEventFlag = false;
        }
        if (!mFlagDoorStuck) {
            event_proc_call();
        }
    }

    if (!mEventFlag) {
        DoorAction();
    }

    *param_0 = &mBgMtx;
    setBaseMtx();
    if (field_0x66c != 0) {
        field_0x66c--;
    }
    setCcCylinder();

    return 1;
}

int daObjCRVGATE_c::Draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModelGate, &tevStr);
    if (mKeyParam == 0x01) {
        g_env_light.setLightTevColorType_MAJI(mpModelKey, &tevStr);
    }

    dComIfGd_setListBG();

    mDoExt_modelUpdateDL(mpModelGate);
    if (mKeyParam == 0x01) {
        mDoExt_modelUpdateDL(mpModelKey);
    }

    return 1;
}

int daObjCRVGATE_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, l_arcName);
    return 1;
}

static actor_method_class l_daObjCRVGATE_Method = {
    (process_method_func)daObjCRVGATE_Create,  (process_method_func)daObjCRVGATE_Delete,
    (process_method_func)daObjCRVGATE_Execute, (process_method_func)daObjCRVGATE_IsDelete,
    (process_method_func)daObjCRVGATE_Draw,
};

actor_process_profile_definition g_profile_Obj_CRVGATE = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_CRVGATE,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObjCRVGATE_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    466,                     // mPriority
    &l_daObjCRVGATE_Method,  // sub_method
    0x00040180,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
