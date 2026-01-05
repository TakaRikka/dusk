/**
 * d_a_alink_crawl.inc
 * Player Crawl action handling
 */

#include "d/actor/d_a_alink.h"

f32 daAlink_c::getCrawlMoveAnmSpeed() {
    return getAnmSpeedStickRate(mpHIO->mCrouch.m.mCrawlAnmSpeedMin,
                                mpHIO->mCrouch.m.mCrawlAnmSpeedMax);
}

f32 daAlink_c::getCrawlMoveSpeed() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    f32 frame;
    if (frame_ctrl->getFrame() >= 17.0f) {
        frame = frame_ctrl->getFrame() - 17.0f;
    } else {
        frame = frame_ctrl->getFrame();
    }

    return (mpHIO->mCrouch.m.mCrawlMoveRate * frame_ctrl->getRate()) * cM_fsin(frame * 0.18479956686496735f);
}

void daAlink_c::setCrawlMoveDirectionArrow() {
    static const u8 data_80452F38 = 1;
    static const u8 data_80452F39 = 4;

    u8 direction = 0;
    camera_class* camera = (camera_class*)dComIfGp_getCamera(field_0x317c);
    BOOL bvar = cLib_distanceAngleS(fopCamM_GetAngleY(camera), shape_angle.y) > 0x4000;

    if (field_0x3198 & 4) {
        if (!bvar) {
            direction |= data_80452F38;
        } else {
            direction |= data_80452F39;
        }
    }

    if (field_0x3198 & 8) {
        if (!bvar) {
            direction |= data_80452F39;
        } else {
            direction |= data_80452F38;
        }
    }

    if (field_0x3198 & 1) {
        if (shape_angle.y == current.angle.y) {
            if (!bvar) {
                direction |= 8;
            } else {
                direction |= 2;
            }
        } else if (!bvar) {
            direction |= 2;
        } else {
            direction |= 8;
        }
    }

    if (shape_angle.y == current.angle.y) {
        if (!bvar) {
            direction |= 2;
        } else {
            direction |= 8;
        }
    } else if (!bvar) {
        direction |= 8;
    } else {
        direction |= 2;
    }

    dComIfGp_setAdvanceDirection(direction);
}

BOOL daAlink_c::changeCrawlAutoMoveProc(cXyz* param_0) {
    cXyz sp70;
    cXyz sp64;
    cXyz sp58;
    cXyz sp4C;
    cXyz sp40;
    cXyz sp34;
    cXyz sp28;
    f32 spC;
    s16 spA;
    s16 sp8;

    f32 temp_f31 = cM_ssin(current.angle.y);
    f32 temp_f30 = cM_scos(current.angle.y);
    f32 temp_f29 = cM_ssin(shape_angle.y);
    f32 temp_f28 = cM_scos(shape_angle.y);

    int var_r29 = 0;

    sp70.x = param_0->x + (95.0f * temp_f31);
    sp70.y = param_0->y;
    sp70.z = param_0->z + (95.0f * temp_f30);

    if (!commonLineCheck(param_0, &sp70) || (!checkWolf() && dComIfG_Bgsp().GetWallCode(mLinkLinChk) == 6)) {
        sp4C.x = sp70.x + (55.0f * temp_f30);
        sp4C.y = sp70.y;
        sp4C.z = sp70.z - (55.0f * temp_f31);

        if (checkCrawlSideWall(&sp70, &sp4C, &sp34, &sp28, &spA, &sp8)) {
            var_r29 |= 1;
        } else {
            return 0;
        }
    }

    sp4C.x = param_0->x + (55.0f * temp_f28);
    sp4C.y = param_0->y;
    sp4C.z = param_0->z - (55.0f * temp_f29);

    sp40 = (*param_0 * 2.0f) - sp4C;

    if (commonLineCheck(param_0, &sp4C)) {
        var_r29 |= 8;

        sp58.x = sp40.x + (95.0f * temp_f29);
        sp58.y = sp40.y;
        sp58.z = sp40.z + (95.0f * temp_f28);
        if (!checkCrawlSideWall(&sp40, &sp58, &sp34, &sp28, &spA, &sp8)) {
            return 0;
        }
    } else {
        var_r29 |= 4;

        sp58.x = sp4C.x - (95.0f * temp_f29);
        sp58.y = sp4C.y;
        sp58.z = sp4C.z - (95.0f * temp_f28);
        if (!checkCrawlSideWall(&sp4C, &sp58, &sp34, &sp28, &spA, &sp8)) {
            return 0;
        }

        if (!commonLineCheck(param_0, &sp40)) {
            var_r29 |= 8;
        }
    }
    

    if (cM3d_Len2dSqPntAndSegLine(0.5f * (sp34.x + sp28.x), 0.5f * (sp34.z + sp28.z), sp70.x, sp70.z, param_0->x - (95.0f * temp_f31), param_0->z - (95.0f * temp_f30), &sp64.x, &sp64.z, &spC)) {
        sp64.y = current.pos.y;
        if (checkWolf()) {
            return procWolfLieAutoMoveInit(var_r29, &sp64);
        } else {
            return procCrawlAutoMoveInit(var_r29, &sp64);
        }
    }

    return 0;
}

