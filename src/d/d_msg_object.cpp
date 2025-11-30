#include "d/dolzel.h" // IWYU pragma: keep

#define DISABLE_MSG_OBJECT_EXTERN

#include "d/d_msg_object.h"
#include "d/d_msg_class.h"
#include "d/d_msg_string.h"
#include "d/d_msg_scrn_base.h"
#include "d/d_msg_out_font.h"
#include "d/d_msg_scrn_howl.h"
#include "d/d_msg_scrn_jimaku.h"
#include "d/d_msg_scrn_tree.h"
#include "d/d_msg_scrn_kanban.h"
#include "d/d_msg_scrn_item.h"
#include "d/d_msg_scrn_staff.h"
#include "d/d_msg_scrn_place.h"
#include "d/d_msg_scrn_boss.h"
#include "d/d_msg_scrn_talk.h"
#include "d/actor/d_a_obj_ss_base.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_alink.h"
#include "d/d_demo.h"
#include "d/d_meter2_info.h"
#include "d/d_meter2.h"
#include "d/actor/d_a_midna.h"
#include "f_op/f_op_msg_mng.h"
#include "stdio.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_lib.h"
#include "JSystem/JKernel/JKRExpHeap.h"

static void dMsgObject_addFundRaising(s16 param_0);
static void dMsgObject_addTotalPayment(s16 param_0);

static s16 s_groupID;

s16 dMsgObject_getGroupID() {
    return s_groupID;
}

static int dMsgObject_searchSSItem(void* param_1, void* param_2) {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == PROC_OBJ_SSITEM) {
        if (static_cast<daObj_SSBase_c*>(param_1)->getProcessID() == player->getGrabActorID()) {
            static_cast<daObj_SSBase_c*>(param_1)->setSoldOut();
            return 0;
        }
    }
    return 0;
}

dMsgObject_HowlHIO_c::dMsgObject_HowlHIO_c() {
    mHighlightScale = 0.5f;
    mHighlightMoyaR0.set(255, 255, 255, 255);
    mHighlightMoyaR1.set(0, 0, 0, 0);
    mHighlightMoyabsR0.set(200, 255, 200, 255);
    mHighlightMoyabsR1.set(0, 255, 128, 0);
    mHighlightPikaR0.set(0, 0, 0, 0);
    mHighlightPikaR1.set(0, 0, 0, 0);
    mAnmSpeed = 0.1f;
    mHowlLineStartCol.set(150, 255, 210, 255);
    mHowlLineEndCol.set(128, 128, 128, 255);
    mGuideUpperBlack.set(0, 105, 150, 0);
    mGuideUpperWhite.set(255, 255, 255, 255);
    mGuideUpperAlpha = 0.3f;
    mGuideMidBlack.set(0, 105, 130, 0);
    mGuideMidWhite.set(255, 255, 255, 255);
    mGuideMidAlpha = 0.3f;
    mGuideLowerBlack.set(0, 90, 150, 0);
    mGuideLowerWhite.set(255, 255, 255, 255);
    mGuideLowerAlpha = 0.4f;
    mAnswerUpperBlack.set(255, 255, 140, 0);
    mAnswerUpperWhite.set(255, 255, 255, 255);
    mAnswerUpperAlpha = 0.05f;
    mAnswerMidBlack.set(255, 255, 140, 0);
    mAnswerMidWhite.set(255, 255, 255, 255);
    mAnswerMidAlpha = 0.05f;
    mAnswerLowerBlack.set(255, 255, 140, 0);
    mAnswerLowerWhite.set(255, 255, 255, 255);
    mAnswerLowerAlpha = 0.05f;
    mDotBlack.set(150, 210, 255, 0);
    mDotWhite.set(255, 255, 255, 255);
    mDotScale = 1.5f;
    mWaveformLineLen = 15;
    mWaveformDotLen = 35;
    mDrawInterval = 2.0f;
    mLineThickness = 0.5f;
    mCrossPosX = 0.0f;
    mCrossPosY = 0.0f;
    mCrossScale = 1.0f;
    mWindowPosX = 0.0f;
    mWindowPosY = 0.0f;
    mWindowScale = 1.0f;
    mInputStartPoint = 0.0f;
    mWaveformThickness = 12;
    mLineSpaceOffset = 2.2f;
}

dMsgObject_HIO_c::dMsgObject_HIO_c() {
    mBoxMidna_fontSizeX = 23.0f;
    mBoxMidna_fontSizeY = 22.0f;
    field_0xc = 26.0f;
    field_0x14 = 25.0f;
    field_0x3c = 1.0f;
    mBoxNaviAnmSpeed = 1.0f;
    field_0x44 = 1.0f;
    mBoxItemAnmSpeed = 1.0f;
    field_0x4c = 1.0f;
    field_0x50 = 1.0f;
    field_0x54 = 1.0f;
    mBoxWolfAnmSpeed = 0.7f;
    mBoxTalkAlphaP = 0.9f;
    mBoxNaviAlphaP = 0.2f;
    mBoxMidnaAlphaP = 0.7f;
    mBoxItemAlphaP = 0.3f;
    mBoxStoneAlphaP = 0.4f;
    mBoxWoodAlphaP = 0.4f;
    mBoxBookAlphaP = 0.7f;
    mBoxWolfAlphaP = 0.9f;
    mSubtitleAlphaP = 1.0f;
    mBoxItemHaloAlpha = 0.0f;
    mBoxNaviHaloAlpha = 1.0f;
    mBoxMidnaHaloAlpha = 1.0f;
    mBoxWolfHaloAlpha = 0.3f;
    mBoxTalkHaloAlpha = 1.0f;
#if REGION_JPN
    mBoxTalkScaleX = 1.1f;
#else
    mBoxTalkScaleX = 1.2f;
#endif
    mBoxNaviScaleX = 1.0f;
    mBoxMidnaScaleX = 1.0f;
    mBoxItemScaleX = 1.05f;
    mBoxStoneScaleX = 1.0f;
    mBoxWoodScaleX = 1.0f;
    mBoxBookScaleX = 1.0f;
    mBoxWolfScaleX = 1.05f;
    mStageTitleScaleX = 1.0f;
    mSubtitleScaleX = 1.0f;
    mBossNameScaleX = 1.0f;
    mBoxTalkScaleY = 1.0f;
    mBoxNaviScaleY = 1.0f;
    mBoxMidnaScaleY = 1.0f;
    mBoxItemScaleY = 0.97f;
    mBoxStoneScaleY = 1.0f;
    mBoxWoodScaleY = 1.0f;
    mBoxBookScaleY = 1.0f;
    mBoxWolfScaleY = 1.1f;
    mStageTitleScaleY = 1.0f;
    mSubtitleScaleY = 1.0f;
    mBossNameScaleY = 1.0f;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 10; j++) {
            mChoicePos[i][j] = 0.0f;
            mBoxPos[i][j] = 0.0f;
        }
    }
    mBoxPos[2][0] = 30.0f;
    mBoxPos[0][3] = 11.0f;
    mPortalIconScale = 1.7f;
    mTextPosX = 4.5f;
    mTextPosY = 0.0f;
    mStageTitleCharPosX = 0.0f;
    mStageTitleCharPosY = -45.0f;
    mStageTitleCharSizeX = 1.0f;
    mStageTitleCharSizeY = 1.0f;
    mStageTitleBasePosX = 0.0f;
    mStageTitleBasePosY = -45.0f;
    mStageTitleBaseSizeX = 1.0f;
    mStageTitleBaseSizeY = 1.0f;
    mStageTitleBaseAlpha = 1.0f;
    mBossNameCharPosX = 0.0f;
    mBossNameCharPosY = 0.0f;
    mBossNameCharSizeX = 1.0f;
    mBossNameCharSizeY = 1.0f;
    mBossNameBasePosX = 0.0f;
    mBossNameBasePosY = 0.0f;
    mBossNameBaseSizeX = 1.0f;
    mBossNameBaseSizeY = 1.0f;
    mBossNameBaseAlpha = 1.0f;
    mStageTitleDisplayType = 0;
    mBoxItemSizeX = 0.75f;
    mBoxItemSizeY = 0.75f;
    mBoxItemPosX = -5.0f;
    mBoxItemPosY = -1.0f;
    mBoxItemTextSizeX = 0.95f;
    mBoxItemTextSizeY = 0.95f;
    mBoxItemTextPosX = -30.0f;
    mBoxItemTextPosY = -1.0f;
    for (int i = 0; i < 10; i++) {
        mBoxStartBlack[0][i].set(0, 0, 0, 0);
        mBoxStartWhite[0][i].set(200, 200, 200, 255);
        mBoxStartBlack[1][i].set(0, 0, 0, 0);
        mBoxStartWhite[1][i].set(0, 0, 0, 255);
    }
    mSelMsgInactiveCol.set(150, 150, 150, 255);
    mMsgIndex = 0;
    mFlowIndex = 0;
    mSaveSeqMsgIndex = 0;
    mSelWaitFrame = 10;
    mBoxAppearBound = 240;
    mBoxAppearFrame = 10;
    mWaitFrame = 0;
    mLightAppearFrame = 10;
    field_0x304 = 5;
    mStageTitleFadeIn = 10;
    mStageTitleFadeOut = 10;
    mBossNameFadeIn = 10;
    mBossNameFadeOut = 10;
    mStageTitleDisplayTime = 60;
    mBossNameDisplayTime = 60;
    mMsgDebug = 0;
    mSaveSeqMsgDebug = 0;
    mBatchDisplayWeight_3 = 30;
    mCharDisplayWeight_5 = 3;
    mBatchDisplayWeight_8 = 10;
    mBatchDisplayWeight_9 = 30;
    mTextColorDebug = 0;
    mTextColorUpperR[0] = 0xff;
    mTextColorUpperG[0] = 0xff;
    mTextColorUpperB[0] = 0xff;
    mTextColorLowerR[0] = 0xff;
    mTextColorLowerG[0] = 0xff;
    mTextColorLowerB[0] = 0xff;
    mTextColorUpperR[1] = 0xf0;
    mTextColorUpperG[1] = 0x78;
    mTextColorUpperB[1] = 0x78;
    mTextColorLowerR[1] = 0xf0;
    mTextColorLowerG[1] = 0x78;
    mTextColorLowerB[1] = 0x78;
    mTextColorUpperR[2] = 0xaa;
    mTextColorUpperG[2] = 0xdc;
    mTextColorUpperB[2] = 0x8c;
    mTextColorLowerR[2] = 0xaa;
    mTextColorLowerG[2] = 0xdc;
    mTextColorLowerB[2] = 0x8c;
    mTextColorUpperR[3] = 0xa0;
    mTextColorUpperG[3] = 0xb4;
    mTextColorUpperB[3] = 0xdc;
    mTextColorLowerR[3] = 0xa0;
    mTextColorLowerG[3] = 0xb4;
    mTextColorLowerB[3] = 0xdc;
    mTextColorUpperR[4] = 0xdc;
    mTextColorUpperG[4] = 0xdc;
    mTextColorUpperB[4] = 0x82;
    mTextColorLowerR[4] = 0xdc;
    mTextColorLowerG[4] = 0xdc;
    mTextColorLowerB[4] = 0x82;
    mTextColorUpperR[5] = 0xb4;
    mTextColorUpperG[5] = 0xc8;
    mTextColorUpperB[5] = 0xe6;
    mTextColorLowerR[5] = 0xb4;
    mTextColorLowerG[5] = 0xc8;
    mTextColorLowerB[5] = 0xe6;
    mTextColorUpperR[6] = 0xc8;
    mTextColorUpperG[6] = 0xa0;
    mTextColorUpperB[6] = 0xdc;
    mTextColorLowerR[6] = 0xc8;
    mTextColorLowerG[6] = 0xa0;
    mTextColorLowerB[6] = 0xdc;
    mTextColorUpperR[7] = 0xff;
    mTextColorUpperG[7] = 0xff;
    mTextColorUpperB[7] = 0xff;
    mTextColorLowerR[7] = 0xff;
    mTextColorLowerG[7] = 0xff;
    mTextColorLowerB[7] = 0xff;
    mTextColorUpperR[8] = 0xdc;
    mTextColorUpperG[8] = 0xaa;
    mTextColorUpperB[8] = 0x78;
    mTextColorLowerR[8] = 0xdc;
    mTextColorLowerG[8] = 0xaa;
    mTextColorLowerB[8] = 0x78;
    mHaloDelayFrame = 0;
    mDisplaySpeed = 1;
    mHaloDelayFrameSpirit = 0;
    mDisplaySpeedSpirit = 4;
    mMessageDisplay = 0;
    mPikariHaloDelay_spirit = 0;
    mPikariScale = 1.5f;
    mPikariMoyaR0.r = 0xff;
    mPikariMoyaR0.g = 0xff;
    mPikariMoyaR0.b = 0xff;
    mPikariMoyaR0.a = 0x14;
    mPikariMoyaR1.r = 0xdc;
    mPikariMoyaR1.g = 0xb0;
    mPikariMoyaR1.b = 0x8a;
    mPikariMoyaR1.a = 0;
    mPikariMoyabsR0.r = 0xff;
    mPikariMoyabsR0.g = 0xff;
    mPikariMoyabsR0.b = 0xff;
    mPikariMoyabsR0.a = 30;
    mPikariMoyabsR1.r = 0xc0;
    mPikariMoyabsR1.g = 0x73;
    mPikariMoyabsR1.b = 0;
    mPikariMoyabsR1.a = 0;
    mPikariPikaR0.r = 0xaf;
    mPikariPikaR0.g = 0xdc;
    mPikariPikaR0.b = 0xa0;
    mPikariPikaR0.a = 0x8c;
    mPikariPikaR1.r = 0x96;
    mPikariPikaR1.g = 0xd2;
    mPikariPikaR1.b = 0x78;
    mPikariPikaR1.a = 0;
    mPikariAnmSpeed = 0.3f;
}

