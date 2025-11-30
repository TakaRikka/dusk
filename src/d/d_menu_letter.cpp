#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_letter.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "stdio.h"
#include "d/d_com_inf_game.h"
#include "d/d_select_cursor.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_string.h"
#include "dolphin/types.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_msg_scrn_arrow.h"
#include "d/d_lib.h"

#if VERSION == VERSION_GCN_JPN
#define LINE_MAX 9
#else
#define LINE_MAX 12
#endif

typedef void (dMenu_Letter_c::*initFunc)();
static initFunc map_init_process[] = {
    &dMenu_Letter_c::wait_init,
    &dMenu_Letter_c::slide_right_init,
    &dMenu_Letter_c::slide_left_init,
    &dMenu_Letter_c::read_open_init,
    &dMenu_Letter_c::read_move_init,
    &dMenu_Letter_c::read_next_fadeout_init,
    &dMenu_Letter_c::read_next_fadein_init,
    &dMenu_Letter_c::read_close_init,
};

typedef void (dMenu_Letter_c::*moveFunc)();
static moveFunc map_move_process[] = {
    &dMenu_Letter_c::wait_move,
    &dMenu_Letter_c::slide_right_move,
    &dMenu_Letter_c::slide_left_move,
    &dMenu_Letter_c::read_open_move,
    &dMenu_Letter_c::read_move_move,
    &dMenu_Letter_c::read_next_fadeout_move,
    &dMenu_Letter_c::read_next_fadein_move,
    &dMenu_Letter_c::read_close_move,
};

dMenu_Letter_c::dMenu_Letter_c(JKRExpHeap* i_heap, STControl* i_stick, CSTControl* i_cstick) {
    mpHeap = i_heap;
    mpMount = NULL;
    mpArchive = NULL;
    mpStick = i_stick;
    mpCStick = i_cstick;
    mStatus = 1;
    mProcess = 0;
    field_0x368 = 0;
    field_0x36a = 0;
    mIndex = 0;
    field_0x36f = 0;
    field_0x372 = 0;
    field_0x3e2 = 0;
    field_0x3e3 = 0;
    field_0x3e4 = 0;
    field_0x370 = 0xff;
    field_0x371 = 0xff;
    letter_init_calc();
    field_0x358 = 0.0f;
    field_0x35c = 0.0f;
    field_0x360 = field_0x364 = 1.0f;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 6; j++) {
            field_0x376[i][j] = 0;
        }
    }
}


dMenu_Letter_c::~dMenu_Letter_c() {
    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpArrow;
    mpArrow = NULL;

    delete mpString;
    mpString = NULL;

    delete mpMenuScreen;
    mpMenuScreen = NULL;

    delete mpMenuDMYScreen;
    mpMenuDMYScreen = NULL;

    delete mpBaseScreen;
    mpBaseScreen = NULL;

    delete mpSdwScreen;
    mpSdwScreen = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpLetterScreen[i];
        mpLetterScreen[i] = NULL;

        delete field_0x2ec[i];
        field_0x2ec[i] = NULL;

        if (field_0x2f4[i] != NULL) {
            delete field_0x2f4[i];
            field_0x2f4[i] = NULL;
        }

        delete mpTextParent[i];
        mpTextParent[i] = NULL;
    }

    delete mpLineParent;
    mpLineParent = NULL;

    delete mpBlackTex;
    mpBlackTex = NULL;

    for (int i = 0; i < 3; i++) {
        delete mpParent[i];
        mpParent[i] = NULL;
    }

    for (int i = 0; i < 6; i++) {
        delete mpLetterParent[i];
        mpLetterParent[i] = NULL;
    }

    delete mpDMYParent;
    mpDMYParent = NULL;

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
        JKRUnmountArchive(mpMount->getArchive());
        mpMount->destroy();
        mpMount = NULL;
    }

    if (mpArchive != NULL) {
        JKRUnmountArchive(mpArchive);
        mpArchive = NULL;
    }
    dComIfGp_getMsgArchive(0)->removeResourceAll();
}

void dMenu_Letter_c::_create() {
    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    JUT_ASSERT(231, mpDrawCursor != NULL);
    #if VERSION == VERSION_SHIELD_DEBUG
    mpDrawCursor->setParam(1.01f, 0.85f, 0.02f, 0.5f, 0.5f);
    #else
    mpDrawCursor->setParam(1.06f, 0.9f, 0.02f, 0.4f, 0.4f);
    #endif
    mpDrawCursor->setAlphaRate(0.0f);
    mpDrawCursor->setScale(0.0f);
    mpDrawCursor->offPlayAnime(0);

    mpString = new dMsgString_c();
    JUT_ASSERT(245, mpString != NULL);
    
    mpArrow = new dMsgScrnArrow_c();
    JUT_ASSERT(249, mpArrow != NULL);

    screenSetMenu();
    screenSetBase();
    screenSetShadow();
    screenSetLetter();
    screenSetDoIcon();
    setHIO(true);
    init();
}


