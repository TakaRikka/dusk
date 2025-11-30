//
// Translation Unit: msg/scrn/d_msg_scrn_talk
//

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_talk.h"
#include "d/d_msg_scrn_light.h"
#include "d/d_msg_scrn_arrow.h"
#include "d/d_msg_scrn_3select.h"
#include "d/d_msg_object.h"
#include "d/d_msg_class.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter2_info.h"
#include "d/d_pane_class.h"
#include "m_Do/m_Do_lib.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"

dMsgScrnTalk_c::dMsgScrnTalk_c(u8 param_1, u8 param_2, JKRExpHeap* param_3) {
    if (param_3 != NULL) {
        field_0xe4 = param_3;
    } else {
        field_0xe4 = dComIfGp_getSubHeap2D(7);
    }
    init();
    mpScreen = new J2DScreen();
    JUT_ASSERT(70, mpScreen != NULL);
    bool fg = mpScreen->setPriority("zelda_message_window_new.blo", 0x20000,
                                    dComIfGp_getMsgArchive(1));
    JUT_ASSERT(77, fg != false);
    dPaneClass_showNullPane(mpScreen);
    mpTxScreen = new J2DScreen();
    JUT_ASSERT(81, mpTxScreen != NULL);
    if (dMsgObject_getMsgObjectClass()->getFukiKind() == 8) {
        fg = mpTxScreen->setPriority("zelda_message_window_text_seirei.blo", 0x20000,
                                     dComIfGp_getMsgCommonArchive());
    } else {
        fg = mpTxScreen->setPriority("zelda_message_window_text.blo", 0x20000,
                                     dComIfGp_getMsgCommonArchive());
    }
    JUT_ASSERT(90, fg != false);
    dPaneClass_showNullPane(mpTxScreen);
    mpArrow_c = new dMsgScrnArrow_c();
    JUT_ASSERT(95, mpArrow_c != NULL);
    if (dComIfGp_isHeapLockFlag() != 3) {
        mpSelect_c = new dMsgScrn3Select_c();
        JUT_ASSERT(100, mpSelect_c != NULL);
    } else {
        mpSelect_c = NULL;
    }
    u8 uVar22;
    switch(dMsgObject_getMsgObjectClass()->getFukiKind()) {
    case 8:
        uVar22 = 2;
        break;
    case 13:
        uVar22 = 1;
        break;
    case 14:
        uVar22 = 4;
        break;
    default:
        uVar22 = 0;
        break;
    }

    mpLight_c = new dMsgScrnLight_c(uVar22, param_2);
    JUT_ASSERT(123, mpLight_c != NULL);
    void* mpBuf = field_0xe4->alloc(0x106a, 0x20);
    JUT_ASSERT(126, mpBuf != NULL);
    memset(mpBuf, 0, 0x106a);
    mCharInfoPtr = (CharInfo_c*)mpBuf;
    for (int i = 0; i < 150; i++) {
        field_0xf8[i] = 18.0f - g_MsgObject_HIO_c.mPikariScale;
        field_0x35c[i] = g_MsgObject_HIO_c.mPikariHaloDelay_spirit;
    }
    field_0xe8 = mpScreen->search('n_all')->getBounds().i.x;
    field_0xec = mpScreen->search('n_all')->getBounds().i.y;
    field_0xf0 = 0.0f;
    for (int i = 0; i < 3; i++) {
        field_0x350[i] = g_MsgObject_HIO_c.mBoxPos[i][0];
    }

    mpScreen->search('mg_null')->move(
        mpScreen->search('mg_null')->getBounds().i.x + g_MsgObject_HIO_c.mTextPosX, 
        mpScreen->search('mg_null')->getBounds().i.y + g_MsgObject_HIO_c.mTextPosY);
    mpArw_c = new CPaneMgr(mpScreen, 'set_ya_n', 0, NULL);
    JUT_ASSERT(147, mpArw_c != NULL);
    mpMg_c[0] = new CPaneMgr(mpScreen, 'mg_null', 0, NULL);
    JUT_ASSERT(150, mpMg_c[0] != NULL);
    mpMg_c[1] = new CPaneMgr(mpTxScreen, 'mg_null', 0, NULL);
    JUT_ASSERT(153, mpMg_c[1] != NULL);
    OSInitFastCast();

#if VERSION == VERSION_GCN_JPN
    field_0xf4 = 0.0f;
#else
    field_0xf4 = -10.0f;
#endif
    fukiPosCalc(param_1);
    mpPmP_c = new CPaneMgr(mpScreen, 'n_all', 3, NULL);
    JUT_ASSERT(176, mpPmP_c != NULL);
    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxTalkScaleX, g_MsgObject_HIO_c.mBoxTalkScaleY);

