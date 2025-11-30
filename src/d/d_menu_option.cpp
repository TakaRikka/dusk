//
// Option Menu
//

#include "d/dolzel.h" // IWYU pragma: keep

#include <dolphin/os.h>
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "d/d_com_inf_game.h"
#include "d/d_file_sel_warning.h"
#include "d/d_lib.h"
#include "d/d_menu_calibration.h"
#include "d/d_menu_option.h"
#include "d/d_menu_window.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_meter_haihai.h"
#include "d/d_msg_string.h"
#include "d/d_select_cursor.h"
#include "dolphin/types.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "string.h"

typedef void (dMenu_Option_c::*initFunc)();
static initFunc init[] = {
    &dMenu_Option_c::atten_init,
#if VERSION == VERSION_GCN_JPN
    &dMenu_Option_c::ruby_init,
#endif
    &dMenu_Option_c::vib_init,
    &dMenu_Option_c::sound_init,          &dMenu_Option_c::change_init,
    &dMenu_Option_c::confirm_open_init,   &dMenu_Option_c::confirm_move_init,
    &dMenu_Option_c::confirm_select_init, &dMenu_Option_c::confirm_close_init,
};

typedef void (dMenu_Option_c::*processFunc)();
static processFunc process[] = {
    &dMenu_Option_c::atten_move,
#if VERSION == VERSION_GCN_JPN
    &dMenu_Option_c::ruby_move,
#endif
    &dMenu_Option_c::vib_move,
    &dMenu_Option_c::sound_move,          &dMenu_Option_c::change_move,
    &dMenu_Option_c::confirm_open_move,   &dMenu_Option_c::confirm_move_move,
    &dMenu_Option_c::confirm_select_move, &dMenu_Option_c::confirm_close_move,
};

typedef void (dMenu_Option_c::*tvProcessFunc)();
static tvProcessFunc tv_process[] = {
    &dMenu_Option_c::tv_open1_move,  &dMenu_Option_c::tv_open2_move,  &dMenu_Option_c::tv_move_move,
    &dMenu_Option_c::tv_close1_move, &dMenu_Option_c::tv_close2_move,
};

typedef void (dMenu_Option_c::*calibrationFunc)();
static calibrationFunc calibration_process[] = {
    &dMenu_Option_c::calibration_open1_move,  &dMenu_Option_c::calibration_open2_move,
    &dMenu_Option_c::calibration_move_move,   &dMenu_Option_c::calibration_close1_move,
    &dMenu_Option_c::calibration_close2_move,
};

enum SelectType {
#if VERSION == VERSION_GCN_JPN
    SelectType0_JPN,
#endif
    SelectType0,
    SelectType1,
    SelectType2,
    SelectType3,
    SelectType4,
    SelectType5,
    SelectType6,
    SelectType7,
    SelectType8,
};

dMenu_Option_c::dMenu_Option_c(JKRArchive* i_archive, STControl* i_stick) {
    mUseFlag = 0;
    mBarScale[0] = g_drawHIO.mOptionScreen.mBarScale[0];
    mBarScale[1] = g_drawHIO.mOptionScreen.mBarScale[1];
    mpArchive = NULL;
    mpStick = i_stick;
    mpMount = NULL;
    mQuitStatus = 1;
    mpCalibration = NULL;
}

dMenu_Option_c::~dMenu_Option_c() {}

static const u32 dMo_soundMode[3] = {0, 1, 2};

void dMenu_Option_c::_create() {
    static const u64 text_a_tag[5] = {'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5'};
    static const u64 text_b_tag[5] = {'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5'};
    static const u64 l_tagName012[2] = {'w_no_n', 'w_yes_n'};
    static const u64 l_tagName013[2] = {'w_no_t', 'w_yes_t'};
    static const u64 l_tagName9[2] = {'w_no_m', 'w_yes_m'};
    static const u64 l_tagName10[2] = {'w_no_g', 'w_yes_g'};
    static const u64 l_tagName11[2] = {'w_no_gr', 'w_yes_gr'};
    static const u8 l_msgNum2[2] = {8, 7};

    mpFont = mDoExt_getMesgFont();
    mpString = new dMsgString_c();
    mpMeterHaihai = new dMeterHaihai_c(3);
    field_0x3f6 = 0;
    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_option_select_menu.blo", 0x20000, mpArchive);
    mpScreen->search('base_a_n')->hide();
    mpScreen->search('y_set_p4')->hide();
    mpScreen->search('y_set_p3')->hide();
    mpScreen->search('y_set_p2')->hide();
    mpScreen->search('y_set_p1')->hide();
    mpScreen->search('y_set_p0')->hide();
    field_0x254[0] = (J2DTextBox*)mpScreen->search('cont_ts');
    field_0x254[1] = (J2DTextBox*)mpScreen->search('cont_t');
    for (int i = 0; i < 2; i++) {
        field_0x254[i]->setFont(mDoExt_getMesgFont());
        field_0x254[i]->setString(0x20, "");
    }
    mpBackScreen = new J2DScreen();
    mpBackScreen->setPriority("zelda_option_base.blo", 0x20000, mpArchive);
    mpBackScreen->search('wi_btn_n')->hide();
    field_0x27c = mpBackScreen->search('let_area');
    mpClipScreen = new J2DScreen();
    mpClipScreen->setPriority("zelda_option_menu_text.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpClipScreen);
    mpClipScreen->setScissor(true);
    mpShadowScreen = new J2DScreen();
    mpShadowScreen->setPriority("zelda_option_menu_shadow.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpShadowScreen);
    mpShadowScreen->search('mw_n_5')->hide();
    mpTVScreen = new J2DScreen();
    mpTVScreen->setPriority("zelda_option_check.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpTVScreen);
    mpTVButtonAB = new CPaneMgr(mpTVScreen, 'g_abtn_n', 0, NULL);
    mpTVButtonText = new CPaneMgr(mpTVScreen, 'a_text_n', 0, NULL);
    mpTVScreen->search('g_abtn_n')->hide();
    mpScreenIcon = new J2DScreen();
    mpScreenIcon->setPriority("zelda_collect_soubi_do_icon_parts.blo", 0x20000, mpArchive);
    for (int i = 0; i < 2; i++) {
        mpButtonAB[i] = NULL;
        mpButtonText[i] = NULL;
    }
    dPaneClass_showNullPane(mpScreenIcon);
    field_0x3dc = 0;
    field_0x3de = 0;
    for (int i = 0; i < 5; i++) {
        ((J2DTextBox*)(mpScreenIcon->search(text_a_tag[i])))->setFont(mDoExt_getMesgFont());
        ((J2DTextBox*)(mpScreenIcon->search(text_b_tag[i])))->setFont(mDoExt_getMesgFont());

        ((J2DTextBox*)(mpScreenIcon->search(text_a_tag[i])))->setString(0x20, "");
        ((J2DTextBox*)(mpScreenIcon->search(text_b_tag[i])))->setString(0x20, "");
    }
    setAButtonString(0x40C);
    setBButtonString(0x3F9);
    ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(timg);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xff));
    mpBlackTex->setAlpha(0);
    field_0x374 = 0.0f;
    mpWarning = new dFile_warning_c(mpArchive, 1);
    mpWarning->setFontColor(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0xc8, 0xc8, 0xc8, 0xff));
    mpSelectScreen = new J2DScreen();
    mpSelectScreen->setPriority("zelda_file_select_yes_no_window.blo", 0x1100000, mpArchive);
    dPaneClass_showNullPane(mpSelectScreen);
    void* resource = JKRGetNameResource("zelda_file_select_yes_no_window.bck", mpArchive);
    field_0x24 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(resource);
    field_0x28 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(resource);
    field_0x20 = (J2DAnmTransform*)J2DAnmLoaderDataBase::load(resource);
    field_0x24->searchUpdateMaterialID(mpSelectScreen);
    field_0x28->searchUpdateMaterialID(mpSelectScreen);
    field_0x20->searchUpdateMaterialID(mpSelectScreen);
    for (int i = 0; i < 2; i++) {
        mpYesNoSelBase_c[i] = new CPaneMgr(mpSelectScreen, l_tagName012[i], 0, NULL);
        mpYesNoTxt_c[i] = new CPaneMgr(mpSelectScreen, l_tagName013[i], 0, NULL);
        J2DTextBox* yesNoTxt = (J2DTextBox*)mpYesNoTxt_c[i]->getPanePtr();
        yesNoTxt->setFont(mDoExt_getMesgFont());
        char message[24];
        fopMsgM_messageGet(message, l_msgNum2[i]);
        J2DTextBox* yesNoTxt2 = (J2DTextBox*)mpYesNoTxt_c[i]->getPanePtr();
        yesNoTxt2->setString(0x20, message);
    }
    resource = JKRGetNameResource("zelda_file_select_yes_no_window.bpk", mpArchive);
    field_0x2c = (J2DAnmColor*)J2DAnmLoaderDataBase::load(resource);
    field_0x2c->searchUpdateMaterialID(mpSelectScreen);
    field_0x3c0 = 0;
    resource = JKRGetNameResource("zelda_file_select_yes_no_window.btk", mpArchive);
    field_0x30 = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(resource);
    field_0x30->searchUpdateMaterialID(mpSelectScreen);
    field_0x3c4 = 0;
    for (int i = 0; i < 2; i++) {
        mpYesNoCurWaku_c[i] = new CPaneMgr(mpSelectScreen, l_tagName9[i], 0, NULL);
        mpYesNoCurWakuG0_c[i] = new CPaneMgr(mpSelectScreen, l_tagName10[i], 0, NULL);
        mpYesNoCurWakuG1_c[i] = new CPaneMgr(mpSelectScreen, l_tagName11[i], 0, NULL);
        mpYesNoCurWaku_c[i]->getPanePtr()->setAnimation(field_0x2c);
        mpYesNoCurWakuG0_c[i]->getPanePtr()->setAnimation(field_0x2c);
        mpYesNoCurWakuG1_c[i]->getPanePtr()->setAnimation(field_0x2c);
        mpYesNoCurWakuG0_c[i]->getPanePtr()->setAnimation(field_0x30);
        mpYesNoCurWakuG1_c[i]->getPanePtr()->setAnimation(field_0x30);
    }
    field_0x3f9 = 0;
    field_0x3fa = 0;
    field_0x401 = 0xff;
    field_0x402 = 0xff;
    for (int i = 0; i < 2; i++) {
        field_0x3fb[i] = 0;
        field_0x3fd[i] = 0;
        field_0x3ff[i] = 0;
    }
    field_0x403 = 0;
    screenSet();
    field_0x3e0 = 0;
    field_0x3e1 = 10;
    field_0x3e2 = 0xff;
    field_0x3e3 = 0xc0;
    field_0x3ef = 0;
    field_0x3f0 = 0xff;
    field_0x3f1 = 0xff;
    field_0x3f2 = 0;
    field_0x3f5 = 0;
    field_0x3f3 = 5;
    field_0x3f4 = 5;
    field_0x334 = 0.0f;
    field_0x330 = 0.0f;
    field_0x338 = 1.0f;
    field_0x3f7 = 0;
    setZButtonString(1);
    field_0x378 = 0.0f;
    field_0x37c = 0.0f;
    field_0x380 = 1.0f;
    for (int i = 0; i < 6; i++) {
        field_0x384[i] = 0.0f;
        field_0x39c[i] = 0.0f;
    }
    field_0x3b8.r = 0xff;
    field_0x3b8.g = 0xff;
    field_0x3b8.b = 0xff;
    field_0x3b8.a = 0xff;
    field_0x3bc.r = 0xff;
    field_0x3bc.g = 0xff;
    field_0x3bc.b = 0xff;
    field_0x3bc.a = 0xff;
    initialize();
    setHIO(true);
}

