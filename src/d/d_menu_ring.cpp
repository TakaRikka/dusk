/**
 * @file d_menu_ring.cpp
 * @brief dolzel2 Menu - Item Wheel
 * 
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_menu_ring.h"
#include "JSystem/J2DGraph/J2DOrthoGraph.h"
#include "JSystem/J2DGraph/J2DTextBox.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_alink.h"
#include "d/d_item_data.h"
#include "d/d_kantera_icon_meter.h"
#include "d/d_lib.h"
#include "d/d_select_cursor.h"
#include "d/d_menu_item_explain.h"
#include "d/d_menu_window.h"
#include "d/d_meter2.h"
#include "d/d_meter2_draw.h"
#include "d/d_meter_HIO.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_string.h"
#include "m_Do/m_Do_controller_pad.h"
#include "m_Do/m_Do_graphic.h"
#include "string.h"

#include "stdio.h"

typedef void (dMenu_Ring_c::*initFunc)();
static initFunc stick_init[] = {
    /* STATUS_WAIT          */ &dMenu_Ring_c::stick_wait_init,
    /* STATUS_MOVE          */ &dMenu_Ring_c::stick_move_init,
    /* STATUS_EXPLAIN       */ &dMenu_Ring_c::stick_explain_init,
    /* STATUS_EXPLAIN_FORCE */ &dMenu_Ring_c::stick_explain_force_init,
};

typedef void (dMenu_Ring_c::*procFunc)();
static procFunc stick_proc[] = {
    /* STATUS_WAIT          */ &dMenu_Ring_c::stick_wait_proc,
    /* STATUS_MOVE          */ &dMenu_Ring_c::stick_move_proc,
    /* STATUS_EXPLAIN       */ &dMenu_Ring_c::stick_explain_proc,
    /* STATUS_EXPLAIN_FORCE */ &dMenu_Ring_c::stick_explain_force_proc,
};

dMenu_Ring_c::dMenu_Ring_c(JKRExpHeap* i_heap, STControl* i_stick, CSTControl* i_cStick,
                           u8 i_ringOrigin) {
    static const u64 xy_text[5] = {
        'yx_text', 'yx_te_s1', 'yx_te_s2', 'yx_te_s3', 'yx_te_s4',
    };
    static const u64 fxy_text[5] = {
        'fyx_tex', 'fyx_te_1', 'fyx_te_2', 'fyx_te_3', 'fyx_te_4',
    };
    static const u64 c_text[5] = {
        'c_text', 'c_te_s1', 'c_te_s2', 'c_te_s3', 'c_te_s4',
    };
    static const u64 fc_text[5] = {
        'fc_text', 'fc_te_s1', 'fc_te_s2', 'fc_te_s3', 'fc_te_s4',
    };
    static const u64 c_text1[5] = {
        'c_text1', 'c_texs1', 'c_texs2', 'c_texs3', 'c_texs4',
    };
    static const u64 fc_text1[5] = {
        'fc_text1', 'fc_texs1', 'fc_texs2', 'fc_texs3', 'fc_texs4',
    };
    static const u64 t_on[5] = {
        'gr_t_on', 'g_tof_s5', 'g_tof_s6', 'g_tof_s7', 'g_tof_s8',
    };
    static const u64 ft_on[5] = {
        'fr_t_on', 'f_tof_s5', 'f_tof_s6', 'f_tof_s7', 'f_tof_s8',
    };
    static const u64 t_off[5] = {
        'gr_t_of', 'g_tof_s1', 'g_tof_s2', 'g_tof_s3', 'g_tof_s4',
    };
    static const u64 ft_off[5] = {
        'fr_t_of', 'f_tof_s1', 'f_tof_s2', 'f_tof_s3', 'f_tof_s4',
    };

    mpHeap = i_heap;
    mpStick = i_stick;
    mpCStick = i_cStick;
    mRingOrigin = i_ringOrigin;
    dMeter2Info_setItemExplainWindowStatus(0);
    mpHeap->getTotalFreeSize(); // in debug, this is used for a size check
    mPikariFlashingSpeed = 0.0f;
    if (mRingOrigin == 0) {
        mCenterPosX = 0.0f;
        mCenterPosY = 448.0f;
    } else if (mRingOrigin == 2) {
        mCenterPosX = 0.0f;
        mCenterPosY = -448.0f;
    }
    if (mRingOrigin == 3) {
        mCenterPosX = 608.0f;
        mCenterPosY = 0.0f;
    } else if (mRingOrigin == 1) {
        mCenterPosX = -608.0f;
        mCenterPosY = 0.0f;
    } else {
        mCenterPosX = 0.0f;
        mCenterPosY = 0.0f;
    }
    mRingItemNamePosX = 0.0f;
    mRingItemNamePosY = 0.0f;
    mRingItemNameScale = 1.0f;
    for (int i = 0; i < 10; i++) {
        mRingGuidePosX[i] = 0.0f;
        mRingGuidePosY[i] = 0.0f;
        mRingGuideScale[i] = 1.0f;
    }
    mRingCursorScale = 1.0f;
    mRingPosX = 0.0f;
    mRingPosY = 0.0f;
    mRingScaleH = 1.0f;
    mRingScaleV = 1.0f;
    mRingAlpha = 1.0f;
    mPlayerIsWolf = daPy_py_c::checkNowWolf();
    mNameStringID = 0;
    field_0x63a = 0;
    field_0x63c = 0;
    mOpenCloseFrames = 0;
    mStatus = STATUS_WAIT;
    mOldStatus = STATUS_WAIT;
    field_0x6b2 = 0;
    mWaitFrames = 0;
    mDirectSelectCursorPos.set(0.0f, 0.0f, 0.0f);
    mCurrentSlot = SLOT_0;
    field_0x6a9 = 0;
    mXButtonSlot = 0xff;
    mYButtonSlot = 0xff;
    field_0x6ac = 0xff;
    field_0x6ad = 0xff;
    field_0x670 = 0;
    field_0x67e = 0;
    field_0x6b3 = 0;
    mAlphaRate = 0.0f;
    mDrawFlag = 0;
    mTotalItemTexToAlloc = 0;
    field_0x67c = 4;
    field_0x6c5 = 0;
    mCursorSpeed = 0;
    field_0x684 = 0;
    field_0x6c6 = 0;
    field_0x6c4 = 0xff;
    mDoStatus = 0;
    field_0x6cb = 0xff;
    field_0x6cd = 0xff;
    mDirectSelectActive = false;
    field_0x68e = 0;
    field_0x6cf = 0xff;
    field_0x6d0 = 0xff;
    field_0x6d1 = 0xff;
    field_0x6d2 = 0xff;
    field_0x6d3 = 0xff;
    int i;
    for (int i = 0; i < 3; i++) {
        field_0x580[i] = 0.0f;
        field_0x574[i] = 0.0f;
    }
    field_0x6c3 = 0xff;
    field_0x6c2 = 0;
    for (int i = 0; i < 3; i++) {
        mpResData[i] = NULL;
    }
    for (int i = 0; i < 4; i++) {
        field_0x6c7[i] = 0xff;
    }
    switch (mRingOrigin) {
    case 0:
        field_0x682 = 0x8000;
        break;
    case 1:
        field_0x682 = 0x4000;
        break;
    case 2:
        field_0x682 = 0;
        break;
    default:
        field_0x682 = 0xc000;
        break;
    }
    for (int i = 0; i < 4; i++) {
        field_0x674[i] = 0;
        field_0x518[i] = 0.0f;
        field_0x528[i] = 0.0f;
        field_0x538[i] = 0.0f;
        field_0x6b4[i] = 0;
        field_0x6b8[i] = 0xff;
        mpSelectItemTex[i][0] = NULL;
        mpSelectItemTex[i][1] = NULL;
        mpSelectItemTex[i][2] = NULL;
        field_0x686[i] = 0;
    }
    for (int i = 0; i < MAX_ITEM_SLOTS; i++) {
        mItemSlotPosY[i] = 0.0f;
        mItemSlotPosX[i] = 0.0f;
        for (int j = 0; j < 3; j++) {
            mpItemTex[i][j] = NULL;
            mpItemBuf[i][j] = NULL;
        }
        mItemSlots[i] = 0;
        field_0x63e[i] = 0;
        mItemSlotParam2[i] = 0.0f;
        mItemSlotParam1[i] = 0.0f;
    }
    for (int i = 0; i < MAX_ITEM_SLOTS; i++) {
        if (dComIfGs_getLineUpItem(i) != fpcNm_ITEM_NONE) {
            mTotalItemTexToAlloc++;
        }
    }
    if (mTotalItemTexToAlloc < 1) {
        mItemsTotal = 1;
    } else {
        mItemsTotal = mTotalItemTexToAlloc;
    }
    for (int i = 0; i < MAX_ITEM_SLOTS; i++) {
        mItemSlots[i] = dComIfGs_getLineUpItem(i);
    }
    for (int i = 0; i < mItemsTotal; i++) {
        mItemSlots[i] = dComIfGs_getLineUpItem(i);
        if (dComIfGs_getSelectItemIndex(0) == dComIfGs_getLineUpItem(i)) {
            mXButtonSlot = i;
        }
        if (dComIfGs_getSelectItemIndex(1) == dComIfGs_getLineUpItem(i)) {
            mYButtonSlot = i;
        }
        if (dComIfGs_getSelectItemIndex(2) == dComIfGs_getWolfAbility(i)) {
            field_0x6ac = i;
        }
    }
    mRingRadiusH = g_ringHIO.mRingRadiusH;
    mRingRadiusV = g_ringHIO.mRingRadiusV;
    field_0x66e = 0x8000;
    field_0x634 = 0x10000 / mItemsTotal;
    for (int i = 0; i < MAX_SELECT_ITEM; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < SELECT_ITEM_NUM; k++) {
                mpSelectItemTexBuf[i][j][k] = (ResTIMG*)mpHeap->alloc(0xC00, 0x20);
            }
        }
        field_0x6be[i] = 0;
        if (i == 2) {
            setSelectItem(i, 0);
        } else {
            setSelectItem(i, 0x43);
        }
        for (int j = 0; j < 3; j++) {
            mpSelectItemTex[i][j] = new J2DPicture(mpSelectItemTexBuf[i][field_0x6be[i]][0]);
            mpSelectItemTex[i][j]->setBasePosition(J2DBasePosition_4);
        }
        field_0x548[i] = 0.0f;
        field_0x558[i] = 0.0f;
    }
    ResTIMG* timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
        'TIMG', dMeter2Info_getNumberTextureName(0));
    for (int i = 0; i < 3; i++) {
        mpItemNumTex[i] = new J2DPicture(timg);
    }
    mpKanteraMeter = new dKantera_icon_c();
    mpScreen = new J2DScreen();
    dPaneClass_setPriority(&mpResData[0], mpHeap, mpScreen,
                           "SCRN/zelda_item_select_icon_message_ver2.blo", 0x20000,
                           dComIfGp_getRingResArchive());
    dPaneClass_showNullPane(mpScreen);
    mpMessageParent = new CPaneMgrAlpha(mpScreen, 'n_all', 2, NULL);
    mpTextParent[0] = new CPaneMgr(mpScreen, 'r_n', 0, NULL);
    mpTextParent[1] = new CPaneMgr(mpScreen, 'c_n', 2, NULL);
    mpTextParent[1]->setAlphaRate(1.0f);
    mpTextParent[2] = NULL;
    mpTextParent[3] = new CPaneMgr(mpScreen, 'c_sen_n', 2, NULL);
    mpTextParent[4] = new CPaneMgr(mpScreen, 'gr_n', 2, NULL);
    mpTextParent[4]->hide();
    for (int i = 5; i < 10; i++) {
        mpTextParent[i] = NULL;
    }
    for (int i = 0; i < mTotalItemTexToAlloc; i++) {
        for (int j = 0; j < 3; j++) {
            mpItemBuf[i][j] = (ResTIMG*)mpHeap->alloc(0xC00, 0x20);
        }
        u8 item = dComIfGs_getItem(mItemSlots[i], false);
        if (item != fpcNm_ITEM_NONE) {
            if (item == fpcNm_ITEM_LIGHT_ARROW) {
                // safety check to prevent attempts setting up a light arrow texture
                item = fpcNm_ITEM_BOW;
            }
            s32 i_textureNum =
                dMeter2Info_readItemTexture(item, mpItemBuf[i][0], NULL, mpItemBuf[i][1], NULL,
                                            mpItemBuf[i][2], NULL, NULL, NULL, -1);
            for (int k = 0; k < i_textureNum; k++) {
                mpItemTex[i][k] = new J2DPicture(mpItemBuf[i][k]);
                mpItemTex[i][k]->setBasePosition(J2DBasePosition_4);
            }
            dMeter2Info_setItemColor(item, mpItemTex[i][0], mpItemTex[i][1], mpItemTex[i][2], NULL);
            u8 texScale = dItem_data::getTexScale(item);
            f32 fVar1 = (texScale / 100.0f);
            f32 fVar2 = (mpItemBuf[i][0]->width / 48.0f);
            fVar1 = fVar2 * fVar1;
            mItemSlotParam1[i] = fVar1;
            mItemSlotParam2[i] = (mpItemBuf[i][0]->height / 48.0f * (texScale / 100.0f));
        }
    }
    mpScreen->search('r_btn_n')->hide();
    if (mPlayerIsWolf) {
        mpScreen->search('yx_te_s1')->hide();
        mpScreen->search('yx_te_s2')->hide();
        mpScreen->search('yx_te_s3')->hide();
        mpScreen->search('yx_te_s4')->hide();
        mpScreen->search('yx_text')->hide();
        mpScreen->search('fyx_te_1')->hide();
        mpScreen->search('fyx_te_2')->hide();
        mpScreen->search('fyx_te_3')->hide();
        mpScreen->search('fyx_te_4')->hide();
        mpScreen->search('fyx_tex')->hide();
        mpScreen->search('x_btn_n')->hide();
        mpScreen->search('y_btn_n')->hide();
    }
    mpString = new dMsgString_c();
    for (i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        J2DTextBox* fxy_TextBox = (J2DTextBox*)mpScreen->search(xy_text[i]);
        mpScreen->search(fxy_text[i])->hide();
#else
        J2DTextBox* fxy_TextBox = (J2DTextBox*)mpScreen->search(fxy_text[i]);
        mpScreen->search(xy_text[i])->hide();
#endif
        fxy_TextBox->setFont(mDoExt_getMesgFont());
        fxy_TextBox->setString(0x40, "");
        field_0x580[0] = mpString->getString(0x380, fxy_TextBox, NULL, NULL, NULL, 0);
    }
    for (i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        J2DTextBox* fc_TextBox = (J2DTextBox*)mpScreen->search(c_text[i]);
        mpScreen->search(fc_text[i])->hide();
#else
        J2DTextBox* fc_TextBox = (J2DTextBox*)mpScreen->search(fc_text[i]);
        mpScreen->search(c_text[i])->hide();
#endif
        fc_TextBox->setFont(mDoExt_getMesgFont());
        fc_TextBox->setString(0x40, "");
        field_0x580[1] = mpString->getString(0x37F, fc_TextBox, NULL, NULL, NULL, 0);
    }
    for (i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        J2DTextBox* fc1_TextBox = (J2DTextBox*)mpScreen->search(c_text1[i]);
        mpScreen->search(fc_text1[i])->hide();
#else
        J2DTextBox* fc1_TextBox = (J2DTextBox*)mpScreen->search(fc_text1[i]);
        mpScreen->search(c_text1[i])->hide();
#endif
        fc1_TextBox->setFont(mDoExt_getMesgFont());
        fc1_TextBox->setString(0x40, "");
        field_0x580[2] = mpString->getString(0x4CD, fc1_TextBox, NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        mpComboOffString[i] = (J2DTextBox*)mpScreen->search(t_on[i]);
        mpScreen->search(ft_on[i])->hide();
#else
        mpComboOffString[i] = (J2DTextBox*)mpScreen->search(ft_on[i]);
        mpScreen->search(t_on[i])->hide();
#endif
        mpComboOffString[i]->setString(0x40, "");
        mpComboOffString[i]->setFont(mDoExt_getMesgFont());
        mpString->getString(0x4D2, mpComboOffString[i], NULL, NULL, NULL, 0);
    }
    for (int i = 0; i < 5; i++) {
#if VERSION == VERSION_GCN_JPN
        mpBowArrowComboString[i] = (J2DTextBox*)mpScreen->search(t_off[i]);
        mpScreen->search(ft_off[i])->hide();
#else
        mpBowArrowComboString[i] = (J2DTextBox*)mpScreen->search(ft_off[i]);
        mpScreen->search(t_off[i])->hide();
#endif
        mpBowArrowComboString[i]->setString(0x40, "");
        mpBowArrowComboString[i]->setFont(mDoExt_getMesgFont());
        mpString->getString(0x4D3, mpBowArrowComboString[i], NULL, NULL, NULL, 0);
    }
    mpHeap->getTotalFreeSize();
    timg = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource('TIMG', "tt_block8x8.bti");
    mpBlackTex = new J2DPicture(timg);
    mpBlackTex->setBlackWhite(JUtility::TColor(0, 0, 0, 0), JUtility::TColor(0, 0, 0, 0xff));
    mpBlackTex->setAlpha(0);
    mpSpotScreen = new J2DScreen();
    dPaneClass_setPriority(&mpResData[1], mpHeap, mpSpotScreen,
                           "SCRN/zelda_item_select_icon3_spot.blo", 0x20000,
                           dComIfGp_getRingResArchive());
    dPaneClass_showNullPane(mpSpotScreen);
    mpSpotParent = new CPaneMgrAlpha(mpSpotScreen, 'n_all', 2, NULL);
    mpCenterScreen = new J2DScreen();
    dPaneClass_setPriority(&mpResData[2], mpHeap, mpCenterScreen,
                           "SCRN/zelda_item_select_icon3_center_parts.blo", 0x20000,
                           dComIfGp_getRingResArchive());
    dPaneClass_showNullPane(mpCenterScreen);
    mpCenterParent = new CPaneMgrAlpha(mpCenterScreen, 'center_n', 2, NULL);
    mpNameParent = new CPaneMgr(mpCenterScreen, 'label_n', 1, NULL);
    mpCircle = new CPaneMgr(mpCenterScreen, 'circle_n', 2, NULL);
    J2DTextBox* textBox[4];
#if VERSION == VERSION_GCN_JPN
    textBox[0] = (J2DTextBox*)mpCenterScreen->search('item_n04');
    textBox[1] = (J2DTextBox*)mpCenterScreen->search('item_n05');
    textBox[2] = (J2DTextBox*)mpCenterScreen->search('item_n06');
    textBox[3] = (J2DTextBox*)mpCenterScreen->search('item_n07');
    J2DPane* pane = mpCenterScreen->search('fitem_n1');
    pane->mVisible = false;
    pane = mpCenterScreen->search('fitem_n2');
    pane->mVisible = false;
    pane = mpCenterScreen->search('fitem_n3');
    pane->mVisible = false;
    pane = mpCenterScreen->search('fitem_n4');
    pane->mVisible = false;
#else
    textBox[0] = (J2DTextBox*)mpCenterScreen->search('fitem_n1');
    textBox[1] = (J2DTextBox*)mpCenterScreen->search('fitem_n2');
    textBox[2] = (J2DTextBox*)mpCenterScreen->search('fitem_n3');
    textBox[3] = (J2DTextBox*)mpCenterScreen->search('fitem_n4');
    mpCenterScreen->search('item_n04');
    mpCenterScreen->search('item_n05');
    mpCenterScreen->search('item_n06');
    mpCenterScreen->search('item_n07');
#endif
    for (int i = 0; i < 4; i++) {
        textBox[i]->setFont(mDoExt_getMesgFont());
        textBox[i]->setString(0x40, "");
    }
    textCentering();
    mpDrawCursor = new dSelect_cursor_c(2, g_ringHIO.mCursorScale, dComIfGp_getMain2DArchive());
    mpDrawCursor->setAlphaRate(1.0f);
    mpItemExplain = new dMenu_ItemExplain_c(mpHeap, dComIfGp_getRingResArchive(), i_stick, true);
    setRotate();
    mpDrawCursor->setPos(mItemSlotPosX[0] + mCenterPosX, mItemSlotPosY[0] + mCenterPosY);
    if (dComIfGs_getItem(mItemSlots[0], false) != fpcNm_ITEM_NONE) {
        mpDrawCursor->setParam(mItemSlotParam1[0], mItemSlotParam2[0], 0.1f, 0.6f, 0.5f);
    } else {
        mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.6f, 0.5f);
    }
}