int daAlink_c::getCrawlMoveVec(cXyz* param_0, cXyz* param_1, cXyz* param_2, int param_3,
                                   int param_4, u8* param_5) {
    if (commonLineCheck(param_0, param_1) && ((param_4 == 0 && (!checkWolf() || !dComIfGp_checkPlayerStatus0(0, 0x08000000))) || dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 6) && (!checkWolf() || dComIfG_Bgsp().GetWallCode(mLinkLinChk) != 7)) {
        cM3dGPla sp3C;
        dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp3C);

        int temp_r27 = dComIfG_Bgsp().GetSpecialCode(mLinkLinChk);
        if (cBgW_CheckBWall(sp3C.mNormal.y) || (param_3 == 2 && cBgW_CheckBGround(sp3C.mNormal.y))) {
            cXyz sp1C = *param_1 - mLinkLinChk.GetCross();
            s16 temp_r26 = sp3C.mNormal.atan2sX_Z();
            s16 temp_r3 = sp1C.atan2sX_Z();

            if (abs((s16)((temp_r3 + 0x8000) - temp_r26)) > 0x3000) {
                return 0;
            }

            f32 temp_f31 = -sp1C.absXZ() * cM_scos(((temp_r3 - temp_r26) - 0x8000));

            param_2->x = temp_f31 * sp3C.mNormal.x;
            if (param_3 != 1) {
                param_2->y = 0.0f;
            } else {
                param_2->y = sp1C.y;
            }
            param_2->z = temp_f31 * sp3C.mNormal.z;

            if (param_5 != NULL && (dKy_pol_argument_get(&mLinkLinChk) & 0x60)) {
                *param_5 = 3;

                cM3dGPla sp28;
                dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp28);

                field_0x311e = sp28.mNormal.atan2sX_Z();

                if (checkIcePolygonDamage(&mLinkLinChk)) {
                    *param_5 |= 0x80;
                }

                if (dKy_pol_argument_get(&mLinkLinChk) & 0x20) {
                    *param_5 |= 0x40;
                }
            }
            return 1;
        }

        if (param_3 == 1 && (temp_r27 == 1 || (sp3C.mNormal.y < field_0x3470 && temp_r27 == 2))) {
            *param_2 = *param_1 - mLinkLinChk.GetCross();
            return 1;
        }
    }

    return 0;
}

