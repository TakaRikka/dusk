#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_explain.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_lib.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_msg_out_font.h"
#include "d/d_msg_scrn_3select.h"
#include "d/d_msg_scrn_arrow.h"
#include "d/d_msg_string.h"
#include "d/d_pane_class.h"
#include "m_Do/m_Do_controller_pad.h"
#include "stdio.h"

#if VERSION == VERSION_GCN_JPN
#define STR_BUF_LEN 528
#else
#define STR_BUF_LEN 512
#endif

static dMsgScrnExplain_c::ProcFunc init_process[] = {
    &dMsgScrnExplain_c::wait_init,        &dMsgScrnExplain_c::open_request_init,
    &dMsgScrnExplain_c::open_init,        &dMsgScrnExplain_c::move_init,
    &dMsgScrnExplain_c::move_select_init, &dMsgScrnExplain_c::close_init,
};

static dMsgScrnExplain_c::ProcFunc move_process[] = {
    &dMsgScrnExplain_c::wait_proc,        &dMsgScrnExplain_c::open_request_proc,
    &dMsgScrnExplain_c::open_proc,        &dMsgScrnExplain_c::move_proc,
    &dMsgScrnExplain_c::move_select_proc, &dMsgScrnExplain_c::close_proc,
};

dMsgScrnExplain_c::dMsgScrnExplain_c(STControl* i_stick, u8 param_1, bool param_2, u8 param_3) {
    mpStick = i_stick;
    field_0x54 = 0;
    field_0x5c = 0;
    field_0x64 = 0;
    mSelCursor = 0;
    field_0x62 = 0xff;
    field_0x63 = 0;
    mStatus = 0;
    field_0x5a = 0;
    mForceSelect = 0;
    field_0x58 = 1;
    field_0x66 = param_1;
    mKeyWaitTimer = 0;
    field_0x67 = param_3;

    mpString_c = new dMsgString_c();

    mpOutFont = new COutFont_c(0);
    mpOutFont->createPane();

    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_message_window_new.blo", 0x20000, dComIfGp_getMsgArchive(1));
    dPaneClass_showNullPane(mpScreen);

    mpTxScreen = new J2DScreen();
    field_0x48 = 608.0f;

    if (param_1 == 1 || param_1 == 3) {
        mpTxScreen->setPriority("zelda_message_window_text_save_road.blo", 0x20000,
                                dComIfGp_getMsgCommonArchive());
        dPaneClass_showNullPane(mpTxScreen);

        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3line', 0, NULL);
        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3_s', 0, NULL);

        field_0x10[0] = NULL;
        field_0x10[1] = NULL;

        mpTxScreen->search('n_3line')->show();
        mpTxScreen->search('n_3fline')->hide();
        mpTxScreen->search('n_e4line')->hide();

#if VERSION == VERSION_GCN_JPN
        field_0x50 = 0.0f;
#else
        field_0x50 = -10.0f;
#endif
        field_0x4c = 0.0f;
    } else {
        mpTxScreen->setPriority("zelda_message_window_text.blo", 0x20000,
                                dComIfGp_getMsgCommonArchive());
        dPaneClass_showNullPane(mpTxScreen);

        mpScreen->search('n_all')->scale(g_MsgObject_HIO_c.mBoxTalkScaleX,
                                         g_MsgObject_HIO_c.mBoxTalkScaleY);
#if VERSION == VERSION_GCN_JPN
        field_0x50 = 0.0f;

        if (dComIfGs_getOptUnk0() == 0) {
            mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3flin', 0, NULL);
            mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3f_s', 0, NULL);

            field_0x10[0] = new CPaneMgr(mpTxScreen, 'mg_3f', 0, NULL);
            field_0x10[1] = new CPaneMgr(mpTxScreen, 'mg_3f_s', 0, NULL);

            mpTxScreen->search('n_3line')->hide();
            mpTxScreen->search('n_3fline')->show();
            mpTxScreen->search('n_e4line')->hide();
        } else {
            mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3line', 0, NULL);
            mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3_s', 0, NULL);

            field_0x10[0] = NULL;
            field_0x10[1] = NULL;

            mpTxScreen->search('n_3line')->show();
            mpTxScreen->search('n_3fline')->hide();
            mpTxScreen->search('n_e4line')->hide();
        }
#else
        field_0x50 = -10.0f;

        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_e4lin', 0, NULL);
        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't4_s', 0, NULL);

        field_0x10[0] = NULL;
        field_0x10[1] = NULL;

        mpTxScreen->search('n_3line')->hide();
        mpTxScreen->search('n_3fline')->hide();
        mpTxScreen->search('n_e4line')->show();
#endif

        if (param_1 == 2 || param_1 == 4) {
            field_0x4c = 0.0f;
        } else {
            field_0x4c = -190.0f;
        }
    }

    f32 lineSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getLineSpace();
    for (int i = 0; i < 2; i++) {
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x210, "");
#else
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x200, "");
#endif
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setLineSpace(lineSpace);

        if (field_0x10[i] != NULL) {
            ((J2DTextBox*)field_0x10[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
            ((J2DTextBox*)field_0x10[i]->getPanePtr())->setString(0x210, "");
#else
            ((J2DTextBox*)field_0x10[i]->getPanePtr())->setString(0x200, "");
#endif
            ((J2DTextBox*)field_0x10[i]->getPanePtr())->setLineSpace(lineSpace);
        }
    }

    mpScreen->search('mg_null')->move(
        g_MsgObject_HIO_c.mTextPosX + mpScreen->search('mg_null')->getBounds().i.x,
        g_MsgObject_HIO_c.mTextPosY + mpScreen->search('mg_null')->getBounds().i.y);

    mpArw_c = new CPaneMgr(mpScreen, 'set_ya_n', 0, NULL);
    mpMg_c[0] = new CPaneMgr(mpScreen, 'mg_null', 0, NULL);
    mpMg_c[1] = new CPaneMgr(mpTxScreen, 'mg_null', 0, NULL);
    mpRoot_c[0] = new CPaneMgr(mpScreen, 'ROOT', 2, NULL);
    mpRoot_c[1] = new CPaneMgr(mpTxScreen, 'ROOT', 2, NULL);

    if (param_2 == true) {
        ResTIMG* texResource =
            (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
        mpBackTex = new J2DPicture(texResource);
        mpBackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                 JUtility::TColor(0, 0, 0, 0xff));
        mpBackTex->setAlpha(0);
    } else {
        mpBackTex = NULL;
    }

    mpArrow_c = new dMsgScrnArrow_c();

    if (i_stick != NULL) {
        mpSelect_c = new dMsgScrn3Select_c();
    } else {
        mpSelect_c = NULL;
    }

    Vec vtxCenter = mpArw_c->getGlobalVtxCenter(mpArw_c->mPane, false, 0);
    mpArrow_c->setPos(vtxCenter.x, vtxCenter.y);

    Vec vtxCenter2 = mpMg_c[0]->getGlobalVtxCenter(mpMg_c[0]->mPane, false, 0);
    mpMg_c[1]->translate(vtxCenter2.x + field_0x50, vtxCenter2.y);
}