int dMsgObject_c::_create(msg_class* param_1) {
    field_0x124 = NULL;
    field_0x100 = param_1;
    field_0x16c = -1;
    field_0x16e = -1;
    mNowTalkFlowNo = 0;
    mpTalkActor = NULL;
    field_0x13c = 0;
    mpTalkPartner = NULL;
    mpTalkHeap = NULL;
    field_0x148 = NULL;
    mProcessID = 0xffffffff;
    mIdx = 0;
    mNodeIdx = 0xff;
    field_0x198 = 0;
    field_0x199 = 0;
    mFukiKind = 0xff;
    field_0x19b = 0;
    mFlowChk = 0;
    field_0x19d = 0;
    mpMsgDt = NULL;
    field_0x168 = 0;
    mInputValue = 0;
    mFundRaisingValue = 0;
    field_0x178 = 0;
    mNowTotalPrice = 0;
    mNowTotalPayment = 0;
    field_0x17e = 0;
    mPortalMessageID = 0;
    for (int i = 0; i < 3; i++) {
        mBombMessageID[i] = 0;
        mBombNum[i] = 0;
        mBombBagID[i] = 0xff;
        mSelectBombBag[i] = 0;
    }
    field_0x193 = 0;
    mInsectItemNo = 0xc0;
    mLetterNameID = 0;
    mItemEquipButton = 0;
    mSelectCancelPos = 0;
    mArrowNum = 0;
    mpMsgRes = dMeter2Info_getMsgResource();
    changeGroup(0);
    mpOutFont = NULL;
    mpScrnDraw = NULL;
    mpResCont = new JMessage::TResourceContainer();
    JUT_ASSERT(1299, mpResCont != NULL);
    mpCtrl = dDemo_c::getMesgControl();
    JUT_ASSERT(1302, mpCtrl != NULL);
    mpRefer = new jmessage_tReference();
    JUT_ASSERT(1305, mpRefer != NULL);
    mpRefer->setpStatus(&param_1->mode);
    mpSeqProc = new jmessage_tSequenceProcessor(mpRefer, mpCtrl);
    JUT_ASSERT(1310, mpSeqProc != NULL);
    mpRenProc = new jmessage_tRenderingProcessor(mpRefer);
    JUT_ASSERT(1313, mpRenProc != NULL);
    mpRefer->setResourceContainer(mpResCont);
    mpCtrl->setSequenceProcessor(mpSeqProc);
    mpCtrl->setRenderingProcessor(mpRenProc);
    mpRefer->setObjectPtr(this);
    field_0x124 = new JMessage::TParse(mpResCont);
    field_0x124->parse(mpMsgDt, 0);
    mpCtrl->reset();
    mpCtrl->resetResourceCache();
    mpRefer->resetCharCountBuffer();
    field_0x150 = 0.0f;
    mCameraCancelFlag = 0;
    mInputFlag = 0;
    mSmellType = 0xff;
    field_0x4ca = 0;
    mNoDemoFlag = 0;
    field_0x4cc = 0;
    field_0x4cd = 0;
    field_0x4ce = 0;
    field_0x4d4 = 0;
    field_0x4c7 = 0;
    mMsgOutputType = 0xff;
    mShopWaitTimer = 0;
    mSelectPushFlag = 0;
    field_0x1a3 = 0;
    mAutoMessageFlag = 0;
    mKillMessageFlag = false;
    mEquipBombInfo = 0;
    mPaymentFlag = 0;
    field_0x4d1 = 0;
    field_0x196 = 0;
    mMsgSendControl = 0;
    field_0x197 = 0;
    mMessageID = 1000;
    field_0x158 = mMessageID;
    field_0x15c = 0;
    field_0x172 = 0;
    setStatusLocal(1);
    mpMsgString = new dMsgString_c();
    JUT_ASSERT(1366, mpMsgString != NULL);
    return 4;
}

static void dummyStrings() {
    DEAD_STRING("zel_00.bmg");
    DEAD_STRING("zel_01.bmg");
    DEAD_STRING("zel_02.bmg");
    DEAD_STRING("zel_03.bmg");
    DEAD_STRING("zel_04.bmg");
    DEAD_STRING("zel_05.bmg");
    DEAD_STRING("zel_06.bmg");
    DEAD_STRING("zel_07.bmg");
    DEAD_STRING("zel_08.bmg");
    DEAD_STRING("zel_99.bmg");
    DEAD_STRING("");
}

dMsgObject_HIO_c g_MsgObject_HIO_c;

int dMsgObject_c::_execute() {
    field_0x4c7 = 0;
    if (mpTalkHeap != NULL) {
        field_0x148 = mDoExt_setCurrentHeap(mpTalkHeap);
    }
    if (isKillMessageFlagLocal()) {
        if (getStatusLocal() != 1) {
            setStatusLocal(19);
        } else {
            mNoDemoFlag = 0;
            field_0x4cc = 0;
        }
        offKillMessageFlagLocal();
    }
    dMeter2Info_decMsgKeyWaitTimer();
    field_0x4ca = mpCtrl->update();
    if (g_MsgObject_HIO_c.mMessageDisplay) {
        if (mpRefer->getRevoMessageID() != 0) {
            mpCtrl->reset();
            mpCtrl->resetResourceCache();
            mpRefer->resetCharCountBuffer();
            mpCtrl->setMessageCode(mpRefer->getRevoMessageID());
            mpRefer->setRevoMessageID(0);
            jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
            strcpy(pRef->getTextPtr(), "");
            strcpy(pRef->getTextSPtr(), "");
            strcpy(pRef->getRubyPtr(), "");
            for (int i = 0; i < 3; i++) {
                strcpy(pRef->getSelTextPtr(i), "");
                strcpy(pRef->getSelRubyPtr(i), "");
            }
        }
        field_0x4ca = mpCtrl->update();
    }
    field_0x166 = getStatusLocal();
    mSelectPushFlag = 0;
    switch (getStatusLocal()) {
    case 1:
        waitProc();
        break;
    case 2:
        openProc();
        break;
    case 6:
        outnowProc();
        break;
    case 5:
        outwaitProc();
        break;
    case 7:
        stopProc();
        break;
    case 15:
        continueProc();
        break;
    case 8:
    case 9:
    case 20:
        selectProc();
        break;
    case 24:
        selectEndProc();
        break;
    case 21:
        inputProc();
        break;
    case 16:
        finishProc();
        break;
    case 17:
        endProc();
        break;
    case 19:
        deleteProc();
        break;
    case 11:
        textmodeProc();
        break;
    }
    if (isDraw() && !isPlaceMessage()) {
        if (mpRefer->getCharSoundInfo().field_0x40e != mpRefer->getCharSoundInfo().mCountBackUp) {
            Z2GetSpeechMgr2()->setTextCount(mpRefer->getCharSoundInfo().field_0x40e);
            mpRefer->setCountBackUp();
        }
    }

    if (mpScrnDraw != NULL) {
        mpScrnDraw->exec();
    }
    
    jmessage_tReference* procRef = (jmessage_tReference*)mpRenProc->getReference();
    if (field_0x17e < procRef->getDemoFrame()) {
        field_0x17e++;
    }
    field_0x168 = getStatusLocal();
    setShopWaitTimerLocal(0);
    if (field_0x148 != NULL) {
        mDoExt_setCurrentHeap(field_0x148);
        field_0x148 = NULL;
    }

    return 1;
}

int dMsgObject_c::_draw() {
    u16 sVar7 = getStatusLocal();
    if (sVar7 != 0 && sVar7 != 1 && sVar7 != 0xb) {
        if (isDraw()) {
            mpCtrl->render();
        }
        if (mpScrnDraw != NULL) {
            jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
            mpScrnDraw->setString(pRef->getTextPtr(), pRef->getTextSPtr());
            mpScrnDraw->setRubyString(pRef->getRubyPtr());
            mpScrnDraw->setSelectString(pRef->getSelTextPtr(0), pRef->getSelTextPtr(1),
                                        pRef->getSelTextPtr(2));
            mpScrnDraw->setSelectRubyString(pRef->getSelRubyPtr(0), pRef->getSelRubyPtr(1),
                                            pRef->getSelRubyPtr(2));
        }
    }
    if (mpScrnDraw != NULL) {
        if (dComIfGp_isPauseFlag()) {
            dComIfGd_set2DOpaTop(mpScrnDraw);
        } else {
            dComIfGd_set2DOpa(mpScrnDraw);
        }
        mpScrnDraw->multiDraw();
    }
    return 1;
}

int dMsgObject_c::_delete() {
    mpResCont->destroyResource_all();
    if (mpScrnDraw != NULL) {
        delete mpScrnDraw;
        mpScrnDraw = 0;
    }
    mpMsgDt = 0;
    mpMsgRes = 0;
    field_0x12c = 0;
    if (mpOutFont != NULL) {
        delete mpOutFont;
        mpOutFont = NULL;
    }
    delete mpResCont;
    mpResCont = NULL;
    delete mpRefer;
    mpRefer = NULL;
    delete mpSeqProc;
    mpSeqProc = NULL;
    delete mpRenProc;
    mpRenProc = NULL;
    delete field_0x124;
    field_0x124 = NULL;
    delete mpMsgString;
    mpMsgString = NULL;
    if (mpTalkHeap != NULL) {
        mpTalkHeap = NULL;
    }
    if (dComIfGp_isHeapLockFlag() == 5) {
        dComIfGp_offHeapLockFlag(7);
        dComIfGp_offHeapLockFlag(8);
    }
    dComIfGp_setMsgObjectClass(NULL);
    return 1;
}

void dMsgObject_c::setMessageIndex(u32 revoIndex, u32 param_2, bool param_3) {
    field_0x158 = revoIndex;
    revoIndex = getRevoMessageIndex(revoIndex);
    if (field_0x4cc == 0) {
        mNoDemoFlag = 1;
    }
    mMessageID = revoIndex;
    field_0x15c = param_2;
    field_0x4d1 = 0;
    if (mpTalkPartner != field_0x13c && mpTalkPartner != NULL) {
        dComIfGp_event_setTalkPartner(mpTalkPartner);
        field_0x4d1 = 1;
        field_0x13c = mpTalkPartner;
    }
    s16 groupID = getMessageGroup(mMessageID);
    JUT_ASSERT(1736, groupID==s_groupID || groupID == 0)
    changeGroup(groupID);
    field_0x172 = 0;
    mpRefer->setPageNum(field_0x172);

    JMSMesgInfo_c* pMsg = (JMSMesgInfo_c*)((char*)mpMsgDt + 0x20);
    u8* iVar2 = (u8*)pMsg + pMsg->header.size;
    u32 msg_id = getMessageIndex(revoIndex);
    dComIfGp_setMesgCameraAttrInfo(pMsg->entries[msg_id].camera_id);
    if (field_0x15c == 1000) {
        mpRefer->setSelMsgPtr(NULL);
    } else {
        u32 msgIndex = getMessageIndex(field_0x15c);
        if (msgIndex == 0x264) {
            mpRefer->setSelMsgPtr(NULL);
        } else {
            char* my_ptr = (char*) (iVar2 + pMsg->entries[msgIndex].string_offset + 8);
            mpRefer->setSelMsgPtr(my_ptr);
        }
    }
    if (param_3) {
        mpCtrl->setMessageID(mMessageID, 0, NULL);
    }
}