void daAlink_c::crawlBgCheck(cXyz* param_0, cXyz* param_1, int param_2) {
    cXyz sp28;
    cXyz sp34;
    cXyz sp40;

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlTopOffset, &sp28);
    int temp_r30 = getCrawlMoveVec(&sp28, param_0, &sp34, 1, param_2, NULL);

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlTopUpOffset, &sp28);
    int temp_r29 = getCrawlMoveVec(&sp28, param_1, &sp40, 1, param_2, NULL);

    if (temp_r30 != 0 && temp_r29 != 0) {
        if (sp34.abs2() > sp40.abs2()) {
            current.pos -= sp34;
        } else {
            current.pos -= sp40;
        }
    } else if (temp_r30 != 0) {
        current.pos -= sp34;
    } else if (temp_r29 != 0) {
        current.pos -= sp40;
    }
}

BOOL daAlink_c::checkCrawlSideWall(cXyz* param_0, cXyz* param_1, cXyz* param_2, cXyz* param_3,
                                       s16* param_4, s16* param_5) {
    cXyz sp58;
    cXyz sp4C;

    if (commonLineCheck(param_0, param_1)) {
        cM3dGPla sp64;
        *param_2 = mLinkLinChk.GetCross();
        dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp64);

        *param_4 = sp64.mNormal.atan2sX_Z();

        sp4C = *param_2 + (sp64.mNormal * 95.0f);
        sp58 = (sp4C + *param_2) * 0.5f;    
        if (commonLineCheck(&sp58, &sp4C)) {
            *param_3 = mLinkLinChk.GetCross();
            dComIfG_Bgsp().GetTriPla(mLinkLinChk, &sp64);

            *param_5 = sp64.mNormal.atan2sX_Z();

            cXyz sp40 = *param_2 - *param_3;
            f32 temp_f31 = sp40.abs2XZ();
            if (cLib_distanceAngleS(*param_4, *param_5) > 0x7F00 && temp_f31 < 9025.0f && temp_f31 > 4900.0f) {
                return 1;
            }
        }
    }

    return 0;
}

void daAlink_c::decideCrawlDoStatus() {
    if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x80)) {
        if (checkSubjectEnd(1) || mWaterY > current.pos.y) {
            dComIfGp_clearPlayerStatus0(0, 0x2000);
        }
    } else {
        dCamera_c* camera = dCam_getBody();

        if (camera->ChangeModeOK(4) && mWaterY <= current.pos.y) {
            onResetFlg0(RFLG0_UNK_4000000);

            if (dComIfGp_checkCameraAttentionStatus(field_0x317c, 0x1000) && !checkEventRun()) {
                setSubjectMode();
            }
        }
    }
}

BOOL daAlink_c::checkNotCrawlStand(cXyz* param_0) {
    mLinkRoofChk.SetPos(*param_0);
    return dComIfG_Bgsp().RoofChk(&mLinkRoofChk) - current.pos.y <= field_0x598;
}

BOOL daAlink_c::checkNotCrawlStand(cXyz* param_0, cXyz* param_1) {
    cXyz sp20;

    sp20 = *param_0 + *param_1;
    if (checkNotCrawlStand(&sp20)) {
        return true;
    }

    sp20 = *param_0 - *param_1;
    if (checkNotCrawlStand(&sp20)) {
        return true;
    }

    return false;
}

BOOL daAlink_c::checkCrawlInHoll(cXyz* param_0, cXyz* param_1, cXyz* param_2, int param_3) {
    cXyz spC0;
    cXyz spB4;
    cXyz spA8;
    cXyz sp9C;
    cXyz sp90;
    cXyz sp84;
    s16 sp16;
    s16 sp14;

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), field_0x2f50, &spC0);
    cMtx_multVec(mpLinkModel->getBaseTRMtx(), field_0x2f54, param_2);

    if (mUnderFrameCtrl[0].getRate() >= 0.0f) {
        sp90 = (*param_0 - *param_2) * 0.5f;
    } else if (mUnderFrameCtrl[0].getRate() < 0.0f) {
        sp90 = (*param_1 - *param_2) * 0.5f;
    }

    spC0 += sp90;
    *param_2 += sp90;
    spB4 = (*param_2 * 2.0f) - spC0;

    if (checkCrawlSideWall(param_2, &spC0, &spA8, &sp9C, &sp16, &sp14) || checkCrawlSideWall(param_2, &spB4, &sp9C, &spA8, &sp14, &sp16)) {        
        field_0x3198 = 1;
        field_0x37c8 = ((spA8 + sp9C) * 0.5f) - sp90;
        mProcVar2.field_0x300c = sp16 + 0x4000;
        return 1;
    }

    return 0;
}