dMenu_Ring_c::~dMenu_Ring_c() {
    mpHeap->getTotalFreeSize();
    dMeter2Info_setItemExplainWindowStatus(0);
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 2; k++) {
                mpHeap->free(mpSelectItemTexBuf[i][j][k]);
                mpSelectItemTexBuf[i][j][k] = NULL;
            }
            if (mpSelectItemTex[i][j] != NULL) {
                delete mpSelectItemTex[i][j];
                mpSelectItemTex[i][j] = NULL;
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        if (mpItemNumTex[i] != NULL) {
            delete mpItemNumTex[i];
            mpItemNumTex[i] = NULL;
        }
    }

    delete mpKanteraMeter;
    mpKanteraMeter = NULL;

    delete mpScreen;
    mpScreen = NULL;

    delete mpMessageParent;
    mpMessageParent = NULL;

    for (int i = 0; i < 10; i++) {
        if (mpTextParent[i] != NULL) {
            delete mpTextParent[i];
            mpTextParent[i] = NULL;
        }
    }

    for (int i = 0; i < MAX_ITEM_SLOTS; i++) {
        for (int j = 0; j < 3; j++) {
            if (mpItemTex[i][j] != NULL) {
                delete mpItemTex[i][j];
                mpItemTex[i][j] = NULL;
            }

            if (mpItemBuf[i][j] != NULL) {
                mpHeap->free(mpItemBuf[i][j]);
                mpItemBuf[i][j] = NULL;
            }
        }
    }

    delete mpString;
    mpString = NULL;

    mpHeap->getTotalFreeSize();

    delete mpBlackTex;
    mpBlackTex = NULL;

    delete mpSpotScreen;
    mpSpotScreen = NULL;

    delete mpSpotParent;
    mpSpotParent = NULL;

    delete mpCenterScreen;
    mpCenterScreen = NULL;

    for (int i = 0; i < 3; i++) {
        if (mpResData[i] != NULL) {
            mpHeap->free(mpResData[i]);
            mpResData[i] = NULL;
        }
    }

    delete mpCenterParent;
    mpCenterParent = NULL;

    delete mpNameParent;
    mpNameParent = NULL;

    delete mpCircle;
    mpCircle = NULL;

    delete mpDrawCursor;
    mpDrawCursor = NULL;

    delete mpItemExplain;
    mpItemExplain = NULL;

    dComIfGp_getRingResArchive()->removeResourceAll();
}

