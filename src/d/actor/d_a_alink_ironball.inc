/**
 * d_a_alink_ironball.inc
 * Player Ball & Chain action handling
 */

#include "d/actor/d_a_alink.h"

cXyz* daAlink_c::getIronBallCenterPos() {
    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        return &mIronBallCenterPos;
    }

    return NULL;
}

int daAlink_c::getIronBallBgHit() const {
    if (mEquipItem == fpcNm_ITEM_IRONBALL) {
        return field_0x301c;
    }

    return 0;
}

bool daAlink_c::checkIronBallThrowMode() const {
    return mEquipItem == fpcNm_ITEM_IRONBALL &&
           (mItemVar0.field_0x3018 == 4 || mItemVar0.field_0x3018 == 5 || mItemVar0.field_0x3018 == 6 || mItemVar0.field_0x3018 == 3);
}

bool daAlink_c::checkIronBallThrowReturnMode() const {
    return mEquipItem == fpcNm_ITEM_IRONBALL && (mItemVar0.field_0x3018 == 4 || mItemVar0.field_0x3018 == 5 || mItemVar0.field_0x3018 == 6 ||
                                      mItemVar0.field_0x3018 == 7 || mItemVar0.field_0x3018 == 3);
}

bool daAlink_c::checkIronBallReturn() const {
    return mEquipItem == fpcNm_ITEM_IRONBALL && mItemVar0.field_0x3018 == 7;
}

bool daAlink_c::checkIronBallGroundStop() const {
    return mEquipItem == fpcNm_ITEM_IRONBALL && mItemVar0.field_0x3018 != 0 && mItemVar0.field_0x3018 != 8 && field_0x301e == 0 &&
           field_0x301e < 10;
}

void daAlink_c::setIronBallWaitUpperAnime(int param_0) {
    if (!checkIronBallWaitAnime() && !checkIronBallPreSwingAnime() &&
        (mItemVar0.field_0x3018 == 0 || mItemVar0.field_0x3018 == 8) && (param_0 || checkNoUpperAnime())) {
        setUpperAnimeBaseSpeed(0x19C, mpHIO->mItem.mIronBall.m.mWaitAnmSpeed,
                               mpHIO->mBasic.m.mBasicInterpolation);
        setFacePriBck(0xE8);
        field_0x2f96 = 254;
        field_0x2f97 = 5;
        mItemVar0.field_0x3018 = 0;
    }
}

void daAlink_c::checkIronBallDelete() {
    if (mProcID == PROC_IRON_BALL_SUBJECT || mProcID == PROC_IRON_BALL_MOVE ||
        mProcID == PROC_IRON_BALL_THROW || mProcID == PROC_IRON_BALL_RETURN) {
        return;
    }

    if (mItemVar0.field_0x3018 == 1 || mItemVar0.field_0x3018 == 2 || mItemVar0.field_0x3018 == 3 ||
        ((mItemVar0.field_0x3018 == 0 || mItemVar0.field_0x3018 == 8) && !checkIronBallWaitAnime() &&
         !checkEquipAnime() &&
         (!checkModeFlg(4) || checkModeFlg(0x70C52) || !mLinkAcch.ChkGroundHit()))) {
        deleteEquipItem(1, 0);
    }
}

void daAlink_c::setIronBallReturn(int param_0) {
    if (param_0 && field_0x301e != 0) {
        field_0x301e--;
    } else if (mItemVar0.field_0x3018 != 7) {
        mItemVar0.field_0x3018 = 7;
        field_0x301e = 0;
        seStartOnlyReverb(Z2SE_AL_IB_PULLBACK);
    }
}

void daAlink_c::setIronBallBgCheckPos() {
    cXyz sp8;
    mDoMtx_stack_c::transS(*mIronBallChainPos);
    mDoMtx_stack_c::ZXYrotM(*mIronBallChainAngle);
    mDoMtx_stack_c::multVec(&l_ironBallCenterVec, &sp8);

    mIronBallBgChkPos.set(sp8.x, (sp8.y - 32.0f) - mSearchBallScale, sp8.z);
    cLib_chaseF(&mSearchBallScale, 0.0f, 2.0f);
}