#if VERSION == VERSION_GCN_JPN
    if (dComIfGs_getOptUnk0() == 0) {
        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3flin', 0, NULL);
        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3f_s', 0, NULL);

        mpTmr_c[0] = new CPaneMgr(mpTxScreen, 'mg_3f', 0, NULL);
        if (dMsgObject_getMsgObjectClass()->getFukiKind() == 8) {
            mpTm_c[2] = new CPaneMgr(mpTxScreen, 't3f_s1', 0, NULL);
            mpTm_c[3] = new CPaneMgr(mpTxScreen, 't3f_s2', 0, NULL);
            mpTm_c[4] = new CPaneMgr(mpTxScreen, 't3f_s3', 0, NULL);
            mpTm_c[5] = new CPaneMgr(mpTxScreen, 't3f_s4', 0, NULL);
            mpTmr_c[1] = new CPaneMgr(mpTxScreen, 'mg_3f_s1', 0, NULL);
            mpTmr_c[2] = new CPaneMgr(mpTxScreen, 'mg_3f_s2', 0, NULL);
        } else {
            mpTmr_c[1] = new CPaneMgr(mpTxScreen, 'mg_3f_s', 0, NULL);
        }

        mpTxScreen->search('n_3line')->hide();
        mpTxScreen->search('n_3fline')->show();
        mpTxScreen->search('n_e4line')->hide();
    } else {
        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3line', 0, NULL);
        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3_s', 0, NULL);

        if (dMsgObject_getMsgObjectClass()->getFukiKind() == 8) {
            mpTm_c[2] = new CPaneMgr(mpTxScreen, 't3_s1', 0, NULL);
            JUT_ASSERT(189, mpTm_c[2] != NULL);
            mpTm_c[3] = new CPaneMgr(mpTxScreen, 't3_s2', 0, NULL);
            JUT_ASSERT(191, mpTm_c[3] != NULL);
            mpTm_c[4] = new CPaneMgr(mpTxScreen, 't3_s3', 0, NULL);
            JUT_ASSERT(193, mpTm_c[4] != NULL);
            mpTm_c[5] = new CPaneMgr(mpTxScreen, 't3_s4', 0, NULL);
            JUT_ASSERT(193, mpTm_c[5] != NULL);
        }

        mpTxScreen->search('n_3line')->show();
        mpTxScreen->search('n_3fline')->hide();
        mpTxScreen->search('n_e4line')->hide();
    }
#else
    mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_e4lin', 0, NULL);
    JUT_ASSERT(182, mpTm_c[0] != NULL);
    mpTm_c[1] = new CPaneMgr(mpTxScreen, 't4_s', 0, NULL);
    JUT_ASSERT(185, mpTm_c[1] != NULL);

    if (dMsgObject_getMsgObjectClass()->getFukiKind() == 8) {
        mpTm_c[2] = new CPaneMgr(mpTxScreen, 't4_s1', 0, NULL);
        JUT_ASSERT(189, mpTm_c[2] != NULL);
        mpTm_c[3] = new CPaneMgr(mpTxScreen, 't4_s2', 0, NULL);
        JUT_ASSERT(191, mpTm_c[3] != NULL);
        mpTm_c[4] = new CPaneMgr(mpTxScreen, 't4_s3', 0, NULL);
        JUT_ASSERT(193, mpTm_c[4] != NULL);
        mpTm_c[5] = new CPaneMgr(mpTxScreen, 't4_s4', 0, NULL);
        JUT_ASSERT(193, mpTm_c[5] != NULL);
    }

    mpTxScreen->search('n_3line')->hide();
    mpTxScreen->search('n_3fline')->hide();
    mpTxScreen->search('n_e4line')->show();