/** @details
 * Initializes the very first status (which is STATUS_WAIT) after the ctor
 * and plays the item wheel opening sound
*/
void dMenu_Ring_c::_create() {
    (this->*stick_init[mStatus])();
    Z2GetAudioMgr()->seStart(Z2SE_ITEM_RING_IN, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Ring_c::_delete() {
    /* empty function */
}

/** @details
 * This is the update function which runs every frame. 
 * It runs a process based on mStatus every frame or 
 * initializes a new process if mStatus changes
*/
void dMenu_Ring_c::_move() {
    mRingRadiusH = g_ringHIO.mRingRadiusH;
    mRingRadiusV = g_ringHIO.mRingRadiusV;
    mOldStatus = mStatus; // Save current status for check
    mpItemExplain->move();
    (this->*stick_proc[mStatus])(); // run process based on status

    // If a new status has been set in mStatus, initialize the new process
    if (mStatus != mOldStatus) {
        (this->*stick_init[mStatus])();
    }

    setScale();
    setActiveCursor();
    setMixMessage();
    if (mRingCursorScale != g_ringHIO.mCursorScale) {
        mRingCursorScale = g_ringHIO.mCursorScale;
        mpDrawCursor->setScale(g_ringHIO.mCursorScale);
    }
}

void dMenu_Ring_c::_draw() {
    J2DGrafContext* grafPort = dComIfGp_getCurrentGrafPort();
    grafPort->setup2D();
    if (mDrawFlag == 0) {
        mpSpotParent->setAlphaRate(mAlphaRate * g_ringHIO.mOverlayAlpha);
        mpSpotScreen->draw(0.0f, 0.0f, grafPort);
        if (mRingItemNameScale != g_ringHIO.mItemNameScale) {
            mRingItemNameScale = g_ringHIO.mItemNameScale;
            mpNameParent->scale(mRingItemNameScale, mRingItemNameScale);
        }
        if (mRingItemNamePosX != g_ringHIO.mItemNamePosX || mRingItemNamePosY != g_ringHIO.mItemNamePosY) {
            mRingItemNamePosX = g_ringHIO.mItemNamePosX;
            mRingItemNamePosY = g_ringHIO.mItemNamePosY;
            mpNameParent->paneTrans(mRingItemNamePosX, mRingItemNamePosY);
        }
        if (mRingPosX != g_ringHIO.mRingPosX || mRingPosY != g_ringHIO.mRingPosY) {
            mRingPosX = g_ringHIO.mRingPosX;
            mRingPosY = g_ringHIO.mRingPosY;
            mpCircle->paneTrans(mRingPosX, mRingPosY);
        }
        if (mRingScaleH != g_ringHIO.mRingScaleH || mRingScaleV != g_ringHIO.mRingScaleV) {
            mRingScaleH = g_ringHIO.mRingScaleH;
            mRingScaleV = g_ringHIO.mRingScaleV;
            mpCircle->scale(mRingScaleH, mRingScaleV);
        }
        f32 ringAlpha = mPlayerIsWolf != 0 ? g_ringHIO.mRingAlpha_Wolf : g_ringHIO.mRingAlpha;
        if (mRingAlpha != ringAlpha) {
            mRingAlpha = ringAlpha;
            mpCircle->setAlphaRate(mRingAlpha);
        }
        mpCenterParent->setAlphaRate(mAlphaRate);
        mpCenterScreen->draw(mCenterPosX, mCenterPosY, grafPort);
        drawItem();
        textScaleHIO();
        f32 alphaRate = mpTextParent[1]->getAlphaRate();
        mpMessageParent->setAlphaRate(mAlphaRate);
        if (mStatus == STATUS_EXPLAIN) {
            mpTextParent[1]->setAlphaRate(alphaRate * mAlphaRate);
        }
        mpScreen->draw(mCenterPosX, mCenterPosY, grafPort);
        if (mStatus != STATUS_EXPLAIN && mPikariFlashingSpeed > 0.0f) {
            Vec pos;
            CPaneMgr paneMgr;
            pos = paneMgr.getGlobalVtxCenter(mpScreen->search('gr_btn'), true, 0);
            dMeter2Info_getMeterClass()->getMeterDrawPtr()->drawPikari(
                pos.x, pos.y, &mPikariFlashingSpeed, g_ringHIO.mPikariScale, g_ringHIO.mPikariFrontOuter,
                g_ringHIO.mPikariFrontInner, g_ringHIO.mPikariBackOuter, g_ringHIO.mPikariBackInner,
                g_ringHIO.mPikariAnimSpeed, 2);
        }
        mDrawFlag = 1;
    } else {
        drawSelectItem();
        drawItem2();
        mpDrawCursor->draw();
        mpItemExplain->trans(mCenterPosX, mCenterPosY);
        mpItemExplain->draw((J2DOrthoGraph*)grafPort);
        drawFlag0();
    }
}

void dMenu_Ring_c::setKanteraPos(f32 i_posX, f32 i_posY) {
    mpKanteraMeter->setPos(i_posX, i_posY);
}

bool dMenu_Ring_c::isOpen() {
    bool opened = false;
    if (mOpenCloseFrames == 0) {
        dMeter2Info_set2DVibrationM();
    }
    mOpenCloseFrames++;
    mAlphaRate = (f32)mOpenCloseFrames / (f32)g_ringHIO.mOpenFrames;
    if (mRingOrigin == 0) {
        mCenterPosX = 0.0f;
        mCenterPosY = (1.0f - mAlphaRate) * 448.0f;
    } else if (mRingOrigin == 2) {
        mCenterPosX = 0.0f;
        mCenterPosY = (1.0f - mAlphaRate) * -448.0f;
    } else if (mRingOrigin == 3) {
        mCenterPosX = (1.0f - mAlphaRate) * 608.0f;
        mCenterPosY = 0.0f;
    } else if (mRingOrigin == 1) {
        mCenterPosX = (1.0f - mAlphaRate) * -608.0f;
        mCenterPosY = 0.0f;
    }
    if (mOpenCloseFrames >= g_ringHIO.mOpenFrames) { 
        // Opening is finished, set to g_ringHIO.mCloseFrames as a
        // preparation for when the player closes the item wheel
        mOpenCloseFrames = g_ringHIO.mCloseFrames;
        mAlphaRate = 1.0f;
        mCenterPosX = 0.0f;
        mCenterPosY = 0.0f;
        opened = true;
    }
    setScale();
    mpDrawCursor->setPos(mItemSlotPosX[SLOT_0] + mCenterPosX, mItemSlotPosY[SLOT_0] + mCenterPosY);
    if (dComIfGs_getItem(mItemSlots[SLOT_0], false) != fpcNm_ITEM_NONE) {
        mpDrawCursor->setParam(mItemSlotParam1[0], mItemSlotParam2[0], 0.1f, 0.6f, 0.5f);
    } else {
        mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.6f, 0.5f);
    }

    return opened;
}

bool dMenu_Ring_c::isMoveEnd() {
    bool ret = 0;
    if (mStatus == STATUS_WAIT && mOldStatus != STATUS_EXPLAIN_FORCE && mOldStatus != STATUS_EXPLAIN) {
        if (dMw_UP_TRIGGER() || dMw_DOWN_TRIGGER() || dMw_B_TRIGGER() ||
            dMeter2Info_getWarpStatus() == 2 || dMeter2Info_getWarpStatus() == 1 ||
            dMeter2Info_isTouchKeyCheck(0xe))
        {
            if (dMw_UP_TRIGGER()) {
                mRingOrigin = 0;
            } else if (dMw_DOWN_TRIGGER()) {
                mRingOrigin = 2;
            } else {
                mRingOrigin = 0xff;
            }
            Z2GetAudioMgr()->seStart(Z2SE_ITEM_RING_OUT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_set2DVibrationM();
            ret = 1;
        }
    }
    return ret;
}

bool dMenu_Ring_c::isClose() {
    bool closed = true;
    if (field_0x674[0] != 0 || field_0x674[1] != 0 || field_0x674[2] != 0 || field_0x674[3] != 0) {
        return 0;
    }
    mOpenCloseFrames--;
    mAlphaRate = (f32)mOpenCloseFrames / (f32)g_ringHIO.mCloseFrames;
    if (mOpenCloseFrames <= 0) {
        for (int i = 0; i < 4; i++) {
            setSelectItemForce(i);
        }
        mOpenCloseFrames = 0;
        mAlphaRate = 0.0f;
    } else {
        closed = false;
    }
    if (mRingOrigin == 0) {
        mCenterPosX = 0.0f;
        mCenterPosY = (1.0f - mAlphaRate) * -448.0f;
    } else if (mRingOrigin == 2) {
        mCenterPosX = 0.0f;
        mCenterPosY = (1.0f - mAlphaRate) * 448.0f;
    } else if (mRingOrigin == 3) {
        mCenterPosX = (1.0f - mAlphaRate) * -608.0f;
        mCenterPosY = 0.0f;
    } else if (mRingOrigin == 1) {
        mCenterPosX = (1.0f - mAlphaRate) * 608.0f;
        mCenterPosY = 0.0f;
    }
    mpDrawCursor->setPos(mItemSlotPosX[mCurrentSlot] + mCenterPosX,
                         mItemSlotPosY[mCurrentSlot] + mCenterPosY);
    if (dComIfGs_getItem(mItemSlots[mCurrentSlot], false) != fpcNm_ITEM_NONE) {
        mpDrawCursor->setParam(mItemSlotParam1[mCurrentSlot], mItemSlotParam2[mCurrentSlot], 0.1f, 0.6f,
                               0.5f);
    } else {
        mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.6f, 0.5f);
    }

    return closed;
}

u8 dMenu_Ring_c::getStickInfo(STControl* i_stick) {
    field_0x6c2 = 0xff;
    if (i_stick->getValueStick() >= 0.75f) {
        s16 stick_angle = i_stick->getAngleStick();
        s16 abs_stick_angle = stick_angle + 0x8000;
        int something_stick = abs_stick_angle + 0x8000;

        s32 temp;
        u32 uVar3 = mItemsTotal / 2;
        u8 val2 = uVar3;
        int val = mItemsTotal;

        if ((val % 2) != 0) {
            temp = field_0x634;
        } else {
            temp = (s16)(field_0x634 / 2);
        }

        for (s32 i = 0; i < val; i++) {
            if (something_stick < temp) {
                break;
            }
            temp += field_0x634;

            if (val2 <= 0) {
                val2 = val - 1;
            } else {
                val2--;
            }
        }

        if (mCurrentSlot != val2) {
            if (mDoCPd_c::getHoldL(PAD_1)) {
                mDirectSelectCursorPos.x = mItemSlotPosX[mCurrentSlot];
                mDirectSelectCursorPos.z = mItemSlotPosY[mCurrentSlot];
                mCurrentSlot = val2;
                mDirectSelectActive = true;
            } else if (mCurrentSlot >= uVar3) {
                if (val2 >= (u8)(mCurrentSlot - uVar3) && val2 < mCurrentSlot) {
                    if (mCurrentSlot == 0) {
                        mCurrentSlot = mItemsTotal - 1;
                    } else {
                        mCurrentSlot--;
                    }
                } else if (mCurrentSlot == (u8)(mItemsTotal - 1)) {
                    mCurrentSlot = 0;
                } else {
                    mCurrentSlot++;
                }
            } else {
                u8 max = mCurrentSlot + uVar3;
                if (val2 > mCurrentSlot && val2 <= max) {
                    if (mCurrentSlot == (u8)(mItemsTotal - 1)) {
                        mCurrentSlot = 0;
                    } else {
                        mCurrentSlot++;
                    }
                } else if (mCurrentSlot == 0) {
                    mCurrentSlot = mItemsTotal - 1;
                } else {
                    mCurrentSlot--;
                }
            }
            field_0x670 = field_0x63e[mCurrentSlot];
            if (mItemsTotal == 2 && stick_angle < 0) {
                if (mCurrentSlot == 0) {
                    field_0x6d3 = 0;
                } else {
                    field_0x6d3 = 1;
                }
            }
            return 1;
        }
    } else {
        mCursorSpeed = 0;
        dpdMove();
        if (field_0x6c2 != 0xff) {
            return 0;
        }
    }
    return 0;
}

s16 dMenu_Ring_c::calcStickAngle(STControl* i_stick, u8 param_1) {
    s16 directionTrig = i_stick->getAngleStick();
    switch (param_1) {
    case 0:
        directionTrig += 0x8000;
        break;
    case 4:
        directionTrig -= 0x6000;
        break;
    case 2:
        break;
    case 1:
        directionTrig -= 0x4000;
        break;
    case 5:
        directionTrig -= 0x2000;
        break;
    case 6:
        directionTrig += 0x2000;
        break;
    case 3:
        directionTrig += 0x4000;
        break;
    case 7:
        directionTrig += 0x6000;
        break;
    default:
        directionTrig += 0x4000;
        break;
    }
    return directionTrig;
}

void dMenu_Ring_c::setRotate() {
    clacEllipsePlotAverage(mItemsTotal, g_ringHIO.mItemRingPosX + 304.0f,
                           g_ringHIO.mItemRingPosY + 224.0f);
    for (int i = 0; i < mItemsTotal; i++) {
        field_0x63e[i] = cM_atan2s(mItemSlotPosX[i] - (304.0f + g_ringHIO.mItemRingPosX),
                                   mItemSlotPosY[i] - (224.0f + g_ringHIO.mItemRingPosY));
    }
}

void dMenu_Ring_c::setItemScale(int i_idx, f32 i_scale) {
    for (int i = 0; i < 3; i++) {
        if (mpItemTex[i_idx][i] != NULL) {
            mpItemTex[i_idx][i]->scale(i_scale, i_scale);
        }
    }
}

void dMenu_Ring_c::setButtonScale(int i_idx, f32 i_scale) {
    i_idx += 8; // reach the offset in mpTextParent for the buttons
    if (mpTextParent[i_idx] != NULL) {
        f32 buttonScale = i_scale * mRingGuideScale[i_idx];
        mpTextParent[i_idx]->scale(buttonScale, buttonScale);
    }
}

void dMenu_Ring_c::setItem() {
    u8 uVar1;
    u8 uVar2;
    u8 uVar3;
    u8 uVar4;

    if (mXButtonSlot != fpcNm_ITEM_NONE) {
        uVar1 = mItemSlots[mXButtonSlot];
    } else {
        uVar1 = fpcNm_ITEM_NONE;
    }
    if (mYButtonSlot != fpcNm_ITEM_NONE) {
        uVar2 = mItemSlots[mYButtonSlot];
    } else {
        uVar2 = fpcNm_ITEM_NONE;
    }
    if (field_0x6ac != fpcNm_ITEM_NONE) {
        uVar3 = mItemSlots[field_0x6ac];
    } else {
        uVar3 = fpcNm_ITEM_NONE;
    }
    if (field_0x6ad != fpcNm_ITEM_NONE) {
        uVar4 = mItemSlots[field_0x6ad];
    } else {
        uVar4 = fpcNm_ITEM_NONE;
    }

    u8 mixItemIndex0 = dComIfGs_getMixItemIndex(0);
    u8 mixItemIndex1 = dComIfGs_getMixItemIndex(1);

    for (int i = 0; i < 4; i++) {
        setSelectItemForce(i);
    }

    checkExplainForce();
    if (field_0x6b3 == 0) {
        uVar1 = dComIfGs_getSelectItemIndex(1);
        if (mItemSlots[mCurrentSlot] == uVar1) {
            uVar2 = dComIfGs_getSelectItemIndex(0);
            mixItemIndex1 = dComIfGs_getMixItemIndex(0);
            if (uVar2 == fpcNm_ITEM_NONE) {
                mYButtonSlot = fpcNm_ITEM_NONE;
            } else {
                mYButtonSlot = mXButtonSlot;
            }
            mXButtonSlot = mCurrentSlot;
            uVar1 = mItemSlots[mXButtonSlot];
            mixItemIndex0 = fpcNm_ITEM_NONE;
        } else {
            if (dComIfGs_getMixItemIndex(1) == mItemSlots[mCurrentSlot]) {
                uVar2 = dComIfGs_getSelectItemIndex(0);
                mixItemIndex1 = fpcNm_ITEM_NONE;
                if (uVar2 == fpcNm_ITEM_NONE) {
                    mYButtonSlot = fpcNm_ITEM_NONE;
                } else {
                    mYButtonSlot = mXButtonSlot;
                }
                mXButtonSlot = mCurrentSlot;
                uVar1 = mItemSlots[mXButtonSlot];
                mixItemIndex0 = fpcNm_ITEM_NONE;
            } else {
                mXButtonSlot = mCurrentSlot;
                uVar1 = mItemSlots[mXButtonSlot];
                mixItemIndex0 = fpcNm_ITEM_NONE;
            }
        }
    } else if (field_0x6b3 == 1) {
        if (mItemSlots[mCurrentSlot] == dComIfGs_getSelectItemIndex(0)) {
            u8 temp = dComIfGs_getSelectItemIndex(1);
            uVar1 = temp;
            mixItemIndex0 = dComIfGs_getMixItemIndex(1);
            if (temp == fpcNm_ITEM_NONE) {
                mXButtonSlot = fpcNm_ITEM_NONE;
            } else {
                mXButtonSlot = mYButtonSlot;
            }
            mYButtonSlot = mCurrentSlot;
            uVar2 = mItemSlots[mYButtonSlot];
            mixItemIndex1 = fpcNm_ITEM_NONE;
        } else {
            if (dComIfGs_getMixItemIndex(0) == mItemSlots[mCurrentSlot]) {
                uVar1 = dComIfGs_getSelectItemIndex(1);
                mixItemIndex0 = fpcNm_ITEM_NONE;
                if (uVar1 == fpcNm_ITEM_NONE) {
                    mXButtonSlot = fpcNm_ITEM_NONE;
                } else {
                    mXButtonSlot = mYButtonSlot;
                }
                mYButtonSlot = mCurrentSlot;
                uVar2 = mItemSlots[mYButtonSlot];
                mixItemIndex1 = fpcNm_ITEM_NONE;
            } else {
                mYButtonSlot = mCurrentSlot;
                uVar2 = mItemSlots[mYButtonSlot];
                mixItemIndex1 = fpcNm_ITEM_NONE;
            }
        }
    }
    field_0x6b4[0] = uVar1;
    field_0x6b4[1] = uVar2;
    field_0x6b4[2] = uVar3;
    field_0x6b4[3] = uVar4;
    field_0x6b8[0] = mixItemIndex0;
    field_0x6b8[1] = mixItemIndex1;
    field_0x6b8[2] = fpcNm_ITEM_NONE;
    field_0x6b8[3] = fpcNm_ITEM_NONE;
    field_0x6cd = fpcNm_ITEM_NONE;
    setJumpItem(true);
}

void dMenu_Ring_c::setJumpItem(bool i_useVibrationM) {
    for (int i = 0; i < 4; i++) {
        if (i == 2) {
            setSelectItem(i, field_0x6b4[i]);
        } else if (i == field_0x6cd) {
            setSelectItem(i, getItem(field_0x6cb, 0));
        } else {
            setSelectItem(i, getItem(field_0x6b4[i], field_0x6b8[i]));
        }
    }
    if (mXButtonSlot != fpcNm_ITEM_NONE) {
        field_0x518[0] = mItemSlotPosX[mXButtonSlot];
        field_0x528[0] = mItemSlotPosY[mXButtonSlot];
    }
    if (mYButtonSlot != fpcNm_ITEM_NONE) {
        field_0x518[1] = mItemSlotPosX[mYButtonSlot];
        field_0x528[1] = mItemSlotPosY[mYButtonSlot];
    }
    if (field_0x6ac != fpcNm_ITEM_NONE) {
        field_0x518[2] = mItemSlotPosX[field_0x6ac];
        field_0x528[2] = mItemSlotPosY[field_0x6ac];
    }
    if (field_0x6ad != fpcNm_ITEM_NONE) {
        field_0x518[3] = mItemSlotPosX[field_0x6ad];
        field_0x528[3] = mItemSlotPosY[field_0x6ad];
    }
    if (field_0x6b3 == 0) {
        field_0x538[0] = g_ringHIO.mSelectItemScale;
        field_0x538[1] = g_ringHIO.mUnselectItemScale;
        if (field_0x6b4[0] != dComIfGs_getSelectItemIndex(0) ||
            field_0x6b8[0] != dComIfGs_getMixItemIndex(0))
        {
            field_0x674[0] = 1;
        }
    } else if (field_0x6b3 == 1) {
        field_0x538[0] = g_ringHIO.mUnselectItemScale;
        field_0x538[1] = g_ringHIO.mSelectItemScale;
        if (field_0x6b4[1] != dComIfGs_getSelectItemIndex(1) ||
            field_0x6b8[1] != dComIfGs_getMixItemIndex(1))
        {
            field_0x674[1] = 1;
        }
    }
    if (field_0x674[0] == 1) {
        if (i_useVibrationM) {
            dMeter2Info_set2DVibrationM();
        }
        Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (field_0x674[1] == 1) {
        if (i_useVibrationM) {
            dMeter2Info_set2DVibrationM();
        }
        Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (field_0x674[2] == 1) {
        if (i_useVibrationM) {
            dMeter2Info_set2DVibrationM();
        }
        Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else if (field_0x674[3] == 1) {
        if (i_useVibrationM) {
            dMeter2Info_set2DVibrationM();
        }
        Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_SET_B, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    } else {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
}

void dMenu_Ring_c::setScale() {
    u32 itemId;
    for (int i = 0; i < mItemsTotal; i++) {
        if (field_0x6cf != 0xff) {
            itemId = 0;
            switch (field_0x6cf) {
            case 0:
                itemId = 0x4DE;
                break;
            case 1:
                itemId = 0x4E0;
                break;
            }
            setNameString(itemId);
            setItemScale(i, g_ringHIO.mUnselectItemScale);
            for (int j = 0; j < 2; j++) {
                if (j == field_0x6cf) {
                    setButtonScale(j, g_ringHIO.mSelectButtonScale);
                } else {
                    setButtonScale(j, g_ringHIO.mUnselectButtonScale);
                }
            }
        } else {
            if (i == mCurrentSlot && (mStatus == STATUS_WAIT || mStatus == STATUS_EXPLAIN || mStatus == STATUS_EXPLAIN_FORCE)) {
                itemId = dComIfGs_getItem(mItemSlots[i], false) + 0x165;
                if (dMeter2Info_getRentalBombBag() != 0xff &&
                    mItemSlots[i] == dMeter2Info_getRentalBombBag() + 0xf)
                {
                    itemId = 0x16D;
                }
                setNameString(itemId);
                setItemScale(i, g_ringHIO.mSelectItemScale);
            } else {
                setItemScale(i, g_ringHIO.mUnselectItemScale);
            }
            for (int j = 0; j < 2; j++) {
                setButtonScale(j, g_ringHIO.mUnselectButtonScale);
            }
        }
    }
}

void dMenu_Ring_c::setNameString(u32 i_stringID) {
    J2DTextBox* textBox[4];
#if VERSION == VERSION_GCN_JPN
    textBox[0] = (J2DTextBox*)mpCenterScreen->search('item_n04');
    textBox[1] = (J2DTextBox*)mpCenterScreen->search('item_n05');
    textBox[2] = (J2DTextBox*)mpCenterScreen->search('item_n06');
    textBox[3] = (J2DTextBox*)mpCenterScreen->search('item_n07');
#else
    textBox[0] = (J2DTextBox*)mpCenterScreen->search('fitem_n1');
    textBox[1] = (J2DTextBox*)mpCenterScreen->search('fitem_n2');
    textBox[2] = (J2DTextBox*)mpCenterScreen->search('fitem_n3');
    textBox[3] = (J2DTextBox*)mpCenterScreen->search('fitem_n4');
#endif
    if (mNameStringID != i_stringID) {
        for (int i = 0; i < 4; i++) {
            if (i_stringID == 0) {
                strcpy(textBox[i]->getStringPtr(), "");
            } else {
                mpString->getString(i_stringID, textBox[i], NULL, NULL, NULL, 0);
            }
        }
        mNameStringID = i_stringID;
    }
}

void dMenu_Ring_c::setActiveCursor() {
    u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], false);
    if (mStatus == STATUS_WAIT && mOldStatus != STATUS_EXPLAIN_FORCE && mOldStatus != STATUS_EXPLAIN && mpItemExplain->getStatus() == 0) {
        if (mDoCPd_c::getTrigR(PAD_1) && !mPlayerIsWolf && item != fpcNm_ITEM_NONE) {
            for (int i = 0; i < MAX_SELECT_ITEM; i++) {
                setSelectItemForce(i);
            }
            setMixItem();
        } else if (mDoCPd_c::getTrigX(PAD_1) && !mPlayerIsWolf && item != fpcNm_ITEM_NONE) {
            for (int i = 0; i < MAX_SELECT_ITEM; i++) {
                setSelectItemForce(i);
            }
            field_0x6b3 = 0;
            if (!checkCombineBomb(field_0x6b3)) {
                setItem();
                if (mpItemExplain->getStatus() == 0) {
                    setStatus(STATUS_WAIT);
                    (this->*stick_init[mStatus])();
                }
            }
        } else if (mDoCPd_c::getTrigY(PAD_1) && !mPlayerIsWolf && item != fpcNm_ITEM_NONE) {
            for (int i = 0; i < MAX_SELECT_ITEM; i++) {
                setSelectItemForce(i);
            }
            field_0x6b3 = 1;
            if (!checkCombineBomb(field_0x6b3)) {
                setItem();
                if (mpItemExplain->getStatus() == 0) {
                    setStatus(STATUS_WAIT);
                    (this->*stick_init[mStatus])();
                }
            }
        } else if (mDoCPd_c::getTrigX(PAD_1) || mDoCPd_c::getTrigY(PAD_1)) {
            // If the player is a wolf or somehow manages to access an item slot with no item, error
            Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
}

void dMenu_Ring_c::setMixItem() {
    u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], false);
    bool bVar1 = false;
    u8 selectItemIndex0 = dComIfGs_getSelectItemIndex(0);
    u8 selectItemIndex1 = dComIfGs_getSelectItemIndex(1);
    u8 local_28[4] = {fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE, fpcNm_ITEM_NONE};

    if (dComIfGs_getMixItemIndex(0) == SLOT_4 &&
        mItemSlots[mCurrentSlot] == dComIfGs_getSelectItemIndex(0))
    {
        Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_COMBINE_OFF, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        field_0x6cb = selectItemIndex0;
        selectItemIndex0 = 4;
        local_28[0] = getCursorPos(4);
        field_0x6b8[0] = 0xff;
        field_0x6b3 = 0;
        field_0x6cd = 0;
        bVar1 = true;
    } else if (dComIfGs_getMixItemIndex(1) == 4 &&
               mItemSlots[mCurrentSlot] == dComIfGs_getSelectItemIndex(1))
    {
        Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_COMBINE_OFF, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        field_0x6cb = selectItemIndex1;
        selectItemIndex1 = 4;
        local_28[1] = getCursorPos(4);
        field_0x6b8[1] = 0xff;
        field_0x6b3 = 1;
        field_0x6cd = 1;
        bVar1 = true;
    } else {
        switch (item) {
        case fpcNm_ITEM_NORMAL_BOMB:
        case fpcNm_ITEM_WATER_BOMB:
        case fpcNm_ITEM_POKE_BOMB:
        case fpcNm_ITEM_HAWK_EYE:
            if ((dComIfGs_getSelectItemIndex(0) == 4 && dComIfGs_getMixItemIndex(0) == fpcNm_ITEM_NONE) ||
                (dComIfGs_getMixItemIndex(0) == 4))
            {
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_COMBINE_ON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
                selectItemIndex0 = mItemSlots[mCurrentSlot];
                field_0x6b8[0] = 4;
                field_0x6b3 = 0;
                mXButtonSlot = mCurrentSlot;
                field_0x6cd = 0xff;
                bVar1 = true;
                if (selectItemIndex1 == mItemSlots[mCurrentSlot]) {
                    selectItemIndex1 = 0xff;
                    mYButtonSlot = 0xff;
                }
            } else if ((dComIfGs_getSelectItemIndex(1) == 4 &&
                        dComIfGs_getMixItemIndex(1) == fpcNm_ITEM_NONE) ||
                       (dComIfGs_getMixItemIndex(1) == 4))
            {
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_COMBINE_ON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
                selectItemIndex1 = mItemSlots[mCurrentSlot];
                field_0x6b8[1] = 4;
                field_0x6b3 = 1;
                mYButtonSlot = mCurrentSlot;
                field_0x6cd = 0xff;
                bVar1 = true;
                if (selectItemIndex0 == mItemSlots[mCurrentSlot]) {
                    selectItemIndex0 = 0xff;
                    mXButtonSlot = 0xff;
                }
            }
            break;
        }
    }
    if (bVar1) {
        field_0x6b4[0] = selectItemIndex0;
        field_0x6b4[1] = selectItemIndex1;
        setJumpItem(false);
        if (local_28[0] != fpcNm_ITEM_NONE) {
            mXButtonSlot = local_28[0];
        }
        if (local_28[1] != fpcNm_ITEM_NONE) {
            mYButtonSlot = local_28[1];
        }
    }
}

void dMenu_Ring_c::drawItem() {
    field_0x684++;
    if (field_0x684 >= g_ringHIO.mItemAlphaFlashDuration) {
        field_0x684 = 0;
    }
    s32 halfFlashDuration = g_ringHIO.mItemAlphaFlashDuration / 2;
    f32 fVar16;
    if (field_0x684 < halfFlashDuration) {
        fVar16 = field_0x684 / (f32)halfFlashDuration;
    } else {
        fVar16 = (g_ringHIO.mItemAlphaFlashDuration - field_0x684) / (f32)halfFlashDuration;
    }
    f32 ringAlpha =
        (g_ringHIO.mItemAlphaMin + fVar16 * (g_ringHIO.mItemAlphaMax - g_ringHIO.mItemAlphaMin));
    for (int i = 0; i < mItemsTotal; i++) {
        if (i != mCurrentSlot || (mStatus != STATUS_WAIT && mStatus != STATUS_EXPLAIN && mStatus != STATUS_EXPLAIN_FORCE)) {
            J2DDrawFrame(mItemSlotPosX[i] - 24.0f + mCenterPosX, mItemSlotPosY[i] - 24.0f + mCenterPosY,
                         48.0f, 48.0f, g_ringHIO.mItemFrame[g_ringHIO.UNSELECT_FRAME], 6);
            f32 fVar17 = 1.0f;
            if (i != mCurrentSlot) {
                fVar17 = ringAlpha / 255.0f;
            }
            for (int j = 0; j < 3; j++) {
                if (mpItemTex[i][j] != NULL) {
                    if (mPlayerIsWolf) {
                        mpItemTex[i][j]->setAlpha(g_ringHIO.mItemIconAlpha_Wolf * mAlphaRate);
                    } else {
                        mpItemTex[i][j]->setAlpha(g_ringHIO.mItemIconAlpha * mAlphaRate * fVar17);
                    }
                    f32 f0 = mItemSlotParam1[i] * 48.0f;
                    f32 f1 = mItemSlotParam2[i] * 48.0f;
                    f32 x = (48.0f - f0) * 0.5f + (mItemSlotPosX[i] - 24.0f + mCenterPosX);
                    f32 y = (48.0f - f1) * 0.5f + (mItemSlotPosY[i] - 24.0f + mCenterPosY);
                    mpItemTex[i][j]->draw(x, y, f0, f1, 0, 0, 0);
                    u8 item = dComIfGs_getItem(mItemSlots[i], false);
                    if ((j == 0 && item != fpcNm_ITEM_BEE_CHILD) || (j == 2 && item == fpcNm_ITEM_BEE_CHILD)) {
                        u8 itemNum = getItemNum(mItemSlots[i]);
                        u8 itemMaxNum = getItemMaxNum(mItemSlots[i]);
                        if (itemMaxNum != 0) {
                            // If it's an ammo-based item, display ammo digits
                            drawNumber(itemNum, itemMaxNum, x + 24.0f, y + 48.0f);
                        }
                    }
                    if (j == 0 && item == fpcNm_ITEM_KANTERA /* Lantern */) {
                        setKanteraPos(x + 24.0f + 15.0f, y + 48.0f + 10.0f);
                        mpKanteraMeter->setScale(0.64f, 0.64f);
                        mpKanteraMeter->setNowGauge(dComIfGs_getMaxOil(), dComIfGs_getOil());
                        u8 alpha = mpItemTex[i][j]->getAlpha();
                        mpKanteraMeter->setAlphaRate(alpha / 255.0f);
                        mpKanteraMeter->drawSelf();
                    }
                }
            }
        }
    }
}

void dMenu_Ring_c::drawItem2() {
    s32 idx = mCurrentSlot;
    if (mStatus == STATUS_WAIT || mStatus == STATUS_EXPLAIN || mStatus == STATUS_EXPLAIN_FORCE) {
        J2DDrawFrame(mItemSlotPosX[idx] - 24.0f + mCenterPosX, mItemSlotPosY[idx] - 24.0f + mCenterPosY,
                     48.0f, 48.0f, g_ringHIO.mItemFrame[g_ringHIO.SELECT_FRAME], 6);

        for (int i = 0; i < 3; i++) {
            if (mpItemTex[idx][i] != NULL) {
                if (mPlayerIsWolf != 0) {
                    mpItemTex[idx][i]->setAlpha(g_ringHIO.mItemIconAlpha_Wolf * mAlphaRate);
                } else {
                    mpItemTex[idx][i]->setAlpha(mAlphaRate * 255.0f);
                }

                f32 f0 = mItemSlotParam1[idx] * 48.0f;
                f32 f1 = mItemSlotParam2[idx] * 48.0f;
                f32 x = (48.0f - f0) * 0.5f + (mItemSlotPosX[idx] - 24.0f + mCenterPosX);
                f32 y = (48.0f - f1) * 0.5f + (mItemSlotPosY[idx] - 24.0f + mCenterPosY);
                mpItemTex[idx][i]->draw(x, y, f0, f1, 0, 0, 0);
                u8 item = dComIfGs_getItem(mItemSlots[idx], false);
                if ((i == 0 && item != fpcNm_ITEM_BEE_CHILD) || (i == 2 && item == fpcNm_ITEM_BEE_CHILD)) {
                    u8 itemNum = getItemNum(mItemSlots[idx]);
                    u8 itemMaxNum = getItemMaxNum(mItemSlots[idx]);
                    if (itemMaxNum != 0) {
                        // If it's an ammo-based item, display ammo digits
                        drawNumber(itemNum, itemMaxNum, x + 24.0f, y + 48.0f);
                    }
                }
                if (i == 0 && item == fpcNm_ITEM_KANTERA) {
                    setKanteraPos(x + 24.0f + 15.0f, y + 48.0f + 10.0f);
                    mpKanteraMeter->setScale(0.64f, 0.64f);
                    mpKanteraMeter->setNowGauge(dComIfGs_getMaxOil(), dComIfGs_getOil());
                    u8 alpha = mpItemTex[idx][i]->getAlpha();
                    mpKanteraMeter->setAlphaRate(alpha / 255.0f);
                    mpKanteraMeter->drawSelf();
                }
            }
        }
    }
}

void dMenu_Ring_c::stick_wait_init() {
    if (mDoCPd_c::getHoldL(PAD_1) != 0) {
        if (mDirectSelectActive) {
            mWaitFrames = g_ringHIO.mDirectSelectWaitFrames;
        } else {
            mWaitFrames = 0;
        }
    } else {
        mWaitFrames = g_ringHIO.mCursorChangeWaitFrames;
    }
    field_0x63a = 0;
    mDirectSelectActive = false;
}

void dMenu_Ring_c::stick_wait_proc() {
    u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], false);

    if (item != fpcNm_ITEM_NONE) {
        setDoStatus(0x24);
    } else {
        setDoStatus(0);
    }
    if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xe) && openExplain(item)) {
        dMeter2Info_setItemExplainWindowStatus(1);
        field_0x6c4 = mCurrentSlot;
        setStatus(STATUS_EXPLAIN);
        dMeter2Info_set2DVibration();
        setDoStatus(0);
        return;
    }
    if (dMw_A_TRIGGER() && !dMeter2Info_isTouchKeyCheck(0xe)) {
        Z2GetAudioMgr()->seStart(Z2SE_SYS_ERROR, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
    if (mWaitFrames > 0) {
        mWaitFrames--;
    } else if (getStickInfo(mpStick) != 0) {
        setStatus(STATUS_MOVE);
        field_0x6b2 = 0;
    }
}

void dMenu_Ring_c::stick_move_init() {
    if (mCursorSpeed == 0) {
        mCursorSpeed = g_ringHIO.mCursorInitSpeed;
    } else if (mCursorSpeed < g_ringHIO.mCursorMax) {
        mCursorSpeed += g_ringHIO.mCursorAccel;
        if (mCursorSpeed > g_ringHIO.mCursorMax) {
            mCursorSpeed = g_ringHIO.mCursorMax;
        }
    }
    field_0x63a = 0;
    Z2GetAudioMgr()->seStart(Z2SE_ITEM_RING_ROLL, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

void dMenu_Ring_c::stick_move_proc() {
    setDoStatus(0x24);
    if (mDirectSelectActive) {
        cXyz target;
        target.set(mItemSlotPosX[mCurrentSlot], 0.0f, mItemSlotPosY[mCurrentSlot]);
        cLib_addCalcPosXZ(&mDirectSelectCursorPos, target, 1.0f, 70.0f, 1.0f);
        cXyz sub = mDirectSelectCursorPos - target;
        if (sub.abs() < 0.5f) {
            mDirectSelectCursorPos.set(target);
            field_0x66e = field_0x670;
            mpDrawCursor->setPos(mItemSlotPosX[mCurrentSlot], mItemSlotPosY[mCurrentSlot]);
            u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], false);
            if (item != fpcNm_ITEM_NONE) {
                mpDrawCursor->setParam(mItemSlotParam1[mCurrentSlot], mItemSlotParam2[mCurrentSlot], 0.1f,
                                       0.6f, 0.5f);
            } else {
                mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.6f, 0.5f);
            }
            setStatus(field_0x6b2);
        } else {
            mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.6f, 0.5f);
            mpDrawCursor->setPos(mDirectSelectCursorPos.x, mDirectSelectCursorPos.z);
        }
    } else {
        if (field_0x6d3 == 0xff) {
            cLib_addCalcAngleS(&field_0x66e, field_0x670, 4, 0x7FFF, mCursorSpeed);
        } else {
            if (field_0x6d3 == 0) {
                field_0x66e = -0x2007;
            } else {
                field_0x66e = -0x6003;
            }
            field_0x6d3 = 0xff;
        }
        s16 subtract = field_0x670 - field_0x66e;
        if (abs(subtract) < 0x80) {
            field_0x66e = field_0x670;
            mpDrawCursor->setPos(mItemSlotPosX[mCurrentSlot], mItemSlotPosY[mCurrentSlot]);
            u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], false);
            if (item != fpcNm_ITEM_NONE) {
                mpDrawCursor->setParam(mItemSlotParam1[mCurrentSlot], mItemSlotParam2[mCurrentSlot], 0.1f,
                                       0.6f, 0.5f);
            } else {
                mpDrawCursor->setParam(1.0f, 1.0f, 0.1f, 0.6f, 0.5f);
            }
            setStatus(field_0x6b2);
        } else {
            f32 itemRingPosX =
                g_ringHIO.mItemRingPosX + 304.0f + mRingRadiusH * cM_ssin(field_0x66e);
            f32 itemRingPosY =
                g_ringHIO.mItemRingPosY + 224.0f + mRingRadiusV * cM_scos(field_0x66e);
            mpDrawCursor->setPos(itemRingPosX, itemRingPosY);
        }
    }
}