void daAlink_c::setIronBallModel() {
    JKRHeap* prev_heap = setItemHeap();

    mHeldItemModel = initModelEnv(loadAramBmd(0x311, 0x2800), 0);
    mpItemModelData = loadAramBmd(0x317, 0x1000);
    mpHookChain = new hsChainShape_c();

    mIronBallChainPos = new cXyz[102];
    mIronBallChainAngle = new csXyz[102];
    field_0x3848 = new cXyz[102];

    field_0x0774 = new dBgS_AcchCir();
    field_0x0778 = new dBgS_ObjAcch();
    mpHookSound = new Z2SoundObjSimple();
    
    mpHookshotLinChk = new dBgS_ObjLinChk();
    mpHookshotLinChk->OffFullGrp();
    mpHookshotLinChk->OnWaterGrp();

    field_0x780 = new dBgS_ObjLinChk();
    field_0x2f1c = new dPaPo_c();
    field_0x2f1c->init(field_0x0778, 32.0f, 64.0f);

    mDoExt_setCurrentHeap(prev_heap);

    mpHookChain->setUserArea((uintptr_t)this);

    field_0xFB8.SetAtType(AT_TYPE_IRON_BALL);
    field_0xFB8.SetAtHitMark(3);
    field_0xFB8.SetAtSe(dCcD_SE_HAMMER);
    field_0xFB8.SetAtAtp(2);
    field_0xFB8.SetAtMtrl(dCcD_MTRL_NONE);
    field_0xFB8.SetR(mpHIO->mItem.mIronBall.m.mAttackRadius);

    csXyz* var_r28 = mIronBallChainAngle;
    for (int i = 0; i < 102; i++, var_r28++) {
        if (i < 2) {
            var_r28->set(0, shape_angle.y, i * 0x3000);
        } else {
            var_r28->set(0, shape_angle.y, var_r28[-1].z + 0x3000);
        }

        field_0x3848[i] = cXyz::Zero;
        mIronBallChainPos[i] = mLeftHandPos;
    }

    mItemVar0.field_0x3018 = 0;
    mItemMode = 6;

    mIronBallBgChkPos = mLeftHandPos;
    field_0x3810 = mLeftHandPos;

    field_0x0774->SetWall(32.0f, 32.0f);
    field_0x0778->Set(&mIronBallBgChkPos, &field_0x3810, this, 1, field_0x0774, field_0x3848, mIronBallChainAngle, mIronBallChainAngle);
    field_0x0778->ClrRoofNone();
    field_0x0778->SetRoofCrrHeight(64.0f);
    field_0x0778->OnLineCheck();
    field_0x0778->ClrObj();
    field_0x0778->SetIronBall();
    
    field_0x780->ClrObj();
    field_0x780->SetIronBall();

    field_0x32d4 = 0;

    mpHookSound->init(&mIronBallCenterPos, 1);
    mZ2Link.setUsingIronBall(true);

    field_0x301c = 0;
    field_0x3024 = 0;
    field_0x3026 = 0;

    field_0x173c.Init(0xDC, 0xFF, this);
    field_0x1778.Set(l_sphSrc);
    field_0x1778.SetStts(&field_0x173c);
    field_0x1778.OffTgSetBit();
    field_0x1778.OffAtSetBit();
    field_0x1778.SetR(40.0f);
    field_0x1778.SetCoIGrp(8);
    field_0x1778.SetCoVsGrp(0x60);
}

int daAlink_c::setIronBallGroundVec(cXyz const* param_0, cXyz* param_1) {
    cXyz sp3C(param_0->x, param_0->y + 50.0f, param_0->z);

    if (fopAcM_gc_c::gndCheck(&sp3C)) {
        f32 var_f30 = (fopAcM_gc_c::getGroundY() + 4.0f) - param_0->y;
        if (var_f30 > 0.0f) {
            f32 var_f31 = param_1->absXZ();
            
            param_1->y += var_f30;

            if (var_f31 < 0.1f) {
                param_1->z += var_f30;
            } else {
                var_f31 = var_f30 / var_f31;
                param_1->x += param_1->x * var_f31;
                param_1->z += param_1->z * var_f31;
            }

            param_1->normalizeZP();
            return 1;
        }
    }

    return 0;
}

