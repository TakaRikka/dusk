/**
 * This should match once the padding for dDlst_FileWarn_c's vtable can be figured out
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_file_sel_warning.h"
#include "d/d_msg_string.h"
#include "d/d_pane_class.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "d/d_com_inf_game.h"

typedef void (dFile_warning_c::*procFunc)();
static procFunc fileWarningProc[] = {&dFile_warning_c::modeWait, &dFile_warning_c::modeMove};

dFile_warning_c::dFile_warning_c(JKRArchive* i_archive, u8 param_1) {
    mpArchive = i_archive;
    field_0x3e = param_1;

    screenSet();
    field_0x18 = 0;
    mPosY = 0.0f;
}

dFile_warning_c::~dFile_warning_c() {
    delete mFileWarn.mMsgString;
    delete mFileWarn.Scr;
    delete field_0x24;
    delete mpRootPane;

    mDoExt_removeMesgFont();
}

void dFile_warning_c::screenSet() {
    const char* blo_name[] = {
        "zelda_file_select_warning_window.blo",
        "zelda_option_set_up_window.blo",
    };

    const char* bck_name[] = {
        "zelda_file_select_warning_window.bck",
        "zelda_option_set_up_window.bck",
    };

    mFileWarn.Scr = new J2DScreen();
    JUT_ASSERT(0, mFileWarn.Scr != NULL);

    mFileWarn.mMsgString = new dMsgString_c();
    JUT_ASSERT(0, mFileWarn.mMsgString != NULL);

    mFileWarn.Scr->setPriority(blo_name[field_0x3e], 0x100000, mpArchive);

    field_0x24 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(
        JKRGetNameResource(bck_name[field_0x3e], mpArchive));
    mFileWarn.Scr->setAnimation(field_0x24);
    field_0x24->setFrame(2849.0f);
    mFileWarn.Scr->animation();

    mFileWarn.mFont = mDoExt_getMesgFont();
    mpRootPane = new CPaneMgr(mFileWarn.Scr, 'Nm_02', 0, NULL);
    JUT_ASSERT(0, mpRootPane != NULL);
    field_0x34 = mpRootPane->getTranslateY();

#if REGION_JPN
    mFileWarn.Scr->search('ms_for_2')->hide();
    mFileWarn.Scr->search('ms_for_3')->hide();

    field_0x20 = static_cast<J2DTextBox*>(mFileWarn.Scr->search('w_msg_jp'));
#else
    mFileWarn.Scr->search('w_msg_jp')->hide();
    mFileWarn.Scr->search('ms_for_2')->hide();

    field_0x20 = static_cast<J2DTextBox*>(mFileWarn.Scr->search('ms_for_3'));
#endif
    field_0x20->show();
    field_0x20->setFont(mFileWarn.mFont);
    field_0x20->setString(0x200, "");
    mFileWarn.mMsgString->getString(0x53, field_0x20, NULL, mFileWarn.mFont, NULL, 0);
}

void dFile_warning_c::_move() {
    (this->*fileWarningProc[field_0x18])();

    if (mPosY != 0.0f) {
        mpRootPane->translate(mpRootPane->getTranslateX(), mPosY - field_0x34);
    }
}

void dFile_warning_c::modeWait() {}

void dFile_warning_c::modeMove() {
    if (baseMoveAnm() == true) {
        field_0x18 = 0;
    }
}

bool dFile_warning_c::baseMoveAnm() {
    bool rt;

    if (field_0x28 != field_0x2c) {
        if (field_0x28 < field_0x2c) {
            field_0x28 += 2;

            if (field_0x28 > field_0x2c) {
                field_0x28 = field_0x2c;
            }
        } else {
            field_0x28 -= 2;

            if (field_0x28 < field_0x2c) {
                field_0x28 = field_0x2c;
            }
        }

        field_0x24->setFrame(field_0x28);
        mFileWarn.Scr->animation();
        rt = false;
    }

    if (field_0x28 == field_0x2c) {
        if (field_0x28 == 2859) {
            field_0x3c = 1;
        } else {
            field_0x3c = 0;
        }

        mStatus = 1;
        rt = true;
    }

    return rt;
}

void dFile_warning_c::openInit() {
    field_0x28 = 2849;
    field_0x2c = 2859;
    field_0x24->setFrame(field_0x28);

    mStatus = 0;
    field_0x18 = 1;
}

void dFile_warning_c::closeInit() {
    field_0x28 = 2859;
    field_0x2c = 2849;
    field_0x24->setFrame(field_0x28);

    mStatus = 0;
    field_0x18 = 1;
}

void dFile_warning_c::init() {
    field_0x24->setFrame(2849.0f);
    mFileWarn.Scr->animation();
}

void dFile_warning_c::_draw() {
    dComIfGd_set2DOpa(&mFileWarn);
}

void dFile_warning_c::drawSelf() {
    mFileWarn.draw();
}

void dFile_warning_c::setText(u32 param_0) {
    mFileWarn.mMsgString->getString(param_0, field_0x20, NULL, mFileWarn.mFont, NULL, 0);
}

void dFile_warning_c::setFontColor(JUtility::TColor param_0, JUtility::TColor param_1) {
    field_0x20->setBlackWhite(param_0, param_1);
    field_0x20->setFontColor(JUtility::TColor(255, 255, 255, 255),
                             JUtility::TColor(255, 255, 255, 255));
}

void dDlst_FileWarn_c::draw() {
    J2DGrafContext* graf_ctx = dComIfGp_getCurrentGrafPort();
    Scr->draw(0.0f, 0.0f, graf_ctx);
}