void dMenu_Ring_c::stick_explain_init() {
    /* empty function */
}

void dMenu_Ring_c::stick_explain_force_init() {
    /* empty function */
}

void dMenu_Ring_c::stick_explain_proc() {
    mpItemExplain->move();
    if (mpItemExplain->getStatus() == 0) {
        if (dMeter2Info_getWarpStatus() == 1) {
            dMeter2Info_warpInProc();
        } else if (dMeter2Info_getWarpStatus() == 2) {
            dMeter2Info_warpInProc();
        }
        dMeter2Info_setItemExplainWindowStatus(0);
        setStatus(STATUS_WAIT);
    }
    f32 alphaRatio = mpItemExplain->getAlphaRatio();
    mpTextParent[1]->setAlphaRate(alphaRatio);
    mpBlackTex->setAlpha((1.0f - alphaRatio) * 150.0f);
}

void dMenu_Ring_c::stick_explain_force_proc() {
    mpItemExplain->move();
    u8 endButton = mpItemExplain->checkEndButton();
    if (endButton == 1) {
        setCombineBomb(field_0x6b3);
    } else if (endButton == 2) {
        u8 itemMaxNum = getItemMaxNum(mItemSlots[mCurrentSlot]);
        u8 itemNum = getItemNum(mItemSlots[mCurrentSlot]);
        mpItemExplain->openExplain(mItemSlots[mCurrentSlot], itemNum, itemMaxNum, true);
        setStatus(STATUS_WAIT);
    }
    f32 alphaRatio = mpItemExplain->getAlphaRatio();
    mpTextParent[1]->setAlphaRate(alphaRatio);
    mpBlackTex->setAlpha((1.0f - alphaRatio) * 150.0f);
}

