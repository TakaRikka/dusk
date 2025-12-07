//
// Item Message Screen
//

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_item.h"
#include "d/d_msg_scrn_light.h"
#include "d/d_msg_scrn_arrow.h"
#include "d/d_msg_scrn_3select.h"
#include "d/d_msg_object.h"
#include "d/d_msg_class.h"
#include "d/d_meter2_info.h"
#include "d/d_meter_HIO.h"
#include "d/d_item_data.h"
#include "d/d_pane_class.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_lib.h"
#include "m_Do/m_Do_graphic.h"
#include "JSystem/J2DGraph/J2DAnmLoader.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JUtility/JUTTexture.h"

dMsgScrnItem_c::dMsgScrnItem_c(u8 param_1, u8 param_2, JKRExpHeap* param_3) {
    if (param_3 != NULL) {
        field_0x138 = param_3;
    } else {
        field_0x138 = dComIfGp_getSubHeap2D(7);
    }
    field_0x19e = false;
    init();
    int iVar6 = dMsgObject_getMessageID();
    bool bVar5 = false;

    #if DEBUG
    if (dMsgObject_getMessageID() >= 0x10cc && dMsgObject_getMessageID() < 0x11cc) {
        mItemIndex = dMsgObject_getMessageID() - 0x10cc;
        bVar5 = true;
    } 
    #endif
    
    if (iVar6 >= 0x645 && iVar6 <= 0x648) {
        mItemIndex = iVar6 - 0x641;
        bVar5 = true;
    } else if (iVar6 >= 0x8f5 && iVar6 <= 0x90c) {
        mItemIndex = iVar6 - 0x835;
        bVar5 = true;
    }
    if (!bVar5) {
        mItemIndex = dMsgObject_getMessageID() - 0x65;
    }
    if (mItemIndex == 0x240) {
        mItemIndex = 0x40;
    } else if (mItemIndex == 0x191e || mItemIndex == 0x402e) {
        mItemIndex = 0x46;
    } else if (mItemIndex == 0x46a || mItemIndex == 0x46b || mItemIndex == 0x46c) {
        mItemIndex = 0xe0;
    } else if (mItemIndex == 0x1d35) {
        mItemIndex = 0x21;
    } else if (mItemIndex == 0x55b || mItemIndex == 0x55c) {
        mItemIndex = 0x23;
    } else if (mItemIndex == 0x6b9 || mItemIndex == 0x6eb) {
        mItemIndex = 0x60;
    }
    if (mItemIndex == 0xec) {
        mItemIndex = 0x33;
    }
    if ((mItemIndex < 0 || 0xff < mItemIndex) && !isOugiID()) {
        if (*dComIfGp_getNextStageName() != 'T') {
            JUT_WARN(85, "Item Index Not Exist!! %d", mItemIndex);
        }
        mItemIndex = 0x43;
    }
    for (int i = 0; i < 3; i++) {
        mpItemPane[i] = 0;
        field_0x0e0[i] = 0;
        mpItemTex[i] = field_0x138->alloc(0xc00, 0x20);
        JUT_ASSERT(100, mpItemTex[i] != NULL);
    }
    int uStack_60 = 0xffffffff;
    if (mItemIndex == 0x46 || mItemIndex == 0x4c) {
        uStack_60 = 0x16;
    } else if (mItemIndex == 0x21 || mItemIndex == 0x37 ||
               mItemIndex == 0x38 ||
                 mItemIndex == 0x39 || mItemIndex == 0x3a)
    {
        switch(dComIfGs_getMaxLife() % 5) {
        case 0:
            uStack_60 = 0x75;
            break;
        case 1:
            uStack_60 = 0x76;
            break;
        case 2:
            uStack_60 = 0x77;
            break;
        case 3:
            uStack_60 = 0x78;
            break;
        case 4:
            uStack_60 = 0x51;
            break;
        }
    }

    if (isOugiID()) {
        JKRReadIdxResource(mpItemTex[0], 0xc00, 0x3d, dComIfGp_getItemIconArchive());
        field_0x0e0[0] = (ResTIMG*)mpItemTex[0];
        mpItemPane[0] = new J2DPicture(field_0x0e0[0]);
        JUT_ASSERT(148, mpItemPane[0] != NULL);
    } else {
        int texNum =
            dMeter2Info_readItemTexture(mItemIndex, mpItemTex[0], (J2DPicture*)NULL, mpItemTex[1],
                                        (J2DPicture*)NULL, mpItemTex[2], (J2DPicture*)NULL, NULL, (J2DPicture*)NULL, uStack_60);
        for (int i = 0; i < texNum; i++) {
            field_0x0e0[i] = (ResTIMG*)mpItemTex[i];
            mpItemPane[i] = new J2DPicture(field_0x0e0[i]);
            JUT_ASSERT(165, mpItemPane[i] != NULL);
        }
        dMeter2Info_setItemColor(
            mItemIndex, mpItemPane[0], mpItemPane[1],
            mpItemPane[2], NULL);
        #if DEBUG
        if (mItemIndex == 0x81 || mItemIndex == 0x23) {
            field_0x19e = true;
        }
        #endif
    }
    mpScreen = new J2DScreen();
    JUT_ASSERT(188, mpScreen != NULL);
    bool fg = mpScreen->setPriority("zelda_item_get_window.blo", 0x1020000, dComIfGp_getMsgArchive(3));
    JUT_ASSERT(195, fg != false);
    dPaneClass_showNullPane(mpScreen);
    mpTxScreen = new J2DScreen();
    JUT_ASSERT(199, mpTxScreen != NULL);
    fg = mpTxScreen->setPriority("zelda_item_get_window_text.blo", 0x20000, dComIfGp_getMsgArchive(3));
    JUT_ASSERT(206, fg != false);
    dPaneClass_showNullPane(mpTxScreen);
    f32 dVar18;
    if (g_drawHIO.mItemScaleAdjustON == 1) {
        dVar18 = g_drawHIO.mItemScalePercent / 100.0f;
    } else if (mItemIndex >= 0 && mItemIndex <= 0xff) {
        dVar18 = dItem_data::getTexScale(mItemIndex) / 100.0f;
    } else {
        dVar18 = 1.0f;
    }
    field_0x178 = field_0x0e0[0]->width * mpScreen->search('set_it_n')->getWidth() / 48.0f;
    field_0x17c = field_0x0e0[0]->height * mpScreen->search('set_it_n')->getHeight() / 48.0f;
    field_0x170 = mpScreen->search('set_it_n')->getWidth();
    field_0x174 = mpScreen->search('set_it_n')->getHeight();
    for (int i = 0; i < 3; i++) {
        if (mpItemPane[i] != NULL) {
            mpItemPane[i]->resize(field_0x178 * dVar18, field_0x17c * dVar18);
            mpItemPane[i]->setBasePosition(J2DBasePosition_4);
        }
    }
    mpArrow_c = new dMsgScrnArrow_c();
    JUT_ASSERT(280, mpArrow_c != NULL);
    mpSelect_c = new dMsgScrn3Select_c();
    JUT_ASSERT(284, mpSelect_c != NULL);
    mpLight_c = new dMsgScrnLight_c(3, param_2);
    JUT_ASSERT(288, mpLight_c != NULL);
    void* mpBuf = field_0x138->alloc(0x106a, 0x20);
    JUT_ASSERT(291, mpBuf != NULL);
    memset(mpBuf, 0, 0x106a);
    mCharInfoPtr = (CharInfo_c*)mpBuf;
    field_0x160 = mpScreen->search('n_all')->getBounds().i.x;
    field_0x164 = mpScreen->search('n_all')->getBounds().i.y;
    field_0x180 = 0.0f;
    field_0x184 = 0.0f;
    for (int i = 0; i < 3; i++) {
        field_0x188[i] = g_MsgObject_HIO_c.mBoxPos[i][3];
    }
    field_0x194 = 0.0f;
    mpArw_c = new CPaneMgr(mpScreen, 'set_ya_n', 0, NULL);
    JUT_ASSERT(306, mpArw_c != NULL);
    mpMg_c[0] = new CPaneMgr(mpScreen, 'mg_null', 0, NULL);
    JUT_ASSERT(309, mpMg_c[0] != NULL);
    mpMg_c[1] = new CPaneMgr(mpTxScreen, 'mg_null', 0, NULL);
    JUT_ASSERT(312, mpMg_c[1] != NULL);
    OSInitFastCast();
    fukiPosCalc(param_1);
    field_0x118[0] = (J2DAnmTextureSRTKey*)J2DAnmLoaderDataBase::load(JKRGetNameResource("zelda_item_get_window.btk", dComIfGp_getMsgArchive(3)));
    field_0x118[0]->searchUpdateMaterialID(mpScreen);
    field_0x140[0] = 0.0f;
    field_0x12c[0] = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(JKRGetNameResource("zelda_item_get_window.bpk", dComIfGp_getMsgArchive(3)));
    field_0x12c[0]->searchUpdateMaterialID(mpScreen);
    field_0x154[0] = 0.0f;
    field_0x12c[1] = (J2DAnmColorKey*)J2DAnmLoaderDataBase::load(JKRGetNameResource("zelda_item_get_window_02.bpk", dComIfGp_getMsgArchive(3)));
    field_0x12c[1]->searchUpdateMaterialID(mpScreen);
    field_0x154[1] = 0.0f;
    field_0x19d = false;
    mpPmP_c = new CPaneMgr(mpScreen, 'n_all', 3, NULL);
    JUT_ASSERT(389, mpPmP_c != NULL);

    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxItemScaleX, g_MsgObject_HIO_c.mBoxItemScaleY);

