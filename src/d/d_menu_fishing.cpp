/**
 * d_menu_fishing.cpp
 * Menu - Fishing Journal
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_fishing.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "d/d_menu_dmap.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

typedef void (dMenu_Fishing_c::*initFunc)();
initFunc map_init_process[] = {
    &dMenu_Fishing_c::wait_init,
};

typedef void (dMenu_Fishing_c::*moveFunc)();
moveFunc map_move_process[] = {
    &dMenu_Fishing_c::wait_move,
};

dMenu_Fishing_c::dMenu_Fishing_c(JKRExpHeap* i_heap, STControl* i_stick, CSTControl* i_cStick) {
    mpHeap = i_heap;
    mpArchive = 0;
    mpMount = 0;
    mpStick = i_stick;
    mpCStick = i_cStick;
    mStatus = READY_OPEN;
    mProcess = 0;
    mFishListScreenFrames = 0;
}

dMenu_Fishing_c::~dMenu_Fishing_c() {
    delete mpString;
    mpString = NULL;

    delete mpBlackTex;
    mpBlackTex = NULL;

    delete mpScreen;
    mpScreen = NULL;

    delete mpParent;
    mpParent = NULL;

    for (int i = 0; i < MAX_FINDABLE_FISHES; i++) {
        delete mpFishParent[i];
        mpFishParent[i] = NULL;

        for (int j = 0; j < MAX_FINDABLE_FISHES; j++) {
            delete mpFishParts[j][i];
            mpFishParts[j][i] = NULL;
        }
    }

    for (int i = 0; i < 2; i++) {
        delete mpFishInfoParent[i];
        mpFishInfoParent[i] = NULL;
    }

    delete mpIconScreen;
    mpIconScreen = NULL;

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

    if (mpMount) {
        mpMount->getArchive()->unmount();
        mpMount->destroy();
        mpMount = NULL;
    }

    if (mpArchive) {
        mpArchive->unmount();
        mpArchive = NULL;
    }
}

void dMenu_Fishing_c::_create() {
    mpString = new dMsgString_c();
    screenSetBase();
    screenSetDoIcon();
    setHIO(true);
    init();
}

void dMenu_Fishing_c::_move() {
    JKRHeap* heap = mDoExt_setCurrentHeap((JKRHeap*)mpHeap);
    u8 old_process = mProcess;
    (this->*map_move_process[mProcess])();
    if (old_process != mProcess) {
        (this->*map_init_process[mProcess])();
    }
    setHIO(false);
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Fishing_c::_draw() {
    if (mpArchive) {
        J2DGrafContext* grafPort = dComIfGp_getCurrentGrafPort();
        mpBlackTex->setAlpha(0xff);
        mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
        mpScreen->draw(0.0f, 0.0f, grafPort);
        mpIconScreen->draw(0.0f, 0.0f, grafPort);
    }
}

bool dMenu_Fishing_c::isSync() {
    if (mpMount != NULL && !mpMount->sync()) {
        return false;
    }
    return true;
}

void dMenu_Fishing_c::init() {
    #if VERSION == VERSION_GCN_PAL
    BOOL isEnglish = FALSE;
    if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGAUGE_ENGLISH) {
        isEnglish = TRUE;
    }
    #endif

    for (int i = 0; i < MAX_FINDABLE_FISHES; i++) {
        if (dComIfGs_getFishNum(i) != 0) {
            // Fish has been caught once, display it along with it's params
            #if VERSION == VERSION_GCN_PAL
            if (isEnglish) {
                setFishParam(i, dComIfGs_getFishNum(i), dComIfGs_getFishSize(i) / 2.54f);
            } else {
                setFishParam(i, dComIfGs_getFishNum(i), dComIfGs_getFishSize(i));
            }
            #else
            setFishParam(i, dComIfGs_getFishNum(i), dComIfGs_getFishSize(i));
            #endif
            mpFishParent[i]->show();
        } else {
            // Fish hasn't been caught yet, don't display it and also hide params
            mpFishParent[i]->hide();
        }
    }
    (this->*map_init_process[mProcess])();
}

int dMenu_Fishing_c::_open() {
    if (mpMount == NULL) {
        mpMount = mDoDvdThd_mountArchive_c::create("/res/Layout/fishres.arc", 0, NULL);
    }
    if (mpArchive == NULL) {
        if (mpMount->sync()) {
            if (mpArchive == NULL) {
                mpArchive = (JKRArchive*)mpMount->getArchive();
                delete mpMount;
                mpMount = NULL;
                _create();
            }
        } else {
            return 0;
        }
    }
    s16 openFrames = g_drawHIO.mFishListScreen.mOpenFrames;
    s16 closeFrames = g_drawHIO.mFishListScreen.mCloseFrames;
    mFishListScreenFrames = g_drawHIO.mFishListScreen.mOpenFrames;
    if (mFishListScreenFrames >= openFrames) {
        mFishListScreenFrames = closeFrames;
        mStatus = READY_MOVE;
        mpParent->scale(1.0f, 1.0f);
        mpParent->setAlphaRate(1.0f);
        return 1;
    } else {
        f32 div = mFishListScreenFrames / (f32)openFrames;
        mpParent->scale(div, div);
        mpParent->setAlphaRate(div);
        return 0;
    }
}

int dMenu_Fishing_c::_close() {
    s16 closeFrames = g_drawHIO.mFishListScreen.mCloseFrames;
    mFishListScreenFrames = 0;
    if (mFishListScreenFrames <= 0) {
        mFishListScreenFrames = 0;
        mStatus = CLOSED;
        mpParent->scale(0.0f, 0.0f);
        mpParent->setAlphaRate(0.0f);
        return 1;
    } else {
        f32 div = mFishListScreenFrames / (f32)closeFrames;
        mpParent->scale(div, div);
        mpParent->setAlphaRate(div);
        return 0;
    }
}

// While the fishing menu is open, 
void dMenu_Fishing_c::wait_init() {
    setAButtonString(0);
    setBButtonString(0x3F9); // "Back"
}

void dMenu_Fishing_c::wait_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 1 && mDoCPd_c::getTrigB(PAD_1)) {
        mStatus = READY_CLOSE;
    }
}

void dMenu_Fishing_c::screenSetBase() {
    static const u64 fish_n[6] = {
        'fish_n_6', 'fish_n_5', 'fish_n_3', 'fish_n_1', 'fish_n_2', 'fish_n_4',
    };

    static const u64 fish_p0[6] = {
        'fi_pa_6n', 'fi_pa_5n', 'fi_pa_3n', 'fi_pa_1n', 'fi_pa_2n', 'fi_pa_4n',
    };

    static const u64 fish_p1[6] = {
        'fi_na_6n', 'fi_na_5n', 'fi_na_3n', 'fi_na_1n', 'fi_na_2n', 'fi_na_4n',
    };

    static const u64 fish_p2[6] = {'fi_li_6n', 'fi_li_5n', 'fi_li_3n',
                                   'fi_li_1n', 'fi_li_2n', 'fi_li_4n'};

    static const u64 fish_p3[6] = {
        'b_box_6n', 'b_box_5n', 'b_box_3n', 'b_box_1n', 'b_box_2n', 'b_box_4n',
    };

    static const u64 fish_p4[6] = {
        'r_box_6n', 'r_box_5n', 'r_box_3n', 'r_box_1n', 'r_box_2n', 'r_box_4n',
    };

    static const u64 fish_p5[6] = {
        'info_6_n', 'info_5_n', 'info_3_n', 'info_1_n', 'info_2_n', 'info_4_n',
    };

    static const u64 size_1[6] = {
        'size_t_6', 'size_t_5', 'size_t_3', 'size_t_1', 'size_t_2', 'size_t_4',
    };

    static const u64 size_unit_1[6] = {
        'cm_t_6', 'cm_t_5', 'cm_t_3', 'cm_t_1', 'cm_t_2', 'cm_t_4',
    };

    static const u64 count_1[6] = {
        'count_t6', 'count_t5', 'count_t3', 'count_t1', 'count_t2', 'count_t4',
    };

    static const u64 count_unit_1[6] = {
        'cou_t_6', 'cou_t_5', 'cou_t_3', 'cou_t_1', 'cou_t_2', 'cou_t_4',
    };

    static const u64 name_0[6] = {
        'name_6', 'name_5', 'name_3', 'name_1', 'name_2', 'name_4',
    };

    static const u64 fname_0[6] = {
        'f_name_6', 'f_name_5', 'f_name_3', 'f_name_1', 'f_name_2', 'f_name_4',
    };

    static const u32 name_id[6] = {
        0x59E, 0x59D, 0x59B, 0x599, 0x59A, 0x59C,
    };

    ResTIMG* TIMG = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(TIMG);

    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xFF));
    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_fish_window.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpScreen);
    mpParent = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
    mpParent->setAlphaRate(0.0f);
    for (int i = 0; i < MAX_FINDABLE_FISHES; i++) {
        mpFishParent[i] = new CPaneMgr(mpScreen, fish_n[i], 0, NULL);
        mpFishParts[0][i] = new CPaneMgr(mpScreen, fish_p0[i], 0, NULL);
        mpFishParts[1][i] = new CPaneMgr(mpScreen, fish_p1[i], 0, NULL);
        mpFishParts[2][i] = new CPaneMgr(mpScreen, fish_p2[i], 0, NULL);
        mpFishParts[3][i] = new CPaneMgr(mpScreen, fish_p3[i], 0, NULL);
        mpFishParts[4][i] = new CPaneMgr(mpScreen, fish_p4[i], 0, NULL);
        mpFishParts[5][i] = new CPaneMgr(mpScreen, fish_p5[i], 0, NULL);
    }
    mpFishInfoParent[0] = new CPaneMgr(mpScreen, 'info_blu', 0, NULL);
    mpFishInfoParent[1] = new CPaneMgr(mpScreen, 'info_red', 0, NULL);

    #if (VERSION == VERSION_GCN_JPN) || (VERSION == VERSION_WII_JPN)
    J2DTextBox* textBox = (J2DTextBox*)mpScreen->search('t_t00');
    mpScreen->search('f_t00')->hide();
    #else
    J2DTextBox* textBox = (J2DTextBox*)mpScreen->search('f_t00');
    mpScreen->search('t_t00')->hide();
    #endif

    textBox->setFont(mDoExt_getSubFont());
    textBox->setString(0x200, "");
    mpString->getString(0x5a1, textBox, NULL, NULL, NULL, 0);
    for (int i = 0; i < MAX_FINDABLE_FISHES; i++) {
        field_0x10c[1][i] = (J2DTextBox*)mpScreen->search(size_1[i]);
        field_0x10c[1][i]->setFont(mDoExt_getSubFont());
        field_0x10c[1][i]->setString(0x20, "");

        field_0x16c[1][i] = (J2DTextBox*)mpScreen->search(size_unit_1[i]);
        field_0x16c[1][i]->setFont(mDoExt_getSubFont());
        field_0x16c[1][i]->setString(0x20, "");

        field_0x13c[1][i] = (J2DTextBox*)mpScreen->search(count_1[i]);
        field_0x13c[1][i]->setFont(mDoExt_getSubFont());
        field_0x13c[1][i]->setString(0x20, "");

        field_0x19c[1][i] = (J2DTextBox*)mpScreen->search(count_unit_1[i]);
        field_0x19c[1][i]->setFont(mDoExt_getSubFont());
        field_0x19c[1][i]->setString(0x20, "");

        mpFishNameString[i] = (J2DTextBox*)mpScreen->search(name_0[i]);
        mpScreen->search(fname_0[i])->hide();
        mpFishNameString[i]->setFont(mDoExt_getSubFont());
        mpFishNameString[i]->setString(0x20, "");
        dMeter2Info_getStringKanji(name_id[i], mpFishNameString[i]->getStringPtr(), NULL);
    }
    mpInfoLargestString = (J2DTextBox*)mpScreen->search('inf_size');
    mpInfoNumCaughtString = (J2DTextBox*)mpScreen->search('inf_cou');
    mpInfoLargestString->setFont(mDoExt_getMesgFont());
    mpInfoNumCaughtString->setFont(mDoExt_getMesgFont());
    mpInfoLargestString->setString(0x20, "");
    dMeter2Info_getStringKanji(0x59f, mpInfoLargestString->getStringPtr(), NULL); // "Largest"
    mpInfoNumCaughtString->setString(0x20, "");
    dMeter2Info_getStringKanji(0x5a0, mpInfoNumCaughtString->getStringPtr(), NULL); // "No. Caught"
}

void dMenu_Fishing_c::screenSetDoIcon() {
    static const u64 text_a_tag[5] = {'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5'};
    static const u64 text_b_tag[5] = {'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5'};

    mpIconScreen = new J2DScreen();
    mpIconScreen->setPriority("zelda_collect_soubi_do_icon_parts.blo", 0x20000, mpArchive);
    for (int i = 0; i < 2; i++) {
        mpButtonAB[i] = NULL;
        mpButtonText[i] = NULL;
    }
    dPaneClass_showNullPane(mpIconScreen);
    for (int i = 0; i < 5; i++) {
        mpAButtonString[i] = (J2DTextBox*)mpIconScreen->search(text_a_tag[i]);
        mpBButtonString[i] = (J2DTextBox*)mpIconScreen->search(text_b_tag[i]);
        mpAButtonString[i]->setFont(mDoExt_getMesgFont());
        mpBButtonString[i]->setFont(mDoExt_getMesgFont());
        mpAButtonString[i]->setString(0x20, "");
        mpBButtonString[i]->setString(0x20, "");
    }
}

void dMenu_Fishing_c::setAButtonString(u16 i_stringID) {
    if (i_stringID == 0) {
        for (int i = 0; i < 5; i++) {
            strcpy(mpAButtonString[i]->getStringPtr(), "");
        }
    } else {
        for (int i = 0; i < 5; i++) {
            dMeter2Info_getStringKanji(i_stringID, mpAButtonString[i]->getStringPtr(), NULL);
        }
    }
}

void dMenu_Fishing_c::setBButtonString(u16 i_stringID) {
    if (i_stringID == 0) {
        for (int i = 0; i < 5; i++) {
            strcpy(mpBButtonString[i]->getStringPtr(), "");
        }
    } else {
        for (int i = 0; i < 5; i++) {
            dMeter2Info_getStringKanji(i_stringID, mpBButtonString[i]->getStringPtr(), NULL);
        }
    }
}

u8 dMenu_Fishing_c::getFigure(int param_0) {
    if (param_0 < 0) {
        param_0 *= -1;
    }
    if (1000 <= param_0) {
        param_0 = 999;
    }
    if (100 <= param_0) {
        return 3;
    }
    if (param_0 >= 10) {
        return 2;
    }
    return 1;
}

void dMenu_Fishing_c::setFishParam(int i_fishIdx, u16 i_fishCount, u8 i_fishSize) {
    u8 fishCountFigure = getFigure(i_fishCount);
    u8 fishSizeFigure = getFigure(i_fishSize);
    char strBuff1[32];
    char strBuff2[32];
    for (int i = 1; i < 2; i++) {
        // part one, i_fishSize
        dComIfGp_setMessageCountNumber(i_fishSize);
        mpString->getString(0x597, field_0x10c[i][i_fishIdx], NULL, NULL, NULL, 0); // "inches"
        char* stringPtr = field_0x10c[i][i_fishIdx]->getStringPtr();
        strcpy(strBuff1, stringPtr);
        int j;
        for (j = 0; strBuff1[j + fishSizeFigure] != 0; j++) {
            strBuff2[j] = strBuff1[j + fishSizeFigure];
        }
        strBuff2[j] = 0;
        strBuff1[fishSizeFigure] = 0;
        strcpy(field_0x10c[i][i_fishIdx]->getStringPtr(), strBuff1);
        strcpy(field_0x16c[i][i_fishIdx]->getStringPtr(), strBuff2);

        // part two, i_fishCount
        dComIfGp_setMessageCountNumber(i_fishCount);
        mpString->getString(0x598, field_0x13c[i][i_fishIdx], NULL, NULL, NULL, 0); // "fish"
        stringPtr = field_0x13c[i][i_fishIdx]->getStringPtr();
        strcpy(strBuff1, stringPtr);
        int k;
        for (k = 0; strBuff1[k + fishCountFigure] != 0; k++) {
            strBuff2[k] = strBuff1[k + fishCountFigure];
        }
        strBuff2[k] = 0;
        strBuff1[fishCountFigure] = 0;
        strcpy(field_0x13c[i][i_fishIdx]->getStringPtr(), strBuff1);
        strcpy(field_0x19c[i][i_fishIdx]->getStringPtr(), strBuff2);
    }
}

void dMenu_Fishing_c::setHIO(bool i_useHIO) {
    if (i_useHIO || g_drawHIO.mFishListScreen.mDebug) {
        for (int i = 0; i < MAX_FINDABLE_FISHES; i++) {
            mpFishParent[i]->paneTrans(g_drawHIO.mFishListScreen.mFishInfoPosX[i],
                                       g_drawHIO.mFishListScreen.mFishInfoPosY[i]);
            mpFishParent[i]->scale(g_drawHIO.mFishListScreen.mFishInfoScale[i],
                                   g_drawHIO.mFishListScreen.mFishInfoScale[i]);

            mpFishParts[0][i]->paneTrans(g_drawHIO.mFishListScreen.mFishIconPosX[i],
                                         g_drawHIO.mFishListScreen.mFishIconPosY[i]);
            mpFishParts[0][i]->scale(g_drawHIO.mFishListScreen.mFishIconScale[i],
                                     g_drawHIO.mFishListScreen.mFishIconScale[i]);

            mpFishParts[1][i]->paneTrans(g_drawHIO.mFishListScreen.mFishNamePosX[i],
                                         g_drawHIO.mFishListScreen.mFishNamePosY[i]);
            mpFishParts[1][i]->scale(g_drawHIO.mFishListScreen.mFishNameScale[i],
                                     g_drawHIO.mFishListScreen.mFishNameScale[i]);

            mpFishParts[2][i]->paneTrans(g_drawHIO.mFishListScreen.mFishLinePosX[i],
                                         g_drawHIO.mFishListScreen.mFishLinePosY[i]);
            mpFishParts[2][i]->scale(g_drawHIO.mFishListScreen.mFishLineScale[i],
                                     g_drawHIO.mFishListScreen.mFishLineScale[i]);

            mpFishParts[3][i]->paneTrans(g_drawHIO.mFishListScreen.mFishSizePosX[i],
                                         g_drawHIO.mFishListScreen.mFishSizePosY[i]);
            mpFishParts[3][i]->scale(g_drawHIO.mFishListScreen.mFishSizeScale[i],
                                     g_drawHIO.mFishListScreen.mFishSizeScale[i]);

            mpFishParts[4][i]->paneTrans(g_drawHIO.mFishListScreen.mFishCountPosX[i],
                                         g_drawHIO.mFishListScreen.mFishCountPosY[i]);
            mpFishParts[4][i]->scale(g_drawHIO.mFishListScreen.mFishCountScale[i],
                                     g_drawHIO.mFishListScreen.mFishCountScale[i]);

            mpFishParts[5][i]->paneTrans(g_drawHIO.mFishListScreen.mFishCountSizePosX[i],
                                         g_drawHIO.mFishListScreen.mFishCountSizePosY[i]);
            mpFishParts[5][i]->scale(g_drawHIO.mFishListScreen.mFishCountSizeScale[i],
                                     g_drawHIO.mFishListScreen.mFishCountSizeScale[i]);
        }
        for (int i = 0; i < 2; i++) {
            mpFishInfoParent[i]->paneTrans(g_drawHIO.mFishListScreen.mPosX[i],
                                           g_drawHIO.mFishListScreen.mPosY[i]);
            mpFishInfoParent[i]->scale(g_drawHIO.mFishListScreen.mScale[i],
                                       g_drawHIO.mFishListScreen.mScale[i]);
        }
    }
    if (g_drawHIO.mCollectScreen.mButtonDebugON || i_useHIO) {
        if (mpButtonAB[A_BUTTON] != NULL) {
            mpButtonAB[A_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonAPosX,
                                     g_drawHIO.mCollectScreen.mButtonAPosY);
            mpButtonAB[A_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonAScale,
                                 g_drawHIO.mCollectScreen.mButtonAScale);
        }
        if (mpButtonAB[B_BUTTON] != NULL) {
            mpButtonAB[B_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonBPosX,
                                     g_drawHIO.mCollectScreen.mButtonBPosY);
            mpButtonAB[B_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonBScale,
                                 g_drawHIO.mCollectScreen.mButtonBScale);
        }
        if (mpButtonText[A_BUTTON] != NULL) {
            mpButtonText[A_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonATextPosX,
                                       g_drawHIO.mCollectScreen.mButtonATextPosY);
            mpButtonText[A_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonATextScale,
                                   g_drawHIO.mCollectScreen.mButtonATextScale);
        }
        if (mpButtonText[B_BUTTON] != NULL) {
            mpButtonText[B_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonBTextPosX,
                                       g_drawHIO.mCollectScreen.mButtonBTextPosY);
            mpButtonText[B_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonBTextScale,
                                   g_drawHIO.mCollectScreen.mButtonBTextScale);
        }
    }
}