void dMenu_Ring_c::setSelectItem(int i_idx, u8 i_itemNo) {
    f32 texScale = 1.0f;

    if (i_itemNo != fpcNm_ITEM_NONE) {
        if (field_0x6be[i_idx] == 0) {
            field_0x6be[i_idx] = 1;
        } else {
            field_0x6be[i_idx] = 0;
        }
        field_0x686[i_idx] = dMeter2Info_readItemTexture(
            i_itemNo, mpSelectItemTexBuf[i_idx][field_0x6be[i_idx]][0], mpSelectItemTex[i_idx][0],
            mpSelectItemTexBuf[i_idx][field_0x6be[i_idx]][1], mpSelectItemTex[i_idx][1],
            mpSelectItemTexBuf[i_idx][field_0x6be[i_idx]][2], mpSelectItemTex[i_idx][2], NULL, NULL,
            -1);
        texScale = dItem_data::getTexScale(i_itemNo) / 100.0f;
    }
    field_0x548[i_idx] = mpSelectItemTexBuf[i_idx][field_0x6be[i_idx]][0]->width / 48.0f * texScale;
    field_0x558[i_idx] =
        mpSelectItemTexBuf[i_idx][field_0x6be[i_idx]][0]->height / 48.0f * texScale;
}

void dMenu_Ring_c::drawSelectItem() {
    for (int i = 0; i < 4; i++) {
        if (field_0x674[i] != 0) {
            if (field_0x674[i] < 10) {
                f32 initSizeX = dMeter2Info_getMeterItemPanePtr(i)->getInitSizeX() * 1.7f;
                f32 initSizeY = dMeter2Info_getMeterItemPanePtr(i)->getInitSizeY() * 1.7f;
                f32 initScaleX = dMeter2Info_getMeterItemPanePtr(i)->getInitScaleX();
                f32 initScaleY = dMeter2Info_getMeterItemPanePtr(i)->getInitScaleY();
                Vec pos = dMeter2Info_getMeterItemPanePtr(i)->getGlobalVtxCenter(
                    dMeter2Info_getMeterItemPanePtr(i)->mPane, true, 0);

                f32 fVar14 = field_0x674[i] / 10.0f;
                if (field_0x6cd != 0xff) {
                    fVar14 = 1.0f - fVar14;
                }
                initSizeX = (initSizeX - 48.0f) * fVar14 + 48.0f;
                initSizeY = (initSizeY - 48.0f) * fVar14 + 48.0f;
                f32 fVar3 = field_0x538[i] + fVar14 * (initScaleX - field_0x538[i]);
                f32 fVar4 = field_0x538[i] + fVar14 * (initScaleY - field_0x538[i]);
                f32 fVar5 = field_0x518[i] + fVar14 * (pos.x - field_0x518[i]);
                f32 fVar6 = field_0x528[i] + fVar14 * (pos.y - field_0x528[i]);

                for (int j = 0; j < field_0x686[i]; j++) {
                    if (mpSelectItemTex[i][j] != NULL) {
                        mpSelectItemTex[i][j]->setAlpha(mAlphaRate * 255.0f);
                        f32 f3 = field_0x548[i] * initSizeX * fVar3;
                        f32 f4 = field_0x558[i] * initSizeY * fVar4;
                        mpSelectItemTex[i][j]->draw(fVar5 + (initSizeX - f3) * 0.5f,
                                                    fVar6 + (initSizeY - f4) * 0.5f, f3, f4, 0, 0,
                                                    0);
                    }
                }
                field_0x674[i]++;
            } else {
                setSelectItemForce(i);
            }
        }
    }
}