#if VERSION == VERSION_GCN_JPN
    if (dComIfGs_getOptUnk0() == 0) {
        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3flin', 0, NULL);
        JUT_ASSERT(407, mpTm_c[0] != NULL);

        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3f_s', 0, NULL);
        JUT_ASSERT(410, mpTm_c[1] != NULL);

        mpTm_c[2] = new CPaneMgr(mpTxScreen, 't3f_w', 0, NULL);
        JUT_ASSERT(413, mpTm_c[2] != NULL);

        mpTmr_c[0] = new CPaneMgr(mpTxScreen, 'mg_3f', 0, NULL);
        JUT_ASSERT(416, mpTmr_c[0] != NULL);

        mpTmr_c[1] = new CPaneMgr(mpTxScreen, 'mg_3f_s', 0, NULL);
        JUT_ASSERT(419, mpTmr_c[1] != NULL);

        mpTmr_c[2] = new CPaneMgr(mpTxScreen, 'mg_3f_w', 0, NULL);
        JUT_ASSERT(422, mpTmr_c[2] != NULL);

        mpTxScreen->search('n_3line')->hide();
        mpTxScreen->search('n_3fline')->show();
        mpTxScreen->search('n_e4line')->hide();
    } else {
        mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_3line', 0, NULL);
        JUT_ASSERT(407, mpTm_c[0] != NULL);
        mpTm_c[1] = new CPaneMgr(mpTxScreen, 't3_s', 0, NULL);
        JUT_ASSERT(410, mpTm_c[1] != NULL);
        mpTm_c[2] = new CPaneMgr(mpTxScreen, 't3_w', 0, NULL);
        JUT_ASSERT(413, mpTm_c[2] != NULL);
        mpTxScreen->search('n_3line')->show();
        mpTxScreen->search('n_3fline')->hide();
        mpTxScreen->search('n_e4line')->hide();
    }