void daAlink_c::setIronBallChainPos() {
    cXyz sp2A4;
    cXyz sp298;
    cXyz sp28C;
    cXyz sp280;
    cXyz sp274;

    *(&mIronBallChainPos[mItemMode] + 1) = mHeldItemRootPos;
    *(&mIronBallChainAngle[mItemMode] + 1) = field_0x316c;

    cXyz* var_r27;
    cXyz* var_r29;
    cXyz* var_r26;
    int i;
    csXyz* sp3C;
    int sp38;
    int sp34;

    if (mItemVar0.field_0x3018 == 2) {
        var_r29 = mIronBallChainPos + 1;
        var_r27 = var_r29 - 1;
        var_r26 = field_0x3848 + 1;

        sp2A4 = mHookshotTopPos - *mIronBallChainPos;
        sp2A4.normalizeZP();

        for (i = 0; i < mItemMode; i++, var_r29++, var_r26++) {
            sp28C = *var_r29;
            *var_r29 = *var_r27 + (sp2A4 * 10.0f);

            *var_r26 = (*var_r29 - sp28C) * mpHIO->mItem.mIronBall.m.mChainSpeedRate;
            var_r27 = var_r29;
        }
    } else if (mItemVar0.field_0x3018 == 7) {
        var_r29 = &mIronBallChainPos[mItemMode];
        var_r26 = &field_0x3848[mItemMode];
        var_r27 = var_r29 + 1;

        sp38 = mItemMode * mpHIO->mItem.mIronBall.m.mReturnChainRate;
        sp34 = sp38;

        for (i = mItemMode; i >= 0; i--, var_r29--, var_r26--) {
            sp2A4 = (*var_r29 - *var_r27) + *var_r26;

            if (i == 0) {
                sp2A4.y += mpHIO->mItem.mIronBall.m.mReturnBallGravity;
            } else {
                sp2A4.y += mpHIO->mItem.mIronBall.m.mChainGravity;
            }

            if (sp38 >= 0) {
                sp2A4.y += mpHIO->mItem.mIronBall.m.mReturnChainAccelY;
                sp38--;
            }

            sp2A4.normalizeZP();
            *var_r29 = *var_r27 + (sp2A4 * 10.0f);
            var_r27 = var_r29;
        }

        setIronBallBgCheckPos();
        sp274 = mIronBallBgChkPos;
        field_0x0778->CrrPos(dComIfG_Bgsp());
    
        if (field_0x0778->ChkGroundHit()) {
            *mIronBallChainPos += mIronBallBgChkPos - sp274;
        }

        mIronBallChainPos->y += mSearchBallScale;

        var_r29 = mIronBallChainPos + 1;
        var_r27 = var_r29 - 1;
        var_r26 = field_0x3848 + 1;

        for (i = 0; i < mItemMode; i++, var_r29++, var_r26++) {
            sp28C = *var_r29;
            sp2A4 = (*var_r29 - *var_r27) + *var_r26;
            sp2A4.y += mpHIO->mItem.mIronBall.m.mChainGravity;
            if (i >= mItemMode - sp34) {
                sp2A4.y += mpHIO->mItem.mIronBall.m.mReturnChainAccelY;
            }

            sp280 = sp2A4;
            sp2A4.normalizeZP();

            *var_r29 = *var_r27 + (sp2A4 * 10.0f);

            if (setIronBallGroundVec(var_r29, &sp280)) {
                *var_r29 = *var_r27 + (sp280 * 10.0f);
            }

            var_r27 = var_r29;
            *var_r26 = (*var_r29 - sp28C) * mpHIO->mItem.mIronBall.m.mChainSpeedRate;
        }
    } else if (mItemVar0.field_0x3018 == 1 || mItemVar0.field_0x3018 == 3 || mItemVar0.field_0x3018 == 4) {
        daPy_frameCtrl_c* sp30 = &mUnderFrameCtrl[0];

        if (mItemVar0.field_0x3018 == 1) {
            f32 temp_f25 = sp30->getEnd() - 13.0f;
            f32 temp_f29 = (sp30->getFrame() - 13.0f) / temp_f25;
            s16 sp12 = -16384.0f - (14336.0f * temp_f29);

            mDoMtx_stack_c::ZXYrotS(mBodyAngle.x, shape_angle.y, 0);
            mDoMtx_stack_c::YrotM(sp12);
            mDoMtx_stack_c::multVec(&cXyz::BaseZ, &sp298);

            sp298 *= 15.0f * (1.0f + temp_f29);
            sp38 = 1000;
        } else if (mItemVar0.field_0x3018 == 4) {
            Vec sp268 = {0.0f, 0.0f, 0.0f};
            sp268.y = mpHIO->mItem.mIronBall.m.mThrowChainAccelY;
            sp268.z = mpHIO->mItem.mIronBall.m.mThrowChainAccelZ;

            mDoMtx_stack_c::ZXYrotS(mBodyAngle.x, shape_angle.y, 0);
            mDoMtx_stack_c::multVec(&sp268, &sp298);

            sp38 = 2.0f * (1.0f + sp30->getFrame());
        } else {
            sp38 = -1;
        }

        var_r29 = &mIronBallChainPos[mItemMode];
        var_r26 = &field_0x3848[mItemMode];
        var_r27 = var_r29 + 1;

        for (i = mItemMode; i >= 0; i--, var_r29--, var_r26--) {
            sp28C = *var_r29;
            sp2A4 = (*var_r29 - *var_r27) + *var_r26;

            if (i == 0) {
                sp2A4.y += mpHIO->mItem.mIronBall.m.mBallGravity;
            } else {
                sp2A4.y += mpHIO->mItem.mIronBall.m.mChainGravity;
            }

            if (sp38 >= 0) {
                sp2A4 += sp298;
                sp38--;
            }

            sp280 = sp2A4;
            sp2A4.normalizeZP();

            *var_r29 = *var_r27 + (sp2A4 * 10.0f);

            if (setIronBallGroundVec(var_r29, &sp280)) {
                *var_r29 = *var_r27 + (sp280 * 10.0f);
            }

            *var_r26 = (*var_r29 - sp28C) * mpHIO->mItem.mIronBall.m.mChainSpeedRate;
            var_r27 = var_r29;
        }

        sp2A4 = mIronBallCenterPos - *mIronBallChainPos;
        sp2A4.y += mpHIO->mItem.mIronBall.m.mBallGravity;

        if (sp38 >= 0) {
            sp2A4 += sp298;
        }

        s16 temp_r25 = sp2A4.atan2sX_Z();
        mIronBallChainAngle->set(sp2A4.atan2sY_XZ(), temp_r25, 0);
    } else {
        f32 var_f30;
        if (mItemVar0.field_0x3018 == 4 || mItemVar0.field_0x3018 == 5) {
            var_f30 = mpHIO->mItem.mIronBall.m.mThrowChainGravity;
        } else {
            var_f30 = mpHIO->mItem.mIronBall.m.mChainGravity;
        }

        var_r29 = mIronBallChainPos + 1;
        var_r27 = var_r29 - 1;
        var_r26 = field_0x3848 + 1;

        for (i = 0; i < mItemMode; i++, var_r29++, var_r26++) {
            sp2A4 = (*var_r29 - *var_r27) + *var_r26;
            sp2A4.y += var_f30;
            sp2A4.normalizeZP();

            *var_r29 = *var_r27 + (sp2A4 * 10.0f);
            var_r27 = var_r29;
        }

        var_r29 = &mIronBallChainPos[mItemMode];
        var_r26 = &field_0x3848[mItemMode];
        var_r27 = var_r29 + 1;

        BOOL sp2C = ((mItemVar0.field_0x3018 == 0 && checkUnderMove1BckNoArc(ANM_IRONBALL_WALK)) && mNowAnmPackUnder[1].getRatio() > 0.8f) && (mUnderFrameCtrl[1].getFrame() >= 24.0f || mUnderFrameCtrl[1].getFrame() <= 3.0f);

        f32 temp_f31 = 5.0f * (5.0f * (mNowAnmPackUnder[1].getRatio() - 0.8f));
        f32 temp_f28 = temp_f31 * cM_scos(shape_angle.y);
        temp_f31 *= cM_ssin(shape_angle.y);

        for (i = mItemMode; i > 0; i--, var_r29--, var_r26--) {
            sp28C = *var_r29;

            if (i == mItemMode) {
                *var_r29 = mHookshotTopPos;
            } else {
                sp2A4 = (*var_r29 - *var_r27) + *var_r26;
                if (sp2C) {
                    sp2A4.x += temp_f31;
                    sp2A4.z += temp_f28;
                }
                sp2A4.y += var_f30;

                sp280 = sp2A4;
                sp2A4.normalizeZP();

                *var_r29 = *var_r27 + (sp2A4 * 10.0f);
                if (setIronBallGroundVec(var_r29, &sp280)) {
                    *var_r29 = *var_r27 + (sp280 * 10.0f);
                }
            }

            var_r27 = var_r29;
            *var_r26 = (*var_r29 - sp28C) * mpHIO->mItem.mIronBall.m.mChainSpeedRate;
        }
    }

    sp3C = mIronBallChainAngle + 1;
    var_r29 = mIronBallChainPos + 1;
    var_r27 = var_r29 - 1;
    s16 sp10;
    s16 spE = mIronBallChainAngle->z + 0x3000;

    for (i = 0; i < mItemMode; i++, sp3C++, var_r29++, spE += 0x3000) {
        sp2A4 = (*var_r27 - *var_r29);

        f32 var_f27 = sp2A4.absXZ();
        if (var_f27 < 1.0f) {
            sp10 = sp3C[-1].y;
        } else {
            sp10 = sp2A4.atan2sX_Z();
        }

        sp3C->set(sp2A4.atan2sY_XZ(), sp10, spE);
        var_r27 = var_r29;
    }

    var_r29 = &mIronBallChainPos[mItemMode] + 1;
    sp3C = &mIronBallChainAngle[mItemMode] + 1;

    for (i = mItemMode + 2; i < 102; i++) {
        mIronBallChainPos[i] = *var_r29;
        mIronBallChainAngle[i] = *sp3C;
        field_0x3848[i] = cXyz::Zero;
    }
}