dMsgScrnExplain_c::~dMsgScrnExplain_c() {
    delete mpString_c;
    mpString_c = NULL;

    delete mpOutFont;
    mpOutFont = NULL;

    delete mpScreen;
    mpScreen = NULL;

    delete mpTxScreen;
    mpTxScreen = NULL;

    delete mpArw_c;
    mpArw_c = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpTm_c[i];
        mpTm_c[i] = NULL;

        if (field_0x10[i] != NULL) {
            delete field_0x10[i];
            field_0x10[i] = NULL;
        }

        delete mpMg_c[i];
        mpMg_c[i] = NULL;

        delete mpRoot_c[i];
        mpRoot_c[i] = NULL;
    }

    if (mpBackTex != NULL) {
        delete mpBackTex;
        mpBackTex = NULL;
    }

    if (mpArrow_c != NULL) {
        delete mpArrow_c;
        mpArrow_c = NULL;
    }

    if (mpSelect_c) {
        delete mpSelect_c;
        mpSelect_c = NULL;
    }

    dComIfGp_getMsgArchive(0)->removeResourceAll();
    dComIfGp_getMsgArchive(1)->removeResourceAll();
    dComIfGp_getMsgCommonArchive()->removeResourceAll();
}

void dMsgScrnExplain_c::move() {
    u8 currentStatus = mStatus;

    f32 y_offset = 0.0f;
    if (field_0x66 == 2) {
        y_offset = -35.0f;
    }

    for (int i = 0; i < 2; i++) {
        mpRoot_c[i]->paneTrans(field_0x48 + g_MsgObject_HIO_c.mChoicePos[0][6],
                               field_0x4c + g_MsgObject_HIO_c.mBoxPos[0][6] + y_offset);
    }

    (this->*move_process[mStatus])();

    if (currentStatus != mStatus) {
        (this->*init_process[mStatus])();
    }

    mpScreen->search('n_all')->scale(g_MsgObject_HIO_c.mBoxTalkScaleX,
                                     g_MsgObject_HIO_c.mBoxTalkScaleY);
}