void dMsgObject_c::setMessageIndexDemo(u32 revoMsgIndex, bool param_2) {
    field_0x158 = revoMsgIndex;
    revoMsgIndex = getRevoMessageIndex(revoMsgIndex);
    mNoDemoFlag = 1;
    field_0x4d4 = 1;
    dMsgObject_onCameraCancelFlag();
    mMessageID = revoMsgIndex;
    field_0x15c = 0x264;
    field_0x4d1 = 0;
    if (mpTalkPartner != field_0x13c && mpTalkPartner != NULL) {
        dComIfGp_event_setTalkPartner(mpTalkPartner);
        field_0x4d1 = 1;
        field_0x13c = mpTalkPartner;
    }
    s16 groupID = getMessageGroup(mMessageID);
    JUT_ASSERT(1804, groupID==s_groupID || groupID == 0);
    changeGroup(groupID);
    field_0x172 = 0;
    mpRefer->setPageNum(field_0x172);
    JMSMesgInfo_c* info_header_p = (JMSMesgInfo_c*)((char*)mpMsgDt + 0x20);
    JMSMesgInfo_c* reg_25 = (JMSMesgInfo_c*)((char*) info_header_p + info_header_p->header.size);
    int ind = getMessageIndex(revoMsgIndex);
    dComIfGp_setMesgCameraAttrInfo(info_header_p->entries[ind].camera_id);
    mpRefer->setSelMsgPtr(NULL);
    if (param_2) {
        mpCtrl->setMessageID(mMessageID, 0, NULL);
    }
}

u32 dMsgObject_c::getMessageIndex(u32 param_0) {
    u32 i = 0;
    JMSMesgInfo_c* pMsg = (JMSMesgInfo_c*)((char*)mpMsgDt + 0x20);
    u32 msgIndexCount = *((u16*)((char*)mpMsgDt + 0x28));
    int rv;
    for (; i < msgIndexCount; i++) {
        if (pMsg->entries[i].message_id == param_0) {
            rv = i;
            break;
        }
    }
    if (i < msgIndexCount) {
        return rv;
    }
    JUT_WARN(1848, "Message Index Not Exist!! %d", param_0)
    return 612;
}

u32 dMsgObject_c::getRevoMessageIndex(u32 param_1) {
    if (!g_MsgObject_HIO_c.mMessageDisplay) {
        return param_1;
    }
    u32 msgIndexCount;
    JMSMesgInfo_c* pMsg;
    int i = 0;
    int rv;
    s16 groupID = getMessageGroup(param_1);
    JUT_ASSERT(1916, groupID==s_groupID || groupID == 0)
    changeGroup(groupID);
    pMsg = (JMSMesgInfo_c*)((char*)mpMsgDt + 0x20);
    msgIndexCount = *((u16*)((char*)mpMsgDt + 0x28));
    for (; i < msgIndexCount; i++) {
        if (pMsg->entries[i].message_id == param_1) {
            s8* ptr = (s8*)pMsg + pMsg->header.size + pMsg->entries[i].string_offset + 8;
            if (ptr[0] == 26 && ptr[2] == 3 && (s8)ptr[4] == 0) {
                rv = pMsg->entries[*(int*)(ptr + 5)].message_id;
            } else {
                rv = param_1;
            }
            break;
        }
    }

    if (i >= msgIndexCount) {
        JUT_WARN(1937, "Message Index Not Exist!! %d", param_1)
        rv = 612;
    }
    return rv;
}

u32 dMsgObject_c::getMessageIndexAlways(u32 param_0) {
    u32 i = 0;
    JMSMesgInfo_c* pMsg = (JMSMesgInfo_c*)((char*)mpMsgRes + 0x20);
    u32 msgIndexCount = *((u16*)((char*)mpMsgRes + 0x28));
    int rv;
    for (; i < msgIndexCount; i++) {
        if (pMsg->entries[i].message_id == param_0) {
            rv = i;
            break;
        }
    }
    if (i < msgIndexCount) {
        return rv;
    }
    JUT_WARN(1972, "Message Index Not Exist!! %d", param_0)
    return 612;
}

u32 dMsgObject_c::getMessageIDAlways(u32 param_0) {
    return ((JMSMesgInfo_c*)((u8*)mpMsgRes + 0x20))->entries[param_0].message_id;
}

s16 dMsgObject_c::getMessageGroup(u32 param_0) {
    s16 messageGroup = 0;
    OS_REPORT("getMessgeGroup! msg no====>%d\n", param_0);
    if (param_0 > 5000) {
        messageGroup = s_groupID;
    }
    return messageGroup;
}

void dMsgObject_c::waitProc() {
    if (mNoDemoFlag || field_0x4cc) {
        if (isMidonaNormalTalkWait()) {
            if (dComIfGp_checkCameraAttentionStatus(dComIfGp_getPlayerCameraID(0), 4) ||
                dMsgObject_isCameraCancelFlag() || dComIfGp_isPauseFlag())
            {
                if (field_0x4ca) {
                    field_0x19d = 0;
                    dComIfGp_setMesgCancelButton(0);
                    offInputFlagLocal();
                    setStatusLocal(2);
                    if (isTalkMessage()) {
                        mDoAud_seStart(Z2SE_SY_TALK_WIN_OPEN, 0, 0, 0);
                    }
                } else {
                    if (mMessageID >= 0x47f && mMessageID <= 0x487) {
                        setMessageIndexDemo(mMessageID, true);
                    } else {
                        setMessageIndex(mMessageID, field_0x15c, true);
                    }
                }
            }
        }
    } else if (field_0x4ca) {
        if (field_0x4cd) {
            setStatusLocal(11);
        } else {
            dComIfGp_setMesgCancelButton(0);
            offInputFlagLocal();
            setStatusLocal(2);
            if (isTalkMessage()) {
                mDoAud_seStart(Z2SE_SY_TALK_WIN_OPEN, 0, 0, 0);
            }
        }
    }
}

void dMsgObject_c::openProc() {
    if (isMidonaMessage()) {
        bool uVar12 = 0;
        if (field_0x16a == 0) {
            jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
            field_0x1a3 = 0;
            if (mpRefer->getMsgID() == 0x7fa) {
                mpScrnDraw->selectAnimeInit(3, pRef->getSelectPos(), pRef->getSelTBoxWidth(),
                                            pRef->getSelectRubyFlag());
            } else {
                mpScrnDraw->selectAnimeInit(2, pRef->getSelectPos() + 1, pRef->getSelTBoxWidth(),
                                            pRef->getSelectRubyFlag());
            }
            pRef->setStopFlag(0);
        } else {
            if (mDoCPd_c::getTrigA(0)) {
                if (getSelectCursorPosLocal() != 0xff) {
                    field_0x1a3 = 1;
                    field_0x16a = 9;
                }
            } else if (mDoCPd_c::getTrigB(0) && getSelectCancelPos() != 0) {
                setSelectCursorPosLocal(getSelectCancelPos() - 1);
                uVar12 = 1;
                field_0x1a3 = 2;
                field_0x16a = 9;
            }
            if (mpRefer->getMsgID() == 0x7fa) {
                mpScrnDraw->selectAnimeMove(2, getSelectCursorPosLocal(), uVar12);
            } else {
                if (getSelectCursorPosLocal() != 0xff) {
                    mpScrnDraw->selectAnimeMove(2, getSelectCursorPosLocal() + 1, uVar12);
                } else {
                    mpScrnDraw->selectAnimeMove(2, getSelectCursorPosLocal(), uVar12);
                }
            }
        }
    }
    field_0x16a++;
    s16 sVar7;
    if (isKanbanMessage()) {
        sVar7 = g_MsgObject_HIO_c.mBoxAppearFrame;
        f32 dVar15 = (f32)field_0x16a / g_MsgObject_HIO_c.mBoxAppearFrame;
        mpScrnDraw->fukiScale(dVar15);
        mpScrnDraw->fukiAlpha(dVar15);
        mpOutFont->setAlphaRatio(dVar15);
    } else if (isPlaceMessage() || isStaffMessage()) {
        sVar7 = g_MsgObject_HIO_c.mStageTitleFadeIn;
        f32 dVar15 = (f32)field_0x16a / g_MsgObject_HIO_c.mStageTitleFadeIn;
        mpScrnDraw->fukiScale(dVar15);
        mpScrnDraw->fukiAlpha(dVar15);
        mpOutFont->setAlphaRatio(dVar15);
    } else if (isBossMessage()) {
        sVar7 = g_MsgObject_HIO_c.mBossNameFadeIn;
        f32 dVar15 = (f32)field_0x16a / g_MsgObject_HIO_c.mBossNameFadeIn;
        mpScrnDraw->fukiScale(dVar15);
        mpScrnDraw->fukiAlpha(dVar15);
        mpOutFont->setAlphaRatio(dVar15);
    } else if (isBookMessage()) {
        sVar7 = g_MsgObject_HIO_c.mBoxAppearFrame + g_MsgObject_HIO_c.mWaitFrame +
                g_MsgObject_HIO_c.mLightAppearFrame;
        if (field_0x16a <= g_MsgObject_HIO_c.mBoxAppearFrame) {
            f32 dVar15 = (f32)field_0x16a / g_MsgObject_HIO_c.mBoxAppearFrame;
            mpScrnDraw->fukiAlpha(dVar15);
            mpOutFont->setAlphaRatio(dVar15);
        } else {
            mpScrnDraw->fukiAlpha(1.0f);
            mpOutFont->setAlphaRatio(1.0f);
        }
        s16 sVar4 = g_MsgObject_HIO_c.mBoxAppearFrame + g_MsgObject_HIO_c.mWaitFrame;
        if (field_0x16a >= sVar4 && field_0x16a <= sVar4 + g_MsgObject_HIO_c.mLightAppearFrame) {
            f32 scale = (f32)(field_0x16a - sVar4);
            scale /= g_MsgObject_HIO_c.mLightAppearFrame;
            mpScrnDraw->fukiScale(scale);
        }
    } else {
        sVar7 = g_MsgObject_HIO_c.mBoxAppearFrame;
        f32 dVar14 = (f32)field_0x16a / sVar7;
        mpScrnDraw->fukiScale(1.0f);
        mpScrnDraw->fukiAlpha(dVar14);
        mpOutFont->setAlphaRatio(dVar14);
    }
    mpRenProc->setTextInitPos(mpScrnDraw->getTextBoxPosX(), mpScrnDraw->getTextBoxPosY());
    mpRenProc->setTextScale(mpScrnDraw->getTextBoxScaleX(), mpScrnDraw->getTextBoxScaleY());
    if (field_0x16a >= sVar7) {
        mpScrnDraw->fukiTrans(0.0f, 0.0f);
        for (int i = 0; i < 3; i++) {
            mpRenProc->setSelTextInitPos(i, mpScrnDraw->getSelTextBoxPosX(i),
                                         mpScrnDraw->getSelTextBoxPosY(i));
        }
        field_0x16a = 0;
        if (isMidonaMessage()) {
            if (mpRefer->getMsgID() == 0x7fa) {
                setStatusLocal(9);
            } else {
                setStatusLocal(8);
            }
        } else {
            setStatusLocal(6);
        }
    }
}

