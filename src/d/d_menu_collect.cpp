/**
 * d_menu_collect.cpp
 * Collection Menu
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_collect.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/J3DGraphLoader/J3DAnmLoader.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"
#include "d/actor/d_a_alink.h"
#include "d/d_select_cursor.h"
#include "d/d_menu_fishing.h"
#include "d/d_menu_insect.h"
#include "d/d_menu_letter.h"
#include "d/d_menu_option.h"
#include "d/d_menu_save.h"
#include "d/d_menu_skill.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_class.h"
#include "d/d_msg_object.h"
#include "d/d_msg_string.h"
#include "d/d_pane_class.h"
#include "d/d_item.h"
#include "d/d_lib.h"
#include "d/d_meter2_info.h"
#include "dolphin/os.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_mtx.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "d/d_menu_window.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"

typedef void (dMenu_Collect2D_c::*initFunc)();
static initFunc init[] = {
    &dMenu_Collect2D_c::wait_init,          &dMenu_Collect2D_c::save_open_init,
    &dMenu_Collect2D_c::save_move_init,     &dMenu_Collect2D_c::save_close_init,
    &dMenu_Collect2D_c::option_open_init,   &dMenu_Collect2D_c::option_move_init,
    &dMenu_Collect2D_c::option_close_init,  &dMenu_Collect2D_c::letter_open_init,
    &dMenu_Collect2D_c::letter_move_init,   &dMenu_Collect2D_c::letter_close_init,
    &dMenu_Collect2D_c::fishing_open_init,  &dMenu_Collect2D_c::fishing_move_init,
    &dMenu_Collect2D_c::fishing_close_init, &dMenu_Collect2D_c::skill_open_init,
    &dMenu_Collect2D_c::skill_move_init,    &dMenu_Collect2D_c::skill_close_init,
    &dMenu_Collect2D_c::insect_open_init,   &dMenu_Collect2D_c::insect_move_init,
    &dMenu_Collect2D_c::insect_close_init};

typedef void (dMenu_Collect2D_c::*processFunc)();
static processFunc process[] = {
    &dMenu_Collect2D_c::wait_proc,          &dMenu_Collect2D_c::save_open_proc,
    &dMenu_Collect2D_c::save_move_proc,     &dMenu_Collect2D_c::save_close_proc,
    &dMenu_Collect2D_c::option_open_proc,   &dMenu_Collect2D_c::option_move_proc,
    &dMenu_Collect2D_c::option_close_proc,  &dMenu_Collect2D_c::letter_open_proc,
    &dMenu_Collect2D_c::letter_move_proc,   &dMenu_Collect2D_c::letter_close_proc,
    &dMenu_Collect2D_c::fishing_open_proc,  &dMenu_Collect2D_c::fishing_move_proc,
    &dMenu_Collect2D_c::fishing_close_proc, &dMenu_Collect2D_c::skill_open_proc,
    &dMenu_Collect2D_c::skill_move_proc,    &dMenu_Collect2D_c::skill_close_proc,
    &dMenu_Collect2D_c::insect_open_proc,   &dMenu_Collect2D_c::insect_move_proc,
    &dMenu_Collect2D_c::insect_close_proc,
};

dMenu_Collect2D_c::dMenu_Collect2D_c(JKRExpHeap* param_0, STControl* param_1, CSTControl* param_2) {
    mpHeap = param_0;
    mpStick = param_1;
    mpCStick = param_2;
    field_0xc = NULL;
    mIsWolf = daPy_py_c::checkNowWolf();
    mLinkGlobalCenterPos.set(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 2; i++) {
        field_0x44[i] = 1.0f;
        field_0x4c[i] = 1.0f;
    }
    mBlueSmokePosX = 0.0f;
    mBlueSmokePosY = 0.0f;
    mBlueSmokeScale = 1.0f;
    mBlueSmokeAlpha = 1.0f;
    mHeartVesselPosX = 0.0f;
    mHeartVesselPosY = 0.0f;
    mHeartVesselScale = 1.0f;
    mHeartPiecePosX = 0.0f;
    mHeartPiecePosY = 0.0f;
    mHeartPieceScale = 1.0f;
    mpSubHeap = JKRCreateExpHeap(0x00046000, mpHeap, 0);
}


dMenu_Collect2D_c::~dMenu_Collect2D_c() {
    if (mpSubHeap) {
        mDoExt_destroyExpHeap(mpSubHeap);
        mpSubHeap = NULL;
    }
}

void dMenu_Collect2D_c::_create() {
    mpHeap->getTotalFreeSize();
    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_collect_soubi_screen.blo", 0x1020000,
                          dComIfGp_getCollectResArchive());
    dPaneClass_showNullPane(mpScreen);
    mpScreenIcon = new J2DScreen();
    mpScreenIcon->setPriority("zelda_collect_soubi_do_icon_parts.blo", 0x20000,
                              dComIfGp_getCollectResArchive());
    for (int i = 0; i < 2; i++) {
        mpButtonAB[i] = NULL;
        mpButtonText[i] = NULL;
    }
    dPaneClass_showNullPane(mpScreenIcon);
    mpDraw2DTop = new dMenu_Collect2DTop_c(this);
    ResTIMG* image = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(image);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
    mpBlackTex->setAlpha(0);
    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpString = new dMsgString_c();
    mpSaveScrn = NULL;
    mpOptionScrn = NULL;
    mpLetterScrn = NULL;
    mpFishingScrn = NULL;
    mpSkillScrn = NULL;
    mpInsectScrn = NULL;
    mProcess = 0;
    animationSet();
    screenSet();
    initialize();
    setHIO(true);
}

void dMenu_Collect2D_c::_delete() {
    mpHeap->getTotalFreeSize();
    dMeter2Info_setCollectCursorPosXY(mCursorX, mCursorY);
    delete mpScreen;
    mpScreen = NULL;
    delete mpScreenIcon;
    mpScreenIcon = NULL;
    for (int i = 0; i < 2; i++) {
        if (mpButtonAB[i]) {
            delete mpButtonAB[i];
            mpButtonAB[i] = NULL;
        }
        if (mpButtonText[i]) {
            delete mpButtonText[i];
            mpButtonText[i] = NULL;
        }
    }
    delete mpDraw2DTop;
    mpDraw2DTop = NULL;
    delete mpBlackTex;
    mpBlackTex = NULL;
    delete mpDrawCursor;
    mpDrawCursor = NULL;
    delete mpString;
    mpString = NULL;
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSaveScrn) {
        mpSaveScrn->_delete();
        delete mpSaveScrn;
        mpSaveScrn = NULL;
        mpSubHeap->freeAll();
    }
    if (mpOptionScrn) {
        mpOptionScrn->_delete();
        delete mpOptionScrn;
        mpOptionScrn = NULL;
        mpSubHeap->freeAll();
    }
    if (mpLetterScrn) {
        delete mpLetterScrn;
        mpLetterScrn = NULL;
        mpSubHeap->freeAll();
    }
    if (mpFishingScrn) {
        delete mpFishingScrn;
        mpFishingScrn = NULL;
        mpSubHeap->freeAll();
    }
    if (mpSkillScrn) {
        delete mpSkillScrn;
        mpSkillScrn = NULL;
        mpSubHeap->freeAll();
    }
    if (mpInsectScrn) {
        delete mpInsectScrn;
        mpInsectScrn = NULL;
        mpSubHeap->freeAll();
    }
    mDoExt_setCurrentHeap(heap);
    delete mpAnmKey;
    mpAnmKey = NULL;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (mpSelPm[i][j]) {
                delete mpSelPm[i][j];
                mpSelPm[i][j] = NULL;
            }
        }
    }
    delete mpLinkPm;
    mpLinkPm = NULL;
    delete mpMaskPm;
    mpMaskPm = NULL;
    delete mpModelBg;
    mpModelBg = NULL;
    delete mpHeartParent;
    mpHeartParent = NULL;
    delete mpHeartPiece;
    mpHeartPiece = NULL;
    dComIfGp_getCollectResArchive()->removeResourceAll();
}

void dMenu_Collect2D_c::initialize() {
    (this->*init[mProcess])();
}

bool dMenu_Collect2D_c::isFishIconVisible() {
    if (dComIfGs_getFishNum(0) || dComIfGs_getFishNum(1) || dComIfGs_getFishNum(2) ||
        dComIfGs_getFishNum(3) || dComIfGs_getFishNum(4) || dComIfGs_getFishNum(5) ||
        g_drawHIO.mFishListScreen.mDebug)
    {
        return true;
    }
    return false;
}

bool dMenu_Collect2D_c::isSkillIconVisible() {
        /* dSv_event_flag_c::F_0338 - Secret techniques - Obtained 1 secret techinques - Shield attack */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[338]) ||
        /* dSv_event_flag_c::F_0339 - Secret techniques - Obtained 2 secret techinques */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[339]) ||
        /* dSv_event_flag_c::F_0340 - Secret techniques - Obtained 3 secret techinques */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[340]) ||
        /* dSv_event_flag_c::F_0341 - Secret techniques - Obtained 4 secret techinques */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[341]) ||
        /* dSv_event_flag_c::F_0342 - Secret techniques - Obtained 5 secret techinques */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[342]) ||
        /* dSv_event_flag_c::F_0343 - Secret techniques - Obtained 6 secret techinques */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[343]) ||
        /* dSv_event_flag_c::F_0344 - Secret techniques - Obtained 7 secret techinques */
        dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[344]))
    {
        return true;
    }
    return false;
}

bool dMenu_Collect2D_c::isInsectIconVisible() {
    return dMenu_Insect_c::getGetInsectNum();
}

static u8 const lit_3778[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void dMenu_Collect2D_c::screenSet() {
    static const u64 text_sv[3] = {'sav_0', 'sav_1', 'sav_2'};
    static const u64 text_op[3] = {'opt_0', 'opt_1', 'opt_2',};
    static const u64 ftext_sv[3] = {'f_sav_0', 'f_sav_1', 'f_sav_2'};
    static const u64 ftext_op[3] = {'f_opt_0', 'f_opt_1', 'f_opt_2'};
    static const u64 text_a_tag[5] = {'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5'};
    static const u64 text_b_tag[5] = {'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5'};

#if REGION_JPN
    static_cast<J2DTextBox*>(mpScreen->search('t_t00'))->setFont(mDoExt_getRubyFont());
    static_cast<J2DTextBox*>(mpScreen->search('t_t00'))->setString(0x20, "");
    dMeter2Info_getStringKanji(
        0x3E1, static_cast<J2DTextBox*>(mpScreen->search('t_t00'))->getStringPtr(), NULL);
    mpScreen->search('f_t00')->hide();
#else
    static_cast<J2DTextBox*>(mpScreen->search('f_t00'))->setFont(mDoExt_getRubyFont());
    static_cast<J2DTextBox*>(mpScreen->search('f_t00'))->setString(0x20, "");
    dMeter2Info_getStringKanji(
        0x3E1, static_cast<J2DTextBox*>(mpScreen->search('f_t00'))->getStringPtr(), NULL);
    mpScreen->search('t_t00')->hide();
#endif

    for (int i = 0; i < 3; i++) {
#if REGION_JPN
        static_cast<J2DTextBox*>(mpScreen->search(text_sv[i]))->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpScreen->search(text_op[i]))->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpScreen->search(text_sv[i]))->setString(0x20, "");
        static_cast<J2DTextBox*>(mpScreen->search(text_op[i]))->setString(0x20, "");
        dMeter2Info_getStringKanji(
            0x60, static_cast<J2DTextBox*>(mpScreen->search(text_sv[i]))->getStringPtr(), NULL);
        dMeter2Info_getStringKanji(
            0x5F, static_cast<J2DTextBox*>(mpScreen->search(text_op[i]))->getStringPtr(), NULL);
        mpScreen->search(ftext_sv[i])->hide();
        mpScreen->search(ftext_op[i])->hide();
#else
        static_cast<J2DTextBox*>(mpScreen->search(ftext_sv[i]))->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpScreen->search(ftext_op[i]))->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpScreen->search(ftext_sv[i]))->setString(0x20, "");
        static_cast<J2DTextBox*>(mpScreen->search(ftext_op[i]))->setString(0x20, "");
        dMeter2Info_getStringKanji(
            0x60, static_cast<J2DTextBox*>(mpScreen->search(ftext_sv[i]))->getStringPtr(), NULL);
        dMeter2Info_getStringKanji(
            0x5F, static_cast<J2DTextBox*>(mpScreen->search(ftext_op[i]))->getStringPtr(), NULL);
        mpScreen->search(text_sv[i])->hide();
        mpScreen->search(text_op[i])->hide();