void dMenu_Ring_c::setSelectItemForce(int i_idx) {
    if (i_idx == 2) {
        if (field_0x674[i_idx] != 0) {
            dComIfGs_setSelectItemIndex(i_idx, field_0x6b4[i_idx]);
            field_0x674[i_idx] = 0;
        }
    } else if (field_0x674[i_idx] != 0) {
        for (int i = 0; i < 2; i++) {
            dComIfGs_setMixItemIndex(i, field_0x6b8[i]);
            dComIfGs_setSelectItemIndex(i, field_0x6b4[i]);
        }
        field_0x674[i_idx] = 0;
    }
}

u8 dMenu_Ring_c::getCursorPos(u8 i_slotNo) {
    for (s32 i = 0; i < mItemsTotal; i++) {
        if (i_slotNo == dComIfGs_getLineUpItem(i)) {
            return i;
        }
    }
    return 0xff;
}

/** @details
 * Returns current ammo depending on the current item slot the cursor is on
 * This can be:
 *  - Ammo of any bomb type
 *  - Number of bee larvae in a bottle
 *  - Bow ammo
 *  - Slingshot ammo
*/
u8 dMenu_Ring_c::getItemNum(u8 i_slotNo) {
    u8 item = dComIfGs_getItem(i_slotNo, false);
    u8 ret = 0;

    switch (item) {
    case fpcNm_ITEM_BOMB_BAG_LV1:
        ret = 0;
        break;
    case fpcNm_ITEM_NORMAL_BOMB:
    case fpcNm_ITEM_WATER_BOMB:
    case fpcNm_ITEM_POKE_BOMB:
        ret = dComIfGs_getBombNum(i_slotNo - 0xF);
        break;

    case fpcNm_ITEM_BEE_CHILD:
        ret = dComIfGs_getBottleNum(i_slotNo - 0xB);
        break;
    case fpcNm_ITEM_BOW:
    case fpcNm_ITEM_LIGHT_ARROW:
    case fpcNm_ITEM_ARROW_LV1:
    case fpcNm_ITEM_ARROW_LV2:
    case fpcNm_ITEM_ARROW_LV3:
        ret = dComIfGs_getArrowNum();
        break;
    case fpcNm_ITEM_PACHINKO:
        ret = dComIfGs_getPachinkoNum();
        break;
    }
    return ret;
}