void dMenu_Option_c::_delete() {
    delete mpString;
    mpString = NULL;

    delete mpScreen;
    mpScreen = NULL;

    delete mpMeterHaihai;
    mpMeterHaihai = NULL;

    delete mpBackScreen;
    mpBackScreen = NULL;

    delete mpClipScreen;
    mpClipScreen = NULL;

    delete mpShadowScreen;
    mpShadowScreen = NULL;

    delete mpTVScreen;
    mpTVScreen = NULL;

    if (mpTVButtonAB != NULL) {
        delete mpTVButtonAB;
        mpTVButtonAB = NULL;
    }

    if (mpTVButtonText != NULL) {
        delete mpTVButtonText;
        mpTVButtonText = NULL;
    }

    delete mpScreenIcon;
    mpScreenIcon = NULL;

    for (int i = 0; i < 2; i++) {
        if (mpButtonAB[i] != NULL) {
            delete mpButtonAB[i];
            mpButtonAB[i] = NULL;
        }

        if (mpButtonText[i] != NULL) {
            delete mpButtonText[i];
            mpButtonText[i] = NULL;
        }
    }

    delete mpBlackTex;
    mpBlackTex = NULL;

    delete mpWarning;
    mpWarning = NULL;

    delete mpSelectScreen;
    mpSelectScreen = NULL;

    delete field_0x24;
    field_0x24 = NULL;

    delete field_0x28;
    field_0x28 = NULL;

    delete field_0x20;
    field_0x20 = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpYesNoSelBase_c[i];
        mpYesNoSelBase_c[i] = NULL;

        delete mpYesNoTxt_c[i];
        mpYesNoTxt_c[i] = NULL;

        delete mpYesNoCurWaku_c[i];
        mpYesNoCurWaku_c[i] = NULL;

        delete mpYesNoCurWakuG0_c[i];
        mpYesNoCurWakuG0_c[i] = NULL;

        delete mpYesNoCurWakuG1_c[i];
        mpYesNoCurWakuG1_c[i] = NULL;
    }

    delete field_0x2c;
    field_0x2c = NULL;

    delete field_0x30;
    field_0x30 = NULL;

    if (isUseFlag(1)) {
        delete mpStick;
        mpStick = NULL;
    }

    delete mpTitle;
    mpTitle = NULL;

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    for (int i = 0; i < 5; i++) {
        delete mpParent[i];
        mpParent[i] = NULL;
    }

    for (int i = 0; i < 5; i++) {
        delete mpHaihaiPosL[i];
        mpHaihaiPosL[i] = NULL;

        delete mpHaihaiPosR[i];
        mpHaihaiPosR[i] = NULL;
    }

    for (int i = 0; i < 6; i++) {
        delete mpMenuNull[i];
        mpMenuNull[i] = NULL;

        delete mpMenuPane[i];
        mpMenuPane[i] = NULL;

        if (mpMenuPaneC[i] != NULL) {
            delete mpMenuPaneC[i];
            mpMenuPaneC[i] = NULL;
        }

        if (mpMenuPane2[i] != NULL) {
            delete mpMenuPane2[i];
            mpMenuPane2[i] = NULL;
        }

        if (mpMenuPane3[i] != NULL) {
            delete mpMenuPane3[i];
            mpMenuPane3[i] = NULL;
        }

        if (mpMenuPane32[i] != NULL) {
            delete mpMenuPane32[i];
            mpMenuPane32[i] = NULL;
        }
    }

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (mpMenuText[i][j] != NULL) {
                delete mpMenuText[i][j];
                mpMenuText[i][j] = NULL;
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        if (field_0x1c0[i] != NULL) {
            delete field_0x1c0[i];
            field_0x1c0[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mpZButtonText[i] != NULL) {
            delete mpZButtonText[i];
            mpZButtonText[i] = NULL;
        }
    }

    if (mpMount != NULL) {
        mpMount->getArchive()->unmount();
        delete mpMount;
        mpMount = NULL;
    }

    if (mpArchive != NULL) {
        mpArchive->unmount();
        mpArchive = NULL;
    }

    dComIfGp_setOptionResArchive(NULL);
}

void dMenu_Option_c::_move() {
    mpStick->checkTrigger();
    if (field_0x3ea != 0 && !isRumbleSupported()) {
        field_0x3ea = 0;
    }
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        if (mDoCPd_c::getTrigA(PAD_1) != 0 && field_0x3ef != SelectType3 && field_0x3f3 == 5) {
            if (field_0x3f4 == 5 && field_0x3ef != SelectType4 && field_0x3ef != SelectType5 && field_0x3ef != SelectType6 &&
                field_0x3ef != SelectType7)
            {
                if (mDoCPd_c::getTrigStart(PAD_1) == 0 && mDoCPd_c::getTrigB(PAD_1) == 0) {
                    if (mDoCPd_c::getTrigUp(PAD_1) == 0 && mDoCPd_c::getTrigDown(PAD_1) == 0 &&
                        mDoCPd_c::getTrigLeft(PAD_1) == 0 && mDoCPd_c::getTrigRight(PAD_1) == 0)
                    {
                        field_0x3f7 = 1;
                        field_0x3f5 = field_0x3ef;
                        field_0x3ef = SelectType4;
                        dMeter2Info_set2DVibration();
                        (this->*init[field_0x3ef])();
                        goto skip;
                    }
                }
            }
        }
        if (mDoCPd_c::getTrigB(PAD_1) != 0 && field_0x3ef != SelectType3 && field_0x3f3 == 5 &&
            field_0x3ef != SelectType4 && field_0x3ef != SelectType5 && field_0x3ef != SelectType6 && field_0x3ef != SelectType7)
        {
            if (field_0x3f4 == 5 && mDoCPd_c::getTrigStart(PAD_1) == 0 &&
                mDoCPd_c::getTrigA(PAD_1) == 0 && mDoCPd_c::getTrigUp(PAD_1) == 0 &&
                mDoCPd_c::getTrigDown(PAD_1) == 0 && mDoCPd_c::getTrigLeft(PAD_1) == 0 &&
                mDoCPd_c::getTrigRight(PAD_1) == 0)
            {
                field_0x3f7 = 0;
                field_0x3f5 = field_0x3ef;
                field_0x3ef = SelectType4;
                dMeter2Info_set2DVibration();
                (this->*init[field_0x3ef])();
            }
        }
    }
skip:
    u8 oldValue = field_0x3ef;
    if (field_0x3f3 == 5 && oldValue != SelectType4 && oldValue != SelectType5 && oldValue != SelectType6 && oldValue != SelectType7) {
        dpdMenuMove();
    }
    field_0x3f2 = 0;
    if (field_0x3f1 != 0xff) {
        if (field_0x3f0 != field_0x3f1 && field_0x3ef != field_0x3f1) {
            field_0x3f0 = field_0x3f1;
            field_0x3f2 = 1;
        }
    } else {
        field_0x3f0 = 0xff;
    }
    (this->*process[field_0x3ef])();
    mpSelectScreen->animation();
    if (oldValue != field_0x3ef) {
        (this->*init[field_0x3ef])();
    }
    setHIO(false);
}

void dMenu_Option_c::_draw() {
    if (mpArchive != NULL) {
        J2DGrafContext* ctx = dComIfGp_getCurrentGrafPort();
        mpBlackTex->setAlpha(0xff);
        mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
        mpBackScreen->draw(0.0f, 0.0f, ctx);
        f32 alpha = (f32)g_drawHIO.mOptionScreen.mBackgroundAlpha * (f32)field_0x374;
        mpBlackTex->setAlpha(alpha);
        mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
        mpScreen->draw(0.0f, 0.0f, ctx);
        mpClipScreen->draw(0.0f, 0.0f, ctx);
        mpShadowScreen->draw(0.0f, 0.0f, ctx);
        if (field_0x3f3 == 1 || field_0x3f3 == 2 || field_0x3f3 == 3) {
            mpTVScreen->draw(0.0f, 0.0f, ctx);
        }
        if (field_0x3f3 == 5) {
            mpDrawCursor->draw();
        }
        drawHaihai();
        mpWarning->drawSelf();
        mpSelectScreen->draw(0.0f, 0.0f, ctx);
        if (field_0x3f3 != 1 && field_0x3f3 != 2 && field_0x3f3 != 3) {
            mpScreenIcon->draw(0.0f, 0.0f, ctx);
        }
    }
}

void dMenu_Option_c::drawHaihai() {
    CPaneMgr pane;
    u8 selectType = getSelectType();
    field_0x3f6 = 0;
    field_0x3f6 |= 1;
    field_0x3f6 |= 4;
    if (selectType < SelectType4 && field_0x3f6 != 0 && field_0x3f3 == 5 && field_0x3ef != SelectType4 &&
        field_0x3ef != SelectType5 && field_0x3ef != SelectType6 && field_0x3ef != SelectType7)
    {
        mpMeterHaihai->_execute(0);
        Vec haihaiPosL =
            mpHaihaiPosL[selectType]->getGlobalVtxCenter(mpHaihaiPosL[selectType]->mPane, false, 0);
        Vec haihaiPosR =
            mpHaihaiPosR[selectType]->getGlobalVtxCenter(mpHaihaiPosR[selectType]->mPane, false, 0);
        haihaiPosL.x += g_drawHIO.mOptionScreen.mArrowOffsetX_4x3;
        haihaiPosR.x -= g_drawHIO.mOptionScreen.mArrowOffsetX_4x3;

        f32 haihaiX = haihaiPosR.x - haihaiPosL.x;
        f32 haihaiY = haihaiPosR.y - haihaiPosL.y;
        mpMeterHaihai->drawHaihai(field_0x3f6, haihaiPosL.x + haihaiX / 2 + -5.0f + 4.3f,
                                  haihaiPosL.y + haihaiY / 2 + -1.0f, haihaiX, haihaiY);
        field_0x3f6 = 0;
    }
}

bool dMenu_Option_c::isSync() {
    if (mpMount != NULL && mpMount->sync() == false) {
        return 0;
    }
    return 1;
}

bool dMenu_Option_c::checkLeftTrigger() {
    return mpStick->checkLeftTrigger();
}

bool dMenu_Option_c::checkRightTrigger() {
    return mpStick->checkRightTrigger();
}

void dMenu_Option_c::setAnimation() {
    s16 frameMax;

    field_0x3c0 += 2;
    frameMax = field_0x2c->getFrameMax();
    if (field_0x3c0 >= frameMax) {
        field_0x3c0 = field_0x3c0 - frameMax;
    }
    field_0x2c->setFrame(field_0x3c0);

    field_0x3c4 += 2;
    frameMax = field_0x30->getFrameMax();
    if (field_0x3c4 >= frameMax) {
        field_0x3c4 = field_0x3c4 - frameMax;
    }
    field_0x30->setFrame(field_0x3c4);
}

bool dMenu_Option_c::_open() {
    if (!mpMount) {
        mpMount = mDoDvdThd_mountArchive_c::create("/res/Layout/optres.arc", 0, NULL);
    }
    if (mpMount->sync() != 0) {
        if (!dComIfGp_getOptionResArchive()) {
            dComIfGp_setOptionResArchive(mpMount->getArchive());
            mpArchive = dComIfGp_getOptionResArchive();
            delete mpMount;
            mpMount = NULL;
            _create();
        }
    } else {
        return 0;
    }

    s16 openFrame = g_drawHIO.mOptionScreen.mOpenFrames;
    s16 closeFrame = g_drawHIO.mOptionScreen.mCloseFrames;
    mFrame = openFrame;
    setHIO(false);
    if (mFrame >= openFrame) {
        mFrame = closeFrame;
        mQuitStatus = 2;
        field_0x3ef = 0;
        atten_init();
        for (int i = 0; i < 5; i++) {
            f32 scale = field_0x380;
            mpParent[i]->scale(scale, scale);
            mpParent[i]->setAlphaRate(1.0f);
        }
        setCursorPos(getSelectType());
        cursorAnime(1.0f);
        mpDrawCursor->onPlayAnime(0);
        return 1;
    } else {
        f32 div = (f32)mFrame / (f32)openFrame;
        for (int i = 0; i < 5; i++) {
            f32 scale = div * field_0x380;
            mpParent[i]->scale(scale, scale);
            mpParent[i]->setAlphaRate(div);
        }
        setCursorPos(getSelectType());
        cursorAnime(div);
        return 0;
    }
}