#endif
    }

    for (int i = 0; i < 5; i++) {
        static_cast<J2DTextBox*>(mpScreenIcon->search(text_a_tag[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpScreenIcon->search(text_b_tag[i]))
            ->setFont(mDoExt_getMesgFont());
        static_cast<J2DTextBox*>(mpScreenIcon->search(text_a_tag[i]))->setString(0x20, "");
        static_cast<J2DTextBox*>(mpScreenIcon->search(text_b_tag[i]))->setString(0x20, "");
    }

#if REGION_JPN
    static_cast<J2DTextBox*>(mpScreen->search('item_n00'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n01'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n02'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n03'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n00'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('item_n01'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('item_n02'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('item_n03'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('i_text1'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('i_text0'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('i_text1'))->setString(0x100, "");
    static_cast<J2DTextBox*>(mpScreen->search('i_text0'))->setString(0x100, "");

    mpScreen->search('item_n04')->hide();
    mpScreen->search('item_n05')->hide();
    mpScreen->search('item_n06')->hide();
    mpScreen->search('item_n07')->hide();
    mpScreen->search('f_text1')->hide();
    mpScreen->search('f_text0')->hide();
#else
    static_cast<J2DTextBox*>(mpScreen->search('item_n04'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n05'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n06'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n07'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('item_n04'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('item_n05'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('item_n06'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('item_n07'))->setString(0x20, "");
    static_cast<J2DTextBox*>(mpScreen->search('f_text1'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('f_text0'))->setFont(mDoExt_getMesgFont());
    static_cast<J2DTextBox*>(mpScreen->search('f_text1'))->setString(0x100, "");
    static_cast<J2DTextBox*>(mpScreen->search('f_text0'))->setString(0x100, "");

    mpScreen->search('item_n00')->hide();
    mpScreen->search('item_n01')->hide();
    mpScreen->search('item_n02')->hide();
    mpScreen->search('item_n03')->hide();
    mpScreen->search('i_text1')->hide();
    mpScreen->search('i_text0')->hide();
#endif

    field_0x22d[0][0] = 0;
    field_0x22d[1][0] = 0;
    field_0x22d[2][0] = 0;
    field_0x22d[3][0] = dComIfGs_isItemFirstBit(0x28) ||
                        (dComIfGs_isItemFirstBit(0x3F)
                            /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
                        && !dComIfGs_isEventBit(0x302)) ?
                            true :
                            false;

    if (field_0x22d[3][0] != 0) {
        if (dComIfGs_isItemFirstBit(0x28)) {
            mpScreen->search('ken_00')->hide();
            mpScreen->search('ken_01')->show();
        } else if (dComIfGs_isItemFirstBit(0x3F)
                       /* dSv_event_flag_c::F_0026 - Ordon Village - gave wooden sword to talo on 3rd day */
                   && !dComIfGs_isEventBit(0x302)) {
            mpScreen->search('ken_00')->show();
            mpScreen->search('ken_01')->hide();
        }
    }

    field_0x22d[4][0] =
        dComIfGs_isItemFirstBit(0x29) || dComIfGs_isItemFirstBit(0x49) ? true : false;
    if (dComIfGs_getMaxLife() > 15) {
        field_0x22d[5][0] = 1;
    } else {
        field_0x22d[5][0] = 0;
    }
    field_0x22d[6][0] = dMenu_Collect3D_c::getMaskMdlVisible();
    field_0x22d[0][1] = 0;
    field_0x22d[1][1] = 0;
    field_0x22d[2][1] = 0;

    field_0x22d[3][1] =
        dComIfGs_isItemFirstBit(0x2B) || dComIfGs_isItemFirstBit(0x2A) ? true : false;
    if (field_0x22d[3][1] != 0) {
        if (dComIfGs_isItemFirstBit(0x2B)) {
            mpScreen->search('tate_00')->show();
            mpScreen->search('tate_01')->hide();
        } else if (dComIfGs_isItemFirstBit(0x2A)) {
            mpScreen->search('tate_00')->hide();
            mpScreen->search('tate_01')->show();
        }
    }
    field_0x22d[4][1] = dComIfGs_isItemFirstBit(0x2C);
    field_0x22d[5][1] = 0;
    field_0x22d[6][1] = 0;
    field_0x22d[0][2] = 0;
    field_0x22d[1][2] = 0;
    field_0x22d[2][2] = 0;
    if (dComIfGs_getSelectEquipClothes() == fpcNm_ITEM_WEAR_CASUAL) {
        field_0x22d[3][2] = 0;
        field_0x22d[4][2] = 0;
        field_0x22d[5][2] = 0;
    } else {
        field_0x22d[3][2] = dComIfGs_isItemFirstBit(0x2F);
        field_0x22d[4][2] = dComIfGs_isItemFirstBit(0x31);
        field_0x22d[5][2] = dComIfGs_isItemFirstBit(0x30);
    }
    field_0x22d[6][2] = 0;
    field_0x22d[0][3] = 1;
    if (checkItemGet(fpcNm_ITEM_BOW, 1)) {
        field_0x22d[1][3] = 1;
    } else {
        field_0x22d[1][3] = 0;
    }
    if (isInsectIconVisible()) {
        field_0x22d[2][3] = 1;
    } else {
        field_0x22d[2][3] = 0;
    }
    field_0x22d[3][3] = isSkillIconVisible();
    field_0x22d[4][3] = 0;
    field_0x22d[5][3] = 0;
    field_0x22d[6][3] = 0;
    if (dComIfGs_getCollectSmell() != fpcNm_ITEM_NONE) {
        field_0x22d[0][4] = 1;
    } else {
        field_0x22d[0][4] = 0;
    }
    field_0x22d[1][4] = dComIfGs_getPohSpiritNum();
    if (isFishIconVisible()) {
        field_0x22d[2][4] = 1;
    } else {
        field_0x22d[2][4] = 0;
    }
    if (g_drawHIO.mLetterSelectScreen.mLetterNum > 0) {
        field_0x22d[3][4] = 1;
    } else {
        if (dMeter2Info_getRecieveLetterNum()) {
            field_0x22d[3][4] = 1;
        } else {
            field_0x22d[3][4] = 0;
        }
    }
    field_0x22d[4][4] = 0;
    field_0x22d[5][4] = 0;
    field_0x22d[6][4] = 0;
    field_0x22d[0][5] = 1;
    field_0x22d[1][5] = 1;
    field_0x22d[2][5] = 0;
    field_0x22d[3][5] = 0;
    field_0x22d[4][5] = 0;
    field_0x22d[5][5] = 0;
    field_0x22d[6][5] = 0;
    field_0x184[0][0] = 0;
    field_0x184[1][0] = 0;
    field_0x184[2][0] = 0;
    if (dComIfGs_isItemFirstBit(0x28)) {
        field_0x184[3][0] = 0x18d;
    } else {
        field_0x184[3][0] = 0x1a4;
    }
    if (dComIfGs_isItemFirstBit(0x49)) {
        field_0x184[4][0] = 0x1ae;
    } else {
        field_0x184[4][0] = 0x18e;
    }
    field_0x184[5][0] = 0x186;
    if (field_0x22d[6][0] == 2) {
        if (dMenu_Collect3D_c::getMirrorNum() >= 4) {
            field_0x184[6][0] = 0x20c;
        } else {
            if (dMenu_Collect3D_c::getMirrorNum() >= 2) {
                field_0x184[6][0] = 0x20b;
            } else {
                field_0x184[6][0] = 0x20a;
            }
        }
    } else {
        if (dMenu_Collect3D_c::getCrystalNum() <= 1) {
            field_0x184[6][0] = 0x5aa;
        } else {
            field_0x184[6][0] = 0x5ab;
        }
    }
    field_0x184[0][1] = 0;
    field_0x184[1][1] = 0;
    field_0x184[2][1] = 0;
    if (dComIfGs_isItemFirstBit(0x2B)) {
        field_0x184[3][1] = 0x190;
    } else {
        field_0x184[3][1] = 0x18f;
    }
    field_0x184[4][1] = 0x191;
    field_0x184[5][1] = 0x192;
    field_0x184[6][1] = 0;
    field_0x184[0][2] = 0;
    field_0x184[1][2] = 0;
    field_0x184[2][2] = 0;
    field_0x184[3][2] = 0x194;
    field_0x184[4][2] = 0x196;
    field_0x184[5][2] = 0x195;
    field_0x184[6][2] = 0;
    if (dComIfGs_getRupeeMax() == WALLET_MAX) {
        field_0x184[0][3] = 0x199;
    } else if (dComIfGs_getRupeeMax() == BIG_WALLET_MAX) {
        field_0x184[0][3] = 0x19a;
    } else {
        field_0x184[0][3] = 0x19b;
    }
    if (dComIfGs_getArrowMax() == QUIVER_MAX) {
        field_0x184[1][3] = 0x1b9;
    } else if (dComIfGs_getArrowMax() == BIG_QUIVER_MAX) {
        field_0x184[1][3] = 0x1ba;
    } else {
        field_0x184[1][3] = 0x1bb;
    }
    field_0x184[2][3] = 0x5b8;
    field_0x184[3][3] = 0x5b0;
    field_0x184[4][3] = 0;
    field_0x184[5][3] = 0;
    field_0x184[6][3] = 0;
    field_0x184[0][4] = dMsgObject_getSmellTypeMessageID();
    field_0x184[1][4] = 0x245;
    field_0x184[2][4] = 0x5a1;
    field_0x184[3][4] = 0x4c8;
    field_0x184[4][4] = 0;
    field_0x184[5][4] = 0;
    field_0x184[6][4] = 0;
    field_0x184[0][5] = 0x60;
    field_0x184[1][5] = 0x5f;
    field_0x184[2][5] = 0;
    field_0x184[3][5] = 0;
    field_0x184[4][5] = 0;
    field_0x184[5][5] = 0;
    field_0x184[6][5] = 0;
    field_0x1d8[0][0] = 0;
    field_0x1d8[1][0] = 0;
    field_0x1d8[2][0] = 0;
    field_0x1d8[3][0] = field_0x184[3][0] + 0x100;
    field_0x1d8[4][0] = field_0x184[4][0] + 0x100;
    field_0x1d8[5][0] = field_0x184[5][0] + 0x100;
    if (field_0x22d[6][0] == 2) {
        if (dMenu_Collect3D_c::getMirrorNum() >= 4) {
            field_0x1d8[6][0] = 0x30c;
        } else {
            if (dMenu_Collect3D_c::getMirrorNum() >= 2) {
                field_0x1d8[6][0] = 0x30b;
            } else {
                field_0x1d8[6][0] = 0x30a;
            }
        }
    } else {
        if (dMenu_Collect3D_c::getCrystalNum() <= 1) {
            field_0x1d8[6][0] = 0x5ac;
        } else {
            field_0x1d8[6][0] = 0x5ad;
        }
    }
    field_0x1d8[0][1] = 0;
    field_0x1d8[1][1] = 0;
    field_0x1d8[2][1] = 0;
    field_0x1d8[3][1] = field_0x184[3][1] + 0x100;
    field_0x1d8[4][1] = field_0x184[4][1] + 0x100;
    field_0x1d8[5][1] = field_0x184[5][1] + 0x100;
    field_0x1d8[6][1] = 0;
    field_0x1d8[0][2] = 0;
    field_0x1d8[1][2] = 0;
    field_0x1d8[2][2] = 0;
    field_0x1d8[3][2] = field_0x184[3][2] + 0x100;
    field_0x1d8[4][2] = field_0x184[4][2] + 0x100;
    field_0x1d8[5][2] = field_0x184[5][2] + 0x100;
    field_0x1d8[6][2] = 0;
    field_0x1d8[0][3] = field_0x184[0][3] + 0x100;
    field_0x1d8[1][3] = field_0x184[1][3] + 0x100;
    field_0x1d8[2][3] = 0x5b9;
    field_0x1d8[3][3] = 0x5b1;
    field_0x1d8[4][3] = 0;
    field_0x1d8[5][3] = 0;
    field_0x1d8[6][3] = 0;
    field_0x1d8[0][4] = dMsgObject_getSmellTypeMessageID() + 0x100;
        /* dSv_event_flag_c::F_0456 - Castle Town - First time meeting with Jovani */
    if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[456])) {
        field_0x1d8[1][4] = 0x12d8;
    } else {
        field_0x1d8[1][4] = field_0x184[1][4] + 0x100;
    }
    field_0x1d8[2][4] = 0x5a2;
    field_0x1d8[3][4] = 0x4c9;
    field_0x1d8[4][4] = 0;
    field_0x1d8[5][4] = 0;
    field_0x1d8[6][4] = 0;
    field_0x1d8[0][5] = 0x4c5;
    field_0x1d8[1][5] = 0x4c6;
    field_0x1d8[2][5] = 0;
    field_0x1d8[3][5] = 0;
    field_0x1d8[4][5] = 0;
    field_0x1d8[5][5] = 0;
    field_0x1d8[6][5] = 0;
    mCursorX = dMeter2Info_getCollectCursorPosX();
    mCursorY = dMeter2Info_getCollectCursorPosY();
    if (dMenu_Collect3D_c::getMaskMdlVisible() == 0) {
        if (mCursorX == 6 && mCursorY == 0) {
            mCursorX = 3;
            mCursorY = 0;
        }
    }
    field_0x259 = mCursorX;
    field_0x25a = mCursorY;
    mCurrentAString = 0;
    mCurrentBString = 0;
    field_0x180 = 0;
    mItemNameString = 0;
    mEquippedSword = 0xff;
    mEquippedShield = 0xff;
    mEquippedClothes = 0xff;
    mSubWindowOpenCheck = 0;
    field_0x25f = 0;
    field_0x260 = 0xff;
    field_0x261 = 0xff;
    setEquipItemFrameColorSword(-1);
    setEquipItemFrameColorShield(-1);
    setEquipItemFrameColorClothes(-1);
    u8 i_copy;
    u8 j_copy;
    bool bVar18 = false;
    bool bVar2 = false;
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (getItemTag(i, j, true)) {
                mpSelPm[i][j] = new CPaneMgr(mpScreen, getItemTag(i, j, true), 0, NULL);
                if (!bVar18) {
                    i_copy = i;
                    j_copy = j;
                    bVar18 = true;
                }
                if (!bVar2) {
                    if (mCursorX == i && mCursorY == j) {
                        bVar2 = true;
                    }
                }
            } else {
                mpSelPm[i][j] = NULL;
            }
            if (getItemTag(i, j, false)) {
                if (field_0x22d[i][j] != 0 || (i == 5 && j == 0)) {
                    mpScreen->search(getItemTag(i, j, false))->show();
                } else {
                    mpScreen->search(getItemTag(i, j, false))->hide();
                }
            }
        }
    }
    if (!bVar2) {
        mCursorX = i_copy;
        mCursorY = j_copy;
    }
    mpLinkPm = new CPaneMgr(mpScreen, 'linki_n', 0, NULL);
    mLinkGlobalCenterPos.x = mpLinkPm->getInitGlobalCenterPosX();
    mLinkGlobalCenterPos.y = mpLinkPm->getInitGlobalCenterPosY();
    mLinkGlobalCenterPos.z = -1000.0f;
    mpMaskPm = new CPaneMgr(mpScreen, 'kamen_n', 0, NULL);
    mpModelBg = new CPaneMgr(mpScreen, 'modelbgn', 2, NULL);
    mpHeartParent = new CPaneMgr(mpScreen, 'heart_n', 0, NULL);
    mpHeartPiece = new CPaneMgr(mpScreen, 'heart_kn', 0, NULL);
    setAButtonString(mCurrentAString);
    setBButtonString(mCurrentBString);
    setItemNameString(mCursorX, mCursorY);
    cursorPosSet();
    setArrowMaxNum(dComIfGs_getArrowMax());
    setWalletMaxNum(dComIfGs_getRupeeMax());
    setSmellType();
    setHeartPiece();
    setPohMaxNum(dComIfGs_getPohSpiritNum());
}

void dMenu_Collect2D_c::animationSet() {
    OSInitFastCast();
    void* resource =
        JKRGetNameResource("zelda_collect_soubi_screen.btk", dComIfGp_getCollectResArchive());
    mpAnmKey = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(resource);
    mpAnmKey->searchUpdateMaterialID(mpScreen);
    field_0x34 = 0.0f;
    mFrame = 0.0f;
    field_0x3c = 0.0f;
}

void dMenu_Collect2D_c::btkAnimeLoop0(J2DAnmTextureSRTKey* i_SRTKey) {
    if (i_SRTKey != NULL) {
        mFrame++;
        if (mFrame >= i_SRTKey->getFrameMax()) {
            mFrame -= i_SRTKey->getFrameMax();
        }
        i_SRTKey->setFrame(mFrame);
    } else {
        mFrame = 0.0f;
    }
    static_cast<J2DTextBox*>(mpScreen->search('modelbg0'))->setAnimation(i_SRTKey);
    static_cast<J2DTextBox*>(mpScreen->search('modelbg1'))->setAnimation(i_SRTKey);
    static_cast<J2DTextBox*>(mpScreen->search('modelbg2'))->setAnimation(i_SRTKey);
    static_cast<J2DTextBox*>(mpScreen->search('modelbg3'))->setAnimation(i_SRTKey);
    static_cast<J2DTextBox*>(mpScreen->search('modelbg4'))->setAnimation(i_SRTKey);
    static_cast<J2DTextBox*>(mpScreen->search('modelbg5'))->setAnimation(i_SRTKey);
}

void dMenu_Collect2D_c::setBackAlpha() {
    f32 alpha = mpBlackTex->getAlpha() / 150.0f;

    switch (mProcess) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
        if (alpha < 1.0f) {
            alpha += 0.2f;
        }
        if (alpha > 1.0f) {
            alpha = 1.0f;
        }
        break;

    default:
        if (alpha > 0.0f) {
            alpha -= 0.2f;
        }
        if (alpha < 0.0f) {
            alpha = 0.0f;
        }
        break;
    }

    mpBlackTex->setAlpha(alpha * 150.0f);
}

// Not sure if this works without gotos
void dMenu_Collect2D_c::cursorMove() {
    u8 dVar1 = mCursorX;
    u8 dVar2 = mCursorY;
    mpStick->checkTrigger();
    if (mpStick->checkRightTrigger()) {
        if (mCursorX < 6) {
            if (mCursorX == 0 && mCursorY == 0 &&
                field_0x259 >= 3 && field_0x25a < 3)
            {
                mCursorX = field_0x259;
                mCursorY = field_0x25a;
            } else {
                do {
                    mCursorX++;
                    if (mCursorX == 6 && mCursorY < 3) {
                        if (field_0x22d[6][0] != 0) {
                            if (getItemTag(6, 0, false) && field_0x22d[6][0] != 0) {
                                mCursorX = 6;
                                mCursorY = 0;
                            }
                        } else {
                            mCursorX = dVar1;
                            mCursorY = dVar2;
                            goto LAB_802ba744;
                        }
                    }
                    if (dVar1 == 4 && dVar2 == 1) {
                        mCursorX = 5;
                        mCursorY = 0;
                        goto LAB_802ba744;
                    }
                    if (getItemTag(mCursorX, mCursorY, true)) {
                        goto LAB_802ba744;
                    }
                } while (mCursorX < 6);
                mCursorX = dVar1;
            }
        }
    } else {
        if (mpStick->checkLeftTrigger() && mCursorX != 0) {
            if (mCursorX == 6 &&
                mCursorY == 0 && field_0x259 >= 3 &&
                  field_0x25a < 3)
            {
                mCursorX = field_0x259;
                mCursorY = field_0x25a;
            } else if (dVar1 == 5 && dVar2 == 0 &&
                        field_0x259 == 4 &&
                       field_0x25a == 1)
            {
                mCursorX = field_0x259;
                mCursorY = field_0x25a;
            } else {
                do {
                    mCursorX--;
                    if (getItemTag(mCursorX, mCursorY, true)) {
                        goto LAB_802ba744;
                    }
                } while (mCursorX != 0);
                mCursorX = dVar1;
            }
        }
    }
LAB_802ba744:
    if (mpStick->checkUpTrigger()) {
        if (mCursorY != 0) {
        begin:
            mCursorY--;
            if (mCursorY == 2) {
                u8 local_3c[9] = {3,3,4,3,4,5,4,5,5};
                u8 local_48[9] = {2,1,2,0,1,2,0,1,0};
                for (int i = 0; i < 9; i++) {
                    if (getItemTag(local_3c[i], local_48[i], true)) {
                        mCursorX = local_3c[i];
                        mCursorY = local_48[i];
                        break;
                    }
                }
            }
            if (dVar2 == 5) {
                if (dVar1 == 0) {
                    if (field_0x25a < 5) {
                        mCursorX = field_0x259;
                        mCursorY = field_0x25a;
                        goto LAB_802bab54;
                    } else {
                        bool bVar3 = false;
                        for (int i = 0; i < 4; i++) {
                            if (getItemTag(i, mCursorY, true)) {
                                mCursorX = i;
                                bVar3 = true;
                                break;
                            }
                        }
                        if (mCursorY == 0) {
                            mCursorX = dVar1;
                            mCursorY = dVar2;
                            bVar3 = true;
                        }
                        if (bVar3) {
                            goto LAB_802bab54;
                        }
                    }
                } else if (field_0x25a < 5) {
                    mCursorX = field_0x259;
                    mCursorY = field_0x25a;
                    goto LAB_802bab54;
                } else {
                    bool bVar4 = false;
                    for (int i = 3; i < 7; i++) {
                        if (getItemTag(i, mCursorY, true)) {
                            mCursorX = i;
                            bVar4 = true;
                            break;
                        }
                    }
                    if (mCursorY == 0) {
                        mCursorX = dVar1;
                        mCursorY = dVar2;
                        bVar4 = true;
                    }
                    if (bVar4) {
                        goto LAB_802bab54;
                    }
                }
                goto begin;
            } else {
                if (getItemTag(mCursorX, mCursorY, true)) {
                    goto LAB_802bab54;
                }
                if (mCursorY != 0) {
                    goto begin;
                }
                mCursorY = dVar2;
            }
        }
    } else if (mpStick->checkDownTrigger()) {
        if (mCursorY < 4) {
            do {
                mCursorY++;
                if (mCursorY == 3) {
                    u8 local_50[8] = {3, 2, 3, 1, 2, 0, 1, 0};
                    u8 local_58[8] = {3, 3, 4, 3, 4, 3, 4, 4};
                    for (int i = 0; i < 8; i++) {
                        if (getItemTag(local_50[i], local_58[i], true)) {
                            mCursorX = local_50[i];
                            mCursorY = local_58[i];
                            break;
                        }
                    }
                }
                if (getItemTag(mCursorX, mCursorY, true)) {
                    goto LAB_802bab54;
                }
            } while (mCursorY < 4);
            mCursorY = 5;
            if (mCursorX <= 2) {
                mCursorX = 0;
            } else {
                mCursorX = 1;
            }
        } else if (mCursorY == 4) {
            mCursorY = 5;
            if (mCursorX <= 3) {
                mCursorX = 0;
            } else {
                mCursorX = 1;
            }
        }
    }

LAB_802bab54:
    if (mCursorX != dVar1 || mCursorY != dVar2) {
        field_0x259 = dVar1;
        field_0x25a = dVar2;
        if ((mCursorX == 1 && mCursorY == 5) || (mCursorX == 0 && mCursorY == 5)) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OPTION, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        } else {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        cursorPosSet();
        setItemNameString(mCursorX, mCursorY);
    }
    if (field_0x44[0] != g_drawHIO.mCollectScreen.mUnselectItemScale ||
        field_0x44[1] != g_drawHIO.mCollectScreen.mSelectItemScale ||
        field_0x4c[0] != g_drawHIO.mCollectScreen.mUnselectSaveOptionScale ||
        field_0x4c[1] != g_drawHIO.mCollectScreen.mSelectSaveOptionScale)
    {
        field_0x44[0] = g_drawHIO.mCollectScreen.mUnselectItemScale;
        field_0x44[1] = g_drawHIO.mCollectScreen.mSelectItemScale;
        field_0x4c[0] = g_drawHIO.mCollectScreen.mUnselectSaveOptionScale;
        field_0x4c[1] = g_drawHIO.mCollectScreen.mSelectSaveOptionScale;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 6; j++) {
                if (getItemTag(i, j, true)) {
                    J2DPane* pane = mpScreen->search(getItemTag(i, j, true));
                    if ((i != 0 || j != 0) && (i != 6 || j != 0)) {
                        if (j == 5) {
                            if (i == mCursorX && j == mCursorY) {
                                pane->scale(g_drawHIO.mCollectScreen.mSelectSaveOptionScale,
                                            g_drawHIO.mCollectScreen.mSelectSaveOptionScale);
                            } else {
                                pane->scale(g_drawHIO.mCollectScreen.mUnselectSaveOptionScale,
                                            g_drawHIO.mCollectScreen.mUnselectSaveOptionScale);
                            }
                        } else if (i == mCursorX && j == mCursorY) {
                            pane->scale(g_drawHIO.mCollectScreen.mSelectItemScale,
                                        g_drawHIO.mCollectScreen.mSelectItemScale);
                        } else {
                            pane->scale(g_drawHIO.mCollectScreen.mUnselectItemScale,
                                        g_drawHIO.mCollectScreen.mUnselectItemScale);
                        }
                    }
                }
            }
        }
    }
}

void dMenu_Collect2D_c::cursorPosSet() {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 6; j++) {
            if (getItemTag(i, j, true) != 0) {
                J2DPane* itemTag = mpScreen->search(getItemTag(i, j, true));
                if ((i != 0 || j != 0) && (i != 6 || j != 0)) {
                    if (j == 5) {
                        if (i == mCursorX && j == mCursorY) {
                            itemTag->scale(g_drawHIO.mCollectScreen.mSelectSaveOptionScale,
                                           g_drawHIO.mCollectScreen.mSelectSaveOptionScale);
                        } else {
                            itemTag->scale(g_drawHIO.mCollectScreen.mUnselectSaveOptionScale,
                                           g_drawHIO.mCollectScreen.mUnselectSaveOptionScale);
                        }
                    } else if (i == mCursorX && j == mCursorY) {
                        itemTag->scale(g_drawHIO.mCollectScreen.mSelectItemScale,
                                       g_drawHIO.mCollectScreen.mSelectItemScale);
                    } else {
                        itemTag->scale(g_drawHIO.mCollectScreen.mUnselectItemScale,
                                       g_drawHIO.mCollectScreen.mUnselectItemScale);
                    }
                }
            }
        }
    }

    mpDrawCursor->setAlphaRate(1.0f);
    Vec pos = mpSelPm[mCursorX][mCursorY]->getGlobalVtxCenter(false, 0);
    mpDrawCursor->setPos(pos.x, pos.y, mpSelPm[mCursorX][mCursorY]->getPanePtr(), false);
    if (mCursorY == 5) {
        mpDrawCursor->setParam(1.1f, 0.85f, 0.05f, 0.5f, 0.5f);
    } else if (mCursorX == 6 && mCursorY == 0) {
        mpDrawCursor->setParam(0.6f, 0.85f, 0.03f, 0.6f, 0.6f);
    } else {
        mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.7f, 0.7f);
    }
}