/** @details
 * Returns maximum capacity obtained depending on the currently selected item slot
 * This can be:
 *  - Capacity of any bomb type
 *  - Capacity of bee larvae in a bottle
 *  - Bow capacity
 *  - Slingshot capacity
*/
u8 dMenu_Ring_c::getItemMaxNum(u8 i_slotNo) {
    u8 item = dComIfGs_getItem(i_slotNo, false);
    u8 ret = 0;

    switch (item) {
    case fpcNm_ITEM_BOMB_BAG_LV1:
        ret = 1;
        break;
    case fpcNm_ITEM_NORMAL_BOMB:
    case fpcNm_ITEM_WATER_BOMB:
    case fpcNm_ITEM_POKE_BOMB:
        ret = dComIfGs_getBombMax(item);
        break;

    case fpcNm_ITEM_BEE_CHILD:
        ret = dComIfGs_getBottleMax();
        break;
    case fpcNm_ITEM_BOW:
    case fpcNm_ITEM_LIGHT_ARROW:
    case fpcNm_ITEM_ARROW_LV1:
    case fpcNm_ITEM_ARROW_LV2:
    case fpcNm_ITEM_ARROW_LV3:
        ret = dComIfGs_getArrowMax();
        break;
    case fpcNm_ITEM_PACHINKO:
        ret = dComIfGs_getPachinkoMax();
        break;
    }
    return ret;
}

bool dMenu_Ring_c::checkExplainForce() {
    u8 local_18[4];

    u8 item0 = dComIfGs_getItem(dComIfGs_getSelectItemIndex(0), false);
    u8 item1 = dComIfGs_getItem(dComIfGs_getSelectItemIndex(1), false);
    u8 item2 = dComIfGs_getItem(dComIfGs_getSelectItemIndex(2), false);
    u8 item3 = dComIfGs_getItem(dComIfGs_getSelectItemIndex(3), false);
    u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], true);

    for (int i = 0; i < 4; i++) {
        local_18[i] = fpcNm_ITEM_NONE;
    }

    switch (item) {
    case fpcNm_ITEM_BOW:
        switch (item0) {
        case fpcNm_ITEM_NORMAL_BOMB:
        case fpcNm_ITEM_WATER_BOMB:
        case fpcNm_ITEM_POKE_BOMB:
            local_18[0] = fpcNm_ITEM_BOMB_ARROW;
            break;
        case fpcNm_ITEM_HAWK_EYE:
            local_18[0] = fpcNm_ITEM_HAWK_ARROW;
            break;
        }
        switch (item1) {
        case fpcNm_ITEM_NORMAL_BOMB:
        case fpcNm_ITEM_WATER_BOMB:
        case fpcNm_ITEM_POKE_BOMB:
            local_18[1] = fpcNm_ITEM_BOMB_ARROW;
            break;
        case fpcNm_ITEM_HAWK_EYE:
            local_18[1] = fpcNm_ITEM_HAWK_ARROW;
            break;
        }
        break;
    case fpcNm_ITEM_NORMAL_BOMB:
    case fpcNm_ITEM_WATER_BOMB:
    case fpcNm_ITEM_POKE_BOMB:
        if (item0 == fpcNm_ITEM_BOW) {
            local_18[0] = fpcNm_ITEM_BOMB_ARROW;
        } else if (item1 == fpcNm_ITEM_BOW) {
            local_18[1] = fpcNm_ITEM_BOMB_ARROW;
        }
        break;
    case fpcNm_ITEM_HAWK_EYE:
        if (item0 == fpcNm_ITEM_BOW) {
            local_18[0] = fpcNm_ITEM_HAWK_ARROW;
        } else if (item1 == fpcNm_ITEM_BOW) {
            local_18[1] = fpcNm_ITEM_HAWK_ARROW;
        }
        break;
    case fpcNm_ITEM_BEE_CHILD:
        if (item0 == fpcNm_ITEM_FISHING_ROD_1) {
            local_18[0] = fpcNm_ITEM_BEE_ROD;
        } else if (item1 == fpcNm_ITEM_FISHING_ROD_1) {
            local_18[1] = fpcNm_ITEM_BEE_ROD;
        }
        break;
    case fpcNm_ITEM_WORM:
        if (item0 == fpcNm_ITEM_FISHING_ROD_1) {
            local_18[0] = fpcNm_ITEM_WORM_ROD;
        } else if (item1 == fpcNm_ITEM_FISHING_ROD_1) {
            local_18[1] = fpcNm_ITEM_WORM_ROD;
        }
        break;
    case fpcNm_ITEM_ZORAS_JEWEL:
        if (item0 == fpcNm_ITEM_FISHING_ROD_1) {
            local_18[0] = fpcNm_ITEM_JEWEL_ROD;
        } else if (item1 == fpcNm_ITEM_FISHING_ROD_1) {
            local_18[1] = fpcNm_ITEM_JEWEL_ROD;
        }
        break;
    case fpcNm_ITEM_FISHING_ROD_1:
        if (item0 == fpcNm_ITEM_BEE_CHILD) {
            local_18[0] = fpcNm_ITEM_BEE_ROD;
        } else if (item1 == fpcNm_ITEM_BEE_CHILD) {
            local_18[1] = fpcNm_ITEM_BEE_ROD;
        } else if (item0 == fpcNm_ITEM_ZORAS_JEWEL) {
            local_18[0] = fpcNm_ITEM_JEWEL_ROD;
        } else if (item1 == fpcNm_ITEM_ZORAS_JEWEL) {
            local_18[1] = fpcNm_ITEM_JEWEL_ROD;
        } else if (item0 == fpcNm_ITEM_WORM) {
            local_18[0] = fpcNm_ITEM_WORM_ROD;
        } else if (item1 == fpcNm_ITEM_WORM) {
            local_18[1] = fpcNm_ITEM_WORM_ROD;
        }
        break;
    }

    if (local_18[0] != fpcNm_ITEM_NONE && local_18[1] == fpcNm_ITEM_NONE && local_18[2] == fpcNm_ITEM_NONE && local_18[3] == fpcNm_ITEM_NONE &&
        dComIfGs_getMixItemIndex(0) == fpcNm_ITEM_NONE)
    {
        field_0x6c7[0] = local_18[0];
        field_0x6c7[1] = fpcNm_ITEM_NONE;
        field_0x6c7[2] = fpcNm_ITEM_NONE;
        field_0x6c7[3] = fpcNm_ITEM_NONE;
    } else if (local_18[0] == fpcNm_ITEM_NONE && local_18[1] != fpcNm_ITEM_NONE && local_18[2] == fpcNm_ITEM_NONE &&
               local_18[3] == fpcNm_ITEM_NONE && dComIfGs_getMixItemIndex(1) == fpcNm_ITEM_NONE)
    {
        field_0x6c7[0] = fpcNm_ITEM_NONE;
        field_0x6c7[1] = local_18[1];
        field_0x6c7[2] = fpcNm_ITEM_NONE;
        field_0x6c7[3] = fpcNm_ITEM_NONE;
    } else {
        field_0x6c7[0] = fpcNm_ITEM_NONE;
        field_0x6c7[1] = fpcNm_ITEM_NONE;
        field_0x6c7[2] = fpcNm_ITEM_NONE;
        field_0x6c7[3] = fpcNm_ITEM_NONE;
    }
    return 0;
}

bool dMenu_Ring_c::checkCombineBomb(int param_0) {
    return false;
}

void dMenu_Ring_c::setCombineBomb(int param_0) {
    /* empty function */
}