void dMsgObject_c::outnowProc() {
    mpRefer->shiftCharCountBuffer();
    if (isBookMessage() && field_0x16a > 0) {
        field_0x16a--;
        f32 alpha = (10 - field_0x16a) / 10.0f;
        mpScrnDraw->fontAlpha(alpha);
        mpOutFont->setAlphaRatio(alpha);
        if (field_0x16a > 0) {
            return;
        }
    }
    jmessage_tReference* pRef =
        (jmessage_tReference*)mpRenProc->getReference();
    if (pRef->getCharAllAlphaRate() < 1.0f) {
        if (mDoCPd_c::getTrigA(0)) {
            pRef->setCharAllAlphaRate(1.0f);
        } else {
            pRef->addCharAllAlphaRate();
        }
        f32 alpha = pRef->getCharAllAlphaRate();
        mpScrnDraw->setCharAlphaRate(alpha);
        mpOutFont->setAlphaRatio(alpha);
    } else if (mpRefer->isLightEnd()) {
        if (pRef->isButtonTagStopFlag()) {
            setButtonStatusLocal();
            mpScrnDraw->arwAnimeMove();
        }
        if (pRef->getStopFlag() == 1) {
            field_0x16a = 0;
            field_0x196 = 0;
            setStatusLocal(5);
            pRef->setStopFlag(0);
        } else if (pRef->getStopFlag() == 7) {
            field_0x196 = 1;
            if (mNoDemoFlag && !field_0x4d4) {
                setStatusLocal(14);
            } else {
                field_0x199 = 0;
                setStatusLocal(16);
            }
            pRef->setStopFlag(0);
        } else if (pRef->getStopFlag() == 2) {
            field_0x1a3 = 0;
            setStatusLocal(8);
            mpScrnDraw->selectAnimeInit(2, pRef->getSelectPos() + 1,
                                        pRef->getSelTBoxWidth(),
                                        pRef->getSelectRubyFlag());
            pRef->setStopFlag(0);
        } else if (pRef->getStopFlag() == 3) {
            field_0x1a3 = 0;
            setStatusLocal(9);
            mpScrnDraw->selectAnimeInit(3, pRef->getSelectPos(),
                                        pRef->getSelTBoxWidth(),
                                        pRef->getSelectRubyFlag());
            pRef->setStopFlag(0);
        } else if (pRef->getStopFlag() == 4 || pRef->getStopFlag() == 5) {
            field_0x1a3 = 0;
            setStatusLocal(20);
            pRef->setStopFlag(0);
        } else if (pRef->getStopFlag() == 6) {
            setStatusLocal(21);
            pRef->setStopFlag(0);
        }
    }
}

void dMsgObject_c::outwaitProc() {
    jmessage_tReference* pRef =
        (jmessage_tReference*)mpRenProc->getReference();
    mpScrnDraw->arwAnimeMove();
    if (isBookMessage()) {
        if (isSend() || field_0x16a > 0) {
            field_0x16a++;
            if (field_0x16a <= 10) {
                f32 alpha = (10 - field_0x16a) / 10.0f;
                mpScrnDraw->fontAlpha(alpha);
                mpOutFont->setAlphaRatio(alpha);
                if (field_0x16a >= 10) {
                    field_0x172++;
                    mpRefer->setPageNum(field_0x172);
                    mpCtrl->render_synchronize();
                    pRef->pageSend();
                    mpScrnDraw->arwAnimeInit();
                    mpRenProc->resetRendering();
                    field_0x199 = 0;
                    setStatusLocal(6);
                }
            }
        }
    } else if (isSend()) {
        field_0x172++;
        mpRefer->setPageNum(field_0x172);
        mpCtrl->render_synchronize();
        pRef->pageSend();
        mAutoMessageFlag = 0;
        mpScrnDraw->arwAnimeInit();
        mpRenProc->resetRendering();
        field_0x199 = 0;
        setStatusLocal(6);
        if (isTalkMessage() && mpRefer->getForm() != 1) {
            mDoAud_seStart(daAlink_c::WANM_HOWL_END, 0, 0, 0);
        }
    }
}

void dMsgObject_c::stopProc() {
}

void dMsgObject_c::continueProc() {
    jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
    if (pRef->getSendFlag() != 1 && pRef->getSendFlag() != 3 && pRef->getSendFlag() != 4 &&
        pRef->getSendFlag() != 6)
    {
        mpScrnDraw->arwAnimeMove();
    }
    if (((mDoCPd_c::getTrigA(0) == 0 && mDoCPd_c::getTrigB(0) == 0) || !isHowlMessage()) &&
        isSend())
    {
        mpCtrl->render_synchronize();
        mpCtrl->reset();
        mpCtrl->resetResourceCache();
        mpRefer->resetCharCountBuffer();
        field_0x17e = 0;
        field_0x199 = 0;
        updateEquipBombInfoLocal();
        offAutoMessageFlagLocal();
        setMessageIndex(field_0x100->msg_idx, field_0x100->field_0xf0, true);
        mpScrnDraw->fukiPosCalc(pRef->getFukiPosType());
        strcpy(pRef->getTextPtr(), "");
        strcpy(pRef->getTextSPtr(), "");
        strcpy(pRef->getRubyPtr(), "");
        for (int i = 0; i < 3; i++) {
            strcpy(pRef->getSelTextPtr(i), "");
            strcpy(pRef->getSelRubyPtr(i), "");
        }
        mpScrnDraw->arwAnimeInit();
        mpRenProc->setTextInitPos(mpScrnDraw->getTextBoxPosX(), mpScrnDraw->getTextBoxPosY());
        mpRenProc->setTextScale(mpScrnDraw->getTextBoxScaleX(), mpScrnDraw->getTextBoxScaleY());
        for (int i = 0; i < 3; i++) {
            mpRenProc->setSelTextInitPos(i, mpScrnDraw->getSelTextBoxPosX(i), mpScrnDraw->getSelTextBoxPosY(i));
        }
        mpRenProc->resetRendering();
        setStatusLocal(6);
        if (isTalkMessage() && mpRefer->getForm() != 1) {
            mDoAud_seStart(daAlink_c::WANM_HOWL_END, 0, 0, 0);
        }
    }
}

void dMsgObject_c::selectProc() {
    bool uVar7 = false;
    bool iVar8 = false;
    if (!isMidonaMessage()) {
        field_0x1a3 = 0;
    }
    if (mpScrnDraw->isSelect() || getStatusLocal() == 20) {
        mpRefer->selectMessage();
        if (dMeter2Info_isShopTalkFlag()) {
            dComIfGp_setAStatusForce(0x2a, 0);
        }
    }
    if (mDoCPd_c::getTrigA(0)) {
        if (getSelectCursorPosLocal() != 0xff) {
            field_0x1a3 = 1;
        }
    } else if (mDoCPd_c::getTrigB(0) &&
               (((mpScrnDraw->isSelect() || getStatusLocal() == 20) && getSelectCancelPos() != 0)))
    {
        setSelectCursorPosLocal(getSelectCancelPos() - 1);
        uVar7 = true;
        if (getStatusLocal() == 8) {
            if (getSelectCancelPos() <= 2) {
                if (getSelectCursorPosLocal() != 0xff) {
                    mpScrnDraw->selectAnimeMove(2, getSelectCursorPosLocal() + 1, 1);
                } else {
                    mpScrnDraw->selectAnimeMove(2, (int)getSelectCursorPosLocal(), 1);
                }
            }
        } else if (getStatusLocal() == 9 && getSelectCancelPos() <= 3) {
            mpScrnDraw->selectAnimeMove(3, getSelectCursorPosLocal(), 1);
        }
        field_0x1a3 = 2;
    }
    jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
    if (getStatusLocal() == 8) {
        if (isMidonaMessage() && field_0x1a3 != 0) {
            if (field_0x1a3 == 2 && getSelectCancelPos() == 3) {
                iVar8 = true;
            } else {
                while (!iVar8) {
                    if (getSelectCursorPosLocal() != 0xff) {
                        iVar8 =
                            mpScrnDraw->selectAnimeMove(2, getSelectCursorPosLocal() + 1, uVar7);
                    } else {
                        iVar8 =
                            mpScrnDraw->selectAnimeMove(2, (int)getSelectCursorPosLocal(), uVar7);
                    }
                }
            }
        } else if (field_0x1a3 == 2 && getSelectCancelPos() == 3) {
            iVar8 = true;
        } else if (getSelectCursorPosLocal() != 0xff) {
            iVar8 = mpScrnDraw->selectAnimeMove(2, getSelectCursorPosLocal() + 1, uVar7);
        } else {
            iVar8 = mpScrnDraw->selectAnimeMove(2, (int)getSelectCursorPosLocal(), uVar7);
        }
        pRef->setStopFlag(0);
    } else {
        if (getStatusLocal() == 9) {
            if (isMidonaMessage() && field_0x1a3 != 0) {
                if (field_0x1a3 == 2 && getSelectCancelPos() == 4) {
                    iVar8 = true;
                } else {
                    while (!iVar8) {
                        iVar8 = mpScrnDraw->selectAnimeMove(3, getSelectCursorPosLocal(), uVar7);
                    }
                }
            } else if (field_0x1a3 == 2 && getSelectCancelPos() == 4) {
                iVar8 = true;
            } else {
                iVar8 = mpScrnDraw->selectAnimeMove(3, getSelectCursorPosLocal(), uVar7);
            }
            pRef->setStopFlag(0);
        } else if (getStatusLocal() == 20) {
            iVar8 = true;
        }
    }
    field_0x100->select_idx = pRef->getSelectPos();
    if (isSend() && field_0x1a3 != 0 && iVar8) {
        field_0x1a3 = 0;
        if (mDoCPd_c::getTrigB(0)) {
            mSelectPushFlag = 2;
        } else {
            mSelectPushFlag = 1;
        }
        setSelectCancelPosLocal(0);
        field_0x16a = 0;
        if (getStatusLocal() == 20) {
            field_0x199 = 1;
            setStatusLocal(14);
        } else {
            setStatusLocal(24);
        }
        dMeter2Info_set2DVibration();
        mDoAud_seStart(daAlink_c::WANM_ATTACK_POE_END, 0, 0, 0);
    }
}

void dMsgObject_c::selectEndProc() {
    if (mpScrnDraw->selectAnimeEnd()) {
        field_0x199 = 1;
        setStatusLocal(14);
    }
}

void dMsgObject_c::inputProc() {
    dMeter2Info_onShopTalkFlag();
    mpRefer->inputNumber();
    if (isSend()) {
        field_0x199 = 0;
        if (mDoCPd_c::getTrigA(0)) {
                         /* dSv_event_tmp_flag_c::T_0080 - Kakariko Village - Put money in fundraiser box */
            BOOL iVar2 = dComIfGs_isTmpBit(dSv_event_tmp_flag_c::tempBitLabels[80]);
                                        /* dSv_event_flag_c::F_0802 - Faron Woods - Trill attacks when stealing */
            if (getInputValue() > 0 && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[802])) {
                onPaymentFlagLocal();
            }

            dComIfGp_setItemRupeeCount(-getInputValue());
            if (iVar2) {
                if (getInputValue() > 0) {
                    dMsgObject_addFundRaising(getInputValue());
                    getFundRaisingValue();
                    dMsgObject_getFundRaising();
                    if (dMsgObject_getFundRaising() >= getFundRaisingValue() &&
                         /* dSv_event_flag_c::M_091 - Kakariko Village - Buy out fundraiser amount (Malo becomes nice) */
                        !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[124]))
                    {
                             /* dSv_event_flag_c::F_0376 - Kakariko Village - Gathered funds for bridge repair! (set by program after raising funds) */
                        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[376])) {
                            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[376]);
                            dMsgObject_setFundRaising(0);
                        } else {
                            /* dSv_event_flag_c::M_091 - Kakariko Village - Buy out fundraiser amount (Malo becomes nice) */
                            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[124]);
                            dMsgObject_setFundRaising(0);
                        }
                    }
                } else {
                    /* dSv_event_tmp_flag_c::T_0080 - Kakariko Village - Put money in fundraiser box */
                    dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[80]);
                }
            } else {
                dMsgObject_addTotalPayment(getInputValue());
                fpcM_Search((fpcLyIt_JudgeFunc)dMsgObject_searchSSItem, this);
            }
            dMeter2Info_offShopTalkFlag();
            setStatusLocal(14);
        } else if (mDoCPd_c::getTrigB(0)) {
            /* dSv_event_tmp_flag_c::T_0080 - Kakariko Village - Put money in fundraiser box */
            dComIfGs_offTmpBit(dSv_event_tmp_flag_c::tempBitLabels[80]);
            dMeter2Info_offShopTalkFlag();
            setStatusLocal(14);
        }
    }
}