void daAlink_c::setCrawlMoveHoll() {
    f32 frame_ctrl_rate = mUnderFrameCtrl[0].getRate();

    if (frame_ctrl_rate) {
        cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 5, 0x1000, 0x800);
        current.angle.y = shape_angle.y;
    }

    if (fabsf(current.pos.x - field_0x37c8.x) > 1.0f) {
        cLib_addCalc(&current.pos.x, field_0x37c8.x, 0.5f, 10.0f, 1.0f);
    }

    if (fabsf(current.pos.z - field_0x37c8.z) > 1.0f) {
        cLib_addCalc(&current.pos.z, field_0x37c8.z, 0.5f, 10.0f, 1.0f);
    }
}

void daAlink_c::setCrawlMoveAngle() {
    cXyz sp20;
    cXyz sp2C;
    cXyz sp38;

    cLib_addCalcAngleS(&shape_angle.y, field_0x2fe2, mpHIO->mCrouch.m.mCrawlTurnRate,
                       mpHIO->mCrouch.m.mCrawlTurnMax,
                       mpHIO->mCrouch.m.mCrawlTurnMin);

    if (shape_angle.y != current.angle.y) {
        if ((s16)(shape_angle.y - current.angle.y) > 0) {
            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlLSideFrontOffset, &sp20);
            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlLSideOffset, &sp38);
        } else {
            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlRSideFrontOffset, &sp20);
            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlRSideOffset, &sp38);
        }

        mLinkGndChk.SetPos(&sp20);
        // Fakematch:
        // dBgS::GroundCross is implicitly inlined to cBgS::GroundCross, but for some
        // reason here that breaks the match. This is the only place in the code so
        // far (September 2025) where it causes issues.
        sp2C.set(sp20.x - sp38.x, dComIfG_Bgsp().cBgS::GroundCross(&mLinkGndChk) - sp38.y,
                 sp20.z - sp38.z);

        if (cLib_distanceAngleS(sp2C.atan2sY_XZ(), shape_angle.x) > 0x800) {
            shape_angle.y = current.angle.y;
        } else {
            current.angle.y = shape_angle.y;
        }
    }
}

void daAlink_c::stopHalfMoveAnime(f32 param_0) {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    f32 frame;
    if (frame_ctrl->checkPass(0.0f)) {
        frame = 0.0f;
    } else {
        frame = param_0;
    }

    frame_ctrl->setRate(0.0f);
    frame_ctrl->setFrame(frame);
    getNowAnmPackUnder(UNDER_0)->setFrame(frame);
}

