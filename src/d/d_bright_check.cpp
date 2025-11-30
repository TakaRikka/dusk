/**
 * d_bright_check.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_bright_check.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_controller_pad.h"

dBrightCheck_c::dBrightCheck_c(JKRArchive* i_archive) {
    mArchive = i_archive;
    mBrightCheck.mMsgString = new dMsgString_c();
    JUT_ASSERT(0, mBrightCheck.mMsgString != NULL);

    screenSet();
    mCompleteCheck = false;
    mMode = MODE_MOVE_e;
}

dBrightCheck_c::~dBrightCheck_c() {
    delete mBrightCheck.Scr;
    delete mBrightCheck.mMsgString;
}

void dBrightCheck_c::screenSet() {
    static u64 const tv_btnA[] = {
        'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4', 'cont_at',
    };

    static u64 const ftv_btnA[] = {
        'font_a1', 'font_at2', 'font_at3', 'font_at4', 'font_at',
    };

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    static u64 const txTV[] = {
        'menu_t6s', 'menu_t6',  'menu_t9s', 'menu_t9',  'menut10s',
        'menu_t10', 'menu_t7s', 'menu_t7',  'menu_t8s', 'menu_t8',
    };
    #else
    static u64 const txTV[] = {
        'menu_t61', 'menu_t2',  'menu_t91', 'menu_t1',  'menut101',
        'menu_t01', 'menu_t71', 'menu_t3',  'menu_t81', 'menu_t4',
    };
    #endif

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    static u64 const txTVhide[] = {
        'fmenu_6n', 'fmenu_9n', 'fmenu_10', 'fmenu_7n', 'fmenu_8n',
    };
    #else
    static u64 const txTVhide[] = {
        'menu_6n', 'menu_9n', 'menu_10n', 'menu_7n', 'menu_8n',
    };
    #endif

    mBrightCheck.Scr = new J2DScreen();
    JUT_ASSERT(0, mBrightCheck.Scr != NULL);
    mBrightCheck.Scr->setPriority("zelda_option_check.blo", 0x1100000, mArchive);

    mBrightCheck.Scr->search('g_abtn_n')->hide();

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    J2DTextBox* settings_text = (J2DTextBox*)mBrightCheck.Scr->search('t_t00');
    mBrightCheck.Scr->search('t_t00')->show();
    mBrightCheck.Scr->search('f_t00')->hide();
    #else
    J2DTextBox* settings_text = (J2DTextBox*)mBrightCheck.Scr->search('f_t00');
    mBrightCheck.Scr->search('f_t00')->show();
    mBrightCheck.Scr->search('t_t00')->hide();
    #endif

    settings_text->setFont(mDoExt_getRubyFont());
    settings_text->setString(0x40, "");
    mBrightCheck.mMsgString->getString(0x55C, settings_text, NULL, NULL, NULL, 0);  // "TV Settings"

    J2DTextBox* btna_text[5];
    for (int i = 0; i < 5; i++) {
        #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
        btna_text[i] = (J2DTextBox*)mBrightCheck.Scr->search(tv_btnA[i]);
        mBrightCheck.Scr->search(ftv_btnA[i])->hide();
        #else
        btna_text[i] = (J2DTextBox*)mBrightCheck.Scr->search(ftv_btnA[i]);
        mBrightCheck.Scr->search(tv_btnA[i])->hide();
        #endif

        btna_text[i]->setFont(mDoExt_getMesgFont());
        btna_text[i]->setString(0x40, "");
        mBrightCheck.mMsgString->getString(0x564, btna_text[i], NULL, NULL, NULL, 0);  // "Complete"
    }

    for (int i = 0; i < 5; i++) {
        mBrightCheck.Scr->search(txTVhide[i])->hide();
    }

    for (int i = 0; i < 10; i++) {
        J2DTextBox* check_text = (J2DTextBox*)mBrightCheck.Scr->search(txTV[i]);
        check_text->setFont(mDoExt_getMesgFont());

        #if (VERSION != VERSION_GCN_JPN) && (VERSION != VERSION_WII_JPN)
        check_text->setCharSpace(0.0f);
        #endif

        if (i < 2) {
            check_text->setString(0x100, "");
            mBrightCheck.mMsgString->getString(0x558, check_text, NULL, NULL, NULL, 0);
        } else if (i < 4) {
            check_text->setString(0x100, "");
            mBrightCheck.mMsgString->getString(0x557, check_text, NULL, NULL, NULL, 0);
        } else if (i < 6) {
            check_text->setString(0x100, "");
            mBrightCheck.mMsgString->getString(0x559, check_text, NULL, NULL, NULL, 0);
        } else if (i < 8) {
            check_text->setString(0x100, "");
            mBrightCheck.mMsgString->getString(0x55A, check_text, NULL, NULL, NULL, 0);
        } else {
            check_text->setString(0x100, "");
            mBrightCheck.mMsgString->getString(0x55B, check_text, NULL, NULL, NULL, 0);
        }
    }
}

typedef void (dBrightCheck_c::*procFunc)();
static procFunc brightChackProc[] = {
    &dBrightCheck_c::modeWait,
    &dBrightCheck_c::modeMove,
};

void dBrightCheck_c::_move() {
    (this->*brightChackProc[mMode])();
}

void dBrightCheck_c::modeWait() {}

void dBrightCheck_c::modeMove() {
    if (mDoCPd_c::getTrigA(PAD_1) || mDoCPd_c::getTrigStart(PAD_1)) {
        mDoAud_seStart(Z2SE_ENTER_GAME, NULL, 0, 0);
        mCompleteCheck = true;
        mMode = MODE_WAIT_e;
    }
}

void dBrightCheck_c::_draw() {
    dComIfGd_set2DOpa(&mBrightCheck);
}

void dDlst_BrightCheck_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    Scr->draw(0.0f, 0.0f, graf_ctx);
}
