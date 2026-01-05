/**
 * d_meter2.cpp
 * Main UI handler
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_meter2.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "d/d_camera.h"
#include "d/d_demo.h"
#include "d/d_scope.h"
#include "d/d_timer.h"
#include "d/d_menu_window_HIO.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_meter_button.h"
#include "d/d_meter_haihai.h"
#include "d/d_meter_hakusha.h"
#include "d/d_meter_map.h"
#include "d/d_meter_string.h"
#include "f_op/f_op_msg_mng.h"
#include "d/actor/d_a_horse.h"

int dMeter2_c::_create() {
    stage_stag_info_class* stag_info = dComIfGp_getStageStagInfo();
    if (dStage_stagInfo_GetUpButton(stag_info) == 1) {
        mpHeap = fopMsgM_createExpHeap(0x5A400, NULL);
    } else {
        mpHeap = fopMsgM_createExpHeap(0x60800, NULL);
    }

    JKRHeap* heap = mDoExt_setCurrentHeap(mpHeap);
    mpHeap->getTotalFreeSize();

    if (!strcmp(dComIfGp_getStartStageName(), "F_SP00")) {
        dMeter2Info_setNowCount(0);
        dMeter2Info_setMaxCount(0);
    }

    field_0x128 = 0;
    field_0x12c = field_0x128;
    mStatus = 0;
    mSubContentType = 0;
    mSubContentsStringType = 0;
    field_0x1e6 = 0;
    field_0x1e7 = 0;

    mItemMaxNum[ARROW_MAX] = dComIfGs_getArrowMax();
    mArrowNum = dComIfGs_getArrowNum();
    mItemMaxNum[PACHINKO_MAX] = dComIfGs_getPachinkoMax();
    mPachinkoNum = dComIfGs_getPachinkoNum();

    for (int i = 0; i < 3; i++) {
        mBombMax[i] = dComIfGs_getBombMax(dComIfGs_getItem((u8)(i + SLOT_15), 1));
        mBombNum[i] = dComIfGs_getBombNum(i);
    }

    for (int i = 0; i < 2; i++) {
        mItemMaxNum[i] = dComIfGs_getSelectItemIndex(i);
    }

    for (int i = 0; i < 4; i++) {
        mBottleNum[i] = dComIfGs_getBottleNum(i);
    }

    field_0x1e8 = 0;
    field_0x1e9 = 0;
    mRupeeSound = 0;
    mArrowSound = 0;

    for (int i = 0; i < 5; i++) {
        field_0x1b8[i] = 0;
    }

    field_0x1ec = 0;
    field_0x1ed = 0;
    mLifeCountType = 0;

    mNowLifeGauge = dComIfGs_getLife();
    dComIfGp_setItemNowLife((u8)mNowLifeGauge);

    mMaxLife = dComIfGs_getMaxLife();

    mNowMagic = dComIfGs_getMagic();
    dComIfGp_setItemNowMagic(mNowMagic);

    mMaxMagic = dComIfGs_getMaxMagic();

    mNowOil = (s16)dComIfGs_getOil();
    dComIfGp_setItemNowOil(mNowOil);

    mMaxOil = (s16)dComIfGs_getMaxOil();

    mNowOxygen = dComIfGp_getOxygen();
    dComIfGp_setNowOxygen(mNowOxygen);

    mMaxOxygen = dComIfGp_getMaxOxygen();

    field_0x130 = 0.0f;

    u8 dark_area = dComIfGp_getStartStageDarkArea();
    mLightDropNum = dComIfGs_getLightDropNum(dark_area);

    mNeedLightDropNum = dComIfGp_getNeedLightDropNum();

    mRupeeNum = dComIfGs_getRupee();
    mKeyNum = dComIfGs_getKeyNum();

    mDoStatus = dComIfGp_getDoStatus();
    mDoSetFlag = dComIfGp_isDoSetFlag(2);

    for (int i = 0; i < 2; i++) {
        mButtonATalkPosX[i] = 0.0f;
        mButtonATalkPosY[i] = 0.0f;
    }
    field_0x144 = 1.0f;

    mAStatus = dComIfGp_getAStatus();
    field_0x1c6 = 0;
    mASetFlag = dComIfGp_isASetFlag(2);

    for (int i = 0; i < 2; i++) {
        field_0x148[i] = 0.0f;
        field_0x150[i] = 0.0f;
    }
    field_0x158 = 1.0f;

    field_0x1e4 = 0;
    mEquipSword = dComIfGs_getSelectEquipSword();
    field_0x1e3 = 0;

    field_0x1fe = 0;
    field_0x1fd = 0;
    field_0x1ff = 0;
    field_0x200 = 0;
    field_0x201 = 0;

    mCollectSmell = dComIfGs_getCollectSmell();
    mRStatus = dComIfGp_getRStatus();
    mRSetFlag = dComIfGp_isRSetFlag(2);
    mXSetFlag = dComIfGp_isXSetFlag(2);
    mYSetFlag = dComIfGp_isYSetFlag(2);

    for (int i = 0; i < 2; i++) {
        dComIfGp_setSelectItem(i);
    }

    mItemStatus[X_ITEM] = dComIfGp_getSelectItem(0);
    mItemStatus[Y_ITEM] = dComIfGp_getSelectItem(1);
    mItemStatus[X_STATUS] = dComIfGp_getXStatus();
    mItemStatus[Y_STATUS] = dComIfGp_getYStatus();
    field_0x188 = 0.0f;
    field_0x18c = 0.0f;

    for (int i = 0; i < 2; i++) {
        field_0x1d6[i] = dMeter2Info_isDirectUseItem(i);
        field_0x1d8[i] = dComIfGp_getSelectItemNum(i);
    }

    field_0x1e1 = 0;
    field_0x1b4 = 0;
    field_0x15c = 0.0f;

    for (int i = 0; i < 4; i++) {
        field_0x160[i] = 0.0f;
        field_0x174[i] = 0.0f;
    }
    field_0x190 = 0;

    mZStatus = dComIfGp_getZStatus();
    m3DStatus = dComIfGp_get3DStatus();
    mCStickStatus = dComIfGp_getCStickStatus();
    mSButtonStatus = dComIfGp_getSButtonStatus();
    mNunStatus = dComIfGp_getNunStatus();
    mRemoConStatus = dComIfGp_getRemoConStatus();
    mNunZStatus = dComIfGp_getNunZStatus();
    mNunCStatus = dComIfGp_getNunCStatus();
    mBottleStatus = dComIfGp_getBottleStatus();

    field_0x1ac = dMeter2Info_isUseButton(16);
    field_0x19a = 0;

    mpMeterDraw = new dMeter2Draw_c(mpHeap);

    field_0x130 = mpMeterDraw->getNowLightDropRateCalc();
    mpHeap->getTotalFreeSize();

    for (int i = 0; i < 2; i++) {
        if (field_0x128 == 0) {
            if (mItemStatus[i * 2] == fpcNm_ITEM_BOMB_BAG_LV1 || mItemStatus[i * 2] == fpcNm_ITEM_NORMAL_BOMB ||
                mItemStatus[i * 2] == fpcNm_ITEM_WATER_BOMB || mItemStatus[i * 2] == fpcNm_ITEM_POKE_BOMB)
            {
                mpMeterDraw->setItemNum(i, dComIfGp_getSelectItemNum(i),
                                        dComIfGp_getSelectItemMaxNum(i));
            } else if (mItemStatus[i * 2] == fpcNm_ITEM_BEE_CHILD) {
                mpMeterDraw->setItemNum(i, dComIfGp_getSelectItemNum(i),
                                        dComIfGp_getSelectItemMaxNum(i));
            } else if (mItemStatus[i * 2] == fpcNm_ITEM_BOW || mItemStatus[i * 2] == fpcNm_ITEM_LIGHT_ARROW ||
                       mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV1 || mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV2 ||
                       mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV3 || mItemStatus[i * 2] == fpcNm_ITEM_HAWK_ARROW)
            {
                mpMeterDraw->setItemNum(i, mArrowNum, dComIfGs_getArrowMax());
            } else if (mItemStatus[i * 2] == fpcNm_ITEM_PACHINKO) {
                mpMeterDraw->setItemNum(i, mPachinkoNum, dComIfGs_getPachinkoMax());
            } else if (mItemStatus[i * 2] == fpcNm_ITEM_BOMB_ARROW) {
                u8 item_num = dComIfGp_getSelectItemNum(i);
                u8 item_max = dComIfGp_getSelectItemMaxNum(i);
                if (item_num > mArrowNum) {
                    item_num = mArrowNum;
                }
                u8 temp = dComIfGs_getArrowMax() & 0xFF;
                if (item_max < temp) {
                    item_max = temp;
                }
                mpMeterDraw->setItemNum(i, item_num, item_max);
            }
        }
    }

    mpMap = NULL;
    if (dMeterMap_c::isEnableDispMapAndMapDispSizeTypeNo()) {
        mpMap = new dMeterMap_c(mpMeterDraw->getMainScreenPtr());
    } else {
        if (g_meter2_info.mMapStatus == 2) {
            g_meter2_info.mMapStatus = 0;
        }
    }
    dMeter2Info_setMeterMapClass(mpMap);

    mpHeap->getTotalFreeSize();
    mpSubHeap = fopMsgM_createExpHeap(0x5000, mpHeap);
    field_0x108 = NULL;
    mpSubContents = NULL;
    mpSubSubContents = NULL;
    mpEmpButton = NULL;

    mpHeap->getTotalFreeSize();
    field_0x11c = NULL;
    mDoExt_setCurrentHeap(heap);
    return cPhs_COMPLEATE_e;
}

int dMeter2_c::_execute() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpHeap);

    if (!dComIfGs_isCollectMirror(0)
           /* dSv_event_flag_c::F_0685 - Cutscene - (Cutscene 32) Sage appears, get first Mirror of Twilight shard */
        && dComIfGs_isEventBit(dSv_event_flag_c::F_0685)) {
        dComIfGs_onCollectMirror(0);
    }

    if (!dComIfGs_isCollectCrystal(3)
           /* dSv_event_flag_c::F_0686 - Palace of Twilight - Get fused shadow piece (final mask) */
        && dComIfGs_isEventBit(dSv_event_flag_c::F_0686)) {
        dComIfGs_onCollectCrystal(3);
    }

    checkStatus();
    mpMeterDraw->exec(mStatus);

    moveLife();
    moveKantera();
    moveOxygen();
    moveLightDrop();
    moveRupee();
    moveKey();
    moveButtonXY();
    moveButtonA();
    moveButtonB();
    moveButtonR();
    moveButtonZ();
    moveButton3D();
    moveButtonC();
    moveButtonS();
    moveButtonCross();
    moveTouchSubMenu();
    moveBombNum();
    moveArrowNum();
    movePachinkoNum();
    moveBottleNum();

    if (mpMap != NULL) {
        mpMap->_move(mStatus);
    } else {
        dMeterMap_c::meter_map_move(mStatus);
    }

    moveSubContents();
    move2DContents();

    if (!dComIfGp_isPauseFlag() && !dComIfGp_event_runCheck()) {
        dMeter2Info_decHotSpringTimer();
    }

    dMeter2Info_allUseButton();
    dMeter2Info_offUseButton(0x800);
    dMeter2Info_resetGameStatus();
    dComIfGp_setNunStatus(0, 0, 0);
    dComIfGp_setRemoConStatus(0, 0, 0);
    dComIfGp_setNunZStatus(0, 0);
    dComIfGp_setNunCStatus(0, 0);
    dComIfGp_setBottleStatus(0, 0);
    dComIfGp_setCStickStatus(0, 0, 0);

    mDoExt_setCurrentHeap(heap);
    return 1;
}

int dMeter2_c::_draw() {
    if (mpMap != NULL) {
        mpMap->_draw();
    }

    if (mpSubContents != NULL) {
        dComIfGd_set2DOpaTop(mpSubContents);
    }

    if (mpSubSubContents != NULL) {
        if (mSubContentType == 5) {
            if (mSubContentsStringType != 0) {
                dComIfGd_set2DOpaTop(mpSubSubContents);
            }
        } else {
            dComIfGd_set2DOpaTop(mpSubSubContents);
        }
    }

    if (dMeter2Info_getWindowStatus() == 2) {
        dComIfGd_set2DOpa(mpMeterDraw);
    } else {
        dComIfGd_set2DOpaTop(mpMeterDraw);
    }

    if (mpEmpButton != NULL) {
        dComIfGd_set2DOpaTop(mpEmpButton);
    }

    return 1;
}

int dMeter2_c::_delete() {
    mpHeap->getTotalFreeSize();
    JKRHeap* heap = mDoExt_setCurrentHeap(mpHeap);

    if (isRupeeSoundBit(2)) {
        mDoAud_seStart(Z2SE_LUPY_INC_CNT_2, 0, 0, 0);
    }

    if (isRupeeSoundBit(3)) {
        mDoAud_seStart(Z2SE_LUPY_DEC_CNT_2, 0, 0, 0);
    }

    if (isArrowSoundBit(2) && (isArrowEquip() || isPachinkoEquip()) &&
        mpMeterDraw->isButtonVisible())
    {
        mDoAud_seStart(Z2SE_CONSUM_INC_CNT_2, 0, 0, 0);
    }

    mpHeap->getTotalFreeSize();
    if (mpMap != NULL) {
        delete mpMap;
        mpMap = NULL;
    }

    mpHeap->getTotalFreeSize();
    mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSubContents != NULL) {
        delete mpSubContents;
        mpSubContents = NULL;
    }

    if (mpSubSubContents != NULL) {
        delete mpSubSubContents;
        mpSubSubContents = NULL;
    }

    mpHeap->getTotalFreeSize();
    if (field_0x11c != NULL) {
        delete field_0x11c;
        field_0x11c = NULL;
    }

    mDoExt_setCurrentHeap(heap);
    delete mpMeterDraw;
    mpMeterDraw = NULL;

    fopMsgM_destroyExpHeap(mpSubHeap);
    fopMsgM_destroyExpHeap(mpHeap);
    emphasisButtonDelete();
    return 1;
}

int dMeter2_c::emphasisButtonDelete() {
    if (mpEmpButton != NULL) {
        JKRExpHeap* heap = dComIfGp_getSubHeap2D(8);
        mpEmpButton->hideAll();

        if (heap != NULL) {
            delete mpEmpButton;
            mpEmpButton = NULL;
            heap->freeAll();
            dComIfGp_offHeapLockFlag(8);
        }
    }

    return 1;
}

void dMeter2_c::setLifeZero() {
    dComIfGs_setLife(1);
    setNowLifeGauge(1);
    dComIfGp_setItemLifeCount(-1.0f, 0);
}