void daAlink_c::setCrawlAutoMoveAimPos() {
    int temp_r30 = getDirectionFromShapeAngle();
    f32 temp_f31 = cM_ssin(shape_angle.y);
    f32 temp_f30 = cM_scos(shape_angle.y);

    if (temp_r30 == 2 && (field_0x3198 & 4)) {
        field_0x37c8.x += 1.5f * (95.0f * temp_f30);
        field_0x37c8.z -= 1.5f * (95.0f * temp_f31);

        mProcVar2.field_0x300c = current.angle.y + 0x4000;

        if (current.angle.y == shape_angle.y) {
            mProcVar3.field_0x300e = 1;
        } else {
            mProcVar3.field_0x300e = -1;
        }

        mProcVar0.field_0x3008 = -1;
    } else if (temp_r30 == 3 && (field_0x3198 & 8)) {
        field_0x37c8.x -= 1.5f * (95.0f * temp_f30);
        field_0x37c8.z += 1.5f * (95.0f * temp_f31);

        mProcVar2.field_0x300c = current.angle.y - 0x4000;

        if (current.angle.y == shape_angle.y) {
            mProcVar3.field_0x300e = 1;
        } else {
            mProcVar3.field_0x300e = -1;
        }

        mProcVar0.field_0x3008 = -1;
    } else if ((field_0x3198 & 1) && ((shape_angle.y == current.angle.y && temp_r30 == 0) || (shape_angle.y != current.angle.y && temp_r30 == 1))) {     
        field_0x37c8.x += 1.5f * (95.0f * cM_ssin(current.angle.y));
        field_0x37c8.z += 1.5f * (95.0f * cM_scos(current.angle.y));

        mProcVar2.field_0x300c = shape_angle.y;
        mProcVar3.field_0x300e = 0;
        mProcVar0.field_0x3008 = -1;
    } else if ((shape_angle.y == current.angle.y && temp_r30 == 1) || (shape_angle.y != current.angle.y && temp_r30 == 0)) {
        current.angle.y += 0x8000;
        field_0x37c8.x -= 1.5f * (95.0f * cM_ssin(current.angle.y));
        field_0x37c8.z -= 1.5f * (95.0f * cM_scos(current.angle.y));

        mProcVar2.field_0x300c = shape_angle.y;
        mProcVar3.field_0x300e = 0;
        mProcVar0.field_0x3008 = -1;
    }
}

int daAlink_c::procCrawlStartInit() {
    if (mEquipItem != fpcNm_ITEM_NONE) {
        return procPreActionUnequipInit(PROC_CRAWL_START, NULL);
    }

    commonProcInit(PROC_CRAWL_START);

    field_0x3588 = l_waitBaseAnime;
    setSingleAnimeParam(ANM_CRAWL_START, &mpHIO->mCrouch.m.mCrawlStartAnm);

    mNormalSpeed = 0.0f;
    shape_angle.y = field_0x306e + 0x8000;
    current.angle.y = shape_angle.y;
    current.pos.x = field_0x34ec.x + (35.0f * cM_ssin(field_0x306e));
    current.pos.z = field_0x34ec.z + (35.0f * cM_scos(field_0x306e));

    if (mpHIO->mCrouch.m.mCrawlStartAnm.mCancelFrame > mpHIO->mCrouch.m.mCrawlStartAnm.mEndFrame) {
        field_0x347c = mpHIO->mCrouch.m.mCrawlStartAnm.mEndFrame;
    } else {
        field_0x347c = mpHIO->mCrouch.m.mCrawlStartAnm.mCancelFrame;
    }

    field_0x3478 = 1.0f / (field_0x347c - mpHIO->mCrouch.m.mCrawlStartAnm.mStartFrame);
    field_0x33cc = 0.0f;

    dComIfGp_setPlayerStatus0(0, 0x08000000);
    return 1;
}

int daAlink_c::procCrawlStart() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    decideCrawlDoStatus();
    field_0x33cc = 1.0f - field_0x3478 * (field_0x347c - frame_ctrl->getFrame());

    cXyz sp20(l_crawlFrontOffset.x, l_crawlFrontOffset.y, l_crawlFrontOffset.z * field_0x33cc);
    cXyz sp2C;
    cXyz sp38;

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &sp20, &sp2C);
    sp20.y = 80.0f;

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &sp20, &sp38);
    crawlBgCheck(&sp2C, &sp38, 0);

    field_0x2f99 = 4;
    if (checkAnmEnd(frame_ctrl)) {
        procCrawlMoveInit(shape_angle.x, shape_angle.z);
    }

    return 1;
}

