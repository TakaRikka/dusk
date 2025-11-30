/**
 * d_menu_insect.cpp
 * Menu - Insect List
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_insect.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRMemArchive.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "d/d_lib.h"
#include "d/d_select_cursor.h"
#include "d/d_com_inf_game.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_scrn_3select.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "stdio.h"

typedef void (dMenu_Insect_c::*initFunc)();
static initFunc map_init_process[] = {
    &dMenu_Insect_c::wait_init,          &dMenu_Insect_c::explain_open_init,
    &dMenu_Insect_c::explain_move_init,  &dMenu_Insect_c::select_move_init,
    &dMenu_Insect_c::explain_close_init,
};

typedef void (dMenu_Insect_c::*moveFunc)();
static moveFunc map_move_process[] = {
    &dMenu_Insect_c::wait_move,          &dMenu_Insect_c::explain_open_move,
    &dMenu_Insect_c::explain_move_move,  &dMenu_Insect_c::select_move_move,
    &dMenu_Insect_c::explain_close_move,
};

dMenu_Insect_c::dMenu_Insect_c(JKRExpHeap* i_heap, STControl* i_stick, CSTControl* i_cstick,
                               u8 param_3) {
    mpHeap = i_heap;
    mpArchive = NULL;
    mpMount = NULL;
    mpStick = i_stick;
    mpCStick = i_cstick;
    mStatus = 1;
    field_0xf3 = 0;
    field_0xf0 = 0;
    field_0xf4 = 0;
    field_0xf5 = 0;
    field_0xf6 = param_3;
    field_0xf8 = 0xff;
    field_0xf9 = 0xff;
    field_0xfa = 0xff;
    field_0xfb = 0xff;
    field_0xf7 = 1;
    ResTIMG* image = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(image);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xff));
    field_0xfc = 0;
    mpDrawCursor = new dSelect_cursor_c(2, 1.0f, NULL);
    mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.7f, 0.7f);
    mpDrawCursor->setAlphaRate(0.0f);
    mpDrawCursor->setScale(0.0f);
    mpDrawCursor->offPlayAnime(0);
    mpSelect_c = new dMsgScrn3Select_c();
    mpExpItemTex = (ResTIMG*)mpHeap->alloc(0xc00, 0x20);
}

dMenu_Insect_c::~dMenu_Insect_c() {
    delete mpBlackTex;
    mpBlackTex = NULL;

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpSelect_c;
    mpSelect_c = NULL;

    mpHeap->free(mpExpItemTex);
    mpExpItemTex = NULL;

    delete mpString;
    mpString = NULL;

    delete mpScreen;
    mpScreen = NULL;

    delete mpParent;
    mpParent = NULL;

    for (int i = 0; i < MAX_INSECT_NUM; i++) {
        delete mpINSParent[i];
        mpINSParent[i] = NULL;
    }

    delete mpExpScreen;
    mpExpScreen = NULL;

    delete mpExpParent;
    mpExpParent = NULL;

    for (int i = 0; i < 2; i++) {
        delete mpExpSubWin[i];
        mpExpSubWin[i] = NULL;
    }

    delete mpInfoText;
    mpInfoText = NULL;

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

    dComIfGp_getMsgArchive(0)->removeResourceAll();
}

void dMenu_Insect_c::_create() {
    mpString = new dMsgString_c();
    screenSetBase();
    screenSetExplain();
    screenSetDoIcon();
    setHIO(true);
    init();
}

void dMenu_Insect_c::_move() {
    JKRHeap* heap = mDoExt_setCurrentHeap((JKRHeap*)mpHeap);
    u8 process = field_0xf3;
    (this->*map_move_process[field_0xf3])();
    if (process != field_0xf3) {
        (this->*map_init_process[field_0xf3])();
    }
    setHIO(false);
    mDoExt_setCurrentHeap(heap);
}

void dMenu_Insect_c::_draw() {
    if (mpArchive != NULL) {
        J2DGrafContext* grafPort = dComIfGp_getCurrentGrafPort();
        mpBlackTex->setAlpha(0xff);
        mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
        mpScreen->draw(0.0f, 0.0f, grafPort);
        mpDrawCursor->draw();
        field_0xfc = mpExpParent->getAlphaRate() * 150.0f;
        mpBlackTex->setAlpha(field_0xfc);
        mpBlackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, 0, 0, 0);
        mpExpScreen->draw(0.0f, 0.0f, grafPort);
        mpSelect_c->setOffsetX(g_drawHIO.mInsectListScreen.mConfirmOptionPosX_4x3);
        mpSelect_c->translate(g_drawHIO.mInsectListScreen.mConfirmOptionPosX_4x3 + 486.0f,
                              g_drawHIO.mInsectListScreen.mConfirmOptionPosY_4x3 + 209.0f);
        mpSelect_c->draw(0.0f, 0.0f);
        mpIconScreen->draw(0.0f, 0.0f, grafPort);
    }
}

bool dMenu_Insect_c::isSync() {
    if (mpMount != NULL && mpMount->sync() == false) {
        return 0;
    }
    return 1;
}

void dMenu_Insect_c::init() {
    (this->*map_init_process[field_0xf3])();
}

int dMenu_Insect_c::_open() {
    if (mpMount == NULL) {
        mpMount = mDoDvdThd_mountArchive_c::create("/res/Layout/insectRes.arc", 0, NULL);
    }
    if (mpArchive == NULL) {
        if (mpMount->sync() != 0) {
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

    s16 openFrame = g_drawHIO.mInsectListScreen.mOpenFrame;
    s16 closeFrame = g_drawHIO.mInsectListScreen.mCloseFrame;
    field_0xf0 = g_drawHIO.mInsectListScreen.mOpenFrame;
    if (field_0xf0 >= openFrame) {
        field_0xf0 = closeFrame;
        mStatus = 2;
        mpParent->scale(1.0f, 1.0f);
        mpParent->setAlphaRate(1.0f);
        setCursorPos();
        mpDrawCursor->setAlphaRate(1.0f);
        mpDrawCursor->setScale(1.0f);
        mpDrawCursor->onPlayAnime(0);
        return 1;
    } else {
        f32 div = (f32)field_0xf0 / (f32)openFrame;
        mpParent->scale(div, div);
        mpParent->setAlphaRate(div);
        setCursorPos();
        mpDrawCursor->setAlphaRate(div);
        mpDrawCursor->setScale(div);
        return 0;
    }
}

int dMenu_Insect_c::_close() {
    s16 closeFrame = g_drawHIO.mInsectListScreen.mCloseFrame;
    field_0xf0 = 0;
    if (field_0xf0 <= 0) {
        field_0xf0 = 0;
        mStatus = 0;
        mpParent->scale(0.0f, 0.0f);
        mpParent->setAlphaRate(0.0f);
        setCursorPos();
        mpDrawCursor->setAlphaRate(0.0f);
        mpDrawCursor->setScale(0.0f);
        return 1;
    } else {
        f32 div = (f32)field_0xf0 / (f32)closeFrame;
        mpParent->scale(div, div);
        mpParent->setAlphaRate(div);
        setCursorPos();
        mpDrawCursor->setAlphaRate(div);
        mpDrawCursor->setScale(div);
        return 0;
    }
    return field_0xf0 <= 0;
}

void dMenu_Insect_c::wait_init() {
    if (isGetInsect(field_0xf4, field_0xf5) != 0) {
        setAButtonString(0x368);
    } else {
        setAButtonString(0);
    }
    setBButtonString(0x3f9);
}

void dMenu_Insect_c::wait_move() {
    if (mDoGph_gInf_c::getFader()->getStatus() == 1) {
        if (mDoCPd_c::getTrigB(PAD_1) || field_0xf7 == 0) {
            if (mDoCPd_c::getTrigB(PAD_1) && field_0xf6 == 1) {
                dMeter2Info_setInsectSelectType(0);
            }
            mStatus = 3;
            mpDrawCursor->offPlayAnime(0);
        } else if (mDoCPd_c::getTrigA(PAD_1)) {
            if (isGetInsect(field_0xf4, field_0xf5)) {
                field_0xf3 = 1;
                Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                         0);
                dMeter2Info_set2DVibration();
            } else {
                Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }
        } else {
            cursorMove();
        }
    }
}

void dMenu_Insect_c::explain_open_init() {
    char local_78[32];
    char local_98[32];
    char local_b8[32];
    char cStack_d8[32];

    setAButtonString(0);
    setBButtonString(0x3F9);
    u8 insectItemId = getInsectItemID(field_0xf4, field_0xf5);
    s32 i_textureNum = dMeter2Info_readItemTexture(insectItemId, mpExpItemTex, NULL, NULL, NULL,
                                                   NULL, NULL, NULL, NULL, -1);
    f32 tex = mpExpItemTex->width / 48.0f;
    f32 tex2 = mpExpItemTex->height / 48.0f;

    for (int i = 0; i < 2; i++) {
        field_0x54[i]->changeTexture(mpExpItemTex, 0);
        field_0x54[i]->scale(tex, tex2);
    }
    mpString->getString(insectItemId + 0x265, (J2DTextBox*)mpInfoText->getPanePtr(), NULL, NULL,
                        NULL, 0);
    if (field_0xf6 == 1) {
        if (!isGiveInsect(field_0xf4, field_0xf5)) {
            mpString->getString(0x5BB, field_0x5c, NULL, NULL, NULL, 0);
            local_78[0] = 0;
            local_98[0] = 0;
            local_b8[0] = 0;
            cStack_d8[1] = 0;
            field_0xf7 = 0;
            dMeter2Info_getString(0x4BD, local_78, NULL);
            dMeter2Info_getString(0x4BE, local_98, NULL);
            f32 stringLength1 =
                dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                            mpSelect_c->getCharSpace(), local_78);
            f32 stringLength2 =
                dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                            mpSelect_c->getCharSpace(), local_98);
            f32 length;
            if (stringLength1 < stringLength2) {
                length = stringLength2;
            } else {
                length = stringLength1;
            }
            f32 textBoxWidth = mpSelect_c->getTextBoxWidth();
            if (length < textBoxWidth) {
                length = mpSelect_c->getTextBoxWidth();
            }

            snprintf(local_b8, 32,
                     "\x1B"
                     "CR[%d]",
                     (int)(0.5f * (length - stringLength1)));
            strcat(local_b8, local_78);
            snprintf(cStack_d8, 32,
                     "\x1B"
                     "CR[%d]",
                     (int)(0.5f * (length - stringLength2)));
            strcat(cStack_d8, local_98);
            mpSelect_c->setString("", local_b8, cStack_d8);
            mpSelect_c->setRubyString("", "", "");
            mpSelect_c->selAnimeInit(2, field_0xf7 + 1, 0, length, 0);
        } else {
            mpString->getString(0x5BC, field_0x5c, NULL, NULL, NULL, 0);
        }
    }
    mpExpParent->alphaAnimeStart(0);
}

void dMenu_Insect_c::explain_open_move() {
    if (mpExpParent->alphaAnime(10, 0, 0xff, 1)) {
        if (field_0xf6 == 1 && isGiveInsect(field_0xf4, field_0xf5) == 0) {
            field_0xf3 = 3;
        } else {
            field_0xf3 = 2;
        }
    }
}

void dMenu_Insect_c::explain_move_init() {
    /* empty function */
}