bool dMenu_Option_c::_close() {
    s16 closeFrame = g_drawHIO.mOptionScreen.mCloseFrames;
    mFrame = 0;
    setHIO(false);
    if (mFrame <= 0) {
        mFrame = 0;
        mQuitStatus = 0;
        f32 scale = 0.0f;
        for (int i = 0; i < 5; i++) {
            mpParent[i]->scale(scale, scale);
            mpParent[i]->setAlphaRate(0.0f);
        }
        setCursorPos(getSelectType());
        cursorAnime(0.0f);
        return 1;
    } else {
        f32 div = (f32)mFrame / (f32)closeFrame;
        for (int i = 0; i < 5; i++) {
            f32 scale = div * field_0x380;
            mpParent[i]->scale(scale, scale);
            mpParent[i]->setAlphaRate(div);
        }
        setCursorPos(getSelectType());
        cursorAnime(div);
        return 0;
    }
}

void dMenu_Option_c::atten_init() {
    mpDrawCursor->setAlphaRate(1.0f);
    setCursorPos(0);
    setAButtonString(0x40C);
    setBButtonString(0x3F9);
}

void dMenu_Option_c::atten_move() {
    bool downTrigger = mpStick->checkDownTrigger();
    bool leftTrigger = checkLeftTrigger();
    bool rightTrigger = checkRightTrigger();

    if (field_0x3f3 != 5) {
        (this->*tv_process[field_0x3f3])();
    } else if (downTrigger) {
        field_0x3ef = 1;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (leftTrigger) {
        if (field_0x3e4 == 0) {
            field_0x3e4 = 1;
            field_0x3da = -5;
        } else if (field_0x3e4 == 1) {
            field_0x3e4 = 0;
            field_0x3da = -5;
        }
        field_0x3ef = SelectType3;
        field_0x3f5 = 0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (rightTrigger) {
        if (field_0x3e4 == 0) {
            field_0x3e4 = 1;
            field_0x3da = 5;
        } else if (field_0x3e4 == 1) {
            field_0x3e4 = 0;
            field_0x3da = 5;
        }
        field_0x3ef = SelectType3;
        field_0x3f5 = 0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else {
        changeTVCheck();
    }
}

#if VERSION == VERSION_GCN_JPN
void dMenu_Option_c::ruby_init() {
    mpDrawCursor->setAlphaRate(1.0f);
    setCursorPos(1);
    setAButtonString(0x40c);
    setBButtonString(0x3f9);
}

void dMenu_Option_c::ruby_move() {

    bool upTrigger = mpStick->checkUpTrigger();
    bool downTrigger= mpStick->checkDownTrigger();
    bool leftTrigger = checkLeftTrigger();
    bool rightTrigger = checkRightTrigger();

    if (field_0x3f3 != 5) {
        (this->*tv_process[field_0x3f3])();
    } else if (upTrigger) {
        field_0x3ef = SelectType0_JPN;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (downTrigger) {
        field_0x3ef = SelectType1;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (leftTrigger) {
        if (field_0x3e5_JPN == 0) {
            field_0x3e5_JPN = 1;
            field_0x3da = -5;
        } else if (field_0x3e5_JPN == 1) {
            field_0x3e5_JPN = 0;
            field_0x3da = -5;
        }
        field_0x3ef = SelectType3;
        field_0x3f5 = SelectType0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (rightTrigger) {
        if (field_0x3e5_JPN == 0) {
            field_0x3e5_JPN = 1;
            field_0x3da = 5;
        } else if (field_0x3e5_JPN == 1) {
            field_0x3e5_JPN = 0;
            field_0x3da = 5;
        }
        field_0x3ef = SelectType3;
        field_0x3f5 = SelectType0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else {
        changeTVCheck();
    }
}
#endif

void dMenu_Option_c::vib_init() {
    mpDrawCursor->setAlphaRate(1.0f);
    setCursorPos(SelectType1);
    setAButtonString(0x40C);
    setBButtonString(0x3F9);
}

void dMenu_Option_c::vib_move() {
    bool upTrigger = mpStick->checkUpTrigger();
    bool downTrigger = mpStick->checkDownTrigger();
    bool leftTrigger = checkLeftTrigger();
    bool rightTrigger = checkRightTrigger();

    if (field_0x3f3 != 5) {
        (this->*tv_process[field_0x3f3])();
    } else if (upTrigger) {
        field_0x3ef = SelectType0;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (downTrigger) {
        field_0x3ef = SelectType2;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (leftTrigger) {
        if (isRumbleSupported()) {
            if (field_0x3ea == 0) {
                field_0x3ea = 1;
                mDoCPd_c::startMotorWave(0, &field_0x3e0, JUTGamePad::CRumble::VAL_0, 0x3c);
                field_0x3da = -5;
            } else if (field_0x3ea == 1) {
                field_0x3ea = 0;
                field_0x3da = -5;
            }
            field_0x3ef = SelectType3;
            field_0x3f5 = SelectType1;
            Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }
    } else if (rightTrigger) {
        if (isRumbleSupported()) {
            if (field_0x3ea == 0) {
                field_0x3ea = 1;
                mDoCPd_c::startMotorWave(0, &field_0x3e0, JUTGamePad::CRumble::VAL_0, 0x3c);
                field_0x3da = 5;
            } else if (field_0x3ea == 1) {
                field_0x3ea = 0;
                field_0x3da = 5;
            }
            field_0x3ef = SelectType3;
            field_0x3f5 = SelectType1;
            Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }
    } else {
        changeTVCheck();
    }
}

void dMenu_Option_c::sound_init() {
    mpDrawCursor->setAlphaRate(1.0f);
    setCursorPos(SelectType2);
    setAButtonString(0x40C);
    setBButtonString(0x3F9);
}

void dMenu_Option_c::sound_move() {
    bool upTrigger = mpStick->checkUpTrigger();
    mpStick->checkDownTrigger();
    bool leftTrigger = checkLeftTrigger();
    bool rightTrigger = checkRightTrigger();

    if (field_0x3f3 != 5) {
        (this->*tv_process[field_0x3f3])();
    } else if (upTrigger) {
        field_0x3ef = SelectType1;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (leftTrigger) {
        if (field_0x3e9 == 2) {
            field_0x3e9 = 0;
        } else {
            field_0x3e9++;
        }
        field_0x3da = -5;
        switch (field_0x3e9) {
        case 0:
            Z2GetAudioMgr()->seStart(Z2SE_SY_SOUND_MODE_MONO, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            break;
        case 1:
            Z2GetAudioMgr()->seStart(Z2SE_SY_SOUND_MODE_STEREO, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            break;
        case 2:
            Z2GetAudioMgr()->seStart(Z2SE_SY_SOUND_MODE_SURROUND, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            break;
        }
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(dMo_soundMode[field_0x3e9]);
        setSoundMode(dMo_soundMode[field_0x3e9]);
        field_0x3ef = SelectType3;
        field_0x3f5 = SelectType2;
        Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (rightTrigger) {
        if (field_0x3e9 == 0) {
            field_0x3e9 = 2;
        } else {
            field_0x3e9--;
        }
        field_0x3da = 5;
        switch (field_0x3e9) {
        case 0:
            Z2GetAudioMgr()->seStart(Z2SE_SY_SOUND_MODE_MONO, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            break;
        case 1:
            Z2GetAudioMgr()->seStart(Z2SE_SY_SOUND_MODE_STEREO, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            break;
        case 2:
            Z2GetAudioMgr()->seStart(Z2SE_SY_SOUND_MODE_SURROUND, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            break;
        }
        Z2AudioMgr::mAudioMgrPtr->setOutputMode(dMo_soundMode[field_0x3e9]);
        setSoundMode(dMo_soundMode[field_0x3e9]);
        field_0x3ef = SelectType3;
        field_0x3f5 = SelectType2;
        Z2GetAudioMgr()->seStart(Z2SE_SY_OPTION_SWITCH, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else {
        changeTVCheck();
    }
}

void dMenu_Option_c::change_init() {
    setAButtonString(0x40C);
    setBButtonString(0x3F9);
}

void dMenu_Option_c::change_move() {
    f32 x = 0.0f;

    if (field_0x3da > 0) {
        field_0x3da--;
    } else if (field_0x3da < 0) {
        field_0x3da++;
    }
    u8 index;
    switch (field_0x3f5) {
#if VERSION == VERSION_GCN_JPN
    case SelectType0_JPN:
        index = SelectType0_JPN;
        if (field_0x3da == 0) {
            setAttenString();
        }
        break;
#endif
    case SelectType0:
        index = SelectType0;
        if (field_0x3da == 0) {
#if VERSION == VERSION_GCN_JPN
            setRubyString();
#else
            setAttenString();
#endif
        }
        break;
    case SelectType1:
        index = SelectType1;
        if (field_0x3da == 0) {
            setVibString();
        }
        break;
    case SelectType2:
        index = SelectType2;
        if (field_0x3da == 0) {
            setSoundString();
        }
        break;
    }
    if (field_0x3da > 0) {
        f32 initPosX = (5 - field_0x3da) / 5.0f;
        if (mpMenuText[index][3] != NULL) {
            x = mpMenuText[index][3]->getInitPosX() - mpMenuText[index][0]->getInitPosX();
        }
        x *= initPosX;
    } else if (field_0x3da < 0) {
        f32 initPosX = (field_0x3da + 5) / 5.0f;
        if (mpMenuText[index][5] != NULL) {
            x = mpMenuText[index][5]->getInitPosX() - mpMenuText[index][0]->getInitPosX();
        }
        x *= initPosX;
    }
    for (int i = 0; i < 6; i++) {
        if (mpMenuText[index][i] != NULL) {
            mpMenuText[index][i]->show();
            mpMenuText[index][i]->paneTrans(x + field_0x3b4, 0.0f);
        }
    }
    if (field_0x3da == 0) {
        for (int i = 0; i < 6; i++) {
            for (int j = 2; j < 6; j++) {
                CPaneMgr* menuText = mpMenuText[i][j];
                if (menuText != NULL) {
                    menuText->hide();
                }
            }
        }
        field_0x3ef = field_0x3f5;
    }
}

void dMenu_Option_c::confirm_open_init() {
    if (field_0x3f7 == 1) {
        mpWarning->setText(0x55F);
    } else {
        mpWarning->setText(0x560);
    }
    mpWarning->openInit();
    yesnoMenuMoveAnmInitSet(0x473, 0x47D);
    field_0x403 = getSelectType();
    setSelectColor(field_0x403, true);
    changeBarColor(true);
    setAButtonString(0);
    setBButtonString(0);
    Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Option_c::confirm_open_move() {
    bool status = mpWarning->getStatus();
    bool yesNoMenuMove = yesnoMenuMoveAnm();

    if (field_0x374 != 1.0f) {
        cLib_addCalc2(&field_0x374, 1.0f, 0.4f, 0.5f);
        if (fabsf(field_0x374 - 1.0f) < 0.1f) {
            field_0x374 = 1.0f;
        }
    }
    if (status == 1 && yesNoMenuMove == 1 && field_0x374 == 1.0f) {
        yesnoCursorShow();
        field_0x3ef = SelectType5;
    }
    mpWarning->_move();
    setAnimation();
}

void dMenu_Option_c::confirm_move_init() {
    setAButtonString(0x40C);
    setBButtonString(0x3F9);
}

void dMenu_Option_c::confirm_move_move() {
    bool leftTrigger = checkLeftTrigger();
    bool rightTrigger = checkRightTrigger();

    if (mDoCPd_c::getTrigA(PAD_1) != 0) {
        yesNoSelectStart();
        field_0x3ef = SelectType7;
        dMeter2Info_set2DVibrationM();
    } else if (mDoCPd_c::getTrigB(PAD_1) != 0) {
        field_0x3f9 = 0;
        yesnoCancelAnmSet();
        field_0x3ef = SelectType7;
        dMeter2Info_set2DVibrationM();
    } else if (rightTrigger != 0) {
        if (field_0x3f9 != 0) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            field_0x3fa = field_0x3f9;
            field_0x3f9 = 0;
            yesnoSelectAnmSet();
            field_0x3ef = SelectType6;
        }
    } else if (leftTrigger != 0) {
        if (field_0x3f9 != 1) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
            field_0x3fa = field_0x3f9;
            field_0x3f9 = 1;
            yesnoSelectAnmSet();
            field_0x3ef = SelectType6;
        }
    }
    mpWarning->_move();
    setAnimation();
}

void dMenu_Option_c::confirm_select_init() {
}

void dMenu_Option_c::confirm_select_move() {
    u8 selectMoveAnm = yesnoSelectMoveAnm();
    u8 wakuAlphaAnm = yesnoWakuAlpahAnm(field_0x3fa);

    if (selectMoveAnm == 1 && wakuAlphaAnm == 1) {
        yesnoCursorShow();
        field_0x3ef = SelectType5;
    }
    mpWarning->_move();
    setAnimation();
}

void dMenu_Option_c::confirm_close_init() {
    mpWarning->closeInit();
    setSelectColor(field_0x403, false);
    changeBarColor(false);
    setAButtonString(0);
    setBButtonString(0);
    Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Option_c::confirm_close_move() {
    bool status = mpWarning->getStatus();
    yesnoMenuMoveAnm();
    if (field_0x374 != 0.0f) {
        cLib_addCalc2(&field_0x374, 0.0f, 0.4f, 0.5);
        if (fabsf(field_0x374) < 0.1f) {
            field_0x374 = 0.0f;
        }
    }
    if (status == 1 && status == 1 && field_0x374 == 0.0f) {
        if (field_0x3f7 == 1) {
            if (field_0x3f9 == 1) {
                mQuitStatus = 3;
                dComIfGs_setOptAttentionType(field_0x3e4);
#if VERSION == VERSION_GCN_JPN
                if (field_0x3e5_JPN == 0) {
                    dComIfGs_setOptUnk0(1);
                } else {
                    dComIfGs_setOptUnk0(0);
                }
#endif
                if (isRumbleSupported()) {
                    dComIfGs_setOptVibration(field_0x3ea);
                }
                dComIfGs_setOptSound(field_0x3e9);
                dComIfGp_setNowVibration(field_0x3ea);
                dComIfGs_setOptCameraControl(field_0x3e5);
                mpDrawCursor->offPlayAnime(0);
            } else {
                mpDrawCursor->setParam(1.01f, 0.85f, 0.02f, 0.5f, 0.5f);
                field_0x3ef = field_0x3f5;
            }
        } else if (field_0x3f9 == 1) {
            mQuitStatus = 3;
            if (field_0x3e9 != dComIfGs_getOptSound()) {
                field_0x3e9 = dComIfGs_getOptSound();
                Z2GetAudioMgr()->setOutputMode(dMo_soundMode[field_0x3e9]);
                setSoundMode(dMo_soundMode[field_0x3e9]);
            }
            mpDrawCursor->offPlayAnime(0);
        } else {
            mpDrawCursor->setParam(1.01f, 0.85f, 0.02f, 0.5f, 0.5f);
            field_0x3ef = field_0x3f5;
        }
    }
    mpWarning->_move();
    setAnimation();
}

void dMenu_Option_c::tv_open1_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 0) {
        dMw_c::dMw_fade_in();
        field_0x330 = 0.0f;
        field_0x3f3 = 1;
        setAButtonString(0);
        setBButtonString(0x3F9);
        setCursorPos(getSelectType());
    }
}

void dMenu_Option_c::tv_open2_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        field_0x3f3 = 2;
    }
}

void dMenu_Option_c::tv_move_move() {
    if (mDoCPd_c::getTrigZ(PAD_1) != 0 || mDoCPd_c::getTrigA(PAD_1) != 0) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_BACK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        field_0x3f3 = 3;
        setAButtonString(0x40C);
        setBButtonString(0x3F9);
        dMw_c::dMw_fade_out();
        dMeter2Info_set2DVibration();
    }
}

void dMenu_Option_c::tv_close1_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 0) {
        dMw_c::dMw_fade_in();
        field_0x3f3 = 4;
        setZButtonString(1);
        setCursorPos(getSelectType());
    }
}

void dMenu_Option_c::tv_close2_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        field_0x3f3 = 5;
    }
}

void dMenu_Option_c::calibration_open1_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 0) {
        dMw_c::dMw_fade_in();
        field_0x3f4 = 1;
    }
}

void dMenu_Option_c::calibration_open2_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        field_0x3f4 = 2;
    }
}

void dMenu_Option_c::calibration_move_move() {
    if (mpCalibration->mStatus == 0) {
        dMw_c::dMw_fade_out();
        field_0x3f4 = 3;
    } else if (mpCalibration->mStatus == 1) {
        mpCalibration->_move();
    }
}

void dMenu_Option_c::calibration_close1_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 0) {
        dMw_c::dMw_fade_in();
        field_0x3f4 = 4;
    }
}

void dMenu_Option_c::calibration_close2_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        field_0x3f4 = 5;
    }
}

void dMenu_Option_c::menuVisible() {
    for (int i = 0; i < 6; i++) {
        if (i < SelectType3) {
            menuShow(i);
        } else {
            menuHide(i);
        }
    }
}

void dMenu_Option_c::menuShow(int index) {
    mpMenuNull[index]->show();
    mpMenuPane[index]->show();
    if (mpMenuPaneC[index] != NULL) {
        mpMenuPaneC[index]->show();
    }
    if (mpMenuPane2[index] != NULL) {
        mpMenuPane2[index]->show();
    }
    if (mpMenuPane3[index] != NULL) {
        mpMenuPane3[index]->show();
    }
    for (int i = 0; i < 2; i++) {
        if (mpMenuText[index][i] != NULL) {
            mpMenuText[index][i]->show();
        }
    }
}

void dMenu_Option_c::menuHide(int index) {
    mpMenuNull[index]->hide();
    mpMenuPane[index]->hide();
    if (mpMenuPaneC[index] != NULL) {
        mpMenuPaneC[index]->hide();
    }
    if (mpMenuPane2[index] != NULL) {
        mpMenuPane2[index]->hide();
    }
    if (mpMenuPane3[index] != NULL) {
        mpMenuPane3[index]->hide();
    }
    for (int i = 0; i < 2; i++) {
        if (mpMenuText[index][i] != NULL) {
            mpMenuText[index][i]->hide();
        }
    }
}

void dMenu_Option_c::screenSet() {
    static const u64 tag_frame[6] = {
        'flame_00', 'flame_01', 'flame_02', 'flame_03', 'flame_04', 'flame_05',
    };
#if VERSION == VERSION_GCN_JPN
    static const u64 tag_menu0[6] = {
        'menu_t0', 'menu_t1', 'menu_t2', 'menu_t3', 'menu_t4', 'menu_t5',
    };
#else
    static const u64 tag_menu0[6] = {
        'fenu_t0', 'fenu_t1', 'fenu_t2', 'fenu_t3', 'fenu_t4', 'fenu_t5',
    };
#endif
    static const u64 let_n[6] = {
        'let_00_n', 'let_01_n', 'let_02_n', 'let_03_n', 'let_04_n', 'let_05_n',
    };
    static const u64 let2_n[6] = {
        'let_00_n', 'let_01_n', 'let_02_n', 'let_03_n', 'let_04_n',
    };
    static const u64 menu_n[6] = {
        'menu_n0', 'menu_n1', 'menu_n2', 'menu_n3', 'menu_n4', 'menu_n5',
    };
    static const u64 menu2_n[6] = {
        'mw_n_0', 'mw_n_1', 'mw_n_2', 'mw_n_3', 'mw_n_4',
    };
    static const u64 al0_n[6] = {
        's_grr_00', 's_grr_01', 's_grr_02', 's_grr_03', 's_grr_04', 's_grr_05',
    };
    static const u64 al1_n[6] = {
        'c_grr_00', 'c_grr_01', 'c_grr_02', 'c_grr_03', 'c_grr_04', 'c_grr_05',
    };
    static const u64 al2_n[6] = {
        's_grl_00', 's_grl_01', 's_grl_02', 's_grl_03', 's_grl_04', 's_grl_05',
    };
    static const u64 al3_n[6] = {
        'c_grl_00', 'c_grl_01', 'c_grl_02', 'c_grl_03', 'c_grl_04', 'c_grl_05',
    };
    static const u64 haihail_n[5] = {
        'y_set_l0', 'y_set_l1', 'y_set_l2', 'y_set_l3', 'y_set_l4',
    };
    static const u64 haihair_n[5] = {
        'y_set_r0', 'y_set_r1', 'y_set_r2', 'y_set_r3', 'y_set_r4',
    };
    static const u64 menu3_n[6] = {
        'menuapn0', 'menuapn1', 'menuapn2', 'menuapn3', 'menuapn4',
    };
    static const u64 tv_btnA[5] = {
        'cont_at1', 'cont_at2', 'cont_at3', 'cont_at4', 'cont_at',
    };
    static const u64 ftv_btnA[5] = {
        'font_a1', 'font_at2', 'font_at3', 'font_at4', 'font_at',
    };
#if VERSION == VERSION_GCN_JPN
    static const u64 fenu_t0[2] = {'fenu_t0s', 'fenu_t0'};
    static const u64 menu_t0[2] = {'menu_t0s', 'menu_t0'};
    static const u64 fenu_t1[2] = {'fenu_t1s', 'fenu_t1'};
    static const u64 menu_t1[2] = {'menu_t1s', 'menu_t1'};
    static const u64 fenu_t2[2] = {'fenu_t2s', 'fenu_t2'};
    static const u64 menu_t2[2] = {'menu_t2s', 'menu_t2'};
    static const u64 fenu_t3[2] = {'fenu_t3s', 'fenu_t3'};
    static const u64 menu_t3[2] = {'menu_t3s', 'menu_t3'};
    static const u64 fenu_t4[2] = {'fenu_t4s', 'fenu_t4'};
    static const u64 menu_t4[2] = {'menu_t4s', 'menu_t4'};
    static const u64 fenu_t5[2] = {'fenu_t5s', 'fenu_t5'};
    static const u64 menu_t5[2] = {'menu_t5s', 'menu_t5'};
#else
    static const u64 fenu_t0[2] = {'fenu_t0s', 'fenu_t0'};
    static const u64 menu_t0[2] = {'menu_t0s', 'menu_t0'};
    static const u64 fenu_t2[2] = {'fenu_t1s', 'fenu_t1'};
    static const u64 menu_t2[2] = {'menu_t1s', 'menu_t1'};
    static const u64 fenu_t3[2] = {'fenu_t2s', 'fenu_t2'};
    static const u64 menu_t3[2] = {'menu_t2s', 'menu_t2'};
    static const u64 fenu_t4[2] = {'fenu_t3s', 'fenu_t3'};
    static const u64 menu_t4[2] = {'menu_t3s', 'menu_t3'};
    static const u64 fenu_t1[2] = {'fenu_t4s', 'fenu_t4'};
    static const u64 menu_t1[2] = {'menu_t4s', 'menu_t4'};
    static const u64 fenu_t5[2] = {'fenu_t5s', 'fenu_t5'};
    static const u64 menu_t5[2] = {'menu_t5s', 'menu_t5'};
#endif
    static const u64 menut_0[6] = {
        'menut0as', 'menut0a', 'menut0a2', 'menut0a1', 'menut0a4', 'menut0a3',
    };
    static const u64 fenut_0[6] = {
        'menut010', 'menut0a9', 'menut0a8', 'menut0a7', 'menut0a6', 'menut0a5',
    };
    static const u64 menut_1[6] = {
        'menut1as', 'menut1a', 'menut1a2', 'menut1a1', 'menut1a4', 'menut1a3',
    };
    static const u64 fenut_1[6] = {
        'menut110', 'menut1a9', 'menut1a8', 'menut1a7', 'menut1a6', 'menut1a5',
    };
    static const u64 menut_2[6] = {
        'menut2as', 'menut2a', 'menut2a2', 'menut2a1', 'menut2a4', 'menut2a3',
    };
    static const u64 fenut_2[6] = {
        'menut210', 'menut2a9', 'menut2a8', 'menut2a7', 'menut2a6', 'menut2a5',
    };
    static const u64 menut_3[6] = {
        'menut3a5', 'menut3a6', 'menut3a7', 'menut3a8', 'menut3a9', 'menut310',
    };
    static const u64 fenut_3[6] = {
        'menut315', 'menut314', 'menut313', 'menut312', 'menut311', 'menut001',
    };
    static const u64 menut_4[6] = {
        'menut3as', 'menut3a', 'menut3a2', 'menut3a1', 'menut3a4', 'menut3a3',
    };
    static const u64 fenut_4[6] = {
        'menut321', 'menut320', 'menut319', 'menut318', 'menut317', 'menut316',
    };
#if VERSION == VERSION_GCN_JPN
    static const u64 tx[6] = {
        'wps_text', 'w_p_text', 'g_ps_tx3', 'g_p_tex3', 'wps_tex1', 'w_p_tex1',
    };
#else
    static const u64 tx[6] = {
        'w_p_tex5', 'w_p_tex6', 'w_p_tex3', 'w_p_tex4', 'fps_tex1', 'f_p_tex1',
    };
#endif
    static const u64 op_tx[4] = {
        'w_text_n', 'w_btn_n', 'w_k_t_n', 'w_abtn_n',
    };
    static const u64 z_tx[3] = {
        'z_gc_n', 0, 0,
    };
#if VERSION == VERSION_GCN_JPN
    static const u64 txTV[10] = {
        'menu_t6s', 'menu_t6',  'menu_t9s', 'menu_t9',  'menut10s',
        'menu_t10', 'menu_t7s', 'menu_t7',  'menu_t8s', 'menu_t8',
    };
#else
    static const u64 txTV[10] = {
        'menu_t61', 'menu_t2',  'menu_t91', 'menu_t1',  'menut101',
        'menu_t01', 'menu_t71', 'menu_t3',  'menu_t81', 'menu_t4',
    };
#endif

    mpTitle = new CPaneMgr(mpBackScreen, 'title_n', 0, NULL);
    Vec pos = mpTitle->getGlobalVtxCenter(mpTitle->mPane, false, 0);
    mpWarning->mPosY = pos.y + g_drawHIO.mOptionScreen.mBackgroundPosY;
    for (int i = 0; i < 6; i++) {
        field_0x280[i] = (J2DPicture*)mpScreen->search(tag_frame[i]);
        field_0x298[i] = (J2DTextBox*)mpScreen->search(tag_menu0[i]);
    }
    field_0x2b0[0] = field_0x280[1]->getBlack();
    field_0x2c0[0] = field_0x280[1]->getWhite();
    field_0x2b0[1] = field_0x298[1]->getBlack();
    field_0x2c0[1] = field_0x298[1]->getWhite();
    field_0x2b8[0] = field_0x280[0]->getBlack();
    field_0x2c8[0] = field_0x280[0]->getWhite();
    field_0x2b8[1] = field_0x298[0]->getBlack();
    field_0x2c8[1] = field_0x298[0]->getWhite();
    field_0x2b8[0].a = 0xff;
    field_0x2b0[0].a = 0xff;
    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setAlphaRate(0.0f);
    mpDrawCursor->setScale(0.0f);
    mpDrawCursor->setParam(1.01f, 0.85f, 0.02f, 0.5f, 0.5f);
    mpDrawCursor->offPlayAnime(0);
    mpParent[0] = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
    mpParent[1] = new CPaneMgr(mpClipScreen, 'n_all', 2, NULL);
    mpParent[2] = new CPaneMgr(mpShadowScreen, 'nall', 2, NULL);
    mpParent[3] = new CPaneMgr(mpTVScreen, 'n_all', 2, NULL);
    mpParent[4] = new CPaneMgr(mpBackScreen, 'n_all', 2, NULL);
    for (int i = 0; i < 6; i++) {
        mpMenuNull[i] = new CPaneMgr(mpScreen, let_n[i], 0, NULL);
        mpMenuPane[i] = new CPaneMgr(mpScreen, menu_n[i], 0, NULL);
        if (menu2_n[i] != 0) {
            mpMenuPaneC[i] = new CPaneMgr(mpScreen, menu2_n[i], 0, NULL);
            Vec pos = mpMenuPaneC[i]->getGlobalVtxCenter(mpMenuPaneC[i]->mPane, false, 0);
            field_0x344[i] = pos.x;
            field_0x35c[i] = pos.y;
        } else {
            mpMenuPaneC[i] = NULL;
            field_0x344[i] = 0.0f;
            field_0x35c[i] = 0.0f;
        }
        field_0x2d0[i][0] = mpScreen->search(al0_n[i]);
        field_0x2d0[i][1] = mpScreen->search(al1_n[i]);
        field_0x2d0[i][2] = mpScreen->search(al2_n[i]);
        field_0x2d0[i][3] = mpScreen->search(al3_n[i]);
    }
    for (int i = 0; i < 4; i++) {
        field_0x404[i] = field_0x2d0[1][i]->getAlpha();
        field_0x408[i] = field_0x2d0[0][i]->getAlpha();
    }
    for (int i = 0; i < 5; i++) {
        mpHaihaiPosL[i] = new CPaneMgr(mpScreen, haihail_n[i], 0, NULL);
        mpHaihaiPosR[i] = new CPaneMgr(mpScreen, haihair_n[i], 0, NULL);
    }
    for (int i = 0; i < 6; i++) {
        if (let2_n[i] != 0) {
            mpMenuPane2[i] = new CPaneMgr(mpShadowScreen, let2_n[i], 0, NULL);
        } else {
            mpMenuPane2[i] = NULL;
        }
    }
    for (int i = 0; i < 6; i++) {
        if (let2_n[i] != NULL) {
            mpMenuPane3[i] = new CPaneMgr(mpClipScreen, let2_n[i], 0, NULL);
        } else {
            mpMenuPane3[i] = NULL;
        }
        if (menu3_n[i] != NULL) {
            mpMenuPane32[i] = new CPaneMgr(mpClipScreen, menu3_n[i], 0, NULL);
        } else {
            mpMenuPane32[i] = NULL;
        }
    }
#if VERSION == VERSION_GCN_JPN
    field_0x270[0] = (J2DTextBox*)mpBackScreen->search('t_t00');
    field_0x270[1] = (J2DTextBox*)mpBackScreen->search('t_t01');
    mpBackScreen->search('f_t00')->hide();
    mpBackScreen->search('t_t01')->hide();
    field_0x270[2] = (J2DTextBox*)mpTVScreen->search('t_t00');
    mpTVScreen->search('f_t00')->hide();
#else
    field_0x270[0] = (J2DTextBox*)mpBackScreen->search('f_t00');
    field_0x270[1] = (J2DTextBox*)mpBackScreen->search('t_t01');
    mpBackScreen->search('t_t00')->hide();
    mpBackScreen->search('t_t01')->hide();
    field_0x270[2] = (J2DTextBox*)mpTVScreen->search('f_t00');
    mpTVScreen->search('t_t00')->hide();
#endif
    for (int i = 0; i < 3; i++) {
        field_0x270[i]->setFont(mDoExt_getRubyFont());
        field_0x270[i]->setString(0x40, "");
    }
    mpString->getString(0x547, field_0x270[0], NULL, NULL, NULL, 0);
    mpString->getString(0x547, field_0x270[1], NULL, NULL, NULL, 0);
    mpString->getString(0x55C, field_0x270[2], NULL, NULL, NULL, 0);
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x25c[i] = (J2DTextBox*)mpTVScreen->search(tv_btnA[i]);
        mpTVScreen->search(ftv_btnA[i])->hide();
#else
        field_0x25c[i] = (J2DTextBox*)mpTVScreen->search(ftv_btnA[i]);
        mpTVScreen->search(tv_btnA[i])->hide();
#endif
        field_0x25c[i]->setFont(mDoExt_getMesgFont());
        field_0x25c[i]->setString(0x40, "");
        mpString->getString(0x564, field_0x25c[i], NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x21c[0][i] = (J2DTextBox*)mpScreen->search(menu_t0[i]);
        mpScreen->search(fenu_t0[i])->hide();
#else
        field_0x21c[0][i] = (J2DTextBox*)mpScreen->search(fenu_t0[i]);
        mpScreen->search(menu_t0[i])->hide();
#endif
        field_0x21c[0][i]->setFont(mpFont);
        field_0x21c[0][i]->setString(0x40, "");
        mpString->getString(0x548, field_0x21c[0][i], NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x21c[1][i] = (J2DTextBox*)mpScreen->search(menu_t1[i]);
        mpScreen->search(fenu_t1[i])->hide();
#else
        field_0x21c[1][i] = (J2DTextBox*)mpScreen->search(fenu_t2[i]);
        mpScreen->search(menu_t2[i])->hide();
#endif
        field_0x21c[1][i]->setFont(mpFont);
        field_0x21c[1][i]->setString(0x40, "");
#if VERSION == VERSION_GCN_JPN
        mpString->getString(0x54B, field_0x21c[1][i], NULL, NULL, NULL, 0);
#else
        mpString->getString(0x54E, field_0x21c[1][i], NULL, NULL, NULL, 0);
#endif
    }
    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x21c[2][i] = (J2DTextBox*)mpScreen->search(menu_t2[i]);
        mpScreen->search(fenu_t2[i])->hide();
#else
        field_0x21c[2][i] = (J2DTextBox*)mpScreen->search(fenu_t3[i]);
        mpScreen->search(menu_t3[i])->hide();
#endif
        field_0x21c[2][i]->setFont(mpFont);
        field_0x21c[2][i]->setString(0x40, "");
#if VERSION == VERSION_GCN_JPN
        mpString->getString(0x54E, field_0x21c[2][i], NULL, NULL, NULL, 0);
#else
        mpString->getString(0x54F, field_0x21c[2][i], NULL, NULL, NULL, 0);
#endif
    }
    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x21c[3][i] = (J2DTextBox*)mpScreen->search(menu_t3[i]);
        mpScreen->search(fenu_t3[i])->hide();
#else
        field_0x21c[3][i] = (J2DTextBox*)mpScreen->search(fenu_t4[i]);
        mpScreen->search(menu_t4[i])->hide();
#endif
        field_0x21c[3][i]->setFont(mpFont);
        field_0x21c[3][i]->setString(0x40, "");
#if VERSION == VERSION_GCN_JPN
        mpString->getString(0x54F, field_0x21c[3][i], NULL, NULL, NULL, 0);
#endif
    }
    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x21c[4][i] = (J2DTextBox*)mpScreen->search(menu_t4[i]);
        mpScreen->search(fenu_t4[i])->hide();
#else
        field_0x21c[4][i] = (J2DTextBox*)mpScreen->search(fenu_t1[i]);
        mpScreen->search(menu_t1[i])->hide();
#endif
        field_0x21c[4][i]->setFont(mpFont);
        field_0x21c[4][i]->setString(0x40, "");
    }
    for (int i = 0; i < 2; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x21c[5][i] = (J2DTextBox*)mpScreen->search(menu_t5[i]);
        mpScreen->search(fenu_t5[i])->hide();
#else
        field_0x21c[5][i] = (J2DTextBox*)mpScreen->search(fenu_t5[i]);
        mpScreen->search(menu_t5[i])->hide();
#endif
        field_0x21c[5][i]->setFont(mpFont);
        field_0x21c[5][i]->setString(0x40, "");
        mpString->getString(0x554, field_0x21c[5][i], NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        paneResize(menut_0[i]);
        mpMenuText[0][i] = new CPaneMgr(mpClipScreen, menut_0[i], 0, NULL);
        mpClipScreen->search(fenut_0[i])->hide();
#else
        paneResize(fenut_0[i]);
        mpMenuText[0][i] = new CPaneMgr(mpClipScreen, fenut_0[i], 0, NULL);
        mpClipScreen->search(menut_0[i])->hide();
#endif
        ((J2DTextBox*)(mpMenuText[0][i]->getPanePtr()))->setFont(mpFont);
        ((J2DTextBox*)(mpMenuText[0][i]->getPanePtr()))->setString(0x40, "");
        mpMenuText[0][i]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        paneResize(menut_1[i]);
        mpMenuText[1][i] = new CPaneMgr(mpClipScreen, menut_1[i], 0, NULL);
        mpClipScreen->search(fenut_1[i])->hide();
#else
        paneResize(fenut_1[i]);
        mpMenuText[1][i] = new CPaneMgr(mpClipScreen, fenut_1[i], 0, NULL);
        mpClipScreen->search(menut_1[i])->hide();
#endif
        mpMenuText[1][i]->show();
        ((J2DTextBox*)(mpMenuText[1][i]->getPanePtr()))->setFont(mpFont);
        ((J2DTextBox*)(mpMenuText[1][i]->getPanePtr()))->setString(0x40, "");
        mpMenuText[1][i]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        paneResize(menut_2[i]);
        mpMenuText[2][i] = new CPaneMgr(mpClipScreen, menut_2[i], 0, NULL);
        mpClipScreen->search(fenut_2[i])->hide();
#else
        paneResize(fenut_2[i]);
        mpMenuText[2][i] = new CPaneMgr(mpClipScreen, fenut_2[i], 0, NULL);
        mpClipScreen->search(menut_2[i])->hide();
#endif
        mpMenuText[2][i]->show();
        ((J2DTextBox*)(mpMenuText[2][i]->getPanePtr()))->setFont(mpFont);
        ((J2DTextBox*)(mpMenuText[2][i]->getPanePtr()))->setString(0x40, "");
        mpMenuText[2][i]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        paneResize(menut_3[i]);
        mpMenuText[3][i] = new CPaneMgr(mpClipScreen, menut_3[i], 0, NULL);
        mpClipScreen->search(fenut_3[i])->hide();
#else
        paneResize(fenut_3[i]);
        mpMenuText[3][i] = new CPaneMgr(mpClipScreen, fenut_3[i], 0, NULL);
        mpClipScreen->search(menut_3[i])->hide();
#endif
        mpMenuText[3][i]->show();
        ((J2DTextBox*)(mpMenuText[3][i]->getPanePtr()))->setFont(mpFont);
        ((J2DTextBox*)(mpMenuText[3][i]->getPanePtr()))->setString(0x40, "");
        mpMenuText[3][i]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        paneResize(menut_4[i]);
        mpMenuText[4][i] = new CPaneMgr(mpClipScreen, menut_4[i], 0, NULL);
        mpClipScreen->search(fenut_4[i])->hide();
#else
        paneResize(fenut_4[i]);
        mpMenuText[4][i] = new CPaneMgr(mpClipScreen, fenut_4[i], 0, NULL);
        mpClipScreen->search(menut_4[i])->hide();
#endif
        mpMenuText[4][i]->show();
        ((J2DTextBox*)(mpMenuText[4][i]->getPanePtr()))->setFont(mpFont);
        ((J2DTextBox*)(mpMenuText[4][i]->getPanePtr()))->setString(0x40, "");
        mpMenuText[4][i]->getPanePtr()->setBasePosition(J2DBasePosition_4);
    }
    for (int i = 0; i < 6; i++) {
        mpMenuText[5][i] = NULL;
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 2; j < 6; j++) {
            if (mpMenuText[i][j] != NULL) {
                mpMenuText[i][j]->hide();
            }
        }
    }
    field_0x3b4 = 0.0f;
    menuVisible();
#if VERSION == VERSION_GCN_JPN
    mpBackScreen->search('jpn_n')->show();
    mpBackScreen->search('foregn_n')->hide();
#else
    mpBackScreen->search('jpn_n')->hide();
    mpBackScreen->search('foregn_n')->show();
#endif
    for (int i = 0; i < 6; i++) {
        J2DTextBox* backScreen = (J2DTextBox*)mpBackScreen->search(tx[i]);
        backScreen->setFont(mpFont);
        backScreen->setString(0x80, "");
        if (i < 2) {
            mpString->getString(0x55D, backScreen, NULL, NULL, NULL, 0);
        } else if (i < 4) {
            mpString->getString(0x55E, backScreen, NULL, NULL, NULL, 0);
        } else {
            mpString->getString(0x556, backScreen, NULL, NULL, NULL, 0);
        }
    }
    mpBackScreen->search('wi_btn_n')->hide();
    for (int i = 0; i < 4; i++) {
        field_0x1c0[i] = 0;
    }
    for (int i = 0; i < 3; i++) {
        if (z_tx[i] != NULL) {
            mpZButtonText[i] = new CPaneMgr(mpBackScreen, z_tx[i], 2, NULL);
        } else {
            mpZButtonText[i] = NULL;
        }
    }
    for (int i = 0; i < 10; i++) {
        J2DTextBox* tvScreen = (J2DTextBox*)mpTVScreen->search(txTV[i]);
        tvScreen->setFont(mpFont);
#if VERSION != VERSION_GCN_JPN
        tvScreen->setCharSpace(0.0f);
#endif

        if (i < 2) {
            tvScreen->setString(0x100, "");
            mpString->getString(0x558, tvScreen, NULL, NULL, NULL, 0);
        } else if (i < 4) {
            tvScreen->setString(0x100, "");
            mpString->getString(0x557, tvScreen, NULL, NULL, NULL, 0);
        } else if (i < 6) {
            tvScreen->setString(0x100, "");
            mpString->getString(0x559, tvScreen, NULL, NULL, NULL, 0);
        } else if (i < 8) {
            tvScreen->setString(0x100, "");
            mpString->getString(0x55A, tvScreen, NULL, NULL, NULL, 0);
        } else {
            tvScreen->setString(0x100, "");
            mpString->getString(0x55B, tvScreen, NULL, NULL, NULL, 0);
        }
    }
    for (int i = 0; i < 5; i++) {
        mpParent[i]->setAlphaRate(0.0f);
    }
}