void dMenu_Collect2D_c::changeSword() {
    switch (mCursorX) {
    case 3:
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_SWORD)) {
            if (dComIfGs_getSelectEquipSword() != fpcNm_ITEM_SWORD) {
                dMeter2Info_setSword(fpcNm_ITEM_SWORD, false);
                setEquipItemFrameColorSword(0);
                mDoAud_seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0);
                dMeter2Info_set2DVibration();
            }
        } else if (dComIfGs_getSelectEquipSword() != fpcNm_ITEM_WOOD_STICK) {
            dMeter2Info_setSword(fpcNm_ITEM_WOOD_STICK, false);
            setEquipItemFrameColorSword(0);
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    case 4:
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_LIGHT_SWORD)) {
            if (dComIfGs_getSelectEquipSword() != fpcNm_ITEM_LIGHT_SWORD) {
                dMeter2Info_setSword(fpcNm_ITEM_LIGHT_SWORD, false);
                setEquipItemFrameColorSword(1);
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            }
        } else if (dComIfGs_getSelectEquipSword() != fpcNm_ITEM_MASTER_SWORD) {
            dMeter2Info_setSword(fpcNm_ITEM_MASTER_SWORD, false);
            setEquipItemFrameColorSword(1);
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    case 5:
        if (dComIfGs_getSelectEquipSword() != fpcNm_ITEM_LIGHT_SWORD) {
            dMeter2Info_setSword(fpcNm_ITEM_LIGHT_SWORD, false);
            setEquipItemFrameColorSword(2);
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    }
}