void dMenu_Insect_c::explain_move_move() {
    if (mDoCPd_c::getTrigA(PAD_1) == 0 && mDoCPd_c::getTrigB(PAD_1) == 0) {
        return;
    }
    field_0xf3 = 4;
}

void dMenu_Insect_c::select_move_init() {
    dMeter2Info_setInsectSelectType(0xff);
    setAButtonString(0x40c);
    setBButtonString(0x3f9);
}

void dMenu_Insect_c::select_move_move() {
    mpStick->checkTrigger();
    if (mDoCPd_c::getTrigA(PAD_1)) {
        field_0xf3 = 4;
        if (field_0xf7 == 0) {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_OK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibration();
        } else {
            Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }
    } else if (mDoCPd_c::getTrigB(PAD_1)) {
        field_0xf3 = 4;
        field_0xf7 = 1;
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_CANCEL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_set2DVibration();
    } else if (mpSelect_c->isSelect() && mpStick->checkUpTrigger()) {
        if (field_0xf7 == 1) {
            field_0xf7 = 0;
            Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
        }

    } else if (mpSelect_c->isSelect() && mpStick->checkDownTrigger()) {
        if (field_0xf7 == 0) {
            field_0xf7 = 1;
            Z2GetAudioMgr()->seStart(Z2SE_SY_MENU_CURSOR_COMMON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                     -1.0f, 0);
        }
    }
    if (field_0xf3 == 3) {
        mpSelect_c->selAnimeMove(2, field_0xf7 + 1, false);
    } else {
        mpSelect_c->selAnimeEnd();
    }
}