BOOL daAlink_c::checkIronBallReturnChange() {
    if (mProcID != PROC_IRON_BALL_THROW ||
        (!(mBodyAngle.x > 0x400) && mIronBallBgChkPos.y < (current.pos.y - 200.0f)) ||
        (mBodyAngle.x > 0x400 && mItemMode == 0x64) ||
        (field_0xFB8.ChkAtHit() && field_0xFB8.GetAtHitGObj() != NULL &&
         static_cast<dCcD_GObjInf*>(field_0xFB8.GetAtHitGObj())->ChkTgIronBallRebound())) {
        setIronBallReturn(0);
        return true;
    }

    return false;
}

void daAlink_c::setIronBallPos() {
    cXyz sp10C;
    cXyz sp100;
    cXyz spF4;
    cXyz spE8;
    cXyz spDC;
    cXyz spD0(mIronBallCenterPos);

    mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(0xF));
    mDoMtx_stack_c::transM(-1.0f, -6.0f, -3.6f);
    mDoMtx_stack_c::XYZrotM(cM_deg2s(150.0f), cM_deg2s(-81.0f), cM_deg2s(111.0f));

    if (mItemVar0.field_0x3018 == 0) {
        mDoMtx_stack_c::transM(0.0f, 0.0f, 10.0f);
        mDoMtx_stack_c::XrotM(0x7FFF);
    }

    mDoMtx_stack_c::multVec(&l_ironBallChainVec, &mHookshotTopPos);
    mDoMtx_stack_c::multVecZero(&mHeldItemRootPos);
    mDoMtx_MtxToRot(mDoMtx_stack_c::get(), &field_0x316c);
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(9), &sp10C);
    mDoMtx_multVecZero(mpLinkModel->getAnmMtx(0xE), &sp100);

    spE8 = (sp10C + sp100) * 0.5f;

    field_0x301c = 0;

    BOOL var_r25 = 0;
    BOOL var_r28 = 0;

    if (field_0x3026 != 0) {
        field_0x3026--;
    }

    cXyz spC4;
    cXyz spB8;

    if (field_0xFB8.ChkAtHit()) {
        dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (mItemVar0.field_0x3018 == 0) {
        mDoMtx_stack_c::copy(mpLinkModel->getAnmMtx(0xA));
        mDoMtx_stack_c::transM(-35.3f, -9.5f, -16.0f);
        mDoMtx_stack_c::XYZrotM(cM_deg2s(61.5f), cM_deg2s(-2.5f), cM_deg2s(50.3f));
        mDoMtx_stack_c::multVecZero(mIronBallChainPos);

        if (mProcID == PROC_IRON_BALL_SUBJECT || mProcID == PROC_IRON_BALL_MOVE) {
            mDoMtx_MtxToRot(mDoMtx_stack_c::get(), mIronBallChainAngle);
        } else {
            mIronBallChainAngle->set(-0x4000, shape_angle.y, 0);
        }

        mItemMode = 6;
    } else if (mItemVar0.field_0x3018 != 1) {
        if (mItemVar0.field_0x3018 == 2) {
            daPy_frameCtrl_c* temp_r24 = &mUpperFrameCtrl[2];
            s16 sp10 = 32768.0f - (65536.0f * (temp_r24->getFrame() / temp_r24->getEnd()));

            mItemMode = 3;

            sp10C.set(0.0f, 0.0f, 10.0f * mItemMode);

            mDoMtx_stack_c::transS(mHookshotTopPos);
            mDoMtx_stack_c::ZXYrotM(mBodyAngle.x, shape_angle.y, 0);
            mDoMtx_stack_c::YrotM(sp10);
            mDoMtx_stack_c::multVec(&sp10C, mIronBallChainPos);
            mDoMtx_MtxToRot(mDoMtx_stack_c::get(), mIronBallChainAngle);
        } else if (mItemVar0.field_0x3018 != 3) {
            if (mItemVar0.field_0x3018 == 4) {
                mItemMode++;
                checkIronBallReturnChange();
            } else if (mItemVar0.field_0x3018 == 5 || mItemVar0.field_0x3018 == 6) {
                *mIronBallChainPos += *field_0x173c.GetCCMoveP();

                if (!checkIronBallReturnChange()) {
                    f32 var_f29;
                    f32 var_f28 = mpHIO->mItem.mIronBall.m.mBallMaxFallSpeed;

                    if (mItemVar0.field_0x3018 == 5) {
                        var_f29 = mpHIO->mItem.mIronBall.m.mThrowBallGravity;
    
                        f32 sp1C;
                        if (fopAcM_getWaterY(mIronBallChainPos, &sp1C) && sp1C > mIronBallChainPos->y) {
                            if (field_0x3022 == 0) {
                                field_0x3022 = 1;
                                *field_0x3848 *= 0.5f;
                            }
                            var_f29 *= 0.5f;
                            var_f28 *= 0.5f;
                        }
                    } else {
                        var_f29 = mpHIO->mItem.mIronBall.m.mBallGravity;
                    }

                    sp10C = mIronBallCenterPos + (*field_0x3848 * 1.25f);

                    s16 temp_r27 = mpHIO->mItem.mIronBall.m.mThrowChainNum + 0.1f * mIronBallChainPos->abs(mHookshotTopPos);
                    if (temp_r27 > 100) {
                        mItemMode = 100;
                    } else if (mItemMode < temp_r27) {
                        mItemMode = temp_r27;
                    }

                    field_0x3848->y += var_f29;
                    if (field_0x3848->y < var_f28) {
                        field_0x3848->y = var_f28;
                    }

                    *mIronBallChainPos += *field_0x3848;
                    spF4 = *mIronBallChainPos - mHookshotTopPos;
                    f32 temp_f1 = spF4.abs();

                    BOOL var_r26 = 0;
                    if (temp_f1 > 1000.0f) {
                        spF4 *= 1000.0f / temp_f1;
                        *mIronBallChainPos = mHookshotTopPos + spF4;

                        if (field_0x3848->abs2() < 1.0f) {
                            var_r26 = 1;
                        } else {
                            field_0x3848->x = 0.0f;
                            field_0x3848->z = 0.0f;
                        }
                    }

                    spF4 = sp10C - *mIronBallChainPos;
                    spF4.y += var_f29;

                    mIronBallChainAngle->set(spF4.atan2sY_XZ(), spF4.atan2sX_Z(), 0);

                    int var_r23;
                    if (mBodyAngle.x >= 0) {
                        var_r23 = 50;
                    } else {
                        var_r23 = 50.0f - (40.0f * cM_ssin(mBodyAngle.x));
                    }

                    setIronBallBgCheckPos();

                    sp10C = mIronBallBgChkPos;

                    f32 temp_f30 = field_0x3848->y;
                    field_0x0778->CrrPos(dComIfG_Bgsp());

                    f32 var_f31;
                    int var_r29;

                    if (field_0x0778->ChkGroundLanding()) {
                        var_r29 = dKy_pol_sound_get(&field_0x0778->m_gnd);
                        var_f31 = fabsf(temp_f30);
                        field_0x301c = dComIfG_Bgsp().ChkMoveBG(field_0x0778->m_gnd) == 0;

                        if (field_0x3024 == 0) {
                            field_0x3024 = 1;
                            var_r25 = 1;
                        }
                    } else if (field_0x0778->ChkWallHit()) {
                        var_r29 = dKy_pol_sound_get(field_0x0774);
                        var_f31 = field_0x3848->absXZ();
                        field_0x301c = dComIfG_Bgsp().ChkMoveBG(*field_0x0774) == 0;

                        if (field_0x3026 == 0 && var_f31 > 3.0f) {
                            dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
                            field_0x3026 = 0xF;
                            var_r28 = 1;

                            spC4 = mIronBallBgChkPos;
                            spC4.y += 32.0f;
        
                            spB8.set(spC4.x - (cM_ssin(field_0x0774->GetWallAngleY()) * 48.0f),
                                     spC4.y,
                                     spC4.z - (cM_scos(field_0x0774->GetWallAngleY()) * 48.0f));
                        }
                    } else if (field_0x0778->ChkRoofHit()) {
                        var_r29 = dKy_pol_sound_get(&field_0x0778->m_roof);
                        var_f31 = fabsf(temp_f30);
                        field_0x301c = dComIfG_Bgsp().ChkMoveBG(field_0x0778->m_roof) == 0;

                        if (field_0x3026 == 0 && var_f31 > 3.0f) {
                            dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
                            field_0x3026 = 0xF;
                            var_r28 = 1;

                            spC4 = mIronBallBgChkPos;
                            spC4.y += 32.0f;

                            spB8 = spC4;
                            spC4.y += 48.0f;
                        }
                    } else {
                        var_r29 = -1;
                    }

                    if (var_r29 != -1) {
                        var_f31 *= 1.6666666f;
                        if (var_f31 > 100.0f) {
                            var_f31 = 100.0f;
                        }

                        mZ2Link.startHitItemSE(Z2SE_HIT_HAMMER, var_r29, mpHookSound, var_f31);
                    }

                    if (field_0x0778->ChkWallHit() || field_0x0778->ChkGroundHit() || field_0x0778->ChkRoofHit() || (mItemVar0.field_0x3018 == 5 && (var_r26 != 0 || mItemMode > var_r23))) {
                        if (field_0x0778->ChkWallHit() || field_0x0778->ChkGroundHit() || field_0x0778->ChkRoofHit()) {
                            *mIronBallChainPos += mIronBallBgChkPos - sp10C;

                            if (field_0x0778->ChkWallHit()) {
                                field_0x3848->x *= -0.2f;
                                field_0x3848->z *= -0.2f;

                                if (temp_f30 > 0.0f) {
                                    field_0x3848->y = 0.8f * temp_f30;
                                }
                            }

                            if (field_0x0778->ChkGroundHit()) {
                                field_0x3848->x *= 0.5f;
                                field_0x3848->z *= 0.5f;

                                if (checkSnowCodePolygon(field_0x0778->m_gnd)) {
                                    mSearchBallScale = -20.0f;
                                } else if (temp_f30 < 0.0f) {
                                    field_0x3848->y = -0.5f * temp_f30;
                                    field_0x3494 -= 1.0f;
                                    if (field_0x3848->y > 50.0f) {
                                        field_0x3848->y = 50.0f;
                                    }
                                }
                            } else if (field_0x0778->ChkRoofHit()) {
                                field_0x3848->x *= 0.5f;
                                field_0x3848->z *= 0.5f;

                                if (temp_f30 > 0.0f) {
                                    field_0x3848->y = -0.5f * temp_f30;
                                }
                            }
                        }

                        if (mItemVar0.field_0x3018 == 5) {
                            mItemVar0.field_0x3018 = 6;
                        } else if (field_0x3494 < 0.0f) {
                            setIronBallReturn(0);
                        } else if (field_0x3848->y < mpHIO->mItem.mIronBall.m.field_0x6C) {
                            setIronBallReturn(1);
                        }
                    } else if (mItemVar0.field_0x3018 == 6 && var_r26 != 0) {
                        setIronBallReturn(1);
                    }

                    mIronBallChainPos->y += mSearchBallScale;
                }
            } else if (mItemVar0.field_0x3018 == 7) {
                f32 sp18;
                int var_r22;
                if (fopAcM_getWaterY(mIronBallChainPos, &sp18) && sp18 > mIronBallChainPos->y) {
                    var_r22 = mpHIO->mItem.mIronBall.m.mReturnChainRemoveNum - 1;
                } else {
                    var_r22 = mpHIO->mItem.mIronBall.m.mReturnChainRemoveNum;
                }

                if (mItemMode > 6) {
                    mItemMode -= var_r22;
                    if (mItemMode <= 6) {
                        mItemVar0.field_0x3018 = 8;
                    }
                } else if (mItemMode <= 6) {
                    mItemVar0.field_0x3018 = 8;
                }
            } else if (mItemVar0.field_0x3018 == 8) {
                static Vec const ironBallPosVec = {0.0f, 0.0f, -42.0f};
                static Vec const handCenterOffsetVec = {2.0f, -25.0f, 12.5f};

                mSearchBallScale = 0.0f;

                mDoMtx_stack_c::transS(spE8);
                mDoMtx_stack_c::YrotM(shape_angle.y);
                mDoMtx_stack_c::multVec(&handCenterOffsetVec, &sp10C);

                sp10C.y += 42.0f;

                cLib_addCalcPos(&mIronBallCenterPos, sp10C, 0.5f, 70.0f, 10.0f);
                cLib_addCalcAngleS(&mIronBallChainAngle->x, -0x4000, 5, 0x1000, 0x400);
                cLib_addCalcAngleS(&mIronBallChainAngle->y, shape_angle.y, 5, 0x1000, 0x400);

                mDoMtx_stack_c::transS(mIronBallCenterPos);
                mDoMtx_stack_c::ZXYrotM(*mIronBallChainAngle);      
                mDoMtx_stack_c::multVec(&ironBallPosVec, mIronBallChainPos);
                
                if (mItemMode > 6) {
                    mItemMode -= 10;
                    if (mItemMode < 6) {
                        mItemMode = 6;
                    }
                } else if (mItemMode < 6) {
                    mItemMode = 6;
                }
            }
        }
    }

    setIronBallChainPos();

    mDoMtx_stack_c::transS(*mIronBallChainPos);
    mDoMtx_stack_c::ZXYrotM(*mIronBallChainAngle);
    mHeldItemModel->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::multVec(&l_ironBallCenterVec, &mIronBallCenterPos);

    field_0x3810 = mIronBallBgChkPos;
    mIronBallBgChkPos.set(mIronBallCenterPos.x, mIronBallCenterPos.y - 32.0f, mIronBallCenterPos.z);

    if (mItemVar0.field_0x3018 == 4) {     
        field_0x0778->CrrPos(dComIfG_Bgsp());
        if (field_0x0778->ChkGroundHit() || field_0x0778->ChkRoofHit() || field_0x0778->ChkWallHit()) {
            mItemVar0.field_0x3018 = 8;
            dComIfGp_getVibration().StartShock(4, 1, cXyz(0.0f, 1.0f, 0.0f));
        }
    }

    field_0x173c.ClrCcMove();
    mpHookSound->framework(0, mVoiceReverbIntensity);

    if (mItemVar0.field_0x3018 != 0) {
        mpHookshotLinChk->Set(&spD0, &mIronBallCenterPos, this);

        if (dComIfG_Bgsp().LineCross(mpHookshotLinChk) && dComIfG_Bgsp().GetPolyAtt0(*mpHookshotLinChk) != 6) {
            fopKyM_createWpillar(mpHookshotLinChk->GetCrossP(), 1.0f, 0);
            mDoAud_seStart(Z2SE_CM_BODYFALL_WATER_S, mpHookshotLinChk->GetCrossP(), 0, mVoiceReverbIntensity);
        }
    }

    if (var_r28 != 0) {
        field_0x780->Set(&spC4, &spB8, this);

        if (dComIfG_Bgsp().LineCross(field_0x780)) {
            cM3dGPla sp118;
            dComIfG_Bgsp().GetTriPla(*field_0x780, &sp118);

            csXyz sp20(cM_atan2s(sp118.mNormal.y, sp118.mNormal.absXZ()), cM_atan2s(-sp118.mNormal.x, -sp118.mNormal.z), 0);
            dComIfGp_setHitMark(9, NULL, field_0x780->GetCrossP(), &sp20, NULL, 0);
        }
    }

    field_0x2f1c->setEffectCenter(&tevStr, &mIronBallBgChkPos, 0x3B, 0x40000, var_r25 ? &mIronBallBgChkPos : NULL, NULL, NULL, fopAcM_GetRoomNo(this), field_0x3420, 0.0f);
}

