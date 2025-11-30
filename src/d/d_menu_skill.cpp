/**
 * d_menu_skill.cpp
 * Menu - Hidden Skills
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_skill.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "d/d_com_inf_game.h"
#include "d/d_lib.h"
#include "d/d_select_cursor.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"

typedef void (dMenu_Skill_c::*initFunc)();
static initFunc map_init_process[] = {
    &dMenu_Skill_c::wait_init,
    &dMenu_Skill_c::read_open_init,
    &dMenu_Skill_c::read_move_init,
    &dMenu_Skill_c::read_close_init,
};

typedef void (dMenu_Skill_c::*moveFunc)();
static moveFunc map_move_process[] = {
    &dMenu_Skill_c::wait_move,
    &dMenu_Skill_c::read_open_move,
    &dMenu_Skill_c::read_move_move,
    &dMenu_Skill_c::read_close_move,
};

dMenu_Skill_c::dMenu_Skill_c(JKRExpHeap* i_heap, STControl* i_stcontrol, CSTControl* i_cstcontrol) {
    mpHeap = i_heap;
    mpArchive = NULL;
    mpMount = NULL;
    mpStick = i_stcontrol;
    mpCStick = i_cstcontrol;
    mStatus = 1;
    mProcess = PROC_CLOSE_MOVE;
    mFrame = 0;
    mStringID = 0;
    mProcFrame = 0;
    mIndex = 0;
    field_0x209 = 255;
    field_0x20a = 255;
    skill_init_calc();
    mPosX = 0.0f;
    mBarScale[1] = 1.0f;
    mBarScale[0] = 1.0f;
}

dMenu_Skill_c::~dMenu_Skill_c() {
    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpString;
    mpString = NULL;

    delete mpMenuScreen;
    mpMenuScreen = NULL;

    delete mpLetterScreen;
    mpLetterScreen = NULL;

    delete mpTextPane;
    mpTextPane = NULL;

    delete mpExpName;
    mpExpName = NULL;

    delete mpTextParent;
    mpTextParent = NULL;

    delete mpBlackTex;
    mpBlackTex = NULL;

    delete mpParent;
    mpParent = NULL;

    for (int i = 0; i < 7; i++) {
        delete mpLetterParent[i];
        mpLetterParent[i] = NULL;
    }

    delete mpIconScreen;
    mpIconScreen = NULL;

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

    if (mpMount != NULL) {
        mpMount->getArchive()->unmount();
        delete mpMount;
        mpMount = NULL;
    }

    if (mpArchive != NULL) {
        mpArchive->unmount();
        mpArchive = NULL;
    }
}

void dMenu_Skill_c::_create() {
    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(1.01f, 0.85f, 0.02f, 0.5f, 0.5f);
    mpDrawCursor->setAlphaRate(0.0f);
    mpDrawCursor->setScale(0.0f);
    mpDrawCursor->offPlayAnime(0);
    mpString = new dMsgString_c();
    screenSetMenu();
    screenSetLetter();
    screenSetDoIcon();
    setHIO(true);
    init();
}

void dMenu_Skill_c::_move() {
    JKRHeap* heap = mDoExt_setCurrentHeap((JKRHeap*)mpHeap);
    u8 process = mProcess;
    (this->*map_move_process[mProcess])();
    if (process != mProcess) {
        (this->*map_init_process[mProcess])();
    }
    setHIO(false);
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Skill_c::_draw() {
    if (mpArchive != NULL) {
        J2DGrafContext* context = dComIfGp_getCurrentGrafPort();
        u8 alpha = mpBlackTex->mAlpha;
        mpBlackTex->setAlpha(0xff);
        mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
        mpBlackTex->setAlpha(alpha);
        mpMenuScreen->draw(mPosX, 0.0f, context);
        mpDrawCursor->draw();
        if (mProcess == 1 || mProcess == 2 || mProcess == 3) {
            mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
            mpLetterScreen->draw(0.0f, 0.0f, context);
            if (mStringID != 0) {
                mpString->getString(mStringID, (J2DTextBox*)mpTextPane->getPanePtr(), NULL, NULL,
                                    NULL, 0);
                mpString->drawOutFont((J2DTextBox*)mpTextPane->getPanePtr(), -1.0f);
            }
        }
        mpIconScreen->draw(0.0f, 0.0f, context);
    }
}

bool dMenu_Skill_c::isSync() {
    if (mpMount != NULL && mpMount->sync() == false) {
        return 0;
    }
    return 1;
}

void dMenu_Skill_c::skill_init_calc() {
    mTotalSkillNum = getSkillNum();
    if (mTotalSkillNum < 7) {
        mSkillNum = mTotalSkillNum;
    } else {
        mSkillNum = 7;
    }

    if (mTotalSkillNum % 7 == 0) {
        mRemainder = mTotalSkillNum / 7;
    } else {
        mRemainder = mTotalSkillNum / 7 + 1;
    }
}

void dMenu_Skill_c::init() {
    setPageText();
    changeActiveColor();
    (this->*map_init_process[mProcess])();
}

int dMenu_Skill_c::_open() {
    if (!mpMount) {
        mpMount = mDoDvdThd_mountArchive_c::create("/res/Layout/skillres.arc", 0, NULL);
    }
    if (!mpArchive) {
        if (mpMount->sync() != 0) {
            if (!mpArchive) {
                mpArchive = (JKRArchive*)mpMount->getArchive();
                delete mpMount;
                mpMount = NULL;
                _create();
            }
        } else {
            return 0;
        }
    }

    s16 openWindowFrame = g_drawHIO.mSkillListScreen.mOpenFrame[dMeter_drawSkillHIO_c::WINDOW];
    s16 closeFrame = g_drawHIO.mSkillListScreen.mCloseFrame[dMeter_drawSkillHIO_c::WINDOW];
    mFrame = g_drawHIO.mSkillListScreen.mOpenFrame[dMeter_drawSkillHIO_c::WINDOW];
    if (mFrame >= openWindowFrame) {
        mFrame = closeFrame;
        mStatus = 2;
        mpParent->scale(1.0f, 1.0f);
        mpParent->setAlphaRate(1.0f);
        setCursorPos();
        mpDrawCursor->setAlphaRate(1.0f);
        mpDrawCursor->setScale(1.0f);
        mpDrawCursor->onPlayAnime(0);
        return 1;
    } else {
        f32 div = (f32)mFrame / (f32)openWindowFrame;
        mpParent->scale(div, div);
        mpParent->setAlphaRate(div);
        setCursorPos();
        mpDrawCursor->setAlphaRate(div);
        mpDrawCursor->setScale(div);
        return 0;
    }
}

int dMenu_Skill_c::_close() {
    s16 closeFrame = g_drawHIO.mSkillListScreen.mCloseFrame[0];
    mFrame = 0;
    if (mFrame <= 0) {
        mFrame = 0;
        mStatus = 0;
        mpParent->scale(0.0f, 0.0f);
        mpParent->setAlphaRate(0.0f);
        setCursorPos();
        mpDrawCursor->setAlphaRate(0.0f);
        mpDrawCursor->setScale(0.0f);
        return 1;
    } else {
        f32 div = (f32)mFrame / (f32)closeFrame;
        mpParent->scale(div, div);
        mpParent->setAlphaRate(div);
        setCursorPos();
        mpDrawCursor->setAlphaRate(div);
        mpDrawCursor->setScale(div);
        return 0;
    }
    return mFrame <= 0;
}

void dMenu_Skill_c::wait_init() {
    setAButtonString(0x40C);
    setBButtonString(0x3f9);
}

void dMenu_Skill_c::wait_move() {
    u8 oldIndex = mIndex;
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        if (mDoCPd_c::getTrigB(PAD_1) != 0) {
            mpDrawCursor->offPlayAnime(0);
            mStatus = 3;
        } else if (mDoCPd_c::getTrigA(PAD_1)) {
            mProcess = PROC_WAIT_MOVE;
            Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        } else if (mpStick->checkUpTrigger()) {
            if (mIndex) {
                mIndex--;
                Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
            }

        } else if (mpStick->checkDownTrigger() && mIndex < mSkillNum - 1) {
            mIndex++;
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
        if (oldIndex != mIndex) {
            changeActiveColor();
        }
    }
}

void dMenu_Skill_c::read_open_init() {
    static const u32 i_id[7] = {
        1716, 1715, 1717, 1718, 1719, 1720, 1721,
    };
    static const u32 i_id1[7] = {
        1709, 1708, 1710, 1711, 1712, 1713, 1714,
    };

    mProcFrame = 0;
    int index = mIndex;
    mStringID = i_id[index];
    mpTextParent->setAlphaRate(0.0f);
    setNameString(i_id1[index]);
    mpString->getString(mStringID, (J2DTextBox*)mpTextPane->getPanePtr(), NULL, NULL, NULL, 0);
    setAButtonString(0);
    setBButtonString(0);
    mpBlackTex->setAlpha(0);
}

void dMenu_Skill_c::read_open_move() {
    s16 openSkillDescFrame =
        g_drawHIO.mSkillListScreen.mOpenFrame[dMeter_drawSkillHIO_c::SKILL_DESC];
    mProcFrame++;
    if (mProcFrame >= openSkillDescFrame) {
        mProcess = PROC_OPEN_MOVE;
        mpTextParent->setAlphaRate(1.0f);
        mpBlackTex->setAlpha(g_drawHIO.mSkillListScreen.mWindowBGalpha);
    } else {
        f32 alphaRate = (f32)mProcFrame / (f32)openSkillDescFrame;
        mpTextParent->setAlphaRate(alphaRate);
        mpBlackTex->setAlpha(g_drawHIO.mSkillListScreen.mWindowBGalpha * alphaRate);
    }
}

void dMenu_Skill_c::read_move_init() {
    setAButtonString(0);
    setBButtonString(0x3f9);
}

void dMenu_Skill_c::read_move_move() {
    if (mDoCPd_c::getTrigA(PAD_1) != 0) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_set2DVibration();
        mProcess = PROC_MOVE_MOVE;

    } else if (mDoCPd_c::getTrigB(PAD_1) != 0) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_set2DVibration();
        mProcess = PROC_MOVE_MOVE;
    }
}

void dMenu_Skill_c::read_close_init() {
    mProcFrame = g_drawHIO.mSkillListScreen.mCloseFrame[dMeter_drawSkillHIO_c::SKILL_DESC];
    mStringID = 0;
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Skill_c::read_close_move() {
    s16 closeSkillDescFrame =
        g_drawHIO.mSkillListScreen.mCloseFrame[dMeter_drawSkillHIO_c::SKILL_DESC];
    mProcFrame--;
    if (mProcFrame <= 0) {
        mProcess = PROC_CLOSE_MOVE;
        mpTextParent->setAlphaRate(0.0f);
        mpBlackTex->setAlpha(g_drawHIO.mSkillListScreen.mWindowBGalpha);
    } else {
        f32 alphaRate = (f32)mProcFrame / (f32)closeSkillDescFrame;
        mpTextParent->setAlphaRate(alphaRate);
        mpBlackTex->setAlpha(g_drawHIO.mSkillListScreen.mWindowBGalpha * alphaRate);
    }
}

void dMenu_Skill_c::screenSetMenu() {
    static const u64 tag_sub0[7] = {
        'menu_t0s', 'menu_t1s', 'menu_t2s', 'menu_t3s', 'menu_t4s', 'menu_t5s', 'menu_t51',
    };

    static const u64 tag_sub1[7] = {
        'menu_t0', 'menu_t1', 'menu_t2', 'menu_t3', 'menu_t4', 'menu_t5', 'menu_t6',
    };

    static const u64 tag_name0[7] = {
        'menu_t6s', 'menu_f7s', 'menu_f8s', 'menu_t9s', 'menu_10s', 'menu_11s', 'menu_112',
    };

    static const u64 tag_name1[7] = {
        'menu_f6', 'menu_f7', 'menu_t8', 'menu_t9', 'menu_t10', 'menu_t11', 'menu_t01',
    };

    static const u64 ftag_sub0[7] = {
        'fenu_t0s', 'fenu_t1s', 'fenu_t2s', 'fenu_t3s', 'fenu_t4s', 'fenu_t5s', 'fenu_t6s',
    };

    static const u64 ftag_sub1[7] = {
        'fenu_t0', 'fenu_t1', 'fenu_t2', 'fenu_t3', 'fenu_t4', 'fenu_t5', 'fenu_t6',
    };

    static const u64 ftag_name0[7] = {
        'fenu_t7s', 'fenu_t8s', 'fenu_t9s', 'fenu_10s', 'fenu_11s', 'fenu_12s', 'fenu_13s',
    };

    static const u64 ftag_name1[7] = {
        'fenu_t7', 'fenu_t8', 'fenu_t9', 'fenu_10', 'fenu_11', 'fenu_12', 'fenu_13',
    };

    static const u64 tag_letter[7] = {
        'let_00_n', 'let_01_n', 'let_02_n', 'let_03_n', 'let_04_n', 'let_05_n', 'let_06_n',
    };

    static const u64 tag_frame[7] = {
        'flame_00', 'flame_01', 'flame_02', 'flame_03', 'flame_04', 'flame_05', 'flame_06',
    };

    static const u64 tag_maki[7] = {
        'maki_0n', 'maki_1n', 'maki_2n', 'maki_3n', 'maki_4n', 'maki_5n', 'maki_6n',
    };

    static const u64 tag_makic[7] = {
        'maki_0', 'maki_1', 'maki_2', 'maki_3', 'maki_4', 'maki_5', 'maki_6',
    };

    mpMenuScreen = new J2DScreen();
    mpMenuScreen->setPriority("zelda_ougi_window.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpMenuScreen);
    mpParent = new CPaneMgr(mpMenuScreen, 'n_all', 2, NULL);
    mpParent->setAlphaRate(0.0f);
    for (int i = 0; i < 7; i++) {
#if VERSION == VERSION_GCN_JPN
        mpFTagPicture[i][0] = (J2DTextBox*)mpMenuScreen->search(tag_sub0[i]);
        mpFTagPicture[i][1] = (J2DTextBox*)mpMenuScreen->search(tag_sub1[i]);
        mpFTagPicture[i][2] = (J2DTextBox*)mpMenuScreen->search(tag_name0[i]);
        mpFTagPicture[i][3] = (J2DTextBox*)mpMenuScreen->search(tag_name1[i]);
        mpMenuScreen->search(ftag_sub0[i])->hide();
        mpMenuScreen->search(ftag_sub1[i])->hide();
        mpMenuScreen->search(ftag_name0[i])->hide();
        mpMenuScreen->search(ftag_name1[i])->hide();
#else
        mpFTagPicture[i][0] = (J2DTextBox*)mpMenuScreen->search(ftag_sub0[i]);
        mpFTagPicture[i][1] = (J2DTextBox*)mpMenuScreen->search(ftag_sub1[i]);
        mpFTagPicture[i][2] = (J2DTextBox*)mpMenuScreen->search(ftag_name0[i]);
        mpFTagPicture[i][3] = (J2DTextBox*)mpMenuScreen->search(ftag_name1[i]);
        mpMenuScreen->search(tag_sub0[i])->hide();
        mpMenuScreen->search(tag_sub1[i])->hide();
        mpMenuScreen->search(tag_name0[i])->hide();
        mpMenuScreen->search(tag_name1[i])->hide();
#endif
        for (int j = 0; j < 4; j++) {
            mpFTagPicture[i][j]->setFont(mDoExt_getMesgFont());
            mpFTagPicture[i][j]->setString(0x40, "");
        }
    }
    for (int i = 0; i < 7; i++) {
        mpLetterParent[i] = new CPaneMgr(mpMenuScreen, tag_letter[i], 0, NULL);
    }
    for (int i = 0; i < 7; i++) {
        mpTagPicture[i][0] = (J2DPicture*)mpMenuScreen->search(tag_frame[i]);
        mpTagPicture[i][1] = (J2DPicture*)mpMenuScreen->search(tag_maki[i]);
        mpTagPicture[i][2] = (J2DPicture*)mpMenuScreen->search(tag_letter[i]);
        field_0x94[i] = (J2DPicture*)mpMenuScreen->search(tag_makic[i]);
        if (i < mSkillNum) {
            mpTagPicture[i][1]->show();
            mpTagPicture[i][2]->show();
        } else {
            mpTagPicture[i][1]->hide();
            mpTagPicture[i][2]->hide();
        }
    }
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            mUnselectBlack[i] = mpTagPicture[0][i]->getBlack();
            mUnselectWhite[i] = mpTagPicture[0][i]->getWhite();
            mSelectBlack[i] = mpTagPicture[1][i]->getBlack();
            mSelectWhite[i] = mpTagPicture[1][i]->getWhite();
            mUnselectBlack[i].a = 255;
            mSelectBlack[i].a = 255;
        } else if (i == 3) {
            mUnselectBlack[i] = mpFTagPicture[0][i]->getBlack();
            mUnselectWhite[i] = mpFTagPicture[0][i]->getWhite();
            mSelectBlack[i] = mpFTagPicture[1][i]->getBlack();
            mSelectWhite[i] = mpFTagPicture[1][i]->getWhite();
        }
    }
#if VERSION == VERSION_GCN_JPN
    J2DTextBox* textBox = (J2DTextBox*)mpMenuScreen->search('t_t00');
    mpMenuScreen->search('f_t00')->hide();
#else
    J2DTextBox* textBox = (J2DTextBox*)mpMenuScreen->search('f_t00');
    mpMenuScreen->search('t_t00')->hide();
#endif
    textBox->setFont(mDoExt_getSubFont());
    textBox->setString(0x200, "");
    mpString->getString(0x6a4, textBox, NULL, NULL, NULL, 0);
}

void dMenu_Skill_c::screenSetLetter() {
    static const u64 name_tag[4] = {
        'item_n04',
        'item_n05',
        'item_n06',
        'item_n07',
    };
    static const u64 fame_tag[4] = {
        'f_item_1',
        'f_item_2',
        'f_item_3',
        'f_item_4',
    };

    mpLetterScreen = new J2DScreen();
    mpLetterScreen->setPriority("zelda_ougi_info.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpLetterScreen);
#if VERSION == VERSION_GCN_JPN
    mpTextPane = new CPaneMgr(mpLetterScreen, 'mg_3line', 0, NULL);
    mpLetterScreen->search('n_e4line')->hide();
#else
    mpTextPane = new CPaneMgr(mpLetterScreen, 'mg_e4lin', 0, NULL);
    mpLetterScreen->search('n_3line')->hide();
#endif
    mpExpName = new CPaneMgr(mpLetterScreen, 'label_n', 0, NULL);
    J2DTextBox* paneFont = (J2DTextBox*)mpTextPane->getPanePtr();
    paneFont->setFont(mDoExt_getMesgFont());
    J2DTextBox* paneString = (J2DTextBox*)mpTextPane->getPanePtr();
    paneString->setString(0x200, "");
    for (int i = 0; i < 4; i++) {
#if VERSION == VERSION_GCN_JPN
        mpNameString[i] = (J2DTextBox*)mpLetterScreen->search(name_tag[i]);
        mpLetterScreen->search(fame_tag[i])->hide();
#else
        mpNameString[i] = (J2DTextBox*)mpLetterScreen->search(fame_tag[i]);
        mpLetterScreen->search(name_tag[i])->hide();
#endif
        mpNameString[i]->setFont(mDoExt_getMesgFont());
        mpNameString[i]->setString(0x40, "");
    }
    mpTextParent = new CPaneMgr(mpLetterScreen, 'n_all', 2, NULL);
    mpTextParent->setAlphaRate(0.0f);
    ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(timg);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xff));
    mpBlackTex->setAlpha(0);
}

void dMenu_Skill_c::screenSetDoIcon() {
    static const u64 text_a_tag[5] = {
        'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5',
    };
    static const u64 text_b_tag[5] = {
        'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5',
    };
    mpIconScreen = new J2DScreen();
    mpIconScreen->setPriority("zelda_collect_soubi_do_icon_parts.blo", 0x20000, mpArchive);
    for (int i = 0; i < 2; i++) {
        mpButtonAB[i] = 0;
        mpButtonText[i] = 0;
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

void dMenu_Skill_c::setCursorPos() {
    Vec pos = mpLetterParent[mIndex]->getGlobalVtxCenter(mpLetterParent[mIndex]->mPane, false, 0);
    mpDrawCursor->setPos(pos.x, pos.y, mpLetterParent[mIndex]->getPanePtr(), false);
}

void dMenu_Skill_c::changeActiveColor() {
    setCursorPos();
    for (int i = 0; i < 7; i++) {
        if (i == mIndex) {
            mpTagPicture[i][0]->setBlackWhite(mSelectBlack[0], mSelectWhite[0]);
            mpFTagPicture[i][1]->setBlackWhite(mSelectBlack[3], mSelectWhite[3]);
            mpFTagPicture[i][3]->setBlackWhite(mSelectBlack[3], mSelectWhite[3]);
            mpLetterParent[i]->scale(mBarScale[0], mBarScale[0]);
            field_0x94[i]->setBlackWhite(mSelectBlack[4], mSelectWhite[4]);
        } else {
            mpTagPicture[i][0]->setBlackWhite(mUnselectBlack[0], mUnselectWhite[0]);
            mpFTagPicture[i][1]->setBlackWhite(mUnselectBlack[3], mUnselectWhite[3]);
            mpFTagPicture[i][3]->setBlackWhite(mUnselectBlack[3], mUnselectWhite[3]);
            mpLetterParent[i]->scale(mBarScale[1], mBarScale[1]);
            field_0x94[i]->setBlackWhite(mUnselectBlack[4], mUnselectWhite[4]);
        }
    }
}

void dMenu_Skill_c::setPageText() {
    static const u32 i_id0[7] = {
        1701, 1702, 1703, 1704, 1705, 1706, 1707,
    };
    static const u32 i_id1[7] = {
        1709, 1708, 1710, 1711, 1712, 1713, 1714,
    };

    for (int i = 0; i < mSkillNum; i++) {
        mpString->getString(i_id0[i], mpFTagPicture[i][0], NULL, NULL, NULL, 0);
        mpString->getString(i_id0[i], mpFTagPicture[i][1], NULL, NULL, NULL, 0);
        mpString->getString(i_id1[i], mpFTagPicture[i][2], NULL, NULL, NULL, 0);
        mpString->getString(i_id1[i], mpFTagPicture[i][3], NULL, NULL, NULL, 0);
    }
}

void dMenu_Skill_c::setAButtonString(u16 i_stringID) {
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

void dMenu_Skill_c::setBButtonString(u16 i_stringID) {
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

void dMenu_Skill_c::setNameString(u16 i_stringID) {
    if (i_stringID == 0) {
        for (int i = 0; i < 4; i++) {
            strcpy(mpNameString[i]->getStringPtr(), "");
        }
    } else {
        for (int i = 0; i < 4; i++) {
            dMeter2Info_getStringKanji(i_stringID, mpNameString[i]->getStringPtr(), NULL);
        }
    }
}

u8 dMenu_Skill_c::getSkillNum() {
    static u32 evt_id[7] = {
        339, /* dSv_event_flag_c::F_0339 - Secret techniques - Obtained 2 secret techinques */
        338, /* dSv_event_flag_c::F_0338 - Secret techniques - Obtained 1 secret techinques - Shield attack */
        340, /* dSv_event_flag_c::F_0340 - Secret techniques - Obtained 3 secret techinques */
        341, /* dSv_event_flag_c::F_0341 - Secret techniques - Obtained 4 secret techinques */
        342, /* dSv_event_flag_c::F_0342 - Secret techniques - Obtained 5 secret techinques */
        343, /* dSv_event_flag_c::F_0343 - Secret techniques - Obtained 6 secret techinques */
        344, /* dSv_event_flag_c::F_0344 - Secret techniques - Obtained 7 secret techinques */
    };

    u8 skillNum = 0;
    for (int i = 0; i < 7; i++) {
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[evt_id[i]])) {
            skillNum++;
        }
    }
    return skillNum;
}