void dMenu_Collect2D_c::changeShield() {
    switch (mCursorX) {
    case 3:
        if (dComIfGs_isItemFirstBit(fpcNm_ITEM_SHIELD)) {
            if (dComIfGs_getSelectEquipShield() != fpcNm_ITEM_SHIELD) {
                dMeter2Info_setShield(fpcNm_ITEM_SHIELD, false);
                setEquipItemFrameColorShield(0);
                daAlink_getAlinkActorClass()->setShieldChange();
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            }
        } else if (dComIfGs_isItemFirstBit(fpcNm_ITEM_WOOD_SHIELD)) {
            if (dComIfGs_getSelectEquipShield() != fpcNm_ITEM_WOOD_SHIELD) {
                dMeter2Info_setShield(fpcNm_ITEM_WOOD_SHIELD, false);
                setEquipItemFrameColorShield(0);
                daAlink_getAlinkActorClass()->setShieldChange();
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            }
        }
        break;
    case 4:
        if (dComIfGs_getSelectEquipShield() != fpcNm_ITEM_HYLIA_SHIELD) {
            dMeter2Info_setShield(fpcNm_ITEM_HYLIA_SHIELD, false);
            setEquipItemFrameColorShield(1);
            daAlink_getAlinkActorClass()->setShieldChange();
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    }
}

void dMenu_Collect2D_c::changeClothe() {
    switch (mCursorX) {
    case 3:
        if (dComIfGs_getSelectEquipClothes() != fpcNm_ITEM_WEAR_KOKIRI) {
            dMeter2Info_setCloth(fpcNm_ITEM_WEAR_KOKIRI, false);
            setEquipItemFrameColorClothes(0);
            daPy_getPlayerActorClass()->setClothesChange(0);
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    case 4:
        if (dComIfGs_getSelectEquipClothes() != fpcNm_ITEM_WEAR_ZORA) {
            dMeter2Info_setCloth(fpcNm_ITEM_WEAR_ZORA, false);
            setEquipItemFrameColorClothes(1);
            daPy_getPlayerActorClass()->setClothesChange(0);
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    case 5:
        if (dComIfGs_getSelectEquipClothes() != fpcNm_ITEM_ARMOR) {
            dMeter2Info_setCloth(fpcNm_ITEM_ARMOR, false);
            setEquipItemFrameColorClothes(2);
            daPy_getPlayerActorClass()->setClothesChange(0);
            Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        }
        break;
    }
}

void dMenu_Collect2D_c::setArrowMaxNum(u8 param_0) {
    switch (param_0) {
    case 0:
        mpScreen->search('item_0_0')->hide();
        mpScreen->search('item_0_1')->hide();
        mpScreen->search('item_0_2')->hide();
        break;
    case 30:
        mpScreen->search('item_0_0')->show();
        mpScreen->search('item_0_1')->hide();
        mpScreen->search('item_0_2')->hide();
        break;
    case 60:
        mpScreen->search('item_0_0')->hide();
        mpScreen->search('item_0_1')->show();
        mpScreen->search('item_0_2')->hide();
        break;
    case 100:
        mpScreen->search('item_0_0')->hide();
        mpScreen->search('item_0_1')->hide();
        mpScreen->search('item_0_2')->show();
        break;
    }
}

void dMenu_Collect2D_c::setWalletMaxNum(u16 i_walletSize) {
    switch (i_walletSize) {
    case 300:
        mpScreen->search('item_1_0')->show();
        mpScreen->search('item_1_1')->hide();
        mpScreen->search('item_1_2')->hide();
        break;
    case 600:
        mpScreen->search('item_1_0')->hide();
        mpScreen->search('item_1_1')->show();
        mpScreen->search('item_1_2')->hide();
        break;
    case 1000:
        mpScreen->search('item_1_0')->hide();
        mpScreen->search('item_1_1')->hide();
        mpScreen->search('item_1_2')->show();
        break;
    }
}

void dMenu_Collect2D_c::setSmellType() {
    static const u64 smell_tag[5] = {
        'wolf_med', 'wolf_chi', 'wolf_fis', 'wolf_iri', 'wolf_pou',
    };
    s32 smellType = -1;
    switch (dComIfGs_getCollectSmell()) {
    case fpcNm_ITEM_SMELL_YELIA_POUCH:
        smellType = 3;
        break;
    case fpcNm_ITEM_SMELL_POH:
        smellType = 4;
        break;
    case fpcNm_ITEM_SMELL_FISH:
        smellType = 2;
        break;
    case fpcNm_ITEM_SMELL_CHILDREN:
        smellType = 1;
        break;
    case fpcNm_ITEM_SMELL_MEDICINE:
        smellType = 0;
        break;
    }
    for (int i = 0; i < 5; i++) {
        if (i == smellType) {
            mpScreen->search(smell_tag[i])->show();
        } else {
            mpScreen->search(smell_tag[i])->hide();
        }
    }
}

void dMenu_Collect2D_c::setHeartPiece() {
    static const u64 heart_tag[4] = {
        'heart_1n',
        'heart_2n',
        'heart_3n',
        'heart_4n',
    };
    s32 life = dComIfGs_getMaxLife() % 5;
    for (int i = 0; i < MAX_VISIBLE_HEARTPIECES; i++) {
        if (i < life) {
            mpScreen->search(heart_tag[i])->show();
        } else {
            mpScreen->search(heart_tag[i])->hide();
        }
    }
}

void dMenu_Collect2D_c::setPohMaxNum(u8 i_pohNum) {
    if (i_pohNum > MAX_POH_NUM) {
        i_pohNum = MAX_POH_NUM;
    }
    if (field_0x22d[0][3] != 0) {
        int div = i_pohNum / 100;
        s32 index = i_pohNum % 100;
        u8 check = 0;
        if (div > 0) {
            JKRArchive* archive = dComIfGp_getMain2DArchive();
            const char* textureName = dMeter2Info_getNumberTextureName(div);
            ResTIMG* timg = (ResTIMG*)archive->getResource('TIMG', textureName);
            static_cast<J2DPicture*>(mpScreen->search('item2_3'))->changeTexture(timg, 0);
            check = 1;
        } else {
            mpScreen->search('item2_3')->hide();
        }
        div = index / 10;
        index = index % 10;
        if (div != 0 || check != 0) {
            JKRArchive* archive = dComIfGp_getMain2DArchive();
            const char* textureName2 = dMeter2Info_getNumberTextureName(div);
            ResTIMG* timg2 = (ResTIMG*)archive->getResource('TIMG', textureName2);
            static_cast<J2DPicture*>(mpScreen->search('item2_2'))->changeTexture(timg2, 0);
        } else {
            mpScreen->search('item2_2')->hide();
        }
        JKRArchive* archive = dComIfGp_getMain2DArchive();
        const char* textureName3 = dMeter2Info_getNumberTextureName(index);
        ResTIMG* timg3 = (ResTIMG*)archive->getResource('TIMG', textureName3);
        static_cast<J2DPicture*>(mpScreen->search('item2_1'))->changeTexture(timg3, 0);
    } else {
        mpScreen->search('item2_3')->hide();
        mpScreen->search('item2_2')->hide();
        mpScreen->search('item2_1')->hide();
    }
}

void dMenu_Collect2D_c::setEquipItemFrameColorSword(int i_frame) {
    static const u64 tag[2] = {
        'ken_g_0',
        'ken_g_1',
    };

    if (i_frame == -1) {
        if (mEquippedSword != dComIfGs_getSelectEquipSword()) {
            mEquippedSword = dComIfGs_getSelectEquipSword();

            switch (mEquippedSword) {
            case fpcNm_ITEM_SWORD:
                i_frame = 0;
                break;

            case fpcNm_ITEM_WOOD_STICK:
                i_frame = 0;
                break;

            case fpcNm_ITEM_MASTER_SWORD:
                i_frame = 1;
                break;

            case fpcNm_ITEM_LIGHT_SWORD:
                if (dComIfGs_isItemFirstBit(fpcNm_ITEM_LIGHT_SWORD)) {
                    i_frame = 1;
                }
                break;
            }

            for (int i = 0; i < 2; i++) {
                if (i == i_frame && field_0x22d[i + 3][0] != 0) {
                    static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                        ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                        JUtility::TColor(255, 255, 0, 255));
                } else {
                    static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                        ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                        JUtility::TColor(107, 107, 107, 255));
                }
            }
        }
    } else {
        for (int i = 0; i < 2; i++) {
            if (i == i_frame && field_0x22d[i + 3][0] != 0) {
                static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                    ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                    JUtility::TColor(255, 255, 0, 255));
            } else {
                static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                    ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                    JUtility::TColor(107, 107, 107, 255));
            }
        }
    }
}