void dMeter2_c::checkStatus() {
    mStatus = 0;
    field_0x12c = field_0x128;

    field_0x128 = daPy_py_c::checkNowWolf();

    if (!dComIfGp_2dShowCheck() || dMsgObject_getMsgObjectClass()->isPlaceMessage()) {
        mStatus |= 0x4000;
    } else if (dComIfGp_checkPlayerStatus1(0, 1) && dComIfGp_getAStatus() == 0x12) {
        mStatus |= 0x200000;
    } else if (dComIfGp_event_runCheck()) {
        mStatus |= 0x40;

        if (dDemo_c::getMode() != 1 && dComIfGp_isHeapLockFlag() != 6 &&
            dMsgObject_isTalkNowCheck() && !dMeter2Info_isFloatingMessageVisible())
        {
            mStatus |= 0x100;
            dComIfGp_setAStatus(0, 0);
            dComIfGp_setRStatus(0, 0);
        }
    }

    if (dComIfGp_isHeapLockFlag() != 6 && !(mStatus & 0x100) && dMsgObject_isTalkNowCheck() &&
        !dMeter2Info_isFloatingMessageVisible())
    {
        mStatus |= 0x100;
        dComIfGp_setAStatus(0, 0);
        dComIfGp_setRStatus(0, 0);
    }

    if (!dComIfGp_isPauseFlag()) {
        if (mpMeterDraw->getCameraSubject()) {
            mStatus |= 0x40000000;
        } else if (mpMeterDraw->getItemSubject()) {
            mStatus |= 0x1000;
        }

        if (mpMeterDraw->getPlayerSubject()) {
            mStatus |= 0x100000;
        }

        if (dComIfGp_getCameraAttentionStatus(0) & 0x40000) {
            mStatus |= 0x80000000;
            dComIfGp_setDoStatusForce(0x12, 2);
        }

        s16 sp8;
        s16 spA;

        if (dComIfGp_checkCameraAttentionStatus(0, 8)) {
            mStatus |= 0x80;
        } else if (dComIfGp_checkCameraAttentionStatus(dComIfGp_getPlayerCameraID(0), 0x10) &&
                   dCam_getBody()->CalcSubjectAngle(&sp8, &spA))
        {
            mStatus |= 0x20000000;
        } else if (daPy_getPlayerActorClass()->checkCanoeRide()) {
            mStatus |= 0x400;
        } else if (dComIfGp_checkPlayerStatus0(0, 0x8000000) ||
                   daPy_getPlayerActorClass()->checkSpinnerRide())
        {
            mStatus |= 0x200;
        } else if (dComIfGp_checkPlayerStatus0(0, 0x800000)) {
            mStatus |= 0x800;
        } else if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
            mStatus |= 0x2000;
        } else if (dComIfGp_checkPlayerStatus0(0, 0x2000108)) {
            mStatus |= 0x8000;
        } else if (dComIfGp_checkPlayerStatus0(0, 0x4000000)) {
            mStatus |= 0x10000;
        } else if (daPy_getPlayerActorClass()->checkHorseRideNotReady() &&
                   dComIfGp_getHorseActor() != NULL &&
                   !dComIfGp_getHorseActor()->checkRodeoMode())
        {
            mStatus |= 0x2000000;
        }
    }

    switch (dMeter2Info_getWindowStatus()) {
    case 1:
        mStatus |= 0x8;
        break;
    case 3:
    case 10:
        mStatus |= 0x10;
        break;
    case 2:
        mStatus |= 0x1000000;
        dComIfGp_setAStatus(0x12, 0);
        dComIfGp_setDoStatus(0, 0);
        dComIfGp_setRStatus(0, 0);
        break;
    case 4:
    case 6:
        mStatus |= 0x20;
        dComIfGp_setAStatus(0x12, 0);
        dComIfGp_setDoStatus(0, 0);
        dComIfGp_setRStatus(0, 0);
        break;
    case 5:
        mStatus |= 0x4000000;
        dComIfGp_setAStatus(0x12, 0);
        dComIfGp_setDoStatus(0, 0);
        dComIfGp_setRStatus(0, 0);
        break;
    case 7:
        mStatus |= 0x8000000;
        dComIfGp_setAStatus(0x12, 0);
        dComIfGp_setDoStatus(0x22, 0);
        dComIfGp_setRStatus(0, 0);
        break;
    case 8:
        mStatus |= 0x10000000;
        dComIfGp_setAStatus(0, 0);
        dComIfGp_setDoStatus(0, 0);
        dComIfGp_setRStatus(0, 0);
        break;
    }

    switch (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo())) {
    case ST_DUNGEON:
        mStatus |= 0x4;
        break;
    case ST_ROOM:
        mStatus |= 0x2;
        break;
    case ST_FIELD:
    default:
        mStatus |= 0x1;
        break;
    }
}

void dMeter2_c::moveLife() {
    s16 life_count = 0;
    bool draw_life = false;

    if (dComIfGp_getItemMaxLifeCount() != 0) {
        s16 max_count = dComIfGs_getMaxLife() + dComIfGp_getItemMaxLifeCount();
        if (max_count > 100) {
            max_count = 100;
        } else if (max_count < 15) {
            max_count = 15;
        }

        life_count = (max_count / 5) * 4;
        dComIfGs_setMaxLife(max_count);

        s16 current_life = life_count - dComIfGs_getLife();
        dComIfGp_setItemLifeCount(current_life, 0);
        dComIfGp_clearItemMaxLifeCount();
        draw_life = true;
    }

    f32 item_life_count = dComIfGp_getItemLifeCount();
    f32 tmp = 0.0f;
    if (item_life_count != tmp) {
        mLifeCountType = dComIfGp_getItemLifeCountType();
        if (!draw_life) {
            life_count = (dComIfGs_getMaxLife() / 5) * 4;
        }

        s16 new_life = dComIfGs_getLife() + dComIfGp_getItemLifeCount();
        if (new_life > life_count) {
            new_life = life_count;
        } else if (new_life < 0) {
            new_life = 0;
        }

        dComIfGs_setLife((u8)new_life);
        dComIfGp_clearItemLifeCount();

        if (mNowLifeGauge == dComIfGs_getLife() && mLifeCountType != 0) {
            mLifeCountType = 0;
        }
        draw_life = true;
    }

    u16 max_life = dComIfGs_getMaxLife();
    if (mMaxLife != max_life) {
        if (mMaxLife < max_life) {
            mMaxLife++;
            dMeter2Info_onLifeGaugeSE();
            draw_life = true;
        } else if (mMaxLife > max_life) {
            mMaxLife--;
            draw_life = true;
        }
    }

    u16 current_life = dComIfGs_getLife();
    if (mNowLifeGauge != current_life) {
        if (mNowLifeGauge < current_life) {
            mNowLifeGauge++;
            if (dComIfGp_checkPlayerStatus1(0, 0x2000) ||
                dComIfGp_checkPlayerStatus0(0, 0x20000000) || dMeter2Info_getLifeGaugeSE())
            {
                if (mNowLifeGauge % 4 == 0) {
                    mDoAud_seStart(Z2SE_HP_GAUGE_INC, 0, 0, 0);
                }
            } else if (mLifeCountType == 1) {
                mDoAud_seStart(Z2SE_HP_GAUGE_INC, 0, 0, 0);
            }

            u16 life = dComIfGs_getLife();
            if (mNowLifeGauge == life && mLifeCountType != 0) {
                mLifeCountType = 0;
            }
            draw_life = true;
        } else if (mNowLifeGauge > current_life) {
            mNowLifeGauge--;
            if (mLifeCountType != 0) {
                mLifeCountType = 0;
            }
            draw_life = true;
        }
    } else if (dMeter2Info_getLifeGaugeSE()) {
        dMeter2Info_offLifeGaugeSE();
    }

    f32 life_gauge_x = g_drawHIO.mLifeGaugePosX;
    f32 life_gauge_y = g_drawHIO.mLifeGaugePosY;

    if (mLifeGaugePosX != g_drawHIO.mLifeGaugePosX) {
        mLifeGaugePosX = g_drawHIO.mLifeGaugePosX;
        draw_life = true;
    }

    if (mLifeGaugePosY != g_drawHIO.mLifeGaugePosY) {
        mLifeGaugePosY = g_drawHIO.mLifeGaugePosY;
        draw_life = true;
    }

    if (mLifeGaugeScale != g_drawHIO.mLifeParentScale) {
        mLifeGaugeScale = g_drawHIO.mLifeParentScale;
        draw_life = true;
    }

    if (mHeartScale != g_drawHIO.mHeartMarkScale) {
        mHeartScale = g_drawHIO.mHeartMarkScale;
        draw_life = true;
    }

    if (mLargeHeartScale != g_drawHIO.mBigHeartScale) {
        mLargeHeartScale = g_drawHIO.mBigHeartScale;
        draw_life = true;
    }

    if (draw_life == true) {
        mpMeterDraw->drawLife(mMaxLife, mNowLifeGauge, life_gauge_x, life_gauge_y);
    }

    alphaAnimeLife();
    dComIfGp_setItemNowLife((u8)mNowLifeGauge);
}

void dMeter2_c::moveKantera() {
    s32 var_r0;
    s32 var_r7;
    bool draw_kantera;

    s32 max_oil = dComIfGs_getMaxOil();
    var_r7 = 0;
    draw_kantera = false;

    if (dComIfGp_getItemMaxOilCount() != 0) {
        var_r7 = dComIfGs_getMaxOil() + dComIfGp_getItemMaxOilCount();
        if (var_r7 > max_oil) {
            var_r7 = max_oil;
        } else if (var_r7 < 0) {
            var_r7 = 0;
        }

        dComIfGs_setMaxOil(var_r7);
        dComIfGp_setItemOilCount(var_r7 - dComIfGs_getOil());
        dComIfGp_clearItemMaxOilCount();
        draw_kantera = true;
    }

    if (dComIfGp_getItemOilCount() != 0) {
        if (draw_kantera == 0) {
            var_r7 = dComIfGs_getMaxOil();
        }

        var_r0 = dComIfGs_getOil() + dComIfGp_getItemOilCount();
        if (var_r0 > var_r7) {
            var_r0 = var_r7;
        } else if (var_r0 < 0) {
            var_r0 = 0;
        }

        dComIfGs_setOil(var_r0);
        dComIfGp_clearItemOilCount();
        draw_kantera = true;
    }

    if (mMaxOil != dComIfGs_getMaxOil()) {
        if (mMaxOil < dComIfGs_getMaxOil()) {
            mMaxOil += 200;
            if (mMaxOil > dComIfGs_getMaxOil()) {
                mMaxOil = dComIfGs_getMaxOil();
            }
            draw_kantera = true;
        } else if (mMaxOil > dComIfGs_getMaxOil()) {
            mMaxOil -= 200;
            if (mMaxOil < dComIfGs_getMaxOil()) {
                mMaxOil = dComIfGs_getMaxOil();
            }
            draw_kantera = true;
        }
    }

    if (mNowOil != dComIfGs_getOil()) {
        if (mNowOil < dComIfGs_getOil()) {
            mNowOil += 200;
            if (mNowOil > dComIfGs_getOil()) {
                mNowOil = dComIfGs_getOil();
            }

            if (!dComIfGp_getOxygenShowFlag() && mpMeterDraw->getMeterGaugeAlphaRate(1) > 0.0f) {
                Z2GetAudioMgr()->seStartLevel(Z2SE_OIL_METER_RECOVER, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                              -1.0f, 0);
            }
            draw_kantera = true;
        } else if (mNowOil > dComIfGs_getOil()) {
            mNowOil -= 200;
            if (mNowOil < dComIfGs_getOil()) {
                mNowOil = dComIfGs_getOil();
            }

            if (mNowOil == 0) {
                if (mpMeterDraw->getMeterGaugeAlphaRate(1) > 0.0f) {
                    Z2GetAudioMgr()->seStart(Z2SE_OIL_METER_FINISH, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                             -1.0f, 0);
                }
            } else if (((f32)dComIfGs_getOil() / (f32)dComIfGs_getMaxOil()) <= 0.1f &&
                       mpMeterDraw->getMeterGaugeAlphaRate(1) > 0.0f)
            {
                Z2GetAudioMgr()->seStartLevel(Z2SE_OIL_METER_LESS, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                              -1.0f, 0);
            }

            draw_kantera = true;
        }
    }

    f32 x_pos = g_drawHIO.mLanternMeterPosX;
    f32 y_pos = g_drawHIO.mLanternMeterPosY;

    if (field_0x246 != mMaxLife) {
        field_0x246 = mMaxLife;
        draw_kantera = true;
    }

    if (mLanternMeterScale != g_drawHIO.mLanternMeterScale) {
        mLanternMeterScale = g_drawHIO.mLanternMeterScale;
        draw_kantera = true;
    }

    if (mLanternMeterPosX != g_drawHIO.mLanternMeterPosX) {
        mLanternMeterPosX = g_drawHIO.mLanternMeterPosX;
        draw_kantera = true;
    }

    if (mLanternMeterPosY != g_drawHIO.mLanternMeterPosY) {
        mLanternMeterPosY = g_drawHIO.mLanternMeterPosY;
        draw_kantera = true;
    }

    if (draw_kantera == true) {
        mpMeterDraw->drawKantera(mMaxOil, mNowOil, x_pos, y_pos);
    }

    alphaAnimeKantera();
    dComIfGp_setItemNowOil(mNowOil);
}