void dMsgScrnExplain_c::draw(J2DOrthoGraph* i_graf) {
    if (mStatus == 0 || mStatus == 1) {
        return;
    }

    if (mpBackTex != NULL) {
        mpBackTex->draw(0.0f, 0.0f, 608.0f, 448.0f, false, false, false);
    }

    if (field_0x66 != 2 && field_0x66 != 3) {
        mpScreen->draw(0.0f, 0.0f, (J2DGrafContext*)i_graf);
    }

    char string_buf[STR_BUF_LEN];
    strcpy(string_buf, ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getStringPtr());

    mpTxScreen->draw(0.0f, 0.0f, (J2DGrafContext*)i_graf);
#if VERSION == VERSION_GCN_JPN
    mpString_c->getString(field_0x54, (J2DTextBox*)mpTm_c[0]->getPanePtr(), NULL, NULL, NULL, 12);
#else
    mpString_c->getString(field_0x54, (J2DTextBox*)mpTm_c[0]->getPanePtr(), NULL, NULL, NULL, 8);
#endif
    mpString_c->drawOutFont((J2DTextBox*)mpTm_c[0]->getPanePtr(), -1.0f);

    strcpy(((J2DTextBox*)mpTm_c[0]->getPanePtr())->getStringPtr(), string_buf);

    if (mpSelect_c != NULL && (field_0x64 == 1 || field_0x64 == 2)) {
        f32 y_offset = 0.0f;
        if (field_0x66 == 2) {
            y_offset = -100.0f;
        }

        mpSelect_c->translate(486.0f + g_MsgObject_HIO_c.mChoicePos[0][7],
                              209.0f + g_MsgObject_HIO_c.mBoxPos[0][7] + y_offset);
        mpSelect_c->draw(0.0f, 0.0f);
    }

    if (mpArrow_c != NULL) {
        mpArrow_c->draw();
    }
}

void dMsgScrnExplain_c::wait_init() {}

void dMsgScrnExplain_c::wait_proc() {}

void dMsgScrnExplain_c::open_request_init() {}

void dMsgScrnExplain_c::open_request_proc() {
    J2DTextBox* uVar1;
    J2DTextBox* uVar6 = NULL;

    uVar1 = (J2DTextBox*)mpTm_c[0]->getPanePtr();
    if (field_0x10[0] != NULL) {
        uVar6 = (J2DTextBox*)field_0x10[0]->getPanePtr();
    }

    char acStack_220[STR_BUF_LEN];
    char acStack_420[STR_BUF_LEN];
    char acStack_620[STR_BUF_LEN];

    if (dMsgObject_getString(field_0x54, uVar1, uVar6, mDoExt_getMesgFont(), mpOutFont, acStack_220,
                             acStack_420, acStack_620, &field_0x5c) != 0)
    {
        strcpy(((J2DTextBox*)mpTm_c[0]->getPanePtr())->getStringPtr(), acStack_220);
        strcpy(((J2DTextBox*)mpTm_c[1]->getPanePtr())->getStringPtr(), acStack_620);
        for (int i = 0; i < 2; i++) {
            if (field_0x10[i] != NULL) {
                strcpy(((J2DTextBox*)field_0x10[i]->getPanePtr())->getStringPtr(), acStack_420);
            }
        }

        if (field_0x67 != 0) {
            Z2GetSpeechMgr2()->setTextCount(0);
        }

        mStatus = 2;
    }
}