void dMenu_Option_c::setSoundMode(u32 param_0) {
    switch (param_0) {
    case 0:
        OSSetSoundMode(OS_SOUND_MODE_MONO);
        break;
    case 1:
    case 2:
        OSSetSoundMode(OS_SOUND_MODE_STEREO);
        break;
    }
}

void dMenu_Option_c::setAttenString() {
    u16 stringId1;
    u16 stringId2;

    if (field_0x3e4 == 0) {
        stringId1 = 0x549;
        stringId2 = 0x54A;
    } else {
        stringId1 = 0x54A;
        stringId2 = 0x549;
    }
    for (int i = 0; i < 6; i++) {
        if (i < 2) {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[0][i]->getPanePtr();
            mpString->getString(stringId1, textBox, NULL, NULL, NULL, 0);
        } else {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[0][i]->getPanePtr();
            mpString->getString(stringId2, textBox, NULL, NULL, NULL, 0);
        }
    }
}

#if VERSION == VERSION_GCN_JPN
void dMenu_Option_c::setRubyString() {
    u16 stringId1;
    u16 stringId2;

    if (field_0x3e5_JPN == 0) {
        stringId1 = 0x54c;
        stringId2 = 0x54d;
    } else {
        stringId1 = 0x54d;
        stringId2 = 0x54c;
    }
    for (int i = 0; i < 6; i++) {
        if (i < 2) {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[1][i]->getPanePtr();
            mpString->getString(stringId1, textBox, NULL, NULL, NULL, 0);
        } else {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[1][i]->getPanePtr();
            mpString->getString(stringId2, textBox, NULL, NULL, NULL, 0);
        }
    }
}
#endif