int daAlink_c::procCrawlMoveInit(s16 param_0, s16 param_1) {
    BOOL var_r29;
    BOOL var_r28 = 0;

    if (mProcID == PROC_CRAWL_AUTO_MOVE) {
        var_r29 = 0;
    } else {
        var_r29 = 1;
        if (mProcID == PROC_CRAWL_START) {
            var_r28 = 1;
        }
    }

    BOOL var_r27;
    if (dComIfGp_checkPlayerStatus0(0, 0x2000)) {
        var_r27 = 1;
    } else {
        var_r27 = 0;
    }

    commonProcInit(PROC_CRAWL_MOVE);
    mProcVar3.field_0x300e = 0;

    if (var_r29 != 0) {
        f32 var_f31 = getCrawlMoveAnmSpeed();
        if (var_r28 != 0) {
            mProcVar3.field_0x300e = 1;
        } else if (getDirectionFromShapeAngle() == 1) {
            var_f31 *= -1.0f;
        }

        current.angle.y = shape_angle.y;
        setSingleAnimeBaseSpeed(ANM_CRAWL, var_f31, mpHIO->mCrouch.m.mCrawlInterpolation);
    }

    field_0x3198 = var_r29 ^ 1;
    shape_angle.x = param_0;
    shape_angle.z = param_1;
    field_0x33cc = 1.0f;
    field_0x2f99 = 0xC;

    dComIfGp_setPlayerStatus0(0, 0x08000000);

    if (var_r27 != 0) {
        dComIfGp_setPlayerStatus0(0, 0x2000);
    }

    return 1;
}

int daAlink_c::procCrawlMove() {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    cXyz sp54;
    cXyz sp48;
    cXyz sp3C;

    field_0x2f99 = 0xC;
    decideCrawlDoStatus();

    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlStandUpOffset, &sp48);
    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlFrontUpOffset, &sp54);
    cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlBackUpOffset, &sp3C);

    cXyz sp30;
    cXyz sp24;
    cMtx_multVecSR(mpLinkModel->getBaseTRMtx(), &l_crawlMinSideOffset, &sp24);

    BOOL temp_r25 = checkNotCrawlStand(&sp48);
    BOOL temp_r24 = checkNotCrawlStand(&sp54);
    BOOL temp_r23 = checkNotCrawlStand(&sp3C);
    BOOL var_r27;
    if (temp_r25 || temp_r24 || temp_r23 || checkNotCrawlStand(&sp54, &sp24) || checkNotCrawlStand(&sp3C, &sp24) || checkNotCrawlStand(&sp48, &sp24)) {
        var_r27 = 0;
        onModeFlg(0x04000000);
    } else {
        var_r27 = 1;
        offModeFlg(0x04000000);
    }

    cM3dGPla sp60;

    if ((var_r27 != 0 && mProcVar3.field_0x300e == 0) || getSlidePolygon(&sp60) || checkCrawlWaterIn()) {
        procCrawlEndInit(1, shape_angle.x, shape_angle.z);
    } else {
        BOOL var_r26 = 0;
        f32 temp_f31 = getCrawlMoveAnmSpeed();

        if (temp_r29->getRate() > 0.0f) {
            setWaterInAnmRate(temp_r29, temp_f31);
        } else if (temp_r29->getRate() < 0.0f) {
            setWaterInAnmRate(temp_r29, -temp_f31);
        } else {
            var_r26 = 1;
        }
    
        if (var_r26 == 1 || temp_r29->checkPass(0.0f) || temp_r29->checkPass(17.0f)) {
            if (mProcVar3.field_0x300e != 0) {
                mProcVar3.field_0x300e--;
                setWaterInAnmRate(temp_r29, temp_f31);
                temp_r29->setLoop(0);
            } else if (checkInputOnR()) {
                if (getDirectionFromShapeAngle() != 1) {
                    setWaterInAnmRate(temp_r29, temp_f31);
                    temp_r29->setLoop(0);
                } else {
                    setWaterInAnmRate(temp_r29, -temp_f31);
                    temp_r29->setLoop(temp_r29->getEnd());
                }

                initBasAnime();
            } else if (var_r26 == 0) {
                stopHalfMoveAnime(17.0f);
            }
        }

        int sp8 = field_0x3198;
        cXyz sp18;
        field_0x3198 = 0;

        if (checkCrawlInHoll(&sp54, &sp3C, &sp18, var_r27)) {
            setCrawlMoveHoll();
        } else if (sp8 != 0 && temp_r25 && temp_r24 && temp_r23 && var_r27 == 0 && changeCrawlAutoMoveProc(&sp18) ) {
            return 1;
        } else if (mProcVar3.field_0x300e == 0 && checkInputOnR() && temp_r29->getRate() > 0.0f) {
            setCrawlMoveAngle();
        }

        mNormalSpeed = getCrawlMoveSpeed();

        cXyz spC;
        if (mNormalSpeed < 0.0f) {
            mNormalSpeed *= -1.0f;
            current.angle.y = shape_angle.y + 0x8000;
            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlBackOffset, &spC);
            crawlBgCheck(&spC, &sp3C, 1);
        } else {
            cMtx_multVec(mpLinkModel->getBaseTRMtx(), &l_crawlFrontOffset, &spC);
            crawlBgCheck(&spC, &sp54, 1);
        }
    }

    return 1;
}