void dMsgScrnExplain_c::open_init() {
    field_0x5a = 0;
    field_0x48 = 608.0f;
    for (int i = 0; i < 2; i++) {
        mpRoot_c[i]->setAlphaRate(0.0f);
    }
    mDoAud_seStart(Z2SE_SY_EXP_WIN_OPEN, 0, 0, 0);
}

void dMsgScrnExplain_c::open_proc() {
    field_0x5a++;
    if (field_0x5a >= 5) {
        field_0x5a = 5;
        if (field_0x64 == 1 || field_0x64 == 2) {
            mStatus = 4;
        } else {
            mStatus = 3;
        }
    }

    field_0x48 = 608.0f * getAlphaRatio();
    for (int i = 0; i < 2; i++) {
        mpRoot_c[i]->setAlphaRate(1.0f - getAlphaRatio());
    }

    if (mpBackTex != NULL) {
        mpBackTex->setAlpha((1.0f - getAlphaRatio()) * 150.0f);
    }
}

void dMsgScrnExplain_c::move_init() {
    mpArrow_c->arwAnimeInit();
}

void dMsgScrnExplain_c::move_proc() {
    if (mKeyWaitTimer > 0) {
        mKeyWaitTimer--;
        return;
    }

    if (mDoCPd_c::getTrigA(PAD_1) ||
        ((field_0x58 != 0x0 &&
          (mDoCPd_c::getTrigB(PAD_1) || mDoCPd_c::getTrigZ(PAD_1) || mDoCPd_c::getTrigX(PAD_1) ||
           mDoCPd_c::getTrigY(PAD_1) || mDoCPd_c::getTrigStart(PAD_1) ||
           mDoCPd_c::getTrigR(PAD_1) || mDoCPd_c::getTrigL(PAD_1) || mForceSelect))))
    {
        mStatus = 5;
        dMeter2Info_set2DVibrationM();
        mDoAud_seStart(Z2SE_SY_EXP_WIN_CLOSE, 0, 0, 0);
    }
}

void dMsgScrnExplain_c::move_select_init() {
    char msg_buf_yes[8];
    char msg_buf_no[8];
    char string_buf_yes[20];
    char string_buf_no[20];

    msg_buf_yes[0] = 0;
    msg_buf_no[0] = 0;
    string_buf_yes[0] = 0;
    string_buf_no[1] = 0;

    if (field_0x66 == 4) {
        dMeter2Info_getString(0x4b0, msg_buf_yes, NULL);  // "YES"
        dMeter2Info_getString(0x4b1, msg_buf_no, NULL);   // "NO"
    } else {
        dMeter2Info_getString(0x51f, msg_buf_yes, NULL);  // "YES"
        dMeter2Info_getString(0x520, msg_buf_no, NULL);   // "NO"
    }

    f32 yes_str_len = dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                                  mpSelect_c->getCharSpace(), msg_buf_yes);
    f32 no_str_len = dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                                 mpSelect_c->getCharSpace(), msg_buf_no);
    f32 dVar13;
    if (yes_str_len < no_str_len) {
        dVar13 = no_str_len;
    } else {
        dVar13 = yes_str_len;
    }

    f32 tbox_width = mpSelect_c->getTextBoxWidth();
    if (dVar13 < tbox_width) {
        dVar13 = mpSelect_c->getTextBoxWidth();
    }

    snprintf(string_buf_yes, 20,
             "\x1B"
             "CR[%d]",
             (int)(0.5f * (dVar13 - yes_str_len)));
    strcat(string_buf_yes, msg_buf_yes);
    snprintf(string_buf_no, 20,
             "\x1B"
             "CR[%d]",
             (int)(0.5f * (dVar13 - no_str_len)));
    strcat(string_buf_no, msg_buf_no);

    mpSelect_c->setString("", string_buf_yes, string_buf_no);
    mpSelect_c->setRubyString("", "", "");
    mpSelect_c->selAnimeInit(2, mSelCursor + 1, 0, dVar13, 0);
    mpArrow_c->arwAnimeInit();
}