BOOL daAlink_c::checkIronBallAnime() const {
    return checkIronBallPreSwingAnime() || checkIronBallSwingAnime();
}

void daAlink_c::setIronBallReadyAnime() {
    resetUpperAnime(UPPER_2, -1.0f);
    setSingleAnime(ANM_IRONBALL_ATTACK, mpHIO->mItem.mIronBall.m.mPrepareAnmSpeed, 0.0f,
                   0x15, mpHIO->mItem.mIronBall.m.mPrepareInterpolation);
    setUpperAnimeBaseSpeed(0x19A, mpHIO->mItem.mIronBall.m.mPrepareAnmSpeed, -1.0f);
    mNowAnmPackUpper[2].setRatio(0.0f);
    voiceStart(Z2SE_AL_V_SWING_IB);

    field_0xFB8.OffAtSetBit();
    field_0xFB8.ResetAtHit();
    field_0xFB8.SetAtAtp(2);

    field_0x301e = 10;
    field_0x3020 = 0;
    field_0x3494 = 5.0f;
    mSearchBallScale = 0.0f;
}

void daAlink_c::setIronBallBaseAnime() {
    if (checkIronBallSwingAnime() && !checkUnderMove0BckNoArc(ANM_IRONBALL_TURN)) {
        setSingleAnimeBaseSpeed(ANM_IRONBALL_TURN, mpHIO->mItem.mIronBall.m.mTurnAnmSpeed, mpHIO->mItem.mIronBall.m.mTurnInterpolation);
        mUnderFrameCtrl[0].setFrame(mUpperFrameCtrl[2].getFrame());
        getNowAnmPackUnder(UNDER_0)->setFrame(mUpperFrameCtrl[2].getFrame());
    }
}