void dMenu_Collect2D_c::setEquipItemFrameColorShield(int i_frame) {
    static const u64 tag[2] = {
        'tate_g_0',
        'tate_g_1',
    };

    if (i_frame == -1) {
        if (mEquippedShield != dComIfGs_getSelectEquipShield()) {
            mEquippedShield = dComIfGs_getSelectEquipShield();

            switch (mEquippedShield) {
            case fpcNm_ITEM_SHIELD:
            case fpcNm_ITEM_WOOD_SHIELD:
                i_frame = 0;
                break;

            case fpcNm_ITEM_HYLIA_SHIELD:
                i_frame = 1;
                break;
            }

            for (int i = 0; i < 2; i++) {
                if (i == i_frame && field_0x22d[i + 3][1] != 0) {
                    static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                        ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                        JUtility::TColor(255, 255, 0, 255));
                } else {
                    static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                        ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                        JUtility::TColor(107, 107, 107, 255));
                }
            }
        }
    } else {
        for (int i = 0; i < 2; i++) {
            if (i == i_frame && field_0x22d[i + 3][1] != 0) {
                static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                    ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                    JUtility::TColor(255, 255, 0, 255));
            } else {
                static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                    ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                    JUtility::TColor(107, 107, 107, 255));
            }
        }
    }
}

void dMenu_Collect2D_c::setEquipItemFrameColorClothes(int i_frame) {
    static const u64 tag[3] = {
        'fuku_g_0',
        'fuku_g_1',
        'fuku_g_2',
    };

    if (i_frame == -1) {
        if (mEquippedClothes != dComIfGs_getSelectEquipClothes()) {
            mEquippedClothes = dComIfGs_getSelectEquipClothes();

            switch (mEquippedClothes) {
            case fpcNm_ITEM_WEAR_KOKIRI:
                i_frame = 0;
                break;

            case fpcNm_ITEM_WEAR_ZORA:
                i_frame = 1;
                break;

            case fpcNm_ITEM_ARMOR:
                i_frame = 2;
                break;
            }

            for (int i = 0; i < 3; i++) {
                if (i == i_frame && field_0x22d[i + 3][2] != 0) {
                    static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                        ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                        JUtility::TColor(255, 255, 0, 255));
                } else {
                    static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                        ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                        JUtility::TColor(107, 107, 107, 255));
                }
            }
        }
    } else {
        for (int i = 0; i < 3; i++) {
            if (i == i_frame && field_0x22d[i + 3][2] != 0) {
                static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                    ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                    JUtility::TColor(255, 255, 0, 255));
            } else {
                static_cast<J2DPicture*>(mpScreen->search(tag[i]))
                    ->setBlackWhite(JUtility::TColor(0, 0, 0, 0),
                                    JUtility::TColor(107, 107, 107, 255));
            }
        }
    }
}

void dMenu_Collect2D_c::setHIO(bool i_useHIO) {
    static const u64 kaz_n[9] = {
        'uzu_00', 'uzu_01', 'uzu_02', 'uzu_03', 'uzu_04', 'uzu_05', 'uzu_06', 'uzu_07', 'hishi',
    };

    if (mBlueSmokePosX != g_drawHIO.mCollectScreen.mBlueSmokePosX ||
        mBlueSmokePosY != g_drawHIO.mCollectScreen.mBlueSmokePosY)
    {
        mBlueSmokePosX = g_drawHIO.mCollectScreen.mBlueSmokePosX;
        mBlueSmokePosY = g_drawHIO.mCollectScreen.mBlueSmokePosY;
        mpModelBg->paneTrans(mBlueSmokePosX, mBlueSmokePosY);
    }

    if (mBlueSmokeScale != g_drawHIO.mCollectScreen.mBlueSmokeScale) {
        mBlueSmokeScale = g_drawHIO.mCollectScreen.mBlueSmokeScale;
        mpModelBg->scale(mBlueSmokeScale, mBlueSmokeScale);
    }

    if (mBlueSmokeAlpha != g_drawHIO.mCollectScreen.mBlueSmokeAlpha) {
        mBlueSmokeAlpha = g_drawHIO.mCollectScreen.mBlueSmokeAlpha;
        mpModelBg->setAlphaRate(mBlueSmokeAlpha);
    }

    if (mHeartVesselPosX != g_drawHIO.mCollectScreen.mHeartVesselPosX ||
        mHeartVesselPosY != g_drawHIO.mCollectScreen.mHeartVesselPosY)
    {
        mHeartVesselPosX = g_drawHIO.mCollectScreen.mHeartVesselPosX;
        mHeartVesselPosY = g_drawHIO.mCollectScreen.mHeartVesselPosY;
        mpHeartParent->paneTrans(mHeartVesselPosX, mHeartVesselPosY);
    }

    if (mHeartVesselScale != g_drawHIO.mCollectScreen.mHeartVesselScale) {
        mHeartVesselScale = g_drawHIO.mCollectScreen.mHeartVesselScale;
        mpHeartParent->scale(mHeartVesselScale, mHeartVesselScale);
    }

    if (mHeartPiecePosX != g_drawHIO.mCollectScreen.mHeartPiecePosX ||
        mHeartPiecePosY != g_drawHIO.mCollectScreen.mHeartPiecePosY)
    {
        mHeartPiecePosX = g_drawHIO.mCollectScreen.mHeartPiecePosX;
        mHeartPiecePosY = g_drawHIO.mCollectScreen.mHeartPiecePosY;
        mpHeartPiece->paneTrans(mHeartPiecePosX, mHeartPiecePosY);
    }

    if (mHeartPieceScale != g_drawHIO.mCollectScreen.mHeartPieceScale) {
        mHeartPieceScale = g_drawHIO.mCollectScreen.mHeartPieceScale;
        mpHeartPiece->scale(mHeartPieceScale, mHeartPieceScale);
    }

    if (g_drawHIO.mCollectScreen.mColorDebugON) {
        J2DPicture* basePicture = (J2DPicture*)mpScreen->search('nht_base');
        basePicture->setBlackWhite(g_drawHIO.mCollectScreen.mVesselBack[0],
                                   g_drawHIO.mCollectScreen.mVesselFront[0]);

        for (int i = 0; i < 9; i++) {
            J2DPicture* kazPicture = (J2DPicture*)mpScreen->search(kaz_n[i]);
            kazPicture->setBlackWhite(g_drawHIO.mCollectScreen.mVesselBack[1],
                                      g_drawHIO.mCollectScreen.mVesselFront[1]);
        }
    }

    if (g_drawHIO.mCollectScreen.mButtonDebugON || i_useHIO) {
        if (mpButtonAB[0] != NULL) {
            mpButtonAB[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonAPosX,
                                     g_drawHIO.mCollectScreen.mButtonAPosY);
            mpButtonAB[0]->scale(g_drawHIO.mCollectScreen.mButtonAScale,
                                 g_drawHIO.mCollectScreen.mButtonAScale);
        }
        if (mpButtonAB[1] != NULL) {
            mpButtonAB[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBPosX,
                                     g_drawHIO.mCollectScreen.mButtonBPosY);
            mpButtonAB[1]->scale(g_drawHIO.mCollectScreen.mButtonBScale,
                                 g_drawHIO.mCollectScreen.mButtonBScale);
        }
        if (mpButtonText[0] != NULL) {
            mpButtonText[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonATextPosX,
                                       g_drawHIO.mCollectScreen.mButtonATextPosY);
            mpButtonText[0]->scale(g_drawHIO.mCollectScreen.mButtonATextScale,
                                   g_drawHIO.mCollectScreen.mButtonATextScale);
        }
        if (mpButtonText[1] != NULL) {
            mpButtonText[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBTextPosX,
                                       g_drawHIO.mCollectScreen.mButtonBTextPosY);
            mpButtonText[1]->scale(g_drawHIO.mCollectScreen.mButtonBTextScale,
                                   g_drawHIO.mCollectScreen.mButtonBTextScale);
        }
    }
}

u64 dMenu_Collect2D_c::getItemTag(int i_tag1, int i_tag2, bool param_3) {
    static const u64 itemTag[6][7] = {
        {0, 0, 0, 'ken_n0', 'ken_n1', 'heart_kn', 'kamen_n'},
        {0, 0, 0, 'tate_n0', 'tate_n1'},
        {
            0,
            0,
            0,
            'fuku_n0',
            'fuku_n1',
            'fuku_n2',
        },
        {
            'item_1_n',
            'item_0_n',
            'kabu_6n',
            'maki_5_n',
        },
        {
            'wolf_n',
            'item_2_n',
            'fish_3_n',
            'lett_4_n',
        },
        {'save_n', 'option_n'},
    };

    if (i_tag2 == 5 && !param_3) {
        return 0;
    }
    return itemTag[i_tag2][i_tag1];
}