void dMenu_Option_c::setVibString() {
    u16 stringId1;
    u16 stringId2;

    if (field_0x3ea == 0) {
        stringId1 = 0x54C;
        stringId2 = 0x54D;
    } else {
        stringId1 = 0x54D;
        stringId2 = 0x54C;
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        const int IDX = 2;
#else
        const int IDX = 1;
#endif
        if (i < 2) {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[IDX][i]->getPanePtr();
            mpString->getString(stringId1, textBox, NULL, NULL, NULL, 0);
        } else {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[IDX][i]->getPanePtr();
            mpString->getString(stringId2, textBox, NULL, NULL, NULL, 0);
        }
    }
}

void dMenu_Option_c::setSoundString() {
    u16 stringId1;
    u16 stringId2;
    u16 stringId3;

    if (field_0x3e9 == 0) {
        stringId1 = 0x551;
        stringId2 = 0x550;
        stringId3 = 0x552;
    } else if (field_0x3e9 == 1) {
        stringId1 = 0x550;
        stringId2 = 0x552;
        stringId3 = 0x551;
    } else {
        stringId1 = 0x552;
        stringId2 = 0x551;
        stringId3 = 0x550;
    }
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        const int IDX = 3;
#else
        const int IDX = 2;
#endif
        if (i < 2) {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[IDX][i]->getPanePtr();
            mpString->getString(stringId1, textBox, NULL, NULL, NULL, 0);
        } else if (i < 4) {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[IDX][i]->getPanePtr();
            mpString->getString(stringId2, textBox, NULL, NULL, NULL, 0);
        } else {
            J2DTextBox* textBox = (J2DTextBox*)mpMenuText[IDX][i]->getPanePtr();
            mpString->getString(stringId3, textBox, NULL, NULL, NULL, 0);
        }
    }
}