void dMenu_Ring_c::drawNumber(int i_itemNum, int i_itemMaxNum, f32 i_posX, f32 i_posY) {
    if (i_itemNum > 100) {
        i_itemNum = 100;
    }

    JUtility::TColor colorBlack;
    JUtility::TColor colorWhite;

    if (i_itemNum == i_itemMaxNum) {
        // Full ammo: Golden digits
        colorBlack.set(30, 30, 30, 0);
        colorWhite.set(255, 200, 50, 255);
    } else if (i_itemNum == 0) {
        // Empty ammo: Gray digits
        colorBlack.set(30, 30, 30, 0);
        colorWhite.set(180, 180, 180, 255);
    } else {
        // White digits
        colorBlack.set(0, 0, 0, 0);
        colorWhite.set(255, 255, 255, 255);
    }
    for (int i = 0; i < 3; i++) {
        mpItemNumTex[i]->setBlackWhite(colorBlack, colorWhite);
    }
    if (i_itemNum < 100) {
        // If player has less than 100 arrows, only display 
        // the first two digits and hide the last one
        ResTIMG* texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_itemNum / 10));
        mpItemNumTex[0]->changeTexture(texture, 0);
        texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_itemNum % 10));
        mpItemNumTex[1]->changeTexture(texture, 0);
        mpItemNumTex[2]->hide(); // This function hides the last digit
    } else {
        // Player has 100 arrows, display all three digits
        ResTIMG* texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_itemNum / 100));
        mpItemNumTex[0]->changeTexture(texture, 0);
        i_itemNum = (u8)(i_itemNum % 100);
        texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_itemNum / 10));
        mpItemNumTex[1]->changeTexture(texture, 0);
        texture = (ResTIMG*)dComIfGp_getMain2DArchive()->getResource(
            'TIMG', dMeter2Info_getNumberTextureName(i_itemNum % 10));
        mpItemNumTex[2]->changeTexture(texture, 0);
        mpItemNumTex[2]->show(); // This function shows the last digit
    }

    // Gray out the item digits in the item wheel if player is wolf
    u8 itemIconAlpha =
        mPlayerIsWolf != 0 ? g_ringHIO.mItemIconAlpha_Wolf : g_ringHIO.mItemIconAlpha;

    for (int i = 0; i < 3; i++) {
        mpItemNumTex[i]->setAlpha(itemIconAlpha * mAlphaRate);

        // Even though the statement iterates through all digits, 
        // the last digit wouldn't be visible to the player with  
        // less than 100 arrows because the hide() function was used 
        mpItemNumTex[i]->draw(i_posX + i * 16.0f, i_posY - 16.0f, 16.0f, 16.0f, 0, 0, 0);
    }
}

u8 dMenu_Ring_c::getItem(int i_slot_no, u8 i_slot_no2) {
    u8 item = dComIfGs_getItem(i_slot_no, 0);
    dComIfGs_getItem(i_slot_no2, 0);
    return item;
}

void dMenu_Ring_c::setDoStatus(u8 i_doStatus) {
    if (i_doStatus == 0 && mDoStatus == 0x24) {
        if (field_0x68e > 0) {
            field_0x68e--;
            if (field_0x68e == 0) {
                mDoStatus = 0;
            }
        } else {
            field_0x68e = 10;
        }
    } else {
        mDoStatus = i_doStatus;
        field_0x68e = 0;
    }
    dComIfGp_setDoStatusForce(mDoStatus, 0);
}

bool dMenu_Ring_c::isMixItemOn() {
    if (!mPlayerIsWolf && dComIfGs_getItem(mItemSlots[mCurrentSlot], false) != fpcNm_ITEM_NONE) {
        u8 item = dComIfGs_getItem(mItemSlots[mCurrentSlot], false);
        switch (item) {
        case fpcNm_ITEM_HAWK_EYE:
        case fpcNm_ITEM_NORMAL_BOMB:
        case fpcNm_ITEM_WATER_BOMB:
        case fpcNm_ITEM_POKE_BOMB:
            if ((dComIfGs_getSelectItemIndex(0) == SLOT_4) && (dComIfGs_getMixItemIndex(0) == fpcNm_ITEM_NONE) ||
                (dComIfGs_getMixItemIndex(0) == SLOT_4))
            {
                return true;
            }
            if ((dComIfGs_getSelectItemIndex(1) == SLOT_4) && (dComIfGs_getMixItemIndex(1) == fpcNm_ITEM_NONE) ||
                (dComIfGs_getMixItemIndex(1) == SLOT_4))
            {
                return true;
            }
            break;
        }
    }
    return false;
}

bool dMenu_Ring_c::isMixItemOff() {
    if ((!mPlayerIsWolf) && (dComIfGs_getItem(mItemSlots[mCurrentSlot], 0) != fpcNm_ITEM_NONE)) {
        if ((dComIfGs_getMixItemIndex(0) == SLOT_4) &&
            (mItemSlots[mCurrentSlot] == dComIfGs_getSelectItemIndex(0)))
        {
            return 1;
        }
        if ((dComIfGs_getMixItemIndex(1) == SLOT_4) &&
            (mItemSlots[mCurrentSlot] == dComIfGs_getSelectItemIndex(1)))
        {
            return 1;
        }
    }
    return 0;
}

void dMenu_Ring_c::setMixMessage() {
    if (mpTextParent[4] != NULL) {
        if (isMixItemOff()) {
            for (int i = 0; i < 5; i++) {
                mpComboOffString[i]->hide();
                mpBowArrowComboString[i]->show();
            }
            if (!mpTextParent[4]->isVisible()) {
                mPikariFlashingSpeed = 18.0f - g_ringHIO.mPikariAnimSpeed;
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_COMBINE_ICON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
            mpTextParent[4]->show();
        } else if (isMixItemOn()) {
            for (int i = 0; i < 5; i++) {
                mpComboOffString[i]->show();
                mpBowArrowComboString[i]->hide();
            }
            if (!mpTextParent[4]->isVisible()) {
                mPikariFlashingSpeed = 18.0f - g_ringHIO.mPikariAnimSpeed;
                Z2GetAudioMgr()->seStart(Z2SE_SY_ITEM_COMBINE_ICON, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
            mpTextParent[4]->show();
        } else {
            mpTextParent[4]->hide();
            mPikariFlashingSpeed = 0.0f;
        }
    }
}

void dMenu_Ring_c::textScaleHIO() {
    for (int i = 0; i < 10; i++) {
        if (mpTextParent[i] != NULL && i != 2) {
            if (mRingGuidePosX[i] != g_ringHIO.mGuidePosX[i] ||
                mRingGuidePosY[i] != g_ringHIO.mGuidePosY[i])
            {
                mRingGuidePosX[i] = g_ringHIO.mGuidePosX[i];
                mRingGuidePosY[i] = g_ringHIO.mGuidePosY[i];
                if (i == 8) {
                    mpTextParent[i]->paneTrans(mRingGuidePosX[i] + field_0x574[0], mRingGuidePosY[i]);
                } else if (i == 9) {
                    mpTextParent[i]->paneTrans(mRingGuidePosX[i] + field_0x574[1], mRingGuidePosY[i]);
                } else {
                    mpTextParent[i]->paneTrans(mRingGuidePosX[i], mRingGuidePosY[i]);
                }
                if (mpTextParent[2] != NULL && i == 1) {
                    mpTextParent[2]->paneTrans(mRingGuidePosX[i], mRingGuidePosY[i]);
                }
            }
            if (mRingGuideScale[i] != g_ringHIO.mGuideScale[i]) {
                mRingGuideScale[i] = g_ringHIO.mGuideScale[i];
                if (i == 8 || i == 9) {
                    if (field_0x6cf == i - 8) {
                        setButtonScale(i - 8, g_ringHIO.mSelectButtonScale);
                    } else {
                        setButtonScale(i - 8, g_ringHIO.mUnselectButtonScale);
                    }
                } else if (i == 5 || i == 7) {
                    mpTextParent[i]->scale(mRingGuideScale[i], mRingGuideScale[i]);
                }
                if (mpTextParent[2] != NULL && i == 1) {
                    mpTextParent[2]->scale(mRingGuideScale[i], mRingGuideScale[i]);
                }
                if (i < 5 && mpTextParent[i] != NULL) {
                    mpTextParent[i]->scale(mRingGuideScale[i], mRingGuideScale[i]);
                }
            }
        }
    }
}

void dMenu_Ring_c::textCentering() {
    textScaleHIO();
}

f32 dMenu_Ring_c::clacEllipseFunction(f32 param_0, f32 param_1, f32 param_2) {
    return -JMAFastSqrt(param_2 * param_2 * (1.0f - (param_0 * param_0) / (param_1 * param_1)));
}

f32 dMenu_Ring_c::calcDistance(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    return JMAFastSqrt((param_2 - param_0) * (param_2 - param_0) +
                       (param_3 - param_1) * (param_3 - param_1));
}

void dMenu_Ring_c::clacEllipsePlotAverage(int param_0, f32 param_1, f32 param_2) {
    f32 ring_radius_h = mRingRadiusH;
    f32 ring_radius_v = mRingRadiusV;
    f32 fVar8 = 0.0f;
    f32 temp2;
    f32 fVar3;

    f32* ptr = (f32*)operator new[](16000);
    f32* ptr_00 = (f32*)operator new[](16000);
    f32* ptr_01 = (f32*)operator new[](16000);

    f32 fVar9 = 0.0f;
    for (int i = 0; i <= 1000; i++) {
        ptr[i] = fVar8;
        ptr_00[i] = clacEllipseFunction(fVar8, ring_radius_h, ring_radius_v);
        fVar8 += ring_radius_h / 1000.0f;
        ptr[i + 2000] = -ptr[i];
        ptr_00[i + 2000] = -ptr_00[i];
        if (i > 0) {
            ptr_01[i - 1] = calcDistance(ptr[i - 1], ptr_00[i - 1], ptr[i], ptr_00[i]);
            ptr_01[2000 - i] = ptr_01[i - 1];
            ptr_01[i + 1999] = ptr_01[i - 1];
            ptr_01[4000 - i] = ptr_01[i - 1];
            fVar9 = fVar9 + ptr_01[2000 - i] + ptr_01[i + 1999] + ptr_01[4000 - i] + ptr_01[i - 1];

            if (i < 1000) {
                ptr[2000 - i] = ptr[i];
                ptr_00[2000 - i] = -ptr_00[i];
                ptr[4000 - i] = -ptr[i];
                ptr_00[4000 - i] = ptr_00[i];
            }
        }
    }

    fVar8 = fVar9 / param_0;
    fVar3 = 0.0f;
    temp2 = 0.0f;
    s32 j = 0;
    mItemSlotPosX[0] = ptr[0] + param_1;
    mItemSlotPosY[0] = ptr_00[0] + param_2;

    for (int i = 0; i < 4000; i++) {
        ring_radius_v = ptr_01[i];
        fVar3 += ring_radius_v;
        if (fVar8 >= temp2 && fVar8 < fVar3) {
            j++;
            if (fVar8 - temp2 <= fVar3 - fVar8) {
                fVar3 = ptr_01[i];
                mItemSlotPosX[j] = param_1 + ptr[i - 1];
                mItemSlotPosY[j] = param_2 + ptr_00[i - 1];
            } else {
                fVar3 = 0.0f;
                mItemSlotPosX[j] = param_1 + ptr[i];
                mItemSlotPosY[j] = param_2 + ptr_00[i];
            }
        }
        ring_radius_h = fVar8;
        temp2 = fVar3;
        if (j >= param_0 - 1)
            break;
    }
    operator delete[](ptr);
    operator delete[](ptr_00);
    operator delete[](ptr_01);
}

bool dMenu_Ring_c::dpdMove() {
    return false;
}

u8 dMenu_Ring_c::openExplain(u8 param_0) {
    if (field_0x6cf == 0xff && field_0x6d0 == 0xff) {
        if (param_0 != 0xff) {
            return mpItemExplain->openExplain(mItemSlots[mCurrentSlot],
                                              getItemNum(mItemSlots[mCurrentSlot]),
                                              getItemMaxNum(mItemSlots[mCurrentSlot]), false);
        }
        return 0;
    }
    u8 idx = field_0x6d0 != 0xff ? field_0x6d0 : field_0x6cf;
    static const u32 i_nameID[2] = {0x4DE, 0x4E0};
    static const u32 i_expID[2] = {0x4DF, 0x4E1};
    return mpItemExplain->openExplainTx(i_nameID[idx], i_expID[idx]);
}
