/**
 * d_menu_item_explain.cpp
 * Menu - Item Explain
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_item_explain.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "stdio.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_item.h"
#include "d/d_kantera_icon_meter.h"
#include "d/d_lib.h"
#include "d/d_select_cursor.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "d/d_msg_scrn_3select.h"
#include "d/d_msg_scrn_arrow.h"

typedef void (dMenu_ItemExplain_c::*initFunc)();
static initFunc init_process[] = {
    &dMenu_ItemExplain_c::wait_init,      &dMenu_ItemExplain_c::open_init,
    &dMenu_ItemExplain_c::move_init,      &dMenu_ItemExplain_c::move_select_init,
    &dMenu_ItemExplain_c::move_next_init, &dMenu_ItemExplain_c::close_init,
};

typedef void (dMenu_ItemExplain_c::*moveFunc)();
static moveFunc move_process[] = {
    &dMenu_ItemExplain_c::wait_proc,      &dMenu_ItemExplain_c::open_proc,
    &dMenu_ItemExplain_c::move_proc,      &dMenu_ItemExplain_c::move_select_proc,
    &dMenu_ItemExplain_c::move_next_proc, &dMenu_ItemExplain_c::close_proc,
};

dMenu_ItemExplain_c::dMenu_ItemExplain_c(JKRExpHeap* i_heap, JKRArchive* i_archive,
                                         STControl* i_stick, bool param_3) {
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
    
    mpHeap = i_heap;
    mpArchive = dComIfGp_getDemoMsgArchive();
    mpStick = i_stick;
    mpHeap->getTotalFreeSize();
    field_0xc8 = 0;
    field_0xcc = 0;
    field_0xd0 = 0;
    field_0xc4 = 0;
    field_0xe1 = 0;
    field_0xe2 = 0;
    mStatus = 0;
    field_0xe7 = 0;
    mEndButton = 0;
    if (i_archive == dComIfGp_getDmapResArchive()) {
        field_0xe5 = 2;
    } else {
        field_0xe5 = 1;
    }
    field_0xe6 = 0;
    field_0xdf = 0;
    field_0xde = 0;
    offForceButton();
    trans(0.0f, 0.0f);
    field_0x74 = 0;
    field_0x14 = 0;
    for (int i = 0; i < 4; i++) {
        field_0x28[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
        field_0x44[i] = 0;
        field_0xa0[i] = 0;
    }
    field_0x50 = 0;
    field_0x5c = 0;
    for (int i = 0; i < 3; i++) {
        field_0x60[i] = 0;
    }
    field_0xdc = 0;
    mpInfoString = new dMsgString_c(1);
    mpKanteraMeter = new dKantera_icon_c();
    mpInfoScreen = new J2DScreen();
    mpInfoScreen->setPriority("zelda_item_screen_info.blo", 0x20000, mpArchive);
    mpParent[0] = new CPaneMgr(mpInfoScreen, 'n_all', 2, NULL);
    mpParent[1] = NULL;
    mpLabel = new CPaneMgr(mpInfoScreen, 'label_n', 0, NULL);
    mDescAlpha = 0.0f;
    field_0x78 = 0;
    mAlphaRatio = 201.0f;
#if VERSION == VERSION_GCN_JPN
    mpInfoText = new CPaneMgr(mpInfoScreen, 'i_text4', 0, NULL);
    mpInfoScreen->search('i_text1')->hide();
#else
    mpInfoText = new CPaneMgr(mpInfoScreen, 'i_text1', 0, NULL);
    mpInfoScreen->search('i_text4')->hide();
#endif
    ((J2DTextBox*)(mpInfoText->getPanePtr()))->setFont(mDoExt_getMesgFont());
    ((J2DTextBox*)(mpInfoText->getPanePtr()))->setString(0x200, "");
    mpInfoText->show();
    for (int i = 0; i < 4; i++) {
#if VERSION == VERSION_GCN_JPN
        mpNameText[i] = new CPaneMgr(mpInfoScreen, name_tag[i], 0, NULL);
        mpInfoScreen->search(fame_tag[i])->hide();
#else
        mpNameText[i] = new CPaneMgr(mpInfoScreen, fame_tag[i], 0, NULL);
        mpInfoScreen->search(name_tag[i])->hide();
#endif
        ((J2DTextBox*)(mpNameText[i]->getPanePtr()))->setFont(mDoExt_getMesgFont());
        ((J2DTextBox*)(mpNameText[i]->getPanePtr()))->setString(0x20, "");
    }
    mpInfoIcon = new CPaneMgr(mpInfoScreen, 'i_icon_p', 0, NULL);
    for (int i = 0; i < 4; i++) {
        mpExpItemTex[i] = (ResTIMG*)mpHeap->alloc(0xC00, 0x20);
    }

    mpExpItemPane[0] = new J2DPicture(
        'i_icon_1',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpInfoIcon->getInitSizeX(), mpInfoIcon->getInitSizeY()),
        ((J2DPicture*)(mpInfoIcon->getPanePtr()))->getTexture(0)->getTexInfo(), NULL);
    mpExpItemPane[0]->setBasePosition(J2DBasePosition_4);
    mpInfoIcon->getPanePtr()->appendChild(mpExpItemPane[0]);
    mpExpItemPane[1] = new J2DPicture(
        'i_icon_2',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpInfoIcon->getInitSizeX(), mpInfoIcon->getInitSizeY()),
        ((J2DPicture*)(mpInfoIcon->getPanePtr()))->getTexture(0)->getTexInfo(), NULL);
    mpExpItemPane[1]->setBasePosition(J2DBasePosition_4);
    mpInfoIcon->getPanePtr()->appendChild(mpExpItemPane[1]);
    mpExpItemPane[2] = new J2DPicture(
        'i_icon_3',
        JGeometry::TBox2<f32>(0.0f, 0.0f, mpInfoIcon->getInitSizeX(), mpInfoIcon->getInitSizeY()),
        ((J2DPicture*)(mpInfoIcon->getPanePtr()))->getTexture(0)->getTexInfo(), NULL);
    mpExpItemPane[2]->setBasePosition(J2DBasePosition_4);
    mpInfoIcon->getPanePtr()->appendChild(mpExpItemPane[2]);

    ResTIMG* texResource = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
        'TIMG', dMeter2Info_getNumberTextureName(0));
    for (int i = 0; i < 3; i++) {
        mpItemNumTex[i] = new J2DPicture(texResource);
        mpInfoScreen->search('info_n1')->appendChild(mpItemNumTex[i]);
        mpItemNumTex[i]->move(i * 16.0f + (mpInfoIcon->getPosX() + mpInfoIcon->getSizeX() * 0.5f),
                              mpInfoIcon->getPosY() + mpInfoIcon->getSizeY() - 3.0f);
        mpItemNumTex[i]->resize(16.0f, 16.0f);
    }

    if (param_3 == 1) {
        texResource = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
        mpBackTex = new J2DPicture(texResource);
        mpBackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xff));
        mpBackTex->setAlpha(0);
    } else {
        mpBackTex = NULL;
    }
    mpArrow = NULL;
    if (i_stick != NULL) {
        mpSelect_c = NULL;
    } else {
        mpSelect_c = NULL;
    }
}

dMenu_ItemExplain_c::~dMenu_ItemExplain_c() {
    delete mpInfoString;
    mpInfoString = NULL;

    delete mpInfoScreen;
    mpInfoScreen = NULL;

    delete mpKanteraMeter;
    mpKanteraMeter = NULL;

    for (int i = 0; i < 2; i++) {
        if (mpParent[i] != NULL) {
            delete mpParent[i];
            mpParent[i] = NULL;
        }
    }

    delete mpLabel;
    mpLabel = NULL;

    delete mpInfoText;
    mpInfoText = NULL;

    for (int i = 0; i < 4; i++) {
        delete mpNameText[i];
        mpNameText[i] = NULL;
    }

    if (field_0x74 != NULL) {
        delete field_0x74;
        field_0x74 = NULL;
    }

    for (int i = 0; i < 2; i++) {
        if (field_0x44[i] != NULL) {
            delete field_0x44[i];
            field_0x44[i] = NULL;
        }
    }

    if (field_0x50 != NULL) {
        delete field_0x50;
        field_0x50 = NULL;
    }

    if (field_0x5c != NULL) {
        delete field_0x5c;
        field_0x5c = NULL;
    }

    if (field_0x14 != NULL) {
        delete field_0x14;
        field_0x14 = NULL;
    }

    for (int i = 0; i < 4; i++) {
        if (field_0x28[i] != NULL) {
            delete field_0x28[i];
            field_0x28[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (field_0x60[i] != NULL) {
            delete field_0x60[i];
            field_0x60[i] = NULL;
        }
    }

    delete mpInfoIcon;
    mpInfoIcon = NULL;

    for (int i = 0; i < 4; i++) {
        if (mpExpItemTex[i] != NULL) {
            mpHeap->free(mpExpItemTex[i]);
            mpExpItemTex[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mpExpItemPane[i] != NULL) {
            mpExpItemPane[i] = NULL;
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mpItemNumTex[i] != NULL) {
            mpItemNumTex[i] = NULL;
        }
    }

    if (mpBackTex != NULL) {
        delete mpBackTex;
        mpBackTex = NULL;
    }

    if (mpArrow != NULL) {
        delete mpArrow;
        mpArrow = NULL;
    }

    if (mpSelect_c != NULL) {
        delete mpSelect_c;
        mpSelect_c = NULL;
    }

    dComIfGp_getMsgArchive(0)->removeResourceAll();
    mpArchive->removeResourceAll();
}

void dMenu_ItemExplain_c::move() {
    u8 status = mStatus;
    (this->*move_process[status])();
    if (status != mStatus) {
        (this->*init_process[mStatus])();
    }
    if (mStatus != NULL) {
        mpInfoScreen->animation();
    }
    if (g_ringHIO.mItemDescAlpha != mDescAlpha) {
        mDescAlpha = g_ringHIO.mItemDescAlpha;
    }
}

void dMenu_ItemExplain_c::draw(J2DOrthoGraph* i_graph) {
    if (mStatus != 0) {
        for (int i = 0; i < 2; i++) {
            if (mpParent[i] != NULL) {
                mpParent[i]->scale(g_ringHIO.mItemDescScale, g_ringHIO.mItemDescScale);
                mpParent[i]->paneTrans(field_0xd4 + g_ringHIO.mItemDescPosX,
                                       field_0xd8 + g_ringHIO.mItemDescPosY);
            }
        }
        mpLabel->scale(g_ringHIO.mItemDescTitleScale, g_ringHIO.mItemDescTitleScale);
        mpLabel->paneTrans(g_ringHIO.mItemDescTitlePosX, g_ringHIO.mItemDescTitlePosY);
        if (mpBackTex != NULL) {
            mpBackTex->draw(0.0f, 0.0f, FB_WIDTH, FB_HEIGHT, false, false, false);
        }
        if (field_0xc8 != field_0xd0) {
            field_0xd0 = field_0xc8;
            for (int i = 0; i < 4; i++) {
                mpInfoString->getString(field_0xcc, (J2DTextBox*)mpNameText[i]->getPanePtr(), NULL,
                                        NULL, NULL, 0);
            }
            mpInfoString->getString(field_0xc8, (J2DTextBox*)mpInfoText->getPanePtr(), NULL, NULL,
                                    NULL, 0);
        }
        mpInfoScreen->draw(0.0f, 0.0f, (J2DGrafContext*)i_graph);
        mpInfoString->drawOutFontLocal((J2DTextBox*)mpInfoText->getPanePtr(), -1.0f);
        drawKantera();
        if (mpSelect_c != NULL) {
            mpSelect_c->translate(486.0f, 209.0f);
            mpSelect_c->draw(0.0f, 0.0f);
        }
        if (mpArrow != NULL) {
            mpArrow->draw();
        }
    }
}

void dMenu_ItemExplain_c::drawKantera() {
    if (field_0xe1 == 0x48) {
        mpKanteraMeter->setPos(mpInfoIcon->getGlobalPosX() + 48.0f,
                               mpInfoIcon->getGlobalPosY() + mpInfoIcon->getSizeY() + 5.0f);
        mpKanteraMeter->setScale(1.0f, 1.0f);
        mpKanteraMeter->setNowGauge(dComIfGs_getMaxOil(), dComIfGs_getOil());
        mpKanteraMeter->setAlphaRate(mpInfoIcon->getAlphaRate());
        mpKanteraMeter->drawSelf();
    }
}

void dMenu_ItemExplain_c::wait_init() {
    /* empty function */
}