void dMenu_Collect2D_c::wait_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);

    if (mpSaveScrn != NULL) {
        mpSaveScrn->_delete();
        delete mpSaveScrn;
        mpSaveScrn = NULL;
        mpSubHeap->freeAll();
    }

    if (mpOptionScrn != NULL) {
        mpOptionScrn->_delete();
        delete mpOptionScrn;
        mpOptionScrn = NULL;
        mpSubHeap->freeAll();
    }

    if (mpLetterScrn != NULL) {
        delete mpLetterScrn;
        mpLetterScrn = NULL;
        mpSubHeap->freeAll();
    }

    if (mpFishingScrn != NULL) {
        delete mpFishingScrn;
        mpFishingScrn = NULL;
        mpSubHeap->freeAll();
    }

    if (mpSkillScrn != NULL) {
        delete mpSkillScrn;
        mpSkillScrn = NULL;
        mpSubHeap->freeAll();
    }

    if (mpInsectScrn != NULL) {
        delete mpInsectScrn;
        mpInsectScrn = NULL;
        mpSubHeap->freeAll();
    }

    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::wait_proc() {
    if (dMw_A_TRIGGER()) {
        if (mCursorX == 0 && mCursorY == 5) {
            if (mDoGph_gInf_c::getFader()->mStatus == 1) {
                mSubWindowOpenCheck = 1;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CHANGE_WINDOW, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
                dMeter2Info_set2DVibrationM();
            }
        } else if (mCursorX == 1 && mCursorY == 5) {
            if (mDoGph_gInf_c::getFader()->mStatus == 1) {
                mSubWindowOpenCheck = 2;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CHANGE_WINDOW, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
                dMeter2Info_set2DVibrationM();
            }
        } else if (mCursorX == 3 && mCursorY == 4) {
            if (field_0x22d[3][4] != 0 && mDoGph_gInf_c::getFader()->mStatus == 1) {
                mSubWindowOpenCheck = 3;
                dMeter2Info_set2DVibration();
            }
        } else if (mCursorX == 2 && mCursorY == 4) {
            if (isFishIconVisible() && mDoGph_gInf_c::getFader()->mStatus == 1) {
                mSubWindowOpenCheck = 4;
                dMeter2Info_set2DVibration();
            }
        } else if (mCursorX == 3 && mCursorY == 3) {
            if (isSkillIconVisible() && mDoGph_gInf_c::getFader()->mStatus == 1) {
                mSubWindowOpenCheck = 5;
                dMeter2Info_set2DVibration();
            }
        } else if (mCursorX == 2 && mCursorY == 3) {
            if (isInsectIconVisible() && mDoGph_gInf_c::getFader()->mStatus == 1) {
                mSubWindowOpenCheck = 6;
                dMeter2Info_set2DVibration();
            }
        } else {
            if (field_0x22d[mCursorX][mCursorY] != 0 && !mIsWolf) {
                if ((mCursorX >= 3 && mCursorX <= 4) || (mCursorX == 5 && mCursorY == 2)) {
                    u8 cursorY = mCursorY;
                    if (cursorY == 0) {
                        if (daPy_getPlayerActorClass()->getSwordChangeWaitTimer() == 0) {
                            changeSword();
                        }
                    } else if (cursorY == 1) {
                        if (daPy_getPlayerActorClass()->getShieldChangeWaitTimer() == 0) {
                            changeShield();
                        }
                    } else if (cursorY == 2 &&
                               daPy_getPlayerActorClass()->getClothesChangeWaitTimer() == 0)
                    {
                        changeClothe();
                    }
                }
            }
        }
    } else {
        if (daPy_getPlayerActorClass()->getSwordChangeWaitTimer() == 0 &&
            daPy_getPlayerActorClass()->getShieldChangeWaitTimer() == 0 &&
            daPy_getPlayerActorClass()->getClothesChangeWaitTimer() == 0)
        {
            cursorMove();
        }
        if (mCursorX >= 3 && mCursorX <= 4 && mCursorY <= 2 || (mCursorX == 5 && mCursorY == 2)) {
            if (field_0x22d[mCursorX][mCursorY] != 0 && !mIsWolf) {
                setAButtonString(0x436);
            } else {
                setAButtonString(0);
            }
        } else if ((mCursorX == 0 && mCursorY == 5) || (mCursorX == 1 && mCursorY == 5)) {
            setAButtonString(0x40C);
        } else {
            if (field_0x22d[mCursorX][mCursorY] != 0) {
                if (mCursorX == 3 && mCursorY == 4) {
                    if (field_0x22d[3][4] != 0) {
                        setAButtonString(0x3EE);
                    }
                } else if (mCursorX == 2 && mCursorY == 4) {
                    if (isFishIconVisible()) {
                        setAButtonString(0x3EE);
                    }
                } else if (mCursorX == 3 && mCursorY == 3) {
                    if (isSkillIconVisible()) {
                        setAButtonString(0x3EE);
                    }
                } else if (mCursorX == 2 && mCursorY == 3) {
                    if (isInsectIconVisible()) {
                        setAButtonString(0x3EE);
                    }
                } else if (mCursorX < 3 && mCursorY < 3) {
                    setAButtonString(0x40C);
                } else {
                    setAButtonString(0);
                }
            } else {
                setAButtonString(0);
            }
        }
        setBButtonString(0x3F9);
    }
}