void dMeter2_c::moveOxygen() {
    s32 var_r0;
    s32 var_r7;
    bool draw_oxygen;

    s32 var_r4 = dComIfGp_getMaxOxygen();
    var_r7 = 0;
    draw_oxygen = false;

    if (dComIfGp_getMaxOxygenCount() != 0) {
        var_r7 = dComIfGp_getMaxOxygen() + dComIfGp_getMaxOxygenCount();
        if (var_r7 > var_r4) {
            var_r7 = var_r4;
        } else if (var_r7 < 0) {
            var_r7 = 0;
        }

        dComIfGp_setMaxOxygen((u16)var_r7);
        dComIfGp_setOxygenCount(var_r7 - dComIfGp_getOxygen());
        dComIfGp_clearMaxOxygenCount();
        draw_oxygen = true;
    }

    if (dComIfGp_getOxygenCount() != 0) {
        if (draw_oxygen == 0) {
            var_r7 = dComIfGp_getMaxOxygen();
        }

        var_r0 = dComIfGp_getOxygen() + dComIfGp_getOxygenCount();
        if (var_r0 > var_r7) {
            var_r0 = var_r7;
        } else if (var_r0 < 0) {
            var_r0 = 0;
        }

        if (dComIfGp_getOxygenShowFlag() && var_r0 == dComIfGp_getMaxOxygen()) {
            if (dComIfGp_getOxygen() < dComIfGp_getMaxOxygen() &&
                mpMeterDraw->getMeterGaugeAlphaRate(2) > 0.0f)
            {
                Z2GetAudioMgr()->seStart(Z2SE_SWIM_TIMER_RECOVER, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
        }

        dComIfGp_setOxygen((u16)var_r0);
        dComIfGp_clearOxygenCount();
        draw_oxygen = true;
    }

    if (mMaxOxygen != dComIfGp_getMaxOxygen()) {
        if (mMaxOxygen < dComIfGp_getMaxOxygen()) {
            mMaxOxygen += 50;
            if (mMaxOxygen > dComIfGp_getMaxOxygen()) {
                mMaxOxygen = dComIfGp_getMaxOxygen();
            }
            draw_oxygen = true;
        } else if (mMaxOxygen > dComIfGp_getMaxOxygen()) {
            mMaxOxygen -= 50;
            if (mMaxOxygen < dComIfGp_getMaxOxygen()) {
                mMaxOxygen = dComIfGp_getMaxOxygen();
            }
            draw_oxygen = true;
        }
    }

    if (mNowOxygen != dComIfGp_getOxygen()) {
        if (mNowOxygen < dComIfGp_getOxygen()) {
            mNowOxygen += 50;
            if (mNowOxygen > dComIfGp_getOxygen()) {
                mNowOxygen = dComIfGp_getOxygen();
            }

            draw_oxygen = true;
        } else if (mNowOxygen > dComIfGp_getOxygen()) {
            mNowOxygen -= 50;
            if (mNowOxygen < dComIfGp_getOxygen()) {
                mNowOxygen = dComIfGp_getOxygen();
            }

            if ((f32)dComIfGp_getOxygen() / (f32)dComIfGp_getMaxOxygen() > 0.5f &&
                mpMeterDraw->getMeterGaugeAlphaRate(2) > 0.0f)
            {
                Z2GetAudioMgr()->seStartLevel(Z2SE_SWIM_TIMER_DEC, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                              -1.0f, 0);
            }

            draw_oxygen = true;
        }
    }

    f32 x_pos = g_drawHIO.mOxygenMeterPosX;
    f32 y_pos = g_drawHIO.mOxygenMeterPosY;

    if (field_0x248 != mMaxLife) {
        field_0x248 = mMaxLife;
        draw_oxygen = true;
    }

    if (mOxygenMeterScale != g_drawHIO.mOxygenMeterScale) {
        mOxygenMeterScale = g_drawHIO.mOxygenMeterScale;
        draw_oxygen = true;
    }

    if (mOxygenMeterPosX != g_drawHIO.mOxygenMeterPosX) {
        mOxygenMeterPosX = g_drawHIO.mOxygenMeterPosX;
        draw_oxygen = true;
    }

    if (mOxygenMeterPosY != g_drawHIO.mOxygenMeterPosY) {
        mOxygenMeterPosY = g_drawHIO.mOxygenMeterPosY;
        draw_oxygen = true;
    }

    if (draw_oxygen == true) {
        mpMeterDraw->drawOxygen(mMaxOxygen, mNowOxygen, x_pos, y_pos);
    }

    alphaAnimeOxygen();
    dComIfGp_setNowOxygen(mNowOxygen);
}

void dMeter2_c::moveLightDrop() {
    f32 scale;
    f32 alpha;
    f32 pos_x;
    f32 pos_y;
    bool draw_lightdrop;
    u8 var_r28;

    draw_lightdrop = false;
    var_r28 = 0;
    if (g_drawHIO.mLightDrop.mAnimDebug) {
        u8 dark_level = dComIfGp_getStartStageDarkArea();
        if (dComIfGp_getNeedLightDropNum() == dComIfGs_getLightDropNum(dark_level)) {
            var_r28 = 2;
        }
    }

    if (mLightDropNum != dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea())) {
        if (dComIfGp_getNeedLightDropNum() ==
            dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea()))
        {
            var_r28 = 2;
            Z2GetAudioMgr()->seStart(Z2SE_SY_LIGHT_DROP_COMPLETE, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
        } else if (mLightDropNum < dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea())) {
            var_r28 = 1;
            Z2GetAudioMgr()->seStart(Z2SE_SY_LIGHT_DROP_GET, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }

        mLightDropNum = dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea());
        draw_lightdrop = true;
    }

    if (mNeedLightDropNum != dComIfGp_getNeedLightDropNum()) {
        mNeedLightDropNum = dComIfGp_getNeedLightDropNum();
        draw_lightdrop = true;
    }

    if ((mStatus & 0x100) || daPy_getPlayerActorClass()->checkHawkWait() ||
        daPy_getPlayerActorClass()->checkGrassWhistle() ||
        g_meter2_info.mItemExplainWindowStatus != 0)
    {
        pos_x = g_drawHIO.mLightDrop.mVesselTalkPosX;
        pos_y = g_drawHIO.mLightDrop.mVesselTalkPosY;
        scale = g_drawHIO.mLightDrop.mVesselTalkScale;
        alpha = g_drawHIO.mLightDrop.mVesselAlpha[1];
    } else {
        pos_x = g_drawHIO.mLightDrop.mVesselPosX;
        pos_y = g_drawHIO.mLightDrop.mVesselPosY;
        scale = g_drawHIO.mLightDrop.mVesselScale;
        alpha = g_drawHIO.mLightDrop.mVesselAlpha[0];
    }

    if (mVesselPosX != pos_x) {
        cLib_addCalc2(&mVesselPosX, pos_x, 1.0f, 10.0f);
        draw_lightdrop = true;
        if ((f32)fabs(mVesselPosX - pos_x) < 0.1f) {
            mVesselPosX = pos_x;
        }
    }

    if (mVesselPosY != pos_y) {
        cLib_addCalc2(&mVesselPosY, pos_y, 1.0f, 10.0f);
        draw_lightdrop = true;
        if ((f32)fabs(mVesselPosY - pos_y) < 0.1f) {
            mVesselPosY = pos_y;
        }
    }

    if (mVesselScale != scale) {
        cLib_addCalc2(&mVesselScale, scale, 0.2f, 1.0f);
        draw_lightdrop = true;
        if ((f32)fabs(mVesselScale - scale) < 0.1f) {
            mVesselScale = scale;
        }
    }

    if (mVesselAlpha != alpha) {
        cLib_addCalc2(&mVesselAlpha, alpha, 0.2f, 1.0f);
        draw_lightdrop = true;
        if ((f32)fabs(mVesselAlpha - alpha) < 0.1f) {
            mVesselAlpha = alpha;
        }
    }

    if (draw_lightdrop == true) {
        mpMeterDraw->drawLightDrop(mLightDropNum, mNeedLightDropNum, mVesselPosX, mVesselPosY,
                                   mVesselScale, mVesselAlpha, var_r28);
    }

    alphaAnimeLightDrop();
}

void dMeter2_c::moveRupee() {
    s16 temp_r5;
    s32 temp_r0;

    temp_r5 = dComIfGs_getRupeeMax();
    s16 r29 = 0;
    bool draw_rupee = false;

    if (dComIfGp_getItemRupeeCount() != 0) {
        r29 = dComIfGs_getRupee() + dComIfGp_getItemRupeeCount();
        if (r29 > temp_r5) {
            r29 = temp_r5;
        } else if (r29 < 0) {
            r29 = 0;
        }

        dComIfGs_setRupee(r29);
        dComIfGp_clearItemRupeeCount();

        if (dComIfGs_getRupee() - mRupeeNum >= 5) {
            onRupeeSoundBit(2);
            if (isRupeeSoundBit(3)) {
                offRupeeSoundBit(3);
                offRupeeSoundBit(1);
            }
        } else if (dComIfGs_getRupee() - mRupeeNum <= -5) {
            onRupeeSoundBit(3);
            if (isRupeeSoundBit(2)) {
                offRupeeSoundBit(2);
                offRupeeSoundBit(0);
            }
        }
    }

    if (mRupeeNum != dComIfGs_getRupee()) {
        if (mRupeeNum < dComIfGs_getRupee()) {
            mRupeeNum++;
            draw_rupee = true;

            if (isRupeeSoundBit(2) & 1) {
                if (mRupeeNum != dComIfGs_getRupee()) {
                    if (!isRupeeSoundBit(0)) {
                        onRupeeSoundBit(0);
                        mDoAud_seStart(Z2SE_LUPY_INC_CNT_1, NULL, 0, 0);
                    } else {
                        offRupeeSoundBit(0);
                    }
                } else {
                    mDoAud_seStart(Z2SE_LUPY_INC_CNT_2, NULL, 0, 0);
                    offRupeeSoundBit(2);
                    offRupeeSoundBit(0);
                }
            }
        } else if (mRupeeNum > dComIfGs_getRupee()) {
            mRupeeNum--;
            draw_rupee = true;

            if (isRupeeSoundBit(3) & 1) {
                if (mRupeeNum != dComIfGs_getRupee()) {
                    if (!isRupeeSoundBit(1)) {
                        onRupeeSoundBit(1);
                        mDoAud_seStart(Z2SE_LUPY_DEC_CNT_1, NULL, 0, 0);
                    } else {
                        offRupeeSoundBit(1);
                    }
                } else {
                    mDoAud_seStart(Z2SE_LUPY_DEC_CNT_2, NULL, 0, 0);
                    offRupeeSoundBit(3);
                    offRupeeSoundBit(1);
                }
            }
        }
    }

    if (mRupeeKeyScale != g_drawHIO.mRupeeKeyScale) {
        mRupeeKeyScale = g_drawHIO.mRupeeKeyScale;
        draw_rupee = true;
    }

    if (mRupeeKeyPosX != g_drawHIO.mRupeeKeyPosX) {
        mRupeeKeyPosX = g_drawHIO.mRupeeKeyPosX;
        draw_rupee = true;
    }

    if (mRupeeKeyPosY != g_drawHIO.mRupeeKeyPosY) {
        mRupeeKeyPosY = g_drawHIO.mRupeeKeyPosY;
        draw_rupee = true;
    }

    if (mRupeeScale != g_drawHIO.mRupeeScale) {
        mRupeeScale = g_drawHIO.mRupeeScale;
        draw_rupee = true;
    }

    if (mRupeePosX != g_drawHIO.mRupeePosX) {
        mRupeePosX = g_drawHIO.mRupeePosX;
        draw_rupee = true;
    }

    if (mRupeePosY != g_drawHIO.mRupeePosY) {
        mRupeePosY = g_drawHIO.mRupeePosY;
        draw_rupee = true;
    }

    if (mRupeeFramePosY != g_drawHIO.mRupeeFramePosY) {
        mRupeeFramePosY = g_drawHIO.mRupeeFramePosY;
        draw_rupee = true;
    }

    if (mRupeeFrameScale != g_drawHIO.mRupeeFrameScale) {
        mRupeeFrameScale = g_drawHIO.mRupeeFrameScale;
        draw_rupee = true;
    }

    if (mRupeeFramePosX != g_drawHIO.mRupeeFramePosX) {
        mRupeeFramePosX = g_drawHIO.mRupeeFramePosX;
        draw_rupee = true;
    }

    if (mRupeeCountScale != g_drawHIO.mRupeeCountScale) {
        mRupeeCountScale = g_drawHIO.mRupeeCountScale;
        draw_rupee = true;
    }

    if (mRupeeCountPosX != g_drawHIO.mRupeeCountPosX) {
        mRupeeCountPosX = g_drawHIO.mRupeeCountPosX;
        draw_rupee = true;
    }

    if (mRupeeCountPosY != g_drawHIO.mRupeeCountPosY) {
        mRupeeCountPosY = g_drawHIO.mRupeeCountPosY;
        draw_rupee = true;
    }

    if (mWalletSize != dComIfGs_getWalletSize()) {
        mWalletSize = dComIfGs_getWalletSize();
        draw_rupee = true;
    }

    if (draw_rupee == true) {
        mpMeterDraw->drawRupee(mRupeeNum);
    }

    alphaAnimeRupee();
}

void dMeter2_c::moveKey() {
    s16 var_r5;
    bool draw_key;

    draw_key = false;
    if (dComIfGp_getItemKeyNumCount() != 0) {
        var_r5 = dComIfGs_getKeyNum() + dComIfGp_getItemKeyNumCount();
        if (var_r5 > 99) {
            var_r5 = 99;
        } else if (var_r5 < 0) {
            var_r5 = 0;
        }

        dComIfGs_setKeyNum(var_r5);
        dComIfGp_clearItemKeyNumCount();
    }

    if (mKeyNum != dComIfGs_getKeyNum()) {
        if (mKeyNum < dComIfGs_getKeyNum()) {
            mKeyNum++;
            draw_key = true;
        } else if (mKeyNum > dComIfGs_getKeyNum()) {
            mKeyNum--;
            draw_key = true;
        }
    }

    if (mKeyScale != g_drawHIO.mKeyScale) {
        mKeyScale = g_drawHIO.mKeyScale;
        draw_key = true;
    }

    if (mKeyPosX != g_drawHIO.mKeyPosX) {
        mKeyPosX = g_drawHIO.mKeyPosX;
        draw_key = true;
    }

    if (mKeyPosY != g_drawHIO.mKeyPosY) {
        mKeyPosY = g_drawHIO.mKeyPosY;
        draw_key = true;
    }

    if (mKeyNumScale != g_drawHIO.mKeyNumScale) {
        mKeyNumScale = g_drawHIO.mKeyNumScale;
        draw_key = true;
    }

    if (mKeyNumPosX != g_drawHIO.mKeyNumPosX) {
        mKeyNumPosX = g_drawHIO.mKeyNumPosX;
        draw_key = true;
    }

    if (mKeyNumPosY != g_drawHIO.mKeyNumPosY) {
        mKeyNumPosY = g_drawHIO.mKeyNumPosY;
        draw_key = true;
    }

    if (draw_key == true) {
        mpMeterDraw->drawKey(mKeyNum);
    }

    alphaAnimeKey();
}