#else
    mpTm_c[0] = new CPaneMgr(mpTxScreen, 'mg_e4lin', 0, NULL);
    JUT_ASSERT(407, mpTm_c[0] != NULL);
    mpTm_c[1] = new CPaneMgr(mpTxScreen, 't4_s', 0, NULL);
    JUT_ASSERT(410, mpTm_c[1] != NULL);
    mpTm_c[2] = new CPaneMgr(mpTxScreen, 't4_w', 0, NULL);
    JUT_ASSERT(413, mpTm_c[2] != NULL);
    mpTxScreen->search('n_3line')->hide();
    mpTxScreen->search('n_3fline')->hide();
    mpTxScreen->search('n_e4line')->show();
#endif

    for (int i = 0; i < 3; i++) {
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        if (mpTmr_c[i] != NULL) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setFont(mDoExt_getMesgFont());
        }
    }
    ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getFontSize(mFontSize);
    mTBoxWidth = mpTm_c[0]->getSizeX();
    mTBoxHeight = mpTm_c[0]->getSizeY();
    mLineSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getLineSpace();
    mCharSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getCharSpace();
    for (int i = 0; i < 3; i++) {
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setLineSpace(mLineSpace);
        mpTm_c[i]->resize(1.2f * mpTm_c[i]->getSizeX(), mpTm_c[i]->getSizeY());
    }
    mTextBoxPosX =
        mpTm_c[0]->getGlobalPosX() +
        (0.5f * ((1.0f - g_MsgObject_HIO_c.mBoxItemTextSizeX) * mpTm_c[0]->getInitSizeX()));
    mTextBoxPosY =
        mpTm_c[0]->getGlobalPosY() +
        (0.5f * ((1.0f - g_MsgObject_HIO_c.mBoxItemTextSizeY) * mpTm_c[0]->getInitSizeY()));
    mTextBoxScaleX = g_MsgObject_HIO_c.mBoxItemTextSizeX;
    mTextBoxScaleY = g_MsgObject_HIO_c.mBoxItemTextSizeY;
    for (int i = 0; i < 3; i++) {
        if (mpTmr_c[i] != NULL) {
            ((J2DTextBox*)mpTmr_c[i]->getPanePtr())->setLineSpace(mLineSpace);
            if (i == 0) {
                J2DTextBox::TFontSize fontSize;
                ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getFontSize(fontSize);
                mRubySize = fontSize.mSizeX;
                mRubyCharSpace = ((J2DTextBox*)mpTmr_c[0]->getPanePtr())->getCharSpace();
            }
            mpTmr_c[i]->resize(1.2f * mpTmr_c[i]->getSizeX(), mpTmr_c[i]->getSizeY());
        }
    }
    mSelTBoxWidth = mpSelect_c->getTextBoxWidth();
    mSelFontSize = mpSelect_c->getFontSize();
    mSelRubySize = mpSelect_c->getRubyFontSize();
    mSelCharSpace = mpSelect_c->getCharSpace();
    mSelRubyCharSpace = mpSelect_c->getRubyCharSpace();
    for (int i = 0; i < 3; i++) {
        mSelTextBoxPosX[i] = mpSelect_c->getTextBoxGlobalPosX(i);
        mSelTextBoxPosY[i] = mpSelect_c->getTextBoxGlobalPosY(i);
    }
    field_0x13c = 0.0f;
}