void dMenu_Insect_c::explain_close_init() {
    setAButtonString(0);
    setBButtonString(0x3f9);
    if (field_0xf6 == 1) {
        dMeter2Info_setInsectSelectType(getInsectItemID(field_0xf4, field_0xf5));
    }
    Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    dMeter2Info_set2DVibration();
    mpExpParent->alphaAnimeStart(0);
}

void dMenu_Insect_c::explain_close_move() {
    bool alphaAnime = mpExpParent->alphaAnime(10, 0xff, 0, 1);
    bool cVar2;
    if (field_0xf6 == 1 && isGiveInsect(field_0xf4, field_0xf5) == 0) {
        cVar2 = mpSelect_c->selAnimeEnd();
    } else {
        cVar2 = 1;
    }
    if (alphaAnime && cVar2 != 0) {
        field_0xf3 = 0;
    }
}

void dMenu_Insect_c::screenSetBase() {
    static const u64 insect_tag[MAX_INSECT_NUM] = {
        'ari_os',  'ari_ms',   'kag_os',  'kag_ms',   'kab_os', 'kab_ms',  'kam_os',  'kam_mes',
        'kuwa_os', 'kuwa_mes', 'dan_os',  'dan_mes',  'cho_os', 'cho_ms',  'tent_os', 'tent_mes',
        'kata_os', 'kata_mes', 'nana_os', 'nana_mes', 'bat_os', 'bat_mes', 'tonb_os', 'tonb_mes',
    };
    static const u64 ageha_tag[MAX_INSECT_NUM] = {
        'ageha00', 'ageha01', 'ageha02', 'ageha03', 'ageha04', 'ageha05', 'ageha06', 'ageha07',
        'ageha08', 'ageha09', 'ageha10', 'ageha11', 'ageha12', 'ageha13', 'ageha14', 'ageha15',
        'ageha16', 'ageha17', 'ageha18', 'ageha19', 'ageha20', 'ageha21', 'ageha22', 'ageha23',
    };

    mpScreen = new J2DScreen();
    mpScreen->setPriority("zelda_gold_insects.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpScreen);
    mpParent = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
    mpParent->setAlphaRate(0.0f);
    for (int i = 0; i < MAX_INSECT_NUM; i++) {
        mpINSParent[i] = new CPaneMgr(mpScreen, insect_tag[i], 0, NULL);
    }
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            int iterateSum = i + j * 6;
            if (isGetInsect(i, j) != 0) {
                mpINSParent[iterateSum]->show();
            } else {
                mpINSParent[iterateSum]->hide();
            }
            if (isGiveInsect(i, j) != 0) {
                mpScreen->search(ageha_tag[iterateSum])->show();
            } else {
                mpScreen->search(ageha_tag[iterateSum])->hide();
            }
        }
    }