void dMenu_ItemExplain_c::wait_proc() {
    /* empty function */
}

void dMenu_ItemExplain_c::open_init() {
    field_0xc4 = field_0xc8;
    mEndButton = 0;
    offForceButton();
    if (field_0xe1 != 0xff) {
        u8 itemNo = field_0xe1;
        s32 iVar = -1;
        if (itemNo == 0x26) {
            if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x11) {
                s32 itemsObtained = 0;
                for (int i = 0; i < 3; i++) {
                    if (checkItemGet(i + fpcNm_ITEM_L2_KEY_PIECES1, 1) != 0) {
                        itemsObtained++;
                    }
                }
                if (itemsObtained == 1) {
                    iVar = 0x43;
                }
                if (itemsObtained == 2) {
                    itemNo = 0xfa;
                    iVar = 0x44;
                } else {
                    itemNo = 0xfd;
                }
            } else {
                if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x14) {
                    itemNo = 0xf6;
                }
            }
        }
        s32 texture = dMeter2Info_readItemTexture(
            itemNo, mpExpItemTex[0], (J2DPicture*)mpInfoIcon->getPanePtr(), mpExpItemTex[1],
            mpExpItemPane[0], mpExpItemTex[2], mpExpItemPane[1], mpExpItemTex[3], mpExpItemPane[2],
            iVar);
        mpInfoIcon->show();
        for (int i = 0; i < 3; i++) {
            if (i < texture - 1) {
                mpExpItemPane[i]->show();
            } else {
                mpExpItemPane[i]->hide();
            }
        }
        setNumber();
        mpInfoScreen->search('i_i_back')->show();
        field_0xe6 = 0;
    } else {
        mpInfoIcon->hide();
        for (int i = 0; i < 3; i++) {
            mpExpItemPane[i]->hide();
        }
        setNumber();
        mpInfoScreen->search('i_i_back')->hide();
        field_0xe6 = 1;
    }
    if (mpArrow != NULL) {
        mpArrow->arwAnimeInit();
    }
    Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_OPEN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_ItemExplain_c::open_proc() {
    mAlphaRatio += 2.0f;
    if (mAlphaRatio >= 201.0f) {
        mAlphaRatio = 201.0f;
        mStatus = 2;
    }
    if (mpBackTex != NULL) {
        mpBackTex->setAlpha((1.0f - getAlphaRatio()) * 150.0f);
    }
}