void dMenu_Option_c::setCursorPos(u8 i_index) {
#if VERSION != VERSION_GCN_JPN
    if (i_index == 4) {
        i_index = 5;
    }
#endif

    Vec pos = mpMenuPane[i_index]->getGlobalVtxCenter(mpMenuPane[i_index]->mPane, false, 0);
    mpDrawCursor->setPos(pos.x + (field_0x330 - field_0x334), pos.y,
                         mpMenuPane[i_index]->getPanePtr(), false);
    setSelectColor(i_index, false);
    changeBarColor(false);
}

void dMenu_Option_c::setSelectColor(u8 param_0, bool param_1) {
    for (int i = 0; i < 6; i++) {
        if (i == param_0 || param_1) {
            if (field_0x21c[i][0] != NULL) {
                field_0x21c[i][0]->setFontColor(field_0x3b8, field_0x3b8);
            }
            if (field_0x21c[i][1] != NULL) {
                field_0x21c[i][1]->setFontColor(field_0x3b8, field_0x3b8);
            }
            mpMenuNull[i]->scale(mBarScale[0], mBarScale[0]);
            if (mpMenuPaneC[i] != NULL) {
                Vec pos = mpMenuPaneC[i]->getGlobalVtxCenter(mpMenuPaneC[i]->mPane, false, 0);
                if (mpMenuPane2[i] != NULL) {
                    mpMenuPane2[i]->scale(mBarScale[0], mBarScale[0]);
                }
                if (mpMenuPane3[i] != NULL) {
                    mpMenuPane3[i]->scale(mBarScale[0], mBarScale[0]);
                }
            }
        } else {
            if (field_0x21c[i][0] != NULL) {
                field_0x21c[i][0]->setFontColor(field_0x3bc, field_0x3bc);
            }
            if (field_0x21c[i][1] != NULL) {
                field_0x21c[i][1]->setFontColor(field_0x3bc, field_0x3bc);
            }
            mpMenuNull[i]->scale(mBarScale[1], mBarScale[1]);
            if (mpMenuPaneC[i] != NULL) {
                Vec pos = mpMenuPaneC[i]->getGlobalVtxCenter(mpMenuPaneC[i]->mPane, false, 0);
                if (mpMenuPane2[i] != NULL) {
                    mpMenuPane2[i]->scale(mBarScale[1], mBarScale[1]);
                }
                if (mpMenuPane3[i] != NULL) {
                    mpMenuPane3[i]->scale(mBarScale[1], mBarScale[1]);
                }
            }
        }
    }
}