void dMenu_Letter_c::_move() {
    JKRHeap* heap = mDoExt_setCurrentHeap((JKRHeap*)mpHeap);
    u8 process = mProcess;
    (this->*map_move_process[mProcess])();
    if (process != mProcess) {
        (this->*map_init_process[mProcess])();
    }
    setHIO(false);
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Letter_c::_draw() {
    if (mpArchive == NULL) {
        return;
    }
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    u8 prevAlpha = mpBlackTex->getAlpha();
    mpBlackTex->setAlpha(0xff);
    mpBlackTex->draw(mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(),
                     mDoGph_gInf_c::getWidthF(), mDoGph_gInf_c::getHeightF(), false, false, false);
    mpBlackTex->setAlpha(prevAlpha);
    mpBaseScreen->draw(0.0f, 0.0f, grafContext);
    u32 left, top, width, height;
    GXGetScissor(&left, &top, &width, &height);
    CPaneMgr afStack_138;
    Mtx mtx;
    Vec local_178;
    local_178 = afStack_138.getGlobalVtx(field_0x1ec, &mtx, 0, false, 0);
    f32 x1 = local_178.x;
    f32 y1 = local_178.y;
    Vec local_184;
    local_184 = afStack_138.getGlobalVtx(field_0x1ec, &mtx, 3, false, 0);
    f32 dVar17 = mDoGph_gInf_c::getWidthF() / FB_WIDTH;
    f32 dVar16 = mDoGph_gInf_c::getHeightF() / FB_HEIGHT;
    f32 fVar1 = (x1 - mDoGph_gInf_c::getMinXF()) / dVar17;
    f32 fVar2 = y1 / dVar16;
    grafContext->scissor(fVar1, fVar2,
                         (local_184.x - x1) / dVar17,
                         (local_184.y - y1) / dVar16);
    grafContext->setScissor();
    mpMenuScreen->draw(field_0x358, 0.0f, grafContext);
    if (mProcess == 2 || mProcess == 1) {
        mpMenuDMYScreen->draw(field_0x358 + field_0x35c, 0.0f, grafContext);
    }
    grafContext->scissor(
        left,
        top,
        width,
        height);
    grafContext->setScissor();
    if (mProcess != 2 && mProcess != 1) {
        mpDrawCursor->draw();
    }
    if (mProcess == 3 || mProcess == 4 || mProcess == 5 || mProcess == 6 || mProcess == 7) {
        mpBlackTex->draw(
            mDoGph_gInf_c::getMinXF(), mDoGph_gInf_c::getMinYF(), mDoGph_gInf_c::getWidthF(),
            mDoGph_gInf_c::getHeightF(), 0, false, 0);
        u8 dVar1 = field_0x3ac[field_0x36f * 6 + mIndex] - 1;
        for (int i = 0; i < 2; i++) {
            mpLetterScreen[i]->draw(0.0f, 0.0f, grafContext);
        }
        for (int i = 0; i < 2; i++) {
            J2DTextBox* uVar10 = NULL;
            if (field_0x2f4[i] != NULL) {
                uVar10 = (J2DTextBox*)field_0x2f4[i]->getPanePtr();
            }
            J2DTextBox* uVar18 = (J2DTextBox*)field_0x2ec[i]->getPanePtr();
            mpString->getStringPage(dMenu_Letter::getLetterText(dVar1), field_0x3e3 - 1, LINE_MAX,
                uVar18, uVar10, NULL, NULL, 0);
        }
        mpString->drawOutFont((J2DTextBox*)field_0x2ec[1]->getPanePtr(), -1.0f);
        if (mProcess == 4) {
            mpArrow->draw();
        }
    }
    mpIconScreen->draw(0.0f, 0.0f, grafContext);
}

bool dMenu_Letter_c::isSync() {
    if (mpMount != NULL && mpMount->sync() == false) {
        return 0;
    }
    return 1;
}

void dMenu_Letter_c::letter_init_calc() {
    m_letter_num = getLetterNum();
    for (int i = 0; i < 54; i++) {
        if (i < m_letter_num) {
            field_0x3ac[i] = dComIfGs_getGetNumber(m_letter_num - i - 1);
        } else {
            field_0x3ac[i] = 0xff;
        }
    }
    if (m_letter_num < 6) {
        field_0x373 = m_letter_num;
    } else {
        field_0x373 = 6;
    }
    if (m_letter_num % 6 == 0) {
        field_0x374 = m_letter_num / 6;
    } else {
        field_0x374 = m_letter_num / 6 + 1;
    }
}

void dMenu_Letter_c::init() {
    setPageText();
    changeActiveColor();
    (this->*map_init_process[mProcess])();
}

int dMenu_Letter_c::_open() {
    if (mpMount == NULL) {
        mpMount = mDoDvdThd_mountArchive_c::create("/res/Layout/letres.arc", 0, NULL);
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

    s16 openWindowFrame =
        g_drawHIO.mLetterSelectScreen.mOpenFrame[dMeter_drawLetterHIO_c::WINDOW_FRAME];
    s16 closeWindowFrame =
        g_drawHIO.mLetterSelectScreen.mCloseFrame[dMeter_drawLetterHIO_c::WINDOW_FRAME];
    field_0x368 = g_drawHIO.mLetterSelectScreen.mOpenFrame[dMeter_drawLetterHIO_c::WINDOW_FRAME];
    if (field_0x368 >= openWindowFrame) {
        field_0x368 = closeWindowFrame;
        mStatus = 2;
        for (int i = 0; i < 3; i++) {
            mpParent[i]->scale(g_drawHIO.mLetterSelectScreen.mWindowScale,
                               g_drawHIO.mLetterSelectScreen.mWindowScale);
            mpParent[i]->setAlphaRate(1.0f);
        }
        setCursorPos();
        mpDrawCursor->setAlphaRate(1.0f);
        mpDrawCursor->setScale(1.0f);
        mpDrawCursor->onPlayAnime(0);
        return 1;
    } else {
        f32 div = (f32)field_0x368 / (f32)openWindowFrame;
        for (int i = 0; i < 3; i++) {
            mpParent[i]->scale(g_drawHIO.mLetterSelectScreen.mWindowScale * div,
                               g_drawHIO.mLetterSelectScreen.mWindowScale * div);
            mpParent[i]->setAlphaRate(div);
        }
        setCursorPos();
        mpDrawCursor->setAlphaRate(div);
        mpDrawCursor->setScale(div);
        return 0;
    }
}

int dMenu_Letter_c::_close() {
    s16 closeWindowFrame =
        g_drawHIO.mLetterSelectScreen.mCloseFrame[dMeter_drawLetterHIO_c::WINDOW_FRAME];
    field_0x368 = 0;
    if (field_0x368 <= 0) {
        field_0x368 = 0;
        mStatus = 0;
        for (int i = 0; i < 3; i++) {
            mpParent[i]->scale(0.0f, 0.0f);
            mpParent[i]->setAlphaRate(0.0f);
        }
        setCursorPos();
        mpDrawCursor->setAlphaRate(0.0f);
        mpDrawCursor->setScale(0.0f);
        return 1;
    } else {
        f32 div = (f32)field_0x368 / (f32)closeWindowFrame;
        for (int i = 0; i < 3; i++) {
            mpParent[i]->scale(g_drawHIO.mLetterSelectScreen.mWindowScale * div,
                               g_drawHIO.mLetterSelectScreen.mWindowScale * div);
            mpParent[i]->setAlphaRate(div);
        }
        setCursorPos();
        mpDrawCursor->setAlphaRate(div);
        mpDrawCursor->setScale(div);
        return 0;
    }
}

void dMenu_Letter_c::wait_init() {
    setAButtonString(0x40c);
    setBButtonString(0x3f9);
}

void dMenu_Letter_c::wait_move() {
    u8 oldIndex = mIndex;
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        if (mDoCPd_c::getTrigB(PAD_1) != 0) {
            mpDrawCursor->offPlayAnime(0);
            mStatus = 3;
        } else if (mDoCPd_c::getTrigA(PAD_1)) {
            mProcess = 3;
            Z2GetAudioMgr()->seStart(Z2SE_SY_LETTER_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        } else if (mpStick->checkUpTrigger()) {
            if (mIndex) {
                mIndex--;
                Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
            }

        } else if (mpStick->checkDownTrigger()) {
            if (mIndex < field_0x373 - 1) {
                mIndex++;
                Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
            }
        } else if (mDoCPd_c::getTrigR(PAD_1)) {
            if (field_0x36f < field_0x374 - 1) {
                field_0x372 = field_0x36f;
                field_0x36f++;
                mProcess = 2;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_SUB_NEXT, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
        } else if (mDoCPd_c::getTrigL(PAD_1)) {
            if (field_0x36f) {
                field_0x372 = field_0x36f;
                field_0x36f--;
                mProcess = 1;
                Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_SUB_NEXT, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
        }

        if (mProcess == 1 || mProcess == 2) {
#if VERSION == VERSION_GCN_JPN
            J2DTextBox* textBox = (J2DTextBox*)mpBaseScreen->search('t_t00');
            mpBaseScreen->search('f_t_00')->hide();
#else
            J2DTextBox* textBox = (J2DTextBox*)mpBaseScreen->search('f_t_00');
            mpBaseScreen->search('t_t00')->hide();
#endif
            dComIfGp_setMessageCountNumber(field_0x374 + (field_0x36f + 1) * 100);
            mpString->getString(0x4d6, textBox, NULL, NULL, NULL, 0);
        }
        if (oldIndex != mIndex) {
            changeActiveColor();
        }
    }
}


void dMenu_Letter_c::slide_right_init() {
    field_0x358 = -field_0x1ec->getWidth() * mDoGph_gInf_c::getInvScale();
    field_0x35c = field_0x1ec->getWidth() * mDoGph_gInf_c::getInvScale();
    changePageLight();
    copyDMYMenu();
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Letter_c::slide_right_move() {
    cLib_addCalc2(&field_0x358, 0.0f, 0.5f, 50.0f);
    if (fabsf(field_0x358) < 0.1f) {
        field_0x358 = 0.0f;
        mProcess = 0;
    }
}

void dMenu_Letter_c::slide_left_init() {
    field_0x358 = field_0x1ec->getWidth() * mDoGph_gInf_c::getInvScale();
    field_0x35c = -field_0x1ec->getWidth() * mDoGph_gInf_c::getInvScale();
    changePageLight();
    copyDMYMenu();
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Letter_c::slide_left_move() {
    cLib_addCalc2(&field_0x358, 0.0f, 0.5f, 50.0f);
    if (fabsf(field_0x358) < 0.1f) {
        field_0x358 = 0.0f;
        mProcess = 0;
    }
}

void dMenu_Letter_c::read_open_init() {
    field_0x36a = 0;
    u8 idx = field_0x3ac[field_0x36f * 6 + mIndex] - 1;
    field_0x3e3 = 1;
    for (int i = 0; i < 2; i++) {
        mpTextParent[i]->scale(0.0f, 0.0f);
        mpTextParent[i]->setAlphaRate(0.0f);
        J2DTextBox* text1 = NULL;
        if (field_0x2f4[i] != 0) {
            text1 = (J2DTextBox*)field_0x2f4[i]->getPanePtr();
        }
        J2DTextBox* text2 = (J2DTextBox*)field_0x2ec[i]->getPanePtr();
        mpString->getStringPage(dMenu_Letter::getLetterText(idx), field_0x3e3 - 1, LINE_MAX, text2, text1,
                                NULL, NULL, NULL);
    }
    field_0x3e2 = mpString->getPageMax(LINE_MAX);
    if (field_0x3e2 > 1) {
        char acStack_30[20];
        sprintf(acStack_30, "%d/%d", field_0x3e3, field_0x3e2);
        for (int i = 0; i < 2; i++) {
            field_0x1e4[i]->show();
            strcpy(field_0x1e4[i]->getStringPtr(), acStack_30);
        }
    } else {
        for (int i = 0; i < 2; i++) {
            field_0x1e4[i]->hide();
        }
    }
    if (!dComIfGs_isLetterReadFlag(idx)) {
        dComIfGs_onLetterReadFlag(idx);
        field_0x34[mIndex][4]->hide();
    }
    setAButtonString(0);
    setBButtonString(0);
    mpBlackTex->setAlpha(0);
}

void dMenu_Letter_c::read_open_move() {
    s16 openLetterFrame =
        g_drawHIO.mLetterSelectScreen.mOpenFrame[dMeter_drawLetterHIO_c::LETTER_FRAME];
    field_0x36a++;
    if (field_0x36a >= openLetterFrame) {
        mProcess = 4;
        for (int i = 0; i < 2; i++) {
            mpTextParent[i]->scale(g_drawHIO.mLetterSelectScreen.mLetterWindowScale,
                                   g_drawHIO.mLetterSelectScreen.mLetterWindowScale);
            mpTextParent[i]->setAlphaRate(1.0f);
        }
        mpBlackTex->setAlpha(g_drawHIO.mLetterSelectScreen.mWindowBGAlpha);
    } else {
        f32 div = (f32)field_0x36a / (f32)openLetterFrame;
        for (int i = 0; i < 2; i++) {
            mpTextParent[i]->scale(g_drawHIO.mLetterSelectScreen.mLetterWindowScale * div,
                                   g_drawHIO.mLetterSelectScreen.mLetterWindowScale * div);
            mpTextParent[i]->setAlphaRate(div);
        }
        mpBlackTex->setAlpha(g_drawHIO.mLetterSelectScreen.mWindowBGAlpha * div);
    }
}

void dMenu_Letter_c::read_move_init() {
    mpArrow->arwAnimeInit();
    mpArrow->dotAnimeInit();
    if (field_0x3e2 > 1) {
        if (field_0x3e3 == field_0x3e2) {
            setAButtonString(0);
            setBButtonString(0x3f9);
        } else {
            setAButtonString(0x408);
            setBButtonString(0x3f9);
        }
        CPaneMgr paneMgr;
        J2DPane* pane = mpLetterScreen[0]->search('set_ya_n');
        Vec pos = paneMgr.getGlobalVtxCenter(pane, false, 0);
        mpArrow->setPos(pos.x, pos.y);
    } else {
        setAButtonString(0);
        setBButtonString(0x3f9);
    }
}

void dMenu_Letter_c::read_move_move() {
    if (mDoCPd_c::getTrigA(PAD_1) != 0) {
        if (field_0x3e3 == field_0x3e2) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_LETTER_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
            mProcess = 7;
        } else {
            mProcess = 5;
            Z2GetAudioMgr()->seStart(Z2SE_SY_LETTER_NEXT_PAGE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            dMeter2Info_set2DVibration();
            field_0x3e4 = 1;
        }
    } else if (mDoCPd_c::getTrigB(PAD_1) != 0) {
        if (field_0x3e3 <= 1) {
            mProcess = 7;
            Z2GetAudioMgr()->seStart(Z2SE_SY_LETTER_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        } else {
            mProcess = 5;
            Z2GetAudioMgr()->seStart(Z2SE_SY_LETTER_NEXT_PAGE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            dMeter2Info_set2DVibration();
            field_0x3e4 = 0;
        }
    } else if (field_0x3e2 > 1) {
        if (field_0x3e3 == field_0x3e2) {
            mpArrow->dotAnimeMove();
        } else {
            mpArrow->arwAnimeMove();
        }
        CPaneMgr paneMgr;
        J2DPane* pane = mpLetterScreen[0]->search('set_ya_n');
        Vec pos = paneMgr.getGlobalVtxCenter(pane, false, 0);
        mpArrow->setPos(pos.x, pos.y);
    }
}

void dMenu_Letter_c::read_next_fadeout_init() {
    field_0x36a = g_drawHIO.mLetterSelectScreen.mCloseFrame[dMeter_drawLetterHIO_c::LETTER_FRAME];
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Letter_c::read_next_fadeout_move() {
    s16 closeWindowFrame =
        g_drawHIO.mLetterSelectScreen.mCloseFrame[dMeter_drawLetterHIO_c::WINDOW_FRAME];
    field_0x36a--;
    if (field_0x36a <= 0) {
        mProcess = 6;
        for (int i = 0; i < 2; i++) {
            field_0x2ec[i]->setAlphaRate(0.0f);

            if (field_0x2f4[i] != NULL) {
                field_0x2f4[i]->setAlphaRate(0.0f);
            }
        }
        if (field_0x3e4 != 0) {
            field_0x3e3++;
        } else {
            field_0x3e3--;
        }
    } else {
        f32 div = (f32)field_0x36a / (f32)closeWindowFrame;
        for (int i = 0; i < 2; i++) {
            field_0x2ec[i]->setAlphaRate(div);
            if (field_0x2f4[i]) {
                field_0x2f4[i]->setAlphaRate(div);
            }
        }
    }
}

void dMenu_Letter_c::read_next_fadein_init() {
    field_0x36a = 0;
    u8 idx = field_0x3ac[field_0x36f * 6 + mIndex] - 1;
    for (int i = 0; i < 2; i++) {
        J2DTextBox* text1 = NULL;
        if (field_0x2f4[i] != 0) {
            text1 = (J2DTextBox*)field_0x2f4[i]->getPanePtr();
        }
        J2DTextBox* text2 = (J2DTextBox*)field_0x2ec[i]->getPanePtr();
        mpString->getStringPage(dMenu_Letter::getLetterText(idx), field_0x3e3 - 1, LINE_MAX, text2, text1, NULL, NULL, NULL);
    }
    char acStack_30[10];
    sprintf(acStack_30, "%d/%d", field_0x3e3, field_0x3e2);
    for (int i = 0; i < 2; i++) {
        field_0x1e4[i]->show();
        strcpy(field_0x1e4[i]->getStringPtr(), acStack_30);
    }
}

void dMenu_Letter_c::read_next_fadein_move() {
    s16 openLetterFrame =
        g_drawHIO.mLetterSelectScreen.mOpenFrame[dMeter_drawLetterHIO_c::LETTER_FRAME];
    field_0x36a++;
    if (field_0x36a >= openLetterFrame) {
        mProcess = 4;
        for (int i = 0; i < 2; i++) {
            field_0x2ec[i]->setAlphaRate(1.0f);
            if (field_0x2f4[i] != NULL) {
                field_0x2f4[i]->setAlphaRate(1.0f);
            }
        }
    } else {
        f32 div = (f32)field_0x36a / (f32)openLetterFrame;
        for (int i = 0; i < 2; i++) {
            field_0x2ec[i]->setAlphaRate(div);
            if (field_0x2f4[i] != NULL) {
                field_0x2f4[i]->setAlphaRate(div);
            }
        }
    }
}

void dMenu_Letter_c::read_close_init() {
    field_0x36a = g_drawHIO.mLetterSelectScreen.mCloseFrame[dMeter_drawLetterHIO_c::LETTER_FRAME];
    setAButtonString(0);
    setBButtonString(0);
}

void dMenu_Letter_c::read_close_move() {
    s16 closeLetterFrame =
        g_drawHIO.mLetterSelectScreen.mCloseFrame[dMeter_drawLetterHIO_c::LETTER_FRAME];
    field_0x36a--;
    if (field_0x36a <= 0) {
        mProcess = 0;
        for (int i = 0; i < 2; i++) {
            mpTextParent[i]->scale(0.0f, 0.0f);
            mpTextParent[i]->setAlphaRate(0.0f);
        }
        mpBlackTex->setAlpha(g_drawHIO.mLetterSelectScreen.mWindowBGAlpha);
    } else {
        f32 div = (f32)field_0x36a / (f32)closeLetterFrame;
        for (int i = 0; i < 2; i++) {
            mpTextParent[i]->scale(g_drawHIO.mLetterSelectScreen.mLetterWindowScale * div,
                                   g_drawHIO.mLetterSelectScreen.mLetterWindowScale * div);
            mpTextParent[i]->setAlphaRate(div);
        }
        mpBlackTex->setAlpha(g_drawHIO.mLetterSelectScreen.mWindowBGAlpha * div);
    }
}

void dMenu_Letter_c::screenSetMenu() {
    static const u64 tag_sub0[6] = {
        'menu_t0s', 'menu_t1s', 'menu_t2s', 'menu_t3s', 'menu_t4s', 'menu_t5s',
    };
    static const u64 tag_sub1[6] = {
        'menu_t0', 'menu_t1', 'menu_t2', 'menu_t3', 'menu_t4', 'menu_t5',
    };
    static const u64 tag_name0[6] = {
        'menu_t6s', 'menu_f7s', 'menu_f8s', 'menu_t9s', 'menu_10s', 'menu_11s',
    };
    static const u64 tag_name1[6] = {
        'menu_f6', 'menu_f7', 'menu_t8', 'menu_t9', 'menu_t10', 'menu_t11',
    };
    static const u64 ftag_sub0[6] = {
        'fenu_t0s', 'fenu_t1s', 'fenu_t2s', 'fenu_t3s', 'fenu_t4s', 'fenu_t5s',
    };
    static const u64 ftag_sub1[6] = {
        'fenu_t0', 'fenu_t1', 'fenu_t2', 'fenu_t3', 'fenu_t4', 'fenu_t5',
    };
    static const u64 ftag_name0[6] = {
        'fenu_t6s', 'fenu_t7s', 'fenu_f8s', 'fenu_t9s', 'fenu_10s', 'fenu_11s',
    };
    static const u64 ftag_name1[6] = {
        'fenu_t6', 'fenu_t7', 'fenu_t8', 'fenu_t9', 'fenu_t10', 'fenu_t11',
    };
    static const u64 tag_letter[6] = {
        'let_00_n', 'let_01_n', 'let_02_n', 'let_03_n', 'let_04_n', 'let_05_n',
    };
    static const u64 tag_frame[6] = {
        'flame_00', 'flame_01', 'flame_02', 'flame_03', 'flame_04', 'flame_05',
    };
    static const u64 tag_menu0[6] = {
#if VERSION == VERSION_GCN_JPN
        'menu_t0', 'menu_t1', 'menu_t2', 'menu_t3', 'menu_t4', 'menu_t5',
#else
        'fenu_t0', 'fenu_t1', 'fenu_t2', 'fenu_t3', 'fenu_t4', 'fenu_t5',
#endif
    };
    static const u64 tag_menu1[6] = {
#if VERSION == VERSION_GCN_JPN
        'menu_f6', 'menu_f7', 'menu_t8', 'menu_t9', 'menu_t10', 'menu_t11',
#else
        'fenu_t6', 'fenu_t7', 'fenu_t8', 'fenu_t9', 'fenu_t10', 'fenu_t11',
#endif
    };
    static const u64 tag_midoku[6] = {
        'midoku_0', 'midoku_1', 'midoku_2', 'midoku_3', 'midoku_4', 'midoku_5',
    };

    mpMenuScreen = new J2DScreen();
    JUT_ASSERT(1146, mpMenuScreen != NULL);
    bool fg = mpMenuScreen->setPriority("zelda_letter_select_6menu.blo", 0x20000, mpArchive);
    JUT_ASSERT(1156, fg != false);
    dPaneClass_showNullPane(mpMenuScreen);
    mpParent[0] = new CPaneMgr(mpMenuScreen, 'n_all', 2, NULL);
    JUT_ASSERT(1161, mpParent[0] != NULL);
    mpParent[0]->setAlphaRate(0.0f);
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x124[i][0] = (J2DTextBox*)mpMenuScreen->search(tag_sub0[i]);
        field_0x124[i][1] = (J2DTextBox*)mpMenuScreen->search(tag_sub1[i]);
        field_0x124[i][2] = (J2DTextBox*)mpMenuScreen->search(tag_name0[i]);
        field_0x124[i][3] = (J2DTextBox*)mpMenuScreen->search(tag_name1[i]);
        mpMenuScreen->search(ftag_sub0[i])->hide();
        mpMenuScreen->search(ftag_sub1[i])->hide();
        mpMenuScreen->search(ftag_name0[i])->hide();
        mpMenuScreen->search(ftag_name1[i])->hide();
#else
        field_0x124[i][0] = (J2DTextBox*)mpMenuScreen->search(ftag_sub0[i]);
        field_0x124[i][1] = (J2DTextBox*)mpMenuScreen->search(ftag_sub1[i]);
        field_0x124[i][2] = (J2DTextBox*)mpMenuScreen->search(ftag_name0[i]);
        field_0x124[i][3] = (J2DTextBox*)mpMenuScreen->search(ftag_name1[i]);
        mpMenuScreen->search(tag_sub0[i])->hide();
        mpMenuScreen->search(tag_sub1[i])->hide();
        mpMenuScreen->search(tag_name0[i])->hide();
        mpMenuScreen->search(tag_name1[i])->hide();
#endif
        for (int j = 0; j < 4; j++) {
            field_0x124[i][j]->setFont(mDoExt_getMesgFont());
            field_0x124[i][j]->setString(0x40, "");
        }
    }
    for (int i = 0; i < 6; i++) {
        mpLetterParent[i] = new CPaneMgr(mpMenuScreen, tag_letter[i], 0, NULL);
        JUT_ASSERT(1212, mpLetterParent[i] != NULL);
    }
    for (int i = 0; i < 6; i++) {
        field_0x34[i][0] = mpMenuScreen->search(tag_frame[i]);
        field_0x34[i][1] = mpMenuScreen->search(tag_menu0[i]);
        field_0x34[i][2] = mpMenuScreen->search(tag_menu1[i]);
        field_0x34[i][3] = mpMenuScreen->search(tag_letter[i]);
        field_0x34[i][4] = mpMenuScreen->search(tag_midoku[i]);
        if (i < field_0x373) {
            field_0x34[i][3]->show();
            field_0x34[i][4]->show();
        } else {
            field_0x34[i][3]->hide();
            field_0x34[i][4]->hide();
        }
    }
    for (int i = 0; i < 4; i++) {
        if (i == 0) {
            field_0x328[i] = ((J2DPicture*)field_0x34[0][i])->getBlack();
            field_0x348[i] = ((J2DPicture*)field_0x34[0][i])->getWhite();
            field_0x318[i] = ((J2DPicture*)field_0x34[1][i])->getBlack();
            field_0x338[i] = ((J2DPicture*)field_0x34[1][i])->getWhite();
            field_0x328[i].a = 0xff;
            field_0x318[i].a = 0xff;
        } else if (i == 3) {
            field_0x328[i] = field_0x124[i][1]->getBlack();
            field_0x348[i] = field_0x124[i][1]->getWhite();
            field_0x318[i] = field_0x124[i][1]->getBlack();
            field_0x338[i] = field_0x124[i][1]->getWhite();
        } else {
            // Unclear what type is field_0x34
            field_0x328[i] = ((J2DTextBox*)field_0x34[0][i])->getBlack();
            field_0x348[i] = ((J2DTextBox*)field_0x34[0][i])->getWhite();
            field_0x318[i] = ((J2DTextBox*)field_0x34[1][i])->getBlack();
            field_0x338[i] = ((J2DTextBox*)field_0x34[1][i])->getWhite();
        }
    }
    mpMenuDMYScreen = new J2DScreen();
    JUT_ASSERT(1270, mpMenuDMYScreen != NULL);
    fg = mpMenuDMYScreen->setPriority("zelda_letter_select_6menu.blo", 0x20000, mpArchive);
    JUT_ASSERT(1280, fg != false);
    dPaneClass_showNullPane(mpMenuDMYScreen);
    mpDMYParent = new CPaneMgr(mpMenuDMYScreen, 'n_all', 2, NULL);
    JUT_ASSERT(1285, mpDMYParent != NULL);
    for (int i = 0; i < 6; i++) {
#if VERSION == VERSION_GCN_JPN
        field_0x184[i][0] = (J2DTextBox*)mpMenuDMYScreen->search(tag_sub0[i]);
        field_0x184[i][1] = (J2DTextBox*)mpMenuDMYScreen->search(tag_sub1[i]);
        field_0x184[i][2] = (J2DTextBox*)mpMenuDMYScreen->search(tag_name0[i]);
        field_0x184[i][3] = (J2DTextBox*)mpMenuDMYScreen->search(tag_name1[i]);
        mpMenuDMYScreen->search(ftag_sub0[i])->hide();
        mpMenuDMYScreen->search(ftag_sub1[i])->hide();
        mpMenuDMYScreen->search(ftag_name0[i])->hide();
        mpMenuDMYScreen->search(ftag_name1[i])->hide();
#else
        field_0x184[i][0] = (J2DTextBox*)mpMenuDMYScreen->search(ftag_sub0[i]);
        field_0x184[i][1] = (J2DTextBox*)mpMenuDMYScreen->search(ftag_sub1[i]);
        field_0x184[i][2] = (J2DTextBox*)mpMenuDMYScreen->search(ftag_name0[i]);
        field_0x184[i][3] = (J2DTextBox*)mpMenuDMYScreen->search(ftag_name1[i]);
        mpMenuDMYScreen->search(tag_sub0[i])->hide();
        mpMenuDMYScreen->search(tag_sub1[i])->hide();
        mpMenuDMYScreen->search(tag_name0[i])->hide();
        mpMenuDMYScreen->search(tag_name1[i])->hide();
#endif
        for (int j = 0; j < 4; j = j + 1) {
            field_0x184[i][j]->setFont(mDoExt_getMesgFont());
            field_0x184[i][j]->setString(0x40, "");
        }
    }
    for (int i = 0; i < 6; i++) {
        field_0xac[i][0] = (J2DTextBox*)mpMenuDMYScreen->search(tag_frame[i]);
        field_0xac[i][1] = (J2DTextBox*)mpMenuDMYScreen->search(tag_menu0[i]);
        field_0xac[i][2] = (J2DTextBox*)mpMenuDMYScreen->search(tag_menu1[i]);
        field_0xac[i][3] = (J2DTextBox*)mpMenuDMYScreen->search(tag_letter[i]);
        field_0xac[i][4] = (J2DTextBox*)mpMenuDMYScreen->search(tag_midoku[i]);
    }
}

void dMenu_Letter_c::screenSetBase() {
    static const u64 tag_pip[9] = {
        'pi_00_n', 'pi_01_n', 'pi_02_n', 'pi_03_n', 'pi_04_n',
        'pi_05_n', 'pi_06_n', 'pi_07_n', 'pi_08_n',
    };
    static const u64 tag_pil[9] = {
        'pi_l_00', 'pi_l_01', 'pi_l_02', 'pi_l_03', 'pi_l_04',
        'pi_l_05', 'pi_l_06', 'pi_l_07', 'pi_l_08',
    };
    static const u64 tag_pii[9] = {
        'pi_00', 'pi_01', 'pi_02', 'pi_03', 'pi_04', 'pi_05', 'pi_06', 'pi_07', 'pi_08',
    };

    mpBaseScreen = new J2DScreen();
    JUT_ASSERT(1357, mpBaseScreen != NULL);
    bool fg = mpBaseScreen->setPriority("zelda_letter_select_base.blo", 0x20000, mpArchive);
    JUT_ASSERT(1361, fg != false);
    dPaneClass_showNullPane(mpBaseScreen);
    mpBaseScreen->search('wi_btn_n')->hide();
    mpParent[1] = new CPaneMgr(mpBaseScreen, 'n_all', 2, NULL);
    JUT_ASSERT(1372, mpParent[1] != NULL);
    mpParent[1]->setAlphaRate(0.0f);
#if VERSION == VERSION_GCN_JPN
    J2DTextBox* piVar9 = (J2DTextBox*)mpBaseScreen->search('t_t00');
    mpBaseScreen->search('f_t_00')->hide();
#else
    J2DTextBox* piVar9 = (J2DTextBox*)mpBaseScreen->search('f_t_00');
    mpBaseScreen->search('t_t00')->hide();
#endif
    piVar9->setString(0x40, "");
    piVar9->setFont(mDoExt_getSubFont());
    dComIfGp_setMessageCountNumber(field_0x374 + (field_0x36f + 1) * 100);
    mpString->getString(0x4d6, piVar9, NULL, NULL, NULL, 0);
#if VERSION == VERSION_GCN_JPN
    J2DTextBox* text1 = (J2DTextBox*)mpBaseScreen->search('wps_text');
    J2DTextBox* text2 = (J2DTextBox*)mpBaseScreen->search('w_p_text');
    J2DTextBox* text3 = (J2DTextBox*)mpBaseScreen->search('g_ps_txt');
    J2DTextBox* text4 = (J2DTextBox*)mpBaseScreen->search('g_p_text');
    mpBaseScreen->search('fwpstex1')->hide();
    mpBaseScreen->search('fwp_tex1')->hide();
    mpBaseScreen->search('fgps_tx1')->hide();
    mpBaseScreen->search('fgp_tex1')->hide();
#else
    J2DTextBox* text1 = (J2DTextBox*)mpBaseScreen->search('fwpstex1');
    J2DTextBox* text2 = (J2DTextBox*)mpBaseScreen->search('fwp_tex1');
    J2DTextBox* text3 = (J2DTextBox*)mpBaseScreen->search('fgps_tx1');
    J2DTextBox* text4 = (J2DTextBox*)mpBaseScreen->search('fgp_tex1');
    mpBaseScreen->search('wps_text')->hide();
    mpBaseScreen->search('w_p_text')->hide();
    mpBaseScreen->search('g_ps_txt')->hide();
    mpBaseScreen->search('g_p_text')->hide();
#endif
    text1->setString(0x40, "");
    text2->setString(0x40, "");
    text3->setString(0x40, "");
    text4->setString(0x40, "");
    text1->setFont(mDoExt_getMesgFont());
    text2->setFont(mDoExt_getMesgFont());
    text3->setFont(mDoExt_getMesgFont());
    text4->setFont(mDoExt_getMesgFont());
    dMeter2Info_getStringKanji(0x4d7, text1->getStringPtr(), 0);
    dMeter2Info_getStringKanji(0x4d7, text2->getStringPtr(), 0);
    dMeter2Info_getStringKanji(0x4d7, text3->getStringPtr(), 0);
    dMeter2Info_getStringKanji(0x4d7, text4->getStringPtr(), 0);
    ((J2DTextBox*)mpBaseScreen->search('pi_no_00'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_01'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_02'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_03'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_04'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_05'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_06'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_07'))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)mpBaseScreen->search('pi_no_08'))->setFont(mDoExt_getMesgFont());
    field_0x1ec = (J2DTextBox*)mpBaseScreen->search('let_area');
    for (int i = 0; i < 9; i++) {
        field_0x1f0[i] = (J2DTextBox*)mpBaseScreen->search(tag_pip[i]);
        if (field_0x374 > 1 && i < field_0x374) {
            field_0x1f0[i]->show();
        } else {
            field_0x1f0[i]->hide();
        }
        mpPageLight[i] = (J2DPicture*)mpBaseScreen->search(tag_pil[i]);
        if (i == field_0x36f) {
            mpPageLight[i]->show();
        } else {
            mpPageLight[i]->hide();
        }
        field_0x238[i] = (J2DTextBox*)mpBaseScreen->search(tag_pii[i]);
    }
    if (field_0x374 > 1) {
        J2DPane* pJVar6 = mpBaseScreen->search('pi_n');
        f32 dVar18 = field_0x1f0[1]->getBounds().i.x - field_0x1f0[0]->getBounds().i.x;
        f32 dVar17 = dVar18 * (field_0x374 - 1);
        f32 dVar16 = (pJVar6->getWidth() / 2) - (dVar17 / 2);
        for (int i = 0; i < 9; i++) {
            f32 y = field_0x1f0[i]->getBounds().i.y;
            field_0x1f0[i]->move(dVar16 + dVar18 * i, y);
        }
    }
}

void dMenu_Letter_c::screenSetShadow() {
    mpSdwScreen = new J2DScreen();
    mpSdwScreen->setPriority("zelda_letter_select_shadow.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpSdwScreen);
    mpParent[2] = new CPaneMgr(mpSdwScreen, 'n_all', 2, NULL);
    mpParent[2]->setAlphaRate(0.0f);
}

void dMenu_Letter_c::screenSetLetter() {
    mpLetterScreen[0] = new J2DScreen();
    JUT_ASSERT(1507, mpLetterScreen[0] != NULL);
    bool fg = mpLetterScreen[0]->setPriority("zelda_letter_window_base.blo", 0x20000, mpArchive);
    JUT_ASSERT(1511, fg != false);
    dPaneClass_showNullPane(mpLetterScreen[0]);

#if VERSION == VERSION_GCN_JPN
    if (dComIfGs_getOptUnk0() == 0) {
        field_0x2ec[0] = new CPaneMgr(mpLetterScreen[0], 't3f_s', 0, NULL);
        field_0x2ec[1] = new CPaneMgr(mpLetterScreen[0], 'mg_3flin', 0, NULL);
        field_0x2f4[0] = new CPaneMgr(mpLetterScreen[0], 'mg_3f_s', 0, NULL);
        field_0x2f4[1] = new CPaneMgr(mpLetterScreen[0], 'mg_3f', 0, NULL);
        mpLetterScreen[0]->search('t4_s')->hide();
        mpLetterScreen[0]->search('mg_e4lin')->hide();
        mpLetterScreen[0]->search('t3_s')->hide();
        mpLetterScreen[0]->search('mg_3line')->hide();
        mpLineParent = new CPaneMgr(mpLetterScreen[0], 'jp_fri_n', 0, NULL);
        JUT_ASSERT(1530, mpLineParent != NULL);
        for (int i = 0; i < 12; i++) {
            static u64 const line_tag[12] = {
                'line00', 'line01', 'line02', 'line03', 'line04', 'line05',
                'line06', 'line07', 'line08',
            };

            if (line_tag[i] != 0) {
                field_0x25c[i] = (J2DTextBox*)mpLetterScreen[0]->search(line_tag[i]);
            } else {
                field_0x25c[i] = NULL;
            }
        }
        mpLetterScreen[0]->search('jp_n')->hide();
        mpLetterScreen[0]->search('us_n')->hide();

        ((J2DTextBox*)field_0x2f4[0]->getPanePtr())->setLineSpace(((J2DTextBox*)field_0x2ec[0]->getPanePtr())->getLineSpace());
        ((J2DTextBox*)field_0x2f4[1]->getPanePtr())->setLineSpace(((J2DTextBox*)field_0x2ec[1]->getPanePtr())->getLineSpace());
    } else {
        field_0x2ec[0] = new CPaneMgr(mpLetterScreen[0], 't3_s', 0, NULL);
        field_0x2ec[1] = new CPaneMgr(mpLetterScreen[0], 'mg_3line', 0, NULL);
        for (int i = 0; i < 2; i++) {
            field_0x2f4[i] = NULL;
        }
        mpLetterScreen[0]->search('t4_s')->hide();
        mpLetterScreen[0]->search('mg_e4lin')->hide();
        mpLetterScreen[0]->search('t3f_s')->hide();
        mpLetterScreen[0]->search('mg_3flin')->hide();
        mpLetterScreen[0]->search('mg_3f_s')->hide();
        mpLetterScreen[0]->search('mg_3f')->hide();
        mpLineParent = new CPaneMgr(mpLetterScreen[0], 'jp_n', 0, NULL);
        JUT_ASSERT(1530, mpLineParent != NULL);
        for (int i = 0; i < 12; i++) {
            static u64 const line_tag[12] = {
                'line21', 'line22', 'line23', 'line24', 'line25', 'line26',
                'line27', 'line28', 'line29',
            };

            if (line_tag[i] != 0) {
                field_0x25c[i] = (J2DTextBox*)mpLetterScreen[0]->search(line_tag[i]);
            } else {
                field_0x25c[i] = NULL;
            }
        }

        mpLetterScreen[0]->search('jp_fri_n')->hide();
        mpLetterScreen[0]->search('us_n')->hide();
    }
#else
    static u64 const line_tag[12] = {
        'line09', 'line10', 'line11', 'line12', 'line13', 'line14',
        'line15', 'line16', 'line17', 'line18', 'line19', 'line20',
    };

    field_0x2ec[0] = new CPaneMgr(mpLetterScreen[0], 't4_s', 0, NULL);
    field_0x2ec[1] = new CPaneMgr(mpLetterScreen[0], 'mg_e4lin', 0, NULL);
    for (int i = 0; i < 2; i++) {
        field_0x2f4[i] = NULL;
    }
    mpLetterScreen[0]->search('t3f_s')->hide();
    mpLetterScreen[0]->search('mg_3flin')->hide();
    mpLetterScreen[0]->search('mg_3f_s')->hide();
    mpLetterScreen[0]->search('mg_3f')->hide();
    mpLetterScreen[0]->search('t3_s')->hide();
    mpLetterScreen[0]->search('mg_3line')->hide();
    mpLineParent = new CPaneMgr(mpLetterScreen[0], 'us_n', 0, NULL);
    JUT_ASSERT(1530, mpLineParent != NULL);
    for (int i = 0; i < 12; i++) {
        if (line_tag[i] != 0) {
            field_0x25c[i] = (J2DTextBox*)mpLetterScreen[0]->search(line_tag[i]);
        } else {
            field_0x25c[i] = NULL;
        }
    }
    mpLetterScreen[0]->search('jp_fri_n')->hide();
    mpLetterScreen[0]->search('jp_n')->hide();
#endif

    for (int i = 0; i < 2; i++) {
        ((J2DTextBox*)field_0x2ec[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
        ((J2DTextBox*)field_0x2ec[i]->getPanePtr())->setString(0x210, "");
#else
        ((J2DTextBox*)field_0x2ec[i]->getPanePtr())->setString(0x200, "");
#endif
        if (field_0x2f4[i] != NULL) {
            ((J2DTextBox*)field_0x2f4[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
#if VERSION == VERSION_GCN_JPN
            ((J2DTextBox*)field_0x2f4[i]->getPanePtr())->setString(0x210, "");
#else
            ((J2DTextBox*)field_0x2f4[i]->getPanePtr())->setString(0x200, "");
#endif
        }
    }
    field_0x1e4[0] = (J2DTextBox*)mpLetterScreen[0]->search('p_texts');
    field_0x1e4[1] = (J2DTextBox*)mpLetterScreen[0]->search('p_text');
    for (int i = 0; i < 2; i++) {
        field_0x1e4[i]->setFont(mDoExt_getSubFont());
        field_0x1e4[i]->setString(0x10, "");
    }
    mpLetterScreen[1] = new J2DScreen();
    JUT_ASSERT(1627, mpLetterScreen[1] != NULL);
    fg = mpLetterScreen[1]->setPriority("zelda_letter_window_spot.blo", 0x20000, mpArchive);
    JUT_ASSERT(1631, fg != false);
    dPaneClass_showNullPane(mpLetterScreen[1]);
    for (int i = 0; i < 2; i++) {
        mpTextParent[i] = new CPaneMgr(mpLetterScreen[i], 'n_all', 2, NULL);
        JUT_ASSERT(1636, mpTextParent[i] != NULL);
        mpTextParent[i]->scale(0.0f, 0.0f);
        mpTextParent[i]->setAlphaRate(0.0f);
    }
    ResTIMG* local_30 = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(local_30);
    JUT_ASSERT(1644, mpBlackTex != NULL);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 255));
    mpBlackTex->setAlpha(0);
}

void dMenu_Letter_c::screenSetDoIcon() {
    static const u64 text_a_tag[5] = {
        'atext1_1', 'atext1_2', 'atext1_3', 'atext1_4', 'atext1_5',
    };
    static const u64 text_b_tag[5] = {
        'btext1_1', 'btext1_2', 'btext1_3', 'btext1_4', 'btext1_5',
    };

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


void dMenu_Letter_c::setCursorPos() {
    Vec pos = mpLetterParent[mIndex]->getGlobalVtxCenter(mpLetterParent[mIndex]->mPane, false, 0);
    mpDrawCursor->setPos(pos.x, pos.y, mpLetterParent[mIndex]->getPanePtr(), false);
}

void dMenu_Letter_c::changeActiveColor() {
    setCursorPos();
    for (int i = 0; (int)i < 6; i++) {
        if (i == mIndex) {
            ((J2DPicture*)field_0x34[i][0])->setBlackWhite(field_0x318[0], field_0x338[0]);
            ((J2DTextBox*)field_0x34[i][1])->setBlackWhite(field_0x318[1], field_0x338[1]);
            ((J2DTextBox*)field_0x34[i][2])->setBlackWhite(field_0x318[2], field_0x338[2]);
            field_0x124[i][1]->setBlackWhite(field_0x318[3], field_0x338[3]);
            field_0x124[i][3]->setBlackWhite(field_0x318[3], field_0x338[3]);
            mpLetterParent[i]->scale(field_0x360, field_0x360);
        } else {
            ((J2DPicture*)field_0x34[i][0])->setBlackWhite(field_0x328[0], field_0x348[0]);
            ((J2DTextBox*)field_0x34[i][1])->setBlackWhite(field_0x328[1], field_0x348[1]);
            ((J2DTextBox*)field_0x34[i][2])->setBlackWhite(field_0x328[2], field_0x348[2]);
            field_0x124[i][1]->setBlackWhite(field_0x328[3], field_0x348[3]);
            field_0x124[i][3]->setBlackWhite(field_0x328[3], field_0x348[3]);
            mpLetterParent[i]->scale(field_0x364, field_0x364);
        }
    }
}

void dMenu_Letter_c::changePageLight() {
    for (int i = 0; i < 9; i++) {
        if (i == field_0x36f) {
            mpPageLight[i]->show();
        } else {
            mpPageLight[i]->hide();
        }
    }
}

void dMenu_Letter_c::setPageText() {
    int dVar1 = field_0x36f * 6;
    for (int i = 0; i < field_0x373; i++) {
        u8 idx = field_0x3ac[i + dVar1] - 1;
        mpString->getString(dMenu_Letter::getLetterSubject(idx),field_0x124[i][0], NULL, NULL, NULL, 0);
        mpString->getString(dMenu_Letter::getLetterSubject(idx),field_0x124[i][1], NULL, NULL, NULL, 0);
        mpString->getString(dMenu_Letter::getLetterName(idx),field_0x124[i][2], NULL, NULL, NULL, 0);
        mpString->getString(dMenu_Letter::getLetterName(idx),field_0x124[i][3], NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 6; i++) {
        if (i < field_0x373) {
            u8 idx = field_0x3ac[i + dVar1] - 1;
            if (!dComIfGs_isLetterReadFlag(idx)) {
                field_0x34[i][4]->show();
            } else {
                field_0x34[i][4]->hide();
            }
        } else {
            field_0x34[i][4]->hide();
        }
    }
}

void dMenu_Letter_c::setDMYPageText() {
    int dVar1 = field_0x372 * 6;
    for (int i = 0; i < field_0x373; i++) {
        u8 idx = field_0x3ac[i + dVar1] - 1;
        mpString->getString(dMenu_Letter::getLetterSubject(idx),field_0x184[i][0], NULL, NULL, NULL, 0);
        mpString->getString(dMenu_Letter::getLetterSubject(idx),field_0x184[i][1], NULL, NULL, NULL, 0);
        mpString->getString(dMenu_Letter::getLetterName(idx),field_0x184[i][2], NULL, NULL, NULL, 0);
        mpString->getString(dMenu_Letter::getLetterName(idx),field_0x184[i][3], NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 6; i++) {
        if (i < field_0x373) {
            u8 idx = field_0x3ac[i + dVar1] - 1;
            if (!dComIfGs_isLetterReadFlag(idx)) {
                field_0xac[i][4]->show();
            } else {
                field_0xac[i][4]->hide();
            }
        } else {
            field_0xac[i][4]->hide();
        }
    }
}

void dMenu_Letter_c::copyDMYMenu() {
    setDMYPageText();
    for (int i = 0; i < 6; i++) {
        if (i == mIndex) {
            ((J2DPicture*)field_0xac[i][0])->setBlackWhite(field_0x318[0], field_0x338[0]);
            field_0xac[i][1]->setBlackWhite(field_0x318[1], field_0x338[1]);
            field_0xac[i][2]->setBlackWhite(field_0x318[2], field_0x338[2]);
            field_0x184[i][1]->setBlackWhite(field_0x318[3], field_0x338[3]);
            field_0x184[i][3]->setBlackWhite(field_0x318[3], field_0x338[3]);
            field_0xac[i][3]->scale(field_0x360, field_0x360);
        } else {
            ((J2DPicture*)field_0xac[i][0])->setBlackWhite(field_0x328[0], field_0x348[0]);
            field_0xac[i][1]->setBlackWhite(field_0x328[1], field_0x348[1]);
            field_0xac[i][2]->setBlackWhite(field_0x328[2], field_0x348[2]);
            field_0x184[i][1]->setBlackWhite(field_0x328[3], field_0x348[3]);
            field_0x184[i][3]->setBlackWhite(field_0x328[3], field_0x348[3]);
            field_0xac[i][3]->scale(field_0x364, field_0x364);
        }
        if (i < field_0x373) {
            field_0xac[i][3]->show();
        } else {
            field_0xac[i][3]->hide();
        }
    }
    mIndex = 0;
    int dVar1 = (m_letter_num - 6 * field_0x36f) & 0xff;
    if (dVar1 < 6) {
        field_0x373 = dVar1;
    } else {
        field_0x373 = 6;
    }
    changeActiveColor();
    setPageText();
    for (int i = 0; i < 6; i++) {
        if (i < field_0x373) {
            field_0x34[i][3]->show();
        } else {
            field_0x34[i][3]->hide();
        }
    }
}

void dMenu_Letter_c::setAButtonString(u16 i_stringID) {
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


void dMenu_Letter_c::setBButtonString(u16 i_stringID) {
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


u8 dMenu_Letter_c::getLetterNum() {
    u8 letterNum = 0;
    if (g_drawHIO.mLetterSelectScreen.mLetterNum > 0) {
        for (int i = 0; i < g_drawHIO.mLetterSelectScreen.mLetterNum; i++) {
            if (dMenu_Letter::getLetterSubject(i) != 0 || dMenu_Letter::getLetterName(i) != 0 ||
                dMenu_Letter::getLetterText(i) != 0)
            {
                dComIfGs_onLetterGetFlag(i);
                dComIfGs_setGetNumber(letterNum, i + 1);
                letterNum++;
            }
        }
    } else {
        letterNum = dMeter2Info_getRecieveLetterNum();
    }
    return letterNum;
}

void dMenu_Letter_c::setHIO(bool i_useHIO) {
    if (i_useHIO || g_drawHIO.mLetterSelectScreen.mDebugON) {
        for (int i = 0; i < 3; i++) {
            mpParent[i]->paneTrans(g_drawHIO.mLetterSelectScreen.mWindowPosX,
                                   g_drawHIO.mLetterSelectScreen.mWindowPosY);
            mpParent[i]->scale(g_drawHIO.mLetterSelectScreen.mWindowScale,
                               g_drawHIO.mLetterSelectScreen.mWindowScale);
        }
        mpDMYParent->paneTrans(g_drawHIO.mLetterSelectScreen.mWindowPosX,
                               g_drawHIO.mLetterSelectScreen.mWindowPosY);
        mpDMYParent->scale(g_drawHIO.mLetterSelectScreen.mWindowScale,
                           g_drawHIO.mLetterSelectScreen.mWindowScale);
        for (int i = 0; i < 9; i++) {
            mpPageLight[i]->setBlackWhite(g_drawHIO.mLetterSelectScreen.mSelectPageIconBack,
                                          g_drawHIO.mLetterSelectScreen.mSelectPageIconFront);
            if (i == field_0x36f) {
                field_0x238[i]->setAlpha(0xff);
            } else {
                field_0x238[i]->setAlpha(g_drawHIO.mLetterSelectScreen.mUnselectPageIconAlpha);
            }
        }
        field_0x318[0].set(g_drawHIO.mLetterSelectScreen.mSelectBarBack);
        field_0x338[0].set(g_drawHIO.mLetterSelectScreen.mSelectBarFront);
        field_0x318[3].set(g_drawHIO.mLetterSelectScreen.mSelectTextBack);
        field_0x338[3].set(g_drawHIO.mLetterSelectScreen.mSelectTextFront);
        field_0x360 = g_drawHIO.mLetterSelectScreen.mSelectBarScale;
        field_0x364 = g_drawHIO.mLetterSelectScreen.mUnselectBarScale;
        for (int i = 0; i < 6; i++) {
            if (i == mIndex) {
                ((J2DPicture*)field_0x34[i][0])->setBlackWhite(field_0x318[0], field_0x338[0]);
                field_0x124[i][1]->setBlackWhite(field_0x318[3], field_0x338[3]);
                field_0x124[i][3]->setBlackWhite(field_0x318[3], field_0x338[3]);
                mpLetterParent[i]->scale(field_0x360, field_0x360);
            } else {
                mpLetterParent[i]->scale(field_0x364, field_0x364);
            }
        }
        for (int i = 0; i < 2; i++) {
            mpTextParent[i]->paneTrans(g_drawHIO.mLetterSelectScreen.mLetterWindowPosX,
                                       g_drawHIO.mLetterSelectScreen.mLetterWindowPosY);
            if (mProcess == 4 || mProcess == 5 || mProcess == 6) {
                mpTextParent[i]->scale(g_drawHIO.mLetterSelectScreen.mLetterWindowScale,
                                       g_drawHIO.mLetterSelectScreen.mLetterWindowScale);
            }
        }
        if (((mProcess == 4) || (mProcess == 5)) ||
            (mProcess == 6))
        {
            mpBlackTex->setAlpha(g_drawHIO.mLetterSelectScreen.mWindowBGAlpha);
        }
        mpLineParent->paneTrans(g_drawHIO.mLetterSelectScreen.mLetterLinePosX,
                                g_drawHIO.mLetterSelectScreen.mLetterLinePosY);
        for (int i = 0; i < 12; i++) {
            if (field_0x25c[i] != NULL) {
                field_0x25c[i]->setAlpha(g_drawHIO.mLetterSelectScreen.mLetterLineAlpha);
            }
        }
        ((J2DTextBox*)field_0x2ec[1]->getPanePtr())->setBlackWhite(g_drawHIO.mLetterSelectScreen.mLetterTextBack, g_drawHIO.mLetterSelectScreen.mLetterTextFront);
    }
    if (g_drawHIO.mCollectScreen.mButtonDebugON != 0 || i_useHIO) {
        if (mpButtonAB[0] != NULL) {
            mpButtonAB[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonAPosX, g_drawHIO.mCollectScreen.mButtonAPosY);
            mpButtonAB[0]->scale(g_drawHIO.mCollectScreen.mButtonAScale, g_drawHIO.mCollectScreen.mButtonAScale);
        }
        if (mpButtonAB[1] != NULL) {
            mpButtonAB[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBPosX, g_drawHIO.mCollectScreen.mButtonBPosY);
            mpButtonAB[1]->scale(g_drawHIO.mCollectScreen.mButtonBScale, g_drawHIO.mCollectScreen.mButtonBScale);
        }
        if (mpButtonText[0] != NULL) {
            mpButtonText[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonATextPosX, g_drawHIO.mCollectScreen.mButtonATextPosY);
            mpButtonText[0]->scale(g_drawHIO.mCollectScreen.mButtonATextScale, g_drawHIO.mCollectScreen.mButtonATextScale);
        }
        if (mpButtonText[1] != NULL) {
            mpButtonText[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBTextPosX, g_drawHIO.mCollectScreen.mButtonBTextPosY);
            mpButtonText[1]->scale(g_drawHIO.mCollectScreen.mButtonBTextScale, g_drawHIO.mCollectScreen.mButtonBTextScale);
        }
    }
}