void dMenu_ItemExplain_c::move_init() {
    if (mpArrow != NULL) {
        mpArrow->arwAnimeInit();
    }
}

void dMenu_ItemExplain_c::move_proc() {
    if (field_0xe1 == 0x25 && field_0xe5 == 2 && mpArrow != NULL) {
        mpArrow->arwAnimeMove();
    }
    if (!mDoCPd_c::getTrigA(PAD_1) && !mDoCPd_c::getTrigB(PAD_1) && mForceButton == 0) {
        if (field_0xe7 == 0) {
            return;
        }
        if (!mDoCPd_c::getTrigX(PAD_1) && !mDoCPd_c::getTrigY(PAD_1)) {
            return;
        }
    }
    if (mForceButton != 0) {
        mEndButton = 6;
    } else if (mDoCPd_c::getTrigA(PAD_1)) {
        mEndButton = 1;
    } else if (mDoCPd_c::getTrigB(PAD_1)) {
        mEndButton = 2;
    } else if (mDoCPd_c::getTrigX(PAD_1)) {
        mEndButton = 3;
    } else if (mDoCPd_c::getTrigY(PAD_1)) {
        mEndButton = 4;
    }
    mStatus = 5;
    Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    dMeter2Info_set2DVibration();
}

void dMenu_ItemExplain_c::move_select_init() {
    char local_88[8];
    char local_80[8];
    char local_64[20];
    char cStack78[20];

    local_88[0] = 0;
    local_80[0] = 0;
    local_64[0] = 0;
    cStack78[1] = 0;

    if (getWarpMarkFlag() == 0) {
        field_0xc8 = 0x517;
        field_0xe2 = 0;
        dMeter2Info_getString(0x518, local_88, NULL);
        dMeter2Info_getString(0x519, local_80, NULL);
    } else {
        field_0xc8 = 0x51A;
        field_0xe2 = 0;
        dMeter2Info_getString(0x51B, local_88, NULL);
        dMeter2Info_getString(0x51C, local_80, NULL);
    }

    f32 stringLength1 = dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                                    mpSelect_c->getCharSpace(), local_88);
    f32 stringLength2 = dMeter2Info_getStringLength(mDoExt_getMesgFont(), mpSelect_c->getFontSize(),
                                                    mpSelect_c->getCharSpace(), local_80);
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

    snprintf(local_64, 20,
             "\x1B"
             "CR[%d]",
             (int)(0.5f * (length - stringLength1)));
    strcat(local_64, local_88);
    snprintf(cStack78, 20,
             "\x1B"
             "CR[%d]",
             (int)(0.5f * (length - stringLength2)));
    strcat(cStack78, local_80);
    mpSelect_c->setString("", local_64, cStack78);
    mpSelect_c->setRubyString("", "", "");
    mpSelect_c->selAnimeInit(2, field_0xe2 + 1, 0, length, 0);
    if (mpArrow != NULL) {
        mpArrow->arwAnimeInit();
    }
}