void dMsgObject_c::finishProc() {
    if (field_0x168 != 0x10 && mNoDemoFlag && isTalkMessage() && mpRefer->getForm() != 1) {
        mDoAud_seStart(daAlink_c::WANM_FALL_LAND, 0, 0, 0);
    }
    mpRefer->selectMessage();
    jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
    if (pRef->getSendFlag() != 1 && pRef->getSendFlag() != 3 && pRef->getSendFlag() != 4 &&
        pRef->getSendFlag() != 6)
    {
        mpScrnDraw->dotAnimeMove();
    }
    if (isHowlMessage() && ((dMsgScrnHowl_c*)mpScrnDraw)->isKeyCheck() && mDoCPd_c::getTrigB(0)) {
        dMsgObject_onMsgSend();
    }
    u8 sendRes = isSend();
    if (((mDoCPd_c::getTrigA(0) == 0 &&
         (mDoCPd_c::getTrigB(0) == 0 || ((dMsgScrnHowl_c*)mpScrnDraw)->isKeyCheck())) ||
        !isHowlMessage()) && sendRes != 0)
    {
        if (mpRefer->getMsgID() == 0xc4e) {
            Z2GetAudioMgr()->changeBgmStatus(2);
        }
        field_0x16a = 0;
        setStatusLocal(17);
        if (isTalkMessage() && sendRes == 2) {
            mDoAud_seStart(daAlink_c::WANM_ATTACK_POE_START, 0, 0, 0);
        }
    }
}

void dMsgObject_c::endProc() {
    field_0x16a++;
    s16 sVar4 = 5;
    if (isKanbanMessage() || isBookMessage()) {
        sVar4 = g_MsgObject_HIO_c.field_0x304;
    } else if (isPlaceMessage() || isStaffMessage()) {
        sVar4 = g_MsgObject_HIO_c.mStageTitleFadeOut;
    } else if (isBossMessage()) {
        sVar4 = g_MsgObject_HIO_c.mBossNameFadeOut;
    }
    f32 dVar6 = (f32)field_0x16a / sVar4;
    mpScrnDraw->fukiAlpha(1.0f - dVar6);
    if (isBookMessage()) {
        mpScrnDraw->fukiScale(1.0f - dVar6);
    }
    mpOutFont->setAlphaRatio(1.0f - dVar6);
    if (field_0x16a >= sVar4) {
        mpScrnDraw->arwAnimeInit();
        mpScrnDraw->dotAnimeInit();
        if (mNoDemoFlag && !field_0x4d4) {
            setStatusLocal(18);
        } else {
            setStatusLocal(19);
        }
        mMessageID = 0;
        field_0x158 = mMessageID;
        field_0x15c = 1000;
        field_0x172 = 0;
        field_0x199 = 0;
        mpRefer->setPageNum(field_0x172);
    }
}

void dMsgObject_c::deleteProc() {
    if (field_0x148 != NULL) {
        mDoExt_setCurrentHeap(field_0x148);
        field_0x148 = NULL;
    }
    delete_screen(true);
    mpCtrl->reset();
    mpCtrl->resetResourceCache();
    mpRefer->resetCharCountBuffer();
    field_0x17e = 0;
    updateEquipBombInfoLocal();
    dComIfGp_clearMesgAnimeTagInfo();
    dComIfGp_clearMesgCameraTagInfo();
    dComIfGp_clearMesgAnimeAttrInfo();
    dComIfGp_clearMesgFaceAnimeAttrInfo();
    dComIfGp_clearMesgCameraAttrInfo();
    dMsgObject_offCameraCancelFlag();
    dMsgObject_setMsgOutputType(0xff);
    offKillMessageFlagLocal();
    jmessage_tReference* pRef =
        (jmessage_tReference*)mpRenProc->getReference();
    if (pRef->getSendFlag() != 0) {
        pRef->setSendFlag(0);
        dMsgObject_offMsgSendControl();
        dMsgObject_offMsgSend();
    }
    if (pRef->getStopFlag() != 0) {
        pRef->setStopFlag(0);
    }
    if (mNoDemoFlag != 0) {
        mNoDemoFlag = 0;
    }
    if (field_0x4cc != 0) {
        field_0x4cc = 0;
    }
    if (field_0x4d4 != 0) {
        field_0x4d4 = 0;
    }
    dDemo_c::getControl()->unsuspend(1);
    if (field_0x4ce == 1) {
        mDoAud_talkOut();
        field_0x4ce = 0;
    }
    field_0x13c = NULL;
    mpTalkPartner = NULL;
    field_0x4d1 = 0;
    resetSelectBomBag();
    if (getArrowNumLocal() != 0) {
        dComIfGp_setItemArrowNumCount(getArrowNumLocal());
        setArrowNumLocal(0);
    }
    mFukiKind = 0xff;
    dMsgObject_setNowTalkFlowNo(0);
    setStatusLocal(1);
}

void dMsgObject_c::textmodeProc() {
}