int daAlink_c::procCrawlAutoMoveInit(int param_0, cXyz* param_1) {
    BOOL var_r30 = dComIfGp_checkPlayerStatus0(0, 0x2000) != 0 ? TRUE : FALSE;
    commonProcInit(PROC_CRAWL_AUTO_MOVE);
    field_0x37c8 = *param_1;
    field_0x33cc = 1.0f;
    field_0x3198 = param_0;
    mProcVar0.field_0x3008 = 20;
    mProcVar1.field_0x300a = 300;

    dComIfGp_setPlayerStatus0(0, 0x8000000);
    setCrawlMoveDirectionArrow();
    mNormalSpeed = 0.0f;
    field_0x2f99 = 12;

    if (var_r30) {
        dComIfGp_setPlayerStatus0(0, 0x2000);
    }

    return 1;
}

int daAlink_c::procCrawlAutoMove() {
    field_0x2f99 = 0xC;

    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    s16 temp_r27 = shape_angle.y;
    s16 temp_r26 = current.angle.y;

    if (mProcVar0.field_0x3008 > 0) {
        if (temp_r29->checkPass(0.0f) || temp_r29->checkPass(17.0f)) {
            stopHalfMoveAnime(17.0f);
            mNormalSpeed = 0.0f;
        } else if (checkAnmEnd(temp_r29)) {
            mNormalSpeed = 0.0f;
            mProcVar0.field_0x3008 -= 1;
        }

        setCrawlMoveDirectionArrow();
        decideCrawlDoStatus();

        shape_angle.y = temp_r27;
        current.angle.y = temp_r26;
    } else if (mProcVar0.field_0x3008 == 0) {
        setCrawlMoveDirectionArrow();
        if (checkInputOnR()) {
            setCrawlAutoMoveAimPos();

            if (mProcVar0.field_0x3008 == -1) {
                dComIfGp_setAdvanceDirection(0);

                if (shape_angle.y == current.angle.y) {
                    setWaterInAnmRate(temp_r29, 2.0f);
                    temp_r29->setLoop(0);
                } else {
                    setWaterInAnmRate(temp_r29, -2.0f);
                    temp_r29->setLoop(temp_r29->getEnd());
                }
    
                initBasAnime();
            }
        } else {
            decideCrawlDoStatus();
            shape_angle.y = temp_r27;
            current.angle.y = temp_r26;
        }
    } else {
        int var_r28 = 1;
        if (mProcVar1.field_0x300a > 0) {
            mProcVar1.field_0x300a--;
        }

        if (mProcVar3.field_0x300e != 0) {
            if (cLib_addCalcAngleS(&shape_angle.y, mProcVar2.field_0x300c, 5, 0x480, 0x80)) {
                var_r28 = 0;
            }

            if (mProcVar3.field_0x300e < 0) {
                current.angle.y = shape_angle.y + 0x8000;
            } else {
                current.angle.y = shape_angle.y;
            }

            cLib_addCalc(&current.pos.x, field_0x37c8.x, 0.5f, 3.0f, 1.0f);
            cLib_addCalc(&current.pos.z, field_0x37c8.z, 0.5f, 3.0f, 1.0f);
        }

        cXyz sp8 = field_0x37c8 - current.pos;
        if (cLib_distanceAngleS(sp8.atan2sX_Z(), current.angle.y) < 0x6000) {
            var_r28 = 0;
        }

        mNormalSpeed = getCrawlMoveSpeed();

        if (mNormalSpeed < 0.0f) {
            mNormalSpeed *= -1.0f;
            current.angle.y = shape_angle.y + 0x8000;
        }

        if (var_r28 != 0 || mProcVar1.field_0x300a == 0) {
            procCrawlMoveInit(shape_angle.x, shape_angle.z);
        }
    }

    field_0x310c = shape_angle.y;
    return 1;
}