u8 dMenu_Option_c::getSelectType() {
    if (field_0x3ef < SelectType3) {
        return field_0x3ef;
    }
    if (field_0x3f5 < SelectType3) {
        return field_0x3f5;
    }
    return 0;
}

void dMenu_Option_c::changeBarColor(bool i_changeColor) {
    u8 selectType = getSelectType();

    for (int i = 0; i < 6; i++) {
        if (i == selectType || i_changeColor) {
            field_0x280[i]->setBlackWhite(field_0x2b0[0], field_0x2c0[0]);
            field_0x298[i]->setBlackWhite(field_0x2b0[1], field_0x2c0[1]);
            for (int j = 0; j < 4; j++) {
                field_0x2d0[i][j]->setAlpha(field_0x404[j]);
            }
        } else {
            field_0x280[i]->setBlackWhite(field_0x2b8[0], field_0x2c8[0]);
            field_0x298[i]->setBlackWhite(field_0x2b8[1], field_0x2c8[1]);
            for (int j = 0; j < 4; j++) {
                field_0x2d0[i][j]->setAlpha(field_0x408[j]);
            }
        }
    }
}

void dMenu_Option_c::setHIO(bool i_useHIO) {
    if (field_0x378 != g_drawHIO.mOptionScreen.mWindowPosX ||
        field_0x37c != g_drawHIO.mOptionScreen.mWindowPosY)
    {
        field_0x378 = g_drawHIO.mOptionScreen.mWindowPosX;
        field_0x37c = g_drawHIO.mOptionScreen.mWindowPosY;
        for (int i = 0; i < 5; i++) {
            mpParent[i]->paneTrans(field_0x378, field_0x37c);
        }
        setCursorPos(getSelectType());
    }
    if (field_0x380 != g_drawHIO.mOptionScreen.mWindowScale) {
        field_0x380 = g_drawHIO.mOptionScreen.mWindowScale;
        for (int i = 0; i < 5; i++) {
            f32 windowScale = field_0x380;
            mpParent[i]->scale(windowScale, windowScale);
        }
        setCursorPos(getSelectType());
    }
    if (field_0x3b8.r != g_drawHIO.mOptionScreen.mSelectColor.r ||
        field_0x3b8.g != g_drawHIO.mOptionScreen.mSelectColor.g ||
        field_0x3b8.b != g_drawHIO.mOptionScreen.mSelectColor.b ||
        field_0x3b8.a != g_drawHIO.mOptionScreen.mSelectColor.a ||
        field_0x3bc.r != g_drawHIO.mOptionScreen.mUnselectColor.r ||
        field_0x3bc.g != g_drawHIO.mOptionScreen.mUnselectColor.g ||
        field_0x3bc.b != g_drawHIO.mOptionScreen.mUnselectColor.b ||
        field_0x3bc.a != g_drawHIO.mOptionScreen.mUnselectColor.a)
    {
        field_0x3b8.set(g_drawHIO.mOptionScreen.mSelectColor);
        field_0x3bc.set(g_drawHIO.mOptionScreen.mUnselectColor);
        setSelectColor(getSelectType(), false);
    }
    for (int i = 0; i < 6; i++) {
        if (field_0x384[i] != g_drawHIO.mOptionScreen.mOptionTypeBGPosX[i] ||
            field_0x39c[i] != g_drawHIO.mOptionScreen.mOptionTypeBGPosY[i])
        {
            field_0x384[i] = g_drawHIO.mOptionScreen.mOptionTypeBGPosX[i];
            field_0x39c[i] = g_drawHIO.mOptionScreen.mOptionTypeBGPosY[i];
            mpMenuPane[i]->paneTrans(field_0x384[i], field_0x39c[i]);
        }
    }
    for (int i = 0; i < 2; i++) {
        if (mBarScale[i] != g_drawHIO.mOptionScreen.mBarScale[i]) {
            mBarScale[i] = g_drawHIO.mOptionScreen.mBarScale[i];
        }
    }
    if (g_drawHIO.mOptionScreen.mDebug) {
        Vec pos = mpTitle->getGlobalVtxCenter(mpTitle->mPane, false, 0);
        mpWarning->mPosY = pos.y + g_drawHIO.mOptionScreen.mBackgroundPosY;
    }
    if (g_drawHIO.mCollectScreen.mButtonDebugON != false || i_useHIO) {
        if (mpButtonAB[0] != NULL) {
            mpButtonAB[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonAPosX,
                                     g_drawHIO.mCollectScreen.mButtonAPosY);
            f32 AButtonScale = g_drawHIO.mCollectScreen.mButtonAScale;
            mpButtonAB[0]->scale(AButtonScale, AButtonScale);
        }
        if (mpButtonAB[1] != NULL) {
            mpButtonAB[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBPosX,
                                     g_drawHIO.mCollectScreen.mButtonBPosY);
            f32 BButtonScale = g_drawHIO.mCollectScreen.mButtonBScale;
            mpButtonAB[1]->scale(BButtonScale, BButtonScale);
        }
        if (mpButtonText[0] != NULL) {
            mpButtonText[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonATextPosX,
                                       g_drawHIO.mCollectScreen.mButtonATextPosY);
            f32 AButtonTextScale = g_drawHIO.mCollectScreen.mButtonATextScale;
            mpButtonText[0]->scale(AButtonTextScale, AButtonTextScale);
        }
        if (mpButtonText[1] != NULL) {
            mpButtonText[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBTextPosX,
                                       g_drawHIO.mCollectScreen.mButtonBTextPosY);
            f32 BButtonTextScale = g_drawHIO.mCollectScreen.mButtonBTextScale;
            mpButtonText[1]->scale(BButtonTextScale, BButtonTextScale);
        }
    }
}

void dMenu_Option_c::cursorAnime(f32 i_cursorValue) {
    mpDrawCursor->setAlphaRate(i_cursorValue);
    mpDrawCursor->setScale(i_cursorValue);
}

void dMenu_Option_c::setZButtonString(u16 i_stringID) {
    if (i_stringID == 0) {
        for (int i = 0; i < 3; i++) {
            if (mpZButtonText[i] != NULL) {
                mpZButtonText[i]->hide();
            }
        }
    } else {
        for (int i = 0; i < 3; i++) {
            if (mpZButtonText[i] != NULL) {
                mpZButtonText[i]->show();
            }
        }
    }
}

void dMenu_Option_c::changeTVCheck() {
    if (mDoCPd_c::getTrigZ(PAD_1) != 0) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CHANGE_WINDOW, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                 0);
        field_0x3f3 = 0;
        setAButtonString(0);
        setBButtonString(0);
        setZButtonString(0);
        dMw_c::dMw_fade_out();
        dMeter2Info_set2DVibration();
    }
}

static void dummy() {
#if VERSION == VERSION_GCN_JPN
    static const u64 txTVhide[5] = {'fmenu_6n', 'fmenu_9n', 'fmenu_10', 'fmenu_7n', 'fmenu_8n'};
#else
    static const u64 txTVhide[5] = {'menu_6n', 'menu_9n', 'menu_10n', 'menu_7n', 'menu_8n'};
#endif
}

void dMenu_Option_c::setAButtonString(u16 i_stringID) {
    static const u64 text_a_tag[5] = {'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5'};
    u32 stringId = i_stringID;
    if (stringId != field_0x3dc) {
        field_0x3dc = i_stringID;
        if (stringId == 0) {
            for (int i = 0; i < 5; i++) {
                J2DTextBox* textBox = (J2DTextBox*)mpScreenIcon->search(text_a_tag[i]);
                strcpy(textBox->getStringPtr(), "");
            }
        } else {
            for (int i = 0; i < 5; i++) {
                J2DTextBox* textBox = (J2DTextBox*)mpScreenIcon->search(text_a_tag[i]);
                dMeter2Info_getStringKanji(stringId, textBox->getStringPtr(), NULL);
            }
        }
    }
}

void dMenu_Option_c::setBButtonString(u16 i_stringID) {
    static const u64 text_b_tag[5] = {'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5'};
    u32 stringId = i_stringID;
    if (stringId != field_0x3de) {
        field_0x3de = i_stringID;

        if (stringId == 0) {
            for (int i = 0; i < 5; i++) {
                J2DTextBox* textBox = (J2DTextBox*)mpScreenIcon->search(text_b_tag[i]);
                strcpy(textBox->getStringPtr(), "");
            }
        } else {
            for (int i = 0; i < 5; i++) {
                J2DTextBox* textBox = (J2DTextBox*)mpScreenIcon->search(text_b_tag[i]);
                dMeter2Info_getStringKanji(stringId, textBox->getStringPtr(), NULL);
            }
        }
    }
}

bool dMenu_Option_c::isRumbleSupported() {
    return JUTGamePad::sRumbleSupported >> 0x1f;
}

bool dMenu_Option_c::dpdMenuMove() {
    return false;
}

void dMenu_Option_c::paneResize(u64 i_tag) {
    f32 boundsY = mpClipScreen->search(i_tag)->getBounds().i.y;
    f32 boundsX = mpClipScreen->search(i_tag)->getBounds().i.x - 5.0f;
    mpClipScreen->search(i_tag)->move(boundsX, boundsY);

    // Fake match
    JGeometry::TBox2<f32>& bounds = mpClipScreen->search(i_tag)->mBounds;
    boundsY = bounds.f.y - bounds.i.y;
    boundsX = mpClipScreen->search(i_tag)->getWidth();

    mpClipScreen->search(i_tag)->resize(boundsX + 10.0f, boundsY);
}

