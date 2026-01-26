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
#include <cstdio>

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

dMsgScrnExplain_c::dMsgScrnExplain_c(STControl* i_stick, u8 param_1, bool i_isUseBackTex, u8 param_3) {
    mpStick = i_stick;
    mOpenMsgId = 0;
    field_0x5c = 0;
    field_0x64 = 0;
    mSelCursor = 0;
    field_0x62 = 0xff;
    field_0x63 = 0;
    mStatus = STATUS_WAIT_e;
    field_0x5a = 0;
    mForceSelect = 0;
    field_0x58 = true;
    field_0x66 = param_1;
    mKeyWaitTimer = 0;
    field_0x67 = param_3;

    mpString_c = new dMsgString_c();
    JUT_ASSERT(79, mpString_c != NULL);

    mpOutFont = new COutFont_c(0);
    JUT_ASSERT(83, mpOutFont != NULL);
    mpOutFont->createPane();

    mpScreen = new J2DScreen();
    JUT_ASSERT(88, mpScreen != NULL);

    #if PLATFORM_GCN
    #define MSG_WINDOW_BLO_FILENAME "zelda_message_window_new.blo"
    #else
    #define MSG_WINDOW_BLO_FILENAME "zelda_message_window_new_revo.blo"
    #endif

    bool fg = mpScreen->setPriority(MSG_WINDOW_BLO_FILENAME, 0x20000, dComIfGp_getMsgArchive(1));
    JUT_ASSERT(98, fg != false);
    dPaneClass_showNullPane(mpScreen);

    mpTxScreen = new J2DScreen();
    JUT_ASSERT(102, mpTxScreen != NULL);
    field_0x48 = 608.0f;

    if (param_1 == 1 || param_1 == 3) {
        #if PLATFORM_GCN
        #define MSG_WINDOW_TXT_SAVEROAD_BLO_FILENAME "zelda_message_window_text_save_road.blo"
        #else
        #define MSG_WINDOW_TXT_SAVEROAD_BLO_FILENAME "zelda_message_window_save_road_text_revo.blo"
        #endif
        fg = mpTxScreen->setPriority(MSG_WINDOW_TXT_SAVEROAD_BLO_FILENAME, 0x20000, dComIfGp_getMsgCommonArchive());
        JUT_ASSERT(119, fg != false);
        dPaneClass_showNullPane(mpTxScreen);

        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3line', 0, NULL);
        JUT_ASSERT(124, mpTm_c[0] != NULL);

        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3_s', 0, NULL);
        JUT_ASSERT(127, mpTm_c[1] != NULL);

        mpTmr_c[0] = NULL;
        mpTmr_c[1] = NULL;

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
        fg = mpTxScreen->setPriority("zelda_message_window_text.blo", 0x20000,
                                     dComIfGp_getMsgCommonArchive());
        JUT_ASSERT(153, fg != false);
        dPaneClass_showNullPane(mpTxScreen);

        mpScreen->search('n_all')->scale(g_MsgObject_HIO_c.mBoxTalkScaleX,
                                         g_MsgObject_HIO_c.mBoxTalkScaleY);
#if VERSION == VERSION_GCN_JPN
        field_0x50 = 0.0f;

        if (dComIfGs_getOptRuby() == 0) {
            mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3flin', 0, NULL);
            mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3f_s', 0, NULL);

            mpTmr_c[0] = new CPaneMgr(mpTxScreen, 'mg_3f', 0, NULL);
            mpTmr_c[1] = new CPaneMgr(mpTxScreen, 'mg_3f_s', 0, NULL);

            mpTxScreen->search('n_3line')->hide();
            mpTxScreen->search('n_3fline')->show();
            mpTxScreen->search('n_e4line')->hide();
        } else {
            mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3line', 0, NULL);
            mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3_s', 0, NULL);

            mpTmr_c[0] = NULL;
            mpTmr_c[1] = NULL;

            mpTxScreen->search('n_3line')->show();
            mpTxScreen->search('n_3fline')->hide();
            mpTxScreen->search('n_e4line')->hide();
        }
#else
        field_0x50 = -10.0f;

        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_e4lin', 0, NULL);
        JUT_ASSERT(162, mpTm_c[0] != NULL);

        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't4_s', 0, NULL);
        JUT_ASSERT(165, mpTm_c[1] != NULL);

        mpTmr_c[0] = NULL;
        mpTmr_c[1] = NULL;

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

        if (mpTmr_c[i] != NULL) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setString(0x210, "");
#else
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setString(0x200, "");
#endif
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setLineSpace(lineSpace);
        }
    }

    mpScreen->search('mg_null')->move(
        g_MsgObject_HIO_c.mTextPosX + mpScreen->search('mg_null')->getBounds().i.x,
        g_MsgObject_HIO_c.mTextPosY + mpScreen->search('mg_null')->getBounds().i.y);

    mpArw_c = new CPaneMgr(mpScreen, 'set_ya_n', 0, NULL);
    JUT_ASSERT(241, mpArw_c != NULL);

    mpMg_c[0] = new CPaneMgr(mpScreen, 'mg_null', 0, NULL);
    JUT_ASSERT(244, mpMg_c[0] != NULL);

    mpMg_c[1] = new CPaneMgr(mpTxScreen, 'mg_null', 0, NULL);
    JUT_ASSERT(247, mpMg_c[1] != NULL);

    mpRoot_c[0] = new CPaneMgr(mpScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(250, mpRoot_c[0] != NULL);

    mpRoot_c[1] = new CPaneMgr(mpTxScreen, 'ROOT', 2, NULL);
    JUT_ASSERT(253, mpRoot_c[1] != NULL);

    if (i_isUseBackTex == true) {
        ResTIMG* texResource = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
        mpBackTex = new J2DPicture(texResource);
        JUT_ASSERT(259, mpBackTex != NULL);
        mpBackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                 JUtility::TColor(0, 0, 0, 0xff));
        mpBackTex->setAlpha(0);
    } else {
        mpBackTex = NULL;
    }

    mpArrow_c = new dMsgScrnArrow_c();
    JUT_ASSERT(269, mpArrow_c != NULL);

    if (i_stick != NULL) {
        mpSelect_c = new dMsgScrn3Select_c();
        JUT_ASSERT(274, mpSelect_c != NULL);
    } else {
        mpSelect_c = NULL;
    }

    Vec vtxCenter = mpArw_c->getGlobalVtxCenter(false, 0);
    mpArrow_c->setPos(vtxCenter.x, vtxCenter.y);

    Vec vtxCenter2 = mpMg_c[0]->getGlobalVtxCenter(false, 0);
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

        if (mpTmr_c[i] != NULL) {
            delete mpTmr_c[i];
            mpTmr_c[i] = NULL;
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
    if (mStatus == STATUS_WAIT_e || mStatus == STATUS_OPEN_REQ_e) {
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
    mpString_c->getString(mOpenMsgId, (J2DTextBox*)mpTm_c[0]->getPanePtr(), NULL, NULL, NULL, 12);
#else
    mpString_c->getString(mOpenMsgId, (J2DTextBox*)mpTm_c[0]->getPanePtr(), NULL, NULL, NULL, 8);
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
    J2DTextBox* tbox = NULL;
    J2DTextBox* rubyTbox = NULL;

    tbox = (J2DTextBox*)mpTm_c[0]->getPanePtr();
    if (mpTmr_c[0] != NULL) {
        rubyTbox = (J2DTextBox*)mpTmr_c[0]->getPanePtr();
    }

    char text[STR_BUF_LEN];
    char ruby[STR_BUF_LEN];
    char textShadow[STR_BUF_LEN];

    bool rt = dMsgObject_getString(mOpenMsgId, tbox, rubyTbox, mDoExt_getMesgFont(), mpOutFont, text,
                                   ruby, textShadow, &field_0x5c);
    if (rt) {
        strcpy(((J2DTextBox*)mpTm_c[0]->getPanePtr())->getStringPtr(), text);
        strcpy(((J2DTextBox*)mpTm_c[1]->getPanePtr())->getStringPtr(), textShadow);
        for (int i = 0; i < 2; i++) {
            if (mpTmr_c[i] != NULL) {
                strcpy(((J2DTextBox*)mpTmr_c[i]->getPanePtr())->getStringPtr(), ruby);
            }
        }

        if (field_0x67 != 0) {
            Z2GetSpeechMgr2()->setTextCount(0);
        }

        mStatus = STATUS_OPEN_e;
    }
}

void dMsgScrnExplain_c::open_init() {
    field_0x5a = 0;
    field_0x48 = 608.0f;
    for (int i = 0; i < 2; i++) {
        mpRoot_c[i]->setAlphaRate(0.0f);
    }
    mDoAud_seStart(Z2SE_SY_EXP_WIN_OPEN, NULL, 0, 0);
}

void dMsgScrnExplain_c::open_proc() {
    field_0x5a++;
    if (field_0x5a >= 5) {
        field_0x5a = 5;
        if (field_0x64 == 1 || field_0x64 == 2) {
            mStatus = STATUS_MOVE_SELECT_e;
        } else {
            mStatus = STATUS_MOVE_e;
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
        ((field_0x58 &&
          (mDoCPd_c::getTrigB(PAD_1) || mDoCPd_c::getTrigZ(PAD_1) || mDoCPd_c::getTrigX(PAD_1) ||
           mDoCPd_c::getTrigY(PAD_1) || mDoCPd_c::getTrigStart(PAD_1) ||
           mDoCPd_c::getTrigR(PAD_1) || mDoCPd_c::getTrigL(PAD_1) || mForceSelect))))
    {
        mStatus = STATUS_CLOSE_e;
        dMeter2Info_set2DVibrationM();
        mDoAud_seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0);
    }
}

void dMsgScrnExplain_c::move_select_init() {
    char msg_buf_yes[8];
    char msg_buf_no[8];
    char string_buf_yes[20];
    char string_buf_no[20];

    f32 var_f31 = 0.0f;
    f32 yes_str_len = 0.0f;
    f32 no_str_len = 0.0f;

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

    yes_str_len = dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                              mpSelect_c->getCharSpace(), msg_buf_yes);
    no_str_len = dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                             mpSelect_c->getCharSpace(), msg_buf_no);

    if (yes_str_len < no_str_len) {
        var_f31 = no_str_len;
    } else {
        var_f31 = yes_str_len;
    }

    if (var_f31 < mpSelect_c->getTextBoxWidth()) {
        var_f31 = mpSelect_c->getTextBoxWidth();
    }

    f32 var_f28 = 0.5f * (var_f31 - yes_str_len);
    f32 var_f27 = 0.5f * (var_f31 - no_str_len);

    snprintf(string_buf_yes, 20,
             "\x1B"
             "CR[%d]",
             (int)var_f28);
    strcat(string_buf_yes, msg_buf_yes);
    snprintf(string_buf_no, 20,
             "\x1B"
             "CR[%d]",
             (int)var_f27);
    strcat(string_buf_no, msg_buf_no);

    mpSelect_c->setString("", string_buf_yes, string_buf_no);
    mpSelect_c->setRubyString("", "", "");
    mpSelect_c->selAnimeInit(2, mSelCursor + 1, 0, var_f31, 0);
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

            mStatus = STATUS_CLOSE_e;
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

            mStatus = STATUS_CLOSE_e;
            mDoAud_seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0);
        } else if (mpSelect_c->isSelect() && mpStick->checkUpTrigger()) {
            if (mSelCursor == 1) {
                mSelCursor = 0;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
        } else if (mpSelect_c->isSelect() && mpStick->checkDownTrigger()) {
            if (mSelCursor == 0) {
                mSelCursor = 1;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
        }
        #if !PLATFORM_GCN
        else if (mpSelect_c->getDPDPoint() != 0xFF) {
            if (mSelCursor != mpSelect_c->getDPDPoint() && field_0x62 != mpSelect_c->getDPDPoint()) {
                mSelCursor = mpSelect_c->getDPDPoint();
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 1, 0, 1.0f, 1.0f, -1.0f,
                                        -1.0f, 0);
                dMeter2Info_set2DVibration();
            }
        }

        field_0x62 = mpSelect_c->getDPDPoint();
        #endif
    }

    if (mStatus == STATUS_MOVE_SELECT_e) {
        mpSelect_c->selAnimeMove(2, mSelCursor + 1, false);
    } else {
        mpSelect_c->selAnimeEnd();
    }
}