BOOL daAlink_c::checkUpperItemActionIronBall() {
    if (!itemButton() && checkReadyItem()) {
        field_0x3020 = 1;
    }

    if (checkIronBallPreSwingAnime()) {
        daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;
        if (field_0x3020 != 0 && frameCtrl->checkPass(19.0f)) {
            return procIronBallThrowInit();
        }

        if (frameCtrl->checkAnmEnd()) {
            if (checkModeFlg(1)) {
                setSingleAnimeBaseSpeed(ANM_IRONBALL_TURN, mpHIO->mItem.mIronBall.m.mTurnAnmSpeed, mpHIO->mItem.mIronBall.m.mTurnInterpolation);
            }

            u16 upperId = getMainBckData(ANM_IRONBALL_TURN)->m_upperID;
            setUpperAnimeBaseSpeed(upperId, mpHIO->mItem.mIronBall.m.mTurnAnmSpeed, mpHIO->mItem.mIronBall.m.mTurnInterpolation);
            mItemVar0.field_0x3018 = 2;
            seStartOnlyReverb(Z2SE_AL_IB_SWING);
            return 1;
        }

        if (frameCtrl->getFrame() > 13.0f) {
            mItemVar0.field_0x3018 = 1;
        }
    } else {
        daPy_frameCtrl_c* frameCtrl = &mUpperFrameCtrl[2];

        if (field_0x3020 != 0 && frameCtrl->checkPass(18.0f)) {
            return procIronBallThrowInit();
        } else if (frameCtrl->checkPass(0.0f)) {
            seStartOnlyReverb(Z2SE_AL_IB_SWING);
        }
    }

    return cancelUpperItemReadyAnime(0);
}