#endif
    for (int i = 0; i < 6; i++) {
        if (mpTm_c[i] != NULL) {
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x210, "");
#else
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x200, "");
#endif
        }
        if (mpTmr_c[i] != NULL) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setString(0x210, "");
#else
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setString(0x200, "");
#endif
        }
    }
    if (dMsgObject_getMsgObjectClass()->getFukiKind() != 8) {
        for (int i = 0; i < 2; i++) {
            if (mpTm_c[i] != NULL) {
                mpTm_c[i]->setBlackWhite(g_MsgObject_HIO_c.mBoxStartBlack[i][0],
                                         g_MsgObject_HIO_c.mBoxStartWhite[i][0]);
            }
            if (mpTmr_c[i] != 0) {
                mpTmr_c[i]->setBlackWhite(g_MsgObject_HIO_c.mBoxStartBlack[i][0],
                                          g_MsgObject_HIO_c.mBoxStartWhite[i][0]);
            }
        }
    }
    ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getFontSize(mFontSize);
    mTBoxWidth = mpTm_c[0]->getSizeX();
    mTBoxHeight = mpTm_c[0]->getSizeY();
    mLineSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getLineSpace();
    mCharSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getCharSpace();
    for (int i = 0; i < 6; i++) {
        if (mpTm_c[i] != NULL) {
            ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setLineSpace(mLineSpace);
            mpTm_c[i]->resize(1.2f * mpTm_c[i]->getSizeX(), mpTm_c[i]->getSizeY());
        }
    }
    mTextBoxPosX = mpTm_c[0]->getGlobalPosX();
    mTextBoxPosY = mpTm_c[0]->getGlobalPosY();
    for (int i = 0; i < 6; i++) {
        if (mpTmr_c[i] != NULL) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setLineSpace(mLineSpace);
            if (i == 0) {
                J2DTextBox::TFontSize fontSize;
                ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getFontSize(fontSize);
                mRubySize = fontSize.mSizeX;
                mRubyCharSpace = ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getCharSpace();
            }
            mpTmr_c[i]->resize(1.2f * mpTmr_c[i]->getSizeX(), mpTmr_c[i]->getSizeY());
        }
    }
    if (mpSelect_c != NULL) {
        mSelTBoxWidth = mpSelect_c->getTextBoxWidth();
        mSelFontSize = mpSelect_c->getFontSize();
        mSelRubySize = mpSelect_c->getRubyFontSize();
        mSelCharSpace = mpSelect_c->getCharSpace();
        mSelRubyCharSpace = mpSelect_c->getRubyCharSpace();
        for (int i = 0; i < 3; i++) {
            mSelTextBoxPosX[i] = mpSelect_c->getTextBoxGlobalPosX(i);
            mSelTextBoxPosY[i] = mpSelect_c->getTextBoxGlobalPosY(i);
        }
    } else {
        mSelTBoxWidth = 0.0f;
        mSelFontSize = 0.0f;
        mSelRubySize = 0.0f;
        mSelCharSpace = 0.0f;
        mSelRubyCharSpace = 0.0f;
        for (int i = 0; i < 3; i++) {
            mSelTextBoxPosX[i] = 0.0f;
            mSelTextBoxPosY[i] = 0.0f;
        }
    }
}