void dMeter2_c::moveButtonA() {
    f32 var_f31;
    f32 pos_x[2];
    f32 pos_y[2];

    bool draw_buttonA;
    bool var_r29;
    bool emphasis_a;
    u8 temp_r0;

    draw_buttonA = false;
    var_r29 = 0;
    emphasis_a = false;

    if (dComIfGp_getDoStatusForce() != 0) {
        dComIfGp_setDoStatus(dComIfGp_getDoStatusForce(), dComIfGp_getDoSetFlagForce());
        dComIfGp_setDoStatusForce(0, 0);
    }

    if (daPy_getPlayerActorClass()->getSumouMode()) {
        if (dComIfGp_getDoStatus() == 0) {
            dComIfGp_setDoStatus(0x15, 0);
            emphasis_a = true;
        } else if (dComIfGp_getDoStatus() == 0x15 && !mpMeterDraw->isEmphasisA()) {
            var_r29 = 1;
            draw_buttonA = true;
        }
    }

    if (mDoStatus != dComIfGp_getDoStatus()) {
        mDoStatus = dComIfGp_getDoStatus();

        if (mDoStatus == 0x2D || mDoStatus == 0x2E) {
            dComIfGp_setDoStatus(mDoStatus, 1);
        }

        var_r29 = 1;
        draw_buttonA = true;
    }

    if (mDoSetFlag != dComIfGp_isDoSetFlag(2)) {
        mDoSetFlag = dComIfGp_isDoSetFlag(2);
        var_r29 = 1;
        draw_buttonA = true;
    }

    if (emphasis_a && mpMeterDraw->isEmphasisA()) {
        if (field_0x1fd == 0) {
            var_r29 = 1;
            draw_buttonA = true;
            field_0x1fd = 1;
        }
    } else if (!emphasis_a && !mpMeterDraw->isEmphasisA() && field_0x1fd == 1) {
        var_r29 = 1;
        draw_buttonA = true;
        field_0x1fd = 0;
    }

    if (mButtonAScale != g_drawHIO.mButtonAScale) {
        mButtonAScale = g_drawHIO.mButtonAScale;
        draw_buttonA = true;
    }

    for (int i = 0; i < 2; i++) {
        if (mButtonATalkScale[i] != g_drawHIO.mButtonATalkScale[i]) {
            mButtonATalkScale[i] = g_drawHIO.mButtonATalkScale[i];
            draw_buttonA = true;
        }
    }

    if (mButtonAPosX != g_drawHIO.mButtonAPosX) {
        mButtonAPosX = g_drawHIO.mButtonAPosX;
        draw_buttonA = true;
    }

    if (mButtonAPosY != g_drawHIO.mButtonAPosY) {
        mButtonAPosY = g_drawHIO.mButtonAPosY;
        draw_buttonA = true;
    }

    if (mButtonATextScale != g_drawHIO.mButtonATextScale) {
        mButtonATextScale = g_drawHIO.mButtonATextScale;
        draw_buttonA = true;
    }

    if (mButtonATextPosX != g_drawHIO.mButtonATextPosX) {
        mButtonATextPosX = g_drawHIO.mButtonATextPosX;
        draw_buttonA = true;
    }

    if (mButtonATextPosY != g_drawHIO.mButtonATextPosY) {
        mButtonATextPosY = g_drawHIO.mButtonATextPosY;
        draw_buttonA = true;
    }

    if (isShowLightDrop()) {
        pos_x[0] = g_drawHIO.mButtonAVesselPosX;
        pos_y[0] = g_drawHIO.mButtonAVesselPosY;
        pos_x[1] = 0.0f;
        pos_y[1] = 0.0f;
        var_f31 = 1.0f;
    } else if (isShowFlag(0)) {
        if (isShowFlag(1)) {
            for (int i = 0; i < 2; i++) {
                pos_x[i] = g_drawHIO.mButtonATalkPosX[i];
                pos_y[i] = g_drawHIO.mButtonATalkPosY[i];
            }
        } else {
            for (int i = 0; i < 2; i++) {
                pos_x[i] = g_drawHIO.mButtonATalkAPosX[i];
                pos_y[i] = g_drawHIO.mButtonATalkAPosY[i];
            }
        }
        var_f31 = 1.0f;
    } else {
        pos_x[0] = g_drawHIO.mButtonAPosX;
        pos_y[0] = g_drawHIO.mButtonAPosY;
        pos_x[1] = 0.0f;
        pos_y[1] = 0.0f;
        var_f31 = 1.0f;
    }

    for (int i = 0; i < 2; i++) {
        if (mButtonATalkPosX[i] != pos_x[i]) {
            cLib_addCalc2(&mButtonATalkPosX[i], pos_x[i], 1.0f, 10.0f);
            draw_buttonA = true;
            if ((f32)fabs(mButtonATalkPosX[i] - pos_x[i]) < 0.1f) {
                mButtonATalkPosX[i] = pos_x[i];
            }
        }

        if (mButtonATalkPosY[i] != pos_y[i]) {
            cLib_addCalc2(&mButtonATalkPosY[i], pos_y[i], 1.0f, 10.0f);
            draw_buttonA = true;
            if ((f32)fabs(mButtonATalkPosY[i] - pos_y[i]) < 0.1f) {
                mButtonATalkPosY[i] = pos_y[i];
            }
        }
    }

    if (field_0x144 != var_f31) {
        cLib_addCalc2(&field_0x144, var_f31, 1.0f, 10.0f);
        draw_buttonA = true;
        if ((f32)fabs(field_0x144 - var_f31) < 0.01f) {
            field_0x144 = var_f31;
        }
    }

    if (field_0x200 != dMsgObject_isTalkNowCheck()) {
        field_0x200 = dMsgObject_isTalkNowCheck();
        draw_buttonA = true;
    }

    if (draw_buttonA) {
        mpMeterDraw->drawButtonA(mDoStatus, mButtonATalkPosX[0], mButtonATalkPosY[0],
                                 mButtonATalkPosX[1], mButtonATalkPosY[1], field_0x144, var_r29,
                                 ((mStatus & 0x100) ||
                                  daPy_getPlayerActorClass()->checkHawkWait() ||
                                  daPy_getPlayerActorClass()->checkGrassWhistle()) != false);

        if (emphasis_a) {
            mpMeterDraw->setEmphasisA(0);
        }
    }
}

void dMeter2_c::moveButtonB() {
    f32 var_f31;
    f32 pos_x[2];
    f32 pos_y[2];

    bool draw_buttonB;
    bool var_r29;
    bool emphasis_b;
    u8 temp_r0;

    draw_buttonB = false;
    var_r29 = 0;
    emphasis_b = false;

    if (dComIfGp_getAStatusForce() != 0) {
        dComIfGp_setAStatus(dComIfGp_getAStatusForce(), dComIfGp_getASetFlagForce());
        dComIfGp_setAStatusForce(0, 0);
    }

    if (daPy_getPlayerActorClass()->getSumouMode()) {
        if (dComIfGp_getAStatus() == 0 || dComIfGp_getAStatus() == 0x26) {
            dComIfGp_setAStatus(0x44, 0);
            emphasis_b = true;
        } else if (dComIfGp_getAStatus() == 0x44 && !mpMeterDraw->isEmphasisB()) {
            var_r29 = 1;
            draw_buttonB = true;
        }
    } else if (daPy_getPlayerActorClass()->checkGoatStopGame() ||
               ((mStatus & 2) && dComIfGp_getAStatus() == 0x26))
    {
        dComIfGp_setAStatus(0, 0);
    }

    if (mAStatus != dComIfGp_getAStatus()) {
        mAStatus = dComIfGp_getAStatus();

        if (mAStatus == 0x2D || mAStatus == 0x2E) {
            dComIfGp_setAStatus(mAStatus, 1);
        }

        var_r29 = 1;
        draw_buttonB = true;
    }

    if (mASetFlag != dComIfGp_isASetFlag(2)) {
        mASetFlag = dComIfGp_isASetFlag(2);
        var_r29 = 1;
        draw_buttonB = true;
    }

    if (mEquipSword != dComIfGs_getSelectEquipSword()) {
        mEquipSword = dComIfGs_getSelectEquipSword();
        var_r29 = 1;
        draw_buttonB = true;
    }

    if (emphasis_b && mpMeterDraw->isEmphasisB()) {
        if (field_0x1fe == 0) {
            var_r29 = 1;
            draw_buttonB = true;
            field_0x1fe = 1;
        }
    } else if (!emphasis_b && !mpMeterDraw->isEmphasisB() && field_0x1fe == 1) {
        var_r29 = 1;
        draw_buttonB = true;
        field_0x1fe = 0;
    }

    if (field_0x2cc != g_drawHIO.mButtonBScale) {
        field_0x2cc = g_drawHIO.mButtonBScale;
        draw_buttonB = true;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x2ec[i] != g_drawHIO.mButtonBTalkScale[i]) {
            field_0x2ec[i] = g_drawHIO.mButtonBTalkScale[i];
            draw_buttonB = true;
        }
    }

    if (mButtonBPosX != g_drawHIO.mButtonBPosX) {
        mButtonBPosX = g_drawHIO.mButtonBPosX;
        draw_buttonB = true;
    }

    if (mButtonBPosY != g_drawHIO.mButtonBPosY) {
        mButtonBPosY = g_drawHIO.mButtonBPosY;
        draw_buttonB = true;
    }

    if (g_drawHIO.mButtonDebug[3]) {
        draw_buttonB = true;
    }

    for (int i = 0; i < 2; i++) {
        if (mItemBBaseScale[i] != g_drawHIO.mItemBBaseScale[i]) {
            mItemBBaseScale[i] = g_drawHIO.mItemBBaseScale[i];
            draw_buttonB = true;
        }

        if (mItemBBasePosX[i] != g_drawHIO.mItemBBasePosX[i]) {
            mItemBBasePosX[i] = g_drawHIO.mItemBBasePosX[i];
            draw_buttonB = true;
        }

        if (mItemBBasePosY[i] != g_drawHIO.mItemBBasePosY[i]) {
            mItemBBasePosY[i] = g_drawHIO.mItemBBasePosY[i];
            draw_buttonB = true;
        }
    }

    if (mButtonBFontScale != g_drawHIO.mButtonBFontScale) {
        mButtonBFontScale = g_drawHIO.mButtonBFontScale;
        draw_buttonB = true;
    }

    if (mButtonBFontPosX != g_drawHIO.mButtonBFontPosX) {
        mButtonBFontPosX = g_drawHIO.mButtonBFontPosX;
        draw_buttonB = true;
    }

    if (mButtonBFontPosY != g_drawHIO.mButtonBFontPosY) {
        mButtonBFontPosY = g_drawHIO.mButtonBFontPosY;
        draw_buttonB = true;
    }

    if (isShowLightDrop()) {
        pos_x[0] = g_drawHIO.mButtonBVesselPosX;
        pos_y[0] = g_drawHIO.mButtonBVesselPosY;
        pos_x[1] = 0.0f;
        pos_y[1] = 0.0f;
        var_f31 = 1.0f;
    } else if (isShowFlag(1)) {
        for (int i = 0; i < 2; i++) {
            pos_x[i] = g_drawHIO.mButtonBTalkPosX[i];
            pos_y[i] = g_drawHIO.mButtonBTalkPosY[i];
        }
        var_f31 = 1.0f;
    } else {
        for (int i = 0; i < 2; i++) {
            if (field_0x128 != 0 && i == 0) {
                pos_x[i] = g_drawHIO.mButtonBWolfPosX;
                pos_y[i] = g_drawHIO.mButtonBWolfPosY;
            } else {
                pos_x[i] = 0.0f;
                pos_y[i] = 0.0f;
            }
        }
        var_f31 = 1.0f;
    }

    if (mpMeterDraw->isBButtonShow(false)) {
        if (field_0x1ff == 0) {
            field_0x1ff = 1;
            draw_buttonB = true;
        }
    } else if (field_0x1ff == 1) {
        field_0x1ff = 0;
        draw_buttonB = true;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x148[i] != pos_x[i]) {
            cLib_addCalc2(&field_0x148[i], pos_x[i], 1.0f, 10.0f);
            draw_buttonB = true;
            if ((f32)fabs(field_0x148[i] - pos_x[i]) < 0.1f) {
                field_0x148[i] = pos_x[i];
            }
        }

        if (field_0x150[i] != pos_y[i]) {
            cLib_addCalc2(&field_0x150[i], pos_y[i], 1.0f, 10.0f);
            draw_buttonB = true;
            if ((f32)fabs(field_0x150[i] - pos_y[i]) < 0.1f) {
                field_0x150[i] = pos_y[i];
            }
        }
    }

    if (field_0x158 != var_f31) {
        cLib_addCalc2(&field_0x158, var_f31, 1.0f, 10.0f);
        draw_buttonB = true;
        if ((f32)fabs(field_0x158 - var_f31) < 0.01f) {
            field_0x158 = var_f31;
        }
    }

    if (g_drawHIO.mItemScaleAdjustON && field_0x4bc != g_drawHIO.mItemScalePercent) {
        field_0x4bc = g_drawHIO.mItemScalePercent;
        draw_buttonB = true;
    }

    if (field_0x1c6 != daPy_getPlayerActorClass()->checkGrassWhistle()) {
        field_0x1c6 = daPy_getPlayerActorClass()->checkGrassWhistle();
        draw_buttonB = true;
    }

    if (draw_buttonB) {
        mpMeterDraw->drawButtonB(mAStatus, field_0x128 == 0, field_0x148[0], field_0x150[0],
                                 field_0x148[1], field_0x150[1], field_0x158, var_r29);

        if (emphasis_b) {
            mpMeterDraw->setEmphasisB(0);
        }
    }
}

void dMeter2_c::moveButtonR() {
    bool var_r7;
    bool draw_buttonR;

    draw_buttonR = false;
    var_r7 = 0;

    if (field_0x128 != 0 &&
        (mCollectSmell != dComIfGs_getCollectSmell() || field_0x128 != field_0x12c))
    {
        mCollectSmell = dComIfGs_getCollectSmell();
        var_r7 = 1;
        draw_buttonR = true;
    }

    if (dComIfGp_getRStatusForce() != 0) {
        dComIfGp_setRStatus(dComIfGp_getRStatusForce(), dComIfGp_getRSetFlagForce());
        dComIfGp_setRStatusForce(0, 0);
    }

    if (mRStatus != dComIfGp_getRStatus()) {
        mRStatus = dComIfGp_getRStatus();

        if (mRStatus == 0x2D || mRStatus == 0x2E) {
            dComIfGp_setRStatus(mRStatus, 1);
        }

        var_r7 = 1;
        draw_buttonR = true;
    }

    if (mRSetFlag != dComIfGp_isRSetFlag(2)) {
        mRSetFlag = dComIfGp_isRSetFlag(2);
        var_r7 = 1;
        draw_buttonR = true;
    }

    if (draw_buttonR) {
        mpMeterDraw->drawButtonR(mCollectSmell, mRStatus, field_0x128 == 0, var_r7);
    }
}