void dMenu_Option_c::initialize() {
    field_0x3e4 = dComIfGs_getOptAttentionType();
#if VERSION == VERSION_GCN_JPN
    if (dComIfGs_getOptUnk0() == 0) {
        field_0x3e5_JPN = 1;
    } else {
        field_0x3e5_JPN = 0;
    }
#endif
    field_0x3e5 = dComIfGs_getOptCameraControl();
    field_0x3e8 = 0;
    field_0x3eb = 0;
    field_0x3e6 = 0;
    field_0x3e7 = 0;
    field_0x3e9 = dComIfGs_getOptSound();
    if (isRumbleSupported()) {
        field_0x3ea = dComIfGp_getNowVibration();
    } else {
        field_0x3ea = 0;
    }
    setAttenString();
#if VERSION == VERSION_GCN_JPN
    setRubyString();
#endif
    setVibString();
    setSoundString();
    mFrame = 0;
    field_0x3da = 0;
}

void dMenu_Option_c::yesnoMenuMoveAnmInitSet(int param_0, int param_1) {
    if (field_0x3f8 == 0) {
        field_0x3f9 = 0;
        field_0x3fa = 1;
    }
    if (param_0 == 0x473) {
        ((J2DTextBox*)mpYesNoTxt_c[field_0x3f9]->getPanePtr())
            ->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
        ((J2DTextBox*)mpYesNoTxt_c[field_0x3fa]->getPanePtr())
            ->setWhite(JUtility::TColor(0x96, 0x96, 0x96, 0xff));
    }
    mpDrawCursor->setAlphaRate(0.0f);
    mpYesNoSelBase_c[0]->getPanePtr()->setAnimation(field_0x20);
    mpYesNoSelBase_c[1]->getPanePtr()->setAnimation(field_0x20);
    field_0x3c8[2] = param_0;
    field_0x3c8[3] = param_1;
    field_0x20->setFrame(field_0x3c8[2]);
    mpYesNoSelBase_c[0]->getPanePtr()->animationTransform();
    mpYesNoSelBase_c[1]->getPanePtr()->animationTransform();
}

bool dMenu_Option_c::yesnoMenuMoveAnm() {
    bool ret;
    if (field_0x3c8[2] != field_0x3c8[3]) {
        if (field_0x3c8[2] < field_0x3c8[3]) {
            field_0x3c8[2] += 2;
            if (field_0x3c8[2] > field_0x3c8[3]) {
                field_0x3c8[2] = field_0x3c8[3];
            }
        } else {
            field_0x3c8[2] -= 2;
            if (field_0x3c8[2] < field_0x3c8[3]) {
                field_0x3c8[2] = field_0x3c8[3];
            }
        }
        field_0x20->setFrame(field_0x3c8[2]);
        mpYesNoSelBase_c[0]->getPanePtr()->animationTransform();
        mpYesNoSelBase_c[1]->getPanePtr()->animationTransform();
        ret = 0;
    }
    if (field_0x3c8[2] == field_0x3c8[3]) {
        mpYesNoSelBase_c[0]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        mpYesNoSelBase_c[1]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        if (field_0x3c8[2] == 0x47D) {
            field_0x3f8 = 1;
        } else {
            field_0x3f8 = 0;
            if (field_0x3f9 != 0xff) {
                mpYesNoCurWaku_c[field_0x3f9]->setAlpha(0);
                mpYesNoCurWakuG0_c[field_0x3f9]->setAlpha(0);
                mpYesNoCurWakuG1_c[field_0x3f9]->setAlpha(0);
            }
        }
        ret = 1;
    }
    return ret;
}

static s32 OptYnSelStartFrameTbl[2] = {1251, 1236};

static s32 OptYnSelEndFrameTbl[2] = {1236, 1251};

u8 dMenu_Option_c::yesnoSelectMoveAnm() {
    u8 ret = 0;
    bool bVar1;
    bool bVar2;

    if (field_0x3fa != 0xff) {
        if (field_0x3c8[field_0x3fa] != OptYnSelStartFrameTbl[field_0x3fa]) {
            if (field_0x3c8[field_0x3fa] < OptYnSelStartFrameTbl[field_0x3fa]) {
                field_0x3c8[field_0x3fa] += 2;
                if (field_0x3c8[field_0x3fa] > OptYnSelStartFrameTbl[field_0x3fa]) {
                    field_0x3c8[field_0x3fa] = OptYnSelStartFrameTbl[field_0x3fa];
                }
            } else {
                field_0x3c8[field_0x3fa] -= 2;
                if (field_0x3c8[field_0x3fa] < OptYnSelStartFrameTbl[field_0x3fa]) {
                    field_0x3c8[field_0x3fa] = OptYnSelStartFrameTbl[field_0x3fa];
                }
            }
            field_0x24->setFrame(field_0x3c8[field_0x3fa]);
            mpYesNoSelBase_c[field_0x3fa]->getPanePtr()->animationTransform();
        }
        if (field_0x3c8[field_0x3fa] == OptYnSelStartFrameTbl[field_0x3fa]) {
            bVar1 = true;
        } else {
            bVar1 = false;
        }
    } else {
        bVar1 = true;
    }
    if (field_0x3f9 != 0xff) {
        if (field_0x3c8[field_0x3f9] != OptYnSelEndFrameTbl[field_0x3f9]) {
            if (field_0x3c8[field_0x3f9] < OptYnSelEndFrameTbl[field_0x3f9]) {
                field_0x3c8[field_0x3f9] += 2;
                if (field_0x3c8[field_0x3f9] > OptYnSelEndFrameTbl[field_0x3f9]) {
                    field_0x3c8[field_0x3f9] = OptYnSelEndFrameTbl[field_0x3f9];
                }
            } else {
                field_0x3c8[field_0x3f9] -= 2;
                if (field_0x3c8[field_0x3f9] < OptYnSelEndFrameTbl[field_0x3f9]) {
                    field_0x3c8[field_0x3f9] = OptYnSelEndFrameTbl[field_0x3f9];
                }
            }
            field_0x28->setFrame(field_0x3c8[field_0x3f9]);
            mpYesNoSelBase_c[field_0x3f9]->getPanePtr()->animationTransform();
        }
        if (field_0x3c8[field_0x3f9] == OptYnSelEndFrameTbl[field_0x3f9]) {
            bVar2 = true;
        } else {
            bVar2 = false;
        }
    } else {
        bVar2 = true;
    }
    if (bVar1 == 1 && bVar2 == 1) {
        if (field_0x3fa != 0xff) {
            mpYesNoSelBase_c[field_0x3fa]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        if (field_0x3f9 != 0xff) {
            mpYesNoSelBase_c[field_0x3f9]->getPanePtr()->setAnimation((J2DAnmTransform*)NULL);
        }
        ret = 1;
    }
    return ret;
}

void dMenu_Option_c::yesnoCursorShow() {
    if (field_0x3f9 != 0xff) {
        ((J2DTextBox*)mpYesNoTxt_c[field_0x3f9]->getPanePtr())
            ->setWhite(JUtility::TColor(0xff, 0xff, 0xff, 0xff));
        mpYesNoCurWaku_c[field_0x3f9]->setAlpha(0xff);
        mpYesNoCurWakuG0_c[field_0x3f9]->setAlpha(0xff);
        mpYesNoCurWakuG1_c[field_0x3f9]->setAlpha(0xff);
        Vec pos = mpYesNoSelBase_c[field_0x3f9]->getGlobalVtxCenter(
            mpYesNoSelBase_c[field_0x3f9]->mPane, false, 0);
        mpDrawCursor->setPos(pos.x, pos.y, mpYesNoSelBase_c[field_0x3f9]->getPanePtr(), true);
        mpDrawCursor->setAlphaRate(1.0f);
        mpDrawCursor->setParam(0.96f, 0.84f, 0.06f, 0.5f, 0.5f);
    }
}

void dMenu_Option_c::yesNoSelectStart() {
    if (field_0x3f9 != 0) {
        if (field_0x3f7 == 1) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_NAME_OK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        } else {
            Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_BACK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        yesnoMenuMoveAnmInitSet(0x47D, 0x473);
        mpDrawCursor->setAlphaRate(0.0f);
    } else {
        yesnoCancelAnmSet();
    }
}

void dMenu_Option_c::yesnoSelectAnmSet() {
    if (field_0x3fa != 0xff) {
        yesnoWakuAlpahAnmInit(field_0x3fa, 0xff, 0, 5);
        mpYesNoSelBase_c[field_0x3fa]->getPanePtr()->setAnimation(field_0x24);
        field_0x3c8[field_0x3fa] = OptYnSelEndFrameTbl[field_0x3fa];
        field_0x24->setFrame(field_0x3c8[field_0x3fa]);
        mpYesNoSelBase_c[field_0x3fa]->getPanePtr()->animationTransform();
        mpDrawCursor->setAlphaRate(0.0f);
    }
    if (field_0x3f9 != 0xff) {
        mpYesNoSelBase_c[field_0x3f9]->getPanePtr()->setAnimation(field_0x28);
        field_0x3c8[field_0x3f9] = OptYnSelStartFrameTbl[field_0x3f9];
        field_0x28->setFrame(field_0x3c8[field_0x3f9]);
        mpYesNoSelBase_c[field_0x3f9]->getPanePtr()->animationTransform();
    }
}

void dMenu_Option_c::yesnoCancelAnmSet() {
    Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    mpDrawCursor->setAlphaRate(0.0f);
    yesnoMenuMoveAnmInitSet(0x47D, 0x473);
}

void dMenu_Option_c::yesnoWakuAlpahAnmInit(u8 i_idx, u8 param_1, u8 param_2, u8 param_3) {
    if (i_idx != 0xff) {
        mpYesNoCurWaku_c[i_idx]->alphaAnimeStart(0);
        mpYesNoCurWakuG0_c[i_idx]->alphaAnimeStart(0);
        mpYesNoCurWakuG1_c[i_idx]->alphaAnimeStart(0);
        field_0x3fb[i_idx] = param_1;
        field_0x3fd[i_idx] = param_2;
        field_0x3ff[i_idx] = param_3;
        mpYesNoTxt_c[i_idx]->colorAnimeStart(0);
    }
}

u8 dMenu_Option_c::yesnoWakuAlpahAnm(u8 i_idx) {
    u8 ret = 0;
    bool wakuAnime;
    bool wakuAnimeG0;
    bool wakuAnimeG1;
    bool colorAnime;

    if (i_idx != 0xff) {
        wakuAnime = mpYesNoCurWaku_c[i_idx]->alphaAnime(field_0x3ff[i_idx], field_0x3fb[i_idx],
                                                        field_0x3fd[i_idx], 0);
        wakuAnimeG0 = mpYesNoCurWakuG0_c[i_idx]->alphaAnime(field_0x3ff[i_idx], field_0x3fb[i_idx],
                                                            field_0x3fd[i_idx], 0);
        wakuAnimeG1 = mpYesNoCurWakuG1_c[i_idx]->alphaAnime(field_0x3ff[i_idx], field_0x3fb[i_idx],
                                                            field_0x3fd[i_idx], 0);
        colorAnime = mpYesNoTxt_c[i_idx]->colorAnime(
            field_0x3ff[i_idx], mpYesNoTxt_c[i_idx]->getInitBlack(),
            mpYesNoTxt_c[i_idx]->getInitBlack(), JUtility::TColor(0xff, 0xff, 0xff, 0xff),
            JUtility::TColor(0x96, 0x96, 0x96, 0xff), 0);
    } else {
        wakuAnime = true;
        wakuAnimeG0 = true;
        wakuAnimeG1 = true;
        colorAnime = true;
    }
    if (wakuAnime == 1 && wakuAnimeG0 == 1 && wakuAnimeG1 == 1 && colorAnime == 1) {
        ret = 1;
    }
    return ret;
}