dMsgScrnTalk_c::~dMsgScrnTalk_c() {
    delete mpScreen;
    mpScreen = NULL;
    delete mpTxScreen;
    mpTxScreen = NULL;
    delete mpArrow_c;
    mpArrow_c = NULL;
    if (mpSelect_c != NULL) {
        delete mpSelect_c;
        mpSelect_c = NULL;
    }
    delete mpLight_c;
    mpLight_c = NULL;
    if (mCharInfoPtr != NULL) {
        field_0xe4->free(mCharInfoPtr);
        mCharInfoPtr = NULL;
    }
    delete mpArw_c;
    mpArw_c = NULL;
    for (int i = 0; i < 2; i++) {
        delete mpMg_c[i];
        mpMg_c[i] = NULL;
    }
    delete mpPmP_c;
    mpPmP_c = NULL;
    for (int i = 0; i < 6; i++) {
        if (mpTm_c[i] != NULL) {
            delete mpTm_c[i];
            mpTm_c[i] = NULL;
        }
        if (mpTmr_c[i] != NULL) {
            delete mpTmr_c[i];
            mpTmr_c[i] = NULL;
        }
    }
    dComIfGp_getMsgArchive(0)->removeResourceAll();
    dComIfGp_getMsgArchive(1)->removeResourceAll();
    dComIfGp_getMsgCommonArchive()->removeResourceAll();
}

void dMsgScrnTalk_c::exec() {
    if (isTalkNow()) {
        fukiAlpha(1.0f);
    }

    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxTalkScaleX, g_MsgObject_HIO_c.mBoxTalkScaleY);
    for (int i = 0; i < 2; i++) {
        if (mpTm_c[i] != NULL) {
            mpTm_c[i]->setBlackWhite(g_MsgObject_HIO_c.mBoxStartBlack[i][0],
                                     g_MsgObject_HIO_c.mBoxStartWhite[i][0]);
        }
        if (mpTmr_c[i] != 0) {
            mpTmr_c[i]->setBlackWhite(g_MsgObject_HIO_c.mBoxStartBlack[i][0],
                                      g_MsgObject_HIO_c.mBoxStartWhite[i][0]);
        }
    }
}

void dMsgScrnTalk_c::drawSelf() {
    J2DGrafContext* grafContext[1];
    grafContext[0] = dComIfGp_getCurrentGrafPort();
    grafContext[0]->setup2D();
    s16* pLen = (s16*)(mCharInfoPtr + 150);
    if (*pLen > 0) {
        f32 dVar10 = mpTm_c[0]->getAlphaRate();
        for (int i = 0; i < *pLen; i++) {
            if (dMsgObject_getMsgObjectClass()->getFukiKind() == 8) {
                if (field_0xf8[i] >= 0.0f) {
                    f32 fVar1;
                    if (mCharInfoPtr[i].field_0x19 == 0) {
                        fVar1 = 0.8f;
                    } else {
                        fVar1 = g_MsgObject_HIO_c.mPikariScale;
                    }
                    dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikari(
                        mCharInfoPtr[i].field_0x00, mCharInfoPtr[i].field_0x04, &field_0xf8[i], fVar1,
                        g_MsgObject_HIO_c.mPikariMoyaR1, g_MsgObject_HIO_c.mPikariMoyaR0,
                        g_MsgObject_HIO_c.mPikariMoyabsR1, g_MsgObject_HIO_c.mPikariMoyabsR0,
                        g_MsgObject_HIO_c.mPikariAnmSpeed, 4);
                    if (field_0xf8[i] == 0.0f) {
                        field_0xf8[i] = -1.0f;
                    }
                }
                if (field_0x35c[i] > 0) {
                    field_0x35c[i]--;
                } else {
                    mpLight_c->draw(
                        &mCharInfoPtr[i].field_0x14, mCharInfoPtr[i].field_0x00, mCharInfoPtr[i].field_0x04,
                        mCharInfoPtr[i].field_0x08, mCharInfoPtr[i].field_0x0c,
                        dVar10 * g_MsgObject_HIO_c.mBoxTalkHaloAlpha, mCharInfoPtr[i].mColorNo);
                }
            } else {
                mpLight_c->draw(
                    &mCharInfoPtr[i].field_0x14, mCharInfoPtr[i].field_0x00, mCharInfoPtr[i].field_0x04,
                    mCharInfoPtr[i].field_0x08, mCharInfoPtr[i].field_0x0c,
                    dVar10 * g_MsgObject_HIO_c.mBoxTalkHaloAlpha, mCharInfoPtr[i].mColorNo);
            }
        }
    } else {
        if (dMsgObject_getMsgObjectClass()->getFukiKind() == 8) {
            for (int i = 0; i < 150; i++) {
                field_0xf8[i] = 18.0f - g_MsgObject_HIO_c.mPikariScale;
                field_0x35c[i] = g_MsgObject_HIO_c.mPikariHaloDelay_spirit;
            }
        }
    }
    drawOutFont(0.0f, 0.0f, 1.0f);
    mpTxScreen->draw(0.0f, 0.0f, grafContext[0]);
    if (mpSelect_c != NULL) {
        mpSelect_c->draw(0.0f, 0.0f);
    }
    mpArrow_c->draw();
}