void dMeter2_c::moveButtonZ() {
    bool draw_buttonZ = false;

    if (field_0x324 != g_drawHIO.mButtonZScale) {
        field_0x324 = g_drawHIO.mButtonZScale;
        draw_buttonZ = true;
    }

    if (field_0x328 != g_drawHIO.mButtonZPosX) {
        field_0x328 = g_drawHIO.mButtonZPosX;
        draw_buttonZ = true;
    }

    if (field_0x32c != g_drawHIO.mButtonZPosY) {
        field_0x32c = g_drawHIO.mButtonZPosY;
        draw_buttonZ = true;
    }

    if (field_0x384 != g_drawHIO.mButtonZItemScale) {
        field_0x384 = g_drawHIO.mButtonZItemScale;
        draw_buttonZ = true;
    }

    if (field_0x388 != g_drawHIO.mButtonZItemPosX) {
        field_0x388 = g_drawHIO.mButtonZItemPosX;
        draw_buttonZ = true;
    }

    if (field_0x38c != g_drawHIO.mButtonZItemPosY) {
        field_0x38c = g_drawHIO.mButtonZItemPosY;
        draw_buttonZ = true;
    }

    if (field_0x3e4 != g_drawHIO.mButtonZItemBaseScale) {
        field_0x3e4 = g_drawHIO.mButtonZItemBaseScale;
        draw_buttonZ = true;
    }

    if (field_0x3e8 != g_drawHIO.mButtonZItemBasePosX) {
        field_0x3e8 = g_drawHIO.mButtonZItemBasePosX;
        draw_buttonZ = true;
    }

    if (field_0x3ec != g_drawHIO.mButtonZItemBasePosY) {
        field_0x3ec = g_drawHIO.mButtonZItemBasePosY;
        draw_buttonZ = true;
    }

    if (field_0x330 != g_drawHIO.mButtonZFontScale) {
        field_0x330 = g_drawHIO.mButtonZFontScale;
        draw_buttonZ = true;
    }

    if (field_0x334 != g_drawHIO.mButtonZFontPosX) {
        field_0x334 = g_drawHIO.mButtonZFontPosX;
        draw_buttonZ = true;
    }

    if (field_0x338 != g_drawHIO.mButtonZFontPosY) {
        field_0x338 = g_drawHIO.mButtonZFontPosY;
        draw_buttonZ = true;
    }

    if (dComIfGp_getZStatusForce() != 0) {
        dComIfGp_setZStatus(dComIfGp_getZStatusForce(), dComIfGp_getZSetFlagForce());
        dComIfGp_setZStatusForce(0, 0);
    }

    if (mZStatus != dComIfGp_getZStatus() || draw_buttonZ) {
        mZStatus = dComIfGp_getZStatus();

        if (mZStatus == 0x2D || mZStatus == 0x2E) {
            dComIfGp_setZStatus(mZStatus, 1);
        }

        mpMeterDraw->drawButtonZ(mZStatus);
    }

    mpMeterDraw->setButtonIconMidonaAlpha(mStatus);
    dComIfGp_setZStatus(0, 0);

    if (dComIfGp_getBottleStatusForce() != 0) {
        dComIfGp_setBottleStatus(dComIfGp_getBottleStatusForce(),
                                   dComIfGp_getBottleSetFlagForce());
        dComIfGp_setBottleStatusForce(0, 0);
    }

    if (mBottleStatus != dComIfGp_getBottleStatus()) {
        mBottleStatus = dComIfGp_getBottleStatus();
        mpMeterDraw->drawButtonBin(mBottleStatus);
    }
}

void dMeter2_c::moveButton3D() {
    u8 var_r6;

    if (dComIfGp_get3DStatusForce() != 0) {
        var_r6 = dComIfGp_get3DSetFlagForce();
        if (dComIfGp_get3DStatusForce() == 0x6A) {
            var_r6 = 1;
        }

        dComIfGp_set3DStatus(dComIfGp_get3DStatusForce(), dComIfGp_get3DDirectionForce(), var_r6);
        dComIfGp_set3DStatusForce(0, 0, 0);
    }

    if (m3DStatus != dComIfGp_get3DStatus()) {
        m3DStatus = dComIfGp_get3DStatus();

        if (m3DStatus == 0x2D || m3DStatus == 0x2E) {
            dComIfGp_set3DStatus(m3DStatus, 5, 1);
        }

        mpMeterDraw->drawButton3D(m3DStatus);
    }
}

void dMeter2_c::moveButtonC() {
    if (dComIfGp_getCStickStatusForce()) {
        u8 setFlag = dComIfGp_getCStickSetFlagForce();
        u8 dirForce = dComIfGp_getCStickDirectionForce();
        u8 statusForce = dComIfGp_getCStickStatusForce();
        dComIfGp_setCStickStatus(statusForce, dirForce, setFlag);
        dComIfGp_setCStickStatusForce(0, 0, 0);
    }

    if (mCStickStatus == dComIfGp_getCStickStatus() && field_0x1ac == dMeter2Info_isUseButton(0x10))
    {
        if ((!mpMeterDraw->isEmphasisC() || dComIfGp_isCStickSetFlag(2)) &&
            (mpMeterDraw->isEmphasisC() || !dComIfGp_isCStickSetFlag(2)) &&
            field_0x19a == mpMeterDraw->getButtonTimer())
        {
            return;
        }
    }

    field_0x19a = mpMeterDraw->getButtonTimer();
    mCStickStatus = dComIfGp_getCStickStatus();
    field_0x1ac = dMeter2Info_isUseButton(0x10);
    mpMeterDraw->drawButtonC(mCStickStatus, field_0x128 == 0);
}

void dMeter2_c::moveButtonS() {
    if (dComIfGp_getSButtonStatusForce()) {
        u8 setFlag = dComIfGp_getSButtonSetFlagForce();
        u8 statusForce = dComIfGp_getSButtonStatusForce();
        dComIfGp_setSButtonStatus(statusForce, setFlag);
        dComIfGp_setSButtonStatusForce(0, 0);
    } else {
        dComIfGp_setSButtonStatus(0, 0);
    }

    if (mSButtonStatus != dComIfGp_getSButtonStatus()) {
        mSButtonStatus = dComIfGp_getSButtonStatus();
        mpMeterDraw->drawButtonS(mSButtonStatus);
    }
}

void dMeter2_c::moveButtonXY() {
    bool sp8[2];
    bool spC[2];

    for (int i = 0; i < 2; i++) {
        sp8[i] = 0;
        spC[i] = 0;
    }

    if (field_0x33c != g_drawHIO.mButtonXScale) {
        field_0x33c = g_drawHIO.mButtonXScale;
        sp8[0] = 1;
    }

    if (field_0x340 != g_drawHIO.mButtonXPosX) {
        field_0x340 = g_drawHIO.mButtonXPosX;
        sp8[0] = 1;
    }

    if (field_0x344 != g_drawHIO.mButtonXPosY) {
        field_0x344 = g_drawHIO.mButtonXPosY;
        sp8[0] = 1;
    }

    if (field_0x348 != g_drawHIO.mButtonYScale) {
        field_0x348 = g_drawHIO.mButtonYScale;
        sp8[1] = 1;
    }

    if (field_0x34c != g_drawHIO.mButtonYPosX) {
        field_0x34c = g_drawHIO.mButtonYPosX;
        sp8[1] = 1;
    }

    if (field_0x350 != g_drawHIO.mButtonYPosY) {
        field_0x350 = g_drawHIO.mButtonYPosY;
        sp8[1] = 1;
    }

    if (field_0x360 != g_drawHIO.mButtonXItemScale) {
        field_0x360 = g_drawHIO.mButtonXItemScale;
        sp8[0] = 1;
    }

    if (field_0x364 != g_drawHIO.mButtonXItemPosX) {
        field_0x364 = g_drawHIO.mButtonXItemPosX;
        sp8[0] = 1;
    }

    if (field_0x368 != g_drawHIO.mButtonXItemPosY) {
        field_0x368 = g_drawHIO.mButtonXItemPosY;
        sp8[0] = 1;
    }

    if (field_0x36c != g_drawHIO.mButtonYItemScale) {
        field_0x36c = g_drawHIO.mButtonYItemScale;
        sp8[1] = 1;
    }

    if (field_0x370 != g_drawHIO.mButtonYItemPosX) {
        field_0x370 = g_drawHIO.mButtonYItemPosX;
        sp8[1] = 1;
    }

    if (field_0x374 != g_drawHIO.mButtonYItemPosY) {
        field_0x374 = g_drawHIO.mButtonYItemPosY;
        sp8[1] = 1;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x39c[i] != g_drawHIO.mButtonXItemBaseScale[i]) {
            field_0x39c[i] = g_drawHIO.mButtonXItemBaseScale[i];
            sp8[0] = 1;
        }

        if (field_0x3a4[i] != g_drawHIO.mButtonXItemBasePosX[i]) {
            field_0x3a4[i] = g_drawHIO.mButtonXItemBasePosX[i];
            sp8[0] = 1;
        }

        if (field_0x3ac[i] != g_drawHIO.mButtonXItemBasePosY[i]) {
            field_0x3ac[i] = g_drawHIO.mButtonXItemBasePosY[i];
            sp8[0] = 1;
        }

        if (field_0x3b4[i] != g_drawHIO.mButtonYItemBaseScale[i]) {
            field_0x3b4[i] = g_drawHIO.mButtonYItemBaseScale[i];
            sp8[1] = 1;
        }

        if (field_0x3bc[i] != g_drawHIO.mButtonYItemBasePosX[i]) {
            field_0x3bc[i] = g_drawHIO.mButtonYItemBasePosX[i];
            sp8[1] = 1;
        }

        if (field_0x3c4[i] != g_drawHIO.mButtonYItemBasePosY[i]) {
            field_0x3c4[i] = g_drawHIO.mButtonYItemBasePosY[i];
            sp8[1] = 1;
        }
    }

    if (g_drawHIO.mItemScaleAdjustON && field_0x4bd != g_drawHIO.mItemScalePercent) {
        field_0x4bd = g_drawHIO.mItemScalePercent;

        for (int i = 0; i < 2; i++) {
            sp8[i] = 1;
        }
    }

    if (field_0x3f0 != g_drawHIO.mButtonXYTextScale) {
        field_0x3f0 = g_drawHIO.mButtonXYTextScale;

        for (int i = 0; i < 2; i++) {
            sp8[i] = 1;
        }
    }

    if (field_0x3f4 != g_drawHIO.mButtonXYTextPosX) {
        field_0x3f4 = g_drawHIO.mButtonXYTextPosX;

        for (int i = 0; i < 2; i++) {
            sp8[i] = 1;
        }
    }

    if (field_0x3f8 != g_drawHIO.mButtonXYTextPosY) {
        field_0x3f8 = g_drawHIO.mButtonXYTextPosY;

        for (int i = 0; i < 2; i++) {
            sp8[i] = 1;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x390[i] != g_drawHIO.mButtonItemRotation[i]) {
            field_0x390[i] = g_drawHIO.mButtonItemRotation[i];
            sp8[i] = 1;
        }

        if (g_drawHIO.mButtonDebug[i]) {
            sp8[i] = 1;
        }

        if (field_0x430 != g_drawHIO.field_0x54c) {
            field_0x430 = g_drawHIO.field_0x54c;
            sp8[i] = 1;
        }
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x128 != 0) {
            if (field_0x128 != field_0x12c) {
                spC[i] = 1;
                sp8[i] = 1;
            }

            if (i == 0) {
                if (dComIfGp_getXStatusForce() != 0) {
                    dComIfGp_setXStatus(dComIfGp_getXStatusForce(), dComIfGp_getXSetFlagForce());
                    dComIfGp_setXStatusForce(0, 0);
                }

                if (mItemStatus[i * 2 + 1] != dComIfGp_getXStatus()) {
                    mItemStatus[i * 2 + 1] = dComIfGp_getXStatus();

                    if (mItemStatus[i * 2 + 1] == 0x2D || mItemStatus[i * 2 + 1] == 0x2E) {
                        dComIfGp_setXStatus(mItemStatus[i * 2 + 1], 1);
                    }

                    spC[i] = 1;
                    sp8[i] = 1;
                }

                if ((&mXSetFlag)[i] != dComIfGp_isXSetFlag(2)) {
                    (&mXSetFlag)[i] = dComIfGp_isXSetFlag(2);
                    spC[i] = 1;
                    sp8[i] = 1;
                }
            } else if (i == 1) {
                if (dComIfGp_getYStatusForce() != 0) {
                    dComIfGp_setYStatus(dComIfGp_getYStatusForce(), dComIfGp_getYSetFlagForce());
                    dComIfGp_setYStatusForce(0, 0);
                }

                if (mItemStatus[i * 2 + 1] != dComIfGp_getYStatus()) {
                    mItemStatus[i * 2 + 1] = dComIfGp_getYStatus();

                    if (mItemStatus[i * 2 + 1] == 0x2D || mItemStatus[i * 2 + 1] == 0x2E) {
                        dComIfGp_setYStatus(mItemStatus[i * 2 + 1], 1);
                    }

                    spC[i] = 1;
                    sp8[i] = 1;
                }

                if ((&mXSetFlag)[i] != dComIfGp_isYSetFlag(2)) {
                    (&mXSetFlag)[i] = dComIfGp_isYSetFlag(2);
                    spC[i] = 1;
                    sp8[i] = 1;
                }
            }
        } else {
            if (mItemStatus[2 * i] != dComIfGp_getSelectItem(i) || field_0x128 != field_0x12c) {
                mItemStatus[2 * i] = dComIfGp_getSelectItem(i);
                spC[i] = 1;
                sp8[i] = 1;
            }

            if (field_0x1d6[i] != dMeter2Info_isDirectUseItem(i)) {
                field_0x1d6[i] = dMeter2Info_isDirectUseItem(i);
                spC[i] = 1;
                sp8[i] = 1;
            }

            if (mItemStatus[2 * i] == 0x48) {
                if (field_0x1ec == 0) {
                    if (dComIfGs_getOil() != 0) {
                        field_0x1ec = 1;
                        sp8[i] = 1;
                    }
                } else {
                    if (dComIfGs_getOil() == 0) {
                        field_0x1ec = 0;
                        sp8[i] = 1;
                    }
                }
            } else if (mItemStatus[2 * i] == 0x46) {
                if (field_0x1ed == 0) {
                    if (daPy_getPlayerActorClass()->checkCopyRodTopUse()) {
                        field_0x1ed = 1;
                        sp8[i] = 1;
                    }
                } else {
                    if (!daPy_getPlayerActorClass()->checkCopyRodTopUse()) {
                        field_0x1ed = 0;
                        sp8[i] = 1;
                    }
                }
            } else if (mItemStatus[2 * i] == 0x50 || mItemStatus[2 * i] == 0x70 ||
                       mItemStatus[2 * i] == 0x71 || mItemStatus[2 * i] == 0x72)
            {
                if (sp8[i] != 0) {
                    mpMeterDraw->setItemNum(i, dComIfGp_getSelectItemNum(i),
                                            dComIfGp_getSelectItemMaxNum(i));
                }
            } else if (mItemStatus[2 * i] == 0x76) {
                if (field_0x1d8[i] != dComIfGp_getSelectItemNum(i)) {
                    field_0x1d8[i] = dComIfGp_getSelectItemNum(i);
                    sp8[i] = 1;
                }

                if (sp8[i] != 0) {
                    mpMeterDraw->setItemNum(i, dComIfGp_getSelectItemNum(i),
                                            dComIfGp_getSelectItemMaxNum(i));
                }
            } else if (mItemStatus[2 * i] == 0x43 || mItemStatus[2 * i] == 0x53 ||
                       mItemStatus[2 * i] == 0x54 || mItemStatus[2 * i] == 0x55 ||
                       mItemStatus[2 * i] == 0x56 || mItemStatus[2 * i] == 0x5A)
            {
                if (sp8[i] != 0) {
                    mpMeterDraw->setItemNum(i, mArrowNum, dComIfGs_getArrowMax());
                }
            } else if (mItemStatus[2 * i] == 0x4B) {
                if (sp8[i] != 0) {
                    mpMeterDraw->setItemNum(i, mPachinkoNum, dComIfGs_getPachinkoMax());
                }
            } else if (mItemStatus[2 * i] == 0x59) {
                u8 var_r25 = dComIfGp_getSelectItemNum(i);
                u8 var_r6_2 = dComIfGp_getSelectItemMaxNum(i);

                if (var_r25 > mArrowNum) {
                    var_r25 = mArrowNum;
                }

                if (var_r6_2 < dComIfGs_getArrowMax()) {
                    var_r6_2 = dComIfGs_getArrowMax();
                }

                mpMeterDraw->setItemNum(i, var_r25, var_r6_2);
            }
        }

        if (sp8[i] != 0) {
            mpMeterDraw->drawButtonXY(i, mItemStatus[2 * i], mItemStatus[i * 2 + 1],
                                      field_0x128 == 0 ? true : false, spC[i]);
        }
    }

    alphaAnimeButton();
    dMeter2Info_resetDirectUseItem();
}