dMsgScrnItem_c::~dMsgScrnItem_c() {
    for (int i = 0; i < 3; i++) {
        if (mpItemTex[i] != NULL) {
            field_0x138->free(mpItemTex[i]);
            mpItemTex[i] = NULL;
        }
        if (mpItemPane[i] != NULL) {
            delete mpItemPane[i];
            mpItemPane[i] = NULL;
        }
    }
    delete mpScreen;
    mpScreen = NULL;
    delete mpTxScreen;
    mpTxScreen = NULL;
    delete mpArrow_c;
    mpArrow_c = NULL;
    delete mpSelect_c;
    mpSelect_c = NULL;
    delete mpLight_c;
    mpLight_c = NULL;
    if (mCharInfoPtr != 0) {
        field_0x138->free(mCharInfoPtr);
        mCharInfoPtr = NULL;
    }
    delete mpArw_c;
    mpArw_c = NULL;
    for (int i = 0; i < 2; i++) {
        delete mpMg_c[i];
        mpMg_c[i] = NULL;
    }
    for (int i = 0; i < 1; i++) {
        delete field_0x118[i];
        field_0x118[i] = NULL;
    }
    for (int i = 0; i < 2; i++) {
        delete field_0x12c[i];
        field_0x12c[i] = NULL;
    }
    delete mpPmP_c;
    mpPmP_c = NULL;
    for (int i = 0; i < 3; i++) {
        delete mpTm_c[i];
        mpTm_c[i] = NULL;
        if (mpTmr_c[i] != NULL) {
            delete mpTmr_c[i];
            mpTmr_c[i] = NULL;
        }
    }
    dComIfGp_getMsgArchive(0)->removeResourceAll();;
    dComIfGp_getMsgArchive(3)->removeResourceAll();;
    dComIfGp_getMsgCommonArchive()->removeResourceAll();;
}

