/**
 * d_msg_scrn_place.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_msg_scrn_place.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "d/d_camera.h"
#include "d/d_msg_object.h"
#include "d/d_pane_class.h"

dMsgScrnPlace_c::dMsgScrnPlace_c() {
    static u64 t_tag[7] = {
        'sfontb0', 'sfontb1', 'sfontb2', 'sfontl0', 'sfontl1', 'sfontl2', 'sfont00',
    };
    init();

    dCamera_c* camera = dCam_getBody();
    if (camera->mTrimSize && dCam_getBody()->mTrimSize != 4) {
        mScaleX = g_MsgObject_HIO_c.mStageTitleCharPosY;
        mScaleY = g_MsgObject_HIO_c.mStageTitleBasePosY;
    } else {
        mScaleY = 0.0f;
        mScaleX = 0.0f;
    }

    mpScreen = new J2DScreen();
#if VERSION == VERSION_GCN_JPN
    mpScreen->setPriority("zelda_stage_title.blo", 0x20000, dComIfGp_getMsgArchive(4));
#else
    mpScreen->setPriority("zelda_stage_title_foreign.blo", 0x20000, dComIfGp_getMsgArchive(4));
#endif
    dPaneClass_showNullPane(mpScreen);

    mpPmP_c = new CPaneMgr(mpScreen, 'n_all', 2, NULL);
    mpPmP_c->scale(g_MsgObject_HIO_c.mStageTitleScaleX, g_MsgObject_HIO_c.mStageTitleScaleY);

    mpFontParent = new CPaneMgr(mpScreen, 's_font_n', 0, NULL);
    mpFontParent->scale(g_MsgObject_HIO_c.mStageTitleCharSizeX,
                        g_MsgObject_HIO_c.mStageTitleCharSizeY);
    mpFontParent->paneTrans(g_MsgObject_HIO_c.mStageTitleCharPosX,
                            g_MsgObject_HIO_c.mStageTitleCharPosY - mScaleX);

    mpBaseParent = new CPaneMgr(mpScreen, 'base_n', 2, NULL);
    mpBaseParent->scale(g_MsgObject_HIO_c.mStageTitleBaseSizeX,
                        g_MsgObject_HIO_c.mStageTitleBaseSizeY);
    mpBaseParent->paneTrans(g_MsgObject_HIO_c.mStageTitleBasePosX,
                            g_MsgObject_HIO_c.mStageTitleBasePosY - mScaleY);

    for (int i = 0; i < 7; i++) {
        mpTm_c[i] = new CPaneMgr(mpScreen, t_tag[i], 0, NULL);
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setFont(mDoExt_getRubyFont());
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setString(0x80, "");
#if VERSION != VERSION_GCN_JPN
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setCharSpace(1.0f);
#endif
    }

    ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getFontSize(mFontSize);
    mTBoxWidth = mpTm_c[0]->getSizeX();
    mTBoxHeight = mpTm_c[0]->getSizeY();
    mLineSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getLineSpace();
    mCharSpace = ((J2DTextBox*)mpTm_c[0]->getPanePtr())->getCharSpace();

    for (int i = 0; i < 7; i++) {
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())->setLineSpace(mLineSpace);
        ((J2DTextBox*)mpTm_c[i]->getPanePtr())
            ->resize(mpTm_c[i]->getSizeX() * 1.2f, mpTm_c[i]->getSizeY());
    }

    mTextBoxPosX = mpTm_c[0]->getGlobalPosX();
    mTextBoxPosY = mpTm_c[0]->getGlobalPosY();
}

dMsgScrnPlace_c::~dMsgScrnPlace_c() {
    delete mpScreen;
    mpScreen = NULL;

    delete mpPmP_c;
    mpPmP_c = NULL;

    delete mpFontParent;
    mpFontParent = NULL;

    delete mpBaseParent;
    mpBaseParent = NULL;

    for (int i = 0; i < 7; i++) {
        delete mpTm_c[i];
        mpTm_c[i] = NULL;
    }

    dComIfGp_getMsgArchive(4)->removeResourceAll();
}

void dMsgScrnPlace_c::exec() {
    mpPmP_c->scale(g_MsgObject_HIO_c.mStageTitleScaleX, g_MsgObject_HIO_c.mStageTitleScaleY);

    mpFontParent->scale(g_MsgObject_HIO_c.mStageTitleCharSizeX,
                        g_MsgObject_HIO_c.mStageTitleCharSizeY);
    mpFontParent->paneTrans(g_MsgObject_HIO_c.mStageTitleCharPosX,
                            g_MsgObject_HIO_c.mStageTitleCharPosY - mScaleX);

    mpBaseParent->scale(g_MsgObject_HIO_c.mStageTitleBaseSizeX,
                        g_MsgObject_HIO_c.mStageTitleBaseSizeY);
    mpBaseParent->paneTrans(g_MsgObject_HIO_c.mStageTitleBasePosX,
                            g_MsgObject_HIO_c.mStageTitleBasePosY - mScaleY);

    if (isTalkNow()) {
        fukiAlpha(1.0f);
    }
}

void dMsgScrnPlace_c::drawSelf() {
    J2DGrafContext* grafContext = dComIfGp_getCurrentGrafPort();
    grafContext->setup2D();
    drawOutFont(0.0f, 0.0f, 1.0f);
}

void dMsgScrnPlace_c::fukiAlpha(f32 i_rate) {
    mpPmP_c->setAlphaRate(i_rate);
    mpBaseParent->setAlphaRate(i_rate * g_MsgObject_HIO_c.mStageTitleBaseAlpha);

    for (int i = 0; i < 7; i++) {
        mpTm_c[i]->setAlphaRate(i_rate);
    }
}

void dMsgScrnPlace_c::fukiScale(f32) {}

void dMsgScrnPlace_c::fukiTrans(f32, f32) {}

void dMsgScrnPlace_c::fontAlpha(f32) {}