void dMenu_ItemExplain_c::move_select_proc() {
    mpStick->checkTrigger();
    if (mDoCPd_c::getTrigA(PAD_1)) {
        mEndButton = 1;
        if (field_0xe2 == 0) {
            if (getWarpMarkFlag() == 0) {
                dMeter2Info_setWarpStatus(1);
            } else {
                dMeter2Info_setWarpStatus(2);
            }
        }
        if (field_0xe5 == 2) {
            mStatus = 5;
            Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            dMeter2Info_set2DVibration();
        } else {
            mStatus = 2;
            field_0xc8 = field_0xc4;
        }
        Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (mDoCPd_c::getTrigB(PAD_1)) {
        mEndButton = 2;
        if (field_0xe5 == 2) {
            mStatus = 5;
            Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            dMeter2Info_set2DVibration();
        } else {
            mStatus = 2;
            field_0xc8 = field_0xc4;
        }
    } else if (mpSelect_c->isSelect() && mpStick->checkUpTrigger()) {
        if (field_0xe2 == 1) {
            field_0xe2 = 0;
            Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    } else if (mpSelect_c->isSelect() && mpStick->checkDownTrigger()) {
        if (field_0xe2 == 0) {
            field_0xe2 = 1;
            Z2GetAudioMgr()->seStart(Z2SE_SY_TALK_CURSOR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
    if (mStatus == 3) {
        mpSelect_c->selAnimeMove(2, field_0xe2 + 1, false);
    } else {
        mpSelect_c->selAnimeEnd();
    }
}

void dMenu_ItemExplain_c::move_next_init() {
    field_0xc8 = 0x51d;
    if (mpArrow != NULL) {
        mpArrow->arwAnimeInit();
    }
}

void dMenu_ItemExplain_c::move_next_proc() {
    if (mDoCPd_c::getTrigA(PAD_1) || mDoCPd_c::getTrigB(PAD_1)) {
        if (mDoCPd_c::getTrigA(PAD_1)) {
            mEndButton = 1;
        } else if (mDoCPd_c::getTrigB(PAD_1)) {
            mEndButton = 2;
        }

        if (field_0xe5 == 2) {
            mStatus = 5;
            Z2GetAudioMgr()->seStart(Z2SE_SY_EXP_WIN_CLOSE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
            dMeter2Info_set2DVibration();
        } else {
            mStatus = 2;
            field_0xc8 = field_0xc4;
        }
        mpSelect_c->selAnimeEnd();
    }
}

void dMenu_ItemExplain_c::close_init() {
    mAlphaRatio = 201.0f;
}

void dMenu_ItemExplain_c::close_proc() {
    bool check;
    if (mpSelect_c != NULL) {
        check = mpSelect_c->selAnimeEnd();
    } else {
        check = 1;
    }
    if (check != 0) {
        mStatus = 0;
    }
}

u8 dMenu_ItemExplain_c::openExplain(u8 i_slotNo, u8 param_1, u8 param_2, bool param_3) {
    u8 item = dComIfGs_getItem(i_slotNo, false);
    if (item == 0xff) {
        return 0;
    } else {
        return openExplainDmap(item, param_1, param_2, param_3, i_slotNo);
    }
}

u8 dMenu_ItemExplain_c::openExplainDmap(u8 param_0, u8 param_1, u8 param_2, bool param_3,
                                        u8 param_4) {
    u8 itemNo = param_0;
    u8 ret = 0;
    s32 itemsObtained = 0;

    if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x11 && param_0 == 0x26) {
        for (int i = 0; i < 3; i++) {
            if (checkItemGet(i + fpcNm_ITEM_L2_KEY_PIECES1, 1) != 0) {
                itemsObtained++;
            }
        }
        if (itemsObtained <= 1) {
            itemNo = 0xf9;
        } else if (itemsObtained <= 2) {
            itemNo = 0xfa;
        } else {
            itemNo = 0xfd;
        }
    }
    if (itemNo == 0x50 && dComIfGs_isItemFirstBit(0x4f) && dMeter2Info_getRentalBombBag() == 0xff)
    {
        itemNo = 0x4f;
    }
    if (mStatus == 0) {
        mStatus = 1;
        field_0xe1 = param_0;
        field_0xcc = itemNo + 0x165;
        if (dStage_stagInfo_GetSaveTbl(dComIfGp_getStageStagInfo()) == 0x14 && itemNo == 0x23) {
            field_0xcc = 0x5bf;
        }
        if (itemNo == 0x25 && dComIfGs_isDungeonItemWarp(0x16)) {
            field_0xcc = 0x251;
        }
        field_0xc8 = itemNo + 0x265;
        if (dMeter2Info_getRentalBombBag() != 0xff) {
            if (param_4 == dMeter2Info_getRentalBombBag() + 0xf) {
                field_0xcc = 0x16d;
                if (dComIfGs_getBombNum(dMeter2Info_getRentalBombBag()) != 0) {
                    field_0xc8 = 0x26d;
                } else {
                    field_0xc8 = 0x26e;
                }
            }
        }
        if (itemNo == 0x46 && daPy_getPlayerActorClass()->checkCopyRodTopUse() == 0) {
            field_0xc8 = 0x2b2;
        }
        if (itemNo == 0x25 && dComIfGs_isDungeonItemWarp(0x16)) {
            field_0xc8 = 0x351;
        }
        field_0xe7 = 0;
        field_0xde = param_1;
        field_0xdf = param_2;
        open_init();
        setScale();
        ret = 1;
    } else if (param_3 && (mStatus == 1 || mStatus == 2)) {
        field_0xe1 = param_0;
        field_0xcc = itemNo + 0x165;
        if (itemNo == 0x25 && dComIfGs_isDungeonItemWarp(0x16)) {
            field_0xcc = 0x251;
        }
        field_0xc8 = itemNo + 0x265;
        if (dMeter2Info_getRentalBombBag() != 0xff) {
            if (param_4 == dMeter2Info_getRentalBombBag() + 0xf) {
                if (dComIfGs_getBombNum(dMeter2Info_getRentalBombBag()) != 0) {
                    field_0xc8 = 0x26d;
                } else {
                    field_0xc8 = 0x26e;
                }
            }
        }
        if (itemNo == 0x25 && dComIfGs_isDungeonItemWarp(0x16)) {
            field_0xc8 = 0x351;
        }
        field_0xe7 = 0;
        field_0xde = param_1;
        field_0xdf = param_2;
        open_init();
        setScale();
        ret = 1;
        ;
    }
    return ret;
}

u8 dMenu_ItemExplain_c::openExplainTx(u32 param_0, u32 param_1) {
    u8 ret = 0;
    if (mStatus == 0) {
        mStatus = 1;
        field_0xe1 = 0xff;
        field_0xcc = param_0;
        field_0xc8 = param_1;
        field_0xe7 = 0;
        field_0xde = 0;
        field_0xdf = 0;
        open_init();
        setScale();
        ret = 1;
    }
    return ret;
}

f32 dMenu_ItemExplain_c::getAlphaRatio() {
    switch (mStatus) {
    case 1:
    case 5:
        return (201.0f - mAlphaRatio) / 7.0f;
    case 0:
        return 1.0f;
    default:
        return 0.0f;
    }
}

void dMenu_ItemExplain_c::setNumber() {
    u8 temp = field_0xdf;
    if (temp == 0) {
        for (int i = 0; i < 3; i++) {
            mpItemNumTex[i]->hide();
        }
    } else {
        s32 temp2 = field_0xde;
        for (int i = 0; i < 3; i++) {
            mpItemNumTex[i]->show();
        }
        if (temp2 > 100) {
            temp2 = 100;
        }
        JUtility::TColor colorBlack;
        JUtility::TColor colorWhite;
        if (temp2 == temp) {
            colorBlack.set(30, 30, 30, 0);
            colorWhite.set(0xff, 200, 50, 0xff);
        } else if (temp2 == 0) {
            colorBlack.set(30, 30, 30, 0);
            colorWhite.set(180, 180, 180, 0xff);
        } else {
            colorBlack.set(0, 0, 0, 0);
            colorWhite.set(0xff, 0xff, 0xff, 0xff);
        }
        for (int i = 0; i < 3; i++) {
            mpItemNumTex[i]->setBlackWhite(colorBlack, colorWhite);
        }
        if (temp2 < 100) {
            ResTIMG* texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
                'TIMG', dMeter2Info_getNumberTextureName(temp2 / 10));
            mpItemNumTex[0]->changeTexture(texture, 0);
            texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
                'TIMG', dMeter2Info_getNumberTextureName(temp2 % 10));
            mpItemNumTex[1]->changeTexture(texture, 0);
            mpItemNumTex[2]->hide();
        } else {
            ResTIMG* texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
                'TIMG', dMeter2Info_getNumberTextureName(temp2 / 100));
            mpItemNumTex[0]->changeTexture(texture, 0);
            temp2 = (u8)(temp2 % 100);
            texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
                'TIMG', dMeter2Info_getNumberTextureName(temp2 / 10));
            mpItemNumTex[1]->changeTexture(texture, 0);
            texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
                'TIMG', dMeter2Info_getNumberTextureName(temp2 % 10));
            mpItemNumTex[2]->changeTexture(texture, 0);
        }
    }
}

bool dMenu_ItemExplain_c::getWarpMarkFlag() {
    if (dStage_stagInfo_GetUpButton(dComIfGp_getStageStagInfo()) == 1) {
        return dComIfGs_getWarpMarkFlag();
    }
    return dComIfGs_getLastWarpAcceptStage() < 0 ? 0 : 1;
}

void dMenu_ItemExplain_c::setScale() {
    if (field_0xe1 != 0xff) {
        f32 scale =
            g_drawHIO.mItemScaleAdjustON == true ? g_drawHIO.mItemScalePercent / 100.0f : 1.0f;
        f32 w = mpExpItemTex[0]->width / 48.0f * scale;
        f32 h = mpExpItemTex[0]->height / 48.0f * scale;
        mpInfoIcon->scale(w, h);
    }
}