void dMsgScrnItem_c::exec() {
    f32 dVar12 = field_0x13c;
    if (!field_0x19d) {
        setBtk0Animation(field_0x118[0]);
        setBpk0Animation(field_0x12c[0]);
        setBpk1Animation(field_0x12c[1]);
        field_0x19d = true;
    }
    for (int i = 0; i < 1; i++) {
        field_0x140[i] += g_MsgObject_HIO_c.mBoxItemAnmSpeed;
        if (field_0x140[i] >= field_0x118[i]->getFrameMax()) {
            field_0x140[i] -= field_0x118[i]->getFrameMax();
        }
        field_0x118[i]->setFrame(field_0x140[i]);
    }
    for (int i = 0; i < 2; i++) {
        field_0x154[i] += g_MsgObject_HIO_c.mBoxItemAnmSpeed;
        if (field_0x154[i] >= field_0x12c[i]->getFrameMax())
        {
            field_0x154[i] -= field_0x12c[i]->getFrameMax();
        }
        field_0x12c[i]->setFrame(field_0x154[i]);
    }
    mpScreen->animation();
    if (isTalkNow()) {
        fukiAlpha(1.0f);
    } else {
        fukiAlpha(dVar12);
    }
    
    f32 yOffset;
    switch(field_0x19c) {
    case 1:
        yOffset = field_0x188[2] - field_0x184;
        break;
    case 2:
        yOffset = field_0x188[1] - field_0x184;
        break;
    default:
        yOffset = field_0x188[0] - field_0x184;
        break;

    }
    mpPmP_c->scale(g_MsgObject_HIO_c.mBoxItemScaleX, g_MsgObject_HIO_c.mBoxItemScaleY);
    mpMg_c[1]->translate(field_0x168 + g_MsgObject_HIO_c.mBoxItemTextPosX,
                          yOffset + (field_0x16c + g_MsgObject_HIO_c.mBoxItemTextPosY));
    mpMg_c[1]->scale(g_MsgObject_HIO_c.mBoxItemTextSizeX, g_MsgObject_HIO_c.mBoxItemTextSizeY);
}

void dMsgScrnItem_c::drawSelf() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    grafContext->setup2D();
    s16* pLen = (short*)((u8*)mCharInfoPtr + 0x1068);
    if (*pLen > 0) {
        f32 dVar12 = mpTm_c[0]->getAlphaRate();
        for (int i = 0; i < *pLen; i = i + 1) {
            mpLight_c->draw(&mCharInfoPtr[i].field_0x14, mCharInfoPtr[i].field_0x00,
                            mCharInfoPtr[i].field_0x04, mCharInfoPtr[i].field_0x08,
                            mCharInfoPtr[i].field_0x0c,
                            dVar12 * g_MsgObject_HIO_c.mBoxItemHaloAlpha, mCharInfoPtr[i].mColorNo);
        }
    }

    f32 globalPosX = mpTm_c[0]->getGlobalPosX();
    
    #if WIDESCREEN_SUPPORT
    if (mDoGph_gInf_c::isWide()) {
        drawOutFont(g_MsgObject_HIO_c.mBoxItemTextPosX + 7.0f + YREG_F(2),
                    g_MsgObject_HIO_c.mBoxItemTextPosY, 1.0f);
    } else
    #endif
    {
        drawOutFont(g_MsgObject_HIO_c.mBoxItemTextPosX - 2.0f + YREG_F(2),
                    g_MsgObject_HIO_c.mBoxItemTextPosY, 1.0f);
    }

    mpTxScreen->draw(0.0f, 0.0f, grafContext);
    mpSelect_c->draw(0.0f, 0.0f);
    mpArrow_c->draw();

    for (int i = 0; i < 3; i++) {
        if (mpItemPane[i] != NULL) {
            mpItemPane[i]->setAlpha(255.0f * field_0x194);
            switch (mItemIndex) {
            case 0x20:
            case 0x2a:
            case 0x2b:
            case 0x21:
            case 0x37:
            case 0x38:
            case 0x39:
            case 0x3a:
            case 0x3d:
            case 0x40:
            case 0x42:
            case 0x44:
            case 0x47:
            case 0x48:
            case 0x4a:
            case 0x58:
            case 0x5b:
            case 0x5c:
            case 0x5d:
            case 0x5e:
            case 0x5f:
            case 0x9c:
            case 0xee:
            case 0xf3:
            case 0xf8:
            case 0xf9:
            case 0xfa:
            case 0xfb:
            case 0xfc:
            case 0xfd:
            case 0xfe:
                mpItemPane[i]->scale(0.6f, 0.6f);
                break;
            default:
                mpItemPane[i]->scale(g_MsgObject_HIO_c.mBoxItemSizeX, g_MsgObject_HIO_c.mBoxItemSizeY);
            }
            
            f32 texScale;
            if (g_drawHIO.mItemScaleAdjustON == true) {
                texScale = (g_drawHIO.mItemScalePercent / 100.0f);
            } else if (mItemIndex >= 0 && mItemIndex <= 0xff) {
                texScale = dItem_data::getTexScale(mItemIndex) / 100.0f;
            } else {
                texScale = 1.0f;
            };

            mpItemPane[i]->draw(mpScreen->search('set_it_n')->getGlbBounds().i.x +
                                    0.5f * (field_0x170 - (field_0x178 * texScale)) +
                                    g_MsgObject_HIO_c.mBoxItemPosX,
                                mpScreen->search('set_it_n')->getGlbBounds().i.y +
                                    0.5f * (field_0x174 - (field_0x17c * texScale)) +
                                    g_MsgObject_HIO_c.mBoxItemPosY,
                                field_0x178 * texScale, field_0x17c * texScale, field_0x19e, false,
                                false);
        }
    }
}