void dMeter2_c::moveButtonCross() {
    f32 temp_f1;
    f32 temp_f30;
    f32 temp_f31;
    f32 var_f31;
    bool draw_cross;

    draw_cross = false;
    var_f31 = 0.0f;

    if (mButtonCrossOFFPosX != g_drawHIO.mButtonCrossOFFPosX) {
        mButtonCrossOFFPosX = g_drawHIO.mButtonCrossOFFPosX;
        draw_cross = true;
    }

    if (mButtonCrossOFFPosY != g_drawHIO.mButtonCrossOFFPosY) {
        mButtonCrossOFFPosY = g_drawHIO.mButtonCrossOFFPosY;
        draw_cross = true;
    }

    if (mButtonCrossONPosX != g_drawHIO.mButtonCrossONPosX) {
        mButtonCrossONPosX = g_drawHIO.mButtonCrossONPosX;
        draw_cross = true;
    }

    if (mButtonCrossONPosY != g_drawHIO.mButtonCrossONPosY) {
        mButtonCrossONPosY = g_drawHIO.mButtonCrossONPosY;
        draw_cross = true;
    }

    if (mButtonCrossScale != g_drawHIO.mButtonCrossScale) {
        mButtonCrossScale = g_drawHIO.mButtonCrossScale;
        draw_cross = true;
    }

    if (mButtonCrossTextScale != g_drawHIO.mButtonCrossTextScale) {
        mButtonCrossTextScale = g_drawHIO.mButtonCrossTextScale;
        draw_cross = true;
    }

    if (mpMap != NULL) {
        temp_f31 = mpMap->getMapDispEdgeTop();
        temp_f1 = (temp_f31 - mpMeterDraw->getButtonCrossParentInitTransY()) - 15.0f;

        if (mpMap->isDispPosInsideFlg()) {
            if (field_0x1b4 < g_drawHIO.mButtonCrossMoveFrame) {
                field_0x1b4++;
                draw_cross = true;
            } else {
                field_0x1b4 = g_drawHIO.mButtonCrossMoveFrame;
            }

            var_f31 = mButtonCrossONPosY + temp_f1;
        } else {
            if (field_0x1b4 > 0) {
                field_0x1b4--;
                draw_cross = true;
            } else {
                field_0x1b4 = 0;
            }

            var_f31 = mButtonCrossOFFPosY;
        }
    }

    temp_f30 = mButtonCrossOFFPosX + (((f32)field_0x1b4 / (f32)g_drawHIO.mButtonCrossMoveFrame) *
                                     (mButtonCrossONPosX - mButtonCrossOFFPosX));
    if (field_0x15c != var_f31) {
        cLib_addCalc2(&field_0x15c, var_f31, 0.5f, 50.0f);
        if ((f32)fabs(field_0x15c - var_f31) < 0.5f) {
            field_0x15c = var_f31;
        }
        draw_cross = true;
    }

    if (draw_cross == true) {
        mpMeterDraw->drawButtonCross(temp_f30, field_0x15c);
    }

    alphaAnimeButtonCross();
}

void dMeter2_c::moveTouchSubMenu() {}

void dMeter2_c::moveSubContents() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    checkSubContents();

    if (mpSubContents != NULL) {
        mpSubContents->_execute(mStatus);
    }

    if (mpSubSubContents != NULL) {
        if (mSubContentType == 5 && mSubContentsStringType != dMeter2Info_getMeterStringType()) {
            mSubContentsStringType = dMeter2Info_getMeterStringType();
            if (mSubContentsStringType != 0) {
                mpSubSubContents->createString(mSubContentsStringType);
            }
        }
        mpSubSubContents->_execute(mStatus);
    }

    mDoExt_setCurrentHeap(heap);
}

void dMeter2_c::move2DContents() {
    bool var_r19;
    bool var_r27;
    bool var_r20;
    bool var_r28;
    bool var_r21;
    bool var_r22;
    bool var_r23;
    bool var_r24;
    bool var_r25;
    bool var_r29;
    bool var_r26;
    u8 var_r30;

    field_0x201 = 0;
    field_0x108 = NULL;

    JKRHeap* temp_r3 = dComIfGp_getSubHeap2D(8);
    if (temp_r3 != NULL) {
        field_0x108 = mDoExt_setCurrentHeap(temp_r3);
    }

    check2DContents();

    if (mpEmpButton != NULL) {
        var_r19 = 0;
        var_r27 = 0;
        var_r20 = 0;
        var_r28 = 0;
        var_r21 = 0;
        var_r22 = 0;
        var_r23 = 0;
        var_r24 = 0;
        var_r25 = 0;
        var_r29 = 0;
        var_r26 = 0;
        var_r30 = 2;

        if (mpMeterDraw->isEmphasisS() && mpEmpButton->isSetButton(6)) {
            mpEmpButton->setString(mpMeterDraw->getActionString(mSButtonStatus, 0, NULL), 6, 0, 0);
            var_r23 = 1;
            var_r30 = 0;
        }

        if (mpMeterDraw->isEmphasisB() && mpMeterDraw->isEmphasis3D() && var_r30 != 0) {
            if (mAStatus == m3DStatus) {
                mpEmpButton->setString(mpMeterDraw->getActionString(mAStatus, 0, NULL), 13,
                                       2 - var_r30, 0);
                var_r29 = 1;
                var_r30 -= 2;
            }
        }

        if (var_r29 == 0) {
            if (mpMeterDraw->isEmphasis3D() && var_r30 != 0 && mpEmpButton->isSetButton(4)) {
                mpEmpButton->setString(mpMeterDraw->getActionString(m3DStatus, 0, NULL), 4,
                                       2 - var_r30, 0);
                var_r21 = 1;
                var_r30 -= 1;
            }
        }

        if (mpMeterDraw->isEmphasisC() && var_r30 != 0 && mpEmpButton->isSetButton(5)) {
            mpEmpButton->setString(mpMeterDraw->getActionString(mCStickStatus, 0, NULL), 5,
                                   2 - var_r30, 0);
            var_r22 = 1;
            var_r30 -= 1;
        }

        if (var_r29 == 0) {
            if (mpMeterDraw->isEmphasisB() && var_r30 != 0 && mpEmpButton->isSetButton(1)) {
                mpEmpButton->setString(mpMeterDraw->getActionString(mAStatus, 0, NULL), 1,
                                       2 - var_r30, mAStatus == 0x4F ? true : false);
                var_r27 = 1;
                var_r30 -= 1;
            }
        }

        if (mpMeterDraw->isEmphasisY() && var_r30 != 0 && mpEmpButton->isSetButton(8)) {
            mpEmpButton->setString(mpMeterDraw->getActionString(mItemStatus[3], 0, NULL), 8,
                                   2 - var_r30, 0);
            var_r25 = 1;
            var_r30 -= 1;
        }

        if (mpMeterDraw->isEmphasisA() && var_r30 != 0 && mpEmpButton->isSetButton(0)) {
            mpEmpButton->setString(mpMeterDraw->getActionString(mDoStatus, 0, NULL), 0, 2 - var_r30,
                                   0);
            var_r19 = 1;
            var_r30 -= 1;
        }

        if (mpMeterDraw->isEmphasisZ() && var_r30 != 0 && mpEmpButton->isSetButton(3)) {
            if (mZStatus == 8) {
                mpEmpButton->setString(mpMeterDraw->getActionString(100, 0, NULL), 3, 2 - var_r30,
                                       0);
            } else {
                mpEmpButton->setString(mpMeterDraw->getActionString(mZStatus, 0, NULL), 3,
                                       2 - var_r30, 0);
            }

            var_r28 = 1;
            var_r30 -= 1;
        }

        if (mpMeterDraw->isEmphasisR() && var_r30 != 0 && mpEmpButton->isSetButton(2)) {
            mpEmpButton->setString(mpMeterDraw->getActionString(mRStatus, 0, NULL), 2, 2 - var_r30,
                                   0);
            var_r20 = 1;
            var_r30 -= 1;
        }

        if (mpMeterDraw->isEmphasisX() && var_r30 != 0 && mpEmpButton->isSetButton(7)) {
            mpEmpButton->setString(mpMeterDraw->getActionString(mItemStatus[1], 0, NULL), 7,
                                   2 - var_r30, 0);
            var_r24 = 1;
            var_r30 -= 1;
        }

        if (mpMeterDraw->isEmphasisBin() && var_r30 != 0 && mpEmpButton->isSetButton(21) &&
            !dMeter2Info_is2DActiveTouchArea())
        {
            mpEmpButton->setString(mpMeterDraw->getActionString(mBottleStatus, 0, NULL), 21,
                                   2 - var_r30, 0);
            var_r26 = 1;
        }

        mpEmpButton->_execute(mStatus, var_r19, var_r27, var_r20, var_r28, var_r21, var_r22,
                              var_r23, var_r24, var_r25, false, false, false, false, var_r29, false,
                              false, false, false, false, false, false, var_r26);

        if ((var_r19 != 0) || (var_r27 != 0) || (var_r20 != 0) || (var_r28 != 0) ||
            (var_r21 != 0) || (var_r22 != 0) || (var_r23 != 0) || (var_r24 != 0) ||
            (var_r25 != 0) || (var_r29 != 0) || (var_r26 != 0))
        {
            field_0x201 = 1;
        }
    }

    if (field_0x108 != NULL) {
        mDoExt_setCurrentHeap(field_0x108);
    }
}

void dMeter2_c::checkSubContents() {
    if (mStatus & 0x80) {
        killSubContents(4);

        if (mSubContentType == 0) {
            mpSubContents = new dScope_c(0);
            mSubContentType = 4;
        }
        return;
    } else if (mStatus & 0x2000000) {
        if (strcmp(dComIfGp_getStartStageName(), "F_SP103") &&
            (strcmp(dComIfGp_getStartStageName(), "F_SP00") || dComIfG_play_c::getLayerNo(0) != 5))
        {
            killSubContents(1);

            if (mSubContentType == 0) {
                mpSubHeap->getTotalFreeSize();
                mpSubContents = new dMeterHakusha_c(mpMeterDraw->getMainScreenPtr());
                mSubContentType = 1;
            }
            return;
        }
    }

    if (daPy_getPlayerActorClass()->getSumouMode() != 0) {
        killSubContents(5);

        if (mSubContentType == 0) {
            mpSubContents = new dMeterHaihai_c(0);
            mpSubSubContents = new dMeterString_c(dMeter2Info_getMeterStringType());
            mSubContentType = 5;
            mSubContentsStringType = dMeter2Info_getMeterStringType();
        }
    } else if (mStatus & 0x200) {
        killSubContents(2);

        if (mSubContentType == 0) {
            mpSubContents = new dMeterHaihai_c(0);
            mSubContentType = 2;
        }
    } else if (dMeter2Info_getMeterStringType() != 0) {
        killSubContents(3);

        if (mSubContentType == 0) {
            mpSubContents = new dMeterString_c(dMeter2Info_getMeterStringType());
            mSubContentType = 3;
        }
    } else if (mSubContentType == 4) {
        if (mpSubContents != NULL || mpSubSubContents != NULL) {
            bool free_heap = false;

            if (mpSubContents != NULL && mpSubContents->isDead()) {
                delete mpSubContents;
                mpSubContents = NULL;
                free_heap = true;
            }

            if (mpSubSubContents != NULL && mpSubSubContents->isDead()) {
                delete mpSubSubContents;
                mpSubSubContents = NULL;
                free_heap = true;
            }

            if (free_heap) {
                mpSubHeap->freeAll();
                mSubContentType = 0;
                mSubContentsStringType = 0;
            }
        } else {
            mSubContentType = 0;
        }
    } else if (mSubContentType != 0) {
        bool free_heap = false;

        if (mpSubContents != NULL) {
            delete mpSubContents;
            mpSubContents = NULL;
            free_heap = true;
        }

        if (mpSubSubContents != NULL) {
            delete mpSubSubContents;
            mpSubSubContents = NULL;
            free_heap = true;
        }

        if (free_heap) {
            mpSubHeap->freeAll();
        }

        mSubContentType = 0;
        mSubContentsStringType = 0;
    }
}