#if VERSION == VERSION_GCN_JPN
    J2DTextBox* textBox = (J2DTextBox*)mpScreen->search('t_t00');
    mpScreen->search('f_t00')->hide();
#else
    J2DTextBox* textBox = (J2DTextBox*)mpScreen->search('f_t00');
    mpScreen->search('t_t00')->hide();
#endif
    textBox->setFont(mDoExt_getSubFont());
    textBox->setString(0x200, "");
    mpString->getString(0x5ba, textBox, NULL, NULL, NULL, 0);
}

void dMenu_Insect_c::screenSetExplain() {
    mpExpScreen = new J2DScreen();
    mpExpScreen->setPriority("zelda_gold_insects_info.blo", 0x20000, mpArchive);
    dPaneClass_showNullPane(mpExpScreen);
    mpExpParent = new CPaneMgr(mpExpScreen, 'n_all', 2, NULL);
    mpExpParent->setAlphaRate(0.0f);
    mpExpSubWin[0] = new CPaneMgr(mpExpScreen, 'in_win_n', 0, NULL);
    mpExpSubWin[1] = new CPaneMgr(mpExpScreen, 'w_d_mo_n', 0, NULL);
    if (field_0xf6 == 0) {
        mpExpSubWin[1]->hide();
    }
#if VERSION == VERSION_GCN_JPN
    mpInfoText = new CPaneMgr(mpExpScreen, 'mg_3line', 0, NULL);
    mpExpScreen->search('n_e4line')->hide();
    field_0x5c = (J2DTextBox*)mpExpScreen->search('w_msg_jp');
    mpExpScreen->search('ms_for_2')->hide();
#else
    mpInfoText = new CPaneMgr(mpExpScreen, 'mg_e4lin', 0, NULL);
    mpExpScreen->search('n_3line')->hide();
    field_0x5c = (J2DTextBox*)mpExpScreen->search('ms_for_2');
    mpExpScreen->search('w_msg_jp')->hide();
#endif
    mpExpScreen->search('ms_for_3')->hide();
    field_0x54[0] = (J2DPicture*)mpExpScreen->search('insects');
    field_0x54[1] = (J2DPicture*)mpExpScreen->search('insectss');
    J2DTextBox* textBox = (J2DTextBox*)mpInfoText->getPanePtr();
    textBox->setFont(mDoExt_getMesgFont());
    J2DTextBox* textBox2 = (J2DTextBox*)mpInfoText->getPanePtr();
    textBox2->setString(0x200, "");
    field_0x5c->setFont(mDoExt_getMesgFont());
    field_0x5c->setString(0x100, "");
}