int daAlink_c::checkNextActionIronBall() {
    if (mFastShotTime != 0) {
        mFastShotTime--;
    }

    if (!checkIronBallAnime()) {
        setIronBallReadyAnime();
        setFastShotTimer();
        
        if (!checkAttentionLock()) {
            field_0x2fe4 = shape_angle.y;
        }
    }

    if (!checkAttentionLock() && mFastShotTime == 0) {
        return procIronBallSubjectInit();
    } else {
        return procIronBallMoveInit();
    }
}

int daAlink_c::procIronBallSubjectInit() {
    if (!commonProcInitNotSameProc(PROC_IRON_BALL_SUBJECT)) {
        return 0;
    }

    mNormalSpeed = 0.0f;

    if (checkIronBallAnime()) {
        setIronBallBaseAnime();
    }

    current.angle.y = shape_angle.y;
    dComIfGp_setPlayerStatus0(0, 0x400);
    return 1;
}

int daAlink_c::procIronBallSubject() {
    if (checkIronBallAnime()) {
        setDoStatus(0x12);
    }

    setShapeAngleToAtnActor(0);

    if (!checkNextAction(0) && itemButton() && mItemVar0.field_0x3018 == 2) {
        setBodyAngleToCamera();
    }

    return 1;
}

int daAlink_c::procIronBallMoveInit() {
    if (!commonProcInitNotSameProc(PROC_IRON_BALL_MOVE)) {
        return 0;
    }

    if (checkZeroSpeedF()) {
        onModeFlg(1);
    }

    if (checkIronBallPreSwingAnime()) {
        mNormalSpeed = 0.0f;
    }

    dComIfGp_setPlayerStatus0(0, 0x400);
    return 1;
}