void dMsgScrnTalk_c::arwAnimeInit() {
    mpArrow_c->arwAnimeInit();
}

void dMsgScrnTalk_c::arwAnimeMove() {
    mpArrow_c->arwAnimeMove();
}

void dMsgScrnTalk_c::dotAnimeInit() {
    mpArrow_c->dotAnimeInit();
}

void dMsgScrnTalk_c::dotAnimeMove() {
    mpArrow_c->dotAnimeMove();
}

void dMsgScrnTalk_c::setSelectString(char* param_0, char* param_1, char* param_2) {
    if (mpSelect_c != NULL) {
        mpSelect_c->setString(param_0, param_1, param_2);
    }
}

void dMsgScrnTalk_c::setSelectRubyString(char* param_0, char* param_1, char* param_2) {
    if (mpSelect_c != NULL) {
        mpSelect_c->setRubyString(param_0, param_1, param_2);
    }
}

bool dMsgScrnTalk_c::isSelect() {
    if (mpSelect_c != NULL) {
        return mpSelect_c->isSelect();
    }
    return false;
}

void dMsgScrnTalk_c::selectAnimeInit(u8 param_0, u8 param_1, f32 param_2, u8 param_3) {
    if (mpSelect_c != NULL) {
        mpSelect_c->selAnimeInit(param_0, param_1, field_0x488, param_2, param_3);
    }
}

bool dMsgScrnTalk_c::selectAnimeMove(u8 param_0, u8 param_1, bool param_2) {
    if (mpSelect_c != NULL) {
        return mpSelect_c->selAnimeMove(param_0, param_1, param_2);
    }
    return false;
}

bool dMsgScrnTalk_c::selectAnimeEnd() {
    if (mpSelect_c != NULL) {
        return mpSelect_c->selAnimeEnd();
    }
    return false;
}

void dMsgScrnTalk_c::fukiScale(f32 param_0) {
}

void dMsgScrnTalk_c::fukiTrans(f32 param_0, f32 param_1) {
}

void dMsgScrnTalk_c::fukiAlpha(f32 param_1) {
    mpPmP_c->setAlphaRate(param_1 * g_MsgObject_HIO_c.mBoxTalkAlphaP);
    for (int i = 0; i < 6; i = i + 1) {
        if (mpTm_c[i] != NULL) {
            mpTm_c[i]->setAlphaRate(param_1 * mCharAlphaRate);
        }
        if (mpTmr_c[i] != NULL) {
            mpTmr_c[i]->setAlphaRate(param_1 * mCharAlphaRate);
        }
    }
}