void dMenu_Insect_c::screenSetDoIcon() {
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

u8 dMenu_Insect_c::getGetInsectNum() {
    static u8 l_itemno[MAX_INSECT_NUM] = {
        fpcNm_ITEM_M_BEETLE,      fpcNm_ITEM_F_BEETLE,      fpcNm_ITEM_M_BUTTERFLY, fpcNm_ITEM_F_BUTTERFLY, fpcNm_ITEM_M_STAG_BEETLE, fpcNm_ITEM_F_STAG_BEETLE,
        fpcNm_ITEM_M_GRASSHOPPER, fpcNm_ITEM_F_GRASSHOPPER, fpcNm_ITEM_M_NANAFUSHI, fpcNm_ITEM_F_NANAFUSHI, fpcNm_ITEM_M_DANGOMUSHI,  fpcNm_ITEM_F_DANGOMUSHI,
        fpcNm_ITEM_M_MANTIS,      fpcNm_ITEM_F_MANTIS,      fpcNm_ITEM_M_LADYBUG,   fpcNm_ITEM_F_LADYBUG,   fpcNm_ITEM_M_SNAIL,       fpcNm_ITEM_F_SNAIL,
        fpcNm_ITEM_M_DRAGONFLY,   fpcNm_ITEM_F_DRAGONFLY,   fpcNm_ITEM_M_ANT,       fpcNm_ITEM_F_ANT,       fpcNm_ITEM_M_MAYFLY,      fpcNm_ITEM_F_MAYFLY,
    };

    u8 insectNum = 0;
    u8* insectList = l_itemno;
    for (u32 i = 0; i < MAX_INSECT_NUM; i++) {
        u8 insectId = *insectList;
        insectList++;

        if (dComIfGs_isItemFirstBit(insectId) != 0) {
            insectNum++;
        }
    }
    return insectNum;
}

u8 dMenu_Insect_c::getInsectItemID(int param_0, int param_1) {
    static u8 l_itemno[MAX_INSECT_NUM] = {
        fpcNm_ITEM_M_ANT,       fpcNm_ITEM_F_ANT,       fpcNm_ITEM_M_MAYFLY,      fpcNm_ITEM_F_MAYFLY,      fpcNm_ITEM_M_BEETLE,     fpcNm_ITEM_F_BEETLE,
        fpcNm_ITEM_M_MANTIS,    fpcNm_ITEM_F_MANTIS,    fpcNm_ITEM_M_STAG_BEETLE, fpcNm_ITEM_F_STAG_BEETLE, fpcNm_ITEM_M_DANGOMUSHI, fpcNm_ITEM_F_DANGOMUSHI,
        fpcNm_ITEM_M_BUTTERFLY, fpcNm_ITEM_F_BUTTERFLY, fpcNm_ITEM_M_LADYBUG,     fpcNm_ITEM_F_LADYBUG,     fpcNm_ITEM_M_SNAIL,      fpcNm_ITEM_F_SNAIL,
        fpcNm_ITEM_M_NANAFUSHI, fpcNm_ITEM_F_NANAFUSHI, fpcNm_ITEM_M_GRASSHOPPER, fpcNm_ITEM_F_GRASSHOPPER, fpcNm_ITEM_M_DRAGONFLY,  fpcNm_ITEM_F_DRAGONFLY,
    };

    int index = param_0 + param_1 * 6;
    return l_itemno[index];
}

bool dMenu_Insect_c::isGetInsect(int param_0, int param_1) {
    return dComIfGs_isItemFirstBit(getInsectItemID(param_0, param_1)) != 0;
}

bool dMenu_Insect_c::isGiveInsect(int param_0, int param_1) {
    static const u32 i_evtID[MAX_INSECT_NUM] = {
        0x1A5, /* dSv_event_flag_c::F_0421 - Misc. - Ant (M) */
        0x1A6, /* dSv_event_flag_c::F_0422 - Misc. - Ant (F) */
        0x1A7, /* dSv_event_flag_c::F_0423 - Misc. - Dayfly (M) */
        0x1A8, /* dSv_event_flag_c::F_0424 - Misc. - Dayfly (F) */
        0x191, /* dSv_event_flag_c::F_0401 - Misc. - Beetle (M) */
        0x192, /* dSv_event_flag_c::F_0402 - Misc. - Beetle (F) */
        0x19D, /* dSv_event_flag_c::F_0413 - Misc. - Mantis (M) */
        0x19E, /* dSv_event_flag_c::F_0414 - Misc. - Mantis (F) */
        0x195, /* dSv_event_flag_c::F_0405 - Misc. - Stag beetle (M) */
        0x196, /* dSv_event_flag_c::F_0406 - Misc. - Stag beetle (F) */
        0x19B, /* dSv_event_flag_c::F_0411 - Misc. - Pill bug (M) */
        0x19C, /* dSv_event_flag_c::F_0412 - Misc. - Pill bug (F) */
        0x193, /* dSv_event_flag_c::F_0403 - Misc. - Butterfly (M) */
        0x194, /* dSv_event_flag_c::F_0404 - Misc. - Butterfly (F) */
        0x19F, /* dSv_event_flag_c::F_0415 - Misc. - Ladybug (M) */
        0x1A0, /* dSv_event_flag_c::F_0416 - Misc. - Ladybug (F) */
        0x1A1, /* dSv_event_flag_c::F_0417 - Misc. - Snail (M) */
        0x1A2, /* dSv_event_flag_c::F_0418 - Misc. - Snail (F) */
        0x199, /* dSv_event_flag_c::F_0409 - Misc. - Phasmid (M) */
        0x19A, /* dSv_event_flag_c::F_0410 - Misc. - Phasmid (F) */
        0x197, /* dSv_event_flag_c::F_0407 - Misc. - Grasshopper (M) */
        0x198, /* dSv_event_flag_c::F_0408 - Misc. - Grasshopper (F) */
        0x1A3, /* dSv_event_flag_c::F_0419 - Misc. - Dragonfly (M) */
        0x1A4, /*dSv_event_flag_c::F_0420 - Misc. - Dragonfly (F) */
    };

    return dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[i_evtID[param_0 + param_1 * 6]]) != 0;
}