void dMsgObject_c::talkStartInit() {
    f32 dVar19 = 0.0f;
    JUTFont* local_30 = mDoExt_getMesgFont();
    field_0x19b = 0;
    bool bVar1 = false;
    if (mFukiKind != mpRefer->getFukiKind()) {
        if (mpScrnDraw != NULL) {
            delete_screen(false);
            dVar19 = 1.0f;
            bVar1 = true;
        }
        mFukiKind = mpRefer->getFukiKind();
    }
    if (dComIfGp_isHeapLockFlag() == 8 ||
        (dComIfGp_isHeapLockFlag() == 5 && dMeter2Info_isFloatingMessageVisible() && !field_0x4cd))
    {
        dMeter2Info_getMeterClass()->emphasisButtonDelete();
    }
    if (((dComIfGp_isHeapLockFlag() == 0 || (dComIfGp_isHeapLockFlag() == 5 && mFukiKind != 15)) ||
         (((dComIfGp_isHeapLockFlag() == 2 ||
            (dComIfGp_isHeapLockFlag() == 3 || dComIfGp_isHeapLockFlag() == 1)) &&
           field_0x4cd == 0 && mpTalkHeap != NULL))) &&
        (mpScrnDraw == NULL && dMeter2Info_getFloatingMessageID() != 0x13eb))
    {
        if (!bVar1 && ((dComIfGp_isHeapLockFlag() == 0 || dComIfGp_isHeapLockFlag() == 5))) {
            dComIfGp_setHeapLockFlag(7);
        }
        if (mpTalkHeap == NULL) {
            mpTalkHeap = dComIfGp_getSubHeap2D(7);
        }
        mpTalkHeap->getTotalFreeSize();
        bool local_98 = false;
        if (mpOutFont == NULL) {
            OS_REPORT("free size (0x%x)=====> %d\n", mDoExt_getCurrentHeap(), mDoExt_getCurrentHeap()->getTotalFreeSize());
            mpOutFont = new COutFont_c(0);
            JUT_ASSERT(3035, mpOutFont != NULL);
            mpOutFont->createPane();
            mpRenProc->setOutFont(mpOutFont);
        }
        dMsgScrnBase_c* pData;
        jmessage_tReference* pRef;
        switch (mFukiKind) {
        case 9:
            pRef = (jmessage_tReference*)mpRenProc->getReference();
            pData = new dMsgScrnItem_c(pRef->getFukiPosType(), pRef->getForm(), mpTalkHeap);
            JUT_ASSERT(3049, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 2:
            if (mFukiKind == 15) {
                local_30 = mDoExt_getRubyFont();
            }
            pData = new dMsgScrnTree_c(local_30, mpTalkHeap);
            JUT_ASSERT(3061, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 6:
            pData = new dMsgScrnKanban_c(mpTalkHeap);
            JUT_ASSERT(3069, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 7:
            pData = new dMsgScrnStaff_c(((jmessage_tReference*)mpRenProc->getReference())->getArrange());
            JUT_ASSERT(3083, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 12:
            pData = new dMsgScrnPlace_c();
            JUT_ASSERT(3092, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 19:
            pData = new dMsgScrnBoss_c();
            JUT_ASSERT(3100, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 17:
            pData = new dMsgScrnHowl_c();
            JUT_ASSERT(3108, pData != NULL);
            mpScrnDraw = pData;
            local_98 = true;
            break;
        case 1:
        case 5:
            pRef = (jmessage_tReference*)mpRenProc->getReference();
            pData = new dMsgScrnJimaku_c(pRef->getForm(), mpTalkHeap);
            JUT_ASSERT(3119, pData != NULL);
            mpScrnDraw = pData;
            break;
        case 10:
            field_0x19b = 1;
        default:
            pRef = (jmessage_tReference*)mpRenProc->getReference();
            if (mpRefer->getMsgID() == 0x2a5) {
                pData = new dMsgScrnItem_c(0, pRef->getForm(), mpTalkHeap);
                JUT_ASSERT(3131, pData != NULL);
                mpScrnDraw = pData;
            } else {
                pData = new dMsgScrnTalk_c(pRef->getFukiPosType(), pRef->getForm(), mpTalkHeap);
                JUT_ASSERT(3138, pData != NULL);
                mpScrnDraw = pData;
                local_98 = true;
            }
            if (field_0x4ce == 0 && mFukiKind != 12) {
                mDoAud_talkIn();
                field_0x4ce = 1;
            }
            break;
        }
        if (mpRefer->getMsgID() == 0xc73) {
            Z2GetAudioMgr()->changeBgmStatus(1);
        }
        if (mpRefer->getMsgID() == 0xc46) {
            Z2GetAudioMgr()->changeBgmStatus(1);
        }
        mpScrnDraw->setMsgID( mpRefer->getMsgID());
        mpScrnDraw->setOutFont(mpOutFont);
        mpRefer->setFont(local_30);
        mpRefer->setActorPos(field_0x100->pos);
        mpRefer->setFontSizeX(mpScrnDraw->getFontSizeX());
        mpRefer->setFontSizeY(mpScrnDraw->getFontSizeY());
        mpRefer->setRubySize(mpScrnDraw->getRubySize());
        mpRefer->setTBoxWidth(mpScrnDraw->getTBoxWidth());
        mpRefer->setTBoxHeight(mpScrnDraw->getTBoxHeight());
        mpRefer->setLineSpace(mpScrnDraw->getLineSpace());
        mpRefer->setCharSpace(mpScrnDraw->getCharSpace());
        mpRefer->setRubyCharSpace(mpScrnDraw->getRubyCharSpace());
        mpRefer->setSelFontSize(mpScrnDraw->getSelFontSize());
        mpRefer->setSelRubySize(mpScrnDraw->getSelRubySize());
        mpRefer->setSelTBoxWidth(mpScrnDraw->getSelTBoxWidth());
        mpRefer->setSelCharSpace(mpScrnDraw->getSelCharSpace());
        mpRefer->setSelRubyCharSpace(mpScrnDraw->getSelRubyCharSpace());
        mpRenProc->setCharInfoPtr(mpScrnDraw->getCharInfoPtr());
        field_0x16a = 0;
        field_0x199 = 0;
        mpScrnDraw->fukiScale(dVar19);
        mpScrnDraw->fukiAlpha(dVar19);
        mpOutFont->setAlphaRatio(dVar19);
        fukiPosCalc(local_98);
        mpScrnDraw->arwAnimeInit();
        mpScrnDraw->dotAnimeInit();
        mpScrnDraw->setString("", "");
        mpScrnDraw->setRubyString("");
        mpScrnDraw->setSelectString("", "", "");
        mpScrnDraw->setSelectRubyString("", "", "");
    }
}

void dMsgObject_c::fukiPosCalc(bool param_1) {
    if (param_1) {
        mpRenProc->setTextInitOffsetPos(mpScrnDraw->getTextBoxPosOffsetY());
        mpScrnDraw->setTextBoxPosOffsetY(0.0f);
    } else {
        jmessage_tReference* pRef =
            (jmessage_tReference*)mpRenProc->getReference();
        u8 bVar4 = pRef->getFukiPosType();
        if (isKanbanMessage() || isBookMessage()) {
            bVar4 = 3;
        } else if (bVar4 == 3) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            cXyz local_3c;
            cXyz cStack_48;
            mDoLib_project(&player->eyePos, &cStack_48);
            f32 temp;
            if ((field_0x100->pos == cXyz(0.0f, 0.0f, 0.0f))) {
                temp = cStack_48.y;
            } else {
                mDoLib_project(&field_0x100->pos, &local_3c);
                if (local_3c.x >= 0.0f && local_3c.x <= FB_WIDTH && local_3c.y >= 0.0f &&
                    local_3c.y <= FB_HEIGHT)
                {
                    temp = 0.5f * (cStack_48.y + local_3c.y);
                } else {
                    temp = cStack_48.y;
                }
            }
            if (temp < g_MsgObject_HIO_c.mBoxAppearBound) {
                bVar4 = 0;
            } else {
                bVar4 = 1;
            }
        }
        switch(bVar4) {
        case 1:
            mpScrnDraw->setTextBoxPosOffsetY(-200.0f);
            break;
        case 2:
            mpScrnDraw->setTextBoxPosOffsetY(-100.0f);
            break;
        default:
            mpScrnDraw->setTextBoxPosOffsetY(0.0f);
            break;
        }
        mpRenProc->setTextInitOffsetPos(mpScrnDraw->getTextBoxPosOffsetY());
    }
}

void dMsgObject_c::setStatusLocal(u16 status) {
    ((jmessage_tReference*)mpRenProc->getReference())->setStatus(status);
}

u16 dMsgObject_c::getStatusLocal() {
    return ((jmessage_tReference*)mpRenProc->getReference())->getStatus();
}

void dMsgObject_c::delete_screen(bool param_1) {
    if (mpOutFont != NULL) {
        delete mpOutFont;
        mpOutFont = NULL;
    }
    if (mpScrnDraw != NULL) {
        delete mpScrnDraw;
        mpScrnDraw = NULL;
        if (param_1 && dComIfGp_isHeapLockFlag() == 5)
        {
            dComIfGp_offHeapLockFlag(7);
        }
    }
    if (param_1 && mpTalkHeap != NULL) {
        mpTalkHeap = NULL;
    }
}

u8 dMsgObject_c::isSend() {
    dComIfGp_setDoStatusForce(0, 0);
    u8 mesgCancelButton = dComIfGp_checkMesgCancelButton();
    if (mesgCancelButton) {
        dComIfGp_setMesgCancelButton(0);
    }
    if (mShopWaitTimer != 0) {
        return 0;
    }
    jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
    if (pRef->getSendFlag() != 0) {
        if (pRef->getSendFlag() == 3) {
            if (dMsgObject_isMsgSend()) {
                dMsgObject_offMsgSendControl();
                dMsgObject_offMsgSend();
                return 1;
            }
            dMsgObject_onMsgSendControl();
            return 0;
        }
        if (pRef->getSendFlag() == 4) {
            if (field_0x17e >= pRef->getDemoFrame()) {
                return 1;
            }
            return 0;
        }
        if (pRef->getSendFlag() == 5) {
            if (getStatusLocal() == 21) {
                setButtonStatusLocal();
                if (mDoCPd_c::getTrigA(0) != 0 || mDoCPd_c::getTrigB(0) != 0) {
                    return 2;
                }
                return 0;
            } else {
                return 1;
            }
        }
        if (pRef->getSendTimer() > 0) {
            pRef->decSendTimer();
        } else {
            if (pRef->getSendFlag() == 6) {
                pRef->setSendFlag(0);
            } else {
                return 1;
            }
        }
        if (pRef->getSendFlag() == 2) {
            setButtonStatusLocal();
            if (mDoCPd_c::getTrigA(0) != 0 || mDoCPd_c::getTrigB(0) != 0) {
                return 2;
            }
        }
    } else {
        setButtonStatusLocal();
        if (isHowlMessage()) {
            if (dMsgObject_isMsgSend()) {
                dMsgObject_offMsgSendControl();
                dMsgObject_offMsgSend();
                return 2;
            }
        } else {
            if (mDoCPd_c::getTrigA(0) != 0 || mDoCPd_c::getTrigB(0) != 0) {
                return 2;
            }
            if (mesgCancelButton) {
                return 1;
            }
        }
    }
    if (field_0x199 || field_0x1a3) {
        return 2;
    } else {
        return 0;
    }
}

void dMsgObject_c::readMessageGroupLocal(mDoDvdThd_mountXArchive_c** p_arcMount) {
    static char arcName[22];

    int msgGroup = dStage_stagInfo_GetMsgGroup(dComIfGp_getStage()->getStagInfo());
    #if REGION_PAL
    switch (dComIfGs_getPalLanguage()) {
    case dSv_player_config_c::LANGAUGE_GERMAN:
        sprintf(arcName, "/res/Msgde/bmgres%d.arc", msgGroup);
        break;
    case dSv_player_config_c::LANGAUGE_FRENCH:
        sprintf(arcName, "/res/Msgfr/bmgres%d.arc", msgGroup);
        break;
    case dSv_player_config_c::LANGAUGE_SPANISH:
        sprintf(arcName, "/res/Msgsp/bmgres%d.arc", msgGroup);
        break;
    case dSv_player_config_c::LANGAUGE_ITALIAN:
        sprintf(arcName, "/res/Msgit/bmgres%d.arc", msgGroup);
        break;
    default:
        sprintf(arcName, "/res/Msguk/bmgres%d.arc", msgGroup);
    }
    #elif REGION_JPN
    sprintf(arcName, "/res/Msgjp/bmgres%d.arc", msgGroup);
    #else
    sprintf(arcName, "/res/Msgus/bmgres%d.arc", msgGroup);
    #endif

    *p_arcMount = mDoDvdThd_mountXArchive_c::create(arcName, 0, JKRArchive::MOUNT_MEM, NULL);

    if (msgGroup == 99) {
        msgGroup = 9;
    }

    s_groupID = msgGroup;
}

void dMsgObject_c::changeFlowGroupLocal(s32 param_0) {
    mFlowChk = 1;
    changeGroup(param_0 >= 3000 ? (s16)0 : s_groupID);
}

void dMsgObject_c::demoMessageGroupLocal() {
    mFlowChk = 0;
    changeGroup(0);
}

void dMsgObject_c::endFlowGroupLocal() {
    mFlowChk = 0;
}

void dMsgObject_c::changeGroupLocal(s16 param_1) {
    JKRHeap* prevHeap = mDoExt_setCurrentHeap(dComIfGp_getMsgExpHeap());
    if (field_0x16c != param_1) {
        if (mFlowChk != 0) {
            JUT_ASSERT(3688, mFlowChk != 2);
            mFlowChk = 2;
        }
        OS_REPORT("group change =====> %d\n", param_1);
        if (param_1 >= 1) {
            OS_REPORT("bmg data change =====> %d --> %d\n", field_0x16c, param_1);
            if (field_0x16c == 0) {
                field_0x19d = 1;
            }
            mpMsgDt = dMeter2Info_getStageMsgResource();
        } else {
            mpMsgDt = mpMsgRes;
        }
        if (field_0x16c >= 0) {
            field_0x124->parse(mpMsgDt, 0x80);
        }
        field_0x16c = param_1;
    }
    mDoExt_setCurrentHeap(prevHeap);
}

bool dMsgObject_c::getStringLocal(u32 param_1, J2DTextBox* param_2, J2DTextBox* param_3,
                                  JUTFont* param_4, COutFont_c* param_5, char* param_6,
                                  char* param_7, char* param_8, s16* param_9) {
    if (field_0x4cd == 0) {
        if (getStatusLocal() == 1) {
            s16 groupID = getMessageGroup(param_1);
            JUT_ASSERT(3740, groupID==s_groupID || groupID == 0);
            changeGroup(groupID);
            mpSeqProc->setForceForm(1);
            mpRefer->setFont(param_4);
            mpRenProc->setOutFont(param_5);
            mpRefer->setActorPos(cXyz(0.0f, 0.0f, 0.0f));
            J2DTextBox::TFontSize fontSize;
            param_2->getFontSize(fontSize);
            mpRefer->setFontSizeX(fontSize.mSizeX);
            mpRefer->setFontSizeY(fontSize.mSizeY);
            if (param_3 != NULL) {
                param_3->getFontSize(fontSize);
                mpRefer->setRubySize(fontSize.mSizeX);
            } else {
                mpRefer->setRubySize(0.0f);
            }
            mpRefer->setTBoxWidth(param_2->getWidth());
            mpRefer->setTBoxHeight(param_2->getHeight());
            mpRefer->setLineSpace(param_2->getLineSpace());
            mpRefer->setCharSpace(param_2->getCharSpace());
            if (param_3 != NULL) {
                mpRefer->setRubyCharSpace(param_3->getCharSpace());
            } else {
                mpRefer->setRubyCharSpace(0.0f);
            }
            mpRenProc->setCharInfoPtr(NULL);
            field_0x4cd = 1;
            mpCtrl->setMessageID(param_1, 0, NULL);
        }
    } else {
        jmessage_tReference* pRef =
            (jmessage_tReference*)mpRenProc->getReference();
        if (getStatusLocal() == 11 && (pRef->getStopFlag() == 7 || pRef->getStopFlag() == 1))
        {
            mpCtrl->render();
            if (param_9 != NULL) {
                *param_9 = mpRefer->getCharSoundInfo().field_0x40e;
            }
            if (param_6 != NULL) {
                strcpy(param_6, pRef->getTextPtr());
            }
            if (param_7 != NULL) {
                strcpy(param_7, pRef->getRubyPtr());
            }
            if (param_8 != NULL) {
                strcpy(param_8, pRef->getTextSPtr());
            }
            mpCtrl->reset();
            mpCtrl->resetResourceCache();
            mpRefer->resetCharCountBuffer();
            dDemo_c::getControl()->unsuspend(1);
            field_0x17e = 0;
            updateEquipBombInfoLocal();
            field_0x4cd = 0;
            pRef->setStopFlag(0);
            setStatusLocal(1);
            return true;
        }
    }
    return false;
}

u8 dMsgObject_c::isGetItemMessage() {
    return mFukiKind == 9;
}

bool dMsgObject_c::isKanbanMessage() {
    if (mFukiKind == 2 || mFukiKind == 6 || mFukiKind == 15) {
        return true;
    }
    return false;
}

bool dMsgObject_c::isHowlMessage() {
    if (mFukiKind == 17) {
        return true;
    }
    return false;
}

bool dMsgObject_c::isMidonaMessage() {
    if (mFukiKind == 13 && (mpRefer->getMsgID() == 0x7d3 || mpRefer->getMsgID() == 0x7f6 ||
                            mpRefer->getMsgID() == 0x7fa))
    {
        return 1;
    }
    return 0;
}

bool dMsgObject_c::isMidonaNormalTalkWait() {
    if (mFukiKind == 13 && !isMidonaMessage()) {
        daMidna_c* midna = daPy_py_c::getMidnaActor();
        if (midna != NULL && !midna->checkShadowModeTalkWait()) {
            return true;
        } else {
            return false;
        }
        return false;
    }
    return true;
}

bool dMsgObject_c::isMidonaTalkWait() {
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    if (midna != NULL) {
        return midna->checkShadowModeTalkWait();
    }
    return false;
}

bool dMsgObject_c::isPlaceMessage() {
    return mFukiKind == 12 ? 1 : 0;
}

bool dMsgObject_c::isBossMessage() {
    return mFukiKind == 19 ? 1 : 0;
}

bool dMsgObject_c::isBookMessage() {
    return false;
}

bool dMsgObject_c::isStaffMessage() {
    return mFukiKind == 7 ? 1 : 0;
}

bool dMsgObject_c::isSaveMessage() {
    return mFukiKind == 16 ? 1 : 0;
}

bool dMsgObject_c::isTalkMessage() {
    if (isKanbanMessage() || isPlaceMessage() || isBossMessage() || isBookMessage() ||
        isHowlMessage() || isStaffMessage()) {
        return false;
    }
    return true;
}

const char* dMsgObject_c::getSmellName() {
    JMSMesgInfo_c* info_header_p = (JMSMesgInfo_c*)((char*)mpMsgRes + 0x20);
    char* data_ptr = (char*)info_header_p + info_header_p->header.size;
    JMSMesgEntry_c* info_entries = (JMSMesgEntry_c*)((char*)info_header_p + 0x10);

    JMSMesgEntry_c* msg_entry = &info_entries[(u16)getMessageIndex(getSmellTypeMessageID())];
    return data_ptr + msg_entry->string_offset + 8;
}

const char* dMsgObject_c::getPortalName() {
    JMSMesgInfo_c* info_header_p = (JMSMesgInfo_c*)((char*)mpMsgRes + 0x20);
    char* data_ptr = (char*)info_header_p + info_header_p->header.size;
    JMSMesgEntry_c* info_entries = (JMSMesgEntry_c*)((char*)info_header_p + 0x10);

    JMSMesgEntry_c* msg_entry = &info_entries[getMessageIndex(getPortalMessageID())];
    return data_ptr + msg_entry->string_offset + 8;
}

const char* dMsgObject_c::getBombName() {
    u8 i_selectPos = getSelectCursorPos();
    JUT_ASSERT(4075, i_selectPos >= 0 && i_selectPos < dSv_player_item_c::BOMB_BAG_MAX);
    u8 i_cursorPos;
    if (field_0x193 != 0) {
        i_cursorPos = getSelectBomBag(i_selectPos);
    } else {
        i_cursorPos = i_selectPos;
    }
    JUT_ASSERT(4083, i_cursorPos >= 0 && i_cursorPos < dSv_player_item_c::BOMB_BAG_MAX);
    JMSMesgInfo_c* info_header_p = (JMSMesgInfo_c*)((char*)mpMsgRes + 0x20);
    char* data_ptr = (char*)info_header_p + info_header_p->header.size;
    JMSMesgEntry_c* info_entries = (JMSMesgEntry_c*)((char*)info_header_p + 0x10);

    JMSMesgEntry_c* msg_entry = &info_entries[getMessageIndexAlways(getBombMessageIDLocal(i_cursorPos) + 0x165)];
    return data_ptr + msg_entry->string_offset + 8;
}

const char* dMsgObject_c::getInsectName() {
    JMSMesgInfo_c* info_header_p = (JMSMesgInfo_c*)((char*)mpMsgRes + 0x20);
    char* data_ptr = (char*)info_header_p + info_header_p->header.size;
    JMSMesgEntry_c* info_entries = (JMSMesgEntry_c*)((char*)info_header_p + 0x10);

    JMSMesgEntry_c* msg_entry = &info_entries[getMessageIndexAlways(getInsectItemNoLocal() + 0x165)];
    return data_ptr + msg_entry->string_offset + 8;
}

const char* dMsgObject_c::getLetterName() {
    JMSMesgInfo_c* info_header_p = (JMSMesgInfo_c*)((char*)mpMsgRes + 0x20);
    char* data_ptr = (char*)info_header_p + info_header_p->header.size;
    JMSMesgEntry_c* info_entries = (JMSMesgEntry_c*)((char*)info_header_p + 0x10);

    JMSMesgEntry_c* msg_entry = &info_entries[getMessageIndexAlways(getLetterNameIDLocal())];
    return data_ptr + msg_entry->string_offset + 8;
}

u8 dMsgObject_c::getSelectBombNum() {
    u8 i_selectPos = getSelectCursorPos();
    JUT_ASSERT(4133, i_selectPos >= 0 && i_selectPos < dSv_player_item_c::BOMB_BAG_MAX)
    if (field_0x193 != 0) {
        i_selectPos = getSelectBomBag(i_selectPos);
    }
    return getBombNumLocal(i_selectPos);
}

void dMsgObject_c::onMsgSendControlLocal() {
    mMsgSendControl |= 1;
}

void dMsgObject_c::offMsgSendControlLocal() {
    mMsgSendControl &= ~1;
    ((jmessage_tReference*)mpRenProc->getReference())->setSendFlag(0);
}

int dMsgObject_c::isMsgSendControlLocal() {
    return (mMsgSendControl & 1) != 0;
}

void dMsgObject_c::onMsgSendLocal() {
    mMsgSendControl |= 2;
}

void dMsgObject_c::offMsgSendLocal() {
    mMsgSendControl &= ~2;
}

int dMsgObject_c::isMsgSendLocal() {
    return (mMsgSendControl & 2) != 0;
}

bool dMsgObject_c::isMouthCheckLocal() {
    return mpSeqProc->getMouthCheck() != 0;
}

int dMsgObject_c::getBombBuyPriceLocal(u8 param_0) {
    switch(param_0) {
    case 0x70:
        return 15;
    case 0x71:
        return 30;
    case 0x72:
        return 30;
    default:
        return 0;
    }
}

void dMsgObject_c::addSelectBomBag(u8 param_1) {
    if (param_1 == 0xff) {
        param_1 = field_0x193;
    }
    mSelectBombBag[field_0x193] = param_1;
    field_0x193++;
    OS_REPORT("addSelectBombBag ==> %d, %d\n", field_0x193, param_1);
}

void dMsgObject_c::resetSelectBomBag() {
    for (int i = 0; i < 3; i++) {
        mSelectBombBag[i] = 0;
    }
    field_0x193 = 0;
}

u8 dMsgObject_c::getSelectBombBagIDLocal() {
    u8 i_selectPos = getSelectCursorPos();
    JUT_ASSERT(4253, i_selectPos >= 0 && i_selectPos < dSv_player_item_c::BOMB_BAG_MAX);
    if (field_0x193 != 0) {
        i_selectPos = getSelectBomBag(i_selectPos);
    }
    return getBombBagIDLocal(i_selectPos);
}

s16 dMsgObject_c::getSelectBombPriceLocal() {
    u8 i_selectPos = getSelectCursorPos();
    JUT_ASSERT(4276, i_selectPos >= 0 && i_selectPos < dSv_player_item_c::BOMB_BAG_MAX);
    u8 i_cursorPos = field_0x193 != 0 ? getSelectBomBag(i_selectPos) : i_selectPos++;
    u16 uVar1 = getBombBuyPriceLocal(getBombMessageIDLocal(i_cursorPos));
    int num = getSelectBombNum();
    return (num * uVar1) / 10.0f;
}

void dMsgObject_c::setEquipBombInfoLocal() {
    mEquipBombInfo = 1;
}

void dMsgObject_c::updateEquipBombInfoLocal() {
    if (mEquipBombInfo == 0) {
        return;
    }
    bool bVar1 = true;
    if (s_groupID == 2 && (mNowTalkFlowNo == 365 || mNowTalkFlowNo == 366 || mNowTalkFlowNo == 367))
    {
        bVar1 = false;
    }
    int iVar5 = 0;
    for (int i = 0; (int)i < 3; i++) {
        setBombBagIDLocal(i, 0xff);
        setBombMessageIDLocal(i, 0);
        setBombNumLocal(i, 0);
        u8 bVar3 = dComIfGs_getItem(i + 15, 0);
        if (bVar3 != 0xff && (bVar3 != 0x50 || bVar1)) {
            setBombBagIDLocal(iVar5, i);
            setBombMessageIDLocal(iVar5, bVar3);
            if (bVar3 == 0x50) {
                setBombNumLocal(iVar5, 0);
            } else {
                setBombNumLocal(iVar5, dComIfGs_getBombNum(i));
            }
            iVar5++;
        }
    }
    if (iVar5 == 1) {
        for (int i = 1; i < 3; i++) {
            setBombBagIDLocal(i, getBombBagIDLocal(0));
            setBombMessageIDLocal(i, getBombMessageIDLocal(0));
            setBombNumLocal(i, getBombNumLocal(0));
        }
    }
    mEquipBombInfo = 0;
}

void dMsgObject_c::setShopWaitTimer(u8 timer) {
    dMsgObject_getMsgObjectClass()->setShopWaitTimerLocal(timer);
}

void dMsgObject_c::setSelectWordFlagLocal(u8 param_0) {
    mSelectWordFlag = param_0;
}

bool dMsgObject_c::isHowlHearingModeLocal() {
    bool rv = false;
    if (isHowlMessage() && mpScrnDraw != NULL && ((dMsgScrnHowl_c*)mpScrnDraw)->isHearingMode()) {
        rv = true;
    }
    return rv;
}

bool dMsgObject_c::isCameraCancelFlag() {
    return dMsgObject_getMsgObjectClass()->isCameraCancelFlagLocal() ? true : false;
}

void dMsgObject_c::onCameraCancelFlag() {
    dMsgObject_getMsgObjectClass()->onCameraCancelFlagLocal();
}

void dMsgObject_c::offCameraCancelFlag() {
    dMsgObject_getMsgObjectClass()->offCameraCancelFlagLocal();
}

bool dMsgObject_c::isKillMessageFlag() {
    return dMsgObject_getMsgObjectClass()->isKillMessageFlagLocal();
}

void dMsgObject_c::onKillMessageFlag() {
    dMsgObject_getMsgObjectClass()->onKillMessageFlagLocal();
}

void dMsgObject_c::setKillMessageFlag() {
    dMsgObject_getMsgObjectClass()->setKillMessageFlagLocal();
}

void dMsgObject_c::setKillMessageFlagLocal() {
    if (!dComIfGp_event_runCheck()) {
        if (getStatusLocal() != 1) {
            if (getStatusLocal() == 11) {
                field_0x4cd = 0;
                dMeter2Info_resetFloatingMessage();
            }
            deleteProc();
            jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
            pRef->setStopFlag(0);
        } else {
            mNoDemoFlag = 0;
            field_0x4cc = 0;
        }
    }
}

void dMsgObject_c::setTalkPartner(fopAc_ac_c* param_0) {
    dMsgObject_getMsgObjectClass()->setTalkPartnerLocal(param_0);
}

void dMsgObject_c::setNowTalkFlowNo(s16 param_0) {
    dMsgObject_getMsgObjectClass()->setNowTalkFlowNoLocal(param_0);
}

s16 dMsgObject_c::getNowTalkFlowNo() {
    return dMsgObject_getMsgObjectClass()->getNowTalkFlowNoLocal();
}

void dMsgObject_c::setDemoMessage(u32 param_0) {
    dMsgObject_getMsgObjectClass()->setDemoMessageLocal(param_0);
}

void dMsgObject_c::setTalkHeap(void* param_0) {
    dMsgObject_getMsgObjectClass()->setTalkHeapLocal(param_0);
}

void dMsgObject_c::setTalkHeapLocal(void* param_1) {
    if (param_1 == NULL) {
        delete_screen(true);
    }
    mpTalkHeap = (JKRExpHeap*)param_1;
}

void* dMsgObject_c::getTalkHeap() {
    return dMsgObject_getMsgObjectClass()->getTalkHeapLocal();
}

bool dMsgObject_c::isDraw() {
    if ((getStatusLocal() != 0 && getStatusLocal() != 1 && field_0x4c7 == 0) &&
        (getStatusLocal() != 11 && (getStatusLocal() != 6 || field_0x166 != 15)))
    {
        return true;
    }
    return false;
}

void dMsgObject_c::setButtonStatusLocal() {
    jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
    if (field_0x168 == getStatusLocal() && (pRef->getSendFlag() != 1 && pRef->getSendFlag() != 3) &&
        pRef->getSendFlag() != 4 && pRef->getSendFlag() != 6 && pRef->getSendFlag() != 2)
    {
        if (getStatusLocal() == 0x10) {
            if (isHowlMessage()) {
                dComIfGp_setDoStatusForce(5, 0);
                dComIfGp_setAStatusForce(42, 0);
            } else {
                dComIfGp_setDoStatusForce(35, 0);
            }
        } else if (getStatusLocal() == 8 || getStatusLocal() == 9 ||
            getStatusLocal() == 20)
        {
            dComIfGp_setDoStatusForce(0x22, 0);
        } else if (getStatusLocal() == 21) {
            dComIfGp_setDoStatusForce(0x22, 0);
            dComIfGp_setAStatusForce(0x2a, 0);
        } else {
            dComIfGp_setDoStatusForce(0x23, 0);
        }
    }
}

void dMsgObject_c::setDemoMessageLocal(u32 param_1) {
    field_0x4cc = 1;
    dMsgObject_onCameraCancelFlag();
    fopMsgM_messageSet(dMsgObject_getMsgObjectClass()->getMessageIDAlways(param_1), 1000);
}

u16 dMsgObject_c::getSmellTypeMessageIDLocal() {
    u16 msgId = 0;
    int smell = dComIfGs_getCollectSmell();
    if (smell < fpcNm_ITEM_SMELL_MEDICINE + 1 && smell >= fpcNm_ITEM_SMELL_YELIA_POUCH) {
        msgId = smell + 0x165;
    } else {
        if (dComIfGs_getCollectSmell() != -1) {
            OS_REPORT("smell type ====> %d\n", dComIfGs_getCollectSmell());
            JUT_WARN(4858, "smell type no entry!");
        }
    }
    return msgId;
}

void dMsgObject_c::setSmellTypeLocal(u8 smellType) {
    mSmellType = smellType;
    dComIfGs_onItemFirstBit(smellType);
    dComIfGs_setCollectSmell(mSmellType);
    dComIfGs_setSelectItemIndex(2, mSmellType);
}

u8 dMsgObject_c::getSelectCursorPosLocal() {
    return ((jmessage_tReference*)mpRenProc->getReference())->getSelectPos();
}

void dMsgObject_c::setSelectCursorPosLocal(u8 param_1) {
    jmessage_tReference* pRef = (jmessage_tReference*)mpRenProc->getReference();
    if (!dComIfGp_isPauseFlag() && param_1 != 0xff && pRef->getSelectPos() != param_1) {
        Z2GetAudioMgr()->seStart(daAlink_c::WANM_ATTACK_POE, NULL, 1, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                 0);
    }
    pRef->setSelectPos(param_1);
}

void dMsgObject_c::setTalkActorLocal(fopAc_ac_c* actor) {
    mpTalkActor = actor;
}

static int dMsgObject_Draw(dMsgObject_c* i_this) {
    return i_this->_draw();
}

static int dMsgObject_Execute(dMsgObject_c* i_this) {
    return i_this->_execute();
}

static int dMsgObject_IsDelete(dMsgObject_c* i_this) {
    return 1;
}

static int dMsgObject_Delete(dMsgObject_c* i_this) {
    return i_this->_delete();
}

int dMsgObject_Create(msg_class* param_1) {
    dMsgObject_c* obj = static_cast<dMsgObject_c*>(param_1);
    dComIfGp_setMsgObjectClass(obj);
    JKRHeap* prevHeap = mDoExt_setCurrentHeap(dComIfGp_getMsgExpHeap());
    dComIfGp_getMsgExpHeap()->getTotalFreeSize();
    fopMsgM_setStageLayer(param_1);
    param_1->mode = 0;
    int rv = obj->_create(param_1);
    g_MsgObject_HIO_c.field_0x4 = -1;
    OS_REPORT("### msg object size =====> %d\n", dComIfGp_getMsgExpHeap()->getTotalFreeSize());
    mDoExt_setCurrentHeap(prevHeap);
    return rv;
}

void dMsgObject_c::readMessageGroup(mDoDvdThd_mountXArchive_c** p_arcMount) {
    dMsgObject_getMsgObjectClass()->readMessageGroupLocal(p_arcMount);
}

void dMsgObject_c::changeFlowGroup(s32 param_0) {
    dMsgObject_getMsgObjectClass()->changeFlowGroupLocal(param_0);
}

void dMsgObject_c::demoMessageGroup() {
    dMsgObject_getMsgObjectClass()->demoMessageGroupLocal();
}

void dMsgObject_c::endFlowGroup() {
    dMsgObject_getMsgObjectClass()->endFlowGroupLocal();
}

void dMsgObject_c::changeGroup(s16 param_0) {
    dMsgObject_getMsgObjectClass()->changeGroupLocal(param_0);
}

bool dMsgObject_c::getString(u32 param_0, J2DTextBox* param_1, J2DTextBox* param_2,
                             JUTFont* param_3, COutFont_c* param_4, char* param_5, char* param_6,
                             char* param_7, s16* param_8) {
    return dMsgObject_getMsgObjectClass()->getStringLocal(
        param_0, param_1, param_2, param_3, param_4, param_5, param_6, param_7, param_8);
}

void* dMsgObject_c::getMsgDtPtr() {
    return dMsgObject_getMsgObjectClass()->getMsgDtPtrLocal();
}

void dMsgObject_c::setProcessID(fpc_ProcID id) {
    dMsgObject_getMsgObjectClass()->setProcessIDLocal(id);
}

msg_class* dMsgObject_c::getActor() {
    if (dMsgObject_getMsgObjectClass() == NULL) {
        return NULL;
    }
    return dMsgObject_getMsgObjectClass()->getActorLocal();
}

fopAc_ac_c* dMsgObject_c::getpTalkActor() {
    return dMsgObject_getMsgObjectClass()->getpTalkActorLocal();
}

int dMsgObject_c::getIdx() {
    return dMsgObject_getMsgObjectClass()->getIdxLocal();
}

u16 dMsgObject_c::getNodeIdx() {
    return dMsgObject_getMsgObjectClass()->getNodeIdxLocal();
}

void dMsgObject_c::setStatus(u16 status) {
    dMsgObject_getMsgObjectClass()->setStatusLocal(status);
}

u16 dMsgObject_c::getStatus() {
    return dMsgObject_getMsgObjectClass()->getStatusLocal();
}

dMsgScrnBase_c* dMsgObject_c::getScrnDrawPtr() {
    return dMsgObject_getMsgObjectClass()->getScrnDrawPtrLocal();
}

void dMsgObject_c::setTalkActor(fopAc_ac_c* actor) {
    dMsgObject_getMsgObjectClass()->setTalkActorLocal(actor);
}

void dMsgObject_c::onMsgSendControl() {
    dMsgObject_getMsgObjectClass()->onMsgSendControlLocal();
}

void dMsgObject_c::offMsgSendControl() {
    dMsgObject_getMsgObjectClass()->offMsgSendControlLocal();
}

int dMsgObject_c::isMsgSendControl() {
    return dMsgObject_getMsgObjectClass()->isMsgSendControlLocal();
}

void dMsgObject_c::onMsgSend() {
    dMsgObject_getMsgObjectClass()->onMsgSendLocal();
}

void dMsgObject_c::offMsgSend() {
    dMsgObject_getMsgObjectClass()->offMsgSendLocal();
}

int dMsgObject_c::isMsgSend() {
    return dMsgObject_getMsgObjectClass()->isMsgSendLocal();
}

bool dMsgObject_c::isMouthCheck() {
    return dMsgObject_getMsgObjectClass()->isMouthCheckLocal();
}

u32 dMsgObject_c::getMessageID() {
    return dMsgObject_getMsgObjectClass()->getMessageIDLocal();
}

u16 dMsgObject_c::getSmellTypeMessageID() {
    return dMsgObject_getMsgObjectClass()->getSmellTypeMessageIDLocal();
}

void dMsgObject_c::setSmellType(u8 smellType) {
    dMsgObject_getMsgObjectClass()->setSmellTypeLocal(smellType);
}

u8 dMsgObject_c::getSelectCursorPos() {
    return dMsgObject_getMsgObjectClass()->getSelectCursorPosLocal();
}

void dMsgObject_c::setSelectCursorPos(u8 pos) {
    dMsgObject_getMsgObjectClass()->setSelectCursorPosLocal(pos);
}

void dMsgObject_c::setPortalMessageID(u16 msgID) {
    dMsgObject_getMsgObjectClass()->setPortalMessageIDLocal(msgID);
}

void dMsgObject_c::setInsectItemNo(u8 itemNo) {
    dMsgObject_getMsgObjectClass()->setInsectItemNoLocal(itemNo);
}

void dMsgObject_c::setLetterNameID(u16 nameID) {
    dMsgObject_getMsgObjectClass()->setLetterNameIDLocal(nameID);
}

void dMsgObject_c::setArrowNum(u8 arrowNum) {
    dMsgObject_getMsgObjectClass()->setArrowNumLocal(arrowNum);
}

void dMsgObject_c::setMsgOutputType(u8 outputType) {
    dMsgObject_getMsgObjectClass()->setMsgOutputTypeLocal(outputType);
}

u8 dMsgObject_c::getMsgOutputType() {
    return dMsgObject_getMsgObjectClass()->getMsgOutputTypeLocal();
}

const char* dMsgObject_c::getWord() {
    return dMsgObject_getMsgObjectClass()->getWordLocal();
}

const char* dMsgObject_c::getSelectWord(int idx) {
    return dMsgObject_getMsgObjectClass()->getSelectWordLocal(idx);
}

void dMsgObject_c::setSelectWordFlag(u8 flag) {
    dMsgObject_getMsgObjectClass()->setSelectWordFlagLocal(flag);
}

u8 dMsgObject_c::getSelectWordFlag() {
    return dMsgObject_getMsgObjectClass()->getSelectWordFlagLocal();
}

bool dMsgObject_c::isHowlHearingMode() {
    return dMsgObject_getMsgObjectClass()->isHowlHearingModeLocal();
}

u8 dMsgObject_c::getSelectBombBagID() {
    return dMsgObject_getMsgObjectClass()->getSelectBombBagIDLocal();
}

s16 dMsgObject_c::getSelectBombPrice() {
    return dMsgObject_getMsgObjectClass()->getSelectBombPriceLocal();
}

void dMsgObject_c::setEquipBombInfo() {
    dMsgObject_getMsgObjectClass()->setEquipBombInfoLocal();
}

u8 dMsgObject_c::getItemEquipButton() {
    return dMsgObject_getMsgObjectClass()->getItemEquipButtonLocal();
}

void dMsgObject_c::setSelectCancelPos(u8 pos) {
    dMsgObject_getMsgObjectClass()->setSelectCancelPosLocal(pos);
}

u16 dMsgObject_getTotalPrice() {
    u8 high = dComIfGs_getEventReg(0xfeff);
    u8 low = dComIfGs_getEventReg(0xfdff);
    return (high << 8) | low;
}

void dMsgObject_setTotalPrice(u16 price) {
    dComIfGs_setEventReg(0xfeff, (price & 0xff00) >> 8);
    dComIfGs_setEventReg(0xfdff, price & 0xff);
}

void dMsgObject_addTotalPrice(s16 price) {
    dMsgObject_getMsgObjectClass()->addNowTotalPrice(price);
    price += dMsgObject_getTotalPrice();
    if (price < 0) {
        price = 0;
    }
    dComIfGs_setEventReg(0xfeff, ((u16)price & 0xff00) >> 8);
    dComIfGs_setEventReg(0xfdff, (u16)price & 0xff);
}

u16 dMsgObject_getTotalPayment() {
    u8 high = dComIfGs_getEventReg(0xfcff);
    u8 low = dComIfGs_getEventReg(0xfbff);
    return (high << 8) | low;
}

void dMsgObject_setTotalPayment(u16 price) {
    dComIfGs_setEventReg(0xfcff, (price & 0xff00) >> 8);
    dComIfGs_setEventReg(0xfbff, price & 0xff);
}

void dMsgObject_addTotalPayment(s16 price) {
    dMsgObject_getMsgObjectClass()->addNowTotalPayment(price);
    price += dMsgObject_getTotalPayment();
    if (price < 0) {
        price = 0;
    }
    dComIfGs_setEventReg(0xfcff, ((u16)price & 0xff00) >> 8);
    dComIfGs_setEventReg(0xfbff, (u16)price & 0xff);
}

u16 dMsgObject_getFundRaising() {
    u8 high = dComIfGs_getEventReg(0xf9ff);
    u8 low = dComIfGs_getEventReg(0xfaff);
    return (high << 8) | low;
}

void dMsgObject_setFundRaising(u16 price) {
    dComIfGs_setEventReg(0xf9ff, (price & 0xff00) >> 8);
    dComIfGs_setEventReg(0xfaff, price & 0xff);
}

void dMsgObject_addFundRaising(s16 price) {
    price += dMsgObject_getFundRaising();
    if (price < 0) {
        price = 0;
    }
    dComIfGs_setEventReg(0xf9ff, ((u16)price & 0xff00) >> 8);
    dComIfGs_setEventReg(0xfaff, (u16)price & 0xff);
}

u16 dMsgObject_getOffering() {
    u8 high = dComIfGs_getEventReg(0xf7ff);
    u8 low = dComIfGs_getEventReg(0xf8ff);
    return (high << 8) | low;
}

void dMsgObject_addOffering(s16 price) {
    s16 offering = price + dMsgObject_getOffering();
    if (offering < 0) {
        offering = 0;
    }
    dComIfGs_setEventReg(0xf7ff, ((u16)offering & 0xff00) >> 8);
    dComIfGs_setEventReg(0xf8ff, (u16)offering & 0xff);
}

static leafdraw_method_class l_dMsgObject_Method = {
    (process_method_func)dMsgObject_Create,
    (process_method_func)dMsgObject_Delete,
    (process_method_func)dMsgObject_Execute,
    (process_method_func)dMsgObject_IsDelete,
    (process_method_func)dMsgObject_Draw,
};

extern msg_process_profile_definition g_profile_MSG_OBJECT = {
  fpcLy_CURRENT_e,        // mLayerID
  12,                     // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_MSG_OBJECT,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(dMsgObject_c),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopMsg_Method,       // sub_method
  770,                    // mPriority
  &l_dMsgObject_Method,   // sub_method
};