void dMsgScrnTalk_c::fukiPosCalc(u8 param_1) {
    msg_class* msgActor = dMsgObject_c::getActor();
    if (mpPmP_c != 0) {
        mpPmP_c->move(field_0xe8, field_0xec);
    }
    if (param_1 == 3) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz local_70;
        cXyz cStack_7c;
        f32 f3y;
        mDoLib_project(&player->eyePos, &cStack_7c);
        if (msgActor->pos == cXyz(0.0f, 0.0f, 0.0f)) {
            f3y = cStack_7c.y;
        } else {
            mDoLib_project(&msgActor->pos, &local_70);
            if (local_70.x >= 0.0f && local_70.x <= 608.0f && local_70.y >= 0.0f &&
                local_70.y <= 448.0f)
            {
                f3y = 0.5f * (cStack_7c.y + local_70.y);
            } else {
                f3y = cStack_7c.y;
            }
        }
        if (mpPmP_c != NULL) {
            switch (field_0x488) {
            case 1:
                if (f3y < g_MsgObject_HIO_c.mBoxAppearBound - 50.0f) {
                    param_1 = 0;
                } else {
                    param_1 = 1;
                }
                break;
            case 0:
                if (f3y < g_MsgObject_HIO_c.mBoxAppearBound + 50.0f) {
                    param_1 = 0;
                } else {
                    param_1 = 1;
                }
                break;
            }
        } else {
            if (f3y < g_MsgObject_HIO_c.mBoxAppearBound) {
                param_1 = 0;
            } else {
                param_1 = 1;
            }
        }
    }
    field_0xf0 = 0.0f;
    field_0x488 = param_1;
    f32 dVar15;
    switch (field_0x488) {
    case 1:
        dVar15 = g_MsgObject_HIO_c.mBoxPos[2][0];
        field_0xf0 = 105.0f - (mpScreen->search('n_all')->getBounds().i.y +
                               mpScreen->search('n_all')->getBounds().f.y) / 2;
        if (mpSelect_c != NULL) {
            mpSelect_c->translate(486.0f, 230.0f);
        }
        break;
    case 2:
        dVar15 = g_MsgObject_HIO_c.mBoxPos[1][0];
        field_0xf0 = 235.0f - (mpScreen->search('n_all')->getBounds().i.y +
                               mpScreen->search('n_all')->getBounds().f.y) / 2;
        if (mpSelect_c != NULL) {
            mpSelect_c->translate(486.0f, 355.0f);
        }
        break;
    default:
        dVar15 = g_MsgObject_HIO_c.mBoxPos[0][0];
        field_0xf0 = 0.0f;
        if (mpSelect_c != NULL) {
            mpSelect_c->translate(486.0f, 209.0f);
        }

    }
    if (dComIfGp_isHeapLockFlag() == 2 || dComIfGp_isHeapLockFlag() == 3) {
        setTextBoxPosOffsetY(-190.0f);
        mpScreen->search('n_all')->move(field_0xe8, field_0xec - 190.0f);
    } else {
        setTextBoxPosOffsetY(field_0xf0 + dVar15);
        mpScreen->search('n_all')->move(field_0xe8, field_0xec + field_0xf0 + dVar15);
    }
    if (mpPmP_c != NULL) {
        mpPmP_c->reinit();
        mpArrow_c->setPos(mpArw_c->getGlobalPosX() + (0.5f * mpArw_c->getSizeX()),
                          mpArw_c->getGlobalPosY() + (0.5f * mpArw_c->getSizeY()));
        mpMg_c[1]->translate(field_0xf4 +
                                 (mpMg_c[0]->getGlobalPosX() + (0.5f * mpMg_c[0]->getSizeX())),
                             (mpMg_c[0]->getGlobalPosY() + (0.5f * mpMg_c[0]->getSizeY())));
        mTextBoxPosX = mpTm_c[0]->getGlobalPosX();
        mTextBoxPosY = mpTm_c[0]->getGlobalPosY();
    } else {
        Vec local_ac = mpArw_c->getGlobalVtxCenter(false, 0);
        mpArrow_c->setPos(local_ac.x, local_ac.y);
        Vec local_b8 = mpMg_c[0]->getGlobalVtxCenter(false, 0);
        mpMg_c[1]->translate(local_b8.x + field_0xf4, local_b8.y);
    }
}