void dMsgScrnItem_c::arwAnimeInit() {
    mpArrow_c->arwAnimeInit();
}

void dMsgScrnItem_c::arwAnimeMove() {
    mpArrow_c->arwAnimeMove();
}

void dMsgScrnItem_c::dotAnimeInit() {
    mpArrow_c->dotAnimeInit();
}

void dMsgScrnItem_c::dotAnimeMove() {
    mpArrow_c->dotAnimeMove();
}

void dMsgScrnItem_c::setSelectString(char* param_0, char* param_1, char* param_2) {
    mpSelect_c->setString(param_0, param_1, param_2);
}

void dMsgScrnItem_c::setSelectRubyString(char* param_0, char* param_1, char* param_2) {
    mpSelect_c->setRubyString(param_0, param_1, param_2);
}

bool dMsgScrnItem_c::isSelect() {
    return mpSelect_c->isSelect();
}

void dMsgScrnItem_c::selectAnimeInit(u8 param_0, u8 param_1, f32 param_2, u8 param_3) {
    mpSelect_c->selAnimeInit(param_0, param_1, field_0x19c, param_2, param_3);
}

bool dMsgScrnItem_c::selectAnimeMove(u8 param_0, u8 param_1, bool param_2) {
    return mpSelect_c->selAnimeMove(param_0, param_1, param_2);
}

bool dMsgScrnItem_c::selectAnimeEnd() {
    return mpSelect_c->selAnimeEnd();
}

void dMsgScrnItem_c::fukiScale(f32 param_0) {
}

void dMsgScrnItem_c::fukiTrans(f32 x, f32 y) {
    mpPmP_c->paneTrans(x, y);
}

void dMsgScrnItem_c::fukiAlpha(f32 param_1) {
    field_0x13c = param_1;
    field_0x194 = param_1;
    mpPmP_c->setAlphaRate(param_1 * g_MsgObject_HIO_c.mBoxItemAlphaP);
    for (int i = 0; i < 3; i++) {
        mpTm_c[i]->setAlphaRate(param_1 * mCharAlphaRate);
        if (mpTmr_c[i] != NULL) {
            mpTmr_c[i]->setAlphaRate(param_1 * mCharAlphaRate);
        }
    }
}