void dMeter2_c::check2DContents() {
    if (mpEmpButton == NULL) {
        if ((dComIfGp_isHeapLockFlag() == 0 || dComIfGp_isHeapLockFlag() == 5) &&
            (dMeter2Info_isFloatingMessageVisible() || mpMeterDraw->isEmphasisA() ||
             mpMeterDraw->isEmphasisB() || mpMeterDraw->isEmphasisR() ||
             mpMeterDraw->isEmphasisZ() || mpMeterDraw->isEmphasis3D() ||
             mpMeterDraw->isEmphasisC() || mpMeterDraw->isEmphasisS() ||
             mpMeterDraw->isEmphasisX() || mpMeterDraw->isEmphasisY() ||
             mpMeterDraw->isEmphasisBin()))
        {
            dComIfGp_setHeapLockFlag(8);

            if (field_0x108 == NULL) {
                field_0x108 = mDoExt_setCurrentHeap(dComIfGp_getSubHeap2D(8));
            }

            mpEmpButton = new dMeterButton_c();
        }
    } else if (dComIfGp_isHeapLockFlag() == 5 && !dMeter2Info_isFloatingMessageVisible()) {
        if (!mpMeterDraw->isEmphasisA() && !mpMeterDraw->isEmphasisB() &&
            !mpMeterDraw->isEmphasisR() && !mpMeterDraw->isEmphasisZ() &&
            !mpMeterDraw->isEmphasis3D() && !mpMeterDraw->isEmphasisC() &&
            !mpMeterDraw->isEmphasisS() && !mpMeterDraw->isEmphasisX() &&
            !mpMeterDraw->isEmphasisY() && !mpMeterDraw->isEmphasisBin() && mpEmpButton->isClose())
        {
            delete mpEmpButton;
            mpEmpButton = NULL;
            dComIfGp_getSubHeap2D(8)->freeAll();

            if (field_0x108 != NULL) {
                mDoExt_setCurrentHeap(field_0x108);
                field_0x108 = NULL;
            }

            dComIfGp_offHeapLockFlag(8);
        }
    }
}

void dMeter2_c::moveBombNum() {
    u8 temp_r28;
    u8 temp_r31;

    for (int i = 0; i < 3; i++) {
        s16 var_r22;
        temp_r31 = dComIfGs_getItem((u8)(i + SLOT_15), true);
        temp_r28 = dComIfGs_getItem((u8)(i + SLOT_15), false);

        if (temp_r31 != fpcNm_ITEM_NONE && temp_r31 != fpcNm_ITEM_BOMB_BAG_LV1) {
            if (g_mwHIO.getBombFlag() ||
                (dMeter2Info_getMiniGameItemSetFlag() == 1 && i == dMeter2Info_getRentalBombBag()))
            {
                if (dComIfGs_getBombMax(temp_r28) != dComIfGs_getBombNum(i)) {
                    dComIfGp_setItemBombNumCount(i, dComIfGs_getBombMax(temp_r28));
                }

                if (dComIfGp_getItemBombNumCount(i) < 0) {
                    dComIfGp_clearItemBombNumCount(i);
                }
            }

            if (dComIfGp_getItemBombNumCount(i) != 0 || mBombNum[i] != dComIfGs_getBombNum(i) ||
                mBombMax[i] != dComIfGs_getBombMax(temp_r28))
            {
                var_r22 = dComIfGs_getBombNum(i) + dComIfGp_getItemBombNumCount(i);
                dComIfGp_clearItemBombNumCount(i);

                if (var_r22 < 0) {
                    var_r22 = 0;
                }

                if (var_r22 > dComIfGs_getBombMax(temp_r28)) {
                    var_r22 = dComIfGs_getBombMax(temp_r28);
                }

                if (var_r22 == 0) {
                    if (temp_r31 == fpcNm_ITEM_BOMB_ARROW) {
                        for (int j = 0; j < 2; j++) {
                            if (i + SLOT_15 == dComIfGs_getSelectItemIndex(j) ||
                                i + SLOT_15 == dComIfGs_getMixItemIndex(j))
                            {
                                dComIfGs_setMixItemIndex(j, 0xFF);
                                dComIfGs_setSelectItemIndex(j, 4);
                                dComIfGp_setSelectItem(j);
                            }
                        }
                    }
                    dComIfGs_setItem(i + SLOT_15, fpcNm_ITEM_BOMB_BAG_LV1);
                    dComIfGp_setItem(i + SLOT_15, fpcNm_ITEM_BOMB_BAG_LV1);

                    for (int j = 0; j < 2; j++) {
                        if (i + SLOT_15 == dComIfGs_getSelectMixItemNoArrowIndex(j)) {
                            dComIfGp_setSelectItem(j);
                        }
                    }
                }

                dComIfGs_setBombNum(i, var_r22);
                mBombMax[i] = dComIfGs_getBombMax(temp_r28);

                if (temp_r31 != fpcNm_ITEM_BOMB_ARROW) {
                    for (int j = 0; j < 2; j++) {
                        if (i + SLOT_15 == dComIfGs_getSelectMixItemNoArrowIndex(j)) {
                            mpMeterDraw->setItemNum(j, dComIfGp_getSelectItemNum(j),
                                                    dComIfGp_getSelectItemMaxNum(j));
                        }
                    }
                }
            }
        } else {
            if (temp_r31 != fpcNm_ITEM_BOMB_BAG_LV1) {
                dComIfGs_setBombNum(i, 0);
            }

            if (dComIfGp_getItemBombNumCount(i) != 0) {
                dComIfGp_clearItemBombNumCount(i);
            }
        }

        if (mBombNum[i] != dComIfGs_getBombNum(i)) {
            mBombNum[i] = dComIfGs_getBombNum(i);
        }
    }

    for (int i = 0; i < 2; i++) {
        if (mItemMaxNum[i] != dComIfGs_getSelectItemIndex(i)) {
            for (int j = 0; j < 3; j++) {
                if (j + SLOT_15 == dComIfGs_getSelectItemIndex(i)) {
                    mpMeterDraw->setItemNum(i, dComIfGp_getSelectItemNum(i),
                                            dComIfGp_getSelectItemMaxNum(i));
                }
            }

            mItemMaxNum[i] = dComIfGs_getSelectItemIndex(i);
        }
    }
}

void dMeter2_c::moveBottleNum() {
    for (int i = 0; i < 4; i++) {
        if (dComIfGs_getItem((u8)(i + SLOT_11), true) == fpcNm_ITEM_BEE_CHILD) {
            if (mBottleNum[i] != dComIfGs_getBottleNum(i)) {
                for (int j = 0; j < 2; j++) {
                    if (i + SLOT_11 == dComIfGs_getSelectItemIndex(j)) {
                        mpMeterDraw->setItemNum(j, dComIfGp_getSelectItemNum(j),
                                                dComIfGp_getSelectItemMaxNum(j));
                        mBottleNum[i] = dComIfGs_getBottleNum(i);
                    }
                }
            }
        }
    }
}

void dMeter2_c::moveArrowNum() {
    s16 var_r6;
    int var_r28;
    int i;
    u8 var_r27;
    u8 var_r6_2;

    if (g_mwHIO.getArrowFlag() || dMeter2Info_getMiniGameItemSetFlag() == 1) {
        if (dComIfGs_getArrowMax() != dComIfGs_getArrowNum()) {
            dComIfGp_setItemArrowNumCount(dComIfGs_getArrowMax());
        }

        if (dComIfGp_getItemArrowNumCount() < 0) {
            dComIfGp_clearItemArrowNumCount();
        }
    }

    if (dComIfGp_getItemArrowNumCount() != 0 || mItemMaxNum[2] != dComIfGs_getArrowMax() ||
        mArrowNum != dComIfGs_getArrowNum())
    {
        var_r6 = dComIfGs_getArrowNum() + dComIfGp_getItemArrowNumCount();
        dComIfGp_clearItemArrowNumCount();

        if (var_r6 < 0) {
            var_r6 = 0;
        }

        if (var_r6 > dComIfGs_getArrowMax()) {
            var_r6 = dComIfGs_getArrowMax();
        }

        dComIfGs_setArrowNum(var_r6);
        mItemMaxNum[2] = dComIfGs_getArrowMax();

        if (mArrowNum < dComIfGs_getArrowNum()) {
            mArrowNum++;
            onArrowSoundBit(2);

            if (isArrowSoundBit(2)) {
                if (mArrowNum != dComIfGs_getArrowNum()) {
                    if (!isArrowSoundBit(0) && isArrowEquip() && mpMeterDraw->isButtonVisible()) {
                        onArrowSoundBit(0);
                        mDoAud_seStart(Z2SE_CONSUM_INC_CNT_1, NULL, 0, 0);
                    } else {
                        offArrowSoundBit(0);
                    }
                } else {
                    if (isArrowEquip() && mpMeterDraw->isButtonVisible()) {
                        mDoAud_seStart(Z2SE_CONSUM_INC_CNT_2, NULL, 0, 0);
                    }
                    offArrowSoundBit(2);
                    offArrowSoundBit(0);
                }
            }
        } else if (mArrowNum > dComIfGs_getArrowNum()) {
            mArrowNum--;
        }

        i = 0;
        var_r28 = 0;
        for (; i < 2; i++, var_r28 += 2) {
            if (mItemStatus[var_r28] == fpcNm_ITEM_BOW || mItemStatus[var_r28] == fpcNm_ITEM_LIGHT_ARROW ||
                mItemStatus[var_r28] == fpcNm_ITEM_ARROW_LV1 || mItemStatus[var_r28] == fpcNm_ITEM_ARROW_LV2 ||
                mItemStatus[var_r28] == fpcNm_ITEM_ARROW_LV3 || mItemStatus[var_r28] == fpcNm_ITEM_HAWK_ARROW)
            {
                mpMeterDraw->setItemNum(i, mArrowNum, dComIfGs_getArrowMax());
            } else if (mItemStatus[var_r28] == fpcNm_ITEM_PACHINKO) {
                mpMeterDraw->setItemNum(i, mPachinkoNum, dComIfGs_getPachinkoMax());
            } else if (mItemStatus[var_r28] == fpcNm_ITEM_BOMB_ARROW) {
                var_r27 = dComIfGp_getSelectItemNum(i);
                var_r6_2 = dComIfGp_getSelectItemMaxNum(i);

                if (var_r27 > mArrowNum) {
                    var_r27 = mArrowNum;
                }

                if (var_r6_2 < dComIfGs_getArrowMax()) {
                    var_r6_2 = dComIfGs_getArrowMax();
                }

                mpMeterDraw->setItemNum(i, var_r27, var_r6_2);
            }
        }
    }
}

void dMeter2_c::movePachinkoNum() {
    if (g_mwHIO.getPachinkoFlag()) {
        if (dComIfGs_getPachinkoNum() != dComIfGs_getPachinkoMax()) {
            u8 max = dComIfGs_getPachinkoMax();
            dComIfGp_setItemPachinkoNumCount(max);
        }

        if (dComIfGp_getItemPachinkoNumCount() < 0) {
            dComIfGp_clearItemPachinkoNumCount();
        }
    }

    if (dComIfGp_getItemPachinkoNumCount() == 0 && mItemMaxNum[3] == dComIfGs_getPachinkoMax() &&
        mPachinkoNum == dComIfGs_getPachinkoNum())
    {
        return;
    }

    s16 pachinko_num = dComIfGs_getPachinkoNum() + dComIfGp_getItemPachinkoNumCount();
    dComIfGp_clearItemPachinkoNumCount();
    if (pachinko_num < 0) {
        pachinko_num = 0;
    }

    if (dComIfGs_getPachinkoMax() < pachinko_num) {
        pachinko_num = dComIfGs_getPachinkoMax();
    }

    dComIfGs_setPachinkoNum(pachinko_num);
    mItemMaxNum[3] = dComIfGs_getPachinkoMax();

    if (mPachinkoNum < dComIfGs_getPachinkoNum()) {
        mPachinkoNum++;
        onArrowSoundBit(2);

        if (isArrowSoundBit(2)) {
            if (mPachinkoNum != dComIfGs_getPachinkoNum()) {
                if (!isArrowSoundBit(0) && isPachinkoEquip() && mpMeterDraw->isButtonVisible()) {
                    onArrowSoundBit(0);
                    mDoAud_seStart(Z2SE_CONSUM_INC_CNT_1, 0, 0, 0);
                } else {
                    offArrowSoundBit(0);
                }
            } else {
                if (isPachinkoEquip() && mpMeterDraw->isButtonVisible()) {
                    mDoAud_seStart(Z2SE_CONSUM_INC_CNT_2, 0, 0, 0);
                }
                offArrowSoundBit(2);
                offArrowSoundBit(0);
            }
        }
    } else if (mPachinkoNum > dComIfGs_getPachinkoNum()) {
        mPachinkoNum--;
    }

    for (int i = 0; i < 2; i++) {
        if (mItemStatus[i * 2] == fpcNm_ITEM_PACHINKO) {
            mpMeterDraw->setItemNum(i, mPachinkoNum, dComIfGs_getPachinkoMax());
        }
    }
}

void dMeter2_c::alphaAnimeLife() {
    if ((mStatus & 0x4000) ||
        ((mStatus & 0x40) && dComIfGp_event_checkHind(0x10) &&
         !dComIfGp_checkPlayerStatus1(0, 0x2000)) ||
        ((daPy_getPlayerActorClass()->getSumouMode() != 0) || (mStatus & 0x100000) ||
         (mStatus & 0x80000000) || (mStatus & 8) || (mStatus & 0x10) || (mStatus & 0x01000000) ||
         (mStatus & 0x20) || (mStatus & 0x04000000) || (mStatus & 0x08000000) ||
         (mStatus & 0x10000000)))
    {
        mpMeterDraw->setAlphaLifeAnimeMin();
    } else {
        mpMeterDraw->setAlphaLifeAnimeMax();
        mDoAud_heartGaugeOn();
    }

    mpMeterDraw->setAlphaLifeChange(false);
}

void dMeter2_c::alphaAnimeKantera() {
    if (dComIfGs_getMaxOil() == 0 || dComIfGs_getItem(SLOT_1, true) != fpcNm_ITEM_KANTERA ||
        !daPy_getPlayerActorClass()->checkUseKandelaar(0) || (mStatus & 0x4000) ||
        ((mStatus & 0x40) && dComIfGp_event_checkHind(0x400)) || dComIfGp_getOxygenShowFlag() ||
        ((daPy_getPlayerActorClass()->getSumouMode() != 0) ||
         (daPy_getPlayerActorClass()->checkCanoeSlider() &&
          (dComIfG_getTimerMode() == 3 || dComIfG_getTimerMode() == 4)) ||
         (mStatus & 0x100000) || (mStatus & 0x80000000) || (mStatus & 8) || (mStatus & 0x10) ||
         (mStatus & 0x01000000) || (mStatus & 0x20) || (mStatus & 0x04000000) ||
         (mStatus & 0x08000000) || (mStatus & 0x10000000)))
    {
        mpMeterDraw->setAlphaKanteraAnimeMin();
    } else {
        mpMeterDraw->setAlphaKanteraAnimeMax();
    }

    mpMeterDraw->setAlphaKanteraChange(true);
}