int daAlink_c::procCrawlEndInit(int param_0, s16 param_1, s16 param_2) {
    daPy_frameCtrl_c* temp_r29 = &mUnderFrameCtrl[0];
    f32 var_f31 = temp_r29->getFrame();

    commonProcInit(PROC_CRAWL_END);

    current.angle.y = shape_angle.y;
    field_0x2f99 = 0xC;

    const daAlinkHIO_anm_c* temp_r30 = &mpHIO->mCrouch.m.mCrawlEndAnm;

    f32 var_f30;
    if (param_0 != 0) {
        var_f30 = temp_r30->mInterpolation;
    } else {
        var_f30 = -1.0f;
    }

    setSingleAnime(ANM_CRAWL_START, temp_r30->mSpeed, temp_r30->mStartFrame, temp_r30->mEndFrame, var_f30);

    if (param_0 == 0) {
        if (temp_r30->mStartFrame > var_f31) {
            var_f31 = temp_r30->mStartFrame;
        } else if (temp_r30->mEndFrame <= var_f31) {
            var_f31 = temp_r30->mEndFrame;
        }

        temp_r29->setFrame(var_f31);
        getNowAnmPackUnder(UNDER_0)->setFrame(var_f31);
    }

    J3DTransformInfo sp10;
    getNowAnmPackUnder(UNDER_0)->getTransform(0, &sp10);

    field_0x34d4.x = sp10.mTranslate.x;
    field_0x34d4.y = sp10.mTranslate.y;
    field_0x34d4.z = sp10.mTranslate.z;

    mNormalSpeed = 0.0f;
    shape_angle.x = param_1;
    shape_angle.z = param_2;

    if (temp_r30->mCancelFrame > temp_r30->mStartFrame) {
        field_0x347c = temp_r30->mCancelFrame;
    } else {
        field_0x347c = temp_r30->mStartFrame;
    }

    field_0x3478 = 1.0f / (temp_r30->mEndFrame - field_0x347c);
    field_0x3588 = l_waitBaseAnime;
    return 1;
}

int daAlink_c::procCrawlEnd() {
    daPy_frameCtrl_c* frame_ctrl = mUnderFrameCtrl;

    field_0x33cc = field_0x3478 * (frame_ctrl->getFrame() - field_0x347c);
    field_0x2f99 = 4;

    if (checkAnmEnd(frame_ctrl)) {
        if (!checkNextActionFromCrouch(0)) {
            procWaitInit();
        }
    } else if (frame_ctrl->getFrame() < mpHIO->mCrouch.m.mCrawlEndAnm.mCancelFrame) {
        checkNextActionFromCrouch(1);
    }

    return 1;
}