void dMsgScrnItem_c::fukiPosCalc(u8 param_1) {
    msg_class* iVar6 = dMsgObject_c::getActor();
    if (mpPmP_c != NULL) {
        mpPmP_c->move(field_0x160, field_0x164);
    }
    if (param_1 == 3) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz local_70;
        cXyz cStack_7c;
        f32 f3;
        mDoLib_project(&player->eyePos, &cStack_7c);
        if (iVar6->pos == cXyz(0.0f, 0.0f, 0.0f)) {
            f3 = cStack_7c.y;
        } else {
            mDoLib_project(&iVar6->pos, &local_70);
            if (local_70.x >= 0.0f && local_70.x <= FB_WIDTH && local_70.y >= 0.0f &&
                local_70.y <= FB_HEIGHT)
            {
                f3 = 0.5f * (cStack_7c.y + local_70.y);
            } else {
                f3 = cStack_7c.y;
            }
        }
        if (mpPmP_c != NULL) {
            switch(field_0x19c) {
            case 1:
                if (f3 < g_MsgObject_HIO_c.mBoxAppearBound - 50.0f) {
                    param_1 = 0;
                } else {
                    param_1 = 1;
                }
                break;
            case 3:
                if (f3 < g_MsgObject_HIO_c.mBoxAppearBound + 50.0f) {
                    param_1 = 0;
                } else {
                    param_1 = 1;
                }
                break;
            }
        } else {
            if (f3 < g_MsgObject_HIO_c.mBoxAppearBound) {
                param_1 = 0;
            } else {
                param_1 = 1;
            }
        }
    }
    field_0x180 = 0.0f;
    field_0x19c = param_1;
    f32 yOffset;
    switch(field_0x19c) {
    case 1:
        yOffset = g_MsgObject_HIO_c.mBoxPos[2][3];
        field_0x180 = 105.0f - (mpScreen->search('n_all')->getBounds().i.y +
                              mpScreen->search('n_all')->getBounds().f.y) / 2;
        mpSelect_c->translate(486.0f, 230.0f);
        break;
    case 2:
        yOffset = g_MsgObject_HIO_c.mBoxPos[1][3];
        field_0x180 = 235.0f - (mpScreen->search('n_all')->getBounds().i.y +
                                mpScreen->search('n_all')->getBounds().f.y) / 2;
        mpSelect_c->translate(486.0f, 355.0f);
        break;
    default:
        yOffset = g_MsgObject_HIO_c.mBoxPos[0][3];
        field_0x180 = 0.0f;
        mpSelect_c->translate(486.0f, 209.0f);
    }
    field_0x184 = yOffset;
    setTextBoxPosOffsetY(field_0x180 + yOffset);
    mpScreen->search('n_all')->move(field_0x160, yOffset + (field_0x164 + field_0x180));
    if (mpPmP_c != NULL) {
        mpPmP_c->reinit();
        mpArrow_c->setPos(mpArw_c->getGlobalPosX() + (0.5f * mpArw_c->getSizeX()),
                          mpArw_c->getGlobalPosY() + (0.5f * mpArw_c->getSizeY()));
        mpMg_c[1]->translate(mpMg_c[0]->getGlobalPosX() + 0.5f * mpMg_c[0]->getSizeX(),
                             mpMg_c[0]->getGlobalPosY() + 0.5f * mpMg_c[0]->getSizeY());
        mTextBoxPosX = mpTm_c[0]->getGlobalPosX();
        mTextBoxPosY = mpTm_c[0]->getGlobalPosY();
        field_0x168 = mpMg_c[0]->getGlobalPosX() + 0.5f * mpMg_c[0]->getSizeX();
        field_0x16c = (mpMg_c[0]->getGlobalPosY() + 0.5f * mpMg_c[0]->getSizeY());
    } else {
        Vec arwCenter = mpArw_c->getGlobalVtxCenter(false, 0);
        mpArrow_c->setPos(arwCenter.x, arwCenter.y);
        Vec local_b8 = mpMg_c[0]->getGlobalVtxCenter(false, 0);
        field_0x168 = local_b8.x;
        field_0x16c = local_b8.y;
        mpMg_c[1]->translate(field_0x168, field_0x16c);
    }
}

void dMsgScrnItem_c::setBtk0Animation(J2DAnmTextureSRTKey* param_1) {
    mpScreen->search('gold00')->setAnimation(param_1);
    mpScreen->search('gold01')->setAnimation(param_1);
}

void dMsgScrnItem_c::setBpk0Animation(J2DAnmColor* param_0) {
    mpScreen->search('map0')->setAnimation(param_0);
}

void dMsgScrnItem_c::setBpk1Animation(J2DAnmColor* param_0) {
    mpScreen->search('moyou_1')->setAnimation(param_0);
}

bool dMsgScrnItem_c::isOugiID() {
    switch(dMsgObject_getMessageID()) {
    case 0x17ed:
    case 0x1823:
    case 0x183f:
    case 0x18b4:
    case 0x185c:
    case 0x1877:
    case 0x1895:
    case 0x3efd:
    case 0x3f33:
    case 0x3f6c:
    case 0x3f87:
    case 0x3fa5:
    case 0x3fc4:
        return true;
    }
    return false;
}