bool dMenu_Insect_c::isCatchInsect(u8 i_insectID) {
    if (i_insectID >= fpcNm_ITEM_M_BEETLE && i_insectID <= fpcNm_ITEM_F_MAYFLY) {
        return dComIfGs_isItemFirstBit(i_insectID);
    }
    return false;
}

bool dMenu_Insect_c::isGiveInsect(u8 i_insectID) {
    if (i_insectID >= fpcNm_ITEM_M_BEETLE && i_insectID <= fpcNm_ITEM_F_MAYFLY) {
        int label = i_insectID + 0xd1;
        return dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[label]);
    }
    return false;
}

bool dMenu_Insect_c::isCatchNotGiveInsect(u8 param_0) {
    if (isCatchInsect(param_0) != 0 && isGiveInsect(param_0) == 0) {
        return 1;
    }
    return 0;
}

void dMenu_Insect_c::cursorMove() {
    u8 oldF4 = field_0xf4;
    u8 oldF5 = field_0xf5;
    u8 dpd = 0;
    mpStick->checkTrigger();
    if (mpStick->checkRightTrigger()) {
        if (field_0xf4 < 5) {
            field_0xf4++;
        }
    } else if (mpStick->checkLeftTrigger()) {
        if (field_0xf4 != 0) {
            field_0xf4--;
        }
    }
    if (mpStick->checkUpTrigger()) {
        if (field_0xf5 != 0) {
            field_0xf5--;
        }
    } else if (mpStick->checkDownTrigger()) {
        if (field_0xf5 < 3) {
            field_0xf5++;
        }
    }
    if (oldF4 == field_0xf4 && oldF5 == field_0xf5) {
        field_0xf8 = 0xff;
        field_0xf9 = 0xff;
        if (mpStick->getValueStick() <= 0.75f) {
            dpd = dpdMove();
        }
        if (field_0xf8 != 0xff && field_0xf9 != 0xff) {
            if (field_0xf8 != field_0xfa || field_0xf9 != field_0xfb) {
                if (field_0xf8 != field_0xf4 || field_0xf9 != field_0xf5) {
                    field_0xf4 = field_0xf8;
                    field_0xf5 = field_0xf9;
                    field_0xfa = field_0xf8;
                    field_0xfb = field_0xf9;
                }
            }
        }
    }
    if (oldF4 == field_0xf4 && oldF5 == field_0xf5) {
        return;
    }
    setCursorPos();
    if (isGetInsect(field_0xf4, field_0xf5) != 0) {
        setAButtonString(0x368);
    } else {
        setAButtonString(0);
    }
    if (dpd != 0) {
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 1, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_set2DVibration();
    } else {
        Z2GetAudioMgr()->seStart(Z2SE_SY_CURSOR_ITEM, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
}

void dMenu_Insect_c::setCursorPos() {
    int index = field_0xf4 + field_0xf5 * 6;
    Vec pos = mpINSParent[index]->getGlobalVtxCenter(false, 0);
    mpDrawCursor->setPos(pos.x, pos.y, mpINSParent[index]->getPanePtr(), false);
    for (int i = 0; i < MAX_INSECT_NUM; i++) {
        if (i == index) {
            mpINSParent[i]->scale(g_drawHIO.mInsectListScreen.mSelectInsectScale,
                                  g_drawHIO.mInsectListScreen.mSelectInsectScale);
        } else {
            mpINSParent[i]->scale(g_drawHIO.mInsectListScreen.mUnselectInsectScale,
                                  g_drawHIO.mInsectListScreen.mUnselectInsectScale);
        }
    }
}

u8 dMenu_Insect_c::dpdMove() {
    return 0;
}

void dMenu_Insect_c::setAButtonString(u16 i_stringID) {
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

void dMenu_Insect_c::setBButtonString(u16 i_stringID) {
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

void dMenu_Insect_c::setHIO(bool i_useHIO) {
    if (i_useHIO || g_drawHIO.mInsectListScreen.mDebugON) {
        mpExpSubWin[0]->paneTrans(g_drawHIO.mInsectListScreen.mDescWindowPosX,
                                  g_drawHIO.mInsectListScreen.mDescWindowPosY);
        mpExpSubWin[1]->paneTrans(g_drawHIO.mInsectListScreen.mGiveOptionPosX_4x3,
                                  g_drawHIO.mInsectListScreen.mGiveOptionPosY_4x3);
    }
    if (g_drawHIO.mCollectScreen.mButtonDebugON != false || i_useHIO) {
        if (mpButtonAB[0]) {
            mpButtonAB[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonAPosX,
                                     g_drawHIO.mCollectScreen.mButtonAPosY);
            mpButtonAB[0]->scale(g_drawHIO.mCollectScreen.mButtonAScale,
                                 g_drawHIO.mCollectScreen.mButtonAScale);
        }
        if (mpButtonAB[1]) {
            mpButtonAB[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBPosX,
                                     g_drawHIO.mCollectScreen.mButtonBPosY);
            mpButtonAB[1]->scale(g_drawHIO.mCollectScreen.mButtonBScale,
                                 g_drawHIO.mCollectScreen.mButtonBScale);
        }
        if (mpButtonText[0]) {
            mpButtonText[0]->paneTrans(g_drawHIO.mCollectScreen.mButtonATextPosX,
                                       g_drawHIO.mCollectScreen.mButtonATextPosY);
            mpButtonText[0]->scale(g_drawHIO.mCollectScreen.mButtonATextScale,
                                   g_drawHIO.mCollectScreen.mButtonATextScale);
        }
        if (mpButtonText[1]) {
            mpButtonText[1]->paneTrans(g_drawHIO.mCollectScreen.mButtonBTextPosX,
                                       g_drawHIO.mCollectScreen.mButtonBTextPosY);
            mpButtonText[1]->scale(g_drawHIO.mCollectScreen.mButtonBTextScale,
                                   g_drawHIO.mCollectScreen.mButtonBTextScale);
        }
    }
}