void dMenu_Skill_c::setHIO(bool i_useHIO) {
    if (i_useHIO || g_drawHIO.mSkillListScreen.mDebug) {
        mSelectBlack[0].set(g_drawHIO.mSkillListScreen.mSelectBarBlack);
        mSelectWhite[0].set(g_drawHIO.mSkillListScreen.mSelectBarWhite);
        mSelectBlack[3].set(g_drawHIO.mSkillListScreen.mSelectTextBlack);
        mSelectWhite[3].set(g_drawHIO.mSkillListScreen.mSelectTextWhite);
        mBarScale[0] = g_drawHIO.mSkillListScreen.mSelectBarScale;
        mBarScale[1] = g_drawHIO.mSkillListScreen.mUnselectBarScale;
        mSelectBlack[4].set(g_drawHIO.mSkillListScreen.mSelectScrollIconBlack);
        mSelectWhite[4].set(g_drawHIO.mSkillListScreen.mSelectScrollIconWhite);
        mUnselectBlack[4].set(g_drawHIO.mSkillListScreen.mUnselectScrollIconBlack);
        mUnselectWhite[4].set(g_drawHIO.mSkillListScreen.mUnselectScrollIconWhite);
        for (int i = 0; i < 7; i++) {
            if (i == mIndex) {
                mpTagPicture[i][0]->setBlackWhite(mSelectBlack[0], mSelectWhite[0]);
                mpFTagPicture[i][1]->setBlackWhite(mSelectBlack[3], mSelectWhite[3]);
                mpFTagPicture[i][3]->setBlackWhite(mSelectBlack[3], mSelectWhite[3]);
                mpLetterParent[i]->scale(mBarScale[0], mBarScale[0]);
                field_0x94[i]->setBlackWhite(mSelectBlack[4], mSelectWhite[4]);
            } else {
                mpLetterParent[i]->scale(mBarScale[1], mBarScale[1]);
                field_0x94[i]->setBlackWhite(mUnselectBlack[4], mUnselectWhite[4]);
            }
        }
        mpTextParent->paneTrans(g_drawHIO.mSkillListScreen.mSkillDescPosX,
                                g_drawHIO.mSkillListScreen.mSkillDescPosY);
        mpTextParent->scale(g_drawHIO.mSkillListScreen.mSkillDescScale,
                            g_drawHIO.mSkillListScreen.mSkillDescScale);
        if (mProcess == 2) {
            mpBlackTex->setAlpha(g_drawHIO.mSkillListScreen.mWindowBGalpha);
        }
        mpExpName->paneTrans(g_drawHIO.mSkillListScreen.mSkillTitlePosX,
                             g_drawHIO.mSkillListScreen.mSkillTitlePosY);
        mpExpName->scale(g_drawHIO.mSkillListScreen.mSkillTitleScale,
                         g_drawHIO.mSkillListScreen.mSkillTitleScale);
    }
    if (g_drawHIO.mCollectScreen.mButtonDebugON || i_useHIO) {
        if (mpButtonAB[A_BUTTON]) {
            mpButtonAB[A_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonAPosX,
                                     g_drawHIO.mCollectScreen.mButtonAPosY);
            mpButtonAB[A_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonAScale,
                                 g_drawHIO.mCollectScreen.mButtonAScale);
        }
        if (mpButtonAB[B_BUTTON]) {
            mpButtonAB[B_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonBPosX,
                                     g_drawHIO.mCollectScreen.mButtonBPosY);
            mpButtonAB[B_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonBScale,
                                 g_drawHIO.mCollectScreen.mButtonBScale);
        }
        if (mpButtonText[A_BUTTON]) {
            mpButtonText[A_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonATextPosX,
                                       g_drawHIO.mCollectScreen.mButtonATextPosY);
            mpButtonText[A_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonATextScale,
                                   g_drawHIO.mCollectScreen.mButtonATextScale);
        }
        if (mpButtonText[B_BUTTON]) {
            mpButtonText[B_BUTTON]->paneTrans(g_drawHIO.mCollectScreen.mButtonBTextPosX,
                                       g_drawHIO.mCollectScreen.mButtonBTextPosY);
            mpButtonText[B_BUTTON]->scale(g_drawHIO.mCollectScreen.mButtonBTextScale,
                                   g_drawHIO.mCollectScreen.mButtonBTextScale);
        }
    }
}