void dMenu_Collect2D_c::save_open_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (!mpSaveScrn) {
        mpSaveScrn = new dMenu_save_c();
    }
    mpSaveScrn->setUseType(1);
    mpSaveScrn->_create();
    mpSaveScrn->initialize();
    setAButtonString(0);
    setBButtonString(0);
    mpDrawCursor->offPlayAllAnime();
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::save_open_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSaveScrn->getSaveStatus() == 1) {
        mpSaveScrn->_open();
    }
    if (mpSaveScrn->getSaveStatus() == 2) {
        mProcess = 2;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::save_move_init() {
    setAButtonString(0x040c);
    setBButtonString(0);
}

void dMenu_Collect2D_c::save_move_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSaveScrn->getSaveStatus() == 2) {
        mpSaveScrn->_move();
    }
    if (mpSaveScrn->getSaveStatus() == 3) {
        mProcess = 3;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::save_close_init() {
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Collect2D_c::save_close_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSaveScrn->getSaveStatus() == 3) {
        mpSaveScrn->_close();
    }
    if (mpSaveScrn->getSaveStatus() == 0) {
        mProcess = 0;
        mpDrawCursor->onPlayAllAnime();
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::option_open_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (!mpOptionScrn) {
        mpOptionScrn = new dMenu_Option_c(dComIfGp_getCollectResArchive(), mpStick);
    }
    mpOptionScrn->initialize();
    setAButtonString(0);
    setBButtonString(0);
    mpDrawCursor->offPlayAllAnime();
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::option_open_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpOptionScrn->getQuitStatus() == 1) {
        mpOptionScrn->_open();
    }
    if (mpOptionScrn->getQuitStatus() == 2) {
        mProcess = 5;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::option_move_init() {
    setAButtonString(0x040c);
    setBButtonString(0x03f9);
}

void dMenu_Collect2D_c::option_move_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpOptionScrn->getQuitStatus() == 2) {
        mpOptionScrn->_move();
    }
    if (mpOptionScrn->getQuitStatus() == 3) {
        mProcess = 6;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::option_close_init() {
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Collect2D_c::option_close_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpOptionScrn->getQuitStatus() == 3) {
        mpOptionScrn->_close();
    }
    if (mpOptionScrn->getQuitStatus() == 0) {
        mProcess = 0;
        mpDrawCursor->onPlayAllAnime();
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::letter_open_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (!mpLetterScrn) {
        mpLetterScrn = new dMenu_Letter_c(mpSubHeap, mpStick, mpCStick);
    }
    setAButtonString(0);
    setBButtonString(0);
    mpDrawCursor->offPlayAllAnime();
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::letter_open_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpLetterScrn->getStatus() == 1) {
        mpLetterScrn->_open();
    }
    if (mpLetterScrn->getStatus() == 2) {
        mProcess = 8;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::letter_move_init() {
    setAButtonString(0x040c);
    setBButtonString(0x03f9);
}

void dMenu_Collect2D_c::letter_move_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpLetterScrn->getStatus() == 2) {
        mpLetterScrn->_move();
    }
    if (mpLetterScrn->getStatus() == 3) {
        mProcess = 9;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::letter_close_init() {
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Collect2D_c::letter_close_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpLetterScrn->getStatus() == 3) {
        mpLetterScrn->_close();
    }
    if (mpLetterScrn->getStatus() == 0) {
        mProcess = 0;
        mpDrawCursor->onPlayAllAnime();
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::fishing_open_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (!mpFishingScrn) {
        mpFishingScrn = new dMenu_Fishing_c(mpSubHeap, mpStick, mpCStick);
    }
    setAButtonString(0);
    setBButtonString(0);
    mpDrawCursor->offPlayAllAnime();
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::fishing_open_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpFishingScrn->getStatus() == 1) {
        mpFishingScrn->_open();
    }
    if (mpFishingScrn->getStatus() == 2) {
        mProcess = 11;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::fishing_move_init() {
    setAButtonString(0x040c);
    setBButtonString(0x03f9);
}

void dMenu_Collect2D_c::fishing_move_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpFishingScrn->getStatus() == 2) {
        mpFishingScrn->_move();
    }
    if (mpFishingScrn->getStatus() == 3) {
        mProcess = 12;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::fishing_close_init() {
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Collect2D_c::fishing_close_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpFishingScrn->getStatus() == 3) {
        mpFishingScrn->_close();
    }
    if (mpFishingScrn->getStatus() == 0) {
        mProcess = 0;
        mpDrawCursor->onPlayAllAnime();
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::skill_open_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (!mpSkillScrn) {
        mpSkillScrn = new dMenu_Skill_c(mpSubHeap, mpStick, mpCStick);
    }
    setAButtonString(0);
    setBButtonString(0);
    mpDrawCursor->offPlayAllAnime();
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::skill_open_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSkillScrn->getStatus() == 1) {
        mpSkillScrn->_open();
    }
    if (mpSkillScrn->getStatus() == 2) {
        mProcess = 14;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::skill_move_init() {
    setAButtonString(0x040c);
    setBButtonString(0x03f9);
}

void dMenu_Collect2D_c::skill_move_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSkillScrn->getStatus() == 2) {
        mpSkillScrn->_move();
    }
    if (mpSkillScrn->getStatus() == 3) {
        mProcess = 15;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::skill_close_init() {
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Collect2D_c::skill_close_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpSkillScrn->getStatus() == 3) {
        mpSkillScrn->_close();
    }
    if (mpSkillScrn->getStatus() == 0) {
        mProcess = 0;
        mpDrawCursor->onPlayAllAnime();
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::insect_open_init() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (!mpInsectScrn) {
        mpInsectScrn = new dMenu_Insect_c(mpSubHeap, mpStick, mpCStick, 0);
    }
    setAButtonString(0);
    setBButtonString(0);
    mpDrawCursor->offPlayAllAnime();
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::insect_open_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpInsectScrn->getStatus() == 1) {
        mpInsectScrn->_open();
    }
    if (mpInsectScrn->getStatus() == 2) {
        mProcess = 17;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::insect_move_init() {
    setAButtonString(0x040c);
    setBButtonString(0x03f9);
}

void dMenu_Collect2D_c::insect_move_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpInsectScrn->getStatus() == 2) {
        mpInsectScrn->_move();
    }
    if (mpInsectScrn->getStatus() == 3) {
        mProcess = 18;
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::insect_close_init() {
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Collect2D_c::insect_close_proc() {
    JKRHeap* heap = mDoExt_setCurrentHeap(mpSubHeap);
    if (mpInsectScrn->getStatus() == 3) {
        mpInsectScrn->_close();
    }
    if (mpInsectScrn->getStatus() == 0) {
        mProcess = 0;
        mpDrawCursor->onPlayAllAnime();
    }
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Collect2D_c::_move() {
    u8 last_process = mProcess;
    (this->*process[last_process])();
    if (mProcess != last_process) {
        (this->*init[mProcess])();
    }
    btkAnimeLoop0(mpAnmKey);
    mpScreen->animation();
    setBackAlpha();
    setHIO(false);
}


void dMenu_Collect2D_c::_draw() {
    J2DGrafContext* grafPort = dComIfGp_getCurrentGrafPort();
    grafPort->setup2D();
    mpScreen->draw(0.0f, 0.0f, grafPort);

    if (mItemNameString == 0) {
#if REGION_JPN
        char* stringPtr1 = static_cast<J2DTextBox*>(mpScreen->search('i_text1'))->getStringPtr();
#else
        char* stringPtr1 = static_cast<J2DTextBox*>(mpScreen->search('f_text1'))->getStringPtr();
#endif
        strcpy(stringPtr1, "");

#if REGION_JPN
        char* stringPtr0 = static_cast<J2DTextBox*>(mpScreen->search('i_text0'))->getStringPtr();
#else
        char* stringPtr0 = static_cast<J2DTextBox*>(mpScreen->search('f_text0'))->getStringPtr();
#endif
        strcpy(stringPtr0, "");
    } else {
#if REGION_JPN
        J2DTextBox* textBox1 = static_cast<J2DTextBox*>(mpScreen->search('i_text1'));
#else
        J2DTextBox* textBox1 = static_cast<J2DTextBox*>(mpScreen->search('f_text1'));
#endif
        mpString->getString(mItemNameString, textBox1, NULL, NULL, NULL, 0);

#if REGION_JPN
        J2DTextBox* textBox0 = static_cast<J2DTextBox*>(mpScreen->search('i_text0'));
#else
        J2DTextBox* textBox0 = static_cast<J2DTextBox*>(mpScreen->search('f_text0'));
#endif
        mpString->getString(mItemNameString, textBox0, NULL, NULL, NULL, 0);

#if REGION_JPN
        textBox0 = static_cast<J2DTextBox*>(mpScreen->search('i_text0'));
#else
        textBox0 = static_cast<J2DTextBox*>(mpScreen->search('f_text0'));
#endif
        mpString->drawOutFontLocal(textBox0, -1.0f);
    }
    mpDrawCursor->draw();
}

void dMenu_Collect2D_c::drawTop() {
    dComIfGd_set2DOpaTop(mpDraw2DTop);
}

bool dMenu_Collect2D_c::isKeyCheck() {
    if (mProcess || field_0x25f) {
        return true;
    }
    return false;
}

bool dMenu_Collect2D_c::isOutCheck() {
    return false;
}

void dMenu_Collect2D_c::setAButtonString(u16 i_stringID) {
    static const u64 text_a_tag[5] = {
        'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5',
    };

    if (i_stringID != mCurrentAString) {
        mCurrentAString = i_stringID;

        if (i_stringID == 0) {
            for (int i = 0; i < 5; i++) {
                char* stringPtr =
                    static_cast<J2DTextBox*>(mpScreenIcon->search(text_a_tag[i]))->getStringPtr();
                strcpy(stringPtr, "");
            }
        } else {
            for (int i = 0; i < 5; i++) {
                char* stringPtr =
                    static_cast<J2DTextBox*>(mpScreenIcon->search(text_a_tag[i]))->getStringPtr();
                dMeter2Info_getStringKanji(i_stringID, stringPtr, NULL);
            }
        }
    }
}

void dMenu_Collect2D_c::setBButtonString(u16 i_stringID) {
    static const u64 text_b_tag[5] = {
        'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5',
    };

    if (i_stringID != mCurrentBString) {
        mCurrentBString = i_stringID;

        if (i_stringID == 0) {
            for (int i = 0; i < 5; i++) {
                char* stringPtr =
                    static_cast<J2DTextBox*>(mpScreenIcon->search(text_b_tag[i]))->getStringPtr();
                strcpy(stringPtr, "");
            }
        } else {
            for (int i = 0; i < 5; i++) {
                char* stringPtr =
                    static_cast<J2DTextBox*>(mpScreenIcon->search(text_b_tag[i]))->getStringPtr();
                dMeter2Info_getStringKanji(i_stringID, stringPtr, NULL);
            }
        }
    }
}

void dMenu_Collect2D_c::setItemNameString(u8 param_0, u8 param_1) {
    if (field_0x22d[param_0][param_1] == 0) {
        setItemNameStringNull();
    } else {
        u32 uVar6 = field_0x184[param_0][param_1];
        mItemNameString = field_0x1d8[param_0][param_1];
        field_0x180 = uVar6;
        if (uVar6 == 0) {
            setItemNameStringNull();
        } else {
#if REGION_JPN
            char* stringPtr =
                static_cast<J2DTextBox*>(mpScreen->search('item_n00'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
            stringPtr = static_cast<J2DTextBox*>(mpScreen->search('item_n01'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
            stringPtr = static_cast<J2DTextBox*>(mpScreen->search('item_n02'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
            stringPtr = static_cast<J2DTextBox*>(mpScreen->search('item_n03'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
#else
            char* stringPtr =
                static_cast<J2DTextBox*>(mpScreen->search('item_n04'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
            stringPtr = static_cast<J2DTextBox*>(mpScreen->search('item_n05'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
            stringPtr = static_cast<J2DTextBox*>(mpScreen->search('item_n06'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
            stringPtr = static_cast<J2DTextBox*>(mpScreen->search('item_n07'))->getStringPtr();
            dMeter2Info_getStringKanji(uVar6, stringPtr, NULL);
#endif
        }
    }
}

void dMenu_Collect2D_c::setItemNameStringNull() {
    mItemNameString = 0;
#if REGION_JPN
    J2DTextBox* textBox = (J2DTextBox*)mpScreen->search('item_n00');
    strcpy(textBox->getStringPtr(), "");
    textBox = (J2DTextBox*)mpScreen->search('item_n01');
    strcpy(textBox->getStringPtr(), "");
    textBox = (J2DTextBox*)mpScreen->search('item_n02');
    strcpy(textBox->getStringPtr(), "");
    textBox = (J2DTextBox*)mpScreen->search('item_n03');
#else
    J2DTextBox* textBox = (J2DTextBox*)mpScreen->search('item_n04');
    strcpy(textBox->getStringPtr(), "");
    textBox = (J2DTextBox*)mpScreen->search('item_n05');
    strcpy(textBox->getStringPtr(), "");
    textBox = (J2DTextBox*)mpScreen->search('item_n06');
    strcpy(textBox->getStringPtr(), "");
    textBox = (J2DTextBox*)mpScreen->search('item_n07');
#endif
    strcpy(textBox->getStringPtr(), "");
}

dMenu_Collect3D_c::dMenu_Collect3D_c(JKRExpHeap* param_0, dMenu_Collect2D_c* param_1,
                                     CSTControl* param_2) {
    mIsWolf = daPy_py_c::checkNowWolf();
    mpHeap = param_0;
    mpSolidHeap = NULL;
    mpCStick = param_2;
    mpCollect2D = param_1;
    if (mIsWolf) {
        mLinkAngle = -20748;
    } else {
        mLinkAngle = -23324;
    }
    mMaskMirrorAnmFrameBrk = 0.0f;
    mMaskMirrorAnmFrameBck = 0.0f;
    mMaskMirrorOffsetY = 0.0f;
    mMaskMirrorOffsetX = 0.0f;
    mMaskMirrorScale = 1.0f;
}


dMenu_Collect3D_c::~dMenu_Collect3D_c() {
    if (mpSolidHeap) {
        mDoExt_destroySolidHeap(mpSolidHeap);
        mpSolidHeap = NULL;
    }
}


void dMenu_Collect3D_c::_create() {
    mpHeap->getTotalFreeSize();
    mpSolidHeap = mDoExt_createSolidHeapToCurrent(0x25800, mpHeap, 0x20);
    mDoExt_setCurrentHeap((JKRHeap*)mpSolidHeap);
    daAlink_c* linkActor = daAlink_getAlinkActorClass();
    if (linkActor != NULL) {
        linkActor->initStatusWindow();
    }
    u8 maskMdl = getMaskMdlVisible();
    if (maskMdl == 2) {
        createMirrorModel();
    } else if (maskMdl == 1) {
        createMaskModel();
    } else {
        mpModel = NULL;
        mpBckAnm = NULL;
        mpBrkAnm = NULL;
    }
    mpSolidHeap->adjustSize();
    mDoExt_restoreCurrentHeap();
    if (mpModel != NULL) {
        dKy_tevstr_init(&mTevStr, -1, 0xff);
        set_mtx();
    }
    _move(mpCollect2D->getCursorX(), mpCollect2D->getCursorY());
}

void dMenu_Collect3D_c::_delete() {
    if (daAlink_getAlinkActorClass()) {
        daAlink_getAlinkActorClass()->resetStatusWindow();
    }
}

void dMenu_Collect3D_c::_move(u8 param_0, u8 param_1) {
    cXyz itemPos;
    Vec linkPos = mpCollect2D->getpLink()->getGlobalVtxCenter(false, 0);
    f32 posY;
    f32 posZ;
    if (mIsWolf != 0) {
        posY = linkPos.y + 60.0f;
        posZ = 580.0f;
    } else {
        posY = linkPos.y + 90.0f;
        posZ = 550.0f;
    }
    toItem3Dpos(linkPos.x, posY, posZ, &itemPos);
    if (param_0 == 0 && param_1 == 0) {
        f32 temp = 450.0f;
        mLinkAngle += (s16)temp;
    } else {
        s16 target = mIsWolf != 0 ? (s16)-0x510C : (s16)-0x5B1C;
        cLib_addCalcAngleS(&mLinkAngle, target, 4, 0x800, 0x80);
    }
    if (daAlink_getAlinkActorClass() != NULL) {
        daAlink_getAlinkActorClass()->statusWindowExecute(&itemPos, mLinkAngle);
    }
    if (mpModel != NULL) {
        cXyz modelPos;
        Vec maskPos = mpCollect2D->getpMask()->getGlobalVtxCenter(false, 0);
        f32 offsetX = 0.0f;
        f32 offsetY = 0.0f;

        if (getMaskMdlVisible() == 2) {
            u8 mirrorNum = getMirrorNum();
            if (mirrorNum == 1 || mirrorNum == 2 || mirrorNum == 3 || mirrorNum == 4) {
                offsetX = -3.0f;
                offsetY = -5.0f;
            }
        } else {
            u8 crystalNum = getCrystalNum();
            if (crystalNum == 1) {
                offsetX = 16.0f;
                offsetY = 0.0f;
            } else if (crystalNum == 2) {
                offsetX = 8.0f;
                offsetY = -3.0f;
            } else if (crystalNum == 3) {
                offsetX = 2.0f;
                offsetY = -4.0f;
            } else if (crystalNum == 4) {
                offsetX = -5.0f;
                offsetY = -10.0f;
            }
        }
        f32 offsetPosX =
            maskPos.x + g_drawHIO.mCollectScreen.mMaskMirrorPos.x + mMaskMirrorOffsetX + offsetX;
        f32 offsetPosY =
            maskPos.y + g_drawHIO.mCollectScreen.mMaskMirrorPos.y + mMaskMirrorOffsetY + offsetY;
        toItem3Dpos(offsetPosX, offsetPosY, 600.0f, &modelPos);
        mMaskMirrorPos.set(modelPos);
        mMaskMirrorAngle.set(g_drawHIO.mCollectScreen.mMaskMirrorAngle.x,
                        g_drawHIO.mCollectScreen.mMaskMirrorAngle.y,
                        g_drawHIO.mCollectScreen.mMaskMirrorAngle.z);
        animePlay();
        set_mtx();
    }
}

void dMenu_Collect3D_c::draw() {
    dComIfGd_setListItem3D();
    if (daAlink_getAlinkActorClass()) {
        daAlink_getAlinkActorClass()->statusWindowDraw();
    }
    if (mpModel) {
        g_env_light.settingTevStruct(13, &mMaskMirrorPos, &mTevStr);
        g_env_light.setLightTevColorType_MAJI(mpModel, &mTevStr);
        animeEntry();
        mDoExt_modelUpdateDL(mpModel);
    }
    dComIfGd_setList();
}

void dMenu_Collect3D_c::setJ3D(const char* param_0, const char* param_1, const char* param_2) {
    JKRArchive* resArchive = dComIfGp_getCollectResArchive();
    J3DModelData* modelData = J3DModelLoaderDataBase::load(
        resArchive->getResource('BMD ', param_0), 0x51020010);

    for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
        J3DMaterialAnm* matAnm = new J3DMaterialAnm();
        modelData->getMaterialNodePointer(i)->change();
        modelData->getMaterialNodePointer(i)->setMaterialAnm(matAnm);
    }
    mpModel = new J3DModel(modelData, 0, 1);
    if (param_1 != NULL) {
        J3DAnmTransform* pbck = (J3DAnmTransform*)J3DAnmLoaderDataBase::load(
            resArchive->getResource('BCK ', param_1), J3DLOADER_UNK_FLAG0);
        mpBckAnm = new mDoExt_bckAnm();
        if (mpBckAnm == NULL || !mpBckAnm->init(pbck, 1, 2, 1.0f, 0, -1, false)) {
            return;
        }
    }
    if (param_2 != NULL) {
        J3DAnmTevRegKey* pbrk = (J3DAnmTevRegKey*)J3DAnmLoaderDataBase::load(
            resArchive->getResource('BRK ', param_2), J3DLOADER_UNK_FLAG0);
        pbrk->searchUpdateMaterialID(modelData);
        mpBrkAnm = new mDoExt_brkAnm();
        if (mpBrkAnm == NULL || !mpBrkAnm->init(modelData, pbrk, -1, 2, 1.0f, 0, -1)) {
            return;
        }
    }
}

void dMenu_Collect3D_c::set_mtx() {
    f32 scale = 0.0f;
    if (getMaskMdlVisible() == 2) {
        u8 mirrorNum = getMirrorNum();
        if (mirrorNum == 2 || mirrorNum == 3 || mirrorNum == 4) {
            scale = -0.1f;
        }
    } else {
        u8 crystalNum = getCrystalNum();
        if (crystalNum == 1 || crystalNum == 2) {
            scale = -0.6f;
        } else if (crystalNum == 3) {
            scale = -0.4f;
        } else if (crystalNum == 4) {
            scale = -0.3f;
        }
    }
    f32 finalScale = g_drawHIO.mCollectScreen.mMaskMirrorScale + scale + mMaskMirrorScale;
    cXyz baseScale;
    baseScale.z = finalScale;
    baseScale.y = finalScale;
    baseScale.x = finalScale;
    mpModel->setBaseScale(baseScale);
    mDoMtx_stack_c::transS(mMaskMirrorPos.x, mMaskMirrorPos.y, mMaskMirrorPos.z);
    mDoMtx_stack_c::YrotM(mMaskMirrorAngle.y);
    mDoMtx_stack_c::XrotM(mMaskMirrorAngle.x);
    mDoMtx_stack_c::ZrotM(mMaskMirrorAngle.z);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}


void dMenu_Collect3D_c::animePlay() {
    if (mpBrkAnm != NULL) {
        mMaskMirrorAnmFrameBrk += g_drawHIO.mCollectScreen.mMaskMirrorAnimSpeed;
        if (mMaskMirrorAnmFrameBrk >= mpBrkAnm->getEndFrame()) {
            mMaskMirrorAnmFrameBrk -= mpBrkAnm->getEndFrame();
        }
        mpBrkAnm->setFrame(mMaskMirrorAnmFrameBrk);
        mpBrkAnm->play();
    }
    if (mpBckAnm != NULL) {
        mMaskMirrorAnmFrameBck += g_drawHIO.mCollectScreen.mMaskMirrorAnimSpeed;
        if (mMaskMirrorAnmFrameBck >= mpBckAnm->getEndFrame()) {
            mMaskMirrorAnmFrameBck -= mpBckAnm->getEndFrame();
        }
        mpBckAnm->setFrame(mMaskMirrorAnmFrameBck);
        mpBckAnm->play();
    }
}


void dMenu_Collect3D_c::animeEntry() {
    if (mpBrkAnm != NULL) {
        mpBrkAnm->entry(mpModel->getModelData());
    }
    if (mpBckAnm != NULL) {
        mpBckAnm->entry(mpModel->getModelData());
    }
}

void dMenu_Collect3D_c::createMaskModel() {
    static const f32 m_kamen_offset_x[5] = {-14.0f, -14.0f, -14.0f, 1.3f, 6.5f};
    static const f32 m_kamen_offset_y[5] = {0.0f, 0.0f, 0.0f, 22.0f, 30.0f};
    static const f32 m_kamen_scale[5] = {2.6f, 2.6f, 2.2f, 1.8f, 1.8f};
    static char* bck_name[4] = {
        "md_mask_parts_spin_1.bck",
        "md_mask_parts_spin_2.bck",
        "md_mask_parts_spin_3.bck",
        "md_mask_parts_spin_4.bck",
    };
    static char* brk_name[4] = {
        "md_mask_parts_spin_1.brk",
        "md_mask_parts_spin_2_3.brk",
        "md_mask_parts_spin_2_3.brk",
        "md_mask_parts_spin_4.brk",
    };

    u8 crystalNum = getCrystalNum();
    mMaskMirrorOffsetX = m_kamen_offset_x[crystalNum];
    mMaskMirrorOffsetY = m_kamen_offset_y[crystalNum];
    mMaskMirrorScale = m_kamen_scale[crystalNum];
    mMaskMirrorPos.set(0.0f, 0.0f, 0.0f);
    mMaskMirrorAngle.set(0, 0, 0);
    mpModel = NULL;
    mpBckAnm = NULL;
    mpBrkAnm = NULL;
    if (crystalNum != 0) {
        setJ3D("md_mask_UI.bmd", bck_name[crystalNum - 1], brk_name[crystalNum - 1]);
        switch (crystalNum) {
        case 1:
            mpModel->getModelData()->getMaterialNodePointer(0)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(1)->getShape()->hide();
        case 2:
            mpModel->getModelData()->getMaterialNodePointer(2)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(3)->getShape()->hide();
        case 3:
            mpModel->getModelData()->getMaterialNodePointer(6)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(7)->getShape()->hide();
            break;
        }
    }
}

void dMenu_Collect3D_c::createMirrorModel() {
    static const f32 m_mirror_offset_x[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    static const f32 m_mirror_offset_y[5] = {4.0f, 4.0f, 4.0f, 4.0f, 4.0f};
    static const f32 m_mirror_scale[5] = {0.6f, 0.6f, 0.6f, 0.6f, 0.6f};
    static char* bck_name[4] = {
        "kageri_mirrer_spin_1.bck",
        "kageri_mirrer_spin_2.bck",
        "kageri_mirrer_spin_3.bck",
        "kageri_mirrer_spin_4.bck",
    };
    static char* brk_name[4] = {
        "kageri_mirrer_spin_1.brk",
        "kageri_mirrer_spin_2_3_4.brk",
        "kageri_mirrer_spin_2_3_4.brk",
        "kageri_mirrer_spin_2_3_4.brk",
    };

    u8 mirrorNum = getMirrorNum();
    mMaskMirrorOffsetX = m_mirror_offset_x[mirrorNum];
    mMaskMirrorOffsetY = m_mirror_offset_y[mirrorNum];
    mMaskMirrorScale = m_mirror_scale[mirrorNum];
    mMaskMirrorPos.set(0.0f, 0.0f, 0.0f);
    mMaskMirrorAngle.set(0, 0, 0);
    mpModel = NULL;
    mpBckAnm = NULL;
    mpBrkAnm = NULL;
    if (mirrorNum != 0) {
        setJ3D("kageri_mirrer_UI.bmd", bck_name[mirrorNum - 1], brk_name[mirrorNum - 1]);
        switch (mirrorNum) {
        case 1:
            mpModel->getModelData()->getMaterialNodePointer(4)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(5)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(6)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(7)->getShape()->hide();
        case 2:
            mpModel->getModelData()->getMaterialNodePointer(8)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(9)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(10)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(11)->getShape()->hide();
        case 3:
            mpModel->getModelData()->getMaterialNodePointer(12)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(13)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(14)->getShape()->hide();
            mpModel->getModelData()->getMaterialNodePointer(15)->getShape()->hide();
            break;
        }
    }
}

u8 dMenu_Collect3D_c::getCrystalNum() {
    u8 count = 0;
    for (int i = 0; i < 4; i++) {
        if (!dComIfGs_isCollectCrystal(i)) {
            break;
        }
        count++;
    }
    return count;
}

u8 dMenu_Collect3D_c::getMirrorNum() {
    u8 count = 0;
    for (int i = 0; i < 4; i++) {
        if (!dComIfGs_isCollectMirror(i)) {
            break;
        }
        count++;
    }
    return count;
}

u8 dMenu_Collect3D_c::getMaskMdlVisible() {
    if ((getCrystalNum() == 0 && getMirrorNum() == 0) ||
         /* dSv_event_flag_c::M_071 - Cutscene - [cutscene: 20] Zant appears (during Midna's desperate hour) */
        (dComIfGs_isEventBit(0xC01) && !dComIfGs_isCollectMirror(0)) ||
         /* dSv_event_flag_c::F_0354 - Cutscene - [cutscene] Mirror complete */
        (dComIfGs_isEventBit(0x2B08) && !dComIfGs_isCollectCrystal(3)))
    {
        return 0;
    }
    if (dComIfGs_isCollectCrystal(3) || getMirrorNum() == 0) {
        return 1;
    }
    return 2;
}

f32 dMenu_Collect3D_c::mViewOffsetY = -100.0f;

void dMenu_Collect3D_c::setupItem3D(Mtx param_0) {
    GXSetViewport(0.0f, mViewOffsetY, FB_WIDTH, FB_HEIGHT, 0.0f, 1.0f);
    mViewOffsetY = -100.0f;
    Mtx44 projection;
    C_MTXPerspective(projection, 45.0f, mDoGph_gInf_c::getAspect(), 1.0f, 100000.0f);
    GXSetProjection(projection, GX_PERSPECTIVE);
    calcViewMtx(param_0);
}

#pragma push
#pragma optimization_level 2
void dMenu_Collect3D_c::toItem3Dpos(f32 param_0, f32 param_1, f32 param_2, cXyz* param_3) {
    Mtx adStack_98;
    Mtx auStack_c8;
    param_0 =
        (2.0f * ((param_0 - mDoGph_gInf_c::getMinXF()) / mDoGph_gInf_c::getWidthF()) - 1.0f);
    param_1 = (2.0f * ((param_1 - -100.0f) / 448.0f) - 1.0f);
    calcViewMtx(adStack_98);
    MTXInverse(adStack_98, auStack_c8);
    f32 tangent = tan(0.39269909262657166);
    f32 dVar12 = -param_2;
    cXyz cStack_d4((param_0 * param_2) * (mDoGph_gInf_c::getAspect() * tangent),
                   (tangent * (param_1 * dVar12)), dVar12);
    MTXMultVec(auStack_c8, &cStack_d4, param_3);
}
#pragma pop

void dMenu_Collect3D_c::calcViewMtx(Mtx param_0) {
    cXyz pos1(0.0f, 0.0f, -1000.0f);
    cXyz pos2(0.0f, 1.0f, 0.0f);
    mDoMtx_lookAt(param_0, &pos1, &cXyz::Zero, &pos2, 0);
}


void dMenu_Collect2DTop_c::draw() {
    J2DGrafContext* graf_port = dComIfGp_getCurrentGrafPort();
    graf_port->setup2D();
    mpCollect2D->getBlackTex()->draw(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                                     mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(),
                                     false, false, false);
    if (mpCollect2D->getSaveScreen() != NULL) {
        mpCollect2D->getSaveScreen()->_draw();
    }
    if (mpCollect2D->getOptionScreen() != NULL) {
        mpCollect2D->getOptionScreen()->_draw();
    }
    if (mpCollect2D->getLetterScreen() != NULL) {
        mpCollect2D->getLetterScreen()->_draw();
    }
    if (mpCollect2D->getFishingScreen() != NULL) {
        mpCollect2D->getFishingScreen()->_draw();
    }
    if (mpCollect2D->getSkillScreen() != NULL) {
        mpCollect2D->getSkillScreen()->_draw();
    }
    if (mpCollect2D->getInsectScreen() != NULL) {
        mpCollect2D->getInsectScreen()->_draw();
    }
    if (mpCollect2D->getIconScreen() != NULL) {
        mpCollect2D->getIconScreen()->draw(0.0f, 0.0f, graf_port);
    }
}

dMenu_Collect_c::dMenu_Collect_c(JKRExpHeap* param_0, STControl* param_1, CSTControl* param_2) {
    mpCollect2D = new dMenu_Collect2D_c(param_0, param_1, param_2);
    mpCollect3D = new dMenu_Collect3D_c(param_0, mpCollect2D, param_2);
}

dMenu_Collect_c::~dMenu_Collect_c() {
    if (mpCollect2D) {
        delete mpCollect2D;
        mpCollect2D = NULL;
    }
    if (mpCollect3D) {
        delete mpCollect3D;
        mpCollect3D = NULL;
    }
}


void dMenu_Collect_c::_create() {
    mpCollect2D->_create();
    mpCollect3D->_create();
}

void dMenu_Collect_c::_delete() {
    mpCollect2D->_delete();
    mpCollect3D->_delete();
}

void dMenu_Collect_c::_move() {
    mpCollect2D->_move();
    mpCollect3D->_move(mpCollect2D->getCursorX(), mpCollect2D->getCursorY());
}

void dMenu_Collect_c::draw() {
    dComIfGd_set2DOpa(mpCollect2D);
    mpCollect3D->draw();
    mpCollect2D->drawTop();
}