void dMeter2_c::alphaAnimeOxygen() {
    if (!dComIfGp_getOxygenShowFlag() || (mStatus & 0x4000) ||
        ((mStatus & 0x40) && dComIfGp_event_checkHind(0x800)) ||
        ((daPy_getPlayerActorClass()->getSumouMode() != 0) ||
         (daPy_getPlayerActorClass()->checkCanoeSlider() &&
          (dComIfG_getTimerMode() == 3 || dComIfG_getTimerMode() == 4)) ||
         (mStatus & 0x100000) || (mStatus & 0x80000000) || (mStatus & 8) || (mStatus & 0x10) ||
         (mStatus & 0x01000000) || (mStatus & 0x20) || (mStatus & 0x04000000) ||
         (mStatus & 0x08000000) || (mStatus & 0x10000000)))
    {
        mpMeterDraw->setAlphaOxygenAnimeMin();

        if (!dComIfGp_getOxygenShowFlag()) {
            field_0x1e1 = 0;
        }
    } else {
        mpMeterDraw->setAlphaOxygenAnimeMax();

        if (field_0x1e1 == 0) {
            field_0x1e1 = 1;

            if (mpMeterDraw->getMeterGaugeAlphaRate(2) > 0.0f) {
                Z2GetAudioMgr()->seStart(Z2SE_SWIM_TIMER_ON, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
            }
        }
    }

    mpMeterDraw->setAlphaOxygenChange(false);
}

void dMeter2_c::alphaAnimeLightDrop() {
    if (!isShowLightDrop()) {
        mpMeterDraw->setAlphaLightDropAnimeMin();
    } else {
        mpMeterDraw->setAlphaLightDropAnimeMax();
    }

    mpMeterDraw->setAlphaLightDropChange(false);
}


void dMeter2_c::alphaAnimeRupee() {
    if ((mStatus & 0x4000) || ((mStatus & 0x40) && dComIfGp_event_checkHind(0x80)) ||
        daPy_getPlayerActorClass()->getSumouMode() ||
        (daPy_getPlayerActorClass()->checkCanoeSlider() && dComIfG_getTimerMode() == 4) ||
        ((strcmp(dComIfGp_getStartStageName(), "R_SP127") != 0 || dComIfGp_event_checkHind(0x80)) &&
         (((mStatus & 0x40000000) && !(mStatus & 0x100)) || (mStatus & 0x1000) ||
          (mStatus & 0x100000))) ||
        (mStatus & 0x80000000) || (mStatus & 8) || (mStatus & 0x10) || (mStatus & 0x01000000) ||
        ((mStatus & 0x20) != 0) || (mStatus & 0x04000000) || (mStatus & 0x08000000) ||
        (mStatus & 0x10000000) || dMeter2Info_isSub2DStatus(1))
    {
        mpMeterDraw->setAlphaRupeeAnimeMin();
    } else {
        mpMeterDraw->setAlphaRupeeAnimeMax();
    }

    mpMeterDraw->setAlphaRupeeChange(false);
}

void dMeter2_c::alphaAnimeKey() {
    if ((mStatus & 0x4000) || ((mStatus & 0x40) && dComIfGp_event_checkHind(0x40)) ||
        !isKeyVisible() || (mStatus & 0x40000000) || (mStatus & 0x1000) || (mStatus & 0x100000) ||
        (mStatus & 0x80000000) || daPy_getPlayerActorClass()->getSumouMode() ||
        (daPy_getPlayerActorClass()->checkCanoeSlider() &&
         (dComIfG_getTimerMode() == 3 || dComIfG_getTimerMode() == 4)) ||
        (mStatus & 0x100) || (mStatus & 8) || (mStatus & 0x10) ||
        (mStatus & 0x1000000) || (mStatus & 0x20) || (mStatus & 0x4000000) ||
        (mStatus & 0x8000000) || (mStatus & 0x10000000) || dMeter2Info_isSub2DStatus(1))
    {
        mpMeterDraw->setAlphaKeyAnimeMin();
    } else {
        mpMeterDraw->setAlphaKeyAnimeMax();
    }

    mpMeterDraw->setAlphaKeyChange(false);
}

void dMeter2_c::alphaAnimeButton() {
    u8 var_31;
    var_31 = 0;

    if ((mStatus & 0x4000) ||
        ((mStatus & 0x100) && dMsgObject_getMsgObjectClass()->isAutoMessageFlag()) ||
        ((mStatus & 0x40000000) && !(mStatus & 0x100)) || (mStatus & 0x80000000) || (mStatus & 8) ||
        (mStatus & 0x10) || (mStatus & 0x20) || (mStatus & 0x04000000) || (mStatus & 0x10000000))
    {
        mpMeterDraw->setAlphaButtonAnimeMin();
        mpMeterDraw->setAlphaButtonAAnimeMin();
        mpMeterDraw->setAlphaButtonBAnimeMin();
    } else if (!(mStatus & 0x1000000) &&
               ((mStatus & 0x100) || daPy_getPlayerActorClass()->checkHawkWait() ||
                (mStatus & 0x40) && dComIfGp_event_checkHind(1)))
    {
        mpMeterDraw->setAlphaButtonAnimeMin();
        var_31 = 1;
    } else if (dMeter2Info_getItemExplainWindowStatus()) {
        mpMeterDraw->setAlphaButtonAnimeMin();
        var_31 = 1;
    } else {
        mpMeterDraw->setAlphaButtonAnimeMax();
    }

    field_0x1e6 = 0;

    if (var_31 == 1) {
        if (dMeter2Info_getItemExplainWindowStatus()) {
            mpMeterDraw->setAlphaButtonAAnimeMin();
        } else if ((mStatus & 0x100) || daPy_getPlayerActorClass()->checkHawkWait() ||
                   !dComIfGp_event_checkHind(2))
        {
            mpMeterDraw->setAlphaButtonAAnimeMax();
            onShowFlag(0);
        } else {
            mpMeterDraw->setAlphaButtonAAnimeMin();
        }

        if (mpMeterDraw->isBButtonShow(false)) {
            mpMeterDraw->setAlphaButtonBAnimeMax();
            onShowFlag(1);
        } else {
            mpMeterDraw->setAlphaButtonBAnimeMin();
        }
    }

    if (!isShowFlag(0)) {
        mpMeterDraw->setButtonIconAAlpha(mDoStatus, mStatus, field_0x128 == 0);
    }

    if (!isShowFlag(1)) {
        mpMeterDraw->setButtonIconBAlpha(mAStatus, mStatus, field_0x128 == 0);
    }

    for (int i = 0; i < 2; i++) {
        mpMeterDraw->setButtonIconAlpha(i, mItemStatus[i * 2], mStatus,
                                        field_0x128 == 0 ? true : false);

        if (field_0x128 == 0 && dMeter2Info_getMiniGameItemSetFlag() != 1 &&
            (mItemStatus[i * 2] == fpcNm_ITEM_BOW || mItemStatus[i * 2] == fpcNm_ITEM_LIGHT_ARROW ||
             mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV1 || mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV2 ||
             mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV3 || mItemStatus[i * 2] == fpcNm_ITEM_BOMB_BAG_LV1 ||
             mItemStatus[i * 2] == fpcNm_ITEM_NORMAL_BOMB || mItemStatus[i * 2] == fpcNm_ITEM_WATER_BOMB ||
             mItemStatus[i * 2] == fpcNm_ITEM_POKE_BOMB || mItemStatus[i * 2] == fpcNm_ITEM_HAWK_ARROW ||
             mItemStatus[i * 2] == fpcNm_ITEM_BOMB_ARROW || mItemStatus[i * 2] == fpcNm_ITEM_PACHINKO ||
             mItemStatus[i * 2] == fpcNm_ITEM_BEE_CHILD))
        {
            mpMeterDraw->drawItemNum(i, 1.0f);
        } else {
            mpMeterDraw->drawItemNum(i, 0.0f);
        }

        if (field_0x128 == 0 && mItemStatus[i * 2] == fpcNm_ITEM_KANTERA) {
            mpMeterDraw->drawKanteraMeter(i, 1.0f);
        } else {
            mpMeterDraw->drawKanteraMeter(i, 0.0f);
        }
    }

    mpMeterDraw->setAlphaButtonChange(false);
}

void dMeter2_c::alphaAnimeButtonCross() {
    if ((mStatus & 0x4000) || ((mStatus & 0x40) && dComIfGp_event_checkHind(0x100)) ||
        ((daPy_getPlayerActorClass()->getSumouMode() != 0) || (mStatus & 0x100) ||
         (mStatus & 0x80) || (mStatus & 0x40000000) || (mStatus & 0x1000) || (mStatus & 0x100000) ||
         (mStatus & 0x80000000) || (mStatus & 8) || (mStatus & 0x10) || (mStatus & 0x01000000) ||
         (mStatus & 0x20) || (mStatus & 0x04000000) || (mStatus & 0x08000000) ||
         (mStatus & 0x10000000) || (mStatus & 0x20000000)) ||
        daPy_getPlayerActorClass()->checkEnemyAttentionLock() || dMeter2Info_isGameStatus(1))
    {
        mpMeterDraw->setAlphaButtonCrossAnimeMin();

        if ((!dComIfGp_event_chkEventFlag(0x40) || dMeter2Info_isGameStatus(2) ||
             (mStatus & 0x100)) &&
            field_0x190 > 0)
        {
            field_0x190--;
        }
    } else if (dMeter2Info_isSub2DStatus(1) || dMeter2Info_isFloatingMessageVisible()) {
        mpMeterDraw->setAlphaButtonCrossAnimeMin();

        if (field_0x190 < 5) {
            field_0x190++;
        }
    } else {
        mpMeterDraw->setAlphaButtonCrossAnimeMax();

        if (field_0x190 < 5) {
            field_0x190++;
        }
    }

    if (mpMap != NULL) {
        mpMap->setMapAlpha((field_0x190 * 255.0f) / 5.0f);
    }
}

bool dMeter2_c::isShowLightDrop() {
    if (!g_drawHIO.mLightDrop.mAnimDebug) {
        if ((mStatus & 0x4000) || !dComIfGs_isLightDropGetFlag(dComIfGp_getStartStageDarkArea()) ||
            dMeter2Info_getLightDropGetFlag(dComIfGp_getStartStageDarkArea()) <= 1 ||
            !dKy_darkworld_check() || ((mStatus & 0x40) && dComIfGp_event_checkHind(0x200)) ||
            daPy_getPlayerActorClass()->getSumouMode() ||
            (daPy_getPlayerActorClass()->checkCanoeSlider() &&
             (dComIfG_getTimerMode() == 3 || dComIfG_getTimerMode() == 4)) ||
            (mStatus & 0x40000000) || (mStatus & 0x00001000) || (mStatus & 0x00100000) ||
            (mStatus & 0x80000000) || (mStatus & 0x00000100) || (mStatus & 0x00000080) ||
            (mStatus & 0x00000008) || (mStatus & 0x00000010) || (mStatus & 0x01000000) ||
            (mStatus & 0x00000020) || (mStatus & 0x04000000) || (mStatus & 0x08000000) ||
            (mStatus & 0x10000000) || dMeter2Info_isSub2DStatus(1))
        {
            return false;
        }
    }

    return true;
}

void dMeter2_c::killSubContents(u8 param_0) {
    if (mSubContentType != param_0 && mSubContentType != 0) {
        bool free = false;

        if (mpSubContents != NULL) {
            delete mpSubContents;
            mpSubContents = NULL;
            free = true;
        }

        if (mpSubSubContents != NULL) {
            delete mpSubSubContents;
            mpSubSubContents = NULL;
            free = true;
        }

        if (free) {
            mpSubHeap->freeAll();
        }

        mSubContentType = 0;
        mSubContentsStringType = 0;
    }
}

u8 dMeter2_c::isKeyVisible() {
    if (dStage_stagInfo_ChkKeyDisp(dComIfGp_getStage()->getStagInfo())) {
        if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_FIELD) {
            return dComIfGs_getKeyNum() != 0;
        } else {
            return true;
        }
    }

    return false;
}

int dMeter2_c::isArrowEquip() {
    for (int i = 0; i < 2; i++) {
        if (mItemStatus[i * 2] == fpcNm_ITEM_BOW || mItemStatus[i * 2] == fpcNm_ITEM_LIGHT_ARROW ||
            mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV1 || mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV2 ||
            mItemStatus[i * 2] == fpcNm_ITEM_ARROW_LV3 || mItemStatus[i * 2] == fpcNm_ITEM_HAWK_ARROW ||
            mItemStatus[i * 2] == fpcNm_ITEM_BOMB_ARROW)
        {
            return i + 1;
        }
    }
    return 0;
}

int dMeter2_c::isPachinkoEquip() {
    for (int i = 0; i < 2; i++) {
        if (mItemStatus[i * 2] == fpcNm_ITEM_PACHINKO) {
            return i + 1;
        }
    }

    return 0;
}

static int dMeter2_Draw(dMeter2_c* i_this) {
    return i_this->_draw();
}

static int dMeter2_Execute(dMeter2_c* i_this) {
    return i_this->_execute();
}

static int dMeter2_IsDelete(dMeter2_c* i_this) {
    return 1;
}

static int dMeter2_Delete(dMeter2_c* i_this) {
    return i_this->_delete();
}

static int dMeter2_Create(msg_class* i_this) {
    dMeter2Info_setMeterClass(static_cast<dMeter2_c*>(i_this));
    dComIfGp_2dShowOn();
    fopMsgM_Create(PROC_MENUWINDOW, NULL, NULL);

    g_drawHIO.field_0x4 = -1;
    g_ringHIO.field_0x4 = -1;
    g_fmapHIO.field_0x4 = -1;
    g_cursorHIO.field_0x4 = -1;

    u32 id = fopMsgM_Create(PROC_MSG_OBJECT, NULL, NULL);
    fopMsgM_setMessageID(id);

    dTimer_createStockTimer();
    fopMsgM_setStageLayer(i_this);

    return static_cast<dMeter2_c*>(i_this)->_create();
}

static leafdraw_method_class l_dMeter2_Method = {
    (process_method_func)dMeter2_Create,  (process_method_func)dMeter2_Delete,
    (process_method_func)dMeter2_Execute, (process_method_func)dMeter2_IsDelete,
    (process_method_func)dMeter2_Draw,
};

msg_process_profile_definition g_profile_METER2 = {
    fpcLy_CURRENT_e,
    12,
    fpcPi_CURRENT_e,
    PROC_METER2,
    &g_fpcLf_Method.base,
    sizeof(dMeter2_c),
    0,
    0,
    &g_fopMsg_Method,
    0x0301,
    &l_dMeter2_Method,
};