void dMsgScrnExplain_c::move_select_proc() {
    if (mKeyWaitTimer > 0) {
        mKeyWaitTimer--;
    } else {
        mpStick->checkTrigger();

        if (checkTriggerA()) {
            if (field_0x64 == 2) {
                if (mSelCursor == 0) {
                    dMeter2Info_setWarpStatus(3);
                    Z2GetAudioMgr()->seStart(Z2SE_WARP_MAP_DECIDE, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                             -1.0f, 0);
                } else {
                    Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                             -1.0f, 0);
                }
            } else if (field_0x66 == 0) {
                mDoAud_seStart(Z2SE_SY_TALK_CURSOR_OK, NULL, 0, 0);
            }

            mStatus = 5;
            mDoAud_seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0);
        } else if (field_0x58 && (checkTriggerB() || mForceSelect)) {
            if (field_0x64 == 2) {
                Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            } else if (field_0x64 == 1) {
                if (field_0x63 != 0xff) {
                    mSelCursor = field_0x63;
                } else {
                    mSelCursor = 0xff;
                }
            }

            mStatus = 5;
            mDoAud_seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0);
        } else if (mpSelect_c->isSelect() && mpStick->checkUpTrigger()) {
            if (mSelCursor == 1) {
                mSelCursor = 0;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
        } else if (mpSelect_c->isSelect() && mpStick->checkDownTrigger() && mSelCursor == 0) {
            mSelCursor = 1;
            Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
        }
    }

    if (mStatus == 4) {
        mpSelect_c->selAnimeMove(2, mSelCursor + 1, false);
    } else {
        mpSelect_c->selAnimeEnd();
    }
}

void dMsgScrnExplain_c::close_init() {
    field_0x5a = 5;
}

void dMsgScrnExplain_c::close_proc() {
    bool iVar1;
    if (mpSelect_c != NULL) {
        iVar1 = mpSelect_c->selAnimeEnd();
    } else {
        iVar1 = true;
    }

    field_0x5a--;
    if (field_0x5a <= 0) {
        field_0x5a = 0;
        if (iVar1) {
            mStatus = 0;
        }
    }

    field_0x48 = 608.0f * getAlphaRatio();
    for (int i = 0; i < 2; i++) {
        mpRoot_c[i]->setAlphaRate(1.0f - getAlphaRatio());
    }

    if (mpBackTex != NULL) {
        mpBackTex->setAlpha(150.0f * (1.0f - getAlphaRatio()));
    }
}

bool dMsgScrnExplain_c::openExplain(u32 param_0, u8 param_1, u8 param_2, u8 param_3, bool param_4) {
    bool rv = false;

    if (g_MsgObject_HIO_c.mSaveSeqMsgDebug) {
        param_0 = g_MsgObject_HIO_c.mSaveSeqMsgIndex;
    }

    if (mStatus == 0) {
        field_0x54 = param_0;
        field_0x64 = param_1;
        mSelCursor = param_2;
        field_0x63 = param_3;
        mForceSelect = 0;
        field_0x58 = param_4;
        mStatus = 1;
        open_request_init();
        rv = true;
    }

    return rv;
}

f32 dMsgScrnExplain_c::getAlphaRatio() {
    switch (mStatus) {
    case 2:
    case 5:
        return 1.0f - (field_0x5a / 5.0f);
    case 0:
        return 1.0f;
    default:
        return 0.0f;
    }
}

bool dMsgScrnExplain_c::checkTriggerA() {
    return mDoCPd_c::getTrigA(PAD_1);
}

bool dMsgScrnExplain_c::checkTriggerB() {
    return mDoCPd_c::getTrigB(PAD_1);
}