int daAlink_c::procIronBallMove() {
    if (checkIronBallAnime()) {
        setDoStatus(0x12);
    }

    setShapeAngleToAtnActor(0);

    if (!checkNextAction(0)) {
        f32 var_f31 = -1.0f;

        if (checkIronBallPreSwingAnime()) {
            mNormalSpeed = 0.0f;
        }

        if (checkZeroSpeedF()) {
            onModeFlg(1);

            if (field_0x2f98 != 2) {
                field_0x2f98 = 2;
                var_f31 = mpHIO->mBasic.m.mBasicInterpolation;
            }
        } else {
            offModeFlg(1);
        }

        if (checkModeFlg(1)) {
            setIronBallBaseAnime();
        } else {
            setBlendAtnMoveAnime(var_f31);
        }

        setBodyAngleXReadyAnime(0);
    }

    return 1;
}

int daAlink_c::procIronBallThrowInit() {
    commonProcInit(PROC_IRON_BALL_THROW);
    setSingleAnime(ANM_IRONBALL_ATTACK, mpHIO->mItem.mIronBall.m.mPreThrowAnmSpeed, 22.0f, 30, mpHIO->mItem.mIronBall.m.mPreThrowAnmInterpolation);
    field_0x2f99 = 12;
    field_0x3588 = l_ironBallBaseAnime;
    mNormalSpeed = 0.0f;
    current.angle.y = shape_angle.y;
    mProcVar2.field_0x300c = 0;
    mItemVar0.field_0x3018 = 3;
    field_0x3024 = 0;
    field_0x3026 = 0;

    voiceStart(Z2SE_AL_V_THROW_IB);
    seStartOnlyReverb(Z2SE_AL_IB_THROW);
    field_0xFB8.SetAtAtp(3);
    return 1;
}

int daAlink_c::procIronBallThrow() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];

    if (mEquipItem != fpcNm_ITEM_IRONBALL) {
        return checkWaitAction();
    }

    field_0x2f99 = 4;

    if (mItemVar0.field_0x3018 == 8) {
        return procIronBallReturnInit();
    }

    if (mItemVar0.field_0x3018 == 4 && temp_r29->checkPass(6.0f)) {
        mItemVar0.field_0x3018 = 5;
        mDoMtx_stack_c::ZXYrotS(mBodyAngle.x, shape_angle.y, 0);
        
        Vec sp8 = {0.0f, mpHIO->mItem.mIronBall.m.mThrowSpeedY, mpHIO->mItem.mIronBall.m.mThrowSpeedZ};
        mDoMtx_stack_c::multVec(&sp8, field_0x3848);
        field_0x3022 = 0;
    }

    if (checkAnmEnd(temp_r29)) {
        if (mProcVar2.field_0x300c == 0) {
            mProcVar2.field_0x300c = 1;
            setSingleAnimeBaseSpeed(ANM_IRONBALL_THROW, mpHIO->mItem.mIronBall.m.mThrowAnmSpeed, mpHIO->mItem.mIronBall.m.mThrowInterpolation);
            field_0x2f99 = 0xC;
            mItemVar0.field_0x3018 = 4;
        } else if (mItemVar0.field_0x3018 == 7) {
            procIronBallReturnInit();
        }
    } else if (mProcVar2.field_0x300c != 0) {
        if (temp_r29->getFrame() >= 5.0f) {
            field_0x2f92 = 0xFE;
            field_0x2f93 = 0xFE;
        }
    }

    return 1;
}

int daAlink_c::procIronBallReturnInit() {
    commonProcInit(PROC_IRON_BALL_RETURN);

    if (mItemVar0.field_0x3018 == 8) {
        mProcVar2.field_0x300c = 1;
        setSingleAnimeParam(ANM_IRONBALL_CATCH, &mpHIO->mItem.mIronBall.m.mCatchAnm);
        field_0x2f99 = 12;
    } else {
        setSingleAnimeBaseSpeed(ANM_IRONBALL_PULL, mpHIO->mItem.mIronBall.m.mPullAnmSpeed, mpHIO->mItem.mIronBall.m.mPullInterpolation);
        field_0x2f99 = 12;
        mProcVar2.field_0x300c = 0;
    }

    field_0x3588 = l_ironBallBaseAnime;
    return 1;
}

int daAlink_c::procIronBallReturn() {
    daPy_frameCtrl_c* frameCtrl = mUnderFrameCtrl;

    if (mEquipItem != fpcNm_ITEM_IRONBALL) {
        return checkWaitAction();
    }

    field_0x2f99 = 4;

    if (mItemVar0.field_0x3018 == 8 && mProcVar2.field_0x300c == 0) {
        mProcVar2.field_0x300c = 1;
        setSingleAnimeParam(ANM_IRONBALL_CATCH, &mpHIO->mItem.mIronBall.m.mCatchAnm);
        field_0x2f99 = 12;
        seStartOnlyReverb(Z2SE_AL_IB_CATCH);

        dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
    } else if (frameCtrl->checkAnmEnd()) {
        if (mItemVar0.field_0x3018 == 8) {
            mItemVar0.field_0x3018 = 0;
            setIronBallWaitUpperAnime(1);
            checkNextAction(0);
        }
    } else if (mItemVar0.field_0x3018 == 8 && frameCtrl->getFrame() > mpHIO->mItem.mIronBall.m.mCatchAnm.mCancelFrame) {
        mItemVar0.field_0x3018 = 0;
        setIronBallWaitUpperAnime(1);
        
        if (!checkNextAction(1)) {
            mItemVar0.field_0x3018 = 8;
            resetUpperAnime(UPPER_2, 0.0f);
        }
    }

    return 1;
}