void dMsgScrnExplain_c::close_init() {
    field_0x5a = 5;
}

void dMsgScrnExplain_c::close_proc() {
    bool iVar1 = false;
    if (mpSelect_c != NULL) {
        iVar1 = mpSelect_c->selAnimeEnd();
    } else {
        iVar1 = true;
    }

    field_0x5a--;
    if (field_0x5a <= 0) {
        field_0x5a = 0;
        if (iVar1) {
            mStatus = STATUS_WAIT_e;
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

bool dMsgScrnExplain_c::openExplain(u32 i_msgId, u8 param_1, u8 i_cursorSel, u8 param_3, bool param_4) {
    bool rv = false;

    if (g_MsgObject_HIO_c.mSaveSeqMsgDebug) {
        i_msgId = g_MsgObject_HIO_c.mSaveSeqMsgIndex;
    }

    if (mStatus == STATUS_WAIT_e) {
        mOpenMsgId = i_msgId;
        field_0x64 = param_1;
        mSelCursor = i_cursorSel;
        field_0x63 = param_3;
        mForceSelect = 0;
        field_0x58 = param_4;
        mStatus = STATUS_OPEN_REQ_e;
        open_request_init();
        rv = true;
    }

    return rv;
}

f32 dMsgScrnExplain_c::getAlphaRatio() {
    f32 ratio;
    f32 var_f30;
    f32 var_f29;
    switch (mStatus) {
    case STATUS_OPEN_e:
    case STATUS_CLOSE_e:
        var_f29 = field_0x5a;
        var_f30 = 5.0f;
        ratio = 1.0f - (var_f29 / var_f30);
        break;
    case STATUS_WAIT_e:
        ratio = 1.0f;
        break;
    default:
        ratio = 0.0f;
        break;
    }

    return ratio;
}

bool dMsgScrnExplain_c::checkTriggerA() {
    if (mDoCPd_c::getTrigA(PAD_1)) {
        return true;
    } else {
        return false;
    }
}

bool dMsgScrnExplain_c::checkTriggerB() {
    if (mDoCPd_c::getTrigB(PAD_1)) {
        return true;
    } else {
        return false;
    }
}
